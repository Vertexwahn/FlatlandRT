# `ape`

> A Bazel module for working with αcτµαlly pδrταblε εxεcµταblε (APE) pre-built binaries.

## Getting Started

Add the following to `MODULE.bazel`:

```py
bazel_dep(name = "ape", version = "0.0.0")
```

Binaries are provided as executable targets:

```sh
bazel run -- @ape//ape:curl --version
```

List the available executables:

```sh
bazel query 'kind(alias, @ape//ape:*)'
```

Register a binary as a toolchain for [supported platforms][supported-platforms]:

```py
load("@ape//ape/toolchain:defs.bzl", "ape_toolchain")

toolchain_type(
    name = "type",
)

ape_toolchain(
    name = "ape",
    toolchain = "@ape//toolchain/info:curl",
    toolchain_type = ":type",
)
```

Use a binary in a repository rule:

```py
cosmos = use_extension("@ape//ape/cosmos:defs.bzl", "ape_cosmos")
use_repo(cosmos, "zstd")

example = use_repo_rule(":example.bzl", "example")
example(
    zstd = "@zstd",
)
```

## Hermeticity

The module is fully hermetic, all binaries are downloaded and verified against subresource integrities.

## Cosmopolitian

The project provides pre-built, hermetic [Cosmopolitan pre-built binaries][cosmo.zip].

Cosmopolitan binaries work across all operating systems as explained [upstream][ape].

Binaries are built with [superconfigure].

The [superconfigure] source code is mirrored in this repository on the [`superconfigure/main`][mirror] branch. The source contains the source code for the binaries, patches and license information.

Binaries are mirrored in the [GitLab generic package registry][package-registry] to protect against upstream outages.

The launcher for Apple Silicon must be built from source. [Zig] is used to compile the [launcher][ape-m1.c] from source. These are both mirrored in the [package registry][package-registry]. Source, licenses and distribution information is available at the upstream links.

---

[cosmo.zip]: https://cosmo.zip
[ape]: https://justine.lol/ape.html
[superconfigure]: https://github.com/ahgamut/superconfigure
[package-registry]: https://gitlab.arm.com/bazel/ape/-/packages
[mirror]: https://gitlab.arm.com/bazel/ape/-/tree/superconfigure/main
[supported-platforms]: https://github.com/jart/cosmopolitan/wiki/Features#systems-and-cpu-arch-supported
[Zig]: https://ziglang.org/
[ape-m1.c]: https://github.com/jart/cosmopolitan/blob/3.3.3/ape/ape-m1.c
