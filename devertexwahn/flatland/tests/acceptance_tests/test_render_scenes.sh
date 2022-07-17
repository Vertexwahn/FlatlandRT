#!/usr/bin/env bash

#
#   SPDX-FileCopyrightText: 2022 Julian Amann <dev@vertexwahn.de>
#   SPDX-License-Identifier: Apache-2.0
#

check_flatland_scene_output() {
    flatland/cli/flatland.cli --scene_filename="$1"

    FILE="$2"
    FILE_REF="$3"

    if [ -f "$FILE" ]; then
        echo "$FILE exists."
    else
        echo "FAILURE: $FILE does not exist."
        exit 1
    fi

    if cmp -s "$FILE" "$FILE_REF"; then
        printf 'The file "%s" is the same as "%s"\n' "$FILE" "$FILE_REF"
    else
        printf 'FAILURE: The file "%s" is different from "%s"\n' "$FILE" "$FILE_REF"
        #cat $FILE
        #echo ""
        #echo "Ref: -------------------------"
        #echo ""
        #cat $FILE_REF
        echo ""
        echo "Diff: ------------------------"
        echo ""
        diff  "$FILE" "$FILE_REF"
        echo ""
        exit 1
    fi
}

check_flatland_scene_output "flatland/scenes/ao.flatland.xml" "flatland/scenes/ao.svg" "flatland/scenes/reference_images/ao_ref.svg"
check_flatland_scene_output "flatland/scenes/bunny/bunny.ao.flatland.xml" "flatland/scenes/bunny/bunny.ao.svg" "flatland/scenes/reference_images/bunny.ao_ref.svg"
check_flatland_scene_output "flatland/scenes/bunny/bunny.flatland.xml" "flatland/scenes/bunny/bunny.svg" "flatland/scenes/reference_images/bunny_ref.svg"
check_flatland_scene_output "flatland/scenes/disk.ao.flatland.xml" "flatland/scenes/disk.ao.svg" "flatland/scenes/reference_images/disk.ao_ref.svg"
check_flatland_scene_output "flatland/scenes/disk.flatland.xml" "flatland/scenes/disk.svg" "flatland/scenes/reference_images/disk_ref.svg"
check_flatland_scene_output "flatland/scenes/disk_2.flatland.xml" "flatland/scenes/disk_2.svg" "flatland/scenes/reference_images/disk_2_ref.svg"
check_flatland_scene_output "flatland/scenes/disk_grid.flatland.xml" "flatland/scenes/disk_grid.svg" "flatland/scenes/reference_images/disk_grid_ref.svg"
check_flatland_scene_output "flatland/scenes/mirror.flatland.xml" "flatland/scenes/mirror.svg" "flatland/scenes/reference_images/mirror_ref.svg"
check_flatland_scene_output "flatland/scenes/mirrors.flatland.xml" "flatland/scenes/mirrors.svg" "flatland/scenes/reference_images/mirrors_ref.svg"
check_flatland_scene_output "flatland/scenes/rectangle.flatland.xml" "flatland/scenes/rectangle.svg" "flatland/scenes/reference_images/rectangle_ref.svg"
check_flatland_scene_output "flatland/scenes/rectangles.flatland.xml" "flatland/scenes/rectangles.svg" "flatland/scenes/reference_images/rectangles_ref.svg"
check_flatland_scene_output "flatland/scenes/gradient_refractive_index.flatland.xml" "flatland/scenes/gradient_refractive_index.svg" "flatland/scenes/reference_images/gradient_refractive_index_ref.svg"
check_flatland_scene_output "flatland/scenes/quadtree/quadtree_scene.flatland.xml" "flatland/scenes/quadtree/quadtree.svg" "flatland/scenes/reference_images/quadtree_ref.svg"
check_flatland_scene_output "flatland/scenes/quadtree/quadtree_corner_case_scene.flatland.xml" "flatland/scenes/quadtree/quadtree_corner_case.svg" "flatland/scenes/reference_images/quadtree_corner_case_ref.svg"