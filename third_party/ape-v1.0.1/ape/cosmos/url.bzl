visibility("//...")

DOC = """Provides a templated URL for use in `cosmos` binary downloads:

```py
cosmos = use_extension("@ape//ape/cosmos:defs.bzl", "ape_cosmos")

# Provide a download URL for the binaries (this it the default)
cosmos.url(
    template = "https://cosmo.zip/pub/cosmos/v/{version}/bin/{name}",
)
```
"""

ATTRS = {
    "template": attr.string(
        doc = "A URLs that can be templated with `{version}`/`{name}` to provide the location of a `cosmos` binary.",
    ),
}

ape_cosmos_url = tag_class(
    doc = DOC,
    attrs = ATTRS,
)

url = ape_cosmos_url
