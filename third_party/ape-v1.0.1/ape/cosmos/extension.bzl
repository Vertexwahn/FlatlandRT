load("@bazel_skylib//lib:types.bzl", "types")
load("@download_utils//download/file:defs.bzl", "download_file")
load("//ape/entrypoint:defs.bzl", "ape_entrypoint")
load("//ape/cosmos/metadata:repository.bzl", "ape_cosmos_metadata")
load("//ape/cosmos/upload:repository.bzl", "ape_cosmos_upload")
load("//ape/cosmos/symlink:repository.bzl", "ape_cosmos_symlink")
load("//ape/cosmos/aliases:repository.bzl", "ape_cosmos_aliases")
load(":download.bzl", "download")
load(":override.bzl", "override")
load(":upload.bzl", "upload")
load(":use.bzl", "USE", "use")
load(":url.bzl", "url")

visibility("//...")

DOC = """An extension to download `cosmos.zip` pre-built binaries.

# Getting Started

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

# Overrides

By default, the extension will create repositories to the latest version of each binary.

In certain cases, newer binaries may have bugs in them.

The `override` tag allows pinning a unversioned repository to point at a specific version.

```py
cosmos.override(
    basename = "gzip",
    version = "3.3.1",
)
```

# Uploads

The `upload` tag can provide a templated URL to provide upload targets for the downloaded binaries:

```py
cosmos = use_extension("@ape//ape/cosmos:defs.bzl", "ape_cosmos")
cosmos.upload(
    template = "https://some.mirror.com/{version}/{name}",
)
```

When `bazel run @upload` is performed, each binary will be uploaded with `@ape//ape:curl`.
"""

TAGS = {
    "download": download,
    "override": override,
    "upload": upload,
    "use": use,
    "url": url,
}

def _bootstrap(name):
    # Need the raw launcher binaries to launch any APE binary
    if name.startswith("ape-"):
        return True

    # `assimilate` binaries are used to bootstrap assimilation
    if name.startswith("assimilate"):
        return True

    return False

def _invalid(name):
    # Repository cannot start with non-alphabetical characters such as `[`
    if not name[0].isalpha():
        return True

    return False

def _collect(mctx):
    upload = None
    root = None
    urls = []
    locks = {}
    overrides = {}
    use = USE

    # Gather all lockfile information across the module graph
    for mod in mctx.modules:
        if mod.is_root:
            for tag in mod.tags.use:
                use = tag

        seen = {}
        for tag in mod.tags.url:
            if tag.template in seen:
                continue
            seen[tag.template] = True
            urls.append(tag.template)

        for tag in mod.tags.upload:
            if upload != None:
                fail("Upload URL already set to `{}` cannot change to `{}`".format(upload, tag.template))
            upload = tag.template

        for tag in mod.tags.override:
            overrides[tag.basename] = tag.version

        for tag in mod.tags.download:
            src = json.decode(mctx.read(tag.lock))
            if not types.is_dict(src):
                fail("Lockfile must contain a dictionary of binary names and sub-resource integrities (SRI) or `null` values: {}".format(tag.lock))

            dst = locks.setdefault(tag.version, {})
            for name, integrity in src.items():
                if _invalid(name):
                    print("Skipping `cosmos` binary `{}-{}`".format(name, tag.version))
                    continue
                elif name not in dst:
                    dst[name] = integrity
                elif not integrity and dst[name]:
                    continue
                elif not dst[name]:
                    dst[name] = integrity
                elif dst[name] != integrity:
                    fail("Mismatching integrity for `{}`. Both `{}` and `{}` have been provided as integrities for the binary".format(name, dst[name], integrity))
                dst[name] = integrity

    # Order the versions in ascending order using natural version sorting
    ordered = [".".join([str(i) for i in v]) for v in sorted([[int(i) for i in v.split(".")] for v in locks])]

    # Calculate the latest version for each binary
    mapped = {}
    for version in ordered:
        mapped |= {k.removesuffix(".ape"): version for k in locks[version]}

    # Perform any version overrides
    invalid = ["{}-{}".format(k, v) for k, v in overrides.items() if k not in mapped or v not in locks]
    if invalid:
        fail("Invalid overrides not available in any provided lockfile: {}".format(", ".join(invalid)))
    mapped |= overrides

    # Validate the used versions exist
    invalid = [v for v in use.versions if v not in locks]
    if invalid:
        fail("Invalid versions to use: {}".format(", ".join(invalid)))

    # Return data to process into repositories
    return struct(
        urls = tuple(urls),
        overrides = overrides,
        upload = upload,
        locks = locks,
        ordered = tuple(ordered),
        latest = ordered[-1],
        mapped = mapped,
        use = use,
    )

