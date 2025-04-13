load("//ape:separator.bzl", "SEPARATOR")

visibility("//ape/...")

DOC = """Provides a executable entrpoint file.

The file can be used as a hermetic binary label to a repository rule as the file is compatible with `rctx.execute`.
"""

ATTRS = {
    "launcher": attr.label(
        doc = "The αcτµαlly pδrταblε εxεcµταblε launcher.",
        allow_single_file = True,
        executable = True,
        cfg = "exec",
        default = "@launcher//:binary",
    ),
    "assimilate": attr.label(
        doc = "The assimilation APE binary.",
        allow_single_file = True,
        executable = True,
        cfg = "exec",
        default = "@assimilate//:binary",
    ),
    "binary": attr.label(
        doc = "The αcτµαlly pδrταblε εxεcµταblε binary.",
        allow_single_file = True,
        mandatory = True,
    ),
    "build": attr.label(
        doc = "The `BUILD.bazel` template.",
        allow_single_file = True,
        default = ":BUILD.tmpl.bazel",
    ),
    "posix": attr.label(
        doc = "The shell script entrypoint template.",
        allow_single_file = True,
        default = ":posix.tmpl.sh",
    ),
}

def implementation(rctx):
    if rctx.os.name != "linux":
        rctx.symlink(rctx.attr.binary, "entrypoint")
    else:
        cmd = (
            rctx.attr.launcher,
            rctx.path(rctx.attr.assimilate),
            "-o",
            "entrypoint",
            rctx.path(rctx.attr.binary),
        )
        result = rctx.execute(cmd)
        if result.return_code != 0:
            fail("Failed to assimilate: {}".format(cmd))

    _, found, name = rctx.name.rpartition(SEPARATOR)
    if not found:
        fail("Failed to find separator in repository name.")
    rctx.symlink("entrypoint", name)

    rctx.template("BUILD.bazel", rctx.attr.build, {
        "{{exports}}": repr(["entrypoint", name]),
        "{{binary}}": str(rctx.attr.binary),
    })

entrypoint = repository_rule(
    doc = DOC,
    attrs = ATTRS,
    implementation = implementation,
    configure = True,
    local = True,
)
