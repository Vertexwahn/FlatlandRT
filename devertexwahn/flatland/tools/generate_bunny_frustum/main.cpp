/*
 *  SPDX-FileCopyrightText: 2022 Julian Amann <dev@vertexwahn.de>
 *  SPDX-License-Identifier: Apache-2.0
 */

#include "flatland/rendering/canvas/svg_canvas.h"

#include "math/geometry.h"
#include "math/transform.h"
#include "math/util.h"
#include "math/warping.h"
#include "flatland/rendering/scene/shape/polygon.h"

#include <iostream>

using namespace de_vertexwahn;

int main() {
    SvgCanvas2f sc{Vector2i{1400, 900}};

    float near_clip_plane_distance = 100.f;
    float far_clip_plane_distance = 500.f;
    float fov = degree_to_radian(30.f);

    // compute corners of 2d view frustum (on y,z-plane)
    float y_far_top = far_clip_plane_distance * tan(.5 * fov);
    float y_near_top = near_clip_plane_distance * tan(.5 * fov);

    Point4f far_top(0.f, y_far_top, far_clip_plane_distance, 1.f);
    Point4f far_bottom(0.f, -y_far_top, far_clip_plane_distance, 1.f);
    Point4f near_top(0.f, y_near_top, near_clip_plane_distance, 1.f);
    Point4f near_bottom(0.f, -y_near_top, near_clip_plane_distance, 1.f);

    std::vector<Point2f> points{
            {0,              0},
            {far_bottom.z(), far_bottom.y()},
            {far_top.z(),    far_top.y()},
    };

    std::vector<Point2f> points2{
            {0,               0},
            {near_bottom.z(), near_bottom.y()},
            {near_top.z(),    near_top.y()},
    };

    auto transform = translate(Vector2f{50.f, 400.f});
    auto transform2 = translate(Vector2f{650.f, 400.f});

    auto point_count = points.size();
    auto far_polygon = make_reference_counted<Polygon2f>(transform, &points[0], point_count);
    auto near_polygon = make_reference_counted<Polygon2f>(transform, &points2[0], point_count);

    PropertySet ps;
    ps.add_property("stroke_color", Color3f(0.f, 0.682f, 0.937f));
    ps.add_property("stroke_width", 3.f);
    ps.add_property("fill_color", Color3f(1.f, 1.f, 1.f));
    ps.add_property("refraction_index", 1.f);
    ps.add_property("reflection_index", 0.f);
    auto material = make_reference_counted<SvgMaterial>(ps);

    far_polygon->set_bxdf(material);
    near_polygon->set_bxdf(material);


    sc.add(far_polygon.get());
    sc.add(near_polygon.get());

    auto per = perspective(fov, near_clip_plane_distance, far_clip_plane_distance);

    Point4f t_far_top = per * far_top;
    Point4f t_far_bottom = per * far_bottom;
    Point4f t_near_top = per * near_top;
    Point4f t_near_bottom = per * near_bottom;

    std::vector<Point2f> points4{
            {t_near_bottom.z(), t_near_bottom.y()},
            {t_far_bottom.z(),  t_far_bottom.y()},
            {t_far_top.z(),     t_far_top.y()},
            {t_near_top.z(),    t_near_top.y()},
    };

    auto transformedFrustum = make_reference_counted<Polygon2f>(transform2, &points4[0], points4.size());
    transformedFrustum->set_bxdf(material);
    sc.add(transformedFrustum.get());

    // homogeneous division
    Point3f w_t_far_top = t_far_top.xyz() / t_far_top.w();
    Point3f w_t_far_bottom = t_far_bottom.xyz() / t_far_bottom.w();
    Point3f w_t_near_top = t_near_top.xyz() / t_near_top.w();
    Point3f w_t_near_bottom = t_near_bottom.xyz() / t_near_bottom.w();

    std::vector<Point2f> points5{
            {w_t_near_bottom.z(), w_t_near_bottom.y()},
            {w_t_far_bottom.z(),  w_t_far_bottom.y()},
            {w_t_far_top.z(),     w_t_far_top.y()},
            {w_t_near_top.z(),    w_t_near_top.y()},
    };

    for (auto point: points5) {
        std::cout << "aftor homo" << std::endl;
        std::cout << point << std::endl;
    }

    auto bunny_points = load_2D_ply<float>("scenes/bunny_coords.ply");
    auto bunny = make_reference_counted<Polygon2f>(transform * translate(350.f, 0.f, 0.f), &bunny_points[0],
                                                   bunny_points.size());
    bunny->set_bxdf(material);
    sc.add(bunny.get());

    auto transformedFrustumAfterHomo = make_reference_counted<Polygon2f>(transform2, &points5[0], points5.size());
    transformedFrustumAfterHomo->set_bxdf(material);
    sc.add(transformedFrustumAfterHomo.get());

    auto bunny_points2 = load_2D_ply<float>("scenes/bunny_coords.ply");
    for (auto &point: bunny_points2) {
        Point4f p4 = per * Point4f(0, point.y(), point.x(), 1.f);

        point = p4.zy();
    }

    auto bunny2 = make_reference_counted<Polygon2f>(transform2 * translate(350.f, 0.f, 0.f), &bunny_points2[0],
                                                    bunny_points2.size());
    bunny2->set_bxdf(material);
    sc.add(bunny2.get());

    //sc.store("/home/vertexwahn/Desktop/frustum.svg");
    sc.store("C:\\Users\\verte\\Desktop\\frustum.svg");

    return 0;
}