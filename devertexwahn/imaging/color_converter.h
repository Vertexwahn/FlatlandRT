/*
 *  Note: This file contains code derived and modified from BlueFramework which is under Apache-2.0 license and
 *  copyrighted by Technical University of Munich, Chair of Computational Modeling and Simulation
 *  Modifications are copyright by Julian Amann and also published under Apache-2.0 license
 */

#pragma once

#include "imaging/color.h"

#include "math/vector.h"

#include "core/namespace.h"

DE_VERTEXWAHN_BEGIN_NAMESPACE

template<typename T>
struct convertTo_ {
    static T apply(const Color3b& c) {
        return T(c[0], c[1], c[2]);
    }
};

template<>
struct convertTo_<Color1b> {
    static Color1b apply(const Color1b& c) {
        return c;
    }

    static Color1b apply(const Color3b& c) {
        return Color1b(c.red());
    }

    static Color1b apply(const Color3f& c) {
        return Color1b(static_cast<std::uint8_t>(std::min(c.red() * 255.0f, 255.0f)));
    }

    static Color1b apply(const Color4f& c) {
        return Color1b(static_cast<std::uint8_t>(std::min(c.red() * 255.0f, 255.0f)));
    }

    static Color1b apply(const Color4b& c) {
        std::uint8_t r = c.red();

        return Color1b(r);
    }
};

template<>
struct convertTo_<Color3b> {
    static Color3b apply(const Color1b& c) {
        // throw away alpha channel
        std::uint8_t r = c.grayvalue();
        std::uint8_t g = c.grayvalue();
        std::uint8_t b = c.grayvalue();

        return Color3b( r, g, b );
    }

    static Color3b apply(const Color3b& c) {
        return c;
    }

    static Color3b apply(const Color4b& c) {
        // throw away alpha channel
        std::uint8_t r = c.red();
        std::uint8_t g = c.green();
        std::uint8_t b = c.blue();

        return Color3b( r, g, b );
    }

    static Color3b apply(const Color1f& c) {
        std::uint8_t grayvalue = static_cast<std::uint8_t>(std::min(c[0] * 255.0f, 255.0f));

        return Color3b( grayvalue, grayvalue, grayvalue );
    }

    static Color3b apply(const Color3f& c) {
        std::uint8_t r = static_cast<std::uint8_t>(std::min(c[0] * 255.0f, 255.0f));
        std::uint8_t g = static_cast<std::uint8_t>(std::min(c[1] * 255.0f, 255.0f));
        std::uint8_t b = static_cast<std::uint8_t>(std::min(c[2] * 255.0f, 255.0f));

        return Color3b( r, g, b );
    }

    static Color3b apply(const Color4f& c) {
        // throw away alpha channel
        std::uint8_t r = static_cast<std::uint8_t>(std::min(c[0] * 255.0f, 255.0f));
        std::uint8_t g = static_cast<std::uint8_t>(std::min(c[1] * 255.0f, 255.0f));
        std::uint8_t b = static_cast<std::uint8_t>(std::min(c[2] * 255.0f, 255.0f));

        return Color3b( r, g, b );
    }
};

template<>
struct convertTo_<Color4b> {
    static Color4b apply(const Vector4f& c) {
        std::uint8_t r = static_cast<std::uint8_t>(std::min(c.x() * 255.0f, 255.0f));
        std::uint8_t g = static_cast<std::uint8_t>(std::min(c.y() * 255.0f, 255.0f));
        std::uint8_t b = static_cast<std::uint8_t>(std::min(c.z() * 255.0f, 255.0f));
        std::uint8_t a = static_cast<std::uint8_t>(std::min(c.w() * 255.0f, 255.0f));

        return Color4b( r, g, b, a );
    }

    static Color4b apply(const Color1b& c) {
        std::uint8_t r = c.grayvalue();
        std::uint8_t g = c.grayvalue();
        std::uint8_t b = c.grayvalue();
        std::uint8_t a = 255;

        return Color4b( r, g, b, a );
    }

    static Color4b apply(const Color3b& c) {
        std::uint8_t r = c.red();
        std::uint8_t g = c.green();
        std::uint8_t b = c.blue();
        std::uint8_t a = 255;

        return Color4b( r, g, b, a );
    }

    static Color4b apply(const Color4b& c) {
        return c;
    }

