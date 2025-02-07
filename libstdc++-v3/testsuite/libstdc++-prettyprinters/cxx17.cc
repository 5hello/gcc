// { dg-options "-g -O0 -std=gnu++1z" }
// { dg-do run { target c++1z } }

// Copyright (C) 2014-2016 Free Software Foundation, Inc.
//
// This file is part of the GNU ISO C++ Library.  This library is free
// software; you can redistribute it and/or modify it under the
// terms of the GNU General Public License as published by the
// Free Software Foundation; either version 3, or (at your option)
// any later version.

// This library is distributed in the hope that it will be useful,
// but WITHOUT ANY WARRANTY; without even the implied warranty of
// MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
// GNU General Public License for more details.

// You should have received a copy of the GNU General Public License along
// with this library; see the file COPYING3.  If not see
// <http://www.gnu.org/licenses/>.

// Type printers only recognize the old std::string for now.
#define _GLIBCXX_USE_CXX11_ABI 0

#include <any>
#include <optional>
#include <variant>
#include <string_view>
#include <string>
#include <map>
#include <unordered_set>
#include <iostream>

using std::any;
using std::optional;
using std::variant;
using std::string_view;
using std::map;
using std::unordered_set;

int
main()
{
  string_view str = "string";
// { dg-final { note-test str "\"string\"" } }

  optional<int> o;
// { dg-final { note-test o {std::optional<int> [no contained value]} } }
  optional<bool> ob{false};
// { dg-final { note-test ob {std::optional<bool> = {[contained value] = false}} } }
  optional<int> oi{5};
// { dg-final { note-test oi {std::optional<int> = {[contained value] = 5}} } }
  optional<void*> op{nullptr};
// { dg-final { note-test op {std::optional<void *> = {[contained value] = 0x0}} } }
  optional<std::map<int, double>> om;
  om = std::map<int, double>{ {1, 2.}, {3, 4.}, {5, 6.} };
// { dg-final { note-test om {std::optional<std::map<int, double>> containing std::map with 3 elements = {[1] = 2, [3] = 4, [5] = 6}} } }
  optional<std::string> os{ "stringy" };
// { dg-final { note-test os {std::optional<std::string> = {[contained value] = "stringy"}} } }

  any a;
// { dg-final { note-test a {std::any [no contained value]} } }
  any ab(false);
// { dg-final { note-test ab {std::any containing bool = {[contained value] = false}} } }
  any ai(6);
// { dg-final { note-test ai {std::any containing int = {[contained value] = 6}} } }
  any ap = (void*)nullptr;
// { dg-final { note-test ap {std::any containing void * = {[contained value] = 0x0}} } }
  any as = *os;
// { dg-final { note-test as {std::any containing std::string = {[contained value] = "stringy"}} } }
  any as2("stringiest");
// { dg-final { regexp-test as2 {std::any containing const char \* = {\[contained value\] = 0x[[:xdigit:]]+ "stringiest"}} } }
  any am = *om;
// { dg-final { note-test am {std::any containing std::map with 3 elements = {[1] = 2, [3] = 4, [5] = 6}} } }

  struct S { operator int() { throw 42; }};
  variant<float, int, string_view> v0;
// { dg-final { note-test v0 {std::variant<float, int, std::string_view> [index 0] = {0}} } }
  variant<float, int, string_view> v1{ 0.5f };
// { dg-final { note-test v1 {std::variant<float, int, std::string_view> [index 0] = {0.5}} } }
  variant<float, int, string_view> v2;
  try {
    v2.emplace<1>(S());
  } catch (int) { }
// { dg-final { note-test v2 {std::variant<float, int, std::string_view> [no contained value]} } }
  variant<float, int, string_view> v3{ 3 };
// { dg-final { note-test v3 {std::variant<float, int, std::string_view> [index 1] = {3}} } }
  variant<float, int, string_view> v4{ str };
// { dg-final { note-test v4 {std::variant<float, int, std::string_view> [index 2] = {"string"}} } }
  variant<string_view&> vref{str};
// { dg-final { note-test vref {std::variant<std::basic_string_view<char, std::char_traits<char> > &> [index 0] = {"string"}} } }

  map<int, string_view> m{ {1, "one"} };
  map<int, string_view>::node_type n0;
// { dg-final { note-test n0 {empty node handle for map}}}
  map<int, string_view>::node_type n1 = m.extract(1);
// { dg-final { note-test n1 {node handle for map with element = {{first = 1, second = "two"}}}}}

  unordered_set<int> s{ 3, 4 };
  unordered_set<int>::node_type n2;
// { dg-final { note-test n2 {empty node handle for unordered set}}}
  unordered_set<int>::node_type n3 = s.extract(3);
// { dg-final { note-test n1 {node handle for unordered set with element = {3}}}}

  std::cout << "\n";
  return 0;			// Mark SPOT
}

// { dg-final { gdb-test SPOT } }
