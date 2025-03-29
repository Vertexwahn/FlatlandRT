/*
 *  SPDX-FileCopyrightText: Copyright 2022-2024 Julian Amann <dev@vertexwahn.de>
 *  SPDX-License-Identifier: Apache-2.0
 */

#pragma once
#ifndef De_Vertexwahn_Imaging_Image_3a91bcbd_ae4d_4c36_83dd_c06ac570322b_h
#define De_Vertexwahn_Imaging_Image_3a91bcbd_ae4d_4c36_83dd_c06ac570322b_h

#include "imaging/color.hpp"

#include "math/point.hpp"
#include "math/vector.hpp"

DE_VERTEXWAHN_BEGIN_NAMESPACE

template <typename ColorType, typename ColorChannelType, int ColorChannelCount>
class Image {
public:
    Image(const int width, const int height) : width_(width), height_(height) {
        const int data_size = width_ * height_ * ColorChannelCount;
        data_ = new ColorChannelType[data_size];

        // make it black
        for (int i = 0; i < data_size; ++i) {
            data_[i] = 0;
        }
    }

    Image(const int width, const int height, ColorType initial_color) : width_(width), height_(height) {
        const int data_size = width_ * height_ * ColorChannelCount;
        data_ = new ColorChannelType[data_size];

        for(int i = 0; i < width_ * height_; ++i) {
            for(int k = 0; k < ColorChannelCount; k++) {
                data_[i*ColorChannelCount+k] = initial_color[k];
            }
        }
    }

    Image(const Vector2i& size) : width_(size.x()), height_(size.y()) {
        const int data_size = width_ * height_ * ColorChannelCount;
        data_ = new ColorChannelType[data_size];

        // make it black
        for (int i = 0; i < data_size; ++i) {
            data_[i] = 0;
        }
    }

    Image(const int width, const int height, const ColorChannelType* data) :
        width_(width), height_(height) {
        const int data_size = width_ * height_ * ColorChannelCount;
        data_ = new ColorChannelType[data_size];

        // make it black
        for (int i = 0; i < data_size; ++i) {
            data_[i] = data[i];
        }
    }

    Image(const Image& src) {
        width_ = src.width_;
        height_ = src.height_;
        int size = width_ * height_ * ColorChannelCount;
        data_ = new ColorChannelType[size];

        // todo use std::copy
        for (int i = 0; i < size; ++i) {
            data_[i] = src.data_[i];
        }
    }

    Image& operator=(const Image& src) {
        if(&src == this) {
            return *this;
        }

        width_ = src.width_;
        height_ = src.height_;
        const int data_size = width_ * height_ * ColorChannelCount;

        assert(data_);
        delete [] data_;

        data_ = new ColorChannelType[data_size];

        for (int i = 0; i < data_size; ++i) {
            data_[i] = src.data_[i];
        }

        return *this;
    }

    virtual ~Image() {
        assert(data_);
        delete[] data_;
    }

    void resize(const int width, const int height) {
        if (data_) {
            delete[] data_;
        }

        width_ = width;
        height_ = height;
        data_ = new ColorChannelType[width_ * height_  * ColorChannelCount];
    }

    [[nodiscard]]
    int width() const {
        return width_;
    }

    [[nodiscard]]
    int height() const {
        return height_;
    }

    [[nodiscard]]
    Vector2i size() const {
        return Vector2i{width_, height_};
    };

    [[nodiscard]]
    const bool check_bounds(const int x, const int y) const {
        return x >= 0 && x < width() && y >= 0 && y < height();
    }

    // todo: should return const ColorChannelType*
    //const ColorChannelType* data() const {
    ColorChannelType* data() const {
        return data_;
    }

    ColorChannelType* data() {
        return data_;
    }

    /*
        The following layout is used:
        +-----+-----+-----+
        |(0|0)|(1|0)|(2|0)|
        +-----+-----+-----+
        |(0|1)|(1|1)|(2|1)|
        +-----+-----+-----+
        |(0|2)|(1|2)|(2|2)|
        +-----+-----+-----+
    */
    void set_pixel(const int x, const int y, const ColorType& color) {
        assert(check_bounds(x, y));
        //assert(!color.has_nans()); // works only on floats
        for(int i = 0; i <color.size(); ++i) {
            data_[(x + y * width_) * ColorChannelCount + i] = color[i];
        }
    }

    void set_pixel(const Point2i &point, const ColorType &color) {
        assert(!color.has_nans());
        set_pixel(point.x(), point.y(), color);
    }

    /*
        The following layout/coordinate system is used:
        y+
       /|\
        | +-----+-----+-----+
        | |(0|2)|(1|2)|(2|2)|
        | +-----+-----+-----+
        | |(0|1)|(1|1)|(2|1)|
        | +-----+-----+-----+
        | |(0|0)|(1|0)|(2|0)|
        | +-----+-----+-----+
        +-------------------> x+
    */
    void set_pixel_standard_cartesian(const int x, const int y, const ColorType& color) {
        assert(check_bounds(x, y));
        set_pixel(x, height_ - y - 1, color);
    }

    void fill(const ColorType& color) {
        for (int x = 0; x < this->width(); x++) {
            for (int y = 0; y < this->height(); y++) {
                set_pixel(x, y, color);
            }
        }
    }

    /*
        The following layout/coordinate system is used:
        +-------------------> x+
        | +-----+-----+-----+
        | |(0|0)|(1|0)|(2|0)|
        | +-----+-----+-----+
        | |(0|1)|(1|1)|(2|1)|
        | +-----+-----+-----+
        | |(0|2)|(1|2)|(2|2)|
        | +-----+-----+-----+
        v
        y+
    */
    ColorType get_pixel(const int x, const int y) const {
        assert(check_bounds(x, y));

        ColorType color;
        for(int i = 0; i < color.size(); ++i) {
            color[i] = data_[(x + y * width_) * ColorChannelCount + i];
        }

        return color;
    }

    ColorType get_pixel(const Point2i& position) const {
        return get_pixel(position.x(), position.y());
    }

    bool operator==(const Image& other) const {
        if (this->width() != other.width() || this->height() != other.height()) {
            return false;
        }

        for (int x = 0; x < this->width(); x++) {
            for (int y = 0; y < this->height(); y++) {
                if (this->get_pixel(x, y) != other.get_pixel(x, y))
                    return false;
            }
        }

        return true;
    }

    bool operator!=(const Image& other) const {
        return !(this->operator==(other));
    }

private:
    int width_ = 0;
    int height_ = 0;

    /*
        The following layout/coordinate system is used:
        +-------------------> x+
        | +-----+-----+-----+
        | |(0|0)|(1|0)|(2|0)|
        | +-----+-----+-----+
        | |(0|1)|(1|1)|(2|1)|
        | +-----+-----+-----+
        | |(0|2)|(1|2)|(2|2)|
        | +-----+-----+-----+
        v
        y+
    */
    ColorChannelType* data_ = nullptr;
};

using Image1b = Image<Color1b, uint8_t, 1>;
using Image3b = Image<ColorRGB3b, uint8_t, 3>;
using Image4b = Image<ColorRGBA4b, uint8_t, 4>;
using Image1f = Image<Color1f, float, 1>;
using Image3f = Image<ColorRGB3f, float, 3>;
using Image4f = Image<ColorRGBA4f, float, 4>;

DE_VERTEXWAHN_END_NAMESPACE

#endif // end define De_Vertexwahn_Imaging_Image_3a91bcbd_ae4d_4c36_83dd_c06ac570322b_h
