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

#pragma once
#ifndef De_Vertexwahn_Core_FileStream_10802a80_e350_4d5a_8af2_c4c1721048a4_hpp
#define De_Vertexwahn_Core_FileStream_10802a80_e350_4d5a_8af2_c4c1721048a4_hpp

#include "core/namespace.hpp"
#include "core/io/stream.hpp"

#include <filesystem>

DE_VERTEXWAHN_BEGIN_NAMESPACE

/** \brief Simple \ref Stream implementation backed-up by a file.
 *
 * The underlying file abstraction is ``std::fstream``, and so most
 * operations can be expected to behave similarly.
 */
class FileStream : public Stream {
public:
    enum EMode {
        /// Opens a file in (binary) read-only mode
        ERead,

        /// Opens (but never creates) a file in (binary) read-write mode
        EReadWrite,

        /// Opens (and truncates) a file in (binary) read-write mode
        ETruncReadWrite
    };

    using Stream::read;
    using Stream::write;

    /** \brief Constructs a new FileStream by opening the file pointed by <tt>p</tt>.
     *
     * The file is opened in read-only or read/write mode as specified by \c mode.
     *
     * Throws if trying to open a non-existing file in with write disabled.
     * Throws an exception if the file cannot be opened / created.
     */
    FileStream(const std::filesystem::path &p, EMode mode = ERead);

    /// Destructor
    ~FileStream();

    /** \brief Closes the stream and the underlying file.
     * No further read or write operations are permitted.
     *
     * This function is idempotent.
     * It is called automatically by the destructor.
     */
    void close() override;

    /// Whether the stream is closed (no read or write are then permitted).
    bool is_closed() const override;

    /// Convenience function for reading a line of text from an ASCII file
    std::string read_line() override;

    /// Return the "native" std::fstream associated with this FileStream
    std::fstream *native() { return m_file.get(); }

    /// Return the path descriptor associated with this FileStream
    const std::filesystem::path &path() const { return m_path; }

    // =========================================================================
    //! @{ \name Implementation of the Stream interface
    // Most methods can be delegated directly to the underlying
    // standard file stream, avoiding having to deal with portability.
    // =========================================================================
    /**
     * \brief Reads a specified amount of data from the stream.
     * Throws an exception when the stream ended prematurely.
     */
    void read(void *p, size_t size) override;

    /**
     * \brief Writes a specified amount of data into the stream.
     * Throws an exception when not all data could be written.
     */
    void write(const void *p, size_t size) override;

    /// Seeks to a position inside the stream. May throw if the resulting state is invalid.
    void seek(size_t pos) override;

    /** \brief Truncates the file to a given size.
     * Automatically flushes the stream before truncating the file.
     * The position is updated to <tt>min(old_position, size)</tt>.
     *
     * Throws an exception if in read-only mode.
     */
    void truncate(size_t size) override;

    /// Gets the current position inside the file
    size_t tell() const override;

    /** \brief Returns the size of the file.
     * \note After a write, the size may not be updated
     * until a \ref flush is performed.
     */
    size_t size() const override;

    /// Flushes any buffered operation to the underlying file.
    void flush() override;

    /// Whether the field was open in write-mode (and was not closed)
    bool can_write() const override { return m_mode != ERead && !is_closed(); }

    /// True except if the stream was closed.
    bool can_read() const override { return !is_closed(); }

    /// Returns a string representation
    std::string to_string() const;

    //! @}
    // =========================================================================

    //MI_DECLARE_CLASS()

private:
    EMode m_mode;
    std::filesystem::path m_path;
    mutable std::unique_ptr<std::fstream> m_file;
};

DE_VERTEXWAHN_END_NAMESPACE

#endif // end define De_Vertexwahn_Core_FileStream_10802a80_e350_4d5a_8af2_c4c1721048a4_hpp
