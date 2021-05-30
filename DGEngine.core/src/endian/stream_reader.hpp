// Copyright (c) 2016 Steinwurf ApS
// All Rights Reserved
//
// Distributed under the "BSD License". See the accompanying LICENSE.rst file.

#pragma once

#include <algorithm>
#include <cassert>
#include <cstdint>

#include "detail/stream.hpp"

namespace endian
{
/// The stream_reader provides a stream-like interface for reading from a
/// fixed-size buffer. All complexity regarding endianness is encapsulated.
template<typename EndianType>
class stream_reader : public detail::stream<const uint8_t*>
{
public:

    /// Creates an endian stream on top of a pre-allocated buffer of the
    /// specified size.
    ///
    /// @param data a data pointer to the buffer
    /// @param size the size of the buffer in bytes
    stream_reader(const uint8_t* data, std::size_t size) noexcept :
        stream(data, size)
    { }

    /// Reads a Bytes-sized integer from the stream and moves the read position.
    ///
    /// @param value reference to the value to be read
    template<uint8_t Bytes, class ValueType>
    void read_bytes(ValueType& value) noexcept
    {
        assert(Bytes <= remaining_size() &&
               "Reading over the end of the underlying buffer");

        peek_bytes<Bytes, ValueType>(value);
        skip(Bytes);
    }

    /// Reads a ValueType-sized integer from the stream and moves the read
    /// position.
    ///
    /// @param value reference to the value to be read
    template<class ValueType>
    void read(ValueType& value) noexcept
    {
        assert(sizeof(ValueType) <= remaining_size() &&
               "Reading over the end of the underlying buffer");

        read_bytes<sizeof(ValueType), ValueType>(value);
    }

    /// Reads a ValueType-sized integer from the stream and moves the read
    /// position.
    ///
    /// @return the read value
    template<class ValueType>
    ValueType read() noexcept
    {
        assert(sizeof(ValueType) <= remaining_size() &&
               "Reading over the end of the underlying buffer");

        ValueType value;
        read(value);
        return value;
    }

    /// Reads raw bytes from the stream to fill a buffer represented by
    /// a mutable storage object.
    ///
    /// Note, that this function is provided only for convenience and
    /// it does not perform any endian conversions.
    ///
    /// @param data The data pointer to fill into
    /// @param size The number of bytes to fill.
    void read(uint8_t* data, std::size_t size) noexcept
    {
        assert(size <= remaining_size() &&
               "Reading over the end of the underlying buffer");

        std::copy_n(remaining_data(), size, data);
        skip(size);
    }

    /// Peek a Bytes-sized integer in the stream without moving the read
    /// position
    ///
    /// @param value reference to the value to be read
    /// @param offset number of bytes to offset the peeking with
    template<uint8_t Bytes, class ValueType>
    void peek_bytes(ValueType& value, std::size_t offset=0) const noexcept
    {
        assert(remaining_size() >= offset && "Offset too large");
        assert(Bytes <= remaining_size() - offset &&
               "Reading over the end of the underlying buffer");

        const uint8_t* data_position = remaining_data() + offset;
        EndianType::template get_bytes<Bytes>(value, data_position);
    }

    /// Peek a ValueType-sized integer in the stream without moving the read
    /// position
    ///
    /// @param value reference to the value to be read
    /// @param offset number of bytes to offset the peeking with
    template<class ValueType>
    void peek(ValueType& value, std::size_t offset=0) const noexcept
    {
        assert(remaining_size() >= offset && "Offset too large");
        assert(sizeof(ValueType) <= remaining_size() - offset &&
               "Reading over the end of the underlying buffer");

        peek_bytes<sizeof(ValueType), ValueType>(value, offset);
    }

    /// Peek a ValueType-sized integer in the stream without moving the read
    /// position
    ///
    /// @param offset number of bytes to offset the peeking with
    /// @return the peeked value
    template<class ValueType>
    ValueType peek(std::size_t offset=0) const noexcept
    {
        assert(remaining_size() >= offset && "Offset too large");
        assert(sizeof(ValueType) <= remaining_size() - offset &&
               "Reading over the end of the underlying buffer");

        ValueType value;
        peek(value, offset);
        return value;
    }
};
}
