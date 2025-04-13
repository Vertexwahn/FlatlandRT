# Contributing

# Getting Started

[Install][bazelisk-install] `bazelisk` and run the tests:

```sh
$ bazelisk test //...
$ (cd e2e; bazelisk test //...)
```

[bazelisk-install]: https://github.com/bazelbuild/bazelisk?tab=readme-ov-file#installation

# Workflow

- Follow the [contributions guide] to be granted forking permissions.
- [Fork] the project
- Implement the change
- [Create merge request] from fork

# Upgrading Binaries

Add a new lock file generator in `ape/cosmos/BUILD.bazel`:

```py
ape_cosmos_lock(
    name = "<version>",
    out = "<version>.json",
    tags = ["manual"],
    version = "<version>",
)
```

Run the target to generate the JSON lock file:

```console
$ bazelisk run ape/cosmos:<version>
$ cat ape/cosmos/<version>.json
```

Add the lock file into the `cosmos` extension in `MODULE.bazel`:

```py
cosmos.download(
    lock = "//ape/cosmos:<version>.json",
    version = "<version>",
)
```

Add any new binaries automatically by tidying the `MODULE.bazel`:

```console
$ bazelisk mod tidy
```

Export any new binaries in the `MODULE.bazel`:

```py
export.symlink(
    name = "ape-<basename>",
    target = "@<basename>",
)
```

Backup the binaries to GitLab generic package registry:

```console
$ bazelisk build ape/cosmos/upload
```

# Release

`semantic-release` performs automatic releases of the project.

Release channels for `alpha`, `beta` and `stable` releases are used.

The [upstream configuration usage guide][semrel-release-channels-usage] provides information on how to perform a
release.

[semrel-release-channels-usage]: https://gitlab.arm.com/semantic-release/config-release-channels/-/blob/main/README.md?ref_type=heads#usage
[contributions guide]: https://gitlab.arm.com/documentation/contributions#contributions
[Fork]: https://docs.gitlab.com/ee/user/project/repository/forking_workflow.html#create-a-fork
[Create merge request]: https://docs.gitlab.com/ee/user/project/repository/forking_workflow.html#merge-changes-back-upstream
