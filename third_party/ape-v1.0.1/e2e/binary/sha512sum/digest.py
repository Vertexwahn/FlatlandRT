from __future__ import annotations

from pathlib import Path
from subprocess import run

from binary import Diff, Relative, Tool


def test_digest(tool: Tool, relative: Relative, tmp_path: Path) -> None:
    binary = tool("sha512sum")
    fixture = relative("fixture.txt")
    expected = relative("expected.txt")
    output = tmp_path / "output.txt"

    cmd = (binary, fixture)
    with open(output, "w") as stream:
        run(cmd, check=True, timeout=30, stdout=stream)
    assert Diff(expected) == Diff(output)
