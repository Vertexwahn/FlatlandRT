visibility("//...")

DOC = """Overrides a downloaded `cosmos` binary version.

By default, the extension will create repositories to the latest version of each binary.

In certain cases, newer binaries may have bugs in them.

The `override` tag allows pinning a unversioned repository to point at a specific version.

```py
cosmos.override(
    basename = "gzip",
    version = "3.3.1",
)
```
"""

ATTRS = {
    "basename": attr.string(
        doc = "Name of the APE binary to version override.",
        mandatory = True,
    ),
    "version": attr.string(
        doc = "Override version for the bianry rather than selecting the latest..",
        mandatory = True,
    ),
}

ape_cosmos_override = tag_class(
    doc = DOC,
    attrs = ATTRS,
)

override = ape_cosmos_override
