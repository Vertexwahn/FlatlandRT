load("@metadata//:mapped.bzl", _MAPPED = "MAPPED")

visibility("public")

BASENAMES = [basename for basename in _MAPPED if not (basename.startswith("ape-") or basename.startswith("assimilate"))]
