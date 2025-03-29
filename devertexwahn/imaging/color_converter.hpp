/*
 *  Note: This file contains code derived and modified from BlueFramework which is under Apache-2.0 license and
 *  copyrighted by Technical University of Munich, Chair of Computational Modeling and Simulation
 *  Modifications are copyright by Julian Amann and also published under Apache-2.0 license
 */

#pragma once

#include "imaging/color.hpp"

#include "math/vector.hpp"

#include "core/namespace.hpp"

DE_VERTEXWAHN_BEGIN_NAMESPACE

template<typename T>
struct convertTo_ {
    static T apply(const ColorRGB3b& c) {
        return T(c[0], c[1], c[2]);
    }
};

template<>
struct convertTo_<Color1b> {
    static Color1b apply(const Color1b& c) {
        return c;
    }

    static Color1b apply(const ColorRGB3b& c) {
        return Color1b(c.red());
    }

    static Color1b apply(const ColorRGB3f& c) {
        return Color1b(static_cast<std::uint8_t>(std::min(c.red() * 255.0f, 255.0f)));
    }

    static Color1b apply(const ColorRGBA4f& c) {
        return Color1b(static_cast<std::uint8_t>(std::min(c.red() * 255.0f, 255.0f)));
    }

    static Color1b apply(const ColorRGBA4b& c) {
        std::uint8_t r = c.red();

        return Color1b(r);
    }
};

template<>
struct convertTo_<ColorRGB3b> {
    static ColorRGB3b apply(const Color1b& c) {
        // throw away alpha channel
        std::uint8_t r = c.grayvalue();
        std::uint8_t g = c.grayvalue();
        std::uint8_t b = c.grayvalue();

        return ColorRGB3b( r, g, b );
    }

    static ColorRGB3b apply(const ColorRGB3b& c) {
        return c;
    }

    static ColorRGB3b apply(const ColorRGBA4b& c) {
        // throw away alpha channel
        std::uint8_t r = c.red();
        std::uint8_t g = c.green();
        std::uint8_t b = c.blue();

        return ColorRGB3b( r, g, b );
    }

    static ColorRGB3b apply(const Color1f& c) {
        std::uint8_t grayvalue = static_cast<std::uint8_t>(std::min(c[0] * 255.0f, 255.0f));

        return ColorRGB3b( grayvalue, grayvalue, grayvalue );
    }

    static ColorRGB3b apply(const ColorRGB3f& c) {
        std::uint8_t r = static_cast<std::uint8_t>(std::min(c[0] * 255.0f, 255.0f));
        std::uint8_t g = static_cast<std::uint8_t>(std::min(c[1] * 255.0f, 255.0f));
        std::uint8_t b = static_cast<std::uint8_t>(std::min(c[2] * 255.0f, 255.0f));

        return ColorRGB3b( r, g, b );
    }

    static ColorRGB3b apply(const ColorRGBA4f& c) {
        // throw away alpha channel
        std::uint8_t r = static_cast<std::uint8_t>(std::min(c[0] * 255.0f, 255.0f));
        std::uint8_t g = static_cast<std::uint8_t>(std::min(c[1] * 255.0f, 255.0f));
        std::uint8_t b = static_cast<std::uint8_t>(std::min(c[2] * 255.0f, 255.0f));

        return ColorRGB3b( r, g, b );
    }
};

template<>
struct convertTo_<ColorRGBA4b> {
    static ColorRGBA4b apply(const Vector4f& c) {
        std::uint8_t r = static_cast<std::uint8_t>(std::min(c.x() * 255.0f, 255.0f));
        std::uint8_t g = static_cast<std::uint8_t>(std::min(c.y() * 255.0f, 255.0f));
        std::uint8_t b = static_cast<std::uint8_t>(std::min(c.z() * 255.0f, 255.0f));
        std::uint8_t a = static_cast<std::uint8_t>(std::min(c.w() * 255.0f, 255.0f));

        return ColorRGBA4b( r, g, b, a );
    }

    static ColorRGBA4b apply(const Color1b& c) {
        std::uint8_t r = c.grayvalue();
        std::uint8_t g = c.grayvalue();
        std::uint8_t b = c.grayvalue();
        std::uint8_t a = 255;

        return ColorRGBA4b( r, g, b, a );
    }

    static ColorRGBA4b apply(const ColorRGB3b& c) {
        std::uint8_t r = c.red();
        std::uint8_t g = c.green();
        std::uint8_t b = c.blue();
        std::uint8_t a = 255;

        return ColorRGBA4b( r, g, b, a );
    }

    static ColorRGBA4b apply(const ColorRGBA4b& c) {
        return c;
    }

