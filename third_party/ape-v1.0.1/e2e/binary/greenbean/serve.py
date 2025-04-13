from __future__ import annotations

from pathlib import Path
from shutil import copyfileobj
from subprocess import PIPE, Popen
from urllib.request import urlopen

from binary import Diff, Relative, Tool, sniff
from binary.port import listening
from pytest import mark


@mark.skipif(listening(8080), reason="port 8080 is in use")
def test_serve(tool: Tool, relative: Relative, tmp_path: Path) -> None:
    binary = tool("greenbean")
    expected = relative("expected.html")
    output = tmp_path / "output.html"

    cmd = (binary, "1")
    server = Popen(cmd, stderr=PIPE, encoding="utf8", cwd=tmp_path)
    match = sniff(server.stderr.fileno(), r"listening on (http://.+)", timeout=2)
    host = match.group(1)

    with urlopen(host) as src, open(output, "w+b") as dst:
        assert 200 <= src.status < 300
        copyfileobj(src, dst)

    assert Diff(expected) == Diff(output)
