"""TODO(aiuto): Write module docstring."""

def _impl(ctx):
    ctx.actions.write(ctx.outputs.out, ctx.attr.var)


genx = rule(
    implementation = _impl,
    # @unsorted-dict-items
    attrs = {
        "out": attr.output(mandatory = True),
        "var": attr.string(mandatory = True),
    }
)
