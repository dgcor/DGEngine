// Copyright (c) 2016 Steinwurf ApS
// All Rights Reserved
//
// Distributed under the "BSD License". See the accompanying LICENSE.rst file.

#pragma once

#include <algorithm>
#include <cassert>
#include <cstdint>
#include <vector>

#include "stream.hpp"

namespace endian
{
/// The idea behind the stream_writer is to provide a stream-like
/// interface for accessing a fixed-size buffer.
/// All complexity regarding endianness is encapsulated.
template<class EndianType>
class stream_writer : public stream
{
public:

    /// Creates an endian stream on top of a pre-allocated buffer of the
    /// specified size.
    ///
    /// @param data a data pointer to the buffer
    /// @param size the size of the buffer in bytes
    stream_writer(uint8_t* data, uint64_t size) :
        stream(size),
        m_data(data)
    {
        assert(m_data != nullptr && "Invalid buffer pointer provided");
    }

    /// Creates an endian stream on top of a pre-allocated buffer
    ///
    /// @param buffer a vector containing the buffer
    stream_writer(std::vector<uint8_t>& buffer) :
        stream_writer(buffer.data(), buffer.size())
    { }

    /// Writes a Bytes-sized integer to the stream.
    ///
    /// @param value the value to write.
    template<uint8_t Bytes, class ValueType>
    void write_bytes(ValueType value)
    {
        // Make sure there is enough space in the underlying buffer
        assert(Bytes <= remaining_size());

        // Write the value at the current position
        EndianType::template put_bytes<Bytes>(value, remaining_data());

        // Advance the current position
        m_position += Bytes;
    }

    /// Writes a Bytes-sized integer to the stream.
    ///
    /// @param value the value to write.
    template<class ValueType>
    void write(ValueType value)
    {
        // Make sure there is enough space in the underlying buffer
        assert(sizeof(ValueType) <= remaining_size());
        write_bytes<sizeof(ValueType), ValueType>(value);
    }

    /// Writes the raw bytes represented by the storage::const_storage
    /// object to the stream.
    ///
    /// Note, that this function is provided only for convenience and
    /// it does not perform any endian conversions.
    ///
    /// @param data Pointer to the data, to be written to the stream.
    /// @param size Number of bytes from the data pointer.
    void write(const uint8_t* data, uint64_t size)
    {
        // Make sure there is enough space in the underlying buffer
        assert(size <= remaining_size());

        // Copy the data to the buffer
        std::copy_n(data, (std::size_t)size, remaining_data());

        // Advance the current position
        m_position += size;
    }

    /// A pointer to the stream's data.
    ///
    /// @return pointer to the stream's data.
    uint8_t* data() const
    {
        return m_data;
    }

    /// A pointer to the stream's data at the current position.
    ///
    /// @return pointer to the stream's data at the current position.
    uint8_t* remaining_data() const
    {
        return m_data + m_position;
    }

private:

    /// Data pointer to buffer
    uint8_t* m_data;
};
}
