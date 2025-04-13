visibility("//...")

DOC = """Generates a lock file for a versioned `cosmos.zip` end point.

```py
load("@ape//ape/cosmos/lock:defs.bzl", "ape_cosmos_lock")

# `bazel run` any of these targets to generate the lockfile

ape_cosmos_lock(
    name = "4.0.2",
    out = "4.0.2.json",
    tags = ["manual"],
    version = "4.0.2",
)
```

Performing `bazel run :4.0.2` will update the `:4.0.2.json` file with a dictionary of binary names and their sub-resource integrity.

The lock file can then be used with the `@ape//ape/cosmos:defs.bzl#ape_cosmos` extension.
"""

ATTRS = {
    "version": attr.string(
        doc = "The version of `cosmos` to generate a lock file for.",
        mandatory = True,
    ),
    "url": attr.string(
        doc = "The Python format template URL to the `cosmos` binaries to lock.",
        default = "https://cosmo.zip/pub/cosmos/v/{version}/bin/{name}",
    ),
    "out": attr.output(
        doc = "The output lockfile. Commonly in `.json` or `.jseq` formats.",
        mandatory = True,
    ),
    "format": attr.string(
        doc = "Forces an output format, rather than determining from `out` extension.",
        values = ["json", "json-seq"],
    ),
    "_lock": attr.label(
        doc = "Locking executable.",
        executable = True,
        cfg = "exec",
        default = ":lock",
    ),
}

JOBS = 16

def _resources(os, inputs):
    return {"cpu": JOBS}

def _runfile(label, file):
    path = file.short_path
    if path.startswith("../"):
        return path.removeprefix("../")
    return "{}/{}".format(label.workspace_name or "_main", path)

def implementation(ctx):
    output = ctx.outputs.out

    lock = ctx.actions.args()
    lock.add("--url", ctx.attr.url)
    lock.add("--jobs", JOBS)
    lock.add("--version", ctx.attr.version)
    lock.add("--output", output)
    if ctx.attr.format:
        lock.add("--{}".format(ctx.attr.format))
    elif output.extension == "json":
        lock.add("--json")
    elif output.extension == "jseq":
        lock.add("--json-seq")
    else:
        fail("Unknown output extension: {}".format(output.extension))

    ctx.actions.run(
        outputs = [output],
        executable = ctx.attr._lock.files_to_run,
        arguments = [lock],
        mnemonic = "ApeCosmosLock",
        execution_requirements = {"requires-network": "1"},
        resource_set = _resources,
    )

    copy = ctx.actions.args()
    copy.add("--version", ctx.attr.version)
    copy.add("--input", _runfile(ctx.label, output))
    copy.add("--output", output.short_path)

    arguments = ctx.actions.declare_file("baked.args")
    ctx.actions.write(
        output = arguments,
        content = copy,
        is_executable = False,
    )

    executable = ctx.actions.declare_file("{}/copy".format(ctx.label.name))
    ctx.actions.symlink(
        output = executable,
        target_file = ctx.executable._lock,
        is_executable = True,
    )

    files = depset([output])
    symlinks = {"baked.args": arguments}
    runfiles = ctx.runfiles(files = [arguments, output], root_symlinks = symlinks)
    runfiles = runfiles.merge(ctx.attr._lock.default_runfiles)

    default = DefaultInfo(executable = executable, files = files, runfiles = runfiles)

    return default
    return DefaultInfo(files = files)

lock = rule(
    doc = DOC,
    attrs = ATTRS,
    implementation = implementation,
    executable = True,
)

ape_cosmos_lock = lock
