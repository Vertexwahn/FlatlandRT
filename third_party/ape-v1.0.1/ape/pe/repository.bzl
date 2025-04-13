visibility("//ape/...")

DOC = """An αcτµαlly pδrταblε εxεcµταblε (APE) launcher for portable executable (PE).

This is _not_ needed as APE binaries are natively executable as PE executables.

The repository is used to make the APE host select consistent.
"""

ATTRS = {
    "output": attr.string(
        doc = "The output file to create",
        mandatory = True,
    ),
    "links": attr.string_dict(
        doc = 'Creates links in the downloaded repository as `{" < target > ": " < link > "}`. Depending on the platform, either symbolic or hard links are created.',
    ),
    "build": attr.label(
        doc = "The `BUILD.bazel` template.",
        allow_single_file = True,
        default = ":BUILD.tmpl.bazel",
    ),
    "_cs": attr.label(
        doc = "C# source code for the APE executable.",
        default = ":ape.cs",
    ),
}

def csc(rctx):
    """Finds the C# compiler from common directories.

    Args:
      rctx: the repository context to perform local command execution with

    Returns:
      A Bazel path to the C# compiler
    """
    result = rctx.execute(("cmd.exe", "/c", "@echo.%SYSTEMROOT%"))
    if result.return_code != 0:
        fail("Failed to find system root: {}".format(result.stderr))
    root = rctx.path(result.stdout.strip())

    framework = root.get_child("Microsoft.NET/Framework64")
    if not framework.exists:
        fail("Failed to find .NET framework: {}".format(framework))

    for child in framework.readdir():
        csc = child.get_child("csc.exe")
        if csc.exists:
            return csc

    fail("Failed to find C# compiler")

def implementation(rctx):
    compiler = csc(rctx)

    ape = rctx.path("ape.exe")
    source = rctx.path(rctx.attr._cs)
    source = str(source).replace("/", "\\")

    rctx.report_progress("Compiling C# APE")
    result = rctx.execute((compiler, "/warnaserror", "/warn:4", "/nologo", "/out:{}".format(ape), "/target:exe", "/optimize", source))
    if result.return_code != 0:
        fail("Failed to compile C# APE: {}".format(result.stdout))

    rctx.symlink("ape.exe", rctx.attr.output)
    for target, linkname in rctx.attr.links.items():
        rctx.symlink(linkname, target)

    rctx.template("BUILD.bazel", rctx.attr.build, {
        "{{exports}}": repr([rctx.attr.output] + rctx.attr.links.keys()),
    })

pe = repository_rule(
    doc = DOC,
    attrs = ATTRS,
    implementation = implementation,
    configure = True,
)
