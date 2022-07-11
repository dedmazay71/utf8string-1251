//                                        Кодировка WIN  !!!!!!!!!!!!!
//                                        Кодировка WIN  !!!!!!!!!!!!!
//                                        Кодировка WIN  !!!!!!!!!!!!!
#pragma once

#include "ansi_tabs.hpp"

#ifndef scopy_xxx
#define scopy_xxx
template<class T> constexpr
void scopy(T* const dest, const T* const src, size_t size) {
    std::char_traits<T>::copy(dest, src, size);
}
#endif
////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//---------------------------------------------------------------
//namespace fs = std::filesystem;
    using std::cout, std::endl, std::wcout;
    using namespace std::literals;
//---------------------------------------------------------------
#define watch(...)  cout << std::left << std::setw(33) << (#__VA_ARGS__) << "  is  " << (__VA_ARGS__) << endl

#define PERFORM_PT(EXPR, y) std::cout <<  "After " #EXPR << '\n'; EXPR; watch(y);

//#define ASSERT_LEVEL 1
#if !defined(ASSERT_LEVEL) || (defined(ASSERT_LEVEL) && ASSERT_LEVEL)

#define ASSERT(x)                                             \
    do {                                                      \
        if(x);                                                \
        else {                                                \
            std::cout << "\nAssertion failed: " << #x         \
                      << ", file "         <<  __FILE__       \
                      << ", line  "        << __LINE__        \
                      << ", function: "    << __FUNCTION__    \
                      << "\n\n";                              \
            throw std::exception("ASSERT exception");         \
        }                                                     \
    } while(0)

#define ASSERTM(x, msg)                                       \
    do {                                                      \
        if(x);                                                \
        else {                                                \
            std::cout << "\nAssertion failed: " << msg        \
                      << '\n' << #x                           \
                      << ", file "         <<  __FILE__       \
                      << ", line  "        << __LINE__        \
                      << ", function: "    << __FUNCTION__    \
                      << "\n\n";                              \
            throw std::exception("ASSERT exception");         \
        }                                                     \
    } while(0)

#else

#define ASSERT(x)                                             \
    (void) (0 && (x))
#define ASSERTM(x, msg)                                       \
    (void) (0 && (x))

#endif
//////////////////////////////////////////////////////////////////////////////////////////////////////////////
template<class T>
struct Is_char : std::false_type {};
template<>
struct Is_char<char> : std::true_type {};
template<>
struct Is_char<signed char> : std::true_type {};
template<>
struct Is_char<unsigned char> : std::true_type {};
template<>
struct Is_char<wchar_t> : std::true_type {};
template<>
struct Is_char<char16_t> : std::true_type {};
template<>
struct Is_char<char32_t> : std::true_type {};

#if _MSVC_LANG >= 201704
template<>
struct Is_char<std::byte> : std::true_type {};
#endif

#if _MSVC_LANG >= 201705
template<>
struct Is_char<char8_t> : std::true_type {};
#endif

namespace std {
template<class T>
struct is_char : Is_char<std::remove_cvref_t<T>>::type {};
template<class T>
  inline constexpr bool is_char_v = is_char<T>::value;
} //namespace std

