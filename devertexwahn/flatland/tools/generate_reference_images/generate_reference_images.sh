#!/usr/bin/env bash

#
#   SPDX-FileCopyrightText: Copyright 2022-2023 Julian Amann <dev@vertexwahn.de>
#   SPDX-License-Identifier: Apache-2.0
#

# This script generates all reference images stored in the folder flatland/scenes/reference_images.
# The reference images are used for acceptance tests

set -euo pipefail

cd ~/dev/Piper/devertexwahn

bazel run --config=gcc9 //flatland/cli:flatland.cli -- ~/dev/Piper/devertexwahn/flatland/scenes/ao.flatland.xml
bazel run --config=gcc9 //flatland/cli:flatland.cli -- ~/dev/Piper/devertexwahn/flatland/scenes/bunny/bunny.ao.flatland.xml
bazel run --config=gcc9 //flatland/cli:flatland.cli -- ~/dev/Piper/devertexwahn/flatland/scenes/bunny/bunny.flatland.xml
bazel run --config=gcc9 //flatland/cli:flatland.cli -- ~/dev/Piper/devertexwahn/flatland/scenes/disk.ao.flatland.xml
bazel run --config=gcc9 //flatland/cli:flatland.cli -- ~/dev/Piper/devertexwahn/flatland/scenes/disk.flatland.xml
bazel run --config=gcc9 //flatland/cli:flatland.cli -- ~/dev/Piper/devertexwahn/flatland/scenes/disk_2.flatland.xml
bazel run --config=gcc9 //flatland/cli:flatland.cli -- ~/dev/Piper/devertexwahn/flatland/scenes/disk_grid.flatland.xml
bazel run --config=gcc9 //flatland/cli:flatland.cli -- ~/dev/Piper/devertexwahn/flatland/scenes/mirror.flatland.xml
bazel run --config=gcc9 //flatland/cli:flatland.cli -- ~/dev/Piper/devertexwahn/flatland/scenes/mirrors.flatland.xml
bazel run --config=gcc9 //flatland/cli:flatland.cli -- ~/dev/Piper/devertexwahn/flatland/scenes/rectangle.flatland.xml
bazel run --config=gcc9 //flatland/cli:flatland.cli -- ~/dev/Piper/devertexwahn/flatland/scenes/rectangles.flatland.xml
bazel run --config=gcc9 //flatland/cli:flatland.cli -- ~/dev/Piper/devertexwahn/flatland/scenes/gradient_refractive_index.flatland.xml
bazel run --config=gcc9 //flatland/cli:flatland.cli -- ~/dev/Piper/devertexwahn/flatland/scenes/quadtree/quadtree_scene.flatland.xml
bazel run --config=gcc9 //flatland/cli:flatland.cli -- ~/dev/Piper/devertexwahn/flatland/scenes/quadtree/quadtree_corner_case_scene.flatland.xml

mv flatland/scenes/ao.svg flatland/scenes/reference_images/ao_ref.svg
mv flatland/scenes/bunny/bunny.ao.svg flatland/scenes/reference_images/bunny.ao_ref.svg
mv flatland/scenes/bunny/bunny.svg flatland/scenes/reference_images/bunny_ref.svg
mv flatland/scenes/disk.ao.svg flatland/scenes/reference_images/disk.ao_ref.svg
mv flatland/scenes/disk.svg flatland/scenes/reference_images/disk_ref.svg
mv flatland/scenes/disk_2.svg flatland/scenes/reference_images/disk_2_ref.svg
mv flatland/scenes/disk_grid.svg flatland/scenes/reference_images/disk_grid_ref.svg
mv flatland/scenes/mirror.svg flatland/scenes/reference_images/mirror_ref.svg
mv flatland/scenes/mirrors.svg flatland/scenes/reference_images/mirrors_ref.svg
mv flatland/scenes/rectangle.svg flatland/scenes/reference_images/rectangle_ref.svg
mv flatland/scenes/rectangles.svg flatland/scenes/reference_images/rectangles_ref.svg
mv flatland/scenes/gradient_refractive_index.svg flatland/scenes/reference_images/gradient_refractive_index_ref.svg
mv flatland/scenes/quadtree/quadtree.svg flatland/scenes/reference_images/quadtree_ref.svg
mv flatland/scenes/quadtree/quadtree_corner_case.svg flatland/scenes/reference_images/quadtree_corner_case_ref.svg 
