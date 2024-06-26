
// Copyright (c) 2010-2023 niXman (github dot nixman at pm dot me). All
// rights reserved.
//
// This file is part of YAS(https://github.com/niXman/yas) project.
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

#ifndef __yas__tests__base__include__map_hpp
#define __yas__tests__base__include__map_hpp

/***************************************************************************/

template<typename archive_traits>
bool map_test(std::ostream &log, const char *archive_type, const char *test_name) {
	std::map<int, int> imap, imap2;
	imap.emplace(1, 2);
	imap.emplace(1, 2);
	imap.emplace(2, 3);
	imap.emplace(3, 4);

	typename archive_traits::oarchive oa;
	archive_traits::ocreate(oa, archive_type);
	oa & YAS_OBJECT_NVP("obj", ("map", imap));

	typename archive_traits::iarchive ia;
	archive_traits::icreate(ia, oa, archive_type);
	ia & YAS_OBJECT_NVP("obj", ("map", imap2));

	if ( imap != imap2 ) {
		YAS_TEST_REPORT(log, archive_type, test_name);
		return false;
	}

	std::map<int, std::string> map, map2;
	map.emplace(1, "1");
	map.emplace(2, "2");
	map.emplace(3, "3");

	typename archive_traits::oarchive oa2;
	archive_traits::ocreate(oa2, archive_type);
	oa2 & YAS_OBJECT_NVP("obj", ("map", map));

	typename archive_traits::iarchive ia2;
	archive_traits::icreate(ia2, oa2, archive_type);
	ia2 & YAS_OBJECT_NVP("obj", ("map", map2));

	if ( map != map2 ) {
		YAS_TEST_REPORT(log, archive_type, test_name);
		return false;
	}

	std::map<std::string, int> map3, map4;
	map3.emplace("1", 1);
	map3.emplace("2", 2);
	map3.emplace("3", 3);

	typename archive_traits::oarchive oa3;
	archive_traits::ocreate(oa3, archive_type);
	oa3 & YAS_OBJECT_NVP("obj", ("map", map3));

	typename archive_traits::iarchive ia3;
	archive_traits::icreate(ia3, oa3, archive_type);
	ia3 & YAS_OBJECT_NVP("obj", ("map", map4));

	if ( map3 != map4 ) {
		YAS_TEST_REPORT(log, archive_type, test_name);
		return false;
	}

#if __cplusplus >= 201402L
    std::map<std::string, std::string, std::less<>> map5, map6;
    map5.emplace("1", "1");
    map5.emplace("2", "2");
    map5.emplace("3", "3");

    typename archive_traits::oarchive oa4;
    archive_traits::ocreate(oa4, archive_type);
    oa4 & YAS_OBJECT_NVP("obj", ("map", map5));

    typename archive_traits::iarchive ia4;
    archive_traits::icreate(ia4, oa4, archive_type);
    ia4 & YAS_OBJECT_NVP("obj", ("map", map6));

    if ( map5 != map6 ) {
        YAS_TEST_REPORT(log, archive_type, test_name);
        return false;
    }
#endif // __cplusplus >= 201402L

	return true;
}

/***************************************************************************/

#endif // __yas__tests__base__include__map_hpp
