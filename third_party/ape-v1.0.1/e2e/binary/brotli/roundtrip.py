from __future__ import annotations

from pathlib import Path
from shutil import copyfileobj
from subprocess import PIPE, Popen

from binary import Diff, Relative, Tool


def test_roundtrip(tool: Tool, relative: Relative, tmp_path: Path) -> None:
    codec = tool("brotli")
    fixture = relative("fixture.txt")
    output = tmp_path / "output.txt"

    # Do roundtrip compress/decompress
    with open(fixture) as src, open(output, "w+b") as dst:
        compress = Popen((codec, "-c"), stdin=src, stdout=PIPE)
        decompress = Popen((codec, "-dc"), stdin=compress.stdout, stdout=PIPE)
        copyfileobj(decompress.stdout, dst)
        assert 0 == decompress.wait(timeout=30)
        dst.flush()

    # Compare the files
    assert Diff(fixture) == Diff(output)
