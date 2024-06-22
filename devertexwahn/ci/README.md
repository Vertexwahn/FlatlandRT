# Continuous Integration

In this folder, you can find CI jobs based on Azure Pipelines.
These jobs make sure that `bazel build` and `bazel test` work across different compilers and operating systems.
An overview of supported Microsoft-hosted agents supported by Azure Pipelines can be found [here](https://learn.microsoft.com/en-us/azure/devops/pipelines/agents/hosted?view=azure-devops&tabs=yaml).

## Static analysis of C/C++ code

For static analysis of C/C++ code [Cppcheck](https://github.com/danmar/cppcheck/) is used.
