from __future__ import annotations

from pathlib import Path
from subprocess import run

from binary import Diff, Relative, Tool


def test_digest(tool: Tool, relative: Relative, tmp_path: Path) -> None:
    binary = tool("base64")
    fixture = relative("fixture.txt")
    expected = relative("expected.txt")
    output = tmp_path / "output.txt"

    cmd = (binary,)
    with open(fixture) as src, open(output, "w") as dst:
        run(cmd, check=True, timeout=30, stdin=src, stdout=dst)
    assert Diff(expected) == Diff(output)
