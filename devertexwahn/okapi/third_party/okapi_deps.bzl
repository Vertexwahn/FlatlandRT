"""
    SPDX-FileCopyrightText: Copyright 2022-2024 Julian Amann <dev@vertexwahn.de>
    SPDX-License-Identifier: Apache-2.0
"""

load("@bazel_tools//tools/build_defs/repo:git.bzl", "git_repository")
load("@bazel_tools//tools/build_defs/repo:http.bzl", "http_archive")
load("@bazel_tools//tools/build_defs/repo:utils.bzl", "maybe")

def okapi_deps():
    """Fetch external dependencies for Okapi."""

    #-------------------------------------------------------------------------------------
    # bazel_clang_tidy
    #-------------------------------------------------------------------------------------

    maybe(
        native.local_repository,
        name = "bazel_clang_tidy",
        path = "../third_party/bazel_clang_tidy-4884c32e09c1ea9ac96b3f08c3004f3ac4c3fe39",
    )

