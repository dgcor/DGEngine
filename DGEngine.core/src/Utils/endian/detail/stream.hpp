// Copyright (c) 2016 Steinwurf ApS
// All Rights Reserved
//
// Distributed under the "BSD License". See the accompanying LICENSE.rst file.

#pragma once

#include <cassert>
#include <cstdint>
#include <limits>
#include <type_traits>
#include <vector>

namespace endian
{
namespace detail
{

using non_const_stream = uint8_t*;
using const_stream = const uint8_t*;

/// @brief Base-class for the endian stream reader and writer.
template <typename data_ptr_type>
class stream
{
public:
    /// Creates an endian stream used to track a buffer of the specified size.
    ///
    /// @param size the size of the buffer in bytes
    stream(data_ptr_type data, std::size_t size) noexcept :
        m_data(data), m_size(size)
    {
    }

    /// Gets the size of the underlying buffer in bytes.
    ///
    /// @return the size of the buffer
    std::size_t size() const noexcept
    {
        return m_size;
    }

    /// Gets the current read/write position in the stream
    ///
    /// @return the current position.
    std::size_t position() const noexcept
    {
        return m_position;
    }

    /// The remaining number of bytes in the stream
    ///
    /// @return the remaining number of bytes.
    std::size_t remaining_size() const noexcept
    {
        return m_size - m_position;
    }

    /// Changes the current read/write position in the stream. The
    /// position is absolute i.e. it is always relative to the
    /// beginning of the buffer which is position 0.
    ///
    /// @param new_position the new position
    void seek(std::size_t new_position) noexcept
    {
        assert(new_position <= m_size);

        m_position = new_position;
    }

    /// Skips over a given number of bytes in the stream
    ///
    /// @param bytes_to_skip the bytes to skip
    void skip(std::size_t bytes_to_skip) noexcept
    {
        assert(bytes_to_skip <= m_size - m_position);

        seek(m_position + bytes_to_skip);
    }

    /// A pointer to the stream's data.
    ///
    /// @return pointer to the stream's data.
    data_ptr_type data() const noexcept
    {
        return m_data;
    }

    /// A pointer to the stream's data at the current position.
    ///
    /// @return pointer to the stream's data at the current position.
    data_ptr_type remaining_data() const noexcept
    {
        return m_data + m_position;
    }

private:
    /// Data pointer to buffer
    data_ptr_type m_data;

    /// The size of the buffer in bytes
    std::size_t m_size;

    /// The current position
    std::size_t m_position = 0;
};
}
}
