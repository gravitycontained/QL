#pragma once
#ifndef QL_HPP
#define QL_HPP

//QL.hpp - "Quick Library"
//github.com/DanielRabl/QL
//written by Daniel Rabl
//created 2021 March, 10
// 
//mini version of QPL: github.com/DanielRabl/QPL

#ifndef QL_NO_SFML
#include <SFML/Graphics.hpp>

////////////////////////////////////////////////////////////
//
// SFML - Simple and Fast Multimedia Library
// Copyright (C) 2007-2018 Laurent Gomila (laurent@sfml-dev.org)
//
// This software is provided 'as-is', without any express or implied warranty.
// In no event will the authors be held liable for any damages arising from the use of this software.
//
// Permission is granted to anyone to use this software for any purpose,
// including commercial applications, and to alter it and redistribute it freely,
// subject to the following restrictions:
//
// 1. The origin of this software must not be misrepresented;
//    you must not claim that you wrote the original software.
//    If you use this software in a product, an acknowledgment
//    in the product documentation would be appreciated but is not required.
//
// 2. Altered source versions must be plainly marked as such,
//    and must not be misrepresented as being the original software.
//
// 3. This notice may not be removed or altered from any source distribution.
//
////////////////////////////////////////////////////////////
#endif

#include <algorithm>
#include <array>
#include <chrono>
#include <cstdint>
#include <filesystem>
#include <fstream>
#include <iostream>
#include <limits>
#include <random>
#include <set>
#include <span>
#include <sstream>
#include <string>
#include <string_view>
#include <type_traits>
#include <unordered_map>

namespace ql {

	//type_traits
	// - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - 

	using u8 = std::uint8_t;
	using i8 = std::int8_t;
	using u16 = std::uint16_t;
	using i16 = std::int16_t;
	using u32 = std::uint32_t;
	using i32 = std::int32_t;
	using u64 = std::uint64_t;
	using i64 = std::int64_t;

	using f32 = float;
	using f64 = double;
	using size = std::size_t;

	constexpr i8 i8_min = std::numeric_limits<i8>::min();
	constexpr i8 i8_max = std::numeric_limits<i8>::max();
	constexpr u8 u8_min = std::numeric_limits<u8>::min();
	constexpr u8 u8_max = std::numeric_limits<u8>::max();

	constexpr i16 i16_min = std::numeric_limits<i16>::min();
	constexpr i16 i16_max = std::numeric_limits<i16>::max();
	constexpr u16 u16_min = std::numeric_limits<u16>::min();
	constexpr u16 u16_max = std::numeric_limits<u16>::max();

	constexpr i32 i32_min = std::numeric_limits<i32>::min();
	constexpr i32 i32_max = std::numeric_limits<i32>::max();
	constexpr u32 u32_min = std::numeric_limits<u32>::min();
	constexpr u32 u32_max = std::numeric_limits<u32>::max();

	constexpr i64 i64_min = std::numeric_limits<i64>::min();
	constexpr i64 i64_max = std::numeric_limits<i64>::max();
	constexpr u64 u64_min = std::numeric_limits<u64>::min();
	constexpr u64 u64_max = std::numeric_limits<u64>::max();

	constexpr f32 f32_min = std::numeric_limits<f32>::lowest();
	constexpr f32 f32_min_zero = std::numeric_limits<f32>::min();
	constexpr f32 f32_max = std::numeric_limits<f32>::max();
	constexpr f64 f64_min = std::numeric_limits<f64>::lowest();
	constexpr f64 f64_min_zero = std::numeric_limits<f64>::min();
	constexpr f64 f64_max = std::numeric_limits<f64>::max();

	constexpr ql::f64 f64_epsilon = std::numeric_limits<ql::f64>::epsilon();
	constexpr ql::f64 f32_epsilon = std::numeric_limits<ql::f32>::epsilon();

	constexpr ql::size f64_digits = std::numeric_limits<ql::f64>::max_digits10;
	constexpr ql::size f32_digits = std::numeric_limits<ql::f32>::max_digits10;

	constexpr ql::f64 pi      = 3.14159265358979323846;
	constexpr ql::f64 sqrt2   = 1.41421356237309504880;
	constexpr ql::f64 sqrt3   = 1.73205080756887729352;
	constexpr ql::f64 sqrt5   = 2.23606797749978969640;
	constexpr ql::f64 sqrt1_2 = 0.70710678118654752440;
	constexpr ql::f64 e       = 2.71828182845904523536;
	constexpr ql::f64 ln2     = 0.69314718055994530941;
	constexpr ql::f64 ln10    = 2.30258509299404568402;
	constexpr ql::f64 phi     = 1.61803398874989484820;

	constexpr ql::size f32_mantissa_size() {
		return 23;
	}
	constexpr ql::size f32_exponent_size() {
		return 8;
	}
	constexpr ql::size f64_mantissa_size() {
		return 52;
	}
	constexpr ql::size f64_exponent_size() {
		return 11;
	}

	template <typename T>
	constexpr auto type_min() {
		return std::numeric_limits<T>::min();
	}
	template <typename T>
	constexpr auto type_max() {
		return std::numeric_limits<T>::max();
	}


	template<typename T>
	constexpr inline ql::size bytes_in_type() {
		return sizeof(T);
	}
	template<typename T>
	constexpr inline ql::size bits_in_type() {
		return sizeof(T) * CHAR_BIT;
	}

	template<typename T1, typename T2>
	using arithmetic_result_type = decltype(std::declval<T1>() + std::declval<T2>());

	template<typename T, typename ... Args>
	using first_type = T;

	using error_type = std::enable_if<false>;

	template<typename T>
	struct identity {
		using type = typename T;
	};

	template<typename T>
	concept is_arithmetic = std::is_arithmetic_v<T>;

	template<typename T>
	concept is_integer = std::numeric_limits<T>::is_integer;

	template<typename T>
	concept is_signed = std::numeric_limits<T>::is_signed;

	template<typename T>
	concept is_unsigned = std::numeric_limits<T>::is_unsigned;

	template<typename T>
	concept is_floating_point = std::is_floating_point_v<T>;

	template<ql::size bits, bool sign>
	using int_type = typename
		std::conditional_t<bits == 8u,  std::conditional_t<sign, ql::identity<ql::i8>,	ql::identity<ql::u8>>,
		std::conditional_t<bits == 16u, std::conditional_t<sign, ql::identity<ql::i16>,	ql::identity<ql::u16>>,
		std::conditional_t<bits == 32u, std::conditional_t<sign, ql::identity<ql::i32>,	ql::identity<ql::u32>>,
		std::conditional_t<bits == 64u, std::conditional_t<sign, ql::identity<ql::i64>,	ql::identity<ql::u64>>,
		ql::error_type>>>>::type;

	template<ql::size bits>
	using float_type = typename
		std::conditional_t<bits == 32u,  ql::identity<ql::f32>,
		std::conditional_t<bits == 64u,  ql::identity<ql::f64>,
		ql::error_type>>::type;

	template<typename T> requires ql::is_integer<T>
	using signed_type = typename
			std::conditional_t<ql::bits_in_type<T>() == 8u,  ql::identity<ql::i8>,
			std::conditional_t<ql::bits_in_type<T>() == 16u, ql::identity<ql::i16>,
			std::conditional_t<ql::bits_in_type<T>() == 32u, ql::identity<ql::i32>,
			std::conditional_t<ql::bits_in_type<T>() == 64u, ql::identity<ql::i64>,
			ql::error_type>>>>::type;

	template<typename T> requires ql::is_integer<T>
	using unsigned_type = typename
			std::conditional_t<ql::bits_in_type<T>() == 8u,  ql::identity<ql::u8>,
			std::conditional_t<ql::bits_in_type<T>() == 16u, ql::identity<ql::u16>,
			std::conditional_t<ql::bits_in_type<T>() == 32u, ql::identity<ql::u32>,
			std::conditional_t<ql::bits_in_type<T>() == 64u, ql::identity<ql::u64>,
			ql::error_type>>>>::type;

	template<typename T>
	constexpr auto signed_cast(T value) {
		return static_cast<signed_type<T>>(value);
	}
	template<typename T>
	constexpr auto unsigned_cast(T value) {
		return static_cast<unsigned_type<T>>(value);
	}

	template<typename T>
	concept is_container = requires(T a, const T b) {
		a.begin();
		a.end();
		b.cbegin();
		b.cend();
	};
	template<typename T>
	concept is_contiguous_container = requires(T a, const T b) {
		requires is_container<T>;
		a.data();
	};
	template<typename T>
	concept is_vector_like = requires(T a, const T b) {
		requires is_contiguous_container<T>;
		a.resize(0u);
	};

	template<typename C> requires is_container<C>
	using container_subtype = std::decay_t<decltype(*(std::declval<C>().begin()))>;


	template<typename T>
	std::string type_name(T value) {
		return std::string(typeid(T).name());
	}
	template<typename T>
	std::string type_name() {
		return std::string(typeid(T).name());
	}


	template<typename T, typename U>
	constexpr bool is_same() {
		return std::is_same_v<T, U>;
	}
	template<typename T, typename U>
	constexpr bool is_same_decayed() {
		return std::is_same_v<std::decay_t<T>, std::decay_t<U>>;
	}


	template<typename T, typename U, typename... Args>
	constexpr bool all_equal() {
		return ql::is_same<T, U>() && (ql::is_same<T, Args>() && ...);
	}
	template<typename T, typename U, typename... Args>
	constexpr bool all_equal_decayed() {
		return ql::is_same_decayed<T, U>() && (ql::is_same_decayed<T, Args>() && ...);
	}
	template<class compare, class... Args>
	constexpr bool is_equal_to_any() {
		return (ql::is_same<compare, Args>() || ...);
	}
	template<class compare, class... Args>
	constexpr bool is_equal_to_any_decayed() {
		return (ql::is_same_decayed<compare, Args>() || ...);
	}


	template<typename T>
	constexpr bool is_standard_string_type() {
		return ql::is_equal_to_any_decayed<T, char, const char*, char*, const char[], std::string, std::string_view>();
	}
	template<typename T>
	constexpr bool is_wstring_type() {
		return ql::is_equal_to_any_decayed<T, wchar_t, const wchar_t*, wchar_t*, const wchar_t[], std::wstring, std::wstring_view>();
	}

	template<typename T>
	constexpr bool is_string_type() {
		return is_standard_string_type<T>() || is_wstring_type<T>();
	}

	template<typename T>
	constexpr bool is_standard_string_type(T value) {
		return is_standard_string_type<T>();
	}
	template<typename T>
	constexpr bool is_wstring_type(T value) {
		return is_wstring_type<T>();
	}
	template<typename T>
	constexpr bool is_string_type(T value) {
		return is_string_type<T>();
	}

	template<typename... Args>
	constexpr bool is_any_standard_string_type(Args... args) {
		return (ql::is_standard_string_type<Args>() || ...);
	}
	template<typename... Args>
	constexpr bool is_any_wstring_type(Args... args) {
		return (ql::is_wstring_type<Args>() || ...);
	}
	template<typename... Args>
	constexpr bool is_any_string_type(Args... args) {
		return (ql::is_string_type<Args>() || ...);
	}

	//string / io
	// - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - 

	template<typename ... Args>
	std::string to_string(Args... args) {
		std::ostringstream stream;

		((stream << args), ...);

		return stream.str();
	}
	template<typename ... Args>
	const char* to_cstring(Args... args) {
		return ql::to_string(args...).c_str();
	}

	template<typename ... Args>
	std::wstring to_wstring(Args... args) {
		std::wostringstream stream;

		((stream << args), ...);

		return stream.str();
	}
	template<typename ... Args>
	const wchar_t* to_cswtring(Args... args) {
		return ql::to_wstring(args...).c_str();
	}


	std::string appended_to_string(const std::string& string, char append, ql::size length) {
		return to_string(string, std::string(string.length() >= length ? 0ull : length - string.length(), append));
	}
	std::string prepended_to_string(const std::string& string, char append, ql::size length) {
		return to_string(std::string(string.length() >= length ? 0ull : length - string.length(), append), string);
	}

	template<typename... Args>
	std::string to_string_precision(ql::size precision, Args&&... args) {
		std::ostringstream stream;
		((stream << std::fixed << std::setprecision(precision) << (ql::is_integer<decltype(args)> ? static_cast<ql::f64>(args) : args)), ...);
		return stream.str();
	}
	template<typename... Args>
	std::string to_string_full_precision(Args&&... args) {
		std::ostringstream stream;
		((stream << std::fixed << std::setprecision(ql::f64_digits) << (ql::is_integer<decltype(args)> ? static_cast<ql::f64>(args) : args)), ...);
		return stream.str();
	}

