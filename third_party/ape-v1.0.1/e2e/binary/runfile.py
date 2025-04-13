from __future__ import annotations

from collections.abc import Mapping
from pathlib import Path, PurePath
from typing import Protocol, TypeAlias, runtime_checkable

from python.runfiles import Runfiles as runfiles

EnvVars: TypeAlias = Mapping[str, str]
Env: TypeAlias = EnvVars


@runtime_checkable
class SupportsRlocation(Protocol):
    def Rlocation(self, path: str) -> str | None: ...


@runtime_checkable
class SupportsEnvVars(Protocol):
    def EnvVars(self) -> EnvVars: ...


@runtime_checkable
class Runfiles(SupportsRlocation, SupportsEnvVars, Protocol):
    pass


class RunfilesError(RuntimeError):
    pass


def create() -> Runfiles:
    created = runfiles.Create()
    if not isinstance(created, Runfiles):
        raise RunfilesError("failed to create runfiles object")
    return created


class RunfileNotFoundError(FileNotFoundError):
    pass


class Runfile:
    def __init__(self, runfiles: SupportsRlocation):
        self.__runfiles = runfiles

    def __call__(self, value: PurePath | str) -> Path:
        path = Path(str(value))
        if path.exists():
            return path

        resolved = self.__runfiles.Rlocation(str(value))
        if resolved is None:
            raise RunfileNotFoundError(value)

        path = Path(resolved)
        if not path.exists():
            raise RunfileNotFoundError(value)

        try:
            return path.relative_to(Path.cwd())
        except ValueError:
            return path
