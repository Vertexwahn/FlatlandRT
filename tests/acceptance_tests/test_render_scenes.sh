#!/usr/bin/env bash
#
#   This file is part of Flatland, a simple 2D ray tracer
#
#   Copyright (c) 2020, Julian Amann (dev@vertexwahn.de), All rights reserved.
#   Distributed under the 3-Clause BSD License software license, see the accompanying
#   file LICENSE.md or https://opensource.org/licenses/BSD-3-Clause.
#

check_scene_output()
{
    srcs/flatland.cli/flatland "$1"

    FILE="$2"
    FILE_REF="$3"

    if [ -f "$FILE" ]; then
        echo "$FILE exists."
    else
        echo "$FILE does not exist."
        exit -1
    fi

    if cmp -s "$FILE" "$FILE_REF"; then
        printf 'The file "%s" is the same as "%s"\n' "$FILE" "$FILE_REF"
        # print diff
    else
        printf 'The file "%s" is different from "%s"\n' "$FILE" "$FILE_REF"
        cat $FILE
        echo "Ref: -------------------------"
        cat $FILE_REF
        exit -1
    fi
}

check_scene_output "scenes/bunny.flatland.xml" "scenes/bunny.svg" "scenes/bunny_ref.svg"
check_scene_output "scenes/mirror.flatland.xml" "scenes/mirror.svg" "scenes/mirror_ref.svg"
check_scene_output "scenes/mirrors.flatland.xml" "scenes/mirrors.svg" "scenes/mirrors_ref.svg"
check_scene_output "scenes/rectangles.flatland.xml" "scenes/rectangles.svg" "scenes/rectangles_ref.svg"
