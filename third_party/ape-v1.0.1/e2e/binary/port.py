from __future__ import annotations

from socket import AF_INET, SOCK_STREAM, socket


def listening(port: int, host: str = "localhost") -> bool:
    with socket(AF_INET, SOCK_STREAM) as s:
        return s.connect_ex((host, port)) == 0
