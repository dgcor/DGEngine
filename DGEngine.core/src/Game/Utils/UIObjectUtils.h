#pragma once

#include "Game/Variable.h"
#include <string_view>

namespace UIObjectUtils
{
	template <class T, class NumType>
	Variable getTuple2Prop(const T& val, const std::string_view prop)
	{
		if (prop.empty() == true)
		{
			return Variable(std::make_tuple((NumType)val.x, (NumType)val.y));
		}
		else if (prop == "x")
		{
			return Variable((NumType)val.x);
		}
		return Variable((NumType)val.y);
	}

	template <class T>
	Variable getTuple2fProp(const T& val, const std::string_view prop)
	{
		return getTuple2Prop<T, double>(val, prop);
	}

	template <class T>
	Variable getTuple2iProp(const T& val, const std::string_view prop)
	{
		return getTuple2Prop<T, int64_t>(val, prop);
	}
}
