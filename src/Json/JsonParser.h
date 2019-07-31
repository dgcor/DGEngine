#pragma once

#define RAPIDJSON_HAS_STDSTRING 1
#define RAPIDJSON_HAS_STDSTRINGVIEW 1

#include "rapidjson/document.h"
#include "rapidjson/pointer.h"
#include "rapidjson/prettywriter.h"
#include "rapidjson/stringbuffer.h"
#include "rapidjson/writer.h"

namespace rapidjson
{
	template <typename Encoding, typename Allocator>
	typename GenericValue<Encoding, Allocator>::ValueIterator begin(GenericValue<Encoding, Allocator>& v) { return v.Begin(); }
	template <typename Encoding, typename Allocator>
	typename GenericValue<Encoding, Allocator>::ConstValueIterator begin(const GenericValue<Encoding, Allocator>& v) { return v.Begin(); }

	template <typename Encoding, typename Allocator>
	typename GenericValue<Encoding, Allocator>::ValueIterator end(GenericValue<Encoding, Allocator>& v) { return v.End(); }
	template <typename Encoding, typename Allocator>
	typename GenericValue<Encoding, Allocator>::ConstValueIterator end(const GenericValue<Encoding, Allocator>& v) { return v.End(); }
}
