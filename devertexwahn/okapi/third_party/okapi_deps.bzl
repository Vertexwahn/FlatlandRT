"""
    SPDX-FileCopyrightText: 2022 Julian Amann <dev@vertexwahn.de>
    SPDX-License-Identifier: Apache-2.0
"""

load("@bazel_tools//tools/build_defs/repo:git.bzl", "git_repository")
load("@bazel_tools//tools/build_defs/repo:http.bzl", "http_archive")
load("@bazel_tools//tools/build_defs/repo:utils.bzl", "maybe")

def okapi_deps():
    """Fetch external dependencies for Okapi."""

    #-------------------------------------------------------------------------------------
    # Skylib
    #-------------------------------------------------------------------------------------

    # Use local repository
    maybe(
        native.local_repository,
        name = "bazel_skylib",
        path = "../third_party/bazel-skylib-d54e8a70ce90a24b921381cbe84fa0b88b22dc46",
    )

    # Use github.com
    #maybe(
    #    git_repository,
    #    name = "bazel_skylib",
    #    #branch = "main",
    #    commit = "6abad3de5fd9c001f67b17fe8c7242b3cc3b8851",
    #    shallow_since = "1649192995 -0400",
    #    remote = "https://github.com/bazelbuild/bazel-skylib",
    #)

    #-------------------------------------------------------------------------------------
    # Google Test
    #-------------------------------------------------------------------------------------

    # Use local repository
    maybe(
        native.local_repository,
        name = "com_google_googletest",
        path = "../third_party/googletest-bf66935e07825318ae519675d73d0f3e313b3ec6",
    )

    # Use github.com
    #maybe(
    #    http_archive,
    #    name = "googletest",
    #    sha256 = "cf8e496a802e6cb2fd02b09b84984f133eb2e79046265c2c6382c3c6df603d97",
    #    strip_prefix = "googletest-3e0e32ba300ce8afe695ad3ba7e81b21b7cf237a",
    #    urls = ["https://github.com/google/googletest/archive/8306020a3e9eceafec65508868d7ab5c63bb41f7.zip"],
    #)

    #-------------------------------------------------------------------------------------
    # Eigen
    #-------------------------------------------------------------------------------------

    # Use local repository
    maybe(
        native.new_local_repository,
        name = "eigen",
        path = "../third_party/eigen-3.4.0",
        build_file = "//okapi/third_party:eigen.BUILD",
    )

    # Use gitlab.com
    #maybe(
    #    http_archive,
    #    name = "eigen",
    #    build_file = "//okapi/third_party:eigen.BUILD",
    #    sha256 = "7985975b787340124786f092b3a07d594b2e9cd53bbfe5f3d9b1daee7d55f56f",
    #    strip_prefix = "eigen-3.3.9",
    #    url = "https://gitlab.com/libeigen/eigen/-/archive/3.3.9/eigen-3.3.9.tar.gz",
    #)

    #-------------------------------------------------------------------------------------
    # glog
    #-------------------------------------------------------------------------------------

    #maybe(
    #    git_repository,
    #    name = "com_github_gflags_gflags",
    #    commit = "827c769e5fc98e0f2a34c47cef953cc6328abced",
    #    remote = "https://github.com/gflags/gflags",
    #    shallow_since = "1604052972 +0000",
    #)

    maybe(
        native.local_repository,
        name = "com_github_gflags_gflags",
        path = "../third_party/gflags-986e8eed00ded8168ef4eaa6f925dc6be50b40fa",
    )

    # Use local repository
    maybe(
        native.local_repository,
        name = "glog",
        path = "../third_party/glog-b33e3bad4c46c8a6345525fd822af355e5ef9446",
    )

    # Use github.com
    #maybe(
    #    git_repository,
    #    name = "glog",
    #    #branch = "master",
    #    commit = "9733727be4aed835e21a8f73c403f546b71663fe",
    #    remote = "https://github.com/google/glog",
    #    shallow_since = "1630690048 +0200",
    #)

    #maybe(
    #    http_archive,
    #    name = "glog",
    #    sha256 = "21bc744fb7f2fa701ee8db339ded7dce4f975d0d55837a97be7d46e8382dea5a",
    #    strip_prefix = "glog-0.5.0",
    #    urls = ["https://github.com/google/glog/archive/refs/tags/v0.5.0.zip"],
    #)

    #-------------------------------------------------------------------------------------
    # Catch2
    #-------------------------------------------------------------------------------------

    # Use local repository
    maybe(
        native.local_repository,
        name = "catch2",
        path = "../third_party/Catch2-f526ff0fc37ae00ff2c0dc8f6550bf8209c30afc",
    )

    # Use github.com
    #maybe(
    #    http_archive,
    #    name = "catch2",
    #    sha256 = "48dfbb77b9193653e4e72df9633d2e0383b9b625a47060759668480fdf24fbd4",
    #    strip_prefix = "Catch2-2.13.6",
    #    urls = ["https://github.com/catchorg/Catch2/archive/refs/tags/v2.13.6.tar.gz"],
    #)

    #-------------------------------------------------------------------------------------
    # {fmt}
    #-------------------------------------------------------------------------------------

    # Use local repository
    maybe(
        native.local_repository,
        name = "fmt",
        path = "../third_party/fmt-ffb5e6a7328986706b48c3b4964ec596026f5e01",
    )

    # Live at Head
    #maybe(
    #    git_repository,
    #    name = "fmt",
    #    branch = "master",
    #    remote = "https://github.com/fmtlib/fmt",
    #    patch_cmds = [
    #        "mv support/bazel/.bazelrc .bazelrc",
    #        "mv support/bazel/.bazelversion .bazelversion",
    #        "mv support/bazel/BUILD.bazel BUILD.bazel",
    #        "mv support/bazel/WORKSPACE.bazel WORKSPACE.bazel",
    #    ],
    #    # Windows-related patch commands are only needed in the case MSYS2 is not installed.
    #    # More details about the installation process of MSYS2 on Windows systems can be found here:
    #    # https://docs.bazel.build/versions/main/install-windows.html#installing-compilers-and-language-runtimes
    #    # Even if MSYS2 is installed the Windows related patch commands can still be used.
    #    patch_cmds_win = [
    #        "Move-Item -Path support/bazel/.bazelrc -Destination .bazelrc",
    #        "Move-Item -Path support/bazel/.bazelversion -Destination .bazelversion",
    #        "Move-Item -Path support/bazel/BUILD.bazel -Destination BUILD.bazel",
    #        "Move-Item -Path support/bazel/WORKSPACE.bazel -Destination WORKSPACE.bazel",
    #    ],
    #)

    # Use github.com
    #maybe(
    #   http_archive,
    #   name = "fmt",
    #   build_file = "//okapi/third_party:fmt.BUILD",
    #   sha256 = "b06ca3130158c625848f3fb7418f235155a4d389b2abc3a6245fb01cb0eb1e01",
    #    strip_prefix = "fmt-8.0.1",
    #   urls = ["https://github.com/fmtlib/fmt/archive/refs/tags/8.0.1.tar.gz"],
    #)

    #-------------------------------------------------------------------------------------
    # hypothesis
    #-------------------------------------------------------------------------------------

    # Use local repository
    maybe(
        native.new_local_repository,
        name = "hypothesis",
        build_file = "//okapi/third_party:hypothesis.BUILD",
        path = "../third_party/hypothesis-e165503f7d796d844d5d6ab69b5c7f839cf0ede8",
    )

    # Use github.com
    #maybe(
    #    http_archive,
    #    name = "hypothesis",
    #    build_file = "//okapi/third_party:hypothesis.BUILD",
    #    sha256 = "257a8448bfc28e04497169caeaea986d8cfeb6c3c116abaedc003c30b23fd645",
    #    strip_prefix = "hypothesis-e165503f7d796d844d5d6ab69b5c7f839cf0ede8",
    #    urls = ["https://github.com/wjakob/hypothesis/archive/e165503f7d796d844d5d6ab69b5c7f839cf0ede8.zip"],
    #)

    #-------------------------------------------------------------------------------------
    # pugixml
    #-------------------------------------------------------------------------------------

    #maybe(
    #    http_archive,
    #    name = "pugixml",
    #    build_file = "//okapi/third_party:pugixml.BUILD",
    #    sha256 = "82226fc0cbdea450a06a4486c6514bd31984a75fbfcf359aea99080f4b878d2a",
    #    strip_prefix = "pugixml-1.12.1",
    #    urls = ["https://github.com/zeux/pugixml/releases/download/v1.11.4/pugixml-1.12.1.zip"],
    #)

    # Use local repository
    maybe(
        native.new_local_repository,
        name = "pugixml",
        build_file = "//okapi/third_party:pugixml.BUILD",
        path = "../third_party/pugixml-1.12.1",
    )

    #-------------------------------------------------------------------------------------
    # zlib
    #-------------------------------------------------------------------------------------

    # Use local repository
    maybe(
        native.new_local_repository,
        name = "zlib",
        build_file = "//okapi/third_party:zlib.BUILD",
        path = "../third_party/zlib-1.2.12",
    )

    # Use https://zlib.net
    #maybe(
    #    http_archive,
    #    name = "zlib",
    #    build_file = "//okapi/third_party:zlib.BUILD",
    #    sha256 = "c3e5e9fdd5004dcb542feda5ee4f0ff0744628baf8ed2dd5d66f8ca1197cb1a1",
    #   strip_prefix = "zlib-1.2.11",
    #    urls = ["https://zlib.net/zlib-1.2.11.tar.gz"],
    #)

    #-------------------------------------------------------------------------------------
    # pcg
    #-------------------------------------------------------------------------------------

    # PCG, A Family of Better Random Number Generators
    # Does not work with Visual Studio 2019
    #http_archive(
    #    name = "pcg",
    #    build_file = "//okapi/third_party:pcg.BUILD",
    #    sha256 = "25bb22f076e8c346fa28c2267ae3564b12122f1f4ab2d2a08ad8909dcd6319fd",
    #    strip_prefix = "pcg-cpp-0.98.1",
    #    urls = ["https://github.com/imneme/pcg-cpp/archive/v0.98.1.tar.gz"],
    #)

    # Use local repository
    maybe(
        native.new_local_repository,
        name = "pcg",
        build_file = "//okapi/third_party:pcg.BUILD",
        path = "../third_party/pcg-cpp-428802d1a5634f96bcd0705fab379ff0113bcf13",
    )

    #maybe(
    #    http_archive,
    #    name = "pcg",
    #    build_file = "//okapi/third_party:pcg.BUILD",
    #    sha256 = "64e7c049624706215e814fa99a73aadd4015d6da1b044b97bca1a66d8d713fcb",
    #    strip_prefix = "pcg-cpp-ffd522e7188bef30a00c74dc7eb9de5faff90092",
    #    urls = ["https://github.com/imneme/pcg-cpp/archive/ffd522e7188bef30a00c74dc7eb9de5faff90092.zip"],
    #)

    #-------------------------------------------------------------------------------------
    # tclap
    #-------------------------------------------------------------------------------------

    #maybe(
    #    http_archive,
    #    name = "tclap",
    #    build_file = "//okapi/third_party:tclap.BUILD",
    #    sha256 = "54d9afd826edf05accd24b56b8a50c7da10e3eaeae0cff698599cd595f50dd6d",
    #    strip_prefix = "tclap-1.4.0-rc1",
    #    urls = ["https://github.com/mirror/tclap/archive/1.4.0-rc1.tar.gz"],
    #)

    maybe(
        native.new_local_repository,
        name = "tclap",
        build_file = "//okapi/third_party:tclap.BUILD",
        path = "../third_party/tclap-1.4.0-rc1",
    )

    #-------------------------------------------------------------------------------------
    # yaml-cpp
    #-------------------------------------------------------------------------------------

    maybe(
        native.local_repository,
        name = "com_github_jbeder_yaml_cpp",
        path = "../third_party/yaml-cpp-yaml-cpp-0.7.0",
    )

    # yaml-cpp-0.6.3
    # Latest release does not support Bazel. Therefore, we use HEAD from master
    #http_archive(
    #    name = "com_github_jbeder_yaml_cpp",
    #    #build_file = "//okapi/third_party:tclap.BUILD",
    #    sha256 = "77ea1b90b3718aa0c324207cb29418f5bced2354c2e483a9523d98c3460af1ed",
    #    strip_prefix = "yaml-cpp-yaml-cpp-0.6.3",
    #    urls = ["https://github.com/jbeder/yaml-cpp/archive/yaml-cpp-0.6.3.tar.gz"],
    #)

    #maybe(
    #    git_repository,
    #    name = "com_github_jbeder_yaml_cpp",
    #    #branch = "master",
    #    commit = "9ee3928754693565fa561e2b6fda4058948d1d56",  # Emit the correct Alias on the key (#908) (#929)
    #   remote = "https://github.com/jbeder/yaml-cpp",
    #    shallow_since = "1625455804 -0500",
    #)

    #-------------------------------------------------------------------------------------
    # OpenEXR
    #-------------------------------------------------------------------------------------

    # Use local openexr
    maybe(
        native.local_repository,
        name = "openexr",
        path = "../third_party/openexr-4854db9316efe645628928dd378f389e1671bf18",
        #path = "../third_party/openexr-3.1.0",
    )

    # Use local Imath
    maybe(
        native.new_local_repository,
        name = "Imath",
        build_file = "@openexr//:bazel/third_party/Imath.BUILD",  # openexr must be fetched first, since it provides the Imath build instructions
        path = "../third_party/Imath-3.1.5",
    )

    # Use git repository from com_github_vertexwahn_openexr
    #git_repository(
    #    name = "openexr",
    #    branch = "bazel-build-update-imath",
    #    remote = "https://github.com/Vertexwahn/openexr",
    #)

    # Use git repository from com_github_academy_software_foundation_openexr
    #git_repository(
    #    name = "openexr",
    #    commit = "3e7d5ac897ddb9bba6298d1c359966a416248392",
    #    remote = "https://github.com/AcademySoftwareFoundation/openexr/",
    #    #shallow_since = "1625794324 -0700",
    #)

    #-------------------------------------------------------------------------------------
    # Libpng
    #-------------------------------------------------------------------------------------

    maybe(
        native.new_local_repository,
        name = "libpng",
        build_file = "//okapi/third_party:libpng.BUILD",
        path = "../third_party/libpng-1.6.37",
    )

    # libpng
    #maybe(
    #    http_archive,
    #    name = "libpng",
    #    build_file = "//okapi/third_party:libpng.BUILD",
    #    sha256 = "7f415186d38ca71c23058386d7cf5135c8beda821ee1beecdc2a7a26c0356615",
    #    strip_prefix = "libpng-1.2.57",
    #    urls = [
    #        "https://sourceforge.net/projects/libpng/files/latest/download
    #    ],
    #)

    #-------------------------------------------------------------------------------------
    # oneTBB
    #-------------------------------------------------------------------------------------

    maybe(
        native.local_repository,
        name = "oneTBB",
        path = "../third_party/oneTBB-9d2a3477ce276d437bf34b1582781e5b11f9b37a",
    )

    # Use git repository from com_github_oneapi_src_onetbb
    #maybe(
    #    git_repository,
    #    name = "oneTBB",
    #    commit = "6164044548e802b3685ad84ffb04e40747d2ff3b",
    #    remote = "https://github.com/oneapi-src/oneTBB",
    #    shallow_since = "1625731441 +0300",
    #)

    # Use git repository from com_github_vertexwahn_onetbb
    #maybe(
    #    git_repository,
    #    name = "oneTBB",
    #    #branch = "add-bazel-macos-support",
    #    commit = "6e737b879a9406681e48c0fc88d8771221a00d8d",
    #    remote = "https://github.com/Vertexwahn/oneTBB",
    #    shallow_since = "1642798690 +0100",
    #)

    #-------------------------------------------------------------------------------------
    # rules_pkg
    #-------------------------------------------------------------------------------------

    maybe(
        native.local_repository,
        name = "rules_pkg",
        path = "../third_party/rules_pkg-0.6.0",
    )

    #maybe(
    #    http_archive,
    #    name = "rules_pkg",
    #    sha256 = "353b20e8b093d42dd16889c7f918750fb8701c485ac6cceb69a5236500507c27",
    #    urls = [
    #        "https://mirror.bazel.build/github.com/bazelbuild/rules_pkg/releases/download/0.5.0/rules_pkg-0.5.0.tar.gz",
    #        "https://github.com/bazelbuild/rules_pkg/releases/download/0.5.0/rules_pkg-0.5.0.tar.gz",
    #    ],
    #)

    #-------------------------------------------------------------------------------------
    # rules_boost
    #-------------------------------------------------------------------------------------

    # Use git repository from github.com
    #git_repository(
    #    name = "com_github_nelhage_rules_boost",
    #    commit = "5a2bde336dc9089025003826ba39af2a27d8b90e",
    #    remote = "https://github.com/nelhage/rules_boost",
    #    shallow_since = "1625099911 -0700",
    #)

    # Use http archive from github.com
    #maybe(
    #    http_archive,
    #    name = "com_github_nelhage_rules_boost",
    #    sha256 = "ab6d5fff6577c9f92cbef05b0b884f84ba91e5f4b70660f8acee8af690467f8d",
    #    strip_prefix = "rules_boost-5a2bde336dc9089025003826ba39af2a27d8b90e",
    #    urls = ["https://github.com/nelhage/rules_boost/archive/5a2bde336dc9089025003826ba39af2a27d8b90e.zip"],
    #)

    # Use local downloaded boost
    #maybe(
    #    native.new_local_repository,
    #    name = "boost",
    #    build_file = "@com_github_nelhage_rules_boost//:BUILD.boost",
    #    path = "../third_party/boost_1_76_0",
    #)

    #maybe(
    #    git_repository,
    #    name = "com_github_nelhage_rules_boost",
    #    remote = "https://github.com/Vertexwahn/rules_boost",
    #    branch = "introduce-maybe",
    #)

    maybe(
        native.local_repository,
        name = "com_github_nelhage_rules_boost",
        path = "../third_party/rules_boost-32164a62e2472077320f48f52b8077207cd0c9c8",
    )

    #-------------------------------------------------------------------------------------
    # bazel_clang_tidy
    #-------------------------------------------------------------------------------------

    #maybe(
    #    git_repository,
    #    name = "bazel_clang_tidy",
    #   branch = "master",
    #    remote = "https://github.com/erenon/bazel_clang_tidy.git",
    #)

    maybe(
        native.local_repository,
        name = "bazel_clang_tidy",
        path = "../third_party/bazel_clang_tidy-c2fe98cfec0430e78bff4169e9ca0a43123e4c99",
    )

    #-------------------------------------------------------------------------------------
    # LLVM
    #-------------------------------------------------------------------------------------

    maybe(
        native.local_repository,
        name = "com_grail_bazel_toolchain",
        path = "../third_party/bazel-toolchain-9e71d562023dc7994e747110ee1ca345ad6b4413",
    )

