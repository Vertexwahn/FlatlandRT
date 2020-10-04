/*
    This file is part of Flatland, a simple 2D ray tracer

    Copyright (c) 2020, Julian Amann (dev@vertexwahn.de), All rights reserved.
    Distributed under the 3-Clause BSD License software license, see the accompanying
    file LICENSE.md or https://opensource.org/licenses/BSD-3-Clause.
*/

#pragma once
#ifndef Flatland_Material_11d72dcc_a416_4530_b706_b1e69711c5c5_h
#define Flatland_Material_11d72dcc_a416_4530_b706_b1e69711c5c5_h

#include "flatland/core/color.h"
#include "flatland/core/namespace.h"
#include "flatland/core/property_set.h"
#include "flatland/core/reference_counted.h"

FLATLAND_BEGIN_NAMESPACE

enum class eInterfaceInteraction {
    specularTransmission,
    mirrorReflection
};

class Material {
public:
    Material();

    Material(const PropertySet& ps);

    // svg output related
    void setStrokeColor(Color3f color);
    Color3f getStrokeColor() const;
	void setStrokeWidth(const float width);
    float getStrokeWidth() const;
	void setFillColor(Color3f color);
	Color3f getFillColor() const;

	// ray tracing related
    float getRefractionIndex() const;

    eInterfaceInteraction getInterInteractionType() const;

private:
    Color3f strokeColor_ = Color3f(0.0f, 0.0f, 0.0f);
	Color3f fillColor_ = Color3f(1.0f, 1.0f, 1.0f);
	float strokeWidth_ = 1.0f;
	float refractionIndex_ = 0.0f;
    eInterfaceInteraction interfaceInteraction_  = eInterfaceInteraction::specularTransmission;
};

ReferenceCounted<Material> createMaterial(const PropertySet& ps);

FLATLAND_END_NAMESPACE

#endif // end define Flatland_Material_11d72dcc_a416_4530_b706_b1e69711c5c5_h
