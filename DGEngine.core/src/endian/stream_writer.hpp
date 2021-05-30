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
/// The stream_writer provides a stream-like interface for writing to a fixed
/// size buffer. All complexity regarding endianness is encapsulated.
template<typename EndianType>
class stream_writer : public detail::stream<uint8_t*>
{
public:

    /// Creates an endian stream on top of a pre-allocated buffer of the
    /// specified size.
    ///
    /// @param data a data pointer to the buffer
    /// @param size the size of the buffer in bytes
    stream_writer(uint8_t* data, std::size_t size) noexcept :
        stream(data, size)
    { }

    /// Writes a Bytes-sized integer to the stream.
    ///
    /// @param value the value to write.
    template<uint8_t Bytes, class ValueType>
    void write_bytes(ValueType value) noexcept
    {
        assert(Bytes <= remaining_size());

        EndianType::template put_bytes<Bytes>(value, this->remaining_data());
        skip(Bytes);
    }

    /// Writes a Bytes-sized integer to the stream.
    ///
    /// @param value the value to write.
    template<class ValueType>
    void write(ValueType value) noexcept
    {
        assert(sizeof(ValueType) <= remaining_size());

        write_bytes<sizeof(ValueType), const ValueType>(value);
    }

    /// Writes the raw bytes represented by the storage::const_storage
    /// object to the stream.
    ///
    /// Note, that this function is provided only for convenience and
    /// it does not perform any endian conversions.
    ///
    /// @param data Pointer to the data, to be written to the stream.
    /// @param size Number of bytes from the data pointer.
    void write(const uint8_t* data, std::size_t size) noexcept
    {
        assert(size <= remaining_size());

        std::copy_n(data, size, this->remaining_data());
        skip(size);
    }
};
}
