/*
 *  SPDX-FileCopyrightText: Copyright 2022-2025 Julian Amann <dev@vertexwahn.de>
 *  SPDX-License-Identifier: Apache-2.0
 */

#pragma once
#ifndef De_Vertexwahn_Imaging_Image_3a91bcbd_ae4d_4c36_83dd_c06ac570322b_h
#define De_Vertexwahn_Imaging_Image_3a91bcbd_ae4d_4c36_83dd_c06ac570322b_h

#include "imaging/color.hpp"

#include "math/point.hpp"
#include "math/vector.hpp"

#include <memory>

DE_VERTEXWAHN_BEGIN_NAMESPACE

/**
 *  \brief Used to represent images with a specific width and height.
 *
 *  Data storage is assumed to be interleaved (not planar).
 */
template <typename ColorType, typename ColorChannelType, int ColorChannelCount>
class Image {
public:
    /// Create an image of specified size and fill it with black color
    Image(const int width, const int height) : width_(width), height_(height) {
        reserve_memory();
        fill_black();
    }

    /// Create an image of the specified size and fill it with black color
    Image(const Vector2i& size) : width_(size.x()), height_(size.y()) {
        reserve_memory();
        fill_black();
    }

    /// Create an image of specified size and fill it with the specified color
    Image(const int width, const int height, const ColorType& initial_color) :
    width_(width), height_(height) {
        reserve_memory();
        fill(initial_color);
    }

    /// Create an image of specified size and fill it with the specified color
    Image(const int width, const int height, const ColorChannelType* data) :
        width_(width), height_(height) {
        assert(data); // data should be not nullptr

        reserve_memory();
        fill(data);
    }

    Image(const Image& src) {
        width_ = src.width_;
        height_ = src.height_;

        reserve_memory();
        fill(src.data_.get());
    }

    Image& operator=(const Image& src) {
        if(&src == this) {
            return *this;
        }

        width_ = src.width_;
        height_ = src.height_;

        reserve_memory();
        fill(src.data_.get());

        return *this;
    }

    Image(Image&& src) {
        width_ = src.width_;
        height_ = src.height_;
        data_size_ = src.data_size_;
        data_ = std::move(src.data_);
        assert(src.data_ == nullptr); // guaranteed to be nullptr after move
    }

    Image& operator=(Image&& src) noexcept {
        width_ = src.width_;
        height_ = src.height_;
        data_size_ = src.data_size_;
        data_ = std::move(src.data_);
        assert(src.data_ == nullptr); // guaranteed to be nullptr after move
        return *this;
    }

    bool operator==(const Image& other) const {
        if (width_ != other.width_ || height_ != other.height_) {
            return false;
        }

        for (int x = 0; x < this->width_; x++) {
            for (int y = 0; y < this->height_; y++) {
                if (this->get_pixel(x, y) != other.get_pixel(x, y))
                    return false;
            }
        }

        return true;
    }

    bool operator!=(const Image& other) const {
        return !(this->operator==(other));
    }

    virtual ~Image() = default;

    void resize(const int width, const int height) {
        width_ = width;
        height_ = height;

        reserve_memory();
    }

    [[nodiscard]]
    int width() const noexcept {
        return width_;
    }

    [[nodiscard]]
    int height() const noexcept {
        return height_;
    }

    [[nodiscard]]
    Vector2i size() const noexcept {
        return Vector2i{width_, height_};
    };

    [[nodiscard]]
    bool check_bounds(const int x, const int y) const noexcept {
        return x >= 0 && x < width_ && y >= 0 && y < height_;
    }

    // todo: should return const ColorChannelType*
    //const ColorChannelType* data() const {
    [[nodiscard]]
    ColorChannelType* data() const {
        return data_.get();
    }

    [[nodiscard]]
    ColorChannelType* data() {
        return data_.get();
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

    /// Set all pixel of image to black (zero / "0") color
    void fill_black() {
        assert(data_);

        // todo: use memset or something more modern
        //std::fill(data_.get(), data_.get() + width_ * height_ * ColorChannelCount, ColorType{0});

        // make it black
        for (int i = 0; i < data_size_; ++i) {
            data_[i] = 0;
        }
    }

    /*
        Sets the whole image to a given color.
    */
    void fill(const ColorType& color) {
        for (int x = 0; x < width_; x++) {
            for (int y = 0; y < height_; y++) {
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
    [[nodiscard]]
    ColorType get_pixel(const int x, const int y) const {
        assert(check_bounds(x, y));

        ColorType color;
        for(int i = 0; i < color.size(); ++i) {
            color[i] = data_[(x + y * width_) * ColorChannelCount + i];
        }

        return color;
    }

    [[nodiscard]]
    ColorType get_pixel(const Point2i& position) const {
        return get_pixel(position.x(), position.y());
    }

private:
    /**
     *  \brief Reserve memory for the image data according to the current values
     *  of <tt>width_</tt> and <tt>height_</tt>
     */
    void reserve_memory() {
        assert(width_ >= 0); // expecting a non-negative width
        assert(height_ >= 0); // excepting a non-negate height

        // memory size need to be allocated for the image data
        data_size_ = width_ * height_ * ColorChannelCount;
        assert(data_size_ >= 0); // expecting a non-negative size
        data_ = std::make_unique<ColorChannelType[]>(data_size_);

        assert(data_); // data should be not nullptr after allocation
    }



    void fill(const ColorChannelType* data) {
        assert(data); // should not be nullptr
        assert(data_); // should not be nullptr

        // todo use std::copy

        // copy data
        for (int i = 0; i < data_size_; ++i) {
            data_[i] = data[i];
        }
    }
private:
    /// Width of image in pixels
    int width_ = 0;

    /// Height of image in pixels
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
    std::unique_ptr<ColorChannelType[]> data_;

    /**
     * \brief  Size of array referenced by data_.
     *
     * Computed by, i.e. widht_ * height_ * ColorChannelCount
     * Updated by the reserve_memory member function
     */
    int data_size_ = 0; // Size of array stored in data, i.e. widht_ * height_ * ColorChannelCount
};

using Image1b = Image<Color1b, uint8_t, 1>;
using Image3b = Image<ColorRGB3b, uint8_t, 3>;
using Image4b = Image<ColorRGBA4b, uint8_t, 4>;
using Image1f = Image<Color1f, float, 1>;
using Image3f = Image<ColorRGB3f, float, 3>;
using Image4f = Image<ColorRGBA4f, float, 4>;

DE_VERTEXWAHN_END_NAMESPACE

#endif // end define De_Vertexwahn_Imaging_Image_3a91bcbd_ae4d_4c36_83dd_c06ac570322b_h
