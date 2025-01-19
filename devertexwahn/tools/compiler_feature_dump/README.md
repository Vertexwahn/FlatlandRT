# How to run?

## Ubuntu 24.04

```shell
bazel run --config=gcc13 //tools/compiler_feature_dump
```

## Ubuntu 22.04

```shell
bazel run --config=gcc11 //tools/compiler_feature_dump
```

```shell
bazel run --config=clang19 //tools/compiler_feature_dump
```

## macOS

```shell
bazel run --config=macos //tools/compiler_feature_dump
```

# Windows

```shell
bazel --output_base=G:/bazel_output_base run --config=vs2022 --compilation_mode=opt //tools/compiler_feature_dump
```
