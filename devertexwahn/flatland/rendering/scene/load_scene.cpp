/*
 *  SPDX-FileCopyrightText: 2022 Julian Amann <dev@vertexwahn.de>
 *  SPDX-License-Identifier: Apache-2.0
 */

#include "flatland/rendering/scene/load_scene.h"

#include "flatland/core/object_factory.h"
#include "flatland/rendering/integrator/path_mirror_reflection.h"
#include "flatland/rendering/integrator/path_specular_transmission.h"
#include "flatland/rendering/intersector/brute_force_intersector.h"
#include "flatland/rendering/intersector/quadtree_intersector.h"
#include "flatland/rendering/scene/shape/disk.h"
#include "flatland/rendering/scene/shape/polygon.h"
#include "flatland/rendering/scene/shape/rectangle.h"
#include "flatland/rendering/scene/shape/triangle_mesh.h"
#include "flatland/rendering/integrator/ambient_occlusion.h"

FLATLAND_BEGIN_NAMESPACE

template <>
Transform44f read_transform<3>(const pugi::xml_node &xmlTransform) {
    Transform44f transform = identity<float>();

    for( auto node : xmlTransform) {
        std::string transformType = node.name();

        if (transformType == "rotate_x") {
            auto angle_in_degree = node.attribute("angle").as_float();
            transform = rotate_x(degree_to_radian(angle_in_degree)) * transform;
        }

        if (transformType == "rotate_z") {
            auto angle_in_degree = node.attribute("angle").as_float();
            transform = rotate_z(degree_to_radian(angle_in_degree)) * transform;
        }

        if (transformType == "translate") {
            auto x = node.attribute("x").as_float();
            auto y = node.attribute("y").as_float();
            auto z = node.attribute("z").as_float();
            transform = translate(x,y,z) * transform;
        }

        if (transformType == "scale") {
            auto x = node.attribute("x").as_float();
            auto y = node.attribute("y").as_float();
            auto z = node.attribute("z").as_float();
            if(node.attribute("z").empty()) {
                z = 1.f;
            }
            transform = scale(x,y,z) * transform;
        }

        if (transformType == "lookat") {
            auto csv_origin = node.attribute("origin").as_string();
            auto origin = convert_csv_to_point3f(csv_origin);

            auto csv_target = node.attribute("target").as_string();
            auto target = convert_csv_to_point3f(csv_target);

            auto csv_up = node.attribute("up").as_string();
            auto up = convert_csv_to_vector3f(csv_up);

            transform = look_at<float>(origin, target, up) * transform;
        }

        if(transformType == "matrix") {
            throw std::runtime_error("matrix as tranform not implemented");
        }
    }

    return transform;
}

std::vector<float> convert_to_float_vector(const std::string& csv) {
    std::vector<float> values;
    std::vector<std::string> tokens;
    boost::split(tokens, csv, boost::is_any_of(","));

    for(auto token : tokens) {
        float value = std::stof(token);
        values.push_back(value);
    }

    return values;
}

Vector2f convert_csv_to_vector2f(const std::string& csv) {
    std::vector<float> values = convert_to_float_vector(csv);
    return Vector2f{values[0], values[1]};
}

Vector3f convert_csv_to_vector3f(const std::string& csv) {
    std::vector<float> values = convert_to_float_vector(csv);
    return Vector3f{values[0], values[1], values[2]};
}

Point2f convert_csv_to_point2f(const std::string& csv) {
    std::vector<float> values = convert_to_float_vector(csv);
    return Point2f{values[0], values[1]};
}

Point3f convert_csv_to_point3f(const std::string& csv) {
    std::vector<float> values = convert_to_float_vector(csv);
    return Point3f{values[0], values[1], values[2]};
}

Color3f convert_csv_to_color3f(const std::string& csv) {
    std::vector<float> values = convert_to_float_vector(csv);
    return Color3f{values[0], values[1], values[2]};
}

