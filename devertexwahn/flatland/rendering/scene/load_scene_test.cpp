/*
 *  SPDX-FileCopyrightText: 2022 Julian Amann <dev@vertexwahn.de>
 *  SPDX-License-Identifier: Apache-2.0
 */

#include "flatland/rendering/scene/load_scene.h"
#include "flatland/rendering/scene/shape/rectangle.h"
#include "flatland/rendering/scene/shape/disk.h"

#include "gmock/gmock.h"

namespace de_vertexwahn {
    // defined in load_scene.cpp
    Vector2f convert_csv_to_vector2f(const std::string& csv);
    Vector3f convert_csv_to_vector3f(const std::string& csv);
    Point3f convert_csv_to_point3f(const std::string& csv);
    template <>
    Transform44f read_transform<3>(const pugi::xml_node &xmlTransform);
}

using namespace de_vertexwahn;

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

TEST(Scene, loadScene_EmptyBSDF) {
    EXPECT_THAT([]() { load_scene2f("flatland/tests/scenes/no_bsdf_type.flatland.xml"); },
                ThrowsMessage<FlatlandException>(testing::HasSubstr("No empty BxDF type allowed")));
}

TEST(Scene, loadScene_MissingSceneTag) {
    EXPECT_THROW(load_scene2f("flatland/tests/scenes/scene_tag_missing.flatland.xml"), FlatlandException);
}

TEST(Scene, loadScene_IntegratorMissing) {
    EXPECT_THROW(load_scene2f("flatland/tests/scenes/integrator_missing.flatland.xml"), FlatlandException);
}

TEST(Scene, WhenLoadingASceneWithARectangle_ThenSceneContainsARectangle) {
    // Arrange
    auto scene = load_scene2f("flatland/scenes/rectangle.flatland.xml");

    EXPECT_NE(scene, nullptr);
    EXPECT_THAT(scene->shape_count(), 1u);
    auto rect = std::dynamic_pointer_cast<Rectangle2f>(scene->shapes()[0]);
    EXPECT_NE(rect, nullptr);
}

TEST(Scene, TestShapeGetter) {
    Scene2f scene;
    PropertySet ps;
    ps.add_property("radius", 10.f);
    ps.add_property("transform", identity<float>());
    scene.add_shape(make_reference_counted<Disk2f>(ps));

    EXPECT_THAT(scene.shapes().size(), 1u);
}

// todo: is this a sensor test or scene test? should it be moved to sensor test cpp file?
TEST(Scene, GivenAPointThatIsAlignedWithCameraSpace_WhenPointIsTransformedToCameraSpace_ThenExpectPointIsAtOrigin) {
    // Arrange
    auto scene = load_scene2f("flatland/scenes/rectangle.flatland.xml");
    Point2f world_space_point(100.f, 600.f);

    // Act
    auto sensor = scene->sensor();
    auto camera_space_point = sensor->transform() * world_space_point;

    // Assert
    EXPECT_NEAR(camera_space_point.x(), 0.f, 0.0001f);
    EXPECT_NEAR(camera_space_point.y(), 0.f, 0.0001f);
    EXPECT_THAT(sensor->film()->width(), 600);
    EXPECT_THAT(sensor->film()->height(), 700);
}

TEST(Scene, TestMaterial) {
    // Arrange
    auto scene = load_scene2f("flatland/scenes/disk.flatland.xml");
    auto material = std::static_pointer_cast<SvgMaterial>(scene->shapes()[0]->bxdf());

    // Assert
    EXPECT_THAT(material->stroke_width(), 3);
}

TEST(Scene, loadScene_WhenSceneFileDoesNotExist_ThenNullptr) {
    auto scene = load_scene2f("non_existing_file.xml");

    EXPECT_THAT(scene, nullptr);
}

TEST(Scene, loadScene_WhenSceneFileDoesExist_ThenExpectValidSceneObject) {
    auto scene = load_scene2f("flatland/scenes/disk.flatland.xml");

    EXPECT_NE(scene, nullptr);
}

TEST(Scene, GivenASceneFileWithCameraParameters_ThenExpectValidCameraParamters_WhenSceneIsLoaded) {
    auto scene = load_scene2f("flatland/scenes/disk.flatland.xml");

    EXPECT_THAT(scene->sensor()->film()->width(), 800);
    EXPECT_THAT(scene->sensor()->film()->height(), 600);
    EXPECT_THAT(scene->sensor()->film()->filename(), "disk.svg");
}

TEST(Scene, loadScene_LoadLabels) {
    auto scene = load_scene2f("flatland/scenes/rectangle.flatland.xml");

    EXPECT_THAT(scene->annotations().size(), 2u);
    EXPECT_THAT(scene->annotations()[0].text, "Incident vector");
    EXPECT_THAT(scene->annotations()[1].text, "Refracted vector");
}

TEST(Scene, loadScene_LoadPolygon2) {
    auto scene = load_scene2f("flatland/scenes/bunny/bunny.flatland.xml");
}

TEST(Scene, ReadPolygonTransform2) {
    // Arrange
    auto scene = load_scene2f("flatland/scenes/bunny/bunny.flatland.xml");

    // Act
    auto transform = scene->shapes()[0]->transform();

    // Assert
    Matrix44f scaling;
    scaling << 3.f, 0.f, 0.f, 0.f,
            0.f, 3.f, 0.f, 0.f,
            0.f, 0.f, 1.f, 0.f,
            0.f, 0.f, 0.f, 1.f;

    Matrix44f translation;
    translation << 1.f, 0.f, 0.f, 400.f,
            0.f, 1.f, 0.f, 300.f,
            0.f, 0.f, 1.f, 0.f,
            0.f, 0.f, 0.f, 1.f;

    Matrix44f expected_result = translation * scaling;

    // Assert
    EXPECT_THAT(transform.matrix(), expected_result);
}

TEST(Scene, GivenSceneWithMultipleShapes_WhenRayHitsAllShapes_ThenClosesHit) {
    // Arrange
    auto scene = load_scene2f("flatland/scenes/disk_2.flatland.xml");
    auto ray = scene->sensor()->generate_ray(Point2f{0.f, 0.f});

    // Act
    MediumEvent2f me;
    bool hit = scene->intersect(ray, me);

    // Assert
    EXPECT_TRUE(hit);
    EXPECT_THAT(me.p.x(), 100.f);
    EXPECT_THAT(me.p.y(), 300.f);
}