    static ColorRGBA4b apply(const Color1f& c) {
        std::uint8_t r = static_cast<std::uint8_t>(std::min(c[0] * 255.0f, 255.0f));
        std::uint8_t g = static_cast<std::uint8_t>(std::min(c[0] * 255.0f, 255.0f));
        std::uint8_t b = static_cast<std::uint8_t>(std::min(c[0] * 255.0f, 255.0f));
        std::uint8_t a = static_cast<std::uint8_t>(255);

        return ColorRGBA4b(r, g, b, a);
    }

    static ColorRGBA4b apply(const ColorRGB3f& c) {
        std::uint8_t r = static_cast<std::uint8_t>(std::min(c[0] * 255.0f, 255.0f));
        std::uint8_t g = static_cast<std::uint8_t>(std::min(c[1] * 255.0f, 255.0f));
        std::uint8_t b = static_cast<std::uint8_t>(std::min(c[2] * 255.0f, 255.0f));

        return ColorRGBA4b( r, g, b, 255 );
    }

    static ColorRGBA4b apply(const ColorRGBA4f& c) {
        std::uint8_t r = static_cast<std::uint8_t>(std::min(c[0] * 255.0f, 255.0f));
        std::uint8_t g = static_cast<std::uint8_t>(std::min(c[1] * 255.0f, 255.0f));
        std::uint8_t b = static_cast<std::uint8_t>(std::min(c[2] * 255.0f, 255.0f));
        std::uint8_t a = static_cast<std::uint8_t>(std::min(c[3] * 255.0f, 255.0f));

        return ColorRGBA4b( r, g, b, a );
    }
};

template<>
struct convertTo_<Color1f> {
    static Color1f apply(const Color1b& c) {
        return Color1f(255.0f * c.grayvalue());
    }

    static Color1f apply(const ColorRGB3b& c) {
        return Color1f(255.0f * c.red());
    }

    static Color1f apply(const ColorRGB3f& c) {
        return Color1f(c.red());
    }

    static Color1f apply(const ColorRGBA4b& c) {
        std::uint8_t r = c.red();

        return Color1f( 255.0f * r );
    }
};

template<>
struct convertTo_<ColorRGB3f>
{
    static ColorRGB3f apply(const ColorRGBA4b& c) {
        float r = static_cast<float>(std::min(c[0] / 255.0f, 1.0f));
        float g = static_cast<float>(std::min(c[1] / 255.0f, 1.0f));
        float b = static_cast<float>(std::min(c[2] / 255.0f, 1.0f));

        return ColorRGB3f(r, g, b);
    }
};

template<>
struct convertTo_<ColorRGBA4f>
{
    static ColorRGBA4f apply(const ColorRGBA4f& c) {
        return c;
    }

    static ColorRGBA4f apply(const Color1b& c) {
        float r = std::min(c[0] / 255.0f, 1.0f);

        return ColorRGBA4f(r, r, r, 1.0f);
    }

    static ColorRGBA4f apply(const ColorRGB3b& c) {
        float r = static_cast<float>(std::min(c[0] / 255.0f, 1.0f));
        float g = static_cast<float>(std::min(c[1] / 255.0f, 1.0f));
        float b = static_cast<float>(std::min(c[2] / 255.0f, 1.0f));

        return ColorRGBA4f(r, g, b, 1.0f);
    }

    static ColorRGBA4f apply(const ColorRGB3f& c) {
        float r = c[0];
        float g = c[1];
        float b = c[2];

        return ColorRGBA4f(r, g, b, 1.0f);
    }

    static ColorRGBA4f apply(const ColorRGBA4b& c) {
        float r = static_cast<float>(std::min(c[0] / 255.0f, 1.0f));
        float g = static_cast<float>(std::min(c[1] / 255.0f, 1.0f));
        float b = static_cast<float>(std::min(c[2] / 255.0f, 1.0f));
        float a = static_cast<float>(std::min(c[3] / 255.0f, 1.0f));

        return ColorRGBA4f(r, g, b, a);
    }
};

class ColorConverter
{
public:
    template<typename T>
    static T convertTo(const Color1b& c) { return convertTo_<T>::apply(c); }

    template<typename T>
    static T convertTo(const ColorRGB3b& c) { return convertTo_<T>::apply(c); }

    template<typename T>
    static T convertTo(const ColorRGBA4b& c) { return convertTo_<T>::apply(c); }

    template<typename T>
    static T convertTo(const Color1f& c) { return convertTo_<T>::apply(c); }

    template<typename T>
    static T convertTo(const ColorRGB3f& c) { return convertTo_<T>::apply(c); }

    template<typename T>
    static T convertTo(const ColorRGBA4f& c) { return convertTo_<T>::apply(c); }

    template<typename T>
    static T convertTo(const Vector4f& c) { return convertTo_<T>::apply(c); }

private:
    ColorConverter() = default;
};

DE_VERTEXWAHN_END_NAMESPACE