	std::string bool_string(bool check) {
		return (check ? "true" : "false");
	}

	template<typename T>
	void print(T arg) {
		if constexpr (ql::is_wstring_type<T>()) {
			std::wcout << arg;
		}
		else {
			std::cout << arg;
		}
	}
	template<typename ... Args>
	void print(Args... args) {
		(ql::print(args), ...);
	}
	template<typename ... Args>
	void println(Args... args) {
		ql::print(args...);
		ql::print('\n');
	}


	template<typename C> requires ql::is_container<C>
	std::string container_to_string(C& data, const std::string_view& format = "(a, b)") {
			if (format.length() < 4u) {
				throw std::exception(ql::to_cstring("ql::container_to_string<", ql::type_name<C>(), ">: format \"", format, "\" is invalid."));
			}
			auto sub_size = format.length() - 4u;
			std::ostringstream stream;
			stream << format.front();
			bool first = true;
			for (auto& i : data) {
				if (!first) {
					stream << format.substr(2, sub_size);
				}
				first = false;
				stream << i;
			}
			stream << format.back();
			return stream.str();
		}
	template<typename C> requires ql::is_container<C>
	void print_container(C& data, const std::string_view& format = "(a, b)") {
		ql::print(ql::container_to_string(data, format));
	}
	template<typename C> requires ql::is_container<C>
	void println_container(C& data, const std::string_view& format = "(a, b)") {
		ql::println(ql::container_to_string(data, format));
	}
	
	//algorithms
	// - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - 

	template<typename T, typename ... Args>
	constexpr auto to_array_t(Args... args) {
		std::array<T, sizeof...(Args)> result{ static_cast<T>(args)... };

		return result;
	}
	template<typename ... Args>
	constexpr auto to_array(Args... args) {
		using type = first_type<Args...>;
		std::array<type, sizeof...(Args)> result{ static_cast<type>(args)... };

		return result;
	}

	template<typename C> requires ql::is_contiguous_container<C>
	void fill_container(C& data, ql::container_subtype<C> fill_value = 0) {
		memset(data.data(), static_cast<ql::i32>(fill_value), data.size() * sizeof(ql::container_subtype<C>));
	}


	template<typename T1, typename T2>
	constexpr auto min(T1 a, T2 b) {
		return (a < b ? a : b);
	}
	template<typename T, typename ... Args>
	constexpr auto min(T first, Args... args) {
		return ql::min(ql::min(first, args)...);
	}

	template<typename T1, typename T2>
	constexpr auto max(T1 a, T2 b) {
		return (a > b ? a : b);
	}
	template<typename T, typename ... Args>
	constexpr auto max(T first, Args... args) {
		return ql::max(ql::max(first, args)...);
	}

	template<typename T>
	constexpr T abs(T value) {
		if constexpr (ql::is_signed<T>) {
			return value;
		}
		else {
			return (value < T{ 0 } ? -value : value);
		}
	}

	template<typename T, typename U>
	constexpr auto div_mod(T a, U b) {
		auto div = a / b;
		auto mod = a % b;
		return std::make_pair(div, mod);
	}

	template<typename T>
	constexpr T clamp(T min, T value, T max) {
		return ql::min(max, ql::max(min, value));
	}
	template<typename T>
	constexpr T clamp_0_1(T value) {
		return ql::min(T{ 1 }, ql::max(T{ 0 }, value));
	}

	template<typename T> requires is_arithmetic<T>
	constexpr ql::size number_of_digits(T value, T base = T{ 10 }) {
		return value < base ? ql::size{ 1 } : ql::size{ 1 } + number_of_digits(value / base, base);
	}

	template<typename T, typename F>
	constexpr auto linear_interpolation(T a, T b, F delta) {
		return a * (F{ 1 } - delta) + (b * delta);
	}

	template<typename T, typename F>
	constexpr auto linear_interpolation_2D(T xx, T xy, T yx, T yy, F fx, F fy) {
		return ql::linear_interpolation(ql::linear_interpolation(xx, xy, fx), ql::linear_interpolation(yx, yy, fx), fy);
	}

	template<typename T, typename F>
	constexpr auto cubic_interpolation(T a, T b, T c, T d, F delta) {
		return
			(d - c - a + b) * (delta * delta * delta) +
			(a * 2 - b * 2 - d + c) * (delta * delta) +
			(c - a) * delta + b;
	}
	template<typename T, typename F>
	constexpr auto cubic_interpolation(T a, T b, F delta) {
		return ql::cubic_interpolation(a, a, b, b, delta);
	}
	template<typename T, typename F>
	constexpr auto cubic_interpolation_2D(
		T a1, T b1, T c1, T d1,
		T a2, T b2, T c2, T d2,
		T a3, T b3, T c3, T d3,
		T a4, T b4, T c4, T d4,
		F fx, F fy) {
		return ql::cubic_interpolation(
			ql::cubic_interpolation(a1, b1, c1, d1, fx),
			ql::cubic_interpolation(a2, b2, c2, d2, fx),
			ql::cubic_interpolation(a3, b3, c3, d3, fx),
			ql::cubic_interpolation(a4, b4, c4, d4, fx), fy);
	}

	template<typename C, typename F> requires ql::is_container<C>
	ql::container_subtype<C> linear_container_interpolation(const C& data, F progress) {
		if (data.empty()) {
			return {};
		}

		progress = ql::clamp_0_1(progress);
		auto index = static_cast<ql::u32>(progress * (data.size() - 1));

		ql::container_subtype<C> a = data[index];
		ql::container_subtype<C> b = a;

		if (index < data.size() - 1) {
			b = data[index + 1];
		}

		auto left_over = ((data.size() - 1) * progress) - index;

		return ql::linear_interpolation(a, b, left_over);
	}

	template<typename C, typename F> requires ql::is_container<C>
	ql::container_subtype<C> cubic_container_interpolation(const C& data, F progress) {
		if (data.empty()) {
			return {};
		}

		progress = ql::clamp_0_1(progress);
		auto index = static_cast<ql::u32>(progress * (data.size() - 1));

		ql::container_subtype<C> a, b, c, d;
		c = a = b = data[index];
		if (index >= 1u) {
			a = data[index - 1];
		}

		if (index < (data.size() - 1)) {
			c = data[index + 1];
		}
		d = c;

		if (index < (data.size() - 2)) {
			d = data[index + 2];
		}

		auto left_over = ((data.size() - 1) * progress) - index;

		return ql::cubic_interpolation(a, b, c, d, left_over);
	}

	//time
	// - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - 

	class time {
	public:
		static time clock_time() {
			return time{
				static_cast<ql::u64>(
					std::chrono::duration_cast<std::chrono::nanoseconds>(std::chrono::high_resolution_clock::now().time_since_epoch()).count())
			};
		}

		constexpr time(ql::u64 ns = ql::u64{}) noexcept : m_ns(ns) {

		}
		time& operator=(const time& other) {
			this->m_ns = other.m_ns;
			return *this;
		}
		time& operator+=(const time& other) {
			this->m_ns += other.m_ns;
			return *this;
		}
		time operator+(const time& other) const {
			auto copy = *this;
			copy += other;
			return copy;
		}
		time& operator-=(const time& other) {
			if (other.m_ns >= this->m_ns) {
				this->m_ns = other.m_ns - this->m_ns;
			}
			else {
				this->m_ns -= other.m_ns;
			}
			return *this;
		}
		time operator-(const time& other) const {
			auto copy = *this;
			copy -= other;
			return copy;
		}
		time& operator/=(const time& other) {
			this->m_ns /= other.m_ns;
			return *this;
		}
		time operator/(const time& other) const {
			auto copy = *this;
			copy /= other;
			return copy;
		}
		time& operator%=(const time& other) {
			this->m_ns %= other.m_ns;
			return *this;
		}
		time operator%(const time& other) const {
			auto copy = *this;
			copy %= other;
			return copy;
		}

		bool operator==(const time& other) const {
			return this->m_ns == other.m_ns;
		}
		bool operator!=(const time& other) const {
			return this->m_ns != other.m_ns;
		}
		bool operator<(const time& other) const {
			return this->m_ns < other.m_ns;
		}
		bool operator<=(const time& other) const {
			return this->m_ns <= other.m_ns;
		}
		bool operator>(const time& other) const {
			return this->m_ns > other.m_ns;
		}
		bool operator>=(const time& other) const {
			return this->m_ns >= other.m_ns;
		}

		std::string string_non_zero() const {
			std::ostringstream stream;
			bool found = false;

			stream << '[';
			if (this->years()) {
				found = true;
				stream << this->years() << 'y';
			}
			if (this->days_mod()) {
				if (found) {
					stream << " : ";
				}
				found = true;
				stream << ql::prepended_to_string(ql::to_string(this->days_mod()), '0', ql::number_of_digits(this->days_in_year - 1)) << 'd';
			}
			if (this->hours_mod()) {
				if (found) {
					stream << " : ";
				}
				found = true;
				stream << ql::prepended_to_string(ql::to_string(this->hours_mod()), '0', ql::number_of_digits(this->hours_in_day - 1)) << 'h';
			}
			if (this->mins_mod()) {
				if (found) {
					stream << " : ";
				}
				found = true;
				stream << ql::prepended_to_string(ql::to_string(this->mins_mod()), '0', ql::number_of_digits(this->mins_in_hour - 1)) << 'm';
			}
			if (this->secs_mod()) {
				if (found) {
					stream << " : ";
				}
				found = true;
				stream << ql::prepended_to_string(ql::to_string(this->secs_mod()), '0', ql::number_of_digits(this->secs_in_min - 1)) << 's';
			}
			if (this->msecs_mod()) {
				if (found) {
					stream << " : ";
				}
				found = true;
				stream << ql::prepended_to_string(ql::to_string(this->msecs_mod()), '0', ql::number_of_digits(this->msecs_in_sec - 1)) << "ms";
			}
			if (this->usecs_mod()) {
				if (found) {
					stream << " : ";
				}
				stream << ql::prepended_to_string(ql::to_string(this->usecs_mod()), '0', ql::number_of_digits(this->usecs_in_msec - 1)) << "us";
			}
			if (this->nsecs_mod() || !found) {
				if (found) {
					stream << " : ";
				}
				stream << ql::prepended_to_string(ql::to_string(this->nsecs_mod()), '0', ql::number_of_digits(this->nsecs_in_usec - 1)) << "ns";
			}
			stream << ']';
			return stream.str();
		}
		std::string string() const {
			std::ostringstream stream;
			bool found = false;

			stream << '[';
			if (this->years() || found) {
				found = true;
				stream << this->years() << "y : ";
			}
			if (this->days_mod() || found) {
				found = true;
				stream << ql::prepended_to_string(ql::to_string(this->days_mod()), '0', ql::number_of_digits(this->days_in_year - 1)) << "d : ";
			}
			if (this->hours_mod() || found) {
				found = true;
				stream << ql::prepended_to_string(ql::to_string(this->hours_mod()), '0', ql::number_of_digits(this->hours_in_day - 1)) << "h : ";
			}
			if (this->mins_mod() || found) {
				found = true;
				stream << ql::prepended_to_string(ql::to_string(this->mins_mod()), '0', ql::number_of_digits(this->mins_in_hour - 1)) << "m : ";
			}
			if (this->secs_mod() || found) {
				found = true;
				stream << ql::prepended_to_string(ql::to_string(this->secs_mod()), '0', ql::number_of_digits(this->secs_in_min - 1)) << "s : ";
			}
			if (this->msecs_mod() || found) {
				found = true;
				stream << ql::prepended_to_string(ql::to_string(this->msecs_mod()), '0', ql::number_of_digits(this->msecs_in_sec - 1)) << "ms : ";
			}
			if (this->usecs_mod() || found) {
				found = true;
				stream << ql::prepended_to_string(ql::to_string(this->usecs_mod()), '0', ql::number_of_digits(this->usecs_in_msec - 1)) << "us : ";
			}
			stream << ql::prepended_to_string(ql::to_string(this->nsecs_mod()), '0', ql::number_of_digits(this->nsecs_in_usec - 1)) << "ns]";
			return stream.str();
		}

