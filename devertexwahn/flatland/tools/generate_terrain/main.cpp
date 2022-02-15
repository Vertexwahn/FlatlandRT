/*
 *  SPDX-FileCopyrightText: 2022 Julian Amann <dev@vertexwahn.de>
 *  SPDX-License-Identifier: Apache-2.0
 */

#include "flatland/math/vector.h"

#include <vector>
#include <iostream>

using namespace flatland;

//struct VertexPosition2f {
//    Vector2f position;
//};

class VertexIndexGenerator {
public:
    size_t add_vertex(const Vector2f& position) {
        size_t index = positions_.size();
        positions_.push_back(position);
        return index;
    }

    std::vector<Vector2f> positions_;
};

const std::string FILE_HEADER = R"(#
#   SPDX-FileCopyrightText: 2022 Julian Amann <dev@vertexwahn.de>
#   SPDX-License-Identifier: Apache-2.0
#
)";

int main(int /*argc*/, char** /* argv */) {
    std::cout << FILE_HEADER << std::endl;

    Vector2f terrain_size{2, 2};
    Vector2f terrain_patch_size{50.f, 50.f};

    std::vector<std::uint32_t> indices;

    VertexIndexGenerator vig;

    for(int y = 0; y < terrain_size.y(); ++y) {
        for(int x = 0; x < terrain_size.x(); ++x) {
            /*
              D       C      
                +---+
                | / |
                +---+
              A       B
            */

           Vector2f a{x * terrain_patch_size.x(), y * terrain_patch_size.y()};
           Vector2f b = a + Vector2f{terrain_patch_size.x(), 0.f};
           Vector2f c = a + terrain_patch_size;
           Vector2f d = a + Vector2f{0, terrain_patch_size.y()};

           size_t ia = vig.add_vertex(a);
           size_t ib = vig.add_vertex(b);
           size_t ic = vig.add_vertex(c);
           size_t id = vig.add_vertex(d);
           
           indices.push_back(ia);
           indices.push_back(ib);
           indices.push_back(ic);

           indices.push_back(ic);
           indices.push_back(id);
           indices.push_back(ia);
        }   
    }

    for(const auto& position : vig.positions_) {
        std::cout << "v " << position.x() << " " << position.y() << std::endl;
    }

    for(size_t i = 0; i < indices.size(); i+=3) {
        std::cout << "f " << indices[i] << " " << indices[i+1] << " " << indices[i+2] << std::endl;
    }
  
    return  0;
}