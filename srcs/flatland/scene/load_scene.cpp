/*
    This file is part of Flatland, a simple 2D ray tracer

    Copyright (c) 2020, Julian Amann (dev@vertexwahn.de), All rights reserved.
    Distributed under the 3-Clause BSD License software license, see the accompanying
    file LICENSE.md or https://opensource.org/licenses/BSD-3-Clause.
*/

#include "flatland/scene/load_scene.h"

#include "flatland/camera/camera.h"
#include "flatland/core/logging.h"
#include "flatland/core/object_factory.h"
#include "flatland/core/property_set.h"
#include "flatland/core/sampler.h"
#include "flatland/integrator/ambient_occlusion.h"
#include "flatland/integrator/path_mirror_reflection.h"
#include "flatland/integrator/path_specular_transmission.h"
#include "flatland/scene/shape/disk.h"
#include "flatland/scene/shape/polygon.h"
#include "flatland/scene/shape/rectangle.h"

#include <pugixml.hpp>

#include <filesystem>
#include <fstream>
#include <limits>
#include <string>
#include <vector>

FLATLAND_BEGIN_NAMESPACE

LoadSceneException::LoadSceneException(std::string_view message) : errorMessage_(message) {
}

const char *LoadSceneException::what() const throw() {
    return errorMessage_.c_str();
}

LoadSceneException::~LoadSceneException() {}

IntegratorMissing::IntegratorMissing() : LoadSceneException("Integrator missing") {
}

std::vector<float> convertToFloatVector(const std::string& csv) {
    std::vector<float> values;
    std::vector<std::string> tokens;
    boost::split(tokens, csv, boost::is_any_of(","));

    for(auto token : tokens) {
        float value = std::stof(token);
        values.push_back(value);
    }

    return values;
}

Vector2f convertCSVToVector2f(const std::string& csv) {
    std::vector<float> values = convertToFloatVector(csv);
    return Vector2f{values[0], values[1]};
}

Point2f convertCSVToPoint2f(const std::string& csv) {
    std::vector<float> values = convertToFloatVector(csv);
    return Point2f{values[0], values[1]};
}

Color3f convertCSVToColor3f(const std::string& csv) {
    std::vector<float> values = convertToFloatVector(csv);
    return Color3f{values[0], values[1], values[2]};
}

