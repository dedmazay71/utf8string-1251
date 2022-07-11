#ifndef STDC_H
#define STDC_H

#if __has_include(<__msvc_all_public_headers.hpp>)
#include <__msvc_all_public_headers.hpp>

#else
// C++ includes used for precompiling -*- C++ -*-
// C
#ifndef _GLIBCXX_NO_ASSERT
#include <cassert>
#endif
 
#if __cplusplus >= 201103L
#include <cfenv>
#include <cinttypes>
#include <cstdint>
#include <cwchar>
#include <cwctype>
#endif
 
// C++
#include <algorithm>
#include <bitset>
#include <complex>
#include <deque>
#include <exception>
#include <fstream>
#include <functional>
#include <iomanip>
#include <ios>
#include <iosfwd>
#include <iostream>
#include <istream>
#include <iterator>
#include <limits>
#include <list>
#include <locale>
#include <map>
#include <memory>
#include <new>
#include <numeric>
#include <ostream>
#include <queue>
#include <set>
#include <sstream>
#include <stack>
#include <stdexcept>
#include <streambuf>
#include <string>
#include <typeinfo>
#include <utility>
#include <valarray>
#include <vector>
 
#if __cplusplus >= 201103L
//Zc:__cplusplus                  /std:c++14 (по умолчанию)       201402L
//Zc:__cplusplus                  /std:c++14 (по умолчанию)       201402L
//Zc:__cplusplus                  /std:c++17                      201703L
//Zc:__cplusplus                  /std:c++latest                  201704L
//Zc:__cplusplus- (отключено)     Любое значение                  199711L
//Не указано                      Любое значение                  199711L
#include <array>
#include <atomic>
//#include <chrono>
#include <condition_variable>
#include <forward_list>
#include <future>
#include <initializer_list>
#include <mutex>
#include <random>
#include <ratio>
#include <regex>
#include <string_view>
#include <scoped_allocator>
#include <system_error>
#include <thread>
#include <tuple>
#include <typeindex>
#include <type_traits>
#include <unordered_map>
#include <unordered_set>
#include <filesystem>
#endif
#if __cplusplus >= 201703L
#include <execution>
#endif

#ifdef __cpp_lib_concepts
#include <concepts>
#endif // __cpp_lib_concepts

#if __has_include(<span>)
#  include <span>
#  define has_span
#endif

#endif    // #if __has_include(<__msvc_all_public_headers.hpp>)

//-------------------------------------------------
#if defined(_CONSTEXPR20)
#define constexpr20 _CONSTEXPR20
#elif _HAS_CXX20
#define constexpr20 constexpr
#else
#define constexpr20 inline
#endif // ^^^ inline (not constexpr) in C++17 and earlier ^^^
//--------------------------------
/*#if defined(_CONSTEXPR20_CONTAINER)
#define constexpr20_container _CONSTEXPR20_CONTAINER
#elif defined(__cpp_lib_constexpr_dynamic_alloc) && !defined(__clang__) // TRANSITION, LLVM-48606
#define constexpr20_container constexpr
#else
#define constexpr20_container inline
#endif*/
//-------------------------------------------------

#endif //#define STDC_H
