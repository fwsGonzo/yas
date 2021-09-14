
// Copyright (c) 2010-2021 niXman (github dot nixman at pm dot me). All
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

#ifndef __yas__detail__type_traits__flags_hpp
#define __yas__detail__type_traits__flags_hpp

#include <cstdint>
#include <type_traits>

namespace yas {

/***************************************************************************/

enum options: std::uint32_t {
    binary    = 1u<<0
    ,text      = 1u<<1
    ,json      = 1u<<2
    ,no_header = 1u<<3
    ,elittle   = 1u<<4
    ,ebig      = 1u<<5
    ,ehost     = 1u<<6
    ,compacted = 1u<<7
    ,mem       = 1u<<8
    ,file      = 1u<<9
};

template<typename Ar>
struct is_binary_archive: std::integral_constant<bool, Ar::type() == options::binary>
{};

template<typename Ar>
struct is_text_archive: std::integral_constant<bool, Ar::type() == options::text>
{};

template<typename Ar>
struct is_json_archive: std::integral_constant<bool, Ar::type() == options::json>
{};

template<typename Ar>
struct is_readable_archive: std::integral_constant<bool, Ar::is_readable()>
{};

template<typename Ar>
struct is_writable_archive: std::integral_constant<bool, Ar::is_writable()>
{};

/***************************************************************************/

} // ns yas

#endif // __yas__detail__type_traits__flags_hpp
