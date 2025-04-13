visibility("//...")

DOC = """Generates a repository to upload APE `cosmos` binaries.

Provides a `genrule` for each that performs an upload with `@ape//ape:curl`.

All `genrule`s are pulled into a `upload` file group.

The upload of all binaries can be performed with `bazel build @upload`.

Commonly, this would be a `bazel run` target but it takes advantage of the local checking to only upload binaries that have changed.
"""

ATTRS = {
    "srcs": attr.label_keyed_string_dict(
        doc = "Binaries to upload to URLs.",
        allow_empty = False,
        mandatory = True,
    ),
    "_upload": attr.label(
        allow_single_file = [".tmpl.bzl"],
        default = ":BUILD.upload.tmpl.bazel",
    ),
    "_filegroup": attr.label(
        allow_single_file = [".tmpl.bzl"],
        default = ":BUILD.filegroup.tmpl.bazel",
    ),
}

def implementation(rctx):
    SEPARATOR = Label("@upload").workspace_name.removesuffix("upload")[-1]

    names = []
    for label, url in rctx.attr.srcs.items():
        name = label.workspace_name.rsplit(SEPARATOR, 1)[-1]
        rctx.template(
            "{}/BUILD.bazel".format(name),
            rctx.attr._upload,
            substitutions = {
                "{{name}}": name,
                "{{target}}": str(label),
                "{{url}}": url,
            },
            executable = True,
        )
        names.append(name)

    rctx.template(
        "BUILD.bazel",
        rctx.attr._filegroup,
        substitutions = {
            "{{name}}": "upload",
            "{{srcs}}": repr(["//{}".format(n) for n in names]),
        },
        executable = False,
    )

ape_cosmos_upload = repository_rule(
    doc = DOC,
    attrs = ATTRS,
    implementation = implementation,
    local = True,
)

upload = ape_cosmos_upload
