<!--
SPDX-FileCopyrightText: Copyright 2022-2023 Julian Amann <dev@vertexwahn.de>
SPDX-License-Identifier: Apache-2.0
-->
# Example Scenes

## Refraction

### bunny.flatland.xml

![out](../scenes/reference_images/bunny_ref.svg)

```shell
# GCC 11 on Ubuntu 22.04 - assuming that pwd is devertexwahn 
bazel run --config=gcc11 //flatland/cli:flatland.cli -- --scene_filename=$(pwd)/flatland/scenes/bunny/bunny.flatland.xml
```

### disk.flatland.xml

![out](../scenes/reference_images/disk_ref.svg)

### disk_grid.flatland.xml

![out](../scenes/reference_images/disk_grid_ref.svg)

### rectangle.flatland.xml

![out](../scenes/reference_images/rectangle_ref.svg)

### rectangles.flatland.xml

![out](../scenes/reference_images/rectangles_ref.svg)

### gradient_refractive_index.flatland.xml

![out](../scenes/reference_images/gradient_refractive_index_ref.svg)

## Reflection

### mirror.flatland.xml

![out](../scenes/reference_images/mirror_ref.svg)

### mirrors.flatland.xml

![out](../scenes/reference_images/mirrors_ref.svg)

## Ambient Occlusion

### ao.flatland.xml

![out](../scenes/reference_images/ao_ref.svg)

### bunny.ao.flatland.xml

![out](../scenes/reference_images/bunny.ao_ref.svg)

### disk.ao.flatland.xml

![out](../scenes/reference_images/disk.ao_ref.svg)
