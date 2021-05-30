// Copyright (c) 2018 Steinwurf ApS
// All Rights Reserved
//
// Distributed under the "BSD License". See the accompanying LICENSE.rst file.

#pragma once

#include <cassert>
#include <cstdint>
#include <cstring>
#include <limits>
#include <type_traits>

namespace endian
{
namespace detail
{

// Where the actual conversion takes place
template<class ValueType, uint8_t Bytes>
struct big_impl
{
    static void put(ValueType& value, uint8_t* buffer)
    {
        big_impl<ValueType, Bytes - 1>::put(value, buffer + 1);

        value = (value >> 8);
        *buffer = value & 0xFF;
    }

    static void get(ValueType& value, const uint8_t* buffer)
    {
        value |= ((ValueType) *buffer);
        value = (value << 8);

        big_impl<ValueType, Bytes - 1>::get(value, buffer + 1);
    }
};

template<class ValueType>
struct big_impl<ValueType, 1>
{
    static void put(ValueType& value, uint8_t* buffer)
    {
        *buffer = value & 0xFF;
    }

    static void get(ValueType& value, const uint8_t* buffer)
    {
        value |= ((ValueType) *buffer);
    }
};

// Helper to delegate to the appropiate specialization depending on the type
// @TODO remove these wrappers when we have CXX17 support and "if constexpr"
template<class ValueType, uint8_t Bytes,
         bool IsUnsigened = std::is_unsigned<ValueType>::value,
         bool IsFloat = std::is_floating_point<ValueType>::value>
struct big
{
    static void put(ValueType& value, uint8_t* buffer)
    {
        big<ValueType, Bytes, IsUnsigened, IsFloat>::put(value, buffer);
    }

    static void get(ValueType& value, const uint8_t* buffer)
    {
        big<ValueType, Bytes, IsUnsigened, IsFloat>::get(value, buffer);
    }
};

// Unsigned specialization
template<class ValueType, uint8_t Bytes>
struct big<ValueType, Bytes, true, false>
{
    static_assert(
        Bytes > sizeof(ValueType) / 2, "ValueType fits in type of"
        "half the size compared to the provide one, use a smaller type");

    static void put(ValueType& value, uint8_t* buffer)
    {
        assert((check<ValueType, Bytes>::value(value)) &&
               "Value too big to fit in the provided bytes");

        big_impl<ValueType, Bytes>::put(value, buffer);
    }

    static void get(ValueType& value, const uint8_t* buffer)
    {
        big_impl<ValueType, Bytes>::get(value, buffer);
    }
};

// Signed specialization
template<class ValueType, uint8_t Bytes>
struct big<ValueType, Bytes, false, false>
{
    static_assert(
        Bytes == sizeof(ValueType),
        "The number of bytes must match the size of the signed type");

    static void put(ValueType& value, uint8_t* buffer)
    {
        big_impl<ValueType, Bytes>::put(value, buffer);
    }

    static void get(ValueType& value, const uint8_t* buffer)
    {
        big_impl<ValueType, Bytes>::get(value, buffer);
    }
};

// Floating point type specialization
template<class ValueType, uint8_t Bytes>
struct big<ValueType, Bytes, false, true>
{
    static_assert(
        std::numeric_limits<ValueType>::is_iec559,
        "Platform must be iec559 compliant when floating point types are used");

    static_assert(
        Bytes == sizeof(ValueType),
        "The number of bytes must match the size of the floating type");

    static void put(ValueType& value, uint8_t* buffer)
    {
        typename floating_point<ValueType>::UnsignedType temp = 0;
        memcpy(&temp, &value, sizeof(ValueType));
        big_impl<decltype(temp), sizeof(ValueType)>::put(temp, buffer);
    }

    static void get(ValueType& value, const uint8_t* buffer)
    {
        typename floating_point<ValueType>::UnsignedType temp = 0;
        big_impl<decltype(temp), sizeof(ValueType)>::get(temp, buffer);
        memcpy(&value, &temp, sizeof(ValueType));
    }
};

}
}
