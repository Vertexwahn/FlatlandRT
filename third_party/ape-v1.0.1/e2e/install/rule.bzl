visibility("//...")

DOC = "Installs a file."

ATTRS = {
    "src": attr.label(
        doc = "The file to install",
        allow_single_file = True,
        mandatory = True,
    ),
}

def implementation(ctx):
    install = ctx.toolchains["//toolchain/install:type"]

    output = ctx.actions.declare_file(ctx.label.name)

    args = ctx.actions.args()
    args.add(ctx.file.src)
    args.add(output)

    ctx.actions.run(
        arguments = [args],
        executable = install.run,
        inputs = ctx.attr.src.files,
        outputs = [output],
        mnemonic = "Install",
        toolchain = "//toolchain/install:type",
    )

    return DefaultInfo(files = depset([output]))

install = rule(
    doc = DOC,
    attrs = ATTRS,
    implementation = implementation,
    toolchains = ["//toolchain/install:type"],
)
