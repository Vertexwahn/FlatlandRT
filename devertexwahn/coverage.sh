#!/usr/bin/env bash

#
#   SPDX-FileCopyrightText: Copyright 2022-2023 Julian Amann <dev@vertexwahn.de>
#   SPDX-License-Identifier: Apache-2.0
#

set -euo pipefail

show_html_page=false
additional_bazel_config=""

# Parse arguments
while [[ "$#" -gt 0 ]]; do
    case $1 in
        --show_report|-s) show_html_page=true ;;
        --additional_bazel_config=*) additional_bazel_config="--config=${1#*=}" ;;
        *) echo "Unknown parameter passed: $1"; exit 1 ;;
    esac
    shift
done

# Check if genhtml is installed, if not, install it
if ! command -v genhtml &> /dev/null; then
    echo "genhtml could not be found, installing it now"
    sudo apt-get install lcov
fi

bazel coverage \
--config=gcc13 \
${additional_bazel_config} \
--combined_report=lcov \
-- //...

output_path=$(bazel info output_path)

# Get last line of genhtml output
genhtml --ignore-errors negative "$output_path"/_coverage/_coverage_report.dat -o coverage_report

# extract percentage from genhtml
current_line_coverage=$(genhtml --ignore-errors negative "$output_path"/_coverage/_coverage_report.dat -o coverage_report | grep lines......: | cut -d ' ' -f 4 | tr -d '%')
echo "Current coverage: $current_line_coverage"

old_line_coverage="80.0"
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
