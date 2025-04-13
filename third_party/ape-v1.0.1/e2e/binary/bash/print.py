from __future__ import annotations

from subprocess import run

from binary.tool import Tool


def test_print(tool: Tool) -> None:
    binary = tool("bash")

    cmd = (binary, "-c", "echo 'Hello, world!'")
    result = run(cmd, check=True, timeout=30, capture_output=True, text=True)
    assert "Hello, world!\n" == result.stdout
