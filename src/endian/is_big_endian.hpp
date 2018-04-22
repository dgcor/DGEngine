// Copyright (c) Steinwurf ApS 2016.
// All Rights Reserved
//
// Distributed under the "BSD License". See the accompanying LICENSE.rst file.

#pragma once

#include <cstdint>

namespace endian
{
/// Checks if the platform is big- or little-endian.
///
/// From a test proposed here:
/// http://stackoverflow.com/questions/1001307/
///
/// @return True if the platform is big endian otherwise false.
inline bool is_big_endian()
{
    union
    {
        uint32_t i;
        uint8_t c[4];
    } test = {0x01020304};

    return test.c[0] == 1;
}
}
