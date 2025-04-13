from __future__ import annotations

from os import environ
from pathlib import Path
from subprocess import PIPE, Popen
from urllib.request import urlopen

from binary import Relative, Tool, sniff
from pytest import fail, mark


def test_serve(tool: Tool, relative: Relative, tmp_path: Path) -> None:
    binary = tool("redbean")

    cmd = (binary, "-p", "0", "-z")
    server = Popen(cmd, stdout=PIPE, encoding="utf8", cwd=tmp_path)
    match = sniff(
        server.stdout.fileno(),
        r"([0-9]{3,5})",
        timeout=30,
    )
    port = match.group(1)

    with urlopen(f"http://localhost:{port}") as stream:
        assert 200 <= stream.status < 300
        for line in stream:
            if b"redbean zip listing" in line:
                break
        else:
            fail("failed to find redbean title in returned HTML page")
