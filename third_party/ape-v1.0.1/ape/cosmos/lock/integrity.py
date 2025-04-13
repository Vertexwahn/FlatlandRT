from __future__ import annotations

from base64 import standard_b64encode as base64
from dataclasses import dataclass
from typing import Collection, Literal


@dataclass(frozen=True)
class SRI:
    hash: Literal["sha256"]
    digest: Collection[bytes]

    def __str__(self) -> str:
        return f"{self.hash}-{base64(self.digest).decode('utf-8')}"