		std::string string_until_sec() const {
			std::ostringstream stream;
			bool found = false;

			stream << '[';
			if (this->years() || found) {
				found = true;
				stream << this->years() << "y : ";
			}
			if (this->days_mod() || found) {
				found = true;
				stream << ql::prepended_to_string(ql::to_string(this->days_mod()), '0', ql::number_of_digits(this->days_in_year - 1)) << "d : ";
			}
			if (this->hours_mod() || found) {
				found = true;
				stream << ql::prepended_to_string(ql::to_string(this->hours_mod()), '0', ql::number_of_digits(this->hours_in_day - 1)) << "h : ";
			}
			if (this->mins_mod() || found) {
				found = true;
				stream << ql::prepended_to_string(ql::to_string(this->mins_mod()), '0', ql::number_of_digits(this->mins_in_hour - 1)) << "m : ";
			}
			if (this->secs_mod() || found) {
				found = true;
				stream << ql::prepended_to_string(ql::to_string(this->secs_mod()), '0', ql::number_of_digits(this->secs_in_min - 1)) << "s";
			}
			stream << "]";
			return stream.str();
		}

		std::string string_until_ms() const {
			std::ostringstream stream;
			bool found = false;

			stream << '[';
			if (this->years() || found) {
				found = true;
				stream << this->years() << "y : ";
			}
			if (this->days_mod() || found) {
				found = true;
				stream << ql::prepended_to_string(ql::to_string(this->days_mod()), '0', ql::number_of_digits(this->days_in_year - 1)) << "d : ";
			}
			if (this->hours_mod() || found) {
				found = true;
				stream << ql::prepended_to_string(ql::to_string(this->hours_mod()), '0', ql::number_of_digits(this->hours_in_day - 1)) << "h : ";
			}
			if (this->mins_mod() || found) {
				found = true;
				stream << ql::prepended_to_string(ql::to_string(this->mins_mod()), '0', ql::number_of_digits(this->mins_in_hour - 1)) << "m : ";
			}
			if (this->secs_mod() || found) {
				found = true;
				stream << ql::prepended_to_string(ql::to_string(this->secs_mod()), '0', ql::number_of_digits(this->secs_in_min - 1)) << "s : ";
			}
			if (this->msecs_mod() || found) {
				found = true;
				stream << ql::prepended_to_string(ql::to_string(this->msecs_mod()), '0', ql::number_of_digits(this->msecs_in_sec - 1)) << "ms";
			}
			stream << "]";
			return stream.str();
		}
		std::string string_full() const {
			std::ostringstream stream;

			stream << '[';
			stream << this->years() << "y : ";
			stream << ql::prepended_to_string(ql::to_string(this->days_mod()), '0', ql::number_of_digits(this->days_in_year - 1)) << "d : ";
			stream << ql::prepended_to_string(ql::to_string(this->hours_mod()), '0', ql::number_of_digits(this->hours_in_day - 1)) << "h : ";
			stream << ql::prepended_to_string(ql::to_string(this->mins_mod()), '0', ql::number_of_digits(this->mins_in_hour - 1)) << "m : ";
			stream << ql::prepended_to_string(ql::to_string(this->secs_mod()), '0', ql::number_of_digits(this->secs_in_min - 1)) << "s : ";
			stream << ql::prepended_to_string(ql::to_string(this->msecs_mod()), '0', ql::number_of_digits(this->msecs_in_sec - 1)) << "ms : ";
			stream << ql::prepended_to_string(ql::to_string(this->usecs_mod()), '0', ql::number_of_digits(this->usecs_in_msec - 1)) << "us : ";
			stream << ql::prepended_to_string(ql::to_string(this->nsecs_mod()), '0', ql::number_of_digits(this->msecs_in_sec - 1)) << "ns]";
			return stream.str();
		}
		ql::f64 frequency() const {
			auto x = static_cast<ql::f64>(this->m_ns);
			return nsecs_in_sec / x;
		}

		ql::u64 nsecs() const {
			return this->m_ns;
		}
		ql::u64 usecs() const {
			return this->m_ns / nsecs_in_usec;
		}
		ql::u64 msecs() const {
			return this->m_ns / nsecs_in_msec;
		}
		ql::u64 secs() const {
			return this->m_ns / nsecs_in_sec;
		}
		ql::u64 mins() const {
			return this->m_ns / nsecs_in_min;
		}
		ql::u64 hours() const {
			return this->m_ns / nsecs_in_hour;
		}
		ql::u64 days() const {
			return this->m_ns / nsecs_in_day;
		}
		ql::u64 years() const {
			return this->m_ns / nsecs_in_year;
		}

		ql::u64 nsecs_mod() const {
			return this->nsecs() % this->nsecs_in_usec;
		}
		ql::u64 usecs_mod() const {
			return this->usecs() % this->usecs_in_msec;
		}
		ql::u64 msecs_mod() const {
			return this->msecs() % this->msecs_in_sec;
		}
		ql::u64 secs_mod() const {
			return this->secs() % this->secs_in_min;
		}
		ql::u64 mins_mod() const {
			return this->mins() % this->mins_in_hour;
		}
		ql::u64 hours_mod() const {
			return this->hours() % this->hours_in_day;
		}
		ql::u64 days_mod() const {
			return this->days() % this->days_in_year;
		}
		ql::u64 years_mod() const {
			return this->years();
		}

		ql::f64 nsecs_f() const {
			return static_cast<ql::f64>(this->m_ns);
		}
		ql::f64 usecs_f() const {
			return static_cast<ql::f64>(this->m_ns) / nsecs_in_usec;
		}
		ql::f64 msecs_f() const {
			return static_cast<ql::f64>(this->m_ns) / nsecs_in_msec;
		}
		ql::f64 secs_f() const {
			return static_cast<ql::f64>(this->m_ns) / nsecs_in_sec;
		}
		ql::f64 mins_f() const {
			return static_cast<ql::f64>(this->m_ns) / nsecs_in_min;
		}
		ql::f64 hours_f() const {
			return static_cast<ql::f64>(this->m_ns) / nsecs_in_hour;
		}
		ql::f64 days_f() const {
			return static_cast<ql::f64>(this->m_ns) / nsecs_in_day;
		}
		ql::f64 years_f() const {
			return static_cast<ql::f64>(this->m_ns) / nsecs_in_year;
		}

		constexpr static ql::u64 nsecs_in_usec = ql::u64{ 1000ull };
		constexpr static ql::u64 usecs_in_msec = ql::u64{ 1000ull };
		constexpr static ql::u64 msecs_in_sec = ql::u64{ 1000ull };
		constexpr static ql::u64 secs_in_min = ql::u64{ 60ull };
		constexpr static ql::u64 mins_in_hour = ql::u64{ 60ull };
		constexpr static ql::u64 hours_in_day = ql::u64{ 24ull };
		constexpr static ql::u64 days_in_year = ql::u64{ 365ull };

		constexpr static ql::u64 nsecs_in_nsec = ql::u64{ 1ull };
		constexpr static ql::u64 nsecs_in_msec = usecs_in_msec * nsecs_in_usec;
		constexpr static ql::u64 nsecs_in_sec = msecs_in_sec * nsecs_in_msec;
		constexpr static ql::u64 nsecs_in_min = secs_in_min * nsecs_in_sec;
		constexpr static ql::u64 nsecs_in_hour = mins_in_hour * nsecs_in_min;
		constexpr static ql::u64 nsecs_in_day = hours_in_day * nsecs_in_hour;
		constexpr static ql::u64 nsecs_in_year = days_in_year * nsecs_in_day;
	private:
		void set(ql::u64 ns) {
			this->m_ns = ns;
		}

		ql::u64 m_ns;
	};

	std::ostream& operator<<(std::ostream& os, const ql::time& time) {
		return (os << time.string());
	}
	template<typename T> requires is_arithmetic<T>
	constexpr ql::time nsecs(T value) {
		return ql::time{ static_cast<ql::u64>(value) };
	}
	template<typename T> requires is_arithmetic<T>
	constexpr ql::time usecs(T value) {
		return ql::time{ static_cast<ql::u64>(value * ql::time::nsecs_in_usec) };
	}
	template<typename T> requires is_arithmetic<T>
	constexpr ql::time msecs(T value) {
		return ql::time{ static_cast<ql::u64>(value * ql::time::nsecs_in_msec) };
	}
	template<typename T> requires is_arithmetic<T>
	constexpr ql::time secs(T value) {
		return ql::time{ static_cast<ql::u64>(value * ql::time::nsecs_in_sec) };
	}
	template<typename T> requires is_arithmetic<T>
	constexpr ql::time mins(T value) {
		return ql::time{ static_cast<ql::u64>(value * ql::time::nsecs_in_min) };
	}
	template<typename T> requires is_arithmetic<T>
	constexpr ql::time hours(T value) {
		return ql::time{ static_cast<ql::u64>(value * ql::time::nsecs_in_hour) };
	}
	template<typename T> requires is_arithmetic<T>
	constexpr ql::time days(T value) {
		return ql::time{ static_cast<ql::u64>(value * ql::time::nsecs_in_day) };
	}
	template<typename T> requires is_arithmetic<T>
	constexpr ql::time years(T value) {
		return ql::time{ static_cast<ql::u64>(value * ql::time::nsecs_in_year) };
	}

	//clock
	// - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - 

	class clock {
	public:
		clock(bool running = true) {
			this->init(running);
		}
		void reset() {
			this->m_begin = ql::time::clock_time();
			this->m_running = true;
			this->m_pause_sum = ql::u64{};
		}
		void pause() {
			if (this->is_running()) {
				this->measure();
			}
			this->m_running = false;
			this->m_pause = ql::time::clock_time();
		}
		void reset_pause() {
			if (this->is_running()) {
				this->measure();
			}
			this->m_begin = ql::time::clock_time();
			this->m_running = false;
			this->m_pause = ql::time::clock_time();
			this->m_pause_sum = ql::u64{};
		}
		ql::time elapsed() const {
			if (this->is_running()) {
				this->measure();
			}
			return this->m_end - (this->m_begin + this->m_pause_sum);
		}
		std::string elapsed_str() const {
			return this->elapsed().string();
		}
		ql::f64 elapsed_f() const {
			return this->elapsed().secs_f();
		}

		ql::time elapsed_reset() {
			auto result = this->elapsed();
			this->reset();
			return result;
		}
		std::string elapsed_str_reset() {
			auto result = this->elapsed_str();
			this->reset();
			return result;
		}
		ql::f64 elapsed_f_reset() {
			auto result = this->elapsed_f();
			this->reset();
			return result;
		}

		bool has_elapsed(ql::f64 seconds) const {
			return this->elapsed_f() > seconds;
		}
		bool has_elapsed(ql::time duration) const {
			return this->elapsed() > duration;
		}
		bool has_elapsed_reset(ql::f64 seconds) {
			auto result = this->has_elapsed(seconds);
			if (result) {
				this->reset();
			}
			return result;
		}
		bool has_elapsed_reset(ql::time duration) {
			auto result = this->has_elapsed(duration);
			if (result) {
				this->reset();
			}
			return result;
		}
		void resume() {
			if (this->is_running()) {
				return;
			}
			this->m_running = true;
			this->m_pause_sum += (ql::time::clock_time() - this->m_pause);
		}
		bool is_running() const {
			return this->m_running;
		}
		bool is_paused() const {
			return !this->m_running;
		}

	private:
		void measure() const {
			if (this->is_running()) {
				this->m_end = ql::time::clock_time();
			}
		}
		void init(bool running) {
			this->m_begin = ql::time::clock_time();
			this->m_end = this->m_begin;
			this->m_pause_sum = ql::u64{};
			this->m_running = running;
			if (this->is_paused()) {
				this->m_pause = this->m_begin;
			}
		}

		ql::time m_pause;
		ql::time m_pause_sum;
		ql::time m_begin;
		mutable ql::time m_end;
		bool m_running;
	};
	class halted_clock : public ql::clock {
	public:
		halted_clock() : ql::clock(false) {

		}
	};
	std::ostream& operator<<(std::ostream& os, const ql::clock& clock) {
		return (os << clock.elapsed().string());
	}
	std::ostream& operator<<(std::ostream& os, const ql::halted_clock& clock) {
		return (os << clock.elapsed().string());
	}

	//random
	// - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - 

	template<typename T>
	class distribution;