    static Color4b apply(const Color1f& c) {
        std::uint8_t r = static_cast<std::uint8_t>(std::min(c[0] * 255.0f, 255.0f));
        std::uint8_t g = static_cast<std::uint8_t>(std::min(c[0] * 255.0f, 255.0f));
        std::uint8_t b = static_cast<std::uint8_t>(std::min(c[0] * 255.0f, 255.0f));
        std::uint8_t a = static_cast<std::uint8_t>(255);

        return Color4b(r, g, b, a);
    }

    static Color4b apply(const Color3f& c) {
        std::uint8_t r = static_cast<std::uint8_t>(std::min(c[0] * 255.0f, 255.0f));
        std::uint8_t g = static_cast<std::uint8_t>(std::min(c[1] * 255.0f, 255.0f));
        std::uint8_t b = static_cast<std::uint8_t>(std::min(c[2] * 255.0f, 255.0f));

        return Color4b( r, g, b, 255 );
    }

    static Color4b apply(const Color4f& c) {
        std::uint8_t r = static_cast<std::uint8_t>(std::min(c[0] * 255.0f, 255.0f));
        std::uint8_t g = static_cast<std::uint8_t>(std::min(c[1] * 255.0f, 255.0f));
        std::uint8_t b = static_cast<std::uint8_t>(std::min(c[2] * 255.0f, 255.0f));
        std::uint8_t a = static_cast<std::uint8_t>(std::min(c[3] * 255.0f, 255.0f));

        return Color4b( r, g, b, a );
    }
};

template<>
struct convertTo_<Color1f> {
    static Color1f apply(const Color1b& c) {
        return Color1f(255.0f * c.grayvalue());
    }

    static Color1f apply(const Color3b& c) {
        return Color1f(255.0f * c.red());
    }

    static Color1f apply(const Color3f& c) {
        return Color1f(c.red());
    }

    static Color1f apply(const Color4b& c) {
        std::uint8_t r = c.red();

        return Color1f( 255.0f * r );
    }
};

template<>
struct convertTo_<Color3f>
{
    static Color3f apply(const Color4b& c) {
        float r = static_cast<float>(std::min(c[0] / 255.0f, 1.0f));
        float g = static_cast<float>(std::min(c[1] / 255.0f, 1.0f));
        float b = static_cast<float>(std::min(c[2] / 255.0f, 1.0f));

        return Color3f(r, g, b);
    }
};

template<>
struct convertTo_<Color4f>
{
    static Color4f apply(const Color4f& c) {
        return c;
    }

    static Color4f apply(const Color1b& c) {
        float r = std::min(c[0] / 255.0f, 1.0f);

        return Color4f(r, r, r, 1.0f);
    }

    static Color4f apply(const Color3b& c) {
        float r = static_cast<float>(std::min(c[0] / 255.0f, 1.0f));
        float g = static_cast<float>(std::min(c[1] / 255.0f, 1.0f));
        float b = static_cast<float>(std::min(c[2] / 255.0f, 1.0f));

        return Color4f(r, g, b, 1.0f);
    }

    static Color4f apply(const Color3f& c) {
        float r = c[0];
        float g = c[1];
        float b = c[2];

        return Color4f(r, g, b, 1.0f);
    }

    static Color4f apply(const Color4b& c) {
        float r = static_cast<float>(std::min(c[0] / 255.0f, 1.0f));
        float g = static_cast<float>(std::min(c[1] / 255.0f, 1.0f));
        float b = static_cast<float>(std::min(c[2] / 255.0f, 1.0f));
        float a = static_cast<float>(std::min(c[3] / 255.0f, 1.0f));

        return Color4f(r, g, b, a);
    }
};

class ColorConverter
{
public:
    template<typename T>
    static T convertTo(const Color1b& c) { return convertTo_<T>::apply(c); }

    template<typename T>
    static T convertTo(const Color3b& c) { return convertTo_<T>::apply(c); }

    template<typename T>
    static T convertTo(const Color4b& c) { return convertTo_<T>::apply(c); }

    template<typename T>
    static T convertTo(const Color1f& c) { return convertTo_<T>::apply(c); }

    template<typename T>
    static T convertTo(const Color3f& c) { return convertTo_<T>::apply(c); }

    template<typename T>
    static T convertTo(const Color4f& c) { return convertTo_<T>::apply(c); }

    template<typename T>
    static T convertTo(const Vector4f& c) { return convertTo_<T>::apply(c); }

private:
    ColorConverter() = default;
};

DE_VERTEXWAHN_END_NAMESPACE
