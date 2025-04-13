visibility("//ape/cosmos/...")

DOC = "Symlinks a repository to another."

ATTRS = {
    "target": attr.label(
        doc = "The repository to symlink to.",
        mandatory = True,
    ),
}

def root(rctx, label):
    workspace = label.relative("WORKSPACE")
    path = rctx.path(workspace)
    if path.exists:
        return path.dirname

    repo = label.relative("REPO.bazel")
    path = rctx.path(repo)
    if path.exists:
        return path.dirname

    return None

def implementation(rctx):
    label = rctx.attr.target
    path = root(rctx, label)
    if not path:
        fail("Can only symlink repository labels.".format(label))
    rctx.delete(".")
    rctx.symlink(path, ".")

ape_cosmos_symlink = repository_rule(
    doc = DOC,
    attrs = ATTRS,
    implementation = implementation,
    configure = True,
    local = True,
)

symlink = ape_cosmos_symlink
