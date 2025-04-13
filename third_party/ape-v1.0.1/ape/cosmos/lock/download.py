from __future__ import annotations

from collections.abc import Container
from concurrent.futures import Executor, ThreadPoolExecutor, as_completed
from dataclasses import asdict, dataclass
from hashlib import new as Hash
from html.parser import HTMLParser
from logging import getLogger
from multiprocessing import cpu_count
from typing import Final, Iterable, Iterator, Literal
from urllib.parse import quote, unquote
from urllib.request import urlopen

from .integrity import SRI

LOG: Final = getLogger(__name__)
URL: Final = "https://cosmo.zip/pub/cosmos/v/{version}/bin/{name}"


@dataclass(frozen=True)
class Scraped:
    name: str
    template: str
    version: str

    @property
    def basename(self) -> str:
        return self.binary.removesuffix(".ape")

    @property
    def url(self) -> str:
        return self.template.format(name=quote(self.name), version=self.version)


class CosmosHTMLParser(HTMLParser):
    def __init__(self, exclude=frozenset(("../",))):
        super().__init__()
        self.__exclude: Final = exclude
        self.__found = []

    @property
    def exclude(self) -> Container[str]:
        return self.__exclude

    def found(self) -> Iterable[str]:
        found = self.__found
        self.__found = []
        return found

    def handle_starttag(self, tag, attrs):
        if tag == "a":
            if found := next(v for k, v in attrs if k == "href"):
                if found not in self.exclude:
                    self.__found.append(unquote(found))


def scrape(version: str, url: str = URL) -> Iterator[Scraped]:
    # TODO: use integrity file if available
    # https://github.com/jart/cosmopolitan/issues/1360
    with urlopen(url.format(version=version, name="")) as stream:
        parser = CosmosHTMLParser()
        while buffer := stream.read(1024):
            parser.feed(buffer.decode("utf-8"))
        for name in parser.found():
            yield Scraped(name=name, template=url, version=version)


def integrity(url: str, hash: Literal["sha256"] = "sha256") -> SRI:
    LOG.debug("calculating integrity: %s", url)
    with urlopen(url) as stream:
        hasher = Hash(hash)
        while buffer := stream.read(1024):
            hasher.update(buffer)
    return SRI(hash="sha256", digest=hasher.digest())


@dataclass(frozen=True)
class Binary(Scraped):
    sri: SRI


def serial(
    version: str,
    url: str = URL,
) -> Iterator[Binary]:
    for scraped in scrape(version, url):
        sri = integrity(scraped.url)
        yield Binary(sri=sri, **asdict(scraped))


def pooled(
    pool: Executor,
    version: str,
    url: str = URL,
) -> Iterator[Binary]:
    futures = {
        pool.submit(integrity, scraped.url): scraped for scraped in scrape(version, url)
    }
    for future in as_completed(futures):
        sri = future.result()
        scraped = futures[future]
        yield Binary(sri=sri, **asdict(scraped))


def threaded(
    version: str,
    url: str = URL,
    threads: int = cpu_count(),
) -> Iterator[Binary]:
    with ThreadPoolExecutor(max_workers=threads) as pool:
        yield from pooled(pool, version, url)
