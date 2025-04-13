visibility("//...")

DOC = """Describes a set of `cosmos` files to download.

The `ape_cosmos_lock` executable rule is used to scrapped binary names and integrities into a lockfile:

```py
# BUILD.bazel
load("@ape//ape/cosmos/lock:defs.bzl", "ape_cosmos_lock")

# `bazel run` any of these targets to generate the lockfile

ape_cosmos_lock(
    name = "4.0.2",
    out = "4.0.2.json",
    tags = ["manual"],
    version = "4.0.2",
)
```

The lockfile is then used in the extension to create download repositories for each binary:

```py
cosmos = use_extension("@ape//ape/cosmos:defs.bzl", "ape_cosmos")

# Provide a download URL for the binaries (this it the default)
cosmos.url(
    template = "https://cosmo.zip/pub/cosmos/v/{version}/bin/{name}",
)

# Ingest the lockfile to expose the binaries as repositories
cosmos.download(
    version = "4.0.2",
    lock = "//:4.0.2.json",
)
```

Running `bazel mod tidy` will automatically update the `use_repo` for the extension.
"""

ATTRS = {
    "version": attr.string(
        doc = "Release version of `cosmos` binaries to download.",
        mandatory = True,
    ),
    "urls": attr.string_list(
        doc = "A list of URLs that can be templated with `{version}`/`{name}` to provide the location of a `cosmos` binary.",
        default = ["https://cosmo.zip/pub/cosmos/v/{version}/bin/{name}"],
        allow_empty = False,
    ),
    "lock": attr.label(
        doc = "A JSON lockfile that has binary names as the keys and sub-resource integrity (SRI) values for the values. The value can be set to `null` to allow uncached downloads. Integrities can be later upgraded by running the `:update` target under the generated repository.",
        allow_single_file = [".json"],
        mandatory = True,
    ),
}

ape_cosmos_download = tag_class(
    doc = DOC,
    attrs = ATTRS,
)

download = ape_cosmos_download