def implementation(mctx):
    # Direct repository names to return so `bazel mod tidy` can update `use_repo` in `MODULE.bazel`
    direct = []

    # Collect all the data from the extension tags across the graph
    data = _collect(mctx)

    # Generate a download/entrypoint for each repository
    for version, lock in data.locks.items():
        for name, integrity in lock.items():
            # Template the URLs for this binary/version
            urls = [u.format(name = name, version = version) for u in data.urls]

            # Bootstrap binaries are used to create entrypoints so just download
            if _bootstrap(name):
                download_file(
                    name = "{}-{}".format(name, version),
                    urls = urls,
                    executable = True,
                    integrity = integrity,
                    output = "binary",
                )
                if version in data.use.versions:
                    direct.append("{}-{}".format(name, version))
                continue

            # Generate a download/entrypoint pairing fo repositories
            basename = name.removesuffix(".ape")
            download_file(
                name = "{}-{}-download".format(basename, version),
                urls = [u.format(name = name, version = version) for u in data.urls],
                executable = True,
                integrity = integrity,
                output = basename,
            )
            ape_entrypoint(
                name = "{}-{}".format(basename, version),
                binary = "@{}-{}-download//:{}".format(basename, version, basename),
            )
            if version in data.use.versions:
                direct.append("{}-{}".format(name, version))

    # Provide unversioned repository names for mapped versions
    # These are the latest version _or_ overridden to a specific version with `cosmos.override`
    for basename, version in data.mapped.items():
        if _bootstrap(basename):
            ape_cosmos_symlink(
                name = basename,
                target = "@{}-{}".format(basename, version),
            )
            if data.use.bootstrap:
                direct.append(basename)
            continue

        ape_entrypoint(
            name = basename,
            binary = "@{}-{}-download//:{}".format(basename, version, basename),
        )

        if data.use.unversioned:
            direct.append(basename)

    # Create an aliases hub repository that can be used to redirect to each binary
    ape_cosmos_aliases(
        name = "aliases",
        srcs = {"@{}//:binary".format(n): n for n in data.mapped},
    )
    if data.use.upload:
        direct.append("aliases")

    # Output the metadata in `.bzl` format for consumption in `BUILD.bazel` files
    ape_cosmos_metadata(
        name = "metadata",
        mapped = data.mapped,
        latest = data.latest,
        ordered = data.ordered,
        basenames = {v: [b.removesuffix(".ape") for b in d] for v, d in data.locks.items()},
    )
    if data.use.metadata:
        direct.append("metadata")

    # Upload targets for each downloaded binary
    if data.upload:
        ape_cosmos_upload(
            name = "upload",
            srcs = {
                "@{}-{}//:binary".format(name.removesuffix(".ape"), version): data.upload.format(name = name, version = version)
                for version, map in data.locks.items()
                for name in map
            },
        )
        if data.use.upload:
            direct.append("upload")

    # Done!
    if direct:
        return mctx.extension_metadata(
            root_module_direct_deps = direct,
            root_module_direct_dev_deps = [],
            reproducible = True,
        )
    return mctx.extension_metadata(
        reproducible = True,
    )

ape_cosmos = module_extension(
    doc = DOC,
    implementation = implementation,
    tag_classes = TAGS,
)

cosmos = ape_cosmos
