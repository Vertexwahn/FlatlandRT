from __future__ import annotations

from pathlib import Path
from platform import uname

from .runfile import Runfile


class Tool:
    def __init__(self, runfile: Runfile):
        self.__runfile = runfile

    def __call__(self, name: str) -> Path:
        u = uname()
        if u.system == "Windows":
            return self.__runfile(f"ape/ape/assimilate/{name}/{name}.exe")
        if u.system == "Darwin" and u.machine in {"aarch64", "arm64"}:
            return self.__runfile(f"ape/ape/assimilate/{name}/{name}.ape")
        return self.__runfile(f"ape/ape/assimilate/{name}/{name}")
