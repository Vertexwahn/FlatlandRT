visibility("//...")

DOC = """Determines which `cosmos` repositories to use via `bazel mod tidy`.

Not often used but can help with importing _all_ generated repositories, if needed.

```py
cosmos = use_extension("@ape//ape/cosmos:defs.bzl", "ape_cosmos")

cosmos.use(
    unversioned = True,
    bootstrap = True,
    metadata = True,
    upload = True,
    aliases = True,
    versions = ["4.0.2"],
)
```
"""

ATTRS = {
    "unversioned": attr.bool(
        doc = "Import the unversioned convenience repositories.",
    ),
    "bootstrap": attr.bool(
        doc = "Import the bootstrap repositories such as the APE launcher and APE assimilator.",
    ),
    "metadata": attr.bool(
        doc = "Import the metadata repository.",
    ),
    "upload": attr.bool(
        doc = "Import the upload repository.",
    ),
    "aliases": attr.bool(
        doc = "Import the alias repository.",
    ),
    "versions": attr.string_list(
        doc = "Imports all repositories for a specific version.",
    ),
}

USE = struct(
    unversioned = False,
    bootstrap = False,
    metadata = False,
    upload = False,
    aliases = False,
    versions = (),
)

ape_cosmos_use = tag_class(
    doc = DOC,
    attrs = ATTRS,
)

use = ape_cosmos_use
