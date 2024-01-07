"""
    SPDX-FileCopyrightText: Copyright 2022-2023 Julian Amann <dev@vertexwahn.de>
    SPDX-License-Identifier: Apache-2.0
"""

load("@bazel_tools//tools/build_defs/repo:git.bzl", "git_repository")
load("@bazel_tools//tools/build_defs/repo:http.bzl", "http_archive")
load("@bazel_tools//tools/build_defs/repo:utils.bzl", "maybe")

def okapi_deps():
    """Fetch external dependencies for Okapi."""

    #-------------------------------------------------------------------------------------
    # GoogleTest
    #-------------------------------------------------------------------------------------

    # Use local repository
    maybe(
        native.local_repository,
        name = "googletest",
        path = "../third_party/googletest",
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
        path = "../third_party/gflags",
    )

    # Use local repository
    maybe(
        native.local_repository,
        name = "glog",
        path = "../third_party/glog",
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
        path = "../third_party/Catch2",
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
        path = "../third_party/fmt",
    )

    # Live at Head
    #maybe(
    #    git_repository,
    #    name = "fmt",
    #    branch = "master",
    #    remote = "https://github.com/fmtlib/fmt",
    #    patch_cmds = [
    #        "mv support/bazel/.bazelversion .bazelversion",
    #        "mv support/bazel/BUILD.bazel BUILD.bazel",
    #        "mv support/bazel/WORKSPACE.bazel WORKSPACE.bazel",
    #    ],
    #    # Windows-related patch commands are only needed in the case MSYS2 is not installed.
    #    # More details about the installation process of MSYS2 on Windows systems can be found here:
    #    # https://docs.bazel.build/versions/main/install-windows.html#installing-compilers-and-language-runtimes
    #    # Even if MSYS2 is installed the Windows related patch commands can still be used.
    #    patch_cmds_win = [
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
        path = "../third_party/hypothesis",
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
    #maybe(
    #    native.new_local_repository,
    #    name = "pugixml",
    #    build_file = "//okapi/third_party:pugixml.BUILD",
    #    #path = "../third_party/pugixml-1.13",
    #    path = "../third_party/pugixml",
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
        path = "../third_party/pcg-cpp",
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
    # libjpeg-turbo
    #-------------------------------------------------------------------------------------

    #maybe(
    #    http_archive,
    #    name = "nasm",
    #    urls = [
    #        "https://storage.googleapis.com/mirror.tensorflow.org/www.nasm.us/pub/nasm/releasebuilds/2.14.02/nasm-2.14.02.tar.bz2",
    #        "http://pkgs.fedoraproject.org/repo/pkgs/nasm/nasm-2.14.02.tar.bz2/sha512/d7a6b4cee8dfd603d8d4c976e5287b5cc542fa0b466ff989b743276a6e28114e64289bf02a7819eca63142a5278aa6eed57773007e5f589e15768e6456a8919d/nasm-2.14.02.tar.bz2",
    #       "http://www.nasm.us/pub/nasm/releasebuilds/2.14.02/nasm-2.14.02.tar.bz2",
    #    ],
    #   sha256 = "34fd26c70a277a9fdd54cb5ecf389badedaf48047b269d1008fbc819b24e80bc",
    #    strip_prefix = "nasm-2.14.02",
    #    build_file = "//okapi/third_party:nasm.BUILD",
    #)

    #maybe(
    #    native.new_local_repository,
    #    name = "nasm",
    #    build_file = "//okapi/third_party:nasm.BUILD",
    #    path = "../third_party/nasm-2.14.02",
    #)

    #maybe(
    #    native.new_local_repository,
    #    name = "libjpeg_turbo",
    #    build_file = "//okapi/third_party:libjpeg_turbo.BUILD",
    #    path = "../third_party/libjpeg-turbo-2.1.4",
    #)

    #-------------------------------------------------------------------------------------
    # oneTBB
    #-------------------------------------------------------------------------------------

    maybe(
        native.local_repository,
        name = "oneTBB",
        path = "../third_party/oneTBB",
    )

    #maybe(
    #    native.local_repository,
    #    name = "oneTBB",
    #    path = "../third_party/oneTBB-85792ca411c3faaf6494c761b0fc575dd0168304",
    #)

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
        name = "openssl",
        path = "../third_party/boringssl",
    )

    # Switch back to this once above thing is tested:
    maybe(
        native.new_local_repository,
        name = "org_lzma_lzma",
        build_file = "@com_github_nelhage_rules_boost//:lzma.BUILD",
        path = "../third_party/xz-5.4.2",
    )

    maybe(
        native.new_local_repository,
        name = "org_bzip_bzip2",
        build_file = "@com_github_nelhage_rules_boost//:bzip2.BUILD",
        path = "../third_party/bzip2-1.0.8",
    )

    maybe(
        native.local_repository,
        name = "com_github_nelhage_rules_boost",
        path = "../third_party/rules_boost",
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

    #maybe(
    #    git_repository,
    #    name = "com_grail_bazel_toolchain",
    #    remote = "https://github.com/Vertexwahn/bazel-toolchain",
    #    branch = "clang-15-support",
    #)

    #maybe(
    #    native.local_repository,
    #    name = "com_grail_bazel_toolchain",
    #    path = "../third_party/bazel-toolchain",
    #)

    maybe(
        native.local_repository,
        name = "com_grail_bazel_toolchain",
        path = "../third_party/bazel-toolchain-0.8",
    )

    #-------------------------------------------------------------------------------------
    # abseil-cpp
    #-------------------------------------------------------------------------------------

    maybe(
        native.local_repository,
        name = "com_google_absl",
        path = "../third_party/abseil-cpp",
    )

    #-------------------------------------------------------------------------------------
    # xtensor
    #-------------------------------------------------------------------------------------

    maybe(
        native.new_local_repository,
        name = "xtl",
        path = "../third_party/xtl",
        build_file = "//okapi/third_party:xtl.BUILD",
    )

    #maybe(
    #    http_archive,
    #    name = "xtl",
    #    urls = ["https://github.com/xtensor-stack/xtl/archive/e697c91e2a3ac571d120d2b093fb3b250d060a7d.zip"],
    #    strip_prefix = "xtl-e697c91e2a3ac571d120d2b093fb3b250d060a7d",
    #    build_file = "//okapi/third_party:xtl.BUILD",
    #)

    maybe(
        native.new_local_repository,
        name = "xtensor",
        path = "../third_party/xtensor",
        build_file = "//okapi/third_party:xtensor.BUILD",
    )

    #maybe(
    #    http_archive,
    #    name = "xtensor",
    #    urls = ["https://github.com/xtensor-stack/xtensor/archive/275a4efb0faaa78b3b2132ea001d9167d4382109.zip"],
    #    strip_prefix = "xtensor-275a4efb0faaa78b3b2132ea001d9167d4382109",
    #    build_file = "//okapi/third_party:xtensor.BUILD",
    #)


    #-------------------------------------------------------------------------------------
    # tinyobjloader
    #-------------------------------------------------------------------------------------

    maybe(
        native.new_local_repository,
        name = "tinyobjloader",
        build_file = "//okapi/third_party:tinyobjloader.BUILD",
        path = "../third_party/tinyobjloader",
    )

    # From GitHub
    #http_archive(
    #    name = "tinyobjloader",
    #    build_file = "//third_party:tinyobjloader.BUILD",
    #    sha256 = "4040bcd3029bfb37042a2bfe20248c369ab5a9c803e18a31476fee1e303ad579",
    #    strip_prefix = "tinyobjloader-ee45fb41db95bf9563f2a41bc63adfa18475c2ee",
    #    urls = ["https://github.com/tinyobjloader/tinyobjloader/archive/ee45fb41db95bf9563f2a41bc63adfa18475c2ee.zip"],
    #)


