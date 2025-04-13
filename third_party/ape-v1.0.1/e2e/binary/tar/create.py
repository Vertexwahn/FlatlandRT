from __future__ import annotations

from pathlib import Path
from subprocess import run
from tarfile import REGTYPE, TarFile

from binary import Relative, Tool


def test_codec(tool: Tool, relative: Relative, tmp_path: Path) -> None:
    binary = tool("tar")
    text = tmp_path / "fixture.txt"
    shell = tmp_path / "hello.sh"
    output = tmp_path / "output.tar"

    with open(text, "w", encoding="utf-8") as stream:
        stream.write("a" * 2048)
    text.chmod(0o644)

    with open(shell, "w", encoding="utf-8") as stream:
        stream.write("builtin echo 'Hello, world!'")
    shell.chmod(0o744)

    cmd = (
        binary,
        "--sort=name",
        "--mtime=2000-01-01T00:00Z",
        "--owner=0",
        "--group=0",
        "--numeric-owner",
        "--pax-option=exthdr.name=%d/PaxHeaders/%f,delete=atime,delete=ctime",
        "-cf",
        str(output),
        str(text.relative_to(tmp_path)),
        str(shell.relative_to(tmp_path)),
    )
    env = {
        "LC_ALL": "C",
    }
    result = run(cmd, timeout=30, env=env, capture_output=True, text=True, cwd=tmp_path)
    assert 0 == result.returncode, result.stderr

    with TarFile(str(output)) as archive:
        assert archive.getnames() == [
            str(text.relative_to(tmp_path)),
            str(shell.relative_to(tmp_path)),
        ]

        m = archive.getmember(str(text.relative_to(tmp_path)))
        assert m.size == 2048
        assert m.mode == 0o644, oct(m.mode)
        assert m.type == REGTYPE
        assert m.uid == 0
        assert m.gid == 0
        assert m.mtime == 946684800
        assert m.pax_headers == {}

        m = archive.getmember(str(shell.relative_to(tmp_path)))
        assert m.size == 28
        assert m.mode == 0o744, oct(m.mode)
        assert m.type == REGTYPE
        assert m.uid == 0
        assert m.gid == 0
        assert m.mtime == 946684800
        assert m.pax_headers == {}
