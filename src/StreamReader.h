#pragma once

#include "endian/big_endian.hpp"
#include "endian/little_endian.hpp"
#include "endian/stream_reader.hpp"

typedef endian::stream_reader<endian::big_endian> BigEndianStreamReader;
typedef endian::stream_reader<endian::little_endian> LittleEndianStreamReader;
