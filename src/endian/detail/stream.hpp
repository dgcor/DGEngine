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

/// @brief Base-class for the endian stream reader and writer.
template<typename DataPointerType>
class stream
{
public:

    /// Type for holding the size of the data.
    using size_type = uint64_t;

private:

    using data_type = typename std::remove_pointer<DataPointerType>::type;

    using non_const_data_type = typename std::remove_const<data_type>::type;

    static_assert(std::is_same<uint8_t, non_const_data_type>::value,
                  "Only uint8_t allowed.");

    static_assert(std::is_pointer<DataPointerType>::value,
                  "The template type must be a pointer type");

    static_assert(
        std::numeric_limits<size_type>::max() >=
        std::numeric_limits<std::vector<uint8_t>::size_type>::max(),
        "std::vector::size() can return a value we cannot hold in size_type");

public:

    /// Creates an endian stream used to track a buffer of the specified size.
    ///
    /// @param size the size of the buffer in bytes
    stream(DataPointerType data, size_type size) noexcept :
        m_data(data), m_size(size)
    {
        assert(data != nullptr && "Null pointer provided");
        assert(size > 0 && "Empty buffer provided");
    }

    /// Gets the size of the underlying buffer in bytes.
    ///
    /// @return the size of the buffer
    size_type size() const noexcept
    {
        return m_size;
    }

    /// Gets the current read/write position in the stream
    ///
    /// @return the current position.
    size_type position() const noexcept
    {
        return m_position;
    }

    /// The remaining number of bytes in the stream
    ///
    /// @return the remaining number of bytes.
    size_type remaining_size() const noexcept
    {
        return m_size - m_position;
    }

    /// Changes the current read/write position in the stream. The
    /// position is absolute i.e. it is always relative to the
    /// beginning of the buffer which is position 0.
    ///
    /// @param new_position the new position
    void seek(size_type new_position) noexcept
    {
        assert(new_position <= m_size);

        m_position = new_position;
    }

    /// Skips over a given number of bytes in the stream
    ///
    /// @param bytes_to_skip the bytes to skip
    void skip(size_type bytes_to_skip) noexcept
    {
        assert(bytes_to_skip <= m_size - m_position);

        seek(m_position + bytes_to_skip);
    }

    /// A pointer to the stream's data.
    ///
    /// @return pointer to the stream's data.
    DataPointerType data() const noexcept
    {
        return m_data;
    }

    /// A pointer to the stream's data at the current position.
    ///
    /// @return pointer to the stream's data at the current position.
    DataPointerType remaining_data() const noexcept
    {
        return m_data + m_position;
    }

private:

    /// Data pointer to buffer
    DataPointerType m_data;

    /// The size of the buffer in bytes
    size_type m_size;

    /// The current position
    size_type m_position = 0;
};
}
}
