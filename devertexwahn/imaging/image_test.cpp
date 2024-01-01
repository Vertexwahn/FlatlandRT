/*
 *  SPDX-FileCopyrightText: Copyright 2022-2023 Julian Amann <dev@vertexwahn.de>
 *  SPDX-License-Identifier: Apache-2.0
 */

#include "imaging/io/io.h"

#include "gmock/gmock.h"

#include <fstream>

using namespace de_vertexwahn;

TEST(Image3f, ctorViaWidthHeight) {
    // Act
    Image3f image{100, 100};

    image.set_pixel(50, 50, Color3f{1.f, 0.f, 0.f});

    // Assert
    EXPECT_THAT(image.width(), 100);
    EXPECT_THAT(image.height(), 100);
    EXPECT_THAT(image.size(), (Vector2i(100, 100)));
    EXPECT_TRUE(image.data());
    EXPECT_THAT(image.get_pixel(50, 50), (Color3f{1.f, 0.f, 0.f}));

    for(int y = 0; y < image.width(); ++y) {
        for(int x = 0; x < image.height(); ++x) {
            if(x != 50 && y != 50) {
                EXPECT_THAT(image.get_pixel(x, y), (Color3f{0.f, 0.f, 0.f}));
            }
        }
    }
}

TEST(Image4f, ctorViaWidthHeight) {
    // Act
    Image4f image{100, 100};

    image.set_pixel(50, 50, Color4f{1.f, 0.f, 0.f, 1.f});

    // Assert
    EXPECT_THAT(image.width(), 100);
    EXPECT_THAT(image.height(), 100);
    EXPECT_THAT(image.size(), (Vector2i(100, 100)));
    EXPECT_TRUE(image.data());
    EXPECT_THAT(image.get_pixel(50, 50), (Color4f{1.f, 0.f, 0.f, 1.f}));

    for(int y = 0; y < image.width(); ++y) {
        for(int x = 0; x < image.height(); ++x) {
            if(x != 50 && y != 50) {
                EXPECT_THAT(image.get_pixel(x, y), (Color4f{0.f, 0.f, 0.f, 0.f}));
            }
        }
    }
}

TEST(Image4f, ctorViaWidthHeightInitalColor) {
    // Act
    Color4f inital_color{.1f, .5f, 1.f, 1.f};
    Image4f image{100, 100, inital_color};

    image.set_pixel(50, 50, Color4f{1.f, 0.f, 0.f, 1.f});

    // Assert
    EXPECT_THAT(image.width(), 100);
    EXPECT_THAT(image.height(), 100);
    EXPECT_THAT(image.size(), (Vector2i(100, 100)));
    EXPECT_TRUE(image.data());
    EXPECT_THAT(image.get_pixel(50, 50), (Color4f{1.f, 0.f, 0.f, 1.f}));

    for(int y = 0; y < image.width(); ++y) {
        for(int x = 0; x < image.height(); ++x) {
            if(x != 50 && y != 50) {
                EXPECT_THAT(image.get_pixel(x, y), inital_color);
            }
        }
    }
}

TEST(Image3f, ctorViaSize) {
    // Act
    Vector2i size{100, 100};
    Image3f image{size};

    image.set_pixel(50, 50, Color3f{1.f, 0.f, 0.f});

    // Assert
    EXPECT_THAT(image.width(), 100);
    EXPECT_THAT(image.height(), 100);
    EXPECT_THAT(image.size(), (Vector2i(100, 100)));
    EXPECT_TRUE(image.data());
    EXPECT_THAT(image.get_pixel(50, 50), (Color3f{1.f, 0.f, 0.f}));

    for(int y = 0; y < image.width(); ++y) {
        for(int x = 0; x < image.height(); ++x) {
            if(x != 50 && y != 50) {
                EXPECT_THAT(image.get_pixel(x, y), (Color3f{0.f, 0.f, 0.f}));
            }
        }
    }
}

