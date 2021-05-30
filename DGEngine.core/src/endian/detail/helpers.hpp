// Copyright (c) 2018 Steinwurf ApS
// All Rights Reserved
//
// Distributed under the "BSD License". See the accompanying LICENSE.rst file.

#pragma once

#include <cstdint>

namespace endian
{
namespace detail
{

// Helper to check that unsigned values can fit in the bytes
template<class ValueType, uint8_t Bytes>
struct check
{
    static bool value(ValueType value)
    {
        (void) value;
        return (sizeof(ValueType) == Bytes);
    }
};

template<uint8_t Bytes>
struct check<uint8_t, Bytes>
{
    static bool value(uint8_t value)
    {
        return value <= 0xFF;
    }
};

template<class ValueType>
struct check<ValueType, 3>
{
    static bool value(ValueType value)
    {
        return value <= 0xFFFFFF;
    }
};

template<class ValueType>
struct check<ValueType, 5>
{
    static bool value(ValueType value)
    {
        return value <= 0xFFFFFFFFFF;
    }
};

template<class ValueType>
struct check<ValueType, 6>
{
    static bool value(ValueType value)
    {
        return value <= 0xFFFFFFFFFFFF;
    }
};

template<class ValueType>
struct check<ValueType, 7>
{
    static bool value(ValueType value)
    {
        return value <= 0xFFFFFFFFFFFFFF;
    }
};

// Helper to convet floating point type into identically sized unsigned integer
template<class Type>
struct floating_point
{ };

template<>
struct floating_point<float>
{
    static_assert(sizeof(float) == 4, "Float type must have a size of 4 bytes");
    using UnsignedType = uint32_t;
};

template<>
struct floating_point<double>
{
    static_assert(sizeof(double) == 8, "Float type must have a size of 8 bytes");
    using UnsignedType = uint64_t;
};

}
}
