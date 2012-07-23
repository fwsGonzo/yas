
// Copyright (c) 2010-2012 niXman (i dot nixman dog gmail dot com)
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

#ifndef _yas_test__fusion_tuple_hpp__included_
#define _yas_test__fusion_tuple_hpp__included_

/***************************************************************************/

#if defined(YAS_HAS_BOOST_FUSION)

template<typename archive_traits>
bool fusion_tuple_test(const char* archive_type, const char* io_type) {
	boost::fusion::tuple<int, double> v1(33, 3.14), v2;

	typename archive_traits::oarchive oa;
	archive_traits::ocreate(oa, archive_type, io_type);
	oa & v1;

	typename archive_traits::iarchive ia;
	archive_traits::icreate(ia, oa, archive_type, io_type);
	ia & v2;
	if ( v1 != v2 ) {
		std::cout << "FUSION_TUPLE deserialization error! [1]" << std::endl;
		return false;
	}

	std::set<std::string> set;
	set.insert("2");
	set.insert("3");
	set.insert("4");

	boost::fusion::tuple<
		std::string,
		std::set<std::string>
	> v3("1", set), v4;

	typename archive_traits::oarchive oa2;
	archive_traits::ocreate(oa2, archive_type, io_type);
	oa2 & v3;

	typename archive_traits::iarchive ia2;
	archive_traits::icreate(ia2, oa2, archive_type, io_type);
	ia2 & v4;
	if ( v3 != v4 ) {
		std::cout << "FUSION_TUPLE deserialization error! [2]" << std::endl;
		return false;
	}

	boost::fusion::tuple<int, int> vv;

	typename archive_traits::oarchive oa3;
	archive_traits::ocreate(oa3, archive_type, io_type);
	oa3 & boost::fusion::make_tuple(33,44);

	typename archive_traits::iarchive ia3;
	archive_traits::icreate(ia3, oa3, archive_type, io_type);
	ia3 & vv;

	if ( vv != boost::fusion::make_tuple(33,44) ) {
		std::cout << "FUSION_TUPLE deserialization error! [3]" << std::endl;
		return false;
	}

	static const char str[] = "str";
	boost::fusion::tuple<yas::uint64_t, std::string> v5(33, str), v6;

	typename archive_traits::oarchive oa4;
	archive_traits::ocreate(oa4, archive_type, io_type);
	oa4 & v5;

	const size_t expected_size =
		4+ // archive information
		sizeof(yas::uint8_t)+ // fusion::tuple size marker
		sizeof(yas::uint64_t)+ // first type
		sizeof(yas::uint32_t)+ // string size marker
		strlen(str); // string length
	if ( yas::is_binary_archive<typename archive_traits::oarchive_type>::value ) {
		const size_t current_size = oa4.size();
		if ( current_size != expected_size ) {
			std::cout << "FUSION_TUPLE deserialization error! [4]" << std::endl;
			return false;
		}
	}

	typename archive_traits::iarchive ia4;
	archive_traits::icreate(ia4, oa4, archive_type, io_type);
	ia4 & v6;
	if ( v5 != v6 ) {
		std::cout << "FUSION_TUPLE deserialization error! [5]" << std::endl;
		return false;
	}

	typename archive_traits::oarchive oa5;
	archive_traits::ocreate(oa5, archive_type, io_type);
	oa5 & boost::fusion::make_tuple<yas::uint64_t, std::string>(33, "str");

	if ( yas::is_binary_archive<typename archive_traits::oarchive_type>::value ) {
		const size_t current_size2 = oa5.size();

		if ( current_size2 != expected_size ) {
			std::cout << "FUSION_TUPLE deserialization error! [6]" << std::endl;
			return false;
		}
	}

	typename archive_traits::iarchive ia5;
	archive_traits::icreate(ia5, oa5, archive_type, io_type);
	ia5 & v6;
	if ( v5 != v6 ) {
		std::cout << "FUSION_TUPLE deserialization error! [7]" << std::endl;
		return false;
	}

	return true;
}

#endif // #if defined(YAS_HAS_BOOST_FUSION)

/***************************************************************************/

#endif // _yas_test__fusion_tuple_hpp__included_