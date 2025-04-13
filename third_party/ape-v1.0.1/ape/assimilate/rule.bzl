visibility("//ape/assimilate/...")

DOC = """Assimilates an αcτµαlly pδrταblε εxεcµταblε (APE) into a native binary."""

ATTRS = {
    "src": attr.label(
        doc = "The APE binary target to assimilate",
        allow_single_file = True,
        executable = True,
        cfg = "exec",
    ),
    "basename": attr.string(
        doc = "The basename for the symlink, which defaults to `name`",
    ),
    "_assimilate": attr.label(
        doc = "The APE assimilate binary.",
        allow_single_file = True,
        executable = True,
        cfg = "exec",
        default = "@assimilate//:binary",
    ),
    "_macos": attr.label(
        providers = [platform_common.ConstraintValueInfo],
        default = "@platforms//os:macos",
    ),
    "_windows": attr.label(
        providers = [platform_common.ConstraintValueInfo],
        default = "@platforms//os:windows",
    ),
    "_arm64": attr.label(
        providers = [platform_common.ConstraintValueInfo],
        default = "@platforms//cpu:aarch64",
    ),
    "_amd64": attr.label(
        providers = [platform_common.ConstraintValueInfo],
        default = "@platforms//cpu:x86_64",
    ),
    "_ppc64": attr.label(
        providers = [platform_common.ConstraintValueInfo],
        default = "@platforms//cpu:ppc",
    ),
}

def implementation(ctx):
    windows = ctx.attr._windows[platform_common.ConstraintValueInfo]
    macos = ctx.attr._macos[platform_common.ConstraintValueInfo]
    arm64 = ctx.attr._arm64[platform_common.ConstraintValueInfo]

    basename = ctx.attr.basename or ctx.label.name
    if ctx.target_platform_has_constraint(windows) and not basename.endswith(".exe"):
        basename += ".exe"

    if ctx.target_platform_has_constraint(windows):
        executable, files, runfiles = _direct(ctx, basename)
    elif ctx.target_platform_has_constraint(macos) and ctx.target_platform_has_constraint(arm64):
        executable, files, runfiles = _indirect(ctx, basename)
    else:
        executable, files, runfiles = _assimilate(ctx, basename)

    return DefaultInfo(
        executable = executable,
        files = files,
        runfiles = runfiles,
    )

def _assimilate(ctx, basename):
    executable = ctx.actions.declare_file("{}/{}".format(ctx.label.name, basename))
    ape = ctx.toolchains["//ape/toolchain/ape:type"]

    windows = ctx.attr._windows[platform_common.ConstraintValueInfo]
    macos = ctx.attr._macos[platform_common.ConstraintValueInfo]
    arm64 = ctx.attr._arm64[platform_common.ConstraintValueInfo]
    amd64 = ctx.attr._amd64[platform_common.ConstraintValueInfo]
    ppc64 = ctx.attr._ppc64[platform_common.ConstraintValueInfo]

    args = ctx.actions.args()
    args.add(ctx.file._assimilate)

    # Executable format
    if ctx.target_platform_has_constraint(windows):
        fail("Cannot assimilate on Windows, APE are already PE format. Use direct symlink instead.")
    elif ctx.target_platform_has_constraint(macos) and ctx.target_platform_has_constraint(arm64):
        fail("Cannot assimilate on Apple Silicon, assimilate binaries are not runnable without APE loader. Use indirect symlink instead.")
    elif ctx.target_platform_has_constraint(macos):
        args.add("-m")  # MachO
    else:
        args.add("-e")  # ELF

    # Executable CPU
    if ctx.target_platform_has_constraint(amd64):
        args.add("-x")  # amd64
    elif ctx.target_platform_has_constraint(arm64):
        args.add("-a")  # arm64
    elif ctx.target_platform_has_constraint(ppc64):
        args.add("-p")  # ppc64
    else:
        fail("Unknown assimilation CPU.")

    args.add("-o", executable.path)
    args.add(ctx.file.src)

    ctx.actions.run(
        outputs = [executable],
        inputs = [ctx.file._assimilate, ctx.file.src],
        executable = ape.run,
        arguments = [args],
        mnemonic = "ApeAssimilate",
        progress_message = "assimilating %{input}",
        toolchain = "//ape/toolchain/ape:type",
    )

    files = depset([executable])
    runfiles = None

    return executable, files, runfiles

def _direct(ctx, basename):
    executable = ctx.actions.declare_file("{}/{}".format(ctx.label.name, basename))

    ctx.actions.symlink(
        output = executable,
        target_file = ctx.file.src,
        is_executable = True,
    )

    files = depset([executable])
    runfiles = ctx.runfiles([executable, ctx.file.src])
    runfiles.merge(ctx.attr.src.default_runfiles)

    return executable, files, runfiles

def _indirect(ctx, basename):
    indirect = ctx.actions.declare_file("{}/{}.ape".format(ctx.label.name, basename))
    ape = ctx.toolchains["//ape/toolchain/ape:type"]

    ctx.actions.symlink(
        output = indirect,
        target_file = ape.executable,
        is_executable = True,
    )

    direct = ctx.actions.declare_file(basename, sibling = indirect)

    ctx.actions.symlink(
        output = direct,
        target_file = ctx.file.src,
        is_executable = True,
    )

    files = depset([indirect, direct])

    runfiles = ctx.runfiles([indirect, direct, ctx.file.src, ape.executable])
    runfiles.merge(ctx.attr.src.default_runfiles)
    runfiles.merge(ape.default.default_runfiles)

    return indirect, files, runfiles

ape_assimilate = rule(
    doc = DOC,
    attrs = ATTRS,
    implementation = implementation,
    toolchains = ["//ape/toolchain/ape:type"],
    executable = True,
)

assimilate = ape_assimilate
