#!/usr/bin/env python3
"""Check LF whitespace invariants for maintained, tracked text files."""

from __future__ import annotations

import argparse
import logging
import re
import subprocess
from dataclasses import dataclass
from pathlib import Path


LOGGER = logging.getLogger(__name__)
EXCLUDED_PREFIXES = ("test/googletest-1.16.0/",)
TRAILING_WHITESPACE = re.compile(rb"[ \t]+(?=\r?\n|\r|$)")


@dataclass(frozen=True)
class Violation:
    path: str
    line: int | str
    message: str


def parse_arguments() -> argparse.Namespace:
    parser = argparse.ArgumentParser(
        description="Check whitespace in tracked maintained text files."
    )
    parser.add_argument(
        "--root",
        type=Path,
        help="repository root (defaults to the root containing this script)",
    )
    return parser.parse_args()


def repository_root(argument: Path | None) -> Path:
    if argument is not None:
        return argument.resolve()
    return Path(__file__).resolve().parents[1]


def tracked_paths(root: Path) -> tuple[str, ...]:
    result = subprocess.run(
        ("git", "-C", root, "ls-files", "-z"),
        check=True,
        stdout=subprocess.PIPE,
    )
    return tuple(
        path.decode("utf-8")
        for path in result.stdout.split(b"\0")
        if path
    )


def is_excluded(path: str) -> bool:
    return any(
        path == prefix.rstrip("/") or path.startswith(prefix)
        for prefix in EXCLUDED_PREFIXES
    )


def is_binary(content: bytes) -> bool:
    return b"\0" in content


def line_number(content: bytes, position: int) -> int:
    return content.count(b"\n", 0, position) + 1


def check_content(path: str, content: bytes) -> tuple[Violation, ...]:
    violations: list[Violation] = []

    for match in TRAILING_WHITESPACE.finditer(content):
        violations.append(
            Violation(
                path,
                line_number(content, match.start()),
                "trailing whitespace",
            )
        )

    if b"\r" in content:
        violations.append(
            Violation(
                path,
                "EOL",
                "carriage return found; use LF line endings",
            )
        )

    if content:
        if not content.endswith(b"\n"):
            violations.append(Violation(path, "EOF", "missing final newline"))
        elif content[:-1].endswith(b"\n"):
            violations.append(
                Violation(path, "EOF", "multiple final newlines")
            )

    return tuple(violations)


def check_file(root: Path, path: str) -> tuple[Violation, ...]:
    if is_excluded(path):
        return ()

    file_path = root / path
    if not file_path.is_file():
        return ()

    content = file_path.read_bytes()
    if is_binary(content):
        return ()
    return check_content(path, content)


def main() -> int:
    arguments = parse_arguments()
    root = repository_root(arguments.root)
    violations: list[Violation] = []

    try:
        paths = tracked_paths(root)
    except (OSError, subprocess.CalledProcessError) as error:
        LOGGER.error("unable to list tracked files in %s: %s", root, error)
        return 2

    for path in paths:
        violations.extend(check_file(root, path))

    for violation in violations:
        LOGGER.error(
            "%s:%s: %s",
            violation.path,
            violation.line,
            violation.message,
        )

    if violations:
        LOGGER.error("%d whitespace violation(s) found", len(violations))
        return 1
    LOGGER.info("Whitespace check passed")
    return 0


if __name__ == "__main__":
    logging.basicConfig(format="%(message)s", level=logging.INFO)
    raise SystemExit(main())
