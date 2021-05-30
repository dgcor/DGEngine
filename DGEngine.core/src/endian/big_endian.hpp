// Copyright (c) 2018 Steinwurf ApS
// All Rights Reserved
//
// Distributed under the "BSD License". See the accompanying LICENSE.rst file.

#pragma once

#include <cassert>
#include <cstdint>
#include <type_traits>

#include "detail/helpers.hpp"
#include "detail/big.hpp"

namespace endian
{

// Inserts and extracts integers in big-endian format.
struct big_endian
{

    /// Inserts a ValueType-sized value into the data buffer.
    /// @param value to put in the data buffer
    /// @param buffer pointer to the data buffer
    template<class ValueType>
    static void put(ValueType value, uint8_t* buffer)
    {
        assert(buffer != nullptr && "Nullpointer provided");

        detail::big<ValueType, sizeof(ValueType)>::put(value, buffer);
    }

    /// Gets a ValueType-sized integer value from a data buffer.
    /// @param value variable where to get the value
    /// @param buffer pointer to the data buffer
    template<class ValueType>
    static void get(ValueType& value, const uint8_t* buffer)
    {
        assert(buffer != nullptr && "Nullpointer provided");

        value = 0;
        detail::big<ValueType, sizeof(ValueType)>::get(value, buffer);
    }

    /// Gets a ValueType-sized integer value from a data buffer.
    /// @return value variable where to get the value
    /// @param buffer pointer to the data buffer
    template<class ValueType>
    static ValueType get(const uint8_t* buffer)
    {
        assert(buffer != nullptr && "Nullpointer provided");

        ValueType value = 0;
        get(value, buffer);
        return value;
    }

    /// Inserts a Bytes-sized integer value into the data buffer.
    /// @param value to put in the data buffer
    /// @param buffer pointer to the data buffer
    template<uint8_t Bytes, class ValueType>
    static void put_bytes(ValueType value, uint8_t* buffer)
    {
        static_assert(Bytes == sizeof(ValueType) ||
                      std::is_unsigned<ValueType>::value, "Must be unsigned");
        // static_assert(sizeof(ValueType) <= Bytes, "ValueType too large");
        assert(buffer != nullptr && "Nullpointer provided");
        assert((detail::check<ValueType, Bytes>::value(value)) &&
               "Value too big to fit in the provided bytes");

        detail::big<ValueType, Bytes>::put(value, buffer);
    }

    /// Gets a Bytes-sized integer value from a data buffer.
    /// @param value variable where to get the value
    /// @param buffer pointer to the data buffer
    template<uint8_t Bytes, class ValueType>
    static void get_bytes(ValueType& value, const uint8_t* buffer)
    {
        static_assert(Bytes == sizeof(ValueType) ||
                      std::is_unsigned<ValueType>::value, "Must be unsigned");
        static_assert(sizeof(ValueType) >= Bytes, "ValueType too small");
        assert(buffer != nullptr && "Nullpointer provided");

        value = 0;
        detail::big<ValueType, Bytes>::get(value, buffer);
    }

    /// Gets a Bytes-sized integer value from a data buffer.
    /// @return value variable where to get the value
    /// @param buffer pointer to the data buffer
    template<uint8_t Bytes, class ValueType>
    static ValueType get_bytes(const uint8_t* buffer)
    {
        static_assert(Bytes == sizeof(ValueType) ||
                      std::is_unsigned<ValueType>::value, "Must be unsigned");
        static_assert(sizeof(ValueType) >= Bytes, "ValueType too small");
        assert(buffer != nullptr && "Nullpointer provided");

        ValueType value = 0;
        get_bytes<Bytes>(value, buffer);
        return value;
    }

};
}