void read_all_properties(const pugi::xml_node &node, PropertySet& out_ps) {
    for (pugi::xml_node xml_property: node.children("integer")) {
        std::string name = xml_property.attribute("name").as_string();
        auto value = xml_property.attribute("value").as_int();
        out_ps.add_property(name, value);
    }

    for (pugi::xml_node xml_property: node.children("float")) {
        std::string name = xml_property.attribute("name").as_string();
        auto value = xml_property.attribute("value").as_float();
        out_ps.add_property(name, value);
    }

    for (pugi::xml_node xml_property: node.children("boolean")) {
        std::string name = xml_property.attribute("name").as_string();
        auto value = xml_property.attribute("value").as_bool();
        out_ps.add_property(name, value);
    }

    for (pugi::xml_node xml_property: node.children("string")) {
        std::string name = xml_property.attribute("name").as_string();
        auto value = xml_property.attribute("value").as_string();
        out_ps.add_property(name, std::string(value));
    }

    for (pugi::xml_node xml_property: node.children("point")) {
        std::string name = xml_property.attribute("name").as_string();
        auto csv = xml_property.attribute("value").as_string();

        std::vector<float> values = convert_to_float_vector(csv);
        if(values.size() == 3) {
            auto value = convert_csv_to_point3f(csv);
            out_ps.add_property(name, value);
        }
        else if(values.size() == 2) {
            auto value = convert_csv_to_point2f(csv);
            out_ps.add_property(name, value);
        }
        else {
            throw std::runtime_error("Point should have 2 or 3 components");
        }
    }

    for (pugi::xml_node xml_property: node.children("vector")) {
        std::string name = xml_property.attribute("name").as_string();
        auto csv = xml_property.attribute("value").as_string();
        auto value = convert_csv_to_vector2f(csv);
        out_ps.add_property(name, value);
    }

    for (pugi::xml_node xmlProperty: node.children("color")) {
        std::string name = xmlProperty.attribute("name").as_string();
        auto csv = xmlProperty.attribute("value").as_string();
        auto value = convert_csv_to_color3f(csv);
        out_ps.add_property(name, value);
    }
}

PropertySet read_all_properties(const pugi::xml_node &node) {
    PropertySet ps;
    read_all_properties(node, ps);
    return ps;
}