	template<ql::size bits>
	class random_engine {
	public:
		using type = typename
			std::conditional_t<bits == 32, identity<std::mt19937>, 
			std::conditional_t<bits == 64, identity<std::mt19937_64>,
			ql::error_type>>::type;

		void seed(ql::u64 value) {
			this->engine.seed(value);
		}
		void seed(const std::seed_seq& seq) {
			this->engine.seed(seq);
		}
		void seed_time() {
			this->engine.seed(static_cast<ql::u32>(ql::time::clock_time()));
		}
		auto get_current() const {
			return this->engine.get_current();
		}
		void discard(ql::u64 count) {
			this->engine.discard(count);
		}
		void seed_random() {
			std::array<ql::u32, random_engine::type::state_size* random_engine::type::word_size / ql::bits_in_type<ql::u32>()> random_data;
			std::random_device source;
			std::generate(std::begin(random_data), std::end(random_data), std::ref(source));
			std::seed_seq seeds(std::begin(random_data), std::end(random_data));
			this->engine.seed(seeds);
		}
		template<typename T>
		T generate(const ql::distribution<T>& dist) {
			return dist.m_dist(this->engine);
		}
		template<typename T>
		T generate(T min, T max) {
			ql::distribution<T> dist(min, max);
			return dist.m_dist(this->engine);
		}
		template<typename T>
		T generate(T max) {
			ql::distribution<T> dist(max);
			return dist.m_dist(this->engine);
		}
		auto generate() {
			return this->engine.generate();
		}

		type engine;
	};


	template<typename T>
	class distribution {
	public:
		distribution(T min, T max) : m_dist(min, max) {

		}
		distribution(T max = T{ 1 }) : m_dist(T{}, max) {

		}

		using type = typename 
			std::conditional_t<ql::is_integer<T>, identity<std::uniform_int_distribution<T>>,
			std::conditional_t<ql::is_floating_point<T>, identity<std::uniform_real_distribution<T>>,
			ql::error_type>>::type;


		void set_range(T min, T max) {
			this->m_dist = type(min, max);
		}
		void set_range(T max) {
			this->m_dist = type(T{}, max);
		}
		void set_min(T min) {
			this->m_dist = type(min, this->max());
		}
		void set_max(T max) {
			this->m_dist = type(this->min(), max);
		}
		T min() const {
			return this->m_dist.min();
		}
		T max() const {
			return this->m_dist.max();
		}

		template<ql::size bits>
		friend class random_engine;

		template<ql::size bits>
		T generate(ql::random_engine<bits>& engine) const {
			return engine.generate(*this);
		}
	private:
		type m_dist;
	};

	namespace detail {
		struct rng_t {
			rng_t() {
				this->rng.seed_random();
			}
			ql::random_engine<64> rng;
			ql::distribution<ql::i64> idist;
			ql::distribution<ql::u64> udist;
			ql::distribution<ql::f64> fdist;
		};

		ql::detail::rng_t rng;
	}

	template<typename T> requires is_arithmetic<T>
	T random(T min, T max) {
		ql::distribution<T> dist(min, max);
		return ql::detail::rng.rng.generate(dist);
	}
	template<typename T>  requires is_arithmetic<T>
	T random(T max) {
		ql::distribution<T> dist(T{}, max);
		return ql::detail::rng.rng.generate(dist);
	}
	template<typename T>  requires is_arithmetic<T>
	T random() { 
		ql::distribution<T> dist(ql::type_min<T>(), ql::type_max<T>());
		return ql::detail::rng.rng.generate(dist);
	}


	template<typename F> requires is_floating_point<F>
	bool random_event(F chance) {
		return ql::random(F{ 0 }, F{ 1 }) < chance;
	}

	template<typename T> requires is_integer<T>
	bool random_event(T n, T out_of) {
		return ql::random(T{ 1 }, out_of) <= n;
	}

	bool random_event() {
		return ql::random(0u, 1u) == 0u;
	}

	//filesys 
	// - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - 

	std::string read_file(const std::string& path) {
		std::ifstream file(path, std::ios::ate | std::ios::binary);

		if (!file.is_open()) {
			throw std::runtime_error(ql::to_cstring("failed to open file \"", path, "\""));
		}

		auto file_size = (size_t)file.tellg();
		std::string buffer;
		buffer.resize(file_size);

		file.seekg(0);
		file.read(buffer.data(), file_size);
		file.close();

		return buffer;
	}
	void write_file(const std::string& data, const std::string& path) {
		std::ofstream file(path, std::ios::binary);
		file.write(data.data(), data.size());
		file.close();
	}
	bool file_exists(const std::string& path) {
		return std::filesystem::exists(path);
	}

	//memory 
	// - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - 

	template<typename C> requires ql::is_vector_like<C>
	constexpr void string_to_vector_memory(const std::string& source, C& dest) {
		if (source.empty()) {
			return;
		}
		dest.resize((source.size() - 1) / ql::bytes_in_type<ql::container_subtype<C>>() + 1);
		memcpy(dest.data(), source.data(), source.size());
	}
	template<typename C> requires ql::is_contiguous_container<C>
	constexpr void container_memory_to_string(const C& data, std::string& dest) {
		dest.resize(data.size() * sizeof(ql::container_subtype<C>));
		memcpy(dest.data(), data.data(), dest.size());
	}

	//improved sf::Vector
	// - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - 

	namespace detail {
		template<typename T>
		struct vector_impl_1 {
			union {
				struct {
					T x;
				};
				std::array<T, 1> data;
			};
		};
		template<typename T>
		struct vector_impl_2 {
			union {
				struct {
					T x;
					T y;
				};
				std::array<T, 2> data;
			};
		};
		template<typename T>
		struct vector_impl_3 {
			union {
				struct {
					T x;
					T y;
					T z;
				};
				std::array<T, 3> data;
			};
		};
		template<typename T>
		struct vector_impl_4 {
			union {
				struct {
					T x;
					T y;
					T z;
					T w;
				};
				std::array<T, 4> data;
			};
		};
	}


	template<ql::size N, typename T>
	using vectorN_type = typename std::conditional_t<
		N == 1, identity<detail::vector_impl_1<T>>, std::conditional_t<
		N == 2, identity<detail::vector_impl_2<T>>, std::conditional_t<
		N == 3, identity<detail::vector_impl_3<T>>, std::conditional_t<
		N == 4, identity<detail::vector_impl_4<T>>,
		ql::error_type>>>>::type;

	template<ql::size N, typename T>
	struct vectorN : vectorN_type<N, T> {

		using impl_type = vectorN_type<N, T>;

		constexpr std::string string() const {
			std::ostringstream stream;
			stream << '(';
			bool first = true;
			for (auto& i : this->data) {
				if (!first) {
					stream << ", ";
				}
				first = false;
				stream << i;
			}
			stream << ')';
			return stream.str();
		}

		constexpr vectorN() {
			this->clear();
		}
		constexpr vectorN(const vectorN& other) {
			*this = other;
		}
		template<ql::size N2, typename U>
		constexpr vectorN(const vectorN<N2, U>& other) {
			*this = other;
		}
		template<typename... Args>
		constexpr vectorN(Args... list) {
			*this = ql::to_array_t<T>(list...);
		}
		template<typename U>
		constexpr vectorN(const std::initializer_list<U>& list) {
			this->set(list);
		}

#ifndef QL_NO_SFML
		template<typename U>
		constexpr vectorN(const sf::Vector2<U>& other) {
			*this = other;
		}
		template<typename U>
		constexpr vectorN& operator=(const sf::Vector2<U>& other) {
			if constexpr (N == 1) {
				this->x = other.x;
			}
			else {
				this->x = other.x;
				this->y = other.y;
				this->clear(2);
			}
			return *this;
		}
		template<typename U>
		constexpr operator sf::Vector2<U>() const {
			if constexpr (N == 1) {
				return sf::Vector2<U>(static_cast<U>(this->x), U{ 0 });
			}
			else {
				return sf::Vector2<U>(static_cast<U>(this->x), static_cast<U>(this->y));
			}
		}
#endif
		constexpr vectorN& operator=(const vectorN& other) {
			this->data = other.data;
			return *this;
		}
		template<typename U>
		constexpr vectorN& operator=(const vectorN<N, U>& other) {
			for (ql::u32 i = 0u; i < N; ++i) {
				this->data[i] = static_cast<T>(other.data[i]);
			}
			return *this;
		}
		template<ql::size N2, typename U>
		constexpr vectorN& operator=(const vectorN<N2, U>& other) {
			auto stop = ql::min(N, N2);
			for (ql::u32 i = 0u; i < stop; ++i) {
				this->data[i] = static_cast<T>(other.data[i]);
			}
			this->clear(stop);
			return *this;
		}
		template<ql::size N, typename U>
		constexpr vectorN& operator=(const std::array<U, N>& array) {
			this->set(array);
			return *this;
		}

		template<ql::size N, typename U>
		constexpr void set(const std::array<U, N>& array) {
			if (array.empty()) {
				this->clear();
				return;
			}
			auto size = ql::min(array.size(), this->data.size());
			for (ql::u32 i = 0u; i < size; ++i) {
				this->data[i] = static_cast<T>(array[i]);
			}
			this->clear(size);
		}
		template<typename U>
		constexpr void set(const std::initializer_list<U>& list) {
			if (list.size() == 0) {
				this->clear();
				return;
			}
			auto size = ql::min(list.size(), this->data.size());
			for (ql::u32 i = 0u; i < size; ++i) {
				this->data[i] = static_cast<T>(*(list.begin() + i));
			}
			this->clear(size);
		}

		T& operator[](ql::u32 index) {
			return this->data[index];
		}
		const T& operator[](ql::u32 index) const {
			return this->data[index];
		}

		T& front() {
			return this->data.front();
		}
		const T& front() const {
			return this->data.front();
		}

		T& back() {
			return this->data.back();
		}
		const T& back() const {
			return this->data.back();
		}

		constexpr void clear(ql::size offset = 0u) {
			for (ql::size i = offset; i < this->data.size(); ++i) {
				this->data[i] = T{ 0 };
			}
		}


		constexpr bool operator<(const vectorN& other) const {
			for (ql::u32 i = 0u; i < this->data.size(); ++i) {
				if (this->data[i] < static_cast<T>(other.data[i])) {
					return true;
				}
				if (this->data[i] > static_cast<T>(other.data[i])) {
					return false;
				}
			}
			return false;
		}
		template<typename U>
		constexpr bool operator==(const vectorN<N, U>& other) const {
			for (ql::u32 i = 0u; i < this->data.size(); ++i) {
				if (this->data[i] != static_cast<T>(other.data[i])) {
					return false;
				}
			}
			return true;
		}
		template<typename U>
		constexpr bool operator!=(const vectorN<N, U>& other) const {
			return !(*this == other);
		}

		template<typename U>
		constexpr vectorN<N, T>& operator+=(const std::initializer_list<U>& list) {
			vectorN<N, T> add;
			add.set(list);
			return (*this += add);
		}
		template<typename U>
		constexpr vectorN<N, T>& operator+=(const vectorN<N, U>& other) {
			for (ql::u32 i = 0u; i < this->data.size(); ++i) {
				this->data[i] += static_cast<T>(other.data[i]);
			}
			return *this;
		}
		template<typename U>
		constexpr auto operator+(const vectorN<N, U>& other) const {
			vectorN<N, ql::arithmetic_result_type<T, U>> copy = *this;
			copy += other;
			return copy;
		}
		template<typename U>
		constexpr vectorN<N, T>& operator+=(U u) {
			for (ql::u32 i = 0u; i < this->data.size(); ++i) {
				this->data[i] += static_cast<T>(u);
			}
			return *this;
		}
		template<typename U>
		constexpr auto operator+(U u) const {
			vectorN<N, ql::arithmetic_result_type<T, U>> copy = *this;
			copy += u;
			return copy;
		}

		template<typename U>
		constexpr vectorN<N, T>& operator-=(const std::initializer_list<U>& list) {
			vectorN<N, T> add;
			add.set(list);
			return (*this -= add);
		}
		template<typename U>
		constexpr vectorN<N, T>& operator-=(const vectorN<N, U>& other) {
			for (ql::u32 i = 0u; i < this->data.size(); ++i) {
				this->data[i] -= static_cast<T>(other.data[i]);
			}
			return *this;
		}
		template<typename U>
		constexpr auto operator-(const vectorN<N, U>& other) const {
			vectorN<N, ql::arithmetic_result_type<T, U>> copy = *this;
			copy -= other;
			return copy;
		}
		template<typename U>
		constexpr vectorN<N, T>& operator-=(U u) {
			for (ql::u32 i = 0u; i < this->data.size(); ++i) {
				this->data[i] -= static_cast<T>(u);
			}
			return *this;
		}
		template<typename U>
		constexpr auto operator-(U u) const {
			vectorN<N, ql::arithmetic_result_type<T, U>> copy = *this;
			copy -= u;
			return copy;
		}

