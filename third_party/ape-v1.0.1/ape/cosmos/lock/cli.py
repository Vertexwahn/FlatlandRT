from __future__ import annotations

import json
from argparse import ArgumentParser, ArgumentTypeError
from concurrent.futures import ThreadPoolExecutor
from multiprocessing import cpu_count
from os import environ
from pathlib import Path, PurePath
from shlex import split
from shutil import copyfileobj
from sys import argv, exit, stdin, stdout
from typing import NoReturn, Protocol, TypeAlias, TypeVar

from python.runfiles import Runfiles

from .download import URL
from .download import pooled as binaries


class RunfileNotFoundError(FileNotFoundError):
    pass


def runfile(path: PurePath) -> Path:
    runfiles = Runfiles.Create()
    assert runfiles, "No runfile directory found."
    resolved = Path(runfiles.Rlocation(path))
    if not resolved.exists():
        raise RunfileNotFoundError(path)
    return resolved


def resolve(value: str) -> Path:
    path = Path(value)
    if path.exists():
        return path

    if bwd := environ.get("BUILD_WORKING_DIRECTORY"):
        resolved = Path(bwd, path)
        if resolved.exists():
            return resolved

    try:
        return runfile(str(path))
    except RunfileNotFoundError as e:
        raise ArgumentTypeError("runfile not found: {e}") from e


class BazelArgumentParser(ArgumentParser):
    def convert_arg_line_to_args(self, line):
        return split(line)


def parser(exe: Path) -> ArgumentParser:
    return BazelArgumentParser(
        prog=str(exe),
        description="Creates `cosmos` lock data",
        fromfile_prefix_chars="@",
    )


class SupportsStr(Protocol):
    def __str__(self) -> str: ...


AnyStrCovariant: TypeAlias = TypeVar("AnyStrCovariant", str, bytes, covariant=True)


class SupportsRead(Protocol[AnyStrCovariant]):
    def read(self, size: int = -1, /) -> AnyStrCovariant: ...


AnyStrContravariant: TypeAlias = TypeVar(
    "AnyStrContravariant", str, bytes, contravariant=True
)


class SupportsWriteFlush(Protocol[AnyStrContravariant]):
    def write(self, data: AnyStrContravariant, /) -> int: ...

    def flush(self) -> None: ...


class Data(Protocol):
    @property
    def binary(self) -> SupportsStr: ...

    @property
    def sri(self) -> SupportsStr: ...


class Dump(Protocol):
    def data(self, data: Data, output: SupportsWriteFlush[str]) -> None: ...

    def end(self, output: SupportsWriteFlush[str]) -> int: ...


class Print(Dump):
    def data(self, data: Data, output: SupportsWriteFlush[str]) -> None:
        print(data, file=output)

    def end(self, output: SupportsWriteFlush[str]) -> int:
        return 0


class JsonSeq(Dump):
    def data(self, data: Data, output: SupportsWriteFlush[str]) -> None:
        output.write("\x1e")
        json.dump({"name": str(data.name), "sri": str(data.sri)}, output)
        output.write("\x0a")
        output.flush()

    def end(self, output: SupportsWriteFlush[str]) -> int:
        return 0


class Json(Dump):
    def __init__(self) -> None:
        self.__data = {}

    def data(self, data: Data, output: SupportsWriteFlush[str]) -> None:
        self.__data[str(data.name)] = str(data.sri)

    def end(self, output: SupportsWriteFlush[str]) -> int:
        json.dump(self.__data, output, indent=2, sort_keys=True)
        return 0


def lock(
    version,
    url="https://cosmo.zip/pub/cosmos/v/{version}/bin/{name}",
    jobs: int = cpu_count(),
    dump: Dump = Print(),
    output: SupportsWriteFlush[str] = stdout,
    input: SupportsRead[str] | None = None,
) -> int:
    if input is not None:
        copyfileobj(input, output)
        return 0

    with ThreadPoolExecutor(max_workers=jobs) as pool:
        for binary in binaries(pool, version, url=url):
            if binary.name.endswith(".c"):
                continue
            if not binary.name[0].isalpha():
                continue
            dump.data(binary, output)
    return dump.end(output)


def input(value: str) -> SupportsRead[str]:
    if value == "-":
        return stdin

    return open(resolve(value))


def output(value: str) -> SupportsWriteFlush[str]:
    if value == "-":
        return stdout

    path = PurePath(value)
    if path.is_absolute():
        pass
    elif bwd := environ.get("BUILD_WORKING_DIRECTORY"):
        path = Path(bwd, path)

    return open(path, "w", encoding="utf-8")


def arguments(prsr: ArgumentParser) -> None:
    prsr.add_argument(
        "--url",
        metavar="TEMPLATE",
        help="The URL template to use for scraping.",
        default=URL,
    )
    prsr.add_argument(
        "--input",
        help="An input file to be copied to the output file..",
        type=input,
    )
    prsr.add_argument(
        "--output",
        help="Output location for the lock data.",
        default=stdout,
        type=output,
    )
    prsr.add_argument(
        "--jobs",
        type=int,
        default=cpu_count(),
        help="Number of downloads to run in parallel.",
    )
    prsr.add_argument(
        "--json",
        help="Output the binaries as `application/json`",
        action="store_const",
        const=Json(),
        dest="dump",
    )
    prsr.add_argument(
        "--json-seq",
        help="Output the binaries as `application/json-seq`",
        action="store_const",
        const=JsonSeq(),
        dest="dump",
    )
    prsr.add_argument(
        "--version",
        required=True,
        help="The version of `cosmos` to lock.",
    )
    prsr.set_defaults(func=lock, dump=JsonSeq())


def main(exe: Path, *args: str) -> int:
    prsr = parser(exe)
    arguments(prsr)

    try:
        baked = runfile("baked.args")
    except RunfileNotFoundError:
        pass
    else:
        args = (f"@{baked}", *args)

    parsed = prsr.parse_args(args)

    data = vars(parsed)
    func = data.pop("func")
    assert callable(func)

    try:
        result = func(**data)
        assert isinstance(result, int)
        return result
    except KeyboardInterrupt:
        return 130


def entry() -> NoReturn:
    exit(main(Path(argv[0]), *argv[1:]))
