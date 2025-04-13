from __future__ import annotations

from difflib import unified_diff
from pathlib import Path
from typing import Any


class Diff:
    def __init__(self, path: Path = Path.cwd()):
        self.__path = path

    @property
    def path(self) -> Path:
        return self.__path

    def __eq__(self, value: Any) -> bool:
        if not isinstance(value, Diff):
            return False

        with open(self.path, encoding="utf-8") as src, open(
            value.path, encoding="utf-8"
        ) as dst:
            try:
                for a, b in zip(src, dst, strict=True):
                    if a != b:
                        return False
            except ValueError:
                return False
        return True


def relative(path: Path) -> Path:
    if Path.cwd() in path.parents:
        return path.relative_to(Path.cwd())
    return path


def repr(op: str, left: Any, right: Any) -> list[str] | None:
    if not isinstance(left, Diff):
        return None

    if not isinstance(right, Diff):
        return None

    with open(left.path, encoding="utf-8") as src, open(
        right.path, encoding="utf-8"
    ) as dst:
        iterator = unified_diff(
            a=src.readlines(),
            b=dst.readlines(),
            fromfile=str(relative(left.path)),
            tofile=str(relative(right.path)),
        )
        return ["Comparing Diff instances:"] + [n.rstrip() for n in iterator]