		template<typename U>
		constexpr vectorN<N, T>& operator*=(const std::initializer_list<U>& list) {
			vectorN<N, T> add;
			add.set(list);
			return (*this *= add);
		}
		template<typename U>
		constexpr vectorN<N, T>& operator*=(const vectorN<N, U>& other) {
			for (ql::u32 i = 0u; i < this->data.size(); ++i) {
				this->data[i] *= static_cast<T>(other.data[i]);
			}
			return *this;
		}
		template<typename U>
		constexpr auto operator*(const vectorN<N, U>& other) const {
			vectorN<N, ql::arithmetic_result_type<T, U>> copy = *this;
			copy *= other;
			return copy;
		}
		template<typename U>
		constexpr vectorN<N, T>& operator*=(U u) {
			for (ql::u32 i = 0u; i < this->data.size(); ++i) {
				this->data[i] *= static_cast<T>(u);
			}
			return *this;
		}
		template<typename U>
		constexpr auto operator*(U u) const {
			vectorN<N, ql::arithmetic_result_type<T, U>> copy = *this;
			copy *= u;
			return copy;
		}

		template<typename U>
		constexpr vectorN<N, T>& operator/=(const std::initializer_list<U>& list) {
			vectorN<N, T> add;
			add.set(list);
			return (*this /= add);
		}
		template<typename U>
		constexpr vectorN<N, T>& operator/=(const vectorN<N, U>& other) {
			for (ql::u32 i = 0u; i < this->data.size(); ++i) {
				this->data[i] /= static_cast<T>(other.data[i]);
			}
			return *this;
		}
		template<typename U>
		constexpr auto operator/(const vectorN<N, U>& other) const {
			vectorN<N, ql::arithmetic_result_type<T, U>> copy = *this;
			copy /= other;
			return copy;
		}
		template<typename U>
		constexpr vectorN<N, T>& operator/=(U u) {
			for (ql::u32 i = 0u; i < this->data.size(); ++i) {
				this->data[i] /= static_cast<T>(u);
			}
			return *this;
		}
		template<typename U>
		constexpr auto operator/(U u) const {
			vectorN<N, ql::arithmetic_result_type<T, U>> copy = *this;
			copy /= u;
			return copy;
		}

		template<typename U>
		constexpr vectorN<N, T>& operator%=(const std::initializer_list<U>& list) {
			vectorN<N, T> add;
			add.set(list);
			return (*this %= add);
		}
		template<typename U>
		constexpr vectorN<N, T>& operator%=(const vectorN<N, U>& other) {
			for (ql::u32 i = 0u; i < this->data.size(); ++i) {
				this->data[i] %= static_cast<T>(other.data[i]);
			}
			return *this;
		}
		template<typename U>
		constexpr auto operator%(const vectorN<N, U>& other) const {
			vectorN<N, ql::arithmetic_result_type<T, U>> copy = *this;
			copy %= other;
			return copy;
		}
		template<typename U>
		constexpr vectorN<N, T>& operator%=(U u) {
			for (ql::u32 i = 0u; i < this->data.size(); ++i) {
				this->data[i] %= static_cast<T>(u);
			}
			return *this;
		}
		template<typename U>
		constexpr auto operator%(U u) const {
			vectorN<N, ql::arithmetic_result_type<T, U>> copy = *this;
			copy %= u;
			return copy;
		}

		constexpr vectorN operator-() const {
			vectorN copy;
			for (ql::u32 i = 0u; i < this->data.size(); ++i) {
				copy[i] = -this->data[i];
			}
			return copy;
		}
		constexpr vectorN operator+() const {
			vectorN copy;
			for (ql::u32 i = 0u; i < this->data.size(); ++i) {
				copy[i] = +this->impl_type::data[i];
			}
			return copy;
		}

		template<ql::size N, typename T>
		friend std::ostream& operator<<(std::ostream& os, const ql::vectorN<N, T>& vec);
	};

	template<ql::size N, typename T>
	std::ostream& operator<<(std::ostream& os, const ql::vectorN<N, T>& vec) {
		return os << vec.string();
	}

	template<typename T>
	using vector2 = ql::vectorN<2, T>;
	using vector2f = ql::vectorN<2, ql::f32>;
	using vector2d = ql::vectorN<2, ql::f64>;
	using vector2i = ql::vectorN<2, ql::i32>;
	using vector2u = ql::vectorN<2, ql::u32>;

	template<typename T>
	using vector3 = ql::vectorN<3, T>;
	using vector3f = ql::vectorN<3, ql::f32>;
	using vector3d = ql::vectorN<3, ql::f64>;
	using vector3i = ql::vectorN<3, ql::i32>;
	using vector3u = ql::vectorN<3, ql::u32>;

	template<typename T>
	using vector4 = ql::vectorN<4, T>;
	using vector4f = ql::vectorN<4, ql::f32>;
	using vector4d = ql::vectorN<4, ql::f64>;
	using vector4i = ql::vectorN<4, ql::i32>;
	using vector4u = ql::vectorN<4, ql::u32>;

	//rgb
	// - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - 

	union rgb;

	struct frgb {
		ql::f32 r = 0.0f;
		ql::f32 g = 0.0f;
		ql::f32 b = 0.0f;

		constexpr frgb() {

		}
		constexpr void clamp() {
			this->r = ql::clamp_0_1(this->r);
			this->g = ql::clamp_0_1(this->g);
			this->b = ql::clamp_0_1(this->b);
		}

		operator rgb() const;
		rgb get_rgb() const;
		ql::frgb& operator=(const ql::rgb& rgb);

#ifndef QL_NO_SFML
		ql::frgb& operator=(const sf::Color& color) {
			this->r = color.r;
			this->g = color.g;
			this->b = color.b;
			return *this;
		}
		operator sf::Color() const {
			sf::Color color;
			color.r = static_cast<ql::u8>(ql::clamp_0_1(this->r) * 255.0f);
			color.g = static_cast<ql::u8>(ql::clamp_0_1(this->g) * 255.0f);
			color.b = static_cast<ql::u8>(ql::clamp_0_1(this->b) * 255.0f);
			color.a = ql::u8{ 255 };
			return color;
		}
#endif


		std::string string() const {
			std::ostringstream stream;
			stream << '(';
			stream << ql::to_string_precision(3, this->r) << ", ";
			stream << ql::to_string_precision(3, this->g) << ", ";
			stream << ql::to_string_precision(3, this->b) << ')';
			return stream.str();
		}

		constexpr ql::frgb& operator*=(ql::frgb other) {
			this->r *= other.r;
			this->g *= other.g;
			this->b *= other.b;
			return *this;
		}
		constexpr ql::frgb operator*(ql::frgb other) const {
			auto copy = *this;
			copy *= other;
			return copy;
		}
		constexpr ql::frgb& operator-=(ql::frgb other) {
			this->r -= other.r;
			this->g -= other.g;
			this->b -= other.b;
			return *this;
		}
		constexpr ql::frgb operator-(ql::frgb other) const {
			auto copy = *this;
			copy -= other;
			return copy;
		}
		constexpr ql::frgb& operator/=(ql::frgb other) {
			this->r /= other.r;
			this->g /= other.g;
			this->b /= other.b;
			return *this;
		}
		constexpr ql::frgb operator/(ql::frgb other) const {
			auto copy = *this;
			copy /= other;
			return copy;
		}
		constexpr ql::frgb& operator+=(ql::frgb other) {
			this->r += other.r;
			this->g += other.g;
			this->b += other.b;
			return *this;
		}
		constexpr ql::frgb operator+(ql::frgb other) const {
			auto copy = *this;
			copy += other;
			return copy;
		}

		template<typename T> requires ql::is_arithmetic<T>
		constexpr ql::frgb& operator*=(T other) {
			this->r = static_cast<ql::f32>(this->r * other);
			this->g = static_cast<ql::f32>(this->g * other);
			this->b = static_cast<ql::f32>(this->b * other);
			return *this;
		}
		template<typename T> requires ql::is_arithmetic<T>
		constexpr ql::frgb operator*(T other) const {
			auto copy = *this;
			copy *= other;
			return copy;
		}
		template<typename T> requires ql::is_arithmetic<T>
		constexpr ql::frgb& operator/=(T other) {
			this->r = static_cast<ql::f32>(this->r / other);
			this->g = static_cast<ql::f32>(this->g / other);
			this->b = static_cast<ql::f32>(this->b / other);
			return *this;
		}
		template<typename T> requires ql::is_arithmetic<T>
		constexpr ql::frgb operator/(T other) const {
			auto copy = *this;
			copy /= other;
			return copy;
		}
		template<typename T> requires ql::is_arithmetic<T>
		constexpr ql::frgb& operator+=(T other) {
			this->r = static_cast<ql::f32>(this->r + other);
			this->g = static_cast<ql::f32>(this->g + other);
			this->b = static_cast<ql::f32>(this->b + other);
			return *this;
		}
		template<typename T> requires ql::is_arithmetic<T>
		constexpr ql::frgb operator+(T other) const {
			auto copy = *this;
			copy += other;
			return copy;
		}
		template<typename T> requires ql::is_arithmetic<T>
		constexpr ql::frgb& operator-=(T other) {
			this->r = static_cast<ql::f32>(this->r - other);
			this->g = static_cast<ql::f32>(this->g - other);
			this->b = static_cast<ql::f32>(this->b - other);
			return *this;
		}
		template<typename T> requires ql::is_arithmetic<T>
		constexpr ql::frgb operator-(T other) const {
			auto copy = *this;
			copy -= other;
			return copy;
		}
	};

	union rgb {
		rgb() {
			*this = rgb::white;
		}
#ifndef QL_NO_SFML
		rgb(sf::Color color) {
			*this = color;
		}
#endif
		rgb(ql::u8 r, ql::u8 g, ql::u8 b, ql::u8 a = ql::u8_max) {
			this->c.r = r;
			this->c.g = g;
			this->c.b = b;
			this->c.a = a;
		}
		rgb(ql::u32 hex) {
			*this = hex;
		}
		rgb(const rgb& other) {
			*this = other;
		}

		struct {
			ql::u8 r;
			ql::u8 g;
			ql::u8 b;
			ql::u8 a;
		} c;
		ql::u32 uint;

		ql::rgb& operator=(const frgb& other) {
			*this = other.get_rgb();
			return *this;
		}
		ql::rgb& operator=(const rgb& other) {
			this->uint = other.uint;
			return *this;
		}
#ifndef QL_NO_SFML
		ql::rgb& operator=(const sf::Color& color) {
			this->c.r = color.r;
			this->c.g = color.g;
			this->c.b = color.b;
			this->c.a = color.a;
			return *this;
		}
		operator sf::Color() const {
			sf::Color color;
			color.r = this->c.r;
			color.g = this->c.g;
			color.b = this->c.b;
			color.a = this->c.a;
			return color;
		}
#endif
		operator frgb() const {
			frgb result;
			result.r = this->c.r / 255.0f;
			result.g = this->c.g / 255.0f;
			result.b = this->c.b / 255.0f;
			return result;
		}

		ql::rgb& operator=(ql::u32 uint) {

			if (uint <= (ql::u32_max >> 8)) {
				uint <<= 8;
				uint |= 0xFFu;
			}

			this->uint = ((uint & 0xFF000000u) >> 24) | ((uint & 0x000000FFu) << 24) | ((uint & 0x00FF0000u) >> 8) | ((uint & 0x0000FF00u) << 8);

			return *this;
		}
		bool operator==(const rgb& other) const {
			return this->uint == other.uint;
		}

		std::string string() const {
			std::ostringstream stream;
			stream << '(';
			stream << static_cast<ql::i16>(this->c.r) << ", ";
			stream << static_cast<ql::i16>(this->c.g) << ", ";
			stream << static_cast<ql::i16>(this->c.b);
			if (this->c.a != ql::u8_max) {
				stream << ", " << static_cast<ql::i16>(this->c.a);
			}
			stream << ')';
			return stream.str();
		}
		bool is_unset() const {
			return (*this == unset);
		}


