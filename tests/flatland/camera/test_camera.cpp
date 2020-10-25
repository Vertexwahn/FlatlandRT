/*
    This file is part of Flatland, a simple 2D ray tracer

    Copyright (c) 2020, Julian Amann (dev@vertexwahn.de), All rights reserved.
    Distributed under the 3-Clause BSD License software license, see the accompanying
    file LICENSE.md or https://opensource.org/licenses/BSD-3-Clause.
*/

#include "flatland/camera/camera.h"
#include "flatland/camera/film.h"

#include <gmock/gmock.h>

using namespace Flatland;

TEST(Camera, Camera_ctor) {
    Camera2f cam{800, 600, "out.svg"};

    EXPECT_THAT(cam.getFilm().getWidth(), 800);
    EXPECT_THAT(cam.getFilm().getHeight(), 600);
    EXPECT_THAT(cam.getFilm().getFilename(), "out.svg");
}

TEST(Camera, Camera_getFilm) {
    Camera2f cam{800, 600, "out.svg"};

    const Film& film = cam.getFilm();

    EXPECT_THAT(film.getWidth(), 800);
    EXPECT_THAT(film.getHeight(), 600);
    EXPECT_THAT(film.getFilename(), "out.svg");
}

TEST(Camera, Camera_generateRay) {
    Camera2f cam{800, 600, "out.svg"};

    Ray2f ray = cam.generateRay();

    EXPECT_NEAR(ray.origin.x(), 0.0f, 0.1f);
    EXPECT_NEAR(ray.origin.y(), 0.0f, 0.1f);

    EXPECT_NEAR(ray.direction.x(), 1.0f, 0.1f);
    EXPECT_NEAR(ray.direction.y(), 0.0f, 0.1f);
}

TEST(Camera, Camera_generateRay2) {
    Camera2f cam{800, 600, "out.svg"};

    cam.setTransform(look_at(Point2f{0.0f, 0.0f}, Point2f{1.0f, 0.0f}));
    Ray2f ray = cam.generateRay();

    EXPECT_THAT(ray.origin, (Point2f{0.0f,0.0f}));
    EXPECT_THAT(ray.direction, (Vector2f{1.0f,0.0f}));
}

TEST(Camera, Camera_generateRay3) {
    Camera2f cam{800, 600, "out.svg"};

    cam.setTransform(look_at(Point2f{1.0f, 1.0f}, Point2f{2.0f, 2.0f}));
    Ray2f ray = cam.generateRay();

    EXPECT_NEAR(ray.origin.x(), 1.0f, 0.1f);
}

TEST(Camera, Camera_generateRay4) {
    Camera2f cam{800, 600, "out.svg"};
    cam.setTransform(look_at(Point2f{1.0f, 1.0f}, Point2f{2.0f, 2.0f}));

    Ray2f ray = cam.generateRay();

    auto direction = Vector2f{1.0f,1.0f}.normalized();
    EXPECT_NEAR(ray.direction.x(), direction.x(), 0.1f);
}