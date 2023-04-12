resolved = [
     {
          "original_rule_class": "local_repository",
          "original_attributes": {
               "name": "bazel_tools",
               "path": "/usr/local/google/home/aiuto/.cache/bazel/_bazel_aiuto/install/81bcd171fd1c11cda2d49bd0ed3a1af3/embedded_tools"
          },
          "native": "local_repository(name = \"bazel_tools\", path = __embedded_dir__ + \"/\" + \"embedded_tools\")"
     },
     {
          "original_rule_class": "@bazel_tools//tools/build_defs/repo:http.bzl%http_archive",
          "definition_information": "Repository bazel_skylib instantiated at:\n  /usr/local/google/home/aiuto/ws/rules_pkg/WORKSPACE:19:23: in <toplevel>\n  /usr/local/google/home/aiuto/ws/rules_pkg/pkg/deps.bzl:24:10: in rules_pkg_dependencies\n  /usr/local/google/home/aiuto/.cache/bazel/_bazel_aiuto/9ebf740b7a9eebf072781ad1b94f037d/external/bazel_tools/tools/build_defs/repo/utils.bzl:233:18: in maybe\nRepository rule http_archive defined at:\n  /usr/local/google/home/aiuto/.cache/bazel/_bazel_aiuto/9ebf740b7a9eebf072781ad1b94f037d/external/bazel_tools/tools/build_defs/repo/http.bzl:370:31: in <toplevel>\n",
          "original_attributes": {
               "name": "bazel_skylib",
               "generator_name": "bazel_skylib",
               "generator_function": "rules_pkg_dependencies",
               "generator_location": None,
               "urls": [
                    "https://mirror.bazel.build/github.com/bazelbuild/bazel-skylib/releases/download/1.1.1/bazel-skylib-1.1.1.tar.gz",
                    "https://github.com/bazelbuild/bazel-skylib/releases/download/1.1.1/bazel-skylib-1.1.1.tar.gz"
               ],
               "sha256": "c6966ec828da198c5d9adbaa94c05e3a1c7f21bd012a0b29ba8ddbccb2c93b0d"
          },
          "repositories": [
               {
                    "rule_class": "@bazel_tools//tools/build_defs/repo:http.bzl%http_archive",
                    "attributes": {
                         "url": "",
                         "urls": [
                              "https://mirror.bazel.build/github.com/bazelbuild/bazel-skylib/releases/download/1.1.1/bazel-skylib-1.1.1.tar.gz",
                              "https://github.com/bazelbuild/bazel-skylib/releases/download/1.1.1/bazel-skylib-1.1.1.tar.gz"
                         ],
                         "sha256": "c6966ec828da198c5d9adbaa94c05e3a1c7f21bd012a0b29ba8ddbccb2c93b0d",
                         "integrity": "",
                         "netrc": "",
                         "auth_patterns": {},
                         "canonical_id": "",
                         "strip_prefix": "",
                         "add_prefix": "",
                         "type": "",
                         "patches": [],
                         "remote_patches": {},
                         "remote_patch_strip": 0,
                         "patch_tool": "",
                         "patch_args": [
                              "-p0"
                         ],
                         "patch_cmds": [],
                         "patch_cmds_win": [],
                         "build_file_content": "",
                         "workspace_file_content": "",
                         "name": "bazel_skylib"
                    },
                    "output_tree_hash": "ec8087f03267ba09d29db54ca9f5a2227132cd78f8797f746c927d0fef549ac5"
               }
          ]
     },
     {
          "original_rule_class": "@bazel_tools//tools/build_defs/repo:http.bzl%http_archive",
          "definition_information": "Repository bazel_stardoc instantiated at:\n  /usr/local/google/home/aiuto/ws/rules_pkg/WORKSPACE:56:13: in <toplevel>\nRepository rule http_archive defined at:\n  /usr/local/google/home/aiuto/.cache/bazel/_bazel_aiuto/9ebf740b7a9eebf072781ad1b94f037d/external/bazel_tools/tools/build_defs/repo/http.bzl:370:31: in <toplevel>\n",
          "original_attributes": {
               "name": "bazel_stardoc",
               "urls": [
                    "https://mirror.bazel.build/github.com/bazelbuild/stardoc/releases/download/0.5.0/stardoc-0.5.0.tar.gz",
                    "https://github.com/bazelbuild/stardoc/releases/download/0.5.0/stardoc-0.5.0.tar.gz"
               ],
               "sha256": "c9794dcc8026a30ff67cf7cf91ebe245ca294b20b071845d12c192afe243ad72"
          },
          "repositories": [
               {
                    "rule_class": "@bazel_tools//tools/build_defs/repo:http.bzl%http_archive",
                    "attributes": {
                         "url": "",
                         "urls": [
                              "https://mirror.bazel.build/github.com/bazelbuild/stardoc/releases/download/0.5.0/stardoc-0.5.0.tar.gz",
                              "https://github.com/bazelbuild/stardoc/releases/download/0.5.0/stardoc-0.5.0.tar.gz"
                         ],
                         "sha256": "c9794dcc8026a30ff67cf7cf91ebe245ca294b20b071845d12c192afe243ad72",
                         "integrity": "",
                         "netrc": "",
                         "auth_patterns": {},
                         "canonical_id": "",
                         "strip_prefix": "",
                         "add_prefix": "",
                         "type": "",
                         "patches": [],
                         "remote_patches": {},
                         "remote_patch_strip": 0,
                         "patch_tool": "",
                         "patch_args": [
                              "-p0"
                         ],
                         "patch_cmds": [],
                         "patch_cmds_win": [],
                         "build_file_content": "",
                         "workspace_file_content": "",
                         "name": "bazel_stardoc"
                    },
                    "output_tree_hash": "1bbdabaf4f25d900c476ee70ed1bf68cda4b9b29bcecdc12efdc63cb29070e56"
               }
          ]
     },
     {
          "original_rule_class": "local_config_platform",
          "original_attributes": {
               "name": "local_config_platform"
          },
          "native": "local_config_platform(name = 'local_config_platform')"
     },
     {
          "original_rule_class": "@bazel_tools//tools/build_defs/repo:http.bzl%http_archive",
          "definition_information": "Repository platforms instantiated at:\n  /usr/local/google/home/aiuto/ws/rules_pkg/WORKSPACE:31:13: in <toplevel>\nRepository rule http_archive defined at:\n  /usr/local/google/home/aiuto/.cache/bazel/_bazel_aiuto/9ebf740b7a9eebf072781ad1b94f037d/external/bazel_tools/tools/build_defs/repo/http.bzl:370:31: in <toplevel>\n",
          "original_attributes": {
               "name": "platforms",
               "urls": [
                    "https://mirror.bazel.build/github.com/bazelbuild/platforms/releases/download/0.0.5/platforms-0.0.5.tar.gz",
                    "https://github.com/bazelbuild/platforms/releases/download/0.0.5/platforms-0.0.5.tar.gz"
               ],
               "sha256": "379113459b0feaf6bfbb584a91874c065078aa673222846ac765f86661c27407"
          },
          "repositories": [
               {
                    "rule_class": "@bazel_tools//tools/build_defs/repo:http.bzl%http_archive",
                    "attributes": {
                         "url": "",
                         "urls": [
                              "https://mirror.bazel.build/github.com/bazelbuild/platforms/releases/download/0.0.5/platforms-0.0.5.tar.gz",
                              "https://github.com/bazelbuild/platforms/releases/download/0.0.5/platforms-0.0.5.tar.gz"
                         ],
                         "sha256": "379113459b0feaf6bfbb584a91874c065078aa673222846ac765f86661c27407",
                         "integrity": "",
                         "netrc": "",
                         "auth_patterns": {},
                         "canonical_id": "",
                         "strip_prefix": "",
                         "add_prefix": "",
                         "type": "",
                         "patches": [],
                         "remote_patches": {},
                         "remote_patch_strip": 0,
                         "patch_tool": "",
                         "patch_args": [
                              "-p0"
                         ],
                         "patch_cmds": [],
                         "patch_cmds_win": [],
                         "build_file_content": "",
                         "workspace_file_content": "",
                         "name": "platforms"
                    },
                    "output_tree_hash": "44677753c5430f37d1aa7f8fe2fd6ae0179eac8c65865787782910bfbc574615"
               }
          ]
     }
]