		void interpolate(ql::rgb color, ql::f64 strength) {
			strength = ql::clamp(ql::f64{ 0 }, strength, ql::f64{ 1 });
			this->c.r = static_cast<ql::u8>((this->c.r * (1 - strength) + color.c.r * strength));
			this->c.g = static_cast<ql::u8>((this->c.g * (1 - strength) + color.c.g * strength));
			this->c.b = static_cast<ql::u8>((this->c.b * (1 - strength) + color.c.b * strength));
			this->c.a = static_cast<ql::u8>((this->c.a * (1 - strength) + color.c.a * strength));
		}
		ql::rgb interpolated(ql::rgb color, ql::f64 strength) const {
			auto copy = *this;
			copy.interpolate(color, strength);
			return copy;
		}
		static ql::rgb interpolation(const std::vector<ql::rgb>& colors, ql::f64 strength) {
			strength = ql::clamp(0.0, strength, 1.0);
			if (strength == 1.0) {
				return colors.back();
			}

			auto index = static_cast<ql::u32>(strength * (colors.size() - 1));
			auto left_over = ((colors.size() - 1) * strength) - index;

			return colors[index].interpolated(colors[index + 1], left_over);
		}

		void invert() {
			this->c.r = 255 - this->c.r;
			this->c.g = 255 - this->c.g;
			this->c.b = 255 - this->c.b;
		}
		ql::rgb inverted() const {
			auto copy = *this;
			copy.invert();
			return copy;
		}
		ql::rgb with_alpha(ql::u8 alpha) const {
			auto copy = *this;
			copy.c.a = alpha;
			return copy;
		}

		ql::rgb& operator*=(ql::rgb other) {
			this->c.r *= other.c.r;
			this->c.g *= other.c.g;
			this->c.b *= other.c.b;
			return *this;
		}
		ql::rgb operator*(ql::rgb other) const {
			auto copy = *this;
			copy *= other;
			return copy;
		}
		ql::rgb& operator-=(ql::rgb other) {
			this->c.r -= other.c.r;
			this->c.g -= other.c.g;
			this->c.b -= other.c.b;
			return *this;
		}
		ql::rgb operator-(ql::rgb other) const {
			auto copy = *this;
			copy -= other;
			return copy;
		}
		ql::rgb& operator/=(ql::rgb other) {
			this->c.r /= other.c.r;
			this->c.g /= other.c.g;
			this->c.b /= other.c.b;
			return *this;
		}
		ql::rgb operator/(ql::rgb other) const {
			auto copy = *this;
			copy /= other;
			return copy;
		}
		ql::rgb& operator+=(ql::rgb other) {
			this->c.r += other.c.r;
			this->c.g += other.c.g;
			this->c.b += other.c.b;
			return *this;
		}
		ql::rgb operator+(ql::rgb other) const {
			auto copy = *this;
			copy += other;
			return copy;
		}


		template<typename T> requires is_arithmetic<T>
		ql::rgb& operator/=(T value) {
			this->c.r = static_cast<ql::u8>(ql::clamp<ql::i64>(ql::i64{ 0 }, static_cast<ql::i64>(this->c.r / value), static_cast<ql::i64>(ql::u8_max)));
			this->c.g = static_cast<ql::u8>(ql::clamp<ql::i64>(ql::i64{ 0 }, static_cast<ql::i64>(this->c.g / value), static_cast<ql::i64>(ql::u8_max)));
			this->c.b = static_cast<ql::u8>(ql::clamp<ql::i64>(ql::i64{ 0 }, static_cast<ql::i64>(this->c.b / value), static_cast<ql::i64>(ql::u8_max)));
			return *this;
		}
		template<typename T> requires is_arithmetic<T>
		ql::rgb operator/(T value) const {
			auto copy = *this;
			copy /= value;
			return copy;
		}
		template<typename T> requires is_arithmetic<T>
		ql::rgb& operator*=(T value) {
			this->c.r = static_cast<ql::u8>(ql::clamp<ql::i64>(ql::i64{ 0 }, static_cast<ql::i64>(this->c.r * value), static_cast<ql::i64>(ql::u8_max)));
			this->c.g = static_cast<ql::u8>(ql::clamp<ql::i64>(ql::i64{ 0 }, static_cast<ql::i64>(this->c.g * value), static_cast<ql::i64>(ql::u8_max)));
			this->c.b = static_cast<ql::u8>(ql::clamp<ql::i64>(ql::i64{ 0 }, static_cast<ql::i64>(this->c.b * value), static_cast<ql::i64>(ql::u8_max)));
			return *this;
		}
		template<typename T> requires is_arithmetic<T>
		ql::rgb operator*(T value) const {
			auto copy = *this;
			copy *= value;
			return copy;
		}
		template<typename T> requires is_arithmetic<T>
		ql::rgb& operator+=(T value) {
			this->c.r = static_cast<ql::u8>(ql::clamp(ql::i16{ 0 }, static_cast<ql::i16>(this->c.r) + value, static_cast<ql::i16>(ql::u8_max)));
			this->c.g = static_cast<ql::u8>(ql::clamp(ql::i16{ 0 }, static_cast<ql::i16>(this->c.g) + value, static_cast<ql::i16>(ql::u8_max)));
			this->c.b = static_cast<ql::u8>(ql::clamp(ql::i16{ 0 }, static_cast<ql::i16>(this->c.b) + value, static_cast<ql::i16>(ql::u8_max)));
			return *this;
		}
		template<typename T> requires is_arithmetic<T>
		ql::rgb operator+(T value) const {
			auto copy = *this;
			copy += value;
			return copy;
		}
		template<typename T> requires is_arithmetic<T>
		ql::rgb& operator-=(T value) {
			this->c.r = static_cast<ql::u8>(ql::clamp(ql::i16{ 0 }, static_cast<ql::i16>(this->c.r) - value, static_cast<ql::i16>(ql::u8_max)));
			this->c.g = static_cast<ql::u8>(ql::clamp(ql::i16{ 0 }, static_cast<ql::i16>(this->c.g) - value, static_cast<ql::i16>(ql::u8_max)));
			this->c.b = static_cast<ql::u8>(ql::clamp(ql::i16{ 0 }, static_cast<ql::i16>(this->c.b) - value, static_cast<ql::i16>(ql::u8_max)));
			return *this;
		}
		template<typename T> requires is_arithmetic<T>
		ql::rgb operator-(T value) const {
			auto copy = *this;
			copy -= value;
			return copy;
		}

		static const ql::rgb red;
		static const ql::rgb green;
		static const ql::rgb blue;
		static const ql::rgb orange;
		static const ql::rgb yellow;
		static const ql::rgb cyan;
		static const ql::rgb magenta;
		static const ql::rgb white;
		static const ql::rgb grey;
		static const ql::rgb black;
		static const ql::rgb transparent;
		static const ql::rgb unset;
	};


	ql::frgb::operator rgb() const {
		rgb result;
		result.c.r = static_cast<ql::u8>(ql::clamp_0_1(this->r) * 255.0f);
		result.c.g = static_cast<ql::u8>(ql::clamp_0_1(this->g) * 255.0f);
		result.c.b = static_cast<ql::u8>(ql::clamp_0_1(this->b) * 255.0f);
		result.c.a = ql::u8{ 255 };
		return result;
	}
	rgb ql::frgb::get_rgb() const {
		return this->operator ql::rgb();
	}
	ql::frgb& ql::frgb::operator=(const ql::rgb& rgb) {
		*this = rgb.operator ql::frgb();
		return *this;
	}
	const ql::rgb ql::rgb::red = 0xFF'00'00;
	const ql::rgb ql::rgb::green = 0x00'FF'00;
	const ql::rgb ql::rgb::blue = 0x00'00'FF;
	const ql::rgb ql::rgb::yellow = 0xFF'FF'00;
	const ql::rgb ql::rgb::orange = 0xFF'88'00;
	const ql::rgb ql::rgb::cyan = 0x00'FF'FF;
	const ql::rgb ql::rgb::magenta = 0xFF'00'FF;
	const ql::rgb ql::rgb::white = 0xFF'FF'FF;
	const ql::rgb ql::rgb::grey = 0x88'88'88;
	const ql::rgb ql::rgb::black = 0x00'00'00;
	const ql::rgb ql::rgb::transparent = 0xFF'FF'FF'00;
	const ql::rgb ql::rgb::unset = 0x00'00'00'00;

	ql::rgb get_random_color() {
		auto value = (ql::random((1u << 24) - 1) << 8) | 0xFF;
		return ql::rgb(value);
	}
	ql::rgb get_random_transparency_color() {
		return ql::rgb(ql::random(ql::u32_max));
	}
	ql::rgb get_rainbow_color(ql::f64 f) {
		const static std::vector<ql::rgb> rainbow = { ql::rgb::red, ql::rgb::yellow, ql::rgb::green, ql::rgb::cyan, ql::rgb::blue, ql::rgb::magenta, ql::rgb::red };
		return ql::rgb::interpolation(rainbow, f);
	}

	//resources
	// - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - 

#ifndef QL_NO_SFML
	struct resources {
		void add_font(const std::string& name, const std::string& path) {
			if (!ql::file_exists(path)) {
				throw std::exception(ql::to_cstring("resources::add_font(", name, "): \"", path, "\" doesn't exist"));
			}
			this->fonts[name].loadFromFile(path);
		}
		void add_texture(const std::string& name, const std::string& path) {
			if (!ql::file_exists(path)) {
				throw std::exception(ql::to_cstring("resources::add_texture(", name, "): \"", path, "\" doesn't exist"));
			}
			this->textures[name].loadFromFile(path);
		}
		void add_sprite(const std::string& name, const std::string& path) {
			if (!ql::file_exists(path)) {
				throw std::exception(ql::to_cstring("resources::add_sprite(", name, "): \"", path, "\" doesn't exist"));
			}
			if (!this->texture_exists(name)) {
				this->textures[name].loadFromFile(path);
			}
			this->sprites[name].setTexture(this->textures[name]);
		}
		void add_sprite_to_texture(const std::string& name, sf::Texture& texture) {

			this->sprites[name].setTexture(texture);
		}
		void add_sprite_to_texture(const std::string& name, const std::string& texture) {

			if (!this->texture_exists(texture)) {
				throw std::exception(ql::to_cstring("resources::add_sprite_to_texture(", name, "): texture \"", texture, "\" doesn't exist"));
			}
			this->sprites[name].setTexture(this->textures[texture]);
		}

		bool font_exists(const std::string& name) const {
			return this->fonts.find(name) != this->fonts.cend();
		}
		bool texture_exists(const std::string& name) const {
			return this->textures.find(name) != this->textures.cend();
		}
		bool sprite_exists(const std::string& name) const {
			return this->sprites.find(name) != this->sprites.cend();
		}

		sf::Font& get_font(const std::string& name) {
			return this->fonts[name];
		}
		sf::Texture& get_texture(const std::string& name) {
			return this->textures[name];
		}
		sf::Sprite& get_sprite(const std::string& name) {
			return this->sprites[name];
		}

		const sf::Font& get_font(const std::string& name) const {
			return this->fonts.at(name);
		}
		const sf::Texture& get_texture(const std::string& name)const {
			return this->textures.at(name);
		}
		const sf::Sprite& get_sprite(const std::string& name) const {
			return this->sprites.at(name);
		}

		std::unordered_map<std::string, sf::Font> fonts;
		std::unordered_map<std::string, sf::Texture> textures;
		std::unordered_map<std::string, sf::Sprite> sprites;
	};

	namespace detail {
		ql::resources resources;
	}

	void add_font(const std::string& name, const std::string& path) {
		detail::resources.add_font(name, path);
	}
	void add_texture(const std::string& name, const std::string& path) {
		detail::resources.add_texture(name, path);
	}
	void add_sprite(const std::string& name, const std::string& path) {
		detail::resources.add_sprite(name, path);
	}
	void add_sprite_to_texture(const std::string& name, sf::Texture& texture) {
		detail::resources.add_sprite_to_texture(name, texture);
	}
	void add_sprite_to_texture(const std::string& name, const std::string& texture) {
		detail::resources.add_sprite_to_texture(name, texture);
	}

	sf::Font& get_font(const std::string& name) {
		return detail::resources.get_font(name);
	}
	sf::Texture& get_texture(const std::string& name) {
		return detail::resources.get_texture(name);
	}
	sf::Sprite& get_sprite(const std::string& name) {
		return detail::resources.get_sprite(name);
	}

