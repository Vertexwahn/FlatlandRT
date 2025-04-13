visibility("//...")

DOC = """The `upload` tag can provide a templated URL to provide upload targets for the downloaded binaries:

```py
cosmos = use_extension("@ape//ape/cosmos:defs.bzl", "ape_cosmos")
cosmos.upload(
    template = "https://some.mirror.com/{version}/{name}",
)
```

When `bazel run @upload` is performed, each binary will be uploaded with `@ape//ape:curl`.
"""

ATTRS = {
    "template": attr.string(
        doc = "A URL that can be templated with `{version}`/`{name}` to provide the location to upload a `cosmos` binary.",
    ),
}

ape_cosmos_upload = tag_class(
    doc = DOC,
    attrs = ATTRS,
)

upload = ape_cosmos_upload
