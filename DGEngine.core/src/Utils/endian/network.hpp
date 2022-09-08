// Copyright (c) 2016 Steinwurf ApS
// All Rights Reserved
//
// Distributed under the "BSD License". See the accompanying LICENSE.rst file.

#pragma once
#include "big_endian.hpp"

namespace endian
{
/// https://en.wikipedia.org/wiki/Endianness#Networking
/// "Many IETF RFCs use the term network order, meaning the order of
/// transmission for bits and bytes over the wire in network protocols. Among
/// others, the historic RFC 1700 (also known as Internet standard STD 2) has
/// defined the network order for protocols in the Internet protocol suite to be
/// big-endian, hence the use of the term "network byte order" for big-endian
/// byte order."
///
/// https://datatracker.ietf.org/doc/html/rfc1700#page-3
using network = big_endian;
}
