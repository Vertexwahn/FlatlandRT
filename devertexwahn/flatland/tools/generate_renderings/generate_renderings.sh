#!/usr/bin/env bash

#
#   SPDX-FileCopyrightText: 2022 Julian Amann <dev@vertexwahn.de>
#   SPDX-License-Identifier: Apache-2.0
#

cd ~/dev/Piper/devertexwahn

bazel run --config=gcc9 //flatland/cli:flatland.cli -- ~/dev/Piper/devertexwahn/flatland/scenes/ao.flatland.xml
bazel run --config=gcc9 //flatland/cli:flatland.cli -- ~/dev/Piper/devertexwahn/flatland/scenes/bunny.flatland.xml
bazel run --config=gcc9 //flatland/cli:flatland.cli -- ~/dev/Piper/devertexwahn/flatland/scenes/disk.flatland.xml
bazel run --config=gcc9 //flatland/cli:flatland.cli -- ~/dev/Piper/devertexwahn/flatland/scenes/disk.ao.flatland.xml
bazel run --config=gcc9 //flatland/cli:flatland.cli -- ~/dev/Piper/devertexwahn/flatland/scenes/gradient_refractive_index.flatland.xml
bazel run --config=gcc9 //flatland/cli:flatland.cli -- ~/dev/Piper/devertexwahn/flatland/scenes/mirror.flatland.xml
bazel run --config=gcc9 //flatland/cli:flatland.cli -- ~/dev/Piper/devertexwahn/flatland/scenes/mirrors.flatland.xml
bazel run --config=gcc9 //flatland/cli:flatland.cli -- ~/dev/Piper/devertexwahn/flatland/scenes/rectangle.flatland.xml
bazel run --config=gcc9 //flatland/cli:flatland.cli -- ~/dev/Piper/devertexwahn/flatland/scenes/rectangles.flatland.xml