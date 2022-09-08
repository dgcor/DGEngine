#pragma once

#include "Game/Variable.h"
#include <type_traits>

namespace VarUtils
{
	bool toBool(const Variable& var) noexcept;

	double toDouble(const Variable& var) noexcept;

	int64_t toInt64(const Variable& var) noexcept;

	std::string toString(const Variable& var);

	template<typename T>
	struct is_variant : std::false_type {};

	template<typename ...Args>
	struct is_variant<std::variant<Args...>> : std::true_type {};

	template <class T, class NumType>
	T getVector2NumberVal(const Variable& var, const T& val = {})
	{
		static_assert(std::is_arithmetic<NumType>::value == true);

		if (std::holds_alternative<Int64Tuple2>(var))
		{
			return T(
				(NumType)std::get<0>(std::get<Int64Tuple2>(var)),
				(NumType)std::get<1>(std::get<Int64Tuple2>(var))
			);
		}
		else if (std::holds_alternative<DoubleTuple2>(var))
		{
			return T(
				(NumType)std::get<0>(std::get<DoubleTuple2>(var)),
				(NumType)std::get<1>(std::get<DoubleTuple2>(var))
			);
		}
		return val;
	}

	template <class T>
	T getVector2fVal(const Variable& elem, const T& val = {})
	{
		return getVector2NumberVal<T, float>(elem, val);
	}

	template <class T>
	T getVector2iVal(const Variable& elem, const T& val = {})
	{
		return getVector2NumberVal<T, int32_t>(elem, val);
	}

	template <class T>
	T getVector2uVal(const Variable& elem, const T& val = {})
	{
		return getVector2NumberVal<T, uint32_t>(elem, val);
	}

	template <class T, class NumType>
	T getVector3NumberVal(const Variable& var, const T& val = {})
	{
		static_assert(std::is_arithmetic<NumType>::value == true);

		if (std::holds_alternative<Int64Tuple3>(var))
		{
			return T(
				(NumType)std::get<0>(std::get<Int64Tuple3>(var)),
				(NumType)std::get<1>(std::get<Int64Tuple3>(var)),
				(NumType)std::get<2>(std::get<Int64Tuple3>(var))
			);
		}
		else if (std::holds_alternative<DoubleTuple3>(var))
		{
			return T(
				(NumType)std::get<0>(std::get<DoubleTuple3>(var)),
				(NumType)std::get<1>(std::get<DoubleTuple3>(var)),
				(NumType)std::get<2>(std::get<DoubleTuple3>(var))
			);
		}
		return val;
	}

	template <class T>
	T getVector3fVal(const Variable& elem, const T& val = {})
	{
		return getVector3NumberVal<T, float>(elem, val);
	}

	template <class T>
	T getVector3iVal(const Variable& elem, const T& val = {})
	{
		return getVector3NumberVal<T, int32_t>(elem, val);
	}

	template <class T>
	T getVector3uVal(const Variable& elem, const T& val = {})
	{
		return getVector3NumberVal<T, uint32_t>(elem, val);
	}

	template <class T, class NumType>
	T getVector4NumberVal(const Variable& var, const T& val = {})
	{
		static_assert(std::is_arithmetic<NumType>::value == true);

		if (std::holds_alternative<Int64Tuple4>(var))
		{
			return T(
				(NumType)std::get<0>(std::get<Int64Tuple4>(var)),
				(NumType)std::get<1>(std::get<Int64Tuple4>(var)),
				(NumType)std::get<2>(std::get<Int64Tuple4>(var)),
				(NumType)std::get<3>(std::get<Int64Tuple4>(var))
			);
		}
		else if (std::holds_alternative<DoubleTuple4>(var))
		{
			return T(
				(NumType)std::get<0>(std::get<DoubleTuple4>(var)),
				(NumType)std::get<1>(std::get<DoubleTuple4>(var)),
				(NumType)std::get<2>(std::get<DoubleTuple4>(var)),
				(NumType)std::get<3>(std::get<DoubleTuple4>(var))
			);
		}
		return val;
	}

	template <class T>
	T getVector4fVal(const Variable& elem, const T& val = {})
	{
		return getVector4NumberVal<T, float>(elem, val);
	}

	template <class T>
	T getVector4iVal(const Variable& elem, const T& val = {})
	{
		return getVector4NumberVal<T, int32_t>(elem, val);
	}

	template <class T>
	T getVector4uVal(const Variable& elem, const T& val = {})
	{
		return getVector4NumberVal<T, uint32_t>(elem, val);
	}
}
