/*
 *  SPDX-FileCopyrightText: Copyright 2023 Julian Amann <dev@vertexwahn.de>
 *  SPDX-License-Identifier: CC-BY-SA-3.0
 */

#include "io_webp.hpp"

#include "webp/encode.h"
#include "webp/decode.h"

#include <filesystem>
#include <fstream>
#include <iterator>

DE_VERTEXWAHN_BEGIN_NAMESPACE

bool store_webp(const char *filename, const Image4b &image) {
    int stride = image.width() * static_cast<int>(sizeof(ColorRGBA4b)); // TODO: stride should be moved to image class
    uint8_t* out = nullptr;
    auto encoded_size = WebPEncodeLosslessRGBA(image.data(), image.width(), image.height(), stride, &out);

    // see https://stackoverflow.com/questions/11563963/how-to-write-a-large-buffer-into-a-binary-file-in-c-fast
    FILE* file = fopen(filename, "wb");
    fwrite(out, 1, encoded_size, file);
    fclose(file);

    return true;
}

// Copied from https://stackoverflow.com/questions/15138353/how-to-read-a-binary-file-into-a-vector-of-unsigned-chars
// Posted by jww
// Under CC-BY-SA-3.0 license
// License text can be found here https://creativecommons.org/licenses/by-sa/3.0/
// Modifications: Changed function name to lower snake case and replaced BYTE by uint8_t
// for this file the same license applies
std::vector<uint8_t> read_file(const char* filename)
{
    // open the file:
    std::ifstream file(filename, std::ios::binary);

    // Stop eating new lines in binary mode!!!
    file.unsetf(std::ios::skipws);

    // get its size:
    std::streampos file_size;

    file.seekg(0, std::ios::end);
    file_size = file.tellg();
    file.seekg(0, std::ios::beg);

    // reserve capacity
    std::vector<uint8_t> vec;
    vec.reserve(file_size);

    // read the data:
    vec.insert(vec.begin(),
               std::istream_iterator<uint8_t>(file),
               std::istream_iterator<uint8_t>());

    return vec;
}

ReferenceCounted<Image4b> load_image_webp(std::string_view filename) {
    if(!std::filesystem::exists(filename)) {
        throw std::runtime_error("File does not exist.");
    }

    std::vector<uint8_t> encoded_webp = read_file(filename.data());
    int width = 0;
    int height = 0;

    //WebPGetInfo(encoded_webp.data(), encoded_webp.size(), &width, &height);

    uint8_t* decoded_data = WebPDecodeRGBA(encoded_webp.data(), encoded_webp.size(), &width, &height);
    if(decoded_data == nullptr) {
        std::runtime_error("Decoding WebP did not work.");
    }
    assert(decoded_data);
    assert(width > 0);
    assert(height > 0);

    auto image = make_reference_counted<Image4b>(width, height, decoded_data);

    WebPFree(decoded_data);

    return image;
}

DE_VERTEXWAHN_END_NAMESPACE
