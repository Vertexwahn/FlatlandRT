load(":repository.bzl", _entrypoint = "entrypoint")

visibility("public")

ape_entrypoint = _entrypoint
