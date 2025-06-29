load("@rules_cc//cc:cc_binary.bzl", "cc_binary")
load("//rules:diff_test.bzl", "diff_test")
load("//rules:run_binary.bzl", "run_binary")
load("//rules:write_file.bzl", "write_file")

package(
    default_applicable_licenses = ["//:license"],
    default_testonly = 1,
    default_visibility = ["//visibility:private"],
)

diff_test(
    name = "run_script_test",
    file1 = ":run_script.out",
    file2 = ":run_script_expected",
)

# Generate this file with write_file instead of checking it in to the source
# tree. This ensures line endings are consistent across "run_script.expected"
# and "run_script.out".
write_file(
    name = "run_script_expected",
    out = "run_script.expected",
    content = [
        "arg1=(foo)",
        "arg2=(bar)",
        "ENV_LOCATION=(a tests/run_binary/BUILD)",
        "ENV_LOCATIONS=(b\\ tests/run_binary/BUILD tests/run_binary/printargs.cc)",
        "ENV_EXECPATH=(a tests/run_binary/BUILD)",
        "ENV_EXECPATHS=(b\\ tests/run_binary/BUILD tests/run_binary/printargs.cc)",
        "ENV_COMPLEX=(xx/yy \\\"zz)",
        "ENV_PATH_BASH=($PATH)",
        "ENV_PATH_CMD=(%PATH%)",
        # Can't prevent "echo" from adding a newline on Windows, so let's add
        # one to the expected output too.
        "",
    ],
)

run_binary(
    name = "run_script",
    srcs = [
        "BUILD",
        ":dummy_srcs",
    ],
    outs = ["run_script.out"],
    # Not testing any complex arguments here, because Windows .bat file argument
    # escaping is different from most MSVC-built Windows binaries. We test
    # argument escaping in "run_bin".
    args = [
        "foo",
        "bar",
    ],
    # Test complex environment variables. They are location-expanded but not
    # Bash-tokenized, and should appear the same for Windows .bat files and Bash
    # .sh scripts.
    env = {
        "ENV_COMPLEX": "xx/yy \\\"zz",
        "ENV_EXECPATH": "a $(execpath BUILD)",
        "ENV_EXECPATHS": "b\\ $(execpaths :dummy_srcs)",
        # Testing $(location) expansion only on source files so the result is
        # predictable. The path of generated files depends on the target
        # platform.
        "ENV_LOCATION": "a $(location BUILD)",
        "ENV_LOCATIONS": "b\\ $(locations :dummy_srcs)",
        "ENV_PATH_BASH": "$PATH",
        "ENV_PATH_CMD": "%PATH%",
        "OUT": "$(location run_script.out)",
    },
    tool = ":script",
)

write_file(
    name = "script",
    # On Windows we need the ".bat" extension.
    # On other platforms the extension doesn't matter.
    # Therefore we can use ".bat" on every platform.
    out = "script.bat",
    content = select({
        "@platforms//os:windows": [
            "@echo>%OUT% arg1=(%1)",
            "@echo>>%OUT% arg2=(%2)",
            "@echo>>%OUT% ENV_LOCATION=(%ENV_LOCATION%)",
            "@echo>>%OUT% ENV_LOCATIONS=(%ENV_LOCATIONS%)",
            "@echo>>%OUT% ENV_EXECPATH=(%ENV_EXECPATH%)",
            "@echo>>%OUT% ENV_EXECPATHS=(%ENV_EXECPATHS%)",
            "@echo>>%OUT% ENV_COMPLEX=(%ENV_COMPLEX%)",
            "@echo>>%OUT% ENV_PATH_BASH=(%ENV_PATH_BASH%)",
            "@echo>>%OUT% ENV_PATH_CMD=(%ENV_PATH_CMD%)",
        ],
        "//conditions:default": [
            "#!/usr/bin/env bash",
            "echo > \"$OUT\" \"arg1=($1)\"",
            "echo >> \"$OUT\" \"arg2=($2)\"",
            "echo >> \"$OUT\" \"ENV_LOCATION=($ENV_LOCATION)\"",
            "echo >> \"$OUT\" \"ENV_LOCATIONS=($ENV_LOCATIONS)\"",
            "echo >> \"$OUT\" \"ENV_EXECPATH=($ENV_EXECPATH)\"",
            "echo >> \"$OUT\" \"ENV_EXECPATHS=($ENV_EXECPATHS)\"",
            "echo >> \"$OUT\" \"ENV_COMPLEX=($ENV_COMPLEX)\"",
            "echo >> \"$OUT\" \"ENV_PATH_BASH=($ENV_PATH_BASH)\"",
            "echo >> \"$OUT\" \"ENV_PATH_CMD=($ENV_PATH_CMD)\"",
        ],
    }),
    is_executable = True,
)

diff_test(
    name = "run_bin_test",
    file1 = ":run_bin.out",
    file2 = ":run_bin_expected",
)

# Generate this file with write_file instead of checking it in to the source
# tree. This ensures line endings are consistent across "run_bin.expected"
# and "run_bin.out".
write_file(
    name = "run_bin_expected",
    out = "run_bin.expected",
    content = [
        "arg1=(a b)",
        "arg2=(\"c d\")",
        "arg3=(e\\ f)",
        "arg4=(xx/yy\\ \\\"zz)",
        "arg5=(tests/run_binary/BUILD)",
        "arg6=(tests/run_binary/BUILD tests/run_binary/printargs.cc)",
        "arg7=('tests/run_binary/BUILD $tests/run_binary/BUILD')",
        "arg8=(tests/run_binary/BUILD)",
        "arg9=(tests/run_binary/BUILD tests/run_binary/printargs.cc)",
        "arg10=('tests/run_binary/BUILD $tests/run_binary/BUILD')",
        "arg11=($PATH)",
        "arg12=($$PATH)",
        "arg13=(${PATH})",
        "arg14=($(echo hello))",
        # Add trailing newline, as printed by printargs.
        "",
    ],
)

run_binary(
    name = "run_bin",
    srcs = [
        "BUILD",
        ":dummy_srcs",
    ],
    outs = ["run_bin.out"],
    # Test complex arguments here. They are location-expanded but not
    # Bash-tokenized, and should appear the same on every platform.
    args = [
        "a b",
        "\"c d\"",
        "e\\ f",
        "xx/yy\\ \\\"zz",
        # Testing $(execpath) expansion only on source files so the result is
        # predictable. The path of generated files depends on the target
        # platform.
        "$(execpath BUILD)",
        "$(execpaths :dummy_srcs)",
        "'$(execpath BUILD) $$(execpath BUILD)'",
        # Test the legacy 'location' expansions
        "$(location BUILD)",
        "$(locations :dummy_srcs)",
        "'$(location BUILD) $$(location BUILD)'",
        "$PATH",
        "$$PATH",
        "${PATH}",
        "$(echo hello)",
    ],
    # Not testing any complex envvars here, because we already did in
    # "run_script".
    env = {"OUT": "$(location run_bin.out)"},
    tool = ":printargs",
)

filegroup(
    name = "dummy_srcs",
    srcs = [
        "BUILD",
        "printargs.cc",
    ],
)

cc_binary(
    name = "printargs",
    srcs = ["printargs.cc"],
)
