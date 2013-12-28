
// Copyright (c) 2010-2014 niXman (i dot nixman dog gmail dot com)
//
// Distributed under the Boost Software License, Version 1.0. (See accompanying
// file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
//
//
//
// Boost Software License - Version 1.0 - August 17th, 2003
//
// Permission is hereby granted, free of charge, to any person or organization
// obtaining a copy of the software and accompanying documentation covered by
// this license (the "Software") to use, reproduce, display, distribute,
// execute, and transmit the Software, and to prepare derivative works of the
// Software, and to permit third-parties to whom the Software is furnished to
// do so, all subject to the following:
//
// The copyright notices in the Software and this entire statement, including
// the above license grant, this restriction and the following disclaimer,
// must be included in all copies of the Software, in whole or in part, and
// all derivative works of the Software, unless such copies or derivative
// works are solely in the form of machine-executable object code generated by
// a source language processor.
//
// THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
// IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
// FITNESS FOR A PARTICULAR PURPOSE, TITLE AND NON-INFRINGEMENT. IN NO EVENT
// SHALL THE COPYRIGHT HOLDERS OR ANYONE DISTRIBUTING THE SOFTWARE BE LIABLE
// FOR ANY DAMAGES OR OTHER LIABILITY, WHETHER IN CONTRACT, TORT OR OTHERWISE,
// ARISING FROM, OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER
// DEALINGS IN THE SOFTWARE.

#ifndef _yas__detail__io__streams_hpp
#define _yas__detail__io__streams_hpp

#include <yas/detail/config/config.hpp>

#include <yas/detail/io/io_exceptions.hpp>
#include <yas/detail/io/convertors.hpp>
#include <yas/detail/io/endian_conv.hpp>
#include <yas/detail/type_traits/properties.hpp>
#include <yas/detail/type_traits/type_traits.hpp>

#include <limits>

namespace yas {
namespace detail {

template<yas::archive_type::type, yas::direction::type, typename IO>
struct stream;

/***************************************************************************/

template<typename IS>
struct stream<yas::archive_type::binary, yas::direction::in, IS> {
	stream(IS &is)
		:is(is)
	{}

	// for arrays
	void read(void *ptr, std::size_t size) {
		YAS_THROW_ON_READ_ERROR(size, !=, is.read(ptr, size));
	}

	// for chars & bools
	template<typename T>
	void read(T &v, typename enable_if_is_any_of<T, char, signed char, unsigned char, bool>::type* = 0) {
		YAS_THROW_ON_READ_ERROR(sizeof(v), !=, is.read(&v, sizeof(v)));
	}

	// for shorts
	template<typename T>
	void read(T &v, typename enable_if_is_any_of<T, std::int16_t, std::uint16_t>::type* = 0) {
		YAS_THROW_ON_READ_ERROR(sizeof(v), !=, is.read(&v, sizeof(v)));
		YAS_NETWORK_TO_LOCAL16(v, v);
	}

	// for 32-bit ints but not for floats
	template<typename T>
	void read(T &v, typename enable_if_is_any_of<T, std::int32_t, std::uint32_t>::type* = 0) {
		YAS_THROW_ON_READ_ERROR(sizeof(v), !=, is.read(&v, sizeof(v)));
		YAS_NETWORK_TO_LOCAL32(v, v);
	}

	// for 64-bit ints but not for floats
	template<typename T>
	void read(T &v, typename enable_if_is_any_of<T, std::int64_t, std::uint64_t>::type* = 0) {
		YAS_THROW_ON_READ_ERROR(sizeof(v), !=, is.read(&v, sizeof(v)));
		YAS_NETWORK_TO_LOCAL64(v, v);
	}

	// for floats and doubles
	template<typename T>
	void read(T &v, typename enable_if_is_any_of<T, float, double>::type* = 0) {
		std::uint8_t buf[sizeof(T)];
		YAS_THROW_ON_READ_ERROR(sizeof(T), !=, is.read(buf, sizeof(buf)));
		endian_convertor<YAS_LITTLE_ENDIAN()>::from_network(v, buf);
	}

private:
	IS &is;
};

template<typename OS>
struct stream<yas::archive_type::binary, yas::direction::out, OS> {
	stream(OS &os)
		:os(os)
	{}

	// for arrays
	void write(const void *ptr, std::size_t size) {
		YAS_THROW_ON_WRITE_ERROR(size, !=, os.write(ptr, size));
	}

	// for chars & bools
	template<typename T>
	void write(T v, typename enable_if_is_any_of<T, char, signed char, unsigned char, bool>::type* = 0) {
		YAS_THROW_ON_WRITE_ERROR(sizeof(v), !=, os.write(&v, sizeof(v)));
	}

	// for shorts
	template<typename T>
	void write(T v, typename enable_if_is_any_of<T, std::int16_t, std::uint16_t>::type* = 0) {
		YAS_LOCAL_TO_NETWORK16(v, v);
		YAS_THROW_ON_WRITE_ERROR(sizeof(v), !=, os.write(&v, sizeof(v)));
	}

	// for 32-bit ints but not for floats
	template<typename T>
	void write(T v, typename enable_if_is_any_of<T, std::int32_t, std::uint32_t>::type* = 0) {
		YAS_LOCAL_TO_NETWORK32(v, v);
		YAS_THROW_ON_WRITE_ERROR(sizeof(v), !=, os.write(&v, sizeof(v)));
	}

	// for 64-bit ibts but not for floats
	template<typename T>
	void write(T v, typename enable_if_is_any_of<T, std::int64_t, std::uint64_t>::type* = 0) {
		YAS_LOCAL_TO_NETWORK64(v, v);
		YAS_THROW_ON_WRITE_ERROR(sizeof(v), !=, os.write(&v, sizeof(v)));
	}

