/*
 *  SPDX-FileCopyrightText: Copyright 2022-2023 Julian Amann <dev@vertexwahn.de>
 *  SPDX-License-Identifier: Apache-2.0
 */

#include "imaging/iqa/iqa.h"

#include <cassert>

DE_VERTEXWAHN_BEGIN_NAMESPACE

bool are_equal(const Image3f &a, const Image3f &b) {
    for (int y = 0; y < a.height(); ++y) {
        for (int x = 0; x < a.width(); ++x) {
            Color3f c = a.get_pixel(x, y);
            Color3f ref_c = b.get_pixel(x, y);

            if (c != ref_c) {
                return false;
            }
        }
    }

    return true;
}

// todo: handle this via operator==
bool are_equal(const Image3b &a, const Image3b &b) {
    for (int y = 0; y < a.height(); ++y) {
        for (int x = 0; x < a.width(); ++x) {
            Color3b c = a.get_pixel(x, y);
            Color3b ref_c = b.get_pixel(x, y);

            if (c != ref_c) {
                return false;
            }
        }
    }

    return true;
}

double mean_squared_error(const Image3f &a, const Image3f &b) {
    assert(a.width() == b.width() && "Size does not match.");
    assert(a.height() == b.height() && "Size does not match.");

    double sum = 0;

    double size = a.width() * a.height();

    for (int y = 0; y < a.height(); ++y) {
        for (int x = 0; x < a.width(); ++x) {
            double error_red = a.get_pixel(x, y).red() - b.get_pixel(x, y).red();
            double error_green = a.get_pixel(x, y).green() - b.get_pixel(x, y).green();
            double error_blue = a.get_pixel(x, y).blue() - b.get_pixel(x, y).blue();

            sum += error_red * error_red + error_green * error_green + error_blue * error_blue;
        }
    }

    return sum / size / 3.0;
}

double mean_squared_error(ReferenceCounted<Image3f> a, ReferenceCounted<Image3f> b) {
    return mean_squared_error(*a.get(), *b.get());
}

DE_VERTEXWAHN_END_NAMESPACE
