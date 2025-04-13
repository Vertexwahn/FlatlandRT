from __future__ import annotations

from pathlib import Path
from subprocess import run

from binary import Diff, Relative, Tool


def test_copy(tool: Tool, relative: Relative, tmp_path: Path) -> None:
    binary = tool("cp")
    fixture = relative("fixture.txt")
    expected = relative("expected.txt")
    output = tmp_path / "output.txt"

    cmd = (binary, fixture, output)
    run(cmd, check=True, timeout=30)
    assert Diff(expected) == Diff(output)
