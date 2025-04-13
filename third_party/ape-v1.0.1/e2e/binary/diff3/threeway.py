from __future__ import annotations

from pathlib import Path
from subprocess import run

from binary import Relative, Tool


def test_threeway(tool: Tool, relative: Relative, tmp_path: Path) -> None:
    binary = tool("diff3")
    diff = tool("diff")
    a = relative("a.txt")
    b = relative("b.txt")
    c = relative("c.txt")
    expected = relative("expected.txt")
    output = tmp_path / "output.txt"

    cmd = (binary, "--diff-program", str(diff), str(a), str(b), str(c))
    with open(output, "w", encoding="utf-8") as stream:
        run(cmd, timeout=30, stdout=stream, check=True)

    with open(expected) as a, open(output) as b:
        assert a.read() == b.read()