namespace std {
template <class _Ty, class... _Types>
inline constexpr bool is_any_of_v = // true if and only if _Ty is in _Types
    disjunction_v<is_same<_Ty, _Types>...>;
} // namespace
/////////////////////////////////////////////////////////////////////////////////////////////////////////////
namespace std {
#ifdef __cpp_lib_concepts

// ALIAS TEMPLATE _With_reference
template <class _Ty>
using with_reference = _Ty&;

// CONCEPT _Can_reference
template <class _Ty>
concept can_reference = requires { typename with_reference<_Ty>; };

// CONCEPT _Dereferenceable
template <class _Ty>
concept dereferenceable = requires(_Ty& __t) {
    { *__t } -> can_reference;
};

// CONCEPT _Has_member_iterator_concept
template <class _Ty>
concept has_type_iterator_concept = requires {
    typename _Ty::iterator_concept;
};
template <class _Ty>
concept has_type_iterator = requires {
    typename _Ty::iterator;
};

// CONCEPT _Has_member_iterator_category
template <class _Ty>
concept has_type_iterator_category = requires {
    typename _Ty::iterator_category;
};

template <class _Ty>
concept has_type_container_type = requires {
    typename _Ty::container_type;
};

// CONCEPT _Has_member_value_type
template <class _Ty>
concept has_type_value_type = requires {
    typename _Ty::value_type;
};

template <class _Ty>
concept has_type_ovalue_type = requires {
    typename _Ty::ovalue_type;
};

// CONCEPT _Has_member_element_type
template <class _Ty>
concept has_type_element_type = requires {
    typename _Ty::element_type;
};

// CONCEPT _Has_member_difference_type
template <class _Ty>
concept has_type_difference_type = requires {
    typename _Ty::difference_type;
};

// CONCEPT _Has_member_pointer
template <class _Ty>
concept has_type_pointer = requires {
    typename _Ty::pointer;
};

// CONCEPT _Has_member_reference
template <class _Ty>
concept has_type_reference = requires {
    typename _Ty::reference;
};

template <class _Ty>
concept can_difference = requires(const _Ty& __a, const _Ty& __b) {
    { __a - __b } -> integral;
};

// ALIAS TEMPLATE _Iter_ref_t
template <class _Iter>
using iter_ref_t = iter_reference_t<_Iter>;

// ALIAS TEMPLATE _Iter_value_t
template <class _Iter>
using iter_value_t = iter_value_t<_Iter>;

// ALIAS TEMPLATE _Iter_diff_t
template <class _Iter>
using iter_diff_t = iter_difference_t<_Iter>;
//---------------------------------------------------- else
#else
// ALIAS TEMPLATE _Iter_ref_t
template <class _Iter>
using iter_ref_t = typename iterator_traits<_Iter>::reference;

// ALIAS TEMPLATE _Iter_value_t
template <class _Iter>
using iter_value_t = typename iterator_traits<_Iter>::value_type;

// ALIAS TEMPLATE _Iter_diff_t
template <class _Iter>
using iter_diff_t = typename iterator_traits<_Iter>::difference_type;

#endif // __cpp_lib_concepts
//-------------------------------------------------------------------
// ALIAS TEMPLATE _Iter_cat_t
template <class _Iter>
using iter_cat_t = typename iterator_traits<_Iter>::iterator_category;

// VARIABLE TEMPLATE _Is_iterator_v
template <class _Ty, class = void>
inline constexpr bool is_iterator_v = false;

template <class _Ty>
inline constexpr bool is_iterator_v<_Ty, void_t<iter_cat_t<_Ty>>> = true;

// TRAIT _Is_iterator
template <class _Ty>
struct is_iterator : bool_constant<is_iterator_v<_Ty>> {};

// VARIABLE TEMPLATE _Is_input_iter_v
template <class _Iter>
inline constexpr bool is_input_iter_v = is_convertible_v<iter_cat_t<_Iter>, input_iterator_tag>;

// VARIABLE TEMPLATE _Is_fwd_iter_v
template <class _Iter>
inline constexpr bool is_fwd_iter_v = is_convertible_v<iter_cat_t<_Iter>, forward_iterator_tag>;

// VARIABLE TEMPLATE _Is_bidi_iter_v
template <class _Iter>
inline constexpr bool is_bidi_iter_v = is_convertible_v<iter_cat_t<_Iter>, bidirectional_iterator_tag>;

// VARIABLE TEMPLATE _Is_random_iter_v
template <class _Iter>
inline constexpr bool is_random_iter_v = is_convertible_v<iter_cat_t<_Iter>, random_access_iterator_tag>;

// ALIAS TEMPLATE _Common_diff_t
template <class... _Iters>
using common_diff_t = common_type_t<_Iter_diff_t<_Iters>...>;

//----------------------------------------------------------------------------
template <class _Ty>
concept has_data_function = requires (_Ty __a) { std::data(__a);  };
/////////////////////////////////////////////////////////////////////////////////////////////////////////////
//                              есть ли в контейнере ф-я член begin
/////////////////////////////////////////////////////////////////////////////////////////////////////////////
template<class T>
concept has_member_begin = std::has_type_iterator<T> && requires(const T& __t) {
    __t.begin();
};
#define has_begin_member_v has_member_begin
/////////////////////////////////////////////////////////////////////////////////////////////////////////////
//                              есть ли в контейнере ф-я член sort : сортировка
/////////////////////////////////////////////////////////////////////////////////////////////////////////////
template<class T>
concept has_member_sort = requires(const T& __t) {
    __t.sort();
};
#define has_sort_member_v has_member_sort
//------------------------------------------------------------------------------
//                  Дополненик к линейке data(...) в xutility
template<class T>  requires std::is_pointer_v<T> && std::is_char_v<std::remove_pointer_t<T>>
[[nodiscard]] constexpr auto data(const T& ptr) noexcept {
    return std::remove_reference_t<decltype(ptr)>(ptr);
}

template<class T>  requires std::is_pointer_v<T> && std::is_char_v<std::remove_pointer_t<T>>
[[nodiscard]] constexpr size_t size(const T& ptr) noexcept {
    return char_traits<std::remove_pointer_t<T>>::length(ptr);
}

} // namespace std
/////////////////////////////////////////////////////////////////////////////////////////////////////////////
inline void set_ust() {
    setlocale(LC_ALL, "ru");
    //синхронизация с буферами C. Поэтому, если в начале кода написать 
    std::ios::sync_with_stdio(false);
    cout.setf(std::ios_base::boolalpha); 
}
/////////////////////////////////////////////////////////////////////////////////////////////////////////////
//////////////////////          Конвертация string to wstring              //////////////////////////////////
/////////////////////////////////////////////////////////////////////////////////////////////////////////////
inline
std::wstring string_to_wstring_l(const std::string_view str, const _locale_t lc) {
  std::wstring result;
  // а если передали lc == nullptr ?
  if(str.empty() || lc == nullptr) return result;
  size_t n(0);
  if(_mbstowcs_s_l(&n, NULL, 0, (const char*)str.data(), 0, lc) == 0 && n > 0) {
    result.resize(n);
    if(_mbstowcs_s_l(&n, &result.at(0), n, (const char*)str.data(), n, lc) == 0)
          result.resize(n - 1); // truncate terminating NULL
    else  result.clear();
  }
  return result;
}
//==============================================================================================================
// для кодовой страница cp1251
std::wstring string_to_wstring_ansi_o(const std::string_view str) {
  std::wstring result;
  if(str.empty()) return result;
  static const _locale_t lc = _create_locale(LC_CTYPE, "ru_RU");  //  1251
  size_t n(0);
  if(_mbstowcs_s_l(&n, NULL, 0, (const char*)str.data(), 0, lc) == 0 && n > 0) {
    result.resize(n);
    if(_mbstowcs_s_l(&n, &result.at(0), n, (const char*)str.data(), n, lc) == 0)
         result.resize(n - 1); // truncate terminating NULL
    else  result.clear();
  } 
  return result;
}
//------------------------------------------------------------------
// эта немного шустрее, чем предыдущая, хотя предыдущая тоже неплоха 
[[nodiscard]] inline ///constexpr
std::wstring string_to_wstring_ansi_d(const std::string_view str) {
  std::wstring dst(str.size(), '\0');
  for(size_t j=0; j<str.size(); ++j)  {
    uint32_t wc;
    cp1251_mbtowc((uint32_t*)(&wc), (const unsigned char)(str[j]));
    dst[j] = static_cast<wchar_t>(wc);
  }
  return dst;  
}
//============================================================================================================
#if defined(ansi_tabs_hpp)
#  define string_to_wstring_ansi(x) string_to_wstring_ansi_tab_wchar(x)
#else  
#  define string_to_wstring_ansi(x) string_to_wstring_ansi_d(x)
#endif
//==============================================================================================================
inline 
std::wstring string_to_wstring(const std::string_view str, [[maybe_unused]] const _locale_t lc = nullptr) {
    return lc? string_to_wstring_l(str, lc) : string_to_wstring_ansi(str);
}
//==============================================================================================================
/// медленная    оставил для предыдущих-последующих    //loc_cur для ДОС кодировки пр-мы
template<class Tstring=std::wstring> 
[[nodiscard]] inline
Tstring string_to_wstring(std::basic_string_view<char> str, const std::locale& loc) {// = loc_glob) {
   Tstring wbuf(Tstring{}, str.size());
   std::use_facet<std::ctype<wchar_t>>(loc).widen(str.data(), str.data()+str.size(), (wchar_t*)wbuf.data());
   return wbuf;
} 
/////////////////////////////////////////////////////////////////////////////////////////////////////////////
//////////////////////          Конвертация wstring to string              //////////////////////////////////
/////////////////////////////////////////////////////////////////////////////////////////////////////////////
[[nodiscard]] inline
std::string wstring_to_string_l(const std::wstring_view str, const _locale_t lc) {
  std::string result;
  if(!str.empty()) {
    size_t n(0);
    if(_wcstombs_s_l(&n, NULL, 0, (const wchar_t*)str.data(), 0, lc) == 0 && n > 0) {
      result.resize(n);
      if(_wcstombs_s_l(&n, &result.at(0), n, (const wchar_t*)str.data(), n, lc) == 0)
          result.resize(n - 1); // truncate terminating NULL
      else  result.clear();
    }
  }
  return result;
}
//=============================================================================================================
// для кодовой страницы cp1251
std::string wstring_to_string_ansi_o(const std::wstring_view str) {
  std::string result;
  if(str.empty()) return result;
  static const _locale_t lc = _create_locale(LC_CTYPE, "ru_RU");  //  1251
  size_t n(0);
  if(_wcstombs_s_l(&n, NULL, 0, (const wchar_t*)str.data(), 0, lc) == 0 && n > 0) {
    result.resize(n);
    if(_wcstombs_s_l(&n, &result.at(0), n, (const wchar_t*)str.data(), n, lc) == 0)
          result.resize(n - 1); // truncate terminating NULL
    else  result.clear();
  }
  return result;
}
// эта немного шустрее, чем предыдущая, хотя предыдущая тоже неплоха 
[[nodiscard]] inline ///constexpr
std::string wstring_to_string_ansi_d(const std::wstring_view src) {
  std::string dst(src.size(), '\0');
  for(char* p = (char*) dst.data(); const wchar_t x: src) { 
    *p++ = (char)cp1251_wctomb(x); 
  }
  return dst;
}

#  define wstring_to_string_ansi wstring_to_string_ansi_d
//=============================================================================================================
inline 
std::string wstring_to_string(const std::wstring_view str, [[maybe_unused]] const _locale_t lc = nullptr) {
   return lc? wstring_to_string_l(str, lc) : wstring_to_string_ansi(str);
}
//////////////////////////////////////////////////////////////////////////////////////////////////////////////
