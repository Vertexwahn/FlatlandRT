/*
 *  SPDX-FileCopyrightText: 2022 Julian Amann <dev@vertexwahn.de>
 *  SPDX-License-Identifier: Apache-2.0
 */

#pragma once

#include "flatland/core/reference_counted.h"
#include "flatland/rendering/scene/scene.h"
#include "flatland/rendering/scene/load_scene_exception.h"
#include "flatland/core/object_factory.h"

#include "pugixml.hpp"

#include <charconv>
#include <clocale>
#include <filesystem>
#include <fstream>
#include <limits>
#include <string>
#include <string_view>
#include <vector>

FLATLAND_BEGIN_NAMESPACE

ReferenceCounted<Scene2f> load_scene2f(std::string_view filename);

Point2f convert_csv_to_point2f(const std::string& csv);
Point3f convert_csv_to_point3f(const std::string& csv);
Color3f convert_csv_to_color3f(const std::string& csv);
Vector3f convert_csv_to_vector3f(const std::string& csv);

template <unsigned int Dimension>
Transform44f read_transform(const pugi::xml_node &xmlTransform) {
    Transform44f transform = identity<float>();

    for( auto node : xmlTransform) {
        std::string transformType = node.name();

        if (transformType == "rotate_z") {
            auto angle_in_degree = node.attribute("angle").as_float();
            transform = rotate_z(degree_to_radian(angle_in_degree)) * transform;
        }

        if (transformType == "translate") {
            auto x = node.attribute("x").as_float();
            auto y = node.attribute("y").as_float();
            transform = translate(x,y) * transform;
        }

        if (transformType == "scale") {
            auto x = node.attribute("x").as_float();
            auto y = node.attribute("y").as_float();
            transform = scale(x,y) * transform;
        }

        if (transformType == "lookat") {
            auto csv_origin = node.attribute("origin").as_string();
            auto origin = convert_csv_to_point2f(csv_origin);

            auto csv_target = node.attribute("target").as_string();
            auto target = convert_csv_to_point2f(csv_target);

            transform = look_at<float>(origin, target) * transform;
        }
    }

    return transform;
}

template <>
Transform44f read_transform<3>(const pugi::xml_node &xmlTransform);

PropertySet read_all_properties(const pugi::xml_node &node);
void read_all_properties(const pugi::xml_node &node, PropertySet& out_ps);

FLATLAND_END_NAMESPACE