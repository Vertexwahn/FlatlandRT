from __future__ import annotations

from inspect import getmodule, stack
from pathlib import Path, PurePath

from .runfile import Runfile


class Relative:
    def __init__(self, runfile: Runfile):
        self.__runfile = runfile

    def __call__(self, pure: PurePath, /) -> Path:
        caller = stack()[1]
        module = getmodule(caller[0])
        name = module.__name__
        path = PurePath(*name.split(".")[:-1])
        return self.__runfile(f"_main/{path / pure}")
