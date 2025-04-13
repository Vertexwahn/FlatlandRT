from __future__ import annotations

from concurrent.futures import Executor, ThreadPoolExecutor, TimeoutError
from os import read
from re import Match, Pattern, compile
from select import select


def consume(fd: int, /) -> bytes:
    buffer = b""
    while r := read(fd, 1024):
        buffer += r
        if len(r) < 1024:
            break
    return buffer


def detect(fd: int, regex: Pattern[str], /, timeout: int) -> Match[str]:
    buffer = b""
    while True:
        ready, _, _ = select((fd,), (), (), timeout)
        if fd not in ready:
            raise TimeoutError(f"failed to select on {fd}")

        buffer += consume(fd)
        if not buffer:
            raise EOFError("no data to sniff")
        line, found, buffer = buffer.partition(b"\n")
        if not found:
            continue
        if match := regex.match(line.decode("utf8")):
            return match


def pooled(pool: Executor, fd: int, regex: Pattern[str], /, timeout: int) -> str:
    future = pool.submit(detect, fd, regex, timeout)
    return future.result(timeout=timeout)


def threaded(fd: int, regex: Pattern[str], /, threads: int, timeout: int) -> str:
    with ThreadPoolExecutor(max_workers=threads) as pool:
        return pooled(pool, fd, regex, timeout=timeout)


def sniff(fd: int, regex: Pattern[str] | str, /, timeout: int) -> str:
    if isinstance(regex, str):
        regex = compile(regex)
    return threaded(fd, regex, timeout=timeout, threads=1)
