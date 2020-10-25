/*
    This file is part of Flatland, a simple 2D ray tracer

    Copyright (c) 2020, Julian Amann (dev@vertexwahn.de), All rights reserved.
    Distributed under the 3-Clause BSD License software license, see the accompanying
    file LICENSE.md or https://opensource.org/licenses/BSD-3-Clause.
*/

#pragma once
#ifndef Flatland_Film_2bdf700f_45a2_433b_96f4_422b900e7d1a_h
#define Flatland_Film_2bdf700f_45a2_433b_96f4_422b900e7d1a_h

#include "flatland/core/namespace.h"
#include "flatland/math/vector.h"

#include <string>

FLATLAND_BEGIN_NAMESPACE

class Film {
public:
    Film(const int width, const int height, std::string_view filename);

    int getWidth() const;
    int getHeight() const;

    std::string_view getFilename() const;

    Vector2i getSize() const;

private:
    int width_;
    int height_;
    std::string filename_;
};

FLATLAND_END_NAMESPACE

#endif // end define Flatland_Film_2bdf700f_45a2_433b_96f4_422b900e7d1a_h
