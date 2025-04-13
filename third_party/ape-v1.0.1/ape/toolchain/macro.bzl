visibility("//...")

def toolchain(*, name, toolchain, toolchain_type, exec_compatible_with = (), target_compatible_with = ()):
    """
    Registers an αcτµαlly pδrταblε εxεcµταblε (APE) binary as a Bazel toolchain.

    Only registers the APE binary for platform constraints that are known to work.

    ```py
    load("@ape//ape/toolchain:defs.bzl", "ape_toolchain")

    toolchain_type(
        name = "type",
    )

    ape_toolchain(
        name = "ape",
        toolchain = "@ape//ape/toolchain:curl",
        toolchain_type = ":type",
    )
    ```
    """
    toolchain = native.package_relative_label(toolchain)
    toolchain_type = native.package_relative_label(toolchain_type)
    exec_compatible_with = tuple([native.package_relative_label(x) for x in exec_compatible_with])
    target_compatible_with = tuple([native.package_relative_label(x) for x in target_compatible_with])

    [
        native.toolchain(
            name = "{}-{}-{}".format(name, cpu, os),
            toolchain = toolchain,
            toolchain_type = toolchain_type,
            exec_compatible_with = (
                Label("@toolchain_utils//toolchain/constraint/cpu:{}".format(cpu)),
                Label("@toolchain_utils//toolchain/constraint/os:{}".format(os)),
            ) + exec_compatible_with,
            target_compatible_with = target_compatible_with,
        )
        # https://github.com/jart/cosmopolitan/wiki/Features#systems-and-cpu-arch-supported
        for cpu, os in (
            ("amd64", "linux"),
            ("amd64", "windows"),
            ("amd64", "macos"),
            ("amd64", "openbsd"),
            ("amd64", "netbsd"),
            ("amd64", "freebsd"),
            ("arm64", "linux"),
            ("arm64", "macos"),
            ("arm64", "freebsd"),
        )
    ]
