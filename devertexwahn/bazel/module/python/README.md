# Python toolchain

This directory defines the Python toolchain.

Currently, Python `3.13.2` is used.

## Updating the lock files

You can regenerate the lock files by running:

*macOS*

```shell
bazel run --config=macos //bazel/module/python:requirements.update
```

*Ubuntu 26.04*

```shell
bazel run --config=gcc15 //bazel/module/python:requirements.update
```

*Windows*:

The `vs2026` config sets some flags that are required to get this update working.

> **Important:** Run the terminal as Administrator. Windows UAC installer detection triggers on executables named `*.update.exe` and requires elevation (`CreateProcessW` error 740).

```shell
bazel --output_base=C:/bazel_output_base run --config=vs2026 //bazel/module/python:requirements.update
```

This has to be repeated on every platform.

## References

- https://github.com/sschaetz/bazel-textual-experiments/
- https://github.com/bazelbuild/rules_python/blob/main/examples/pip_parse/MODULE.bazel