Transform44f readTransform(const pugi::xml_node &xmlTransform) {
    Transform44f transform = identity<float>();

    for( auto node : xmlTransform) {
        std::string transformType = node.name();

        if (transformType == "rotate_z") {
            auto angle_in_degree = node.attribute("angle").as_float();
            transform = rotateZ(degreeToRadian(angle_in_degree)) * transform;
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

        if (transformType == "lookAt") {
            auto csv_origin = node.attribute("origin").as_string();
            auto origin = convertCSVToVector2f(csv_origin);

            auto csv_target = node.attribute("target").as_string();
            auto target = convertCSVToVector2f(csv_target);

            transform = look_at<float>(origin, target) * transform;
        }
    }

    return transform;
}

PropertySet readAllProperties(const pugi::xml_node &node) {
    PropertySet ps;

    // Todo: what happens if name is missing

    for (pugi::xml_node xmlProperty: node.children("integer")) {
        std::string name = xmlProperty.attribute("name").as_string();
        auto value = xmlProperty.attribute("value").as_int();
        ps.addProperty(name, value);
    }

    for (pugi::xml_node xmlProperty: node.children("float")) {
        std::string name = xmlProperty.attribute("name").as_string();
        auto value = xmlProperty.attribute("value").as_float();
        ps.addProperty(name, value);
    }

    for (pugi::xml_node xmlProperty: node.children("boolean")) {
        std::string name = xmlProperty.attribute("name").as_string();
        auto value = xmlProperty.attribute("value").as_bool();
        ps.addProperty(name, value);
    }

    for (pugi::xml_node xmlProperty: node.children("string")) {
        std::string name = xmlProperty.attribute("name").as_string();
        auto value = xmlProperty.attribute("value").as_string();
        ps.addProperty(name, std::string(value));
    }

    for (pugi::xml_node xmlProperty: node.children("point")) {
        std::string name = xmlProperty.attribute("name").as_string();
        auto csv = xmlProperty.attribute("value").as_string();
        auto value = convertCSVToPoint2f(csv);
        ps.addProperty(name, value);
    }

    for (pugi::xml_node xmlProperty: node.children("vector")) {
        std::string name = xmlProperty.attribute("name").as_string();
        auto csv = xmlProperty.attribute("value").as_string();
        auto value = convertCSVToVector2f(csv);
        ps.addProperty(name, value);
    }

    for (pugi::xml_node xmlProperty: node.children("color")) {
        std::string name = xmlProperty.attribute("name").as_string();
        auto csv = xmlProperty.attribute("value").as_string();
        auto value = convertCSVToColor3f(csv);
        ps.addProperty(name, value);
    }

    return ps;
}

ReferenceCounted<Scene2f> loadScene(std::string_view filename) {
    std::filesystem::path p(filename);

    ObjectFactory sf;
    sf.registerClass<Disk2f>("disk");
    sf.registerClass<Rectangle2f>("rectangle");
    sf.registerClass<Polygon2f>("polygon");
    sf.registerClass<PathSpecularTransmission>("path_specular_transmission");
    sf.registerClass<PathMirrorReflection2f>("path_mirror_reflection");
    sf.registerClass<AmbientOcclusion2f>("ambient_occlusion");

    bool bIntegratorTagFound = false;

    pugi::xml_document doc;
    auto result = doc.load_file(filename.data());

    if (!result)
        return nullptr;

    auto scene = makeReferenceCounted<Scene2f>();

    scene->setSampler(makeReferenceCounted<Sampler2f>());

    for (pugi::xml_node scene_node: doc.root()) {
        if (std::string(scene_node.name()) == "scene") {
            for (pugi::xml_node sceneElements: scene_node) {
                if (std::string(sceneElements.name()) == "camera") {
                    auto xmlFilm = sceneElements.child("film");
                    int width = xmlFilm.attribute("width").as_int();
                    int height = xmlFilm.attribute("height").as_int();
                    auto strFilename = xmlFilm.attribute("filename").as_string();

                    auto camera = makeReferenceCounted<Camera2f>(width, height, strFilename);

                    auto xmlTransform = sceneElements.child("transform");
                    if(xmlTransform) {
                        auto transform = readTransform(xmlTransform);
                        camera->setTransform(transform);
                    }

                    scene->setCamera(camera);
                }

                if (std::string(sceneElements.name()) == "integrator") {
                    std::string type = sceneElements.attribute("type").as_string();

                    auto ps = readAllProperties(sceneElements);

                    try {
                        auto integrator = std::dynamic_pointer_cast<Integrator2f>(sf.createInstance(type, ps));
                        integrator->setSampler(scene->getSampler().get());
                        scene->setIntegrator(integrator);
                    }
                    catch (...) {
                        throw IntegratorMissing();
                    }

                    bIntegratorTagFound = true;
                }

                if (std::string(sceneElements.name()) == "label") {
                    auto ps = readAllProperties(sceneElements);

                    auto p = ps.getProperty<Point2f>("position");
                    auto t = ps.getProperty<std::string>("text");
                    scene->addAnnotation(Label2f{p, t});
                }

                if (std::string(sceneElements.name()) == "shape") {
                    std::string type = sceneElements.attribute("type").as_string();

                    auto ps = readAllProperties(sceneElements);

                    if(type == "polygon") {
                        ps.addProperty("parent_path", p.parent_path().string());
                    }

                    auto xmlTransform = sceneElements.child("transform");
                    if(xmlTransform) {
                        auto transform = readTransform(xmlTransform);
                        ps.addProperty("transform", transform);
                    }
                    auto shape = std::dynamic_pointer_cast<Shape2f>(sf.createInstance(type, ps));

                    auto xmlMaterial = sceneElements.child("material");
                    if(xmlMaterial) {
                        auto material_ps = readAllProperties(xmlMaterial);
                        ReferenceCounted<Material> material = createMaterial(material_ps);
                        shape->setMaterial(material);
                    }

                    scene->addShape(shape);
                }
            }
        }
        else {
            throw LoadSceneException("scene_node xml tag missing");
        }
    }

    if(!bIntegratorTagFound) {
        throw IntegratorMissing();
    }

    return scene;
}

FLATLAND_END_NAMESPACE
