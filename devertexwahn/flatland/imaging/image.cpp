/*
 *  SPDX-FileCopyrightText: 2022 Julian Amann <dev@vertexwahn.de>
 *  SPDX-License-Identifier: Apache-2.0
 */

#include "flatland/imaging/image.h"

FLATLAND_BEGIN_NAMESPACE

Image3f::~Image3f() {
    delete[] data_;
}

void Image3f::set_pixel(int x, int y, const Color3f &color) {
    assert(x >= 0);
    assert(y >= 0);
    assert(x < width_);
    assert(y < height_);

    data_[(width_ * y + x) * 3] = color.red();
    data_[(width_ * y + x) * 3 + 1] = color.green();
    data_[(width_ * y + x) * 3 + 2] = color.blue();
}

void Image3f::set_pixel(const Point2i &point, const Color3f &color) {
    assert(!color.has_nans());
    set_pixel(point.x(), point.y(), color);
}

Color3f Image3f::get_pixel(int x, int y) const {
    assert(x >= 0);
    assert(y >= 0);
    assert(x < width_);
    assert(y < height_);

    auto r = data_[(width_ * y + x) * 3];
    auto g = data_[(width_ * y + x) * 3 + 1];
    auto b = data_[(width_ * y + x) * 3 + 2];

    return Color3f{r, g, b};
}

Image3f::Image3f(const int width, const int height) : width_(width), height_(height) {
    data_ = new float[width * height * 3];

    for (int x = 0; x < width; ++x) {
        for (int y = 0; y < height; ++y) {
            auto r = 0.f;
            auto g = 0.f;
            auto b = 0.f;

            data_[(width * y + x) * 3] = r;
            data_[(width * y + x) * 3 + 1] = g;
            data_[(width * y + x) * 3 + 2] = b;
        }
    }
}

Image3f::Image3f(const Image3f &src) : width_(src.width_), height_(src.height_) {
    data_ = new float[width_ * height_ * 3];

    for (int x = 0; x < width_; ++x) {
        for (int y = 0; y < height_; ++y) {
            data_[(width_ * y + x) * 3] = src.data_[(width_ * y + x) * 3];
            data_[(width_ * y + x) * 3 + 1] = src.data_[(width_ * y + x) * 3 + 1];
            data_[(width_ * y + x) * 3 + 2] = src.data_[(width_ * y + x) * 3 + 2];
        }
    }

    //std::copy(std::begin(src.data()), std::end(src.data()[width_ * height_]), std::begin(data_));
}

Image3f &Image3f::operator=(const Image3f &other) {
    Image3f copy{other};

    float *data = copy.data_;
    int width = copy.width_;
    int height = copy.height_;

    copy.data_ = this->data_;
    copy.width_ = width_;
    copy.height_ = height_;

    width_ = width;
    height_ = height;
    data_ = data;

    return *this;
}

int Image3f::width() const {
    return width_;
}

int Image3f::height() const {
    return height_;
}

Vector2i Image3f::size() const {
    return Vector2i(width_, height_);
}

float *Image3f::data() const {
    return data_;
}

Color3f Image3f::get_pixel(const Point2i &point) const {
    return get_pixel(point.x(), point.y());
}

FLATLAND_END_NAMESPACE
