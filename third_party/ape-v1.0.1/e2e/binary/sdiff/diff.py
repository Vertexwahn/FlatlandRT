from __future__ import annotations

from pathlib import Path
from subprocess import run

from binary import Relative, Tool


def test_diff(tool: Tool, relative: Relative, tmp_path: Path) -> None:
    binary = tool("sdiff")
    diff = tool("diff")
    a = relative("a.txt")
    b = relative("b.txt")
    expected = relative("expected.txt")
    output = tmp_path / "output.txt"

    cmd = (binary, f"--diff-program={diff}", "--width=80", str(a), str(b))
    with open(output, "w", encoding="utf-8") as stream:
        result = run(cmd, timeout=30, stdout=stream)
        assert 1 == result.returncode

    with open(expected) as a, open(output) as b:
        assert a.read() == b.read()
