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

#include "core/io/file_stream.hpp"

#include <fstream>
#include <sstream>
#include <iostream>

DE_VERTEXWAHN_BEGIN_NAMESPACE

NAMESPACE_BEGIN(detail)

inline std::ios::openmode ios_flag(FileStream::EMode mode) {
    switch (mode) {
        case FileStream::ERead:
            return std::ios::binary | std::ios::in;
        case FileStream::EReadWrite:
            return std::ios::binary | std::ios::in | std::ios::out;
        case FileStream::ETruncReadWrite:
            return std::ios::binary | std::ios::in | std::ios::out | std::ios::trunc;
        default:
            throw std::runtime_error("Internal error");
    }
}

NAMESPACE_END(detail)

FileStream::FileStream(const std::filesystem::path &p, EMode mode)
        : Stream(), m_mode(mode), m_path(p), m_file(new std::fstream) {

    m_file->open(p.string(), detail::ios_flag(mode));

    if (!m_file->good()) {
        //Throw("\"%s\": I/O error while attempting to open file: %s",
        //      m_path.string(), strerror(errno));
        throw std::runtime_error("I/O error while attempting to open file");
    }

}

FileStream::~FileStream() {
    close();
}

void FileStream::close() {
    m_file->close();
};

bool FileStream::is_closed() const {
    return !m_file->is_open();
};

void FileStream::read(void *p, size_t size) {
    m_file->read((char *) p, size);

    if (unlikely(!m_file->good())) {
        bool eof = m_file->eof();
        //size_t gcount = m_file->gcount();
        m_file->clear();
        if (eof) {
            throw std::runtime_error("I/O error while attempting to read");
            //throw EOFException(tfm::format("\"%s\": read %zu out of %zu bytes",
            //                               m_path.string(), gcount, size), gcount);
        }
        else {
            //Throw("\"%s\": I/O error while attempting to read %zu bytes: %s",
            //      m_path.string(), size, strerror(errno));
            throw std::runtime_error("I/O error while attempting to read");
        }
    }
}

void FileStream::write(const void *p, size_t size) {
    m_file->write((char *) p, size);

    if (unlikely(!m_file->good())) {
        m_file->clear();
        throw std::runtime_error("I/O error while attempting to write");
        //Throw("\"%s\": I/O error while attempting to write %zu bytes: %s",
        //      m_path.string(), size, strerror(errno));
    }
}

void FileStream::truncate(size_t size) {
    if (m_mode == ERead) {
        throw std::runtime_error("attempting to truncate a read-only FileStream");
        //Throw("\"%s\": attempting to truncate a read-only FileStream",
        //      m_path.string());
    }

    flush();
    const size_t old_pos = tell();
#if defined(_WIN32)
    // Windows won't allow a resize if the file is open
close();
#else
    seek(0);
#endif

    std::filesystem::resize_file(m_path, size);

#if defined(_WIN32)
    m_file->open(m_path, detail::ios_flag(EReadWrite));
if (!m_file->good())
    std::runtime_error("I/O error while attempting to open file");
    //Throw("\"%s\": I/O error while attempting to open file: %s",
    //      m_path.string(), strerror(errno));
#endif

    seek(std::min(old_pos, size));
}

void FileStream::seek(size_t pos) {
    m_file->seekg(static_cast<std::ios::pos_type>(pos));

    if (unlikely(!m_file->good())) {
        throw std::runtime_error("I/O error while attempting to seek");
        //Throw("\"%s\": I/O error while attempting to seek to offset %zu: %s",
        //      m_path.string(), pos, strerror(errno));
    }

}

size_t FileStream::tell() const {
    std::ios::pos_type pos = m_file->tellg();
    if (unlikely(pos == std::ios::pos_type(-1))) {
        std::runtime_error("I/O error while attempting to determine position in file");
        //Throw("\"%s\": I/O error while attempting to determine "
        //      "position in file", m_path.string());
    }

    return static_cast<size_t>(pos);
}

void FileStream::flush() {
    m_file->flush();
    if (unlikely(!m_file->good())) {
        m_file->clear();
        throw std::runtime_error("I/O error while attempting flush file stream");
        //Throw("\"%s\": I/O error while attempting flush "
        //      "file stream: %s", m_path.string(), strerror(errno));
    }
}

size_t FileStream::size() const {
    return std::filesystem::file_size(m_path);
}

std::string FileStream::read_line() {
    std::string result;
    if (!std::getline(*m_file, result)) {
        //Log(Error, "\"%s\": I/O error while attempting to read a line of text: %s", m_path.string(),
        //    strerror(errno));
        std::cout << "Error: I/O error while attempting to read a line of text" << std::endl;
    }

    return result;
}

std::string FileStream::to_string() const {
    std::ostringstream oss;

    oss << /*class_()->name()*/ "FileStream" << "[" << std::endl;
    if (is_closed()) {
        oss << "  closed" << std::endl;
    } else {
        size_t pos = (size_t) -1;
        try { pos = tell(); } catch (...) { }
        oss << "  path = \"" << m_path.string() << "\"" << "," << std::endl
            << "  host_byte_order = " << host_byte_order() << "," << std::endl
            << "  byte_order = " << byte_order() << "," << std::endl
            << "  can_read = " << can_read() << "," << std::endl
            << "  can_write = " << can_write() << "," << std::endl
            << "  pos = " << pos << "," << std::endl
            << "  size = " << size() << std::endl;
    }

    oss << "]";

    return oss.str();
}

DE_VERTEXWAHN_END_NAMESPACE
