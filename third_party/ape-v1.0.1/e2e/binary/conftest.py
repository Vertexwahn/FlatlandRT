from __future__ import annotations

from os import environ

from pytest import fixture

from .difference import repr as diff
from .relative import Relative
from .runfile import (
    Env,
    EnvVars,
    Runfile,
    Runfiles,
    SupportsEnvVars,
    SupportsRlocation,
    create,
)
from .tool import Tool

__all__ = ("Relative", "Tool", "Runfile", "Env")


@fixture
def runfiles() -> Runfiles:
    return create()


@fixture
def runfile(runfiles: SupportsRlocation) -> Runfile:
    return Runfile(runfiles)


@fixture
def env_vars(runfiles: SupportsEnvVars) -> EnvVars:
    return runfiles.EnvVars()


@fixture
def env(env_vars: EnvVars) -> Env:
    return environ | env_vars


@fixture
def tool(runfile: Runfile) -> Tool:
    return Tool(runfile)


@fixture
def relative(runfile: Runfile) -> Relative:
    return Relative(runfile)


def pytest_assertrepr_compare(op, left, right) -> list[str] | None:
    if value := diff(op, left, right):
        return value
