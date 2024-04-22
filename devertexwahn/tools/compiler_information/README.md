# How to run?

## macOS

```shell
bazel run --config=macos //tools/compiler_information
```

## Ubuntu

```shell
bazel run --config=gcc11 //tools/compiler_information
```

```shell
bazel run --config=clang16 //tools/compiler_information
```

# Windows

```shell
bazel --output_base=G:/bazel_output_base run --config=vs2022 --compilation_mode=opt //tools/compiler_information
```
