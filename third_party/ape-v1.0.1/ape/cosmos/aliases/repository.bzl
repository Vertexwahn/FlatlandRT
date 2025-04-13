load("@bazel_skylib//lib:structs.bzl", "structs")

visibility("//...")

DOC = """Generates aliases to the binary downloads."""

ATTRS = {
    "srcs": attr.label_keyed_string_dict(
        doc = "Aliases to create (`{'<actual>': '<name>'}`, ...).",
        allow_empty = False,
        mandatory = True,
    ),
    "_template": attr.label(
        allow_single_file = [".tmpl.bzl"],
        default = ":BUILD.tmpl.bazel",
    ),
}

def implementation(rctx):
    for actual, name in rctx.attr.srcs.items():
        rctx.template(
            "{}/BUILD.bazel".format(name),
            rctx.attr._template,
            substitutions = {
                "{{name}}": name,
                "{{actual}}": str(actual),
            },
            executable = False,
        )

ape_cosmos_aliases = repository_rule(
    doc = DOC,
    attrs = ATTRS,
    implementation = implementation,
    local = True,
)

aliases = ape_cosmos_aliases