ReferenceCounted<Scene2f> load_scene2f(std::string_view filename) {
    ObjectFactory<PropertySet> object_factory;

    // Intersector
    object_factory.register_class<BruteForceIntersector2f>("brute_force");
    object_factory.register_class<QuadtreeIntersector2f>("quadtree");

    // Integrator
    object_factory.register_class<AmbientOcclusion2f>("ambient_occlusion");
    object_factory.register_class<PathMirrorReflection2f>("path_mirror_reflection");
    object_factory.register_class<PathSpecularTransmission>("path_specular_transmission");

    // Shapes
    object_factory.register_class<Disk2f>("disk");
    object_factory.register_class<Polygon2f>("polygon");
    object_factory.register_class<Rectangle2f>("rectangle");
    object_factory.register_class<TriangleMesh2f>("triangle_mesh");

    // BSDFs
    object_factory.register_class<SvgMaterial>("svg_material");

    // Sampler
    object_factory.register_class<IndependentSampler>("independent");

    //return load_scene<2,float>(filename, object_factory);

    // https://stackoverflow.com/questions/12316972/how-to-tweak-stdstod-string-to-double-for-decimal-separator-and-number-of-di
    // save locale setting
    const auto old_locale = std::setlocale(LC_NUMERIC, nullptr);

    // force '.' as the radix point
    std::setlocale(LC_NUMERIC,"C");

    pugi::xml_document doc;
    auto result = doc.load_file(filename.data());

    if (!result)
        return nullptr;

    auto scene = make_reference_counted<SceneType<2, float>>();

    std::filesystem::path p(filename);
    bool integrator_tag_found = false;

    for (pugi::xml_node scene_node : doc.root()) {
        if (std::string(scene_node.name()) == "scene") {
            for (pugi::xml_node scene_elements : scene_node) {
                if (std::string(scene_elements.name()) == "sensor") {
                    // Construct film
                    auto xml_film = scene_elements.child("film");

                    PropertySet film_ps = read_all_properties(xml_film);
                    //auto filter = read_filter<float>(xml_film, object_factory);
                    //film_ps.add_property("filter", filter);

                    auto film = make_reference_counted<Film_>(film_ps);

                    // Construct sensor
                    PropertySet sensor_ps = read_all_properties(scene_elements);
                    sensor_ps.add_property("film", film);
                    auto sensor = make_reference_counted<SensorType<2, float>>(sensor_ps);

                    auto xml_transform = scene_elements.child("transform");
                    if (xml_transform) {
                        auto transform = read_transform<2>(xml_transform);
                        sensor->set_transform(transform);
                    }

                    scene->set_sensor(sensor);

                    auto xml_sampler = scene_elements.child("sampler");
                    if(xml_sampler) {
                        std::string type = xml_sampler.attribute("type").as_string();

                        auto ps = read_all_properties(xml_sampler);
                        auto sampler = std::dynamic_pointer_cast<SamplerType<float>>(
                                object_factory.create_instance(type, ps));
                        scene->set_sampler(sampler);
                    }
                }

                if (std::string(scene_elements.name()) == "integrator") {
                    std::string type = scene_elements.attribute("type").as_string();

                    auto ps = read_all_properties(scene_elements);

                    try {
                        auto integrator = std::dynamic_pointer_cast<IntegratorType<2, float>>(
                                object_factory.create_instance(type, ps));
                        scene->set_integrator(integrator);
                    } catch (...) {
                        throw FlatlandException("Integrator missing");
                    }

                    integrator_tag_found = true;
                }

                if (std::string(scene_elements.name()) == "label") {
                    auto ps = read_all_properties(scene_elements);

                    auto label_position = ps.get_property<Point2f>("position");
                    auto label_text = ps.get_property<std::string>("text");
                    scene->add_annotation(Label2f{label_position, label_text});
                }

                if (std::string(scene_elements.name()) == "shape") {
                    std::string type = scene_elements.attribute("type").as_string();

                    auto ps = read_all_properties(scene_elements);

                    if (type == "polygon" || type == "obj" || type == "triangle_mesh") {
                        ps.add_property("parent_path", p.parent_path().string());
                    }

                    auto transform = identity<float>();

                    auto xml_transform = scene_elements.child("transform");
                    if (xml_transform) {
                        transform = read_transform<2>(xml_transform);
                    }

                    ps.add_property("transform", transform);

                    auto shape = std::dynamic_pointer_cast<ShapeType<2, float>>(
                            object_factory.create_instance(type, ps));

                    auto xml_bsdf = scene_elements.child("bsdf");
                    std::string str_bsdf_type = xml_bsdf.attribute("type").as_string();
                    if(str_bsdf_type == "") {
                        throw FlatlandException("No empty BSDF type allowed.");
                    }
                    if (xml_bsdf) {
                        auto bsdf_ps = read_all_properties(xml_bsdf);
                        auto bsdf = std::dynamic_pointer_cast<BSDF>(
                                object_factory.create_instance(str_bsdf_type, bsdf_ps));
                        shape->set_bsdf(bsdf);
                    }

                    scene->add_shape(shape);
                }

                if (std::string(scene_elements.name()) == "intersector") {
                    std::string type = scene_elements.attribute("type").as_string();

                    auto ps = read_all_properties(scene_elements);

                    auto intersector = std::dynamic_pointer_cast<IntersectorType<2, float>>(
                            object_factory.create_instance(type, ps));
                    scene->set_intersector(intersector);
                }
            }
        } else {
            throw FlatlandException("scene_node xml tag missing");
        }
    }

    if (!integrator_tag_found) {
        throw FlatlandException("Integrator missing");
    }

    if (!scene->has_intersector()) {
        auto intersector = std::dynamic_pointer_cast<IntersectorType<2, float>>(
                object_factory.create_instance("brute_force", PropertySet{}));
        scene->set_intersector(intersector);
    }

    auto intersector = scene->intersector();
    intersector->build_acceleration_structure(scene->shapes());

    if (scene->sampler() == nullptr) {
        scene->set_sampler(make_reference_counted<IndependentSamplerType<float>>(PropertySet{}));
    }

    // restore locale setting
    std::setlocale(LC_NUMERIC, old_locale);

    return scene;
}

FLATLAND_END_NAMESPACE