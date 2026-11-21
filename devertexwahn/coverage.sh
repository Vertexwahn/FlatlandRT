#!/usr/bin/env bash

#
#   SPDX-FileCopyrightText: Copyright 2022-2023 Julian Amann <dev@vertexwahn.de>
#   SPDX-License-Identifier: Apache-2.0
#

set -euo pipefail

show_html_page=false
additional_bazel_config=""

usage() {
    cat <<EOF
Usage: $(basename "$0") [options]

Options:
  -s, --show_report                    Open the generated HTML coverage report
                                       when finished.
  --additional_bazel_config=<config>   Forward an extra '--config=<config>' to
                                       the underlying 'bazel coverage'
                                       invocation (e.g. homelab_remote_cache).
  -h, --help                           Show this help message and exit.

Examples:
  $(basename "$0")
  $(basename "$0") --show_report
  $(basename "$0") --additional_bazel_config=homelab_remote_cache
  $(basename "$0") --additional_bazel_config=homelab_remote_cache --show_report
EOF
}

# Parse arguments
while [[ "$#" -gt 0 ]]; do
    case $1 in
        -h|--help) usage; exit 0 ;;
        --show_report|-s) show_html_page=true ;;
        --additional_bazel_config=*) additional_bazel_config="--config=${1#*=}" ;;
        *)
            echo "Unknown parameter passed: $1" >&2
            echo >&2
            usage >&2
            exit 1
            ;;
    esac
    shift
done

# Check if genhtml is installed, if not, install it
if ! command -v genhtml &> /dev/null; then
    echo "genhtml could not be found, installing it now"
    sudo apt-get install lcov
fi

# NOTE: protobuf's port_def.inc unconditionally enables `[[clang::musttail]]`
# when the compiler advertises support for the attribute. With GCC, the
# attribute requires at least `-O1`; otherwise the compiler aborts with
# "error: cannot tail-call: ...". Coverage builds default to `-c fastbuild`
# (no optimization), which triggers exactly that failure when protobuf has to
# be rebuilt from source (e.g. when switching remote caches such as
# `--config=homelab_remote_cache`). Force a minimum of `-O1` for the
# protobuf sources so the tail-call attribute is honored.
bazel coverage \
--config=gcc15 \
${additional_bazel_config} \
--per_file_copt=external/protobuf\\+/.*@-O1 \
--combined_report=lcov \
-- //...

output_path=$(bazel info output_path)

# `--ignore-errors` flags:
#  * negative      - lcov sometimes records negative hit counts when merging
#                    coverage from header files included in multiple TUs.
#  * inconsistent  - GCC/gcov occasionally emits records where a line is
#                    reported as hit while the enclosing function is not.
#  * corrupt       - downgrade the resulting trace-file load failure that
#                    would otherwise be raised because of `inconsistent`.
#  * unsupported   - silence the GCC<9 function-end-line warning.
GENHTML_IGNORE_ERRORS="negative,inconsistent,corrupt,unsupported"

# Get last line of genhtml output
genhtml --ignore-errors "$GENHTML_IGNORE_ERRORS" "$output_path"/_coverage/_coverage_report.dat -o coverage_report

# extract percentage from genhtml
current_line_coverage=$(genhtml --ignore-errors "$GENHTML_IGNORE_ERRORS" "$output_path"/_coverage/_coverage_report.dat -o coverage_report | grep -E '^[[:space:]]*lines[.]+:' | awk '{print $2}' | tr -d '%')
echo "Current coverage: $current_line_coverage"

old_line_coverage="75.0"
echo "Old coverage: $old_line_coverage"

# Open coverage report if flag is set
if [ "$show_html_page" = true ]; then
    open coverage_report/index.html
fi

# if old coverage is higher than current coverage, fail
if (( $(echo "$current_line_coverage < $old_line_coverage"|bc -l) )); then
    echo "Line coverage is lower than expected"
    exit 1
fi
