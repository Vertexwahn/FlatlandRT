/*
    This file is part of Flatland, a simple 2D ray tracer

    Copyright (c) 2020, Julian Amann (dev@vertexwahn.de), All rights reserved.
    Distributed under the 3-Clause BSD License software license, see the accompanying
    file LICENSE.md or https://opensource.org/licenses/BSD-3-Clause.
*/

#include "film.h"

FLATLAND_BEGIN_NAMESPACE

Film::Film(const int width, const int height, std::string_view filename) : width_(width), height_(height), filename_(filename) {
}

int Film::getWidth() const { return  width_; }

int Film::getHeight() const { return height_; }

std::string_view Film::getFilename() const {
    return filename_;
}

Vector2i Film::getSize() const {
    return Vector2i(width_, height_);
}


FLATLAND_END_NAMESPACE