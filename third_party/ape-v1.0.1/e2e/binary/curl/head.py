from __future__ import annotations

from pathlib import Path
from subprocess import run
from typing import Final
from urllib.error import HTTPError
from urllib.request import Request, urlopen

from binary import Tool
from pytest import mark

URL: Final = "https://gitlab.arm.com/api/v4/projects"


def ping(url: str) -> bool:
    request = Request(url, method="HEAD")

    # FIXME: remove this when the CI image works with Python SSL verification
    import ssl

    context = ssl._create_unverified_context()

    try:
        with urlopen(request, context=context) as stream:
            if "Content-Length" not in stream.headers:
                return False
            return 200 <= stream.status < 300
    except HTTPError:
        return False


@mark.skipif(not ping(URL), reason="No network availability")
def test_curl(tool: Tool, tmp_path: Path) -> None:
    binary = tool("curl")

    cmd = (binary, "-sfSLX", "HEAD", URL)
    result = run(cmd, timeout=30, capture_output=True, text=True)

    assert 0 == result.returncode, result.stderr
