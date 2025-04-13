load("@bazel_skylib//lib:structs.bzl", "structs")

visibility("//...")

DOC = """Writes out APE `cosmos` extension metadata into `.bzl` files.

Repository attributes are written to file, for `mapped` attribute a `mapped.bzl#MAPPED` will be created.
"""

ATTRS = {
    "mapped": attr.string_dict(
        doc = "Mapped versions for for each `cosmos` binary (`{'<basename>': '<version>'}`, ...).",
        allow_empty = False,
        mandatory = True,
    ),
    "latest": attr.string(
        doc = "The latest version of `cosmos` configured.",
        mandatory = True,
    ),
    "ordered": attr.string_list(
        doc = "The ascending ordered versions of `cosmos`.",
        mandatory = True,
    ),
    "basenames": attr.string_list_dict(
        doc = "Binaries in each version (`{'<version>': ['<basename>', ...], ...}`).",
        allow_empty = False,
        mandatory = True,
    ),
    "_template": attr.label(
        allow_single_file = [".tmpl.bzl"],
        default = ":value.tmpl.bzl",
    ),
}

def _template(rctx, attr):
    rctx.template(
        "{}.bzl".format(attr),
        rctx.attr._template,
        substitutions = {
            "{{name}}": attr.upper(),
            "{{value}}": repr(getattr(rctx.attr, attr)),
        },
        executable = False,
    )

def implementation(rctx):
    for attr in ATTRS:
        if not attr.startswith("_"):
            _template(rctx, attr)

    rctx.file("BUILD.bazel", "", executable = False)

ape_cosmos_metadata = repository_rule(
    doc = DOC,
    attrs = ATTRS,
    implementation = implementation,
    local = True,
)

metadata = ape_cosmos_metadata
