# Python toolchain

This directory defines the Python toolchain.

Currently Python `3.13.2` is used.

## Updating the lock files

You can regenerate the lock files by running:

*macOS*

```shell
bazel run --config=macos //bazel/module/python:requirements.update
```

*Ubuntu 24.04*

```shell
bazel run --config=gcc13 //bazel/module/python:requirements.update
```

*Windows*:

The `vs2022` config sets some flagas that required to get this update working:

```shell
bazel run --config=vs2022 //bazel/module/python:requirements.update
```

This as to be repeated on every platform.

## References

- https://github.com/sschaetz/bazel-textual-experiments/
- https://github.com/bazelbuild/rules_python/blob/main/examples/pip_parse/MODULE.bazel
