<!--
SPDX-FileCopyrightText: Copyright 2022-2023 Julian Amann <dev@vertexwahn.de>
SPDX-License-Identifier: Apache-2.0
-->
# FlatlandRT

## User manual

### About

FlatlandRT is a 2D toy ray tracer. It expects a scene file as an input and generates a Scalable Vector Graphics (SVG) as an output.

Given this scene file:

```xml
<?xml version="1.0" encoding="UTF-8"?>
<scene>
    <sensor>
        <film width="800" height="600" filename="bunny.svg"></film>

        <transform>
            <lookat origin="200, 500" target="400, 300" />
        </transform>
    </sensor>

    <integrator type="path_specular_transmission">
        <integer name="max_depth" value="7"/>
    </integrator>

    <shape type="polygon">
        <transform>
            <!-- <rotate_z angle="30" /> -->
            <scale x="3" y="3" />
            <translate x="400" y="300" />
        </transform>

        <string name="filename" value="bunny_coords.ply"/>

        <bsdf type="svg_material">
            <rgb name="stroke_color" value="0.0, 0.682, 0.937" />
            <float name="stroke_width" value="3.0" />
            <rgb name="fill_color" value="1.0, 1.0, 1.0" />
            <string name="interface_interaction" value= "specular_transmission" />
        </bsdf>
    </shape>
</scene>
```

This output is generated:

![out](../scenes/reference_images/bunny_ref.svg)

### Command-Line Reference

```shell
flatland <filename.scene.xml>
```

### Scene file format

The scene file format is heavily influenced by the scene format used by [Mitsuba 2](https://mitsuba2.readthedocs.io/en/latest/src/getting_started/file_format.html) and [Nori](https://github.com/wjakob/nori).
