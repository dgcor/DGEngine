#pragma once

#include "endian/big_endian.hpp"
#include "endian/little_endian.hpp"
#include "endian/stream_reader.hpp"

using BigEndianStreamReader = endian::stream_reader<endian::big_endian>;
using LittleEndianStreamReader = endian::stream_reader<endian::little_endian>;