TEST(Image4f, ctorViaSize) {
    // Act
    Vector2i size{100, 100};
    Image4f image{size};

    image.set_pixel(50, 50, Color4f{1.f, 0.f, 0.f, 1.f});

    // Assert
    EXPECT_THAT(image.width(), 100);
    EXPECT_THAT(image.height(), 100);
    EXPECT_THAT(image.size(), (Vector2i(100, 100)));
    EXPECT_TRUE(image.data());
    EXPECT_THAT(image.get_pixel(50, 50), (Color4f{1.f, 0.f, 0.f, 1.f}));

    for(int y = 0; y < image.width(); ++y) {
        for(int x = 0; x < image.height(); ++x) {
            if(x != 50 && y != 50) {
                EXPECT_THAT(image.get_pixel(x, y), (Color4f{0.f, 0.f, 0.f, 0.f}));
            }
        }
    }
}

TEST(Image4b, ctor) {
    // Act
    Image4b image{100, 100};

    image.set_pixel(50, 50, Color4b{255, 255, 255, 255});

    // Assert
    EXPECT_THAT(image.width(), 100);
    EXPECT_THAT(image.height(), 100);
    EXPECT_THAT(image.size(), (Vector2i(100, 100)));
    EXPECT_TRUE(image.data());
    EXPECT_THAT(image.get_pixel(50, 50), (Color4b{255, 255, 255, 255}));

    for(int y = 0; y < image.width(); ++y) {
        for(int x = 0; x < image.height(); ++x) {
            if(x != 50 && y != 50) {
                EXPECT_THAT(image.get_pixel(x, y), (Color4b{0, 0, 0, 0}));
            }
        }
    }
}

TEST(Image, copy_ctor) {
    // Arrange
    Image3f image_a{100, 100};
    auto color = Color3f{1.f, 0.5f, 0.1f};
    image_a.set_pixel(50, 50, color);
    Image3f image_b{50, 50};

    // Act
    image_b = image_a;

    // Assert
    EXPECT_THAT(image_b.width(), 100);
    EXPECT_THAT(image_b.height(), 100);
    EXPECT_THAT(image_b.size(), (Vector2i(100, 100)));
    EXPECT_TRUE(image_b.data());
    EXPECT_THAT(image_b.get_pixel(50, 50), color);
}

TEST(Image, WriteAndReadFile) {
    std::string filename = "file.txt";

    // Write file
    {
        std::ofstream file;
        file.open(filename);
        file << "Writing this to a file.";
        file.close();
    }

    std::string text = "test";

    // Read file
    {
        std::ifstream file(filename);
        std::string content((std::istreambuf_iterator<char>(file)), std::istreambuf_iterator<char>());
        text = content;
    }

    EXPECT_THAT(text, "Writing this to a file.");
}

TEST(Image, storeLoadOpenEXR) {
    // Arrange
    Image3f image{100, 100};

    for (int y = 0; y < image.height(); ++y) {
        for (int x = 0; x < image.width(); ++x) {
            image.set_pixel(x, y, Color3f{1.f, 0.f, 0.f});
        }
    }

    // Act
    store_image("test.exr", image);

    // Assert
    Image3f ref_image = load_image("test.exr");

    EXPECT_THAT(image.width(), ref_image.width());
    EXPECT_THAT(image.height(), ref_image.height());

    for (int y = 0; y < image.height(); ++y) {
        for (int x = 0; x < image.width(); ++x) {
            Color3f c = image.get_pixel(x, y);
            Color3f ref_c = ref_image.get_pixel(x, y);
            EXPECT_THAT(ref_c, (Color3f{1.f, 0.f, 0.f}));
            EXPECT_THAT(ref_c, c);
        }
    }
}

TEST(Image, storeLoadPNG) {
    // Arrange
    Image3f image{100, 100};

    for (int y = 0; y < image.height(); ++y) {
        for (int x = 0; x < image.width(); ++x) {
            image.set_pixel(x, y, Color3f{1.f, 0.f, 0.f});
        }
    }

    // Act
    store_image("test.png", image);

    // Assert
    Image3f ref_image = load_image("test.png");

    EXPECT_THAT(image.width(), ref_image.width());
    EXPECT_THAT(image.height(), ref_image.height());

    for (int y = 0; y < image.height(); ++y) {
        for (int x = 0; x < image.width(); ++x) {
            Color3f c = image.get_pixel(x, y);
            Color3f ref_c = ref_image.get_pixel(x, y);
            EXPECT_THAT(ref_c, (Color3f{1.f, 0.f, 0.f}));
            EXPECT_THAT(ref_c, c);
        }
    }
}