	bool font_exists(const std::string& name) {
		return detail::resources.font_exists(name);
	}
	bool texture_exists(const std::string& name) {
		return detail::resources.texture_exists(name);
	}
	bool sprite_exists(const std::string& name) {
		return detail::resources.sprite_exists(name);
	}

	//event_info
	// - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - 

	struct base_state;

	class event_info {
	public:
		bool key_single_pressed(sf::Keyboard::Key key) const {
			return this->m_keys_single_pressed.find(key) != this->m_keys_single_pressed.cend();
		}
		bool key_single_released(sf::Keyboard::Key key) const {
			return this->m_keys_single_released.find(key) != this->m_keys_single_released.cend();
		}
		bool key_pressed(sf::Keyboard::Key key) const {
			return this->m_keys_pressed.find(key) != this->m_keys_pressed.cend();
		}
		bool keys_pressed(const std::vector<sf::Keyboard::Key>& keys) const {
			for (auto& i : keys) {
				if (this->m_keys_pressed.find(i) == this->m_keys_pressed.cend()) {
					return false;
				}
			}
			return true;
		}
		bool key_released(sf::Keyboard::Key key) const {
			return this->m_keys_released.find(key) != this->m_keys_released.cend();
		}
		bool keys_released(const std::vector<sf::Keyboard::Key>& keys) const {
			for (auto& i : keys) {
				if (this->m_keys_released.find(i) == this->m_keys_released.cend()) {
					return false;
				}
			}
			return true;
		}
		bool key_holding(sf::Keyboard::Key key) const {
			return this->m_keys_holding.find(key) != this->m_keys_holding.cend();
		}
		bool keys_holding(const std::vector<sf::Keyboard::Key>& keys) const {
			for (auto& i : keys) {
				if (this->m_keys_holding.find(i) == this->m_keys_holding.cend()) {
					return false;
				}
			}
			return true;
		}

		bool mouse_button_clicked(sf::Mouse::Button button) const {
			if (button == sf::Mouse::Button::Left) {
				return this->m_left_mouse_clicked;
			}
			else if (button == sf::Mouse::Button::Middle) {
				return this->m_middle_mouse_clicked;
			}
			else if (button == sf::Mouse::Button::Right) {
				return this->m_right_mouse_clicked;
			}
			return false;
		}
		bool mouse_button_released(sf::Mouse::Button button) const {
			if (button == sf::Mouse::Button::Left) {
				return this->m_left_mouse_released;
			}
			else if (button == sf::Mouse::Button::Middle) {
				return this->m_middle_mouse_released;
			}
			else if (button == sf::Mouse::Button::Right) {
				return this->m_right_mouse_released;
			}
			return false;
		}
		bool mouse_button_holding(sf::Mouse::Button button) const {
			if (button == sf::Mouse::Button::Left) {
				return this->m_holding_left_mouse;
			}
			else if (button == sf::Mouse::Button::Middle) {
				return this->m_holding_middle_mouse;
			}
			else if (button == sf::Mouse::Button::Right) {
				return this->m_holding_right_mouse;
			}
			return false;
		}
		bool mouse_button_clicked() const {
			return this->m_mouse_clicked;
		}
		bool mouse_button_released() const {
			return this->m_mouse_released;
		}
		bool mouse_button_holding() const {
			return this->m_mouse_holding;
		}

		bool mouse_moved() const {
			return this->m_mouse_moved;
		}
		bool left_mouse_clicked() const {
			return this->m_left_mouse_clicked;
		}
		bool left_mouse_released() const {
			return this->m_left_mouse_released;
		}
		bool right_mouse_clicked() const {
			return this->m_right_mouse_clicked;
		}
		bool right_mouse_released() const {
			return this->m_right_mouse_released;
		}
		bool middle_mouse_clicked() const {
			return this->m_middle_mouse_clicked;
		}
		bool middle_mouse_released() const {
			return this->m_middle_mouse_released;
		}
		bool scrolled_up() const {
			return this->m_scrolled_up;
		}
		bool scrolled_down() const {
			return this->m_scrolled_down;
		}
		bool key_pressed() const {
			return this->m_key_pressed;
		}
		bool key_single_pressed() const {
			return this->m_key_single_pressed;
		}
		bool key_released() const {
			return this->m_key_released;
		}
		bool key_single_released() const {
			return this->m_key_single_released;
		}
		bool key_holding() const {
			return this->m_key_holding;
		}

		bool resized() const {
			return this->m_resized;
		}
		bool window_closed() const {
			return this->m_window_closed;
		}

		bool holding_left_mouse() const {
			return this->m_holding_left_mouse;
		}
		bool holding_right_mouse() const {
			return this->m_holding_right_mouse;
		}
		bool holding_middle_mouse() const {
			return this->m_holding_middle_mouse;
		}
		bool holding_key() const {
			return this->m_holding_key;
		}

		ql::vector2i resized_size() const {
			return this->m_resized_size;
		}
		bool text_entered(wchar_t c) const {
			auto str = this->text_entered();
			for (auto& i : str) {
				if (i == c) {
					return true;
				}
			}
			return false;
		}
		bool text_entered(std::wstring c) const {
			for (auto& i : c) {
				if (!this->text_entered(i)) {
					return false;
				}
			}
			return true;
		}


		ql::vector2i mouse_position() const {
			return this->m_mouse_position;
		}
		ql::vector2i mouse_position_desktop() const {
			return this->m_mouse_position_desktop;
		}
		std::wstring text_entered() const {
			return this->m_text_entered;
		}

		std::wstring all_text_entered() const {
			return this->m_text_entered_stream.str();
		}

		friend struct base_state;

	private:
		bool m_mouse_clicked;
		bool m_mouse_released;
		bool m_mouse_holding;
		bool m_left_mouse_clicked;
		bool m_left_mouse_released;
		bool m_right_mouse_clicked;
		bool m_right_mouse_released;
		bool m_middle_mouse_clicked;
		bool m_middle_mouse_released;
		bool m_scrolled_up;
		bool m_scrolled_down;
		bool m_key_pressed;
		bool m_key_single_pressed;
		bool m_key_released;
		bool m_key_single_released;
		bool m_key_holding;
		bool m_mouse_moved;
		bool m_window_closed;
		bool m_resized;

		bool m_holding_left_mouse;
		bool m_holding_right_mouse;
		bool m_holding_middle_mouse;
		bool m_holding_key;

		std::wstring m_text_entered;
		std::wostringstream m_text_entered_stream;

		ql::vector2i m_resized_size;
		ql::vector2i m_mouse_position;
		ql::vector2i m_mouse_position_desktop;
		std::set<sf::Keyboard::Key> m_keys_pressed;
		std::set<sf::Keyboard::Key> m_keys_released;
		std::set<sf::Keyboard::Key> m_keys_single_pressed;
		std::set<sf::Keyboard::Key> m_keys_single_released;
		std::set<sf::Keyboard::Key> m_keys_holding;
	};

	template<typename T>
	struct hitbox {
		ql::vector2<T> position;
		ql::vector2<T> dimension;

		template<typename U>
		bool contains(ql::vector2<U> position) const {
			return position.x >= this->position.x && position.x <= (this->position.x + this->dimension.y) &&
				   position.y >= this->position.y && position.y <= (this->position.y + this->dimension.y);
		}
	};

	ql::hitbox<ql::f32> text_hitbox(const sf::Text& text) {
		ql::hitbox<ql::f32> hitbox;
		auto local_bounds = text.getLocalBounds();
		auto global_bounds = text.getGlobalBounds();

		hitbox.position = { global_bounds.left, global_bounds.top };
		hitbox.dimension = { local_bounds.width, local_bounds.height };

		return hitbox;
	}
	void centerize_text(sf::Text& text) {
		auto rect = ql::text_hitbox(text).dimension;
		text.move(-rect / 2);
	}

	//drawables
	// - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - 

	struct draw_object;

	template<typename C>
	concept has_draw_object = requires (C x, draw_object & object) {
		x.draw(object);
	};
	template<typename C>
	concept has_draw_sf = requires (const C x, sf::RenderTarget & render, sf::RenderStates states) {
		x.draw(render, states);
	};

	struct draw_object {
		draw_object(sf::RenderWindow& window, sf::RenderStates states = sf::RenderStates::Default) {
			this->window = &window;
			this->states = states;
		}
		template<typename T>
		void draw(const T& object) {
			if constexpr (std::is_base_of<sf::Drawable, T>()) {
				this->window->draw(object, this->states);
			}
			else if constexpr (ql::has_draw_object<T>) {
				object.draw(*this->window, this->states);
			}
			else if constexpr (ql::has_draw_sf<T>) {
				object.draw(*this->window, this->states);
			}
			else {
				ql::println(ql::type_name<T>(), " is not drawable");
			}
		}
		sf::RenderWindow* window;
		sf::RenderStates states;
	};
	//base state / framework
	// - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - 


	struct base_state;

	struct framework {
		void draw_call();
		void display();

		void check_resize();

		bool game_loop_segment();
		bool game_loop_segment_no_display();
		bool game_loop_update_segment();
		void game_loop();
		ql::time run_time() const;
		ql::time frame_time() const;

		void add_font(const std::string& name, const std::string& path);
		void add_texture(const std::string& name, const std::string& path);
		void add_sprite(const std::string& name, const std::string& path);

		sf::Font& get_font(const std::string& name);
		sf::Texture& get_texture(const std::string& name);
		sf::Sprite& get_sprite(const std::string& name);

		const sf::Font& get_font(const std::string& name) const;
		const sf::Texture& get_texture(const std::string& name) const;
		const sf::Sprite& get_sprite(const std::string& name) const;


		template<typename C>
		void add_state() {
			this->states.push_back(std::make_unique<C>());
			this->states.back()->framework = this;
			this->states.back()->init();
		}
		template<typename C>
		void add_state(C& state) {
			this->states.push_back(std::make_unique<C>(state));
			this->states.back()->framework = this;
			this->states.back()->init();
		}

		void create();
		bool is_open() const;
		bool is_created() const;
		void set_info(const std::string& title, ql::vector2u dimension, ql::u32 style);
		void set_title(const std::string& title);
		void set_dimension(ql::vector2u dimension);
		void set_antialising(ql::u32 antialising);
		ql::u32 get_antialising() const;
		void set_style(ql::u32 style);
		void hide_cursor();
		void set_window_position(ql::vector2u position);
		ql::vector2u get_window_position() const;
		void show_cursor();
		void set_cursor_position(ql::vector2i position);

		std::vector<std::unique_ptr<ql::base_state>> states;
		sf::RenderWindow window;
		std::string m_title;
		ql::vector2u m_dimension;
		ql::u32 m_style = sf::Style::Default;
		ql::clock m_run_time_clock;
		ql::clock m_frametime_clock;
		ql::time m_frametime;
		ql::u32 m_antialising = 12u;
		bool m_created = false;
	};

	/* TO OVERLOAD:
		void init() override {
			
		}
		void updating() override {
			
		}
		void drawing() override {
			
		}
	*/
	struct base_state {
		virtual void init() = 0;
		virtual void updating() = 0;
		virtual void drawing() = 0;

