/*
 *  SPDX-FileCopyrightText: 2022 Julian Amann <dev@vertexwahn.de>
 *  SPDX-License-Identifier: Apache-2.0
 */

#include "pugixml.hpp"
#include "gmock/gmock.h"

#include <fstream>

TEST(pugixml, load_file) {
    pugi::xml_document doc;

    std::ofstream outfile("test.xml");
    outfile << "<?xml version = \"1.0\" encoding=\"utf-8\"?>" << "\n";
    outfile << "<scene />" << "\n";
    outfile.close();

    pugi::xml_parse_result result = doc.load_file("test.xml");

    EXPECT_THAT(result.status, pugi::xml_parse_status::status_ok);
}

TEST(pugixml, load_De_Vertexwahn_Flatland_file) {
    // Arrange
    pugi::xml_document doc;

    // Act
    pugi::xml_parse_result result = doc.load_file("flatland/scenes/disk.flatland.xml");

    // Assert
    EXPECT_TRUE(result.status != pugi::status_file_not_found);
    EXPECT_TRUE(result);

    int countShapes = 0;
    for (pugi::xml_node shape: doc.child("scene").children("shape")) {
        countShapes++;
        std::string strType = shape.attribute("type").as_string();
        EXPECT_TRUE(strType == "disk");
    }

    EXPECT_TRUE(countShapes == 1);
}