	// for floats and doubles
	template<typename T>
	void write(const T &v, typename enable_if_is_any_of<T, float, double>::type* = 0) {
		std::uint8_t buf[sizeof(T)];
		endian_convertor<YAS_LITTLE_ENDIAN()>::to_network(buf, v);
		YAS_THROW_ON_WRITE_ERROR(sizeof(buf), !=, os.write(buf, sizeof(buf)));
	}

private:
	OS &os;
};

/***************************************************************************/

#define YAS_READ_BY_CHAR(buf, cnt) \
	for ( char *cur = &buf[0], *end = &buf[sizeof(buf)-1]; cur != end; ++cur ) { \
		*cur = getch(); \
		if ( *cur == space_sep || *cur == EOF) { \
			if ( *cur == space_sep ) \
				ungetch(*cur); \
			*cur = 0; \
			break; \
		} else { \
			++cnt; \
		} \
	}

template<typename IS>
struct stream<yas::archive_type::text, yas::direction::in, IS> {
	stream(IS &is)
		:is(is)
		,unch(EOF)
	{}

	char getch() {
		char res = EOF;
		if ( unch != EOF ) {
			res = unch;
			unch = EOF;
			return res;
		}
		is.read(&res, sizeof(res));
		return res;
	}
	void ungetch(char c) { unch = c; }

	// for arrays
	void read(void *ptr, std::size_t size) {
		char *sptr = ((char*)ptr);
		if ( unch != EOF ) {
			*sptr++ = unch;
			unch = EOF;
			--size;
		}
		if ( size == 0 ) return;
		YAS_THROW_ON_READ_ERROR(size, !=, is.read(sptr, size));
	}

	// for chars only
	template<typename T>
	void read(T &v, typename enable_if_is_any_of<T, char, signed char, unsigned char>::type* = 0) {
		YAS_THROW_ON_READ_ERROR(sizeof(v), !=, is.read(&v, sizeof(v)));
	}

	// for bools only
	template<typename T>
	void read(T &v, typename enable_if_is_any_of<T, bool>::type* = 0) {
		const char ch = getch();
		if ( ch == EOF ) return;
		v = (ch == '1' ? true : false);
	}

	// for 16-bits/32-bits/64-bits
	template<typename T>
	void read(T &v, typename enable_if_is_any_of<T, std::int16_t, std::uint16_t, std::int32_t, std::uint32_t, std::int64_t, std::uint64_t>::type* = 0) {
		char buf[sizeof(T)*4] = "\0";
		std::uint32_t cnt = 0;
		YAS_READ_BY_CHAR(buf, cnt)
		v = (std::is_unsigned<T>::value ? atou<T>(buf, cnt) : atoi<T>(buf, cnt));
	}

	// for floats and doubles
	template<typename T>
	void read(T &v, typename enable_if_is_any_of<T, float, double>::type* = 0) {
		char buf[std::numeric_limits<T>::max_exponent10+20] = "\0";
		std::uint32_t cnt = 0;
		YAS_READ_BY_CHAR(buf, cnt)
		v = atod<T>(buf);
	}

private:
	IS &is;
	char unch;
};

template<typename OS>
struct stream<yas::archive_type::text, yas::direction::out, OS> {
	stream(OS &os)
		:os(os)
	{}

	// for arrays
	void write(const void *ptr, std::size_t size) {
		YAS_THROW_ON_WRITE_ERROR(size, !=, os.write(ptr, size));
	}

	// for chars only
	template<typename T>
	void write(const T &v, typename enable_if_is_any_of<T, char, signed char, unsigned char>::type* = 0) {
		YAS_THROW_ON_WRITE_ERROR(sizeof(v), !=, os.write(&v, sizeof(v)));
	}

	// for bools only
	template<typename T>
	void write(const T &v, typename enable_if_is_any_of<T, bool>::type* = 0) {
		const char c = v ? '1' : '0';
		YAS_THROW_ON_WRITE_ERROR(sizeof(c), !=, os.write(&c, sizeof(v)));
	}

	// for 16-bits/32-bits/64-bits
	template<typename T>
	void write(const T &v, typename enable_if_is_any_of<T, std::int16_t, std::uint16_t, std::int32_t, std::uint32_t, std::int64_t, std::uint64_t>::type* = 0) {
		enum { bufsize = sizeof(T)*4 };
		char buf[bufsize+1] = "\0";
		const int n = (std::is_unsigned<T>::value ? utoa(v, buf) : itoa(v, buf));
		YAS_THROW_ON_WRITE_ERROR(n, !=, os.write(buf, n));
	}

	// for floats and doubles
	template<typename T>
	void write(const T &v, typename enable_if_is_any_of<T, float, double>::type* = 0) {
		enum { bufsize = std::numeric_limits<T>::max_exponent10 + 20 };
		char buf[bufsize+1] = "\0";
		const int n = modp_dtoa(v, buf, 4);
		YAS_THROW_ON_WRITE_ERROR(n, !=, os.write(buf, n));
	}

private:
	OS &os;
};

/***************************************************************************/

template<typename IS>
struct stream<yas::archive_type::json, yas::direction::in, IS>
	: stream<yas::archive_type::text, yas::direction::in, IS>
{
	stream(IS &is)
		:stream<yas::archive_type::text, yas::direction::in, IS>(is)
	{}
};

template<typename OS>
struct stream<yas::archive_type::json, yas::direction::out, OS>
	: stream<yas::archive_type::text, yas::direction::out, OS>
{
	stream(OS &os)
		:stream<yas::archive_type::text, yas::direction::out, OS>(os)
	{}
};

/***************************************************************************/

} // ns detail
} // ns yas

#endif // _yas__detail__io__streams_hpp
