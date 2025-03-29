/*
 *  SPDX-FileCopyrightText: Copyright 2025 Julian Amann <dev@vertexwahn.de>
 *  SPDX-License-Identifier: BSD-3-Clause-LBNL
 */

// This file contains code copied from Mitsuba 3 (v3.6.2)
// The copied code is available under the following license:

/*
    Copyright (c) 2017 Wenzel Jakob <wenzel.jakob@epfl.ch>, All rights reserved.

    Redistribution and use in source and binary forms, with or without
    modification, are permitted provided that the following conditions are met:

    1. Redistributions of source code must retain the above copyright notice, this
       list of conditions and the following disclaimer.

    2. Redistributions in binary form must reproduce the above copyright notice,
       this list of conditions and the following disclaimer in the documentation
       and/or other materials provided with the distribution.

    3. Neither the name of the copyright holder nor the names of its contributors
       may be used to endorse or promote products derived from this software
       without specific prior written permission.

    THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS" AND
    ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE IMPLIED
    WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE
    DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT HOLDER OR CONTRIBUTORS BE LIABLE
    FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL
    DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR
    SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER
    CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY,
    OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE
    OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.

    You are under no obligation whatsoever to provide any bug fixes, patches, or
    upgrades to the features, functionality or performance of the source code
    ("Enhancements") to anyone; however, if you choose to make your Enhancements
    available either publicly, or directly to the author of this software, without
    imposing a separate written license agreement for such Enhancements, then you
    hereby grant the following license: a non-exclusive, royalty-free perpetual
    license to install, use, modify, prepare derivative works, incorporate into
    other computer software, distribute, and sublicense such enhancements or
    derivative works thereof, in binary and source code form.
*/

// Modifications are licensed under the same license as the original work,
// i.e. BSD-3-Clause-LBNL

#include "core/io/stream.hpp"

#include <sstream>
#include <cctype>

NAMESPACE_BEGIN(de_vertexwahn)

NAMESPACE_BEGIN(detail)
static Stream::EByteOrder byte_order() {
    union {
        uint8_t  char_value[2];
        uint16_t short_value;
    };
    char_value[0] = 1;
    char_value[1] = 0;

    if (short_value == 1)
        return Stream::ELittleEndian;
    else
        return Stream::EBigEndian;
}
NAMESPACE_END(detail)

const Stream::EByteOrder Stream::m_host_byte_order = detail::byte_order();

// -----------------------------------------------------------------------------

Stream::Stream()
    : m_byte_order(m_host_byte_order) { }

Stream::~Stream() { }

void Stream::set_byte_order(EByteOrder value) {
    m_byte_order = value;
}

// -----------------------------------------------------------------------------

std::string Stream::to_string() const {
    std::ostringstream oss;

    oss << /*class_()->name()*/ "Stream" << "[" << std::endl;
    if (is_closed()) {
        oss << "  closed" << std::endl;
    } else {
        oss << "  host_byte_order = " << m_host_byte_order << "," << std::endl
            << "  byte_order = " << m_byte_order << "," << std::endl
            << "  can_read = " << can_read() << "," << std::endl
            << "  can_write = " << can_write() << "," << std::endl
            << "  pos = " << tell() << "," << std::endl
            << "  size = " << size() << std::endl;
    }

    oss << "]";

    return oss.str();
}

std::ostream &operator<<(std::ostream &os, const Stream::EByteOrder &value) {
    switch (value) {
    case Stream::ELittleEndian: os << "little-endian"; break;
    case Stream::EBigEndian: os << "big-endian"; break;
    default: os << "invalid"; break;
    }
    return os;
}

void Stream::write_line(const std::string &text) {
    write(text.data(), text.length() * sizeof(char));
    write('\n');
}

std::string Stream::read_line() {
    std::string result;
    result.reserve(80);

    try {
        do {
            char data;
            read(&data, sizeof(char));
            if (data == '\n')
                break;
            else if (data != '\r')
                result += data;
        } while (true);
    } catch (...) {
        if (tell() == size() && !result.empty())
            return result;
        throw;
    }

    return result;
}

std::string Stream::read_token() {
    std::string result;

    try {
        do {
            char data;
            read(&data, sizeof(char));
            if (std::isspace(data)) {
                if (result.empty())
                    continue;
                else
                    break;
            }
            result += data;
        } while (true);
    } catch (...) {
        if (tell() == size() && !result.empty())
            return result;
        throw;
    }

    return result;
}

void Stream::skip(size_t amount) {
    seek(tell() + amount);
}

NAMESPACE_END(de_vertexwahn)
