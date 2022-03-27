/*
 *  SPDX-FileCopyrightText: 2022 Julian Amann <dev@vertexwahn.de>
 *  SPDX-License-Identifier: Apache-2.0
 */

#include "flatland/rendering/scene/load_scene.h"

#include "gmock/gmock.h"

namespace flatland {
    // defined in load_scene.cpp
    Vector2f convert_csv_to_vector2f(const std::string& csv);
    Vector3f convert_csv_to_vector3f(const std::string& csv);
    Point3f convert_csv_to_point3f(const std::string& csv);
    template <>
    Transform44f read_transform<3>(const pugi::xml_node &xmlTransform);
}

using namespace flatland;

TEST(load_scene, convert_csv_to_vector2f) {
    std::string input = "2.3, 3.4";
    Vector2f v = convert_csv_to_vector2f(input);

    EXPECT_THAT(v.x(), 2.3f);
    EXPECT_THAT(v.y(), 3.4f);
}

TEST(load_scene, convert_csv_to_vector3f) {
    std::string input = "2.3, 3.4, 4.5";
    Vector3f v = convert_csv_to_vector3f(input);

    EXPECT_THAT(v.x(), 2.3f);
    EXPECT_THAT(v.y(), 3.4f);
    EXPECT_THAT(v.z(), 4.5f);
}

TEST(load_scene, convert_csv_to_point3f) {
    std::string input = "2.3, 3.4, 4.5";
    Point3f p = convert_csv_to_point3f(input);

    EXPECT_THAT(p.x(), 2.3f);
    EXPECT_THAT(p.y(), 3.4f);
    EXPECT_THAT(p.z(), 4.5f);
}

TEST(load_scene, read_translate_transform) {
    // Arrange
    pugi::xml_document doc;

    std::ofstream outfile("test.xml");
    outfile << "<?xml version = \"1.0\" encoding=\"utf-8\"?>" << "\n";
    outfile << "<transform>" << "\n";
    outfile << "    <translate x=\"400\" y=\"300\"/>" << "\n";
    outfile << "</transform>" << "\n";
    outfile.close();

    pugi::xml_parse_result result = doc.load_file("test.xml");
    auto xml_transform = doc.child("transform");

    EXPECT_THAT(result.status, pugi::xml_parse_status::status_ok);

    // Act
    Transform44f trafo = read_transform<3>(xml_transform);

    // Assert
    EXPECT_THAT(trafo.matrix(), translate(400.f, 300.f).matrix());
}

TEST(load_scene, read_scale_transform) {
    // Arrange
    pugi::xml_document doc;

    std::ofstream outfile("test.xml");
    outfile << "<?xml version = \"1.0\" encoding=\"utf-8\"?>" << "\n";
    outfile << "<transform>" << "\n";
    outfile << "    <scale x=\"400\" y=\"300\"/>" << "\n";
    outfile << "</transform>" << "\n";
    outfile.close();

    pugi::xml_parse_result result = doc.load_file("test.xml");
    auto xml_transform = doc.child("transform");

    EXPECT_THAT(result.status, pugi::xml_parse_status::status_ok);

    // Act
    Transform44f trafo = read_transform<3>(xml_transform);

    // Assert
    EXPECT_THAT(trafo.matrix(), scale(400.f, 300.f).matrix());
}

TEST(load_scene, read_lookat_transform) {
    // Arrange
    pugi::xml_document doc;

    std::ofstream outfile("test.xml");
    outfile << "<?xml version = \"1.0\" encoding=\"utf-8\"?>" << "\n";
    outfile << "<transform>" << "\n";
    outfile << "    <lookat target=\"0.9, 2.6, 0\"" << "\n";
    outfile << "        origin=\"-16, 10, -16\"" << "\n";
    outfile << "        up=\"0.0, 1.0, 0.0\" />" << "\n";
    outfile << "</transform>" << "\n";
    outfile.close();

    pugi::xml_parse_result result = doc.load_file("test.xml");
    auto xml_transform = doc.child("transform");

    EXPECT_THAT(result.status, pugi::xml_parse_status::status_ok);

    // Act
    Transform44f trafo = read_transform<3>(xml_transform);

    // Assert
    EXPECT_THAT(trafo.matrix(), look_at(Point3f{-16.f, 10.f, -16}, Point3f{0.9f, 2.6f, 0.f}, Vector3f{0.f, 1.f, 0.f}).matrix());
}

TEST(load_scene, read_rotate_z_transform) {
    // Arrange
    pugi::xml_document doc;

    std::ofstream outfile("test.xml");
    outfile << "<?xml version = \"1.0\" encoding=\"utf-8\"?>" << "\n";
    outfile << "<transform>" << "\n";
    outfile << "    <rotate_z angle=\"45\"/>" << "\n";
    outfile << "</transform>" << "\n";
    outfile.close();

    pugi::xml_parse_result result = doc.load_file("test.xml");
    auto xml_transform = doc.child("transform");

    EXPECT_THAT(result.status, pugi::xml_parse_status::status_ok);

    // Act
    Transform44f trafo = read_transform<3>(xml_transform);

    // Assert
    EXPECT_THAT(trafo.matrix(), rotate_z(degree_to_radian(45.f)).matrix());
}

TEST(load_scene, read_matrix_transform) {
    // Arrange
    pugi::xml_document doc;

    std::ofstream outfile("test.xml");
    outfile << "<?xml version = \"1.0\" encoding=\"utf-8\"?>" << "\n";
    outfile << "<transform>" << "\n";
    outfile << "    <matrix />" << "\n";
    outfile << "</transform>" << "\n";
    outfile.close();

    pugi::xml_parse_result result = doc.load_file("test.xml");
    auto xml_transform = doc.child("transform");

    EXPECT_THAT(result.status, pugi::xml_parse_status::status_ok);

    // Act
    EXPECT_THROW(read_transform<3>(xml_transform), std::runtime_error);
}
