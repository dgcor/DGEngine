// Copyright (c) 2016 Steinwurf ApS
// All Rights Reserved
//
// Distributed under the "BSD License". See the accompanying LICENSE.rst file.

#pragma once

#include <cstdint>
#include <cassert>
#include <limits>

namespace endian
{
/// @brief Base-class for the endian stream reader and writer.
class stream
{
public:

    /// Creates an endian stream used to track a buffer of the specified size.
    ///
    /// @param size the size of the buffer in bytes
    stream(uint64_t size) :
        m_size(size)
    {
        assert(m_size <= std::numeric_limits<std::size_t>::max() &&
               "Size is too large to be handled on this OS.");
    }

    /// Gets the size of the underlying buffer in bytes.
    ///
    /// @return the size of the buffer
    uint64_t size() const
    {
        return m_size;
    }

    /// Gets the current read/write position in the stream
    ///
    /// @return the current position.
    uint64_t position() const
    {
        return m_position;
    }

    /// The remaining number of bytes in the stream
    ///
    /// @return the remaining number of bytes.
    uint64_t remaining_size() const
    {
        assert(m_size >= m_position);
        return m_size - m_position;
    }

    /// Changes the current read/write position in the stream. The
    /// position is absolute i.e. it is always relative to the
    /// beginning of the buffer which is position 0.
    ///
    /// @param new_position the new position
    void seek(uint64_t new_position)
    {
        assert(new_position <= m_size);
        m_position = new_position;
    }

    /// Skips over a given number of bytes in the stream
    ///
    /// @param bytes_to_skip the bytes to skip
    void skip(uint64_t bytes_to_skip)
    {
        assert(bytes_to_skip + m_position <= m_size);
        seek(m_position + bytes_to_skip);
    }

protected:

    /// The size of the buffer in bytes
    uint64_t m_size;

    /// The current position
    uint64_t m_position = 0;
};
}
