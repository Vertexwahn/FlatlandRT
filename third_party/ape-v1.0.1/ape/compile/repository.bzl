load("@toolchain_utils//toolchain/triplet:local.bzl", "TRIPLET")

visibility("//ape/...")

DOC = """Performs repository time compilation of source files."""

ATTRS = {
    "build": attr.label(
        doc = "The `BUILD.bazel` template.",
        allow_single_file = True,
        default = ":BUILD.tmpl.bazel",
    ),
    "srcs": attr.label_list(
        doc = "The source files to compile.",
        allow_files = [".c"],
        allow_empty = False,
    ),
    "target": attr.string(
        doc = "`zig` target triplet to build for. Can use `{cpu}`/`{os}`/`{libc}` replacements",
        default = "{cpu}-{os}-{libc}",
    ),
    "output": attr.string(
        doc = "The compiled output file.",
        mandatory = True,
    ),
    "links": attr.string_dict(
        doc = 'Creates links in the downloaded repository as `{" < target > ": " < link > "}`. Depending on the platform, either symbolic or hard links are created.',
    ),
    "zig": attr.label(
        doc = "The `zig` compiler to use.",
        allow_single_file = True,
        executable = True,
        cfg = "exec",
        default = "@zig",
    ),
}

def implementation(rctx):
    map = {
        "amd64": "x86_64",
        "arm64": "aarch64",
    }

    triplet = rctx.attr.target.format(
        cpu = map.get(TRIPLET.cpu, TRIPLET.cpu),
        os = TRIPLET.os.kind,
        libc = TRIPLET.libc.kind,
    )

    cmd = [
        rctx.attr.zig,
        "cc",
        "-target",
        triplet,
        "-o",
        rctx.attr.output,
    ]
    cmd.extend(rctx.attr.srcs)

    rctx.watch(rctx.attr.zig)
    for src in rctx.attr.srcs:
        rctx.watch(src)

    result = rctx.execute(cmd)
    if result.return_code != 0:
        fail("Failed to compile: {}".format(result.stderr))

    for target, linkname in rctx.attr.links.items():
        rctx.symlink(linkname, target)

    rctx.template("BUILD.bazel", rctx.attr.build, {
        "{{exports}}": repr([rctx.attr.output] + rctx.attr.links.keys()),
    })

compile = repository_rule(
    doc = DOC,
    attrs = ATTRS,
    implementation = implementation,
    configure = True,
)
