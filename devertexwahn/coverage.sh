#!/usr/bin/env bash

#
#   SPDX-FileCopyrightText: Copyright 2022-2023 Julian Amann <dev@vertexwahn.de>
#   SPDX-License-Identifier: Apache-2.0
#

set -euo pipefail

# Provide a additional bazel config (bazel remote cache config expected)
if [ "$#" -ne 1 ]; then
    echo "Wrong number of parameters detected"
    echo "Usage: $0 <additional_bazel_config>"
    exit 1
fi

additional_bazel_config=$1

bazel coverage \
--config=gcc11 \
--config=${additional_bazel_config} \
--combined_report=lcov \
-- //...

output_path=$(bazel info output_path)

# Get last line of genhtml output
genhtml "$output_path"/_coverage/_coverage_report.dat -o coverage_report

# extract percentage from genhtml
current_line_coverage=$(genhtml "$output_path"/_coverage/_coverage_report.dat -o coverage_report | grep lines......: | cut -d ' ' -f 4 | tr -d '%')
echo "Current coverage: $current_line_coverage"

old_line_coverage="95.0"
echo "Old coverage: $old_line_coverage"

# if old coverage is higher than current coverage, fail
if (( $(echo "$current_line_coverage < $old_line_coverage"|bc -l) )); then
    echo "Line coverage is lower than expected"
    exit 1
fi

#open coverage_report/index.html