		void clear() {
			this->framework->window.clear(this->clear_color);
		}
		virtual void call_on_resize() {

		}
		virtual void call_on_close() {

		}
		virtual void call_after_window_create() {

		}
		void draw_call() {
			this->framework->draw_call();
		}
		void display() {
			this->framework->display();
		}
		bool game_loop_update_segment() {
			return this->framework->game_loop_update_segment();
		}
		bool game_loop_segment() {
			return this->framework->game_loop_segment();
		}
		bool game_loop_segment_no_display() {
			return this->framework->game_loop_segment_no_display();
		}
		void create() {
			return this->framework->create();
		}
		bool is_open() const {
			return this->framework->is_open();
		}
		void event_update() {
			sf::Event event;


			this->event.m_mouse_clicked = false;
			this->event.m_mouse_released = false;
			this->event.m_left_mouse_clicked = false;
			this->event.m_left_mouse_released = false;
			this->event.m_right_mouse_clicked = false;
			this->event.m_right_mouse_released = false;
			this->event.m_middle_mouse_clicked = false;
			this->event.m_middle_mouse_released = false;
			this->event.m_scrolled_up = false;
			this->event.m_scrolled_down = false;
			this->event.m_key_pressed = false;
			this->event.m_key_single_pressed = false;
			this->event.m_key_released = false;
			this->event.m_mouse_moved = false;
			this->event.m_key_holding = false;
			this->event.m_window_closed = false;
			this->event.m_resized = false;

			this->event.m_keys_pressed.clear();
			this->event.m_keys_released.clear();
			this->event.m_keys_single_pressed.clear();

			this->event.m_text_entered.clear();

			while (this->framework->window.pollEvent(event)) {
				if (event.type == sf::Event::TextEntered) {
					this->event.m_text_entered.push_back(event.text.unicode);
					this->event.m_text_entered_stream << (wchar_t)event.text.unicode;

				}
				if (event.type == sf::Event::MouseButtonPressed) {
					this->event.m_mouse_clicked = true;
					this->event.m_mouse_holding = true;
					if (event.mouseButton.button == sf::Mouse::Left) {
						this->event.m_left_mouse_clicked = true;
						this->event.m_holding_left_mouse = true;
					}
					else if (event.mouseButton.button == sf::Mouse::Right) {
						this->event.m_right_mouse_clicked = true;
						this->event.m_holding_right_mouse = true;
					}
					else if (event.mouseButton.button == sf::Mouse::Middle) {
						this->event.m_middle_mouse_clicked = true;
						this->event.m_holding_middle_mouse = true;
					}
				}
				else if (event.type == sf::Event::MouseButtonReleased) {
					this->event.m_mouse_released = true;
					this->event.m_mouse_holding = false;
					if (event.mouseButton.button == sf::Mouse::Left) {
						this->event.m_left_mouse_released = true;
						this->event.m_holding_left_mouse = false;
					}
					else if (event.mouseButton.button == sf::Mouse::Right) {
						this->event.m_right_mouse_released = true;
						this->event.m_holding_right_mouse = false;
					}
					else if (event.mouseButton.button == sf::Mouse::Middle) {
						this->event.m_middle_mouse_released = true;
						this->event.m_holding_middle_mouse = false;
					}
				}
				else if (event.type == sf::Event::KeyPressed) {
					this->event.m_key_pressed = true;
					this->event.m_key_holding = true;
					this->event.m_keys_pressed.insert(event.key.code);
					if (!this->event.key_holding(event.key.code)) {
						this->event.m_keys_single_pressed.insert(event.key.code);
						this->event.m_key_single_pressed = true;
					}
					this->event.m_keys_holding.insert(event.key.code);

				}
				else if (event.type == sf::Event::KeyReleased) {
					this->event.m_key_released = true;
					this->event.m_key_holding = false;
					this->event.m_keys_released.insert(event.key.code);
					this->event.m_keys_holding.erase(event.key.code);
				}
				else if (event.type == sf::Event::MouseWheelMoved) {
					if (event.mouseWheel.delta < 0) {
						this->event.m_scrolled_down = true;
					}
					if (event.mouseWheel.delta > 0) {
						this->event.m_scrolled_up = true;
					}
				}
				else if (event.type == sf::Event::Closed) {
					this->event.m_window_closed = true;
				}
				else if (event.type == sf::Event::MouseMoved) {
					this->event.m_mouse_moved = true;
				}
				else if (event.type == sf::Event::Resized) {
					this->event.m_resized = true;
					this->event.m_resized_size = { event.size.width, event.size.height };
				}
			}
			this->event.m_mouse_position = sf::Mouse::getPosition(this->framework->window);
			this->event.m_mouse_position_desktop = sf::Mouse::getPosition();
		}
		void update_close_window() {
			if (this->event.window_closed() && this->m_allow_exit) {
				this->framework->window.close();
				this->call_on_close();
			}
		}
		void hide_cursor() {
			this->framework->hide_cursor();
		}
		void show_cursor() {
			this->framework->show_cursor();
		}
		void set_cursor_position(ql::vector2i position) {
			this->framework->set_cursor_position(position);
		}
		void set_window_position(ql::vector2u position) {
			this->framework->set_window_position(position);
		}
		ql::vector2u get_window_position() const {
			return this->framework->get_window_position();
		}
		ql::vector2i dimension() const {
			return this->framework->m_dimension;
		}
		ql::vector2f center() const {
			return ql::vector2f(this->framework->m_dimension) / 2;
		}
		void add_font(const std::string& name, const std::string& path) {
			ql::add_font(name, path);
		}
		void add_texture(const std::string& name, const std::string& path) {
			ql::add_texture(name, path);
		}
		void add_sprite(const std::string& name, const std::string& path) {
			ql::add_sprite(name, path);
		}
		void add_sprite_to_texture(const std::string& name, sf::Texture& texture) {
			ql::add_sprite_to_texture(name, texture);
		}
		void add_sprite_to_texture(const std::string& name, std::string& texture) {
			ql::add_sprite_to_texture(name, texture);
		}


		sf::Font& get_font(const std::string& name) {
			return ql::get_font(name);
		}
		sf::Texture& get_texture(const std::string& name) {
			return ql::get_texture(name);
		}
		sf::Sprite& get_sprite(const std::string& name) {
			return ql::get_sprite(name);
		}

		const sf::Font& get_font(const std::string& name) const {
			return ql::get_font(name);
		}
		const sf::Texture& get_texture(const std::string& name) const {
			return ql::get_texture(name);
		}
		const sf::Sprite& get_sprite(const std::string& name) const {
			return ql::get_sprite(name);
		}

		void pop_this_state() {
			this->m_pop_this_state = true;
		}
		void allow_exit() {
			this->m_allow_exit = true;
		}
		void disallow_exit() {
			this->m_allow_exit = false;
		}
		bool is_exit_allowed() const {
			return this->m_allow_exit;
		}
		void allow_clear() {
			this->m_allow_clear = true;
		}
		void disallow_clear() {
			this->m_allow_clear = false;
		}
		bool is_clear_allowed() const {
			return this->m_allow_clear;
		}
		void allow_display() {
			this->m_allow_display = true;
		}
		void disallow_display() {
			this->m_allow_display = false;
		}
		bool is_display_allowed() const {
			return this->m_allow_display;
		}
		ql::time frame_time() const {
			return this->framework->frame_time();
		}
		ql::time run_time() const {
			return this->framework->run_time();
		}

		template<typename T>
		void draw(const T& drawable, sf::RenderStates states = sf::RenderStates::Default) {
			if constexpr (std::is_base_of<sf::Drawable, T>()) {
				this->framework->window.draw(drawable, states);
			}
			else if constexpr (ql::has_draw_object<T>) {
				draw_object draw(this->framework->window, states);
				drawable.draw(draw);
			}
			else if constexpr (ql::has_draw_sf<T>) {
				drawable.draw(this->framework->window, states);
			}
			else {
				ql::println(ql::type_name<T>(), " is not drawable");
			}
		}
		template<typename T>
		void update(T& updatable) {
			updatable.update(this->event);
		}


		template<typename C>
		void add_state() {
			this->framework->add_state<C>();
		}

		ql::framework* framework;
		ql::event_info event;

		ql::rgb clear_color = ql::rgb::black;
		bool m_pop_this_state = false;
		bool m_allow_exit = true;
		bool m_allow_clear = true;
		bool m_allow_display = true;
	};

	void framework::draw_call() {
		if (this->states.back()->is_clear_allowed()) {
			this->states.back()->clear();
		}
		this->states.back()->drawing();
		if (this->states.back()->is_display_allowed()) {
			this->window.display();
		}
	}
	void framework::display() {
		this->window.display();
	}
	void framework::check_resize() {
		if (this->states.back()->event.resized()) {
			auto new_dimension = this->states.back()->event.resized_size();
			sf::FloatRect view(0.0f, 0.0f, static_cast<ql::f32>(new_dimension.x), static_cast<ql::f32>(new_dimension.y));
			this->window.setView(sf::View(view));
			this->m_dimension = new_dimension;
			this->states.back()->call_on_resize();
		}
	}
	bool framework::game_loop_segment() {
		if (!this->is_created()) {
			this->create();
		}

		this->m_frametime = this->m_frametime_clock.elapsed_reset();

		this->states.back()->event_update();

		this->check_resize();

		this->states.back()->updating();
		this->states.back()->update_close_window();

		if (this->states.back()->m_pop_this_state) {
			this->states.pop_back();
			if (this->states.empty()) {
				return false;
			}
		}
		this->draw_call();
		return true;
	}
	bool framework::game_loop_segment_no_display() {
		if (!this->is_created()) {
			this->create();
		}

		this->m_frametime = this->m_frametime_clock.elapsed_reset();

		this->states.back()->event_update();

		this->check_resize();
		this->states.back()->updating();
		this->states.back()->update_close_window();

		if (this->states.back()->m_pop_this_state) {
			this->states.pop_back();
			if (this->states.empty()) {
				return false;
			}
		}
		if (this->states.back()->is_clear_allowed()) {
			this->states.back()->clear();
		}
		this->states.back()->drawing();
		return true;
	}
	bool framework::game_loop_update_segment() {
		if (!this->is_created()) {
			this->create();
		}

		this->m_frametime = this->m_frametime_clock.elapsed_reset();

		this->states.back()->event_update();

		this->check_resize();
		this->states.back()->update_close_window();

		if (this->states.back()->m_pop_this_state) {
			this->states.pop_back();
			if (this->states.empty()) {
				return false;
			}
		}
		return true;
	}
	void framework::game_loop() {
		if (!this->is_created()) {
			this->create();
		}

		while (this->is_open()) {
			if (!this->game_loop_segment()) {
				break;
			}
		}
	}
	ql::time framework::run_time() const {
		return this->m_run_time_clock.elapsed();
	}
	ql::time framework::frame_time() const {
		return this->m_frametime;
	}

	void framework::add_font(const std::string& name, const std::string& path) {
		ql::add_font(name, path);
	}
	void framework::add_texture(const std::string& name, const std::string& path) {
		ql::add_texture(name, path);
	}
	void framework::add_sprite(const std::string& name, const std::string& path) {
		ql::add_sprite(name, path);
	}

	sf::Font& framework::get_font(const std::string& name) {
		return ql::get_font(name);
	}
	sf::Texture& framework::get_texture(const std::string& name) {
		return ql::get_texture(name);
	}
	sf::Sprite& framework::get_sprite(const std::string& name) {
		return ql::get_sprite(name);
	}

	const sf::Font& framework::get_font(const std::string& name) const {
		return ql::get_font(name);
	}
	const sf::Texture& framework::get_texture(const std::string& name) const {
		return ql::get_texture(name);
	}
	const sf::Sprite& framework::get_sprite(const std::string& name) const {
		return ql::get_sprite(name);
	}

	void framework::create() {
		if (!this->is_created()) {
			sf::ContextSettings settings;
			settings.antialiasingLevel = this->m_antialising;

			sf::String s = this->m_title.c_str(); //??? SFML why is this needed
			this->window.create(sf::VideoMode({ this->m_dimension.x, this->m_dimension.y }), s, this->m_style, settings);
			this->m_created = true;

			if (this->states.size()) {
				this->states.back()->call_after_window_create();
			}
		}
	}
	bool framework::is_open() const {
		return this->window.isOpen();
	}
	bool framework::is_created() const {
		return this->m_created;
	}
	void framework::set_info(const std::string& title, ql::vector2u dimension, ql::u32 style) {
		this->set_title(title);
		this->set_dimension(dimension);
		this->set_style(style);
	}
	void framework::set_title(const std::string& title) {
		this->m_title = title;
		if (this->m_created) {
			this->window.setTitle(title);
		}
	}
	void framework::set_dimension(ql::vector2u dimension) {
		this->m_dimension = dimension;
	}
	void framework::set_antialising(ql::u32 antialising) {
		this->m_antialising = antialising;
	}
	ql::u32 framework::get_antialising() const {
		return this->m_antialising;
	}
	void framework::set_style(ql::u32 style) {
		this->m_style = style;
	}
	void framework::hide_cursor() {
		this->window.setMouseCursorVisible(false);
		//todo
	}
	void framework::set_window_position(ql::vector2u position) {
		this->window.setPosition(sf::Vector2i(position));
	}
	ql::vector2u framework::get_window_position() const {
		return ql::vector2u(this->window.getPosition());
	}
	void framework::show_cursor() {
		this->window.setMouseCursorVisible(true);
		//todo
	}
	void framework::set_cursor_position(ql::vector2i position) {
		//todo
	}
#endif
}



#endif