/*
 *  SPDX-FileCopyrightText: 2022 Julian Amann <dev@vertexwahn.de>
 *  SPDX-License-Identifier: Apache-2.0
 */

#pragma once
#ifndef Flatland_Imaging_Image_3a91bcbd_ae4d_4c36_83dd_c06ac570322b_h
#define Flatland_Imaging_Image_3a91bcbd_ae4d_4c36_83dd_c06ac570322b_h

#include "flatland/math/point.h"
#include "flatland/math/vector.h"
#include "flatland/imaging/color.h"

FLATLAND_BEGIN_NAMESPACE

class Image3f  {
public:
    Image3f(const int width, const int height);
    Image3f(const Image3f& src);

    virtual ~Image3f();

    Image3f& operator=(const Image3f& other);

    int width() const;
    int height() const;
    Vector2i size() const;

    float* data() const;

    void set_pixel(int x, int y, const Color3f& color);
    void set_pixel(const Point2i& point, const Color3f& color);

    Color3f get_pixel(int x, int y) const;
    Color3f get_pixel(const Point2i& point) const;

private:
    int width_ = 0;
    int height_ = 0;
    float *data_;
};

class Image4b {
public:
    Image4b(const int width, const int height) : width_(width), height_(height) {
        data_ = new uint8_t[width*height*4];
        for(int i = 0; i < width*height*4; ++i) {
            data_[i] = 0;
        }
    }

    Image4b() {
        delete [] data_;
    }

    const uint8_t* data() const {
        return data_;
    }

    int width() const {
        return width_;
    }

    int height() const {
        return height_;
    }

    Vector2i size() const {
        return Vector2i{width_,height_};
    };

    void set_pixel(const int x, const int y, const Color4b& color) {
        data_[(x+y*width_)*4+0] = color.red();
        data_[(x+y*width_)*4+1] = color.green();
        data_[(x+y*width_)*4+2] = color.blue();
        data_[(x+y*width_)*4+3] = 255;
    }

    Color4b get_pixel(const int x, const int y) const {
        return Color4b{data_[(x+y*width_)*4+0],
                       data_[(x+y*width_)*4+1],
                       data_[(x+y*width_)*4+2],
                       data_[(x+y*width_)*4+3]};
    }

private:
    uint8_t* data_ = nullptr;
    int width_ = 0;
    int height_ = 0;
};

FLATLAND_END_NAMESPACE

#endif // end define Flatland_Imaging_Image_3a91bcbd_ae4d_4c36_83dd_c06ac570322b_h