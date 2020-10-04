/*
    This file is part of Flatland, a simple 2D ray tracer

    Copyright (c) 2020, Julian Amann (dev@vertexwahn.de), All rights reserved.
    Distributed under the 3-Clause BSD License software license, see the accompanying
    file LICENSE.md or https://opensource.org/licenses/BSD-3-Clause.
*/

#include "flatland/scene/load_scene.h"

#include "flatland/core/camera.h"
#include "flatland/core/filesystem.h"
#include "flatland/core/logging.h"
#include "flatland/core/property_set.h"
#include "flatland/integrator/ambient_occlusion.h"
#include "flatland/integrator/path_mirror_reflection.h"
#include "flatland/integrator/path_specular_transmission.h"
#include "flatland/scene/scene_factory.h"
#include "flatland/scene/shape/disk.h"
#include "flatland/scene/shape/polygon.h"
#include "flatland/scene/shape/rectangle.h"

#include <boost/algorithm/string.hpp>
#include <pugixml.hpp>

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

ReferenceCounted<Shape2f> createDisk(const PropertySet& ps, const fs::path& path) {
    auto transform = ps.getProperty<Transform44f>("transform");
    auto radius = ps.getProperty<float>("radius");
    return makeReferenceCounted<Disk2f>(transform, radius);
}

ReferenceCounted<Shape2f> createRectangle(const PropertySet& ps, const fs::path& path) {
    auto transform = ps.getProperty<Transform44f>("transform");
    auto width = ps.getProperty<float>("width");
    auto height = ps.getProperty<float>("height");
    return makeReferenceCounted<Rectangle2f>(transform, width, height);
}

ReferenceCounted<PathSpecularTransmission> createIntegrator(const PropertySet& ps, const fs::path& path) {
    return makeReferenceCounted<PathSpecularTransmission>(ps);
}

std::vector<Point2f> read2DPlyFile(const fs::path& filename) {
    std::vector<Point2f> points;
    std::ifstream plyFile(filename);
    std::string line;
    while (std::getline(plyFile, line)) {

        std::vector<float> values;
        std::vector<std::string> tokens;

        // skip comment
        if(boost::starts_with(line, "#"))
            continue;

        // skip empty line
        if(line == std::string(""))
            continue;

        boost::split(tokens, line, boost::is_any_of(" "));

        float x = std::stof(tokens[1]);
        float y = std::stof(tokens[2]);

        points.push_back(Point2f{x,y});
    }
    return points;
}

ReferenceCounted<Shape2f> createPolygon(const PropertySet& ps, const fs::path& path) {
    auto transform = ps.getProperty<Transform44f>("transform");

    std::string filename = ps.getProperty<std::string>("filename");

    std::stringstream ss;
    ss << path.parent_path().string() << "/" << filename;
    fs::path outPath = ss.str();

    std::vector<Point2f> points = read2DPlyFile(outPath);

    return makeReferenceCounted<Polygon2f>(transform, &points[0], points.size());
}

ReferenceCounted<Scene2f> loadScene(std::string_view filename) {
    fs::path p(filename);

    bool bIntegratorTagFound = false;

    SceneFactory sf;
    sf.registerClass("disk", createDisk);
    sf.registerClass("rectangle", createRectangle);
    sf.registerClass("polygon", createPolygon);

    pugi::xml_document doc;
    auto result = doc.load_file(filename.data());

    if (!result)
        return nullptr; // todo - what if there is an syntax error!?

    auto fl_scene = makeReferenceCounted<Scene2f>();

    for (pugi::xml_node scene: doc.root()) {
        if (std::string(scene.name()) == "scene") {
            for (pugi::xml_node sceneElements: scene) {
                if (std::string(sceneElements.name()) == "camera") {
                    auto xmlFilm = sceneElements.child("film");
                    int width = xmlFilm.attribute("width").as_int();
                    int height = xmlFilm.attribute("height").as_int();
                    std::string strFilename = xmlFilm.attribute("filename").as_string();

                    auto camera = makeReferenceCounted<Camera2f>(width, height, strFilename);

                    auto xmlTransform = sceneElements.child("transform");
                    if(xmlTransform) {
                        auto transform = readTransform(xmlTransform);
                        camera->setTransform(transform);
                    }

                    fl_scene->setCamera(camera);
                }

                if (std::string(sceneElements.name()) == "integrator") {
                    std::string type = sceneElements.attribute("type").as_string();

                    auto ps = readAllProperties(sceneElements);

                    if(type == "path_specular_transmission") {
                        fl_scene->setIntegrator(makeReferenceCounted<PathSpecularTransmission>(ps));
                    }
                    else if(type == "path_mirror_reflection") {
                        fl_scene->setIntegrator(makeReferenceCounted<PathMirrorReflection2f>(ps));
                    }
                    else if(type == "ambient_occlusion") {
                        fl_scene->setIntegrator(makeReferenceCounted<AmbientOcclusion2f>(ps));
                    }
                    else {
                        LOG(WARNING) << "Unknown integrator. Using path specular transmission path integrator.";
                        fl_scene->setIntegrator(makeReferenceCounted<PathSpecularTransmission>(ps));
                    }

                    bIntegratorTagFound = true;
                }

                if (std::string(sceneElements.name()) == "label") {
                    auto ps = readAllProperties(sceneElements);

                    auto p = ps.getProperty<Point2f>("position");
                    auto t = ps.getProperty<std::string>("text");
                    fl_scene->addAnnotation(Label2f{p, t});
                }

                if (std::string(sceneElements.name()) == "shape") {
                    std::string type = sceneElements.attribute("type").as_string();

                    auto ps = readAllProperties(sceneElements);

                    auto xmlTransform = sceneElements.child("transform");
                    if(xmlTransform) {
                        auto transform = readTransform(xmlTransform);
                        ps.addProperty("transform", transform);
                    }
                    auto shape = sf.createShape(type, ps, p);

                    auto xmlMaterial = sceneElements.child("material");
                    if(xmlMaterial) {
                        auto material_ps = readAllProperties(xmlMaterial);
                        ReferenceCounted<Material> material = createMaterial(material_ps);
                        shape->setMaterial(material);
                    }

                    fl_scene->addShape(shape);
                }
            }
        }
        else {
            throw LoadSceneException("scene xml tag missing");
        }
    }

    if(!bIntegratorTagFound) {
        throw IntegratorMissing();
    }

    return fl_scene;
}

FLATLAND_END_NAMESPACE
