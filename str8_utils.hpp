//
//                      str8_utils.hpp   --  Alexander Prilepsky 2021
//
// Этот заголовочный файл содержит функции, оперирующие с UTF-8 последовательностями символов
//
// ПРОГРАММНОЕ ОБЕСПЕЧЕНИЕ ПРЕДОСТАВЛЯЕТСЯ "КАК ЕСТЬ", БЕЗ КАКИХ-ЛИБО ГАРАНТИЙ,
// ЯВНЫЕ ИЛИ ПОДРАЗУМЕВАЕМЫЕ, ВКЛЮЧАЯ, НО НЕ ОГРАНИЧИВАЯСЬ ГАРАНТИЯМИ
// ТОВАРНАЯ ПРИГОДНОСТЬ, ПРИГОДНОСТЬ ДЛЯ ОПРЕДЕЛЕННОЙ ЦЕЛИ И ОТСУТСТВИЕ НАРУШЕНИЙ.
// НИ В КОЕМ СЛУЧАЕ АВТОР НЕ НЕСЕТ ОТВЕТСТВЕННОСТИ ЗА КАКИЕ-ЛИБО ПРЕТЕНЗИИ, УБЫТКИ ИЛИ
// ИНАЯ ОТВЕТСТВЕННОСТЬ, БУДЬ ТО В СИЛУ ДОГОВОРА, ДЕЛИКТА ИЛИ ИНЫМ ОБРАЗОМ,
// ВЫТЕКАЮЩИЕ ИЗ, ИЛИ В СВЯЗИ С ПРОГРАММНЫМ ОБЕСПЕЧЕНИЕМ ИЛИ ИСПОЛЬЗОВАНИЕМ ИЛИ
// ДРУГИЕ СДЕЛКИ С ПРОГРАММНЫМ ОБЕСПЕЧЕНИЕМ.
//
//                      c++ Visual Studio 2022 version 17.2
//
///////////////////////////////////////////////////////////////////////////////////////////////////////////////
#pragma once
#include <mbstring.h>
#include "my_adds.hpp"
#include "ansi_tabs.hpp"
///////////////////////////////////////////////////////////////////////////////////////////////////////////////
//                                   std::u8string -> lower std::u8string
//////////////////////////////////////////////////////////////////////////////////////////////////////////////
[[nodiscard]] inline
std::u8string utf8_tolower_ansi(const std::u8string_view src) {   // u8size(src);        // in symbols
    std::u8string     dst(src);
    for(const char8_t* pdst = dst.data(); pdst < dst.data()+src.size(); )  
        pdst += cp1251_mb8_tolower_mb8((char8_t*)pdst);     // inplace
    return dst;
}
///////////////////////////////////////////////////////////////////////////////////////////////////////////////
//                                   std::u8string -> lower STD::STRING
//////////////////////////////////////////////////////////////////////////////////////////////////////////////
[[nodiscard]] inline
std::string utf8_tolower_string_ansi(const std::u8string_view src) {  // for ANSI
    std::string          dst(u8size(src), '\0');

    for(const unsigned char* p = (unsigned char*)src.data(); char& x: dst)  
        p += cp1251_mb8tolowermb((unsigned char&)x, p);
    return dst;
}
//////////////////////////////////////////////////////////////////////////////////////////////////////////////
//                          compare two utf8 symbols
[[nodiscard]] constexpr
int compare_two_utf8_symbols(const auto* const p1, const auto* const p2) noexcept {
    const auto left_size  = len8(*p1);
    const auto right_size = len8(*p2);
    const int ans = std::char_traits<char8_t>::compare(p1, p2, (std::min) (left_size, right_size));
    if(ans != 0) return ans;

    if(left_size < right_size) return -1;
    if(left_size > right_size) return 1;

    return 0;
}
//////////////////////////////////////////////////////////////////////////////////////////////////////////////
//                         Сравнение UTF8 ANSI символов без учета регистра
//////////////////////////////////////////////////////////////////////////////////////////////////////////////
struct iequal_to_ansi_8_chars {
//                              сравнивает chars ( char8_t* -> char - compare
    [[nodiscard]] constexpr 
    bool operator() (const char8_t* const p1, const char8_t* const p2) const noexcept {
        return cp1251_mb8tolowermb((unsigned char*)p1) == cp1251_mb8tolowermb((unsigned char*)p2);
    }

    [[nodiscard]] constexpr 
    bool operator() (const std::u8string_view s1, const std::u8string_view s2) const noexcept {
        return cp1251_mb8tolowermb((unsigned char*)(s1.data())) == cp1251_mb8tolowermb((unsigned char*)(s2.data()));
    }
}; 
//------------------------
struct iless_ansi_8_chars {
//                              сравнивает chars ( char8_t* -> char - compare
    [[nodiscard]] constexpr 
    bool operator() (const char8_t* const p1, const char8_t* const p2) const noexcept {
        return cp1251_mb8tolowermb((unsigned char*)p1) < cp1251_mb8tolowermb((unsigned char*)p2);
    }

    [[nodiscard]] constexpr 
    bool operator() (const std::u8string_view s1, const std::u8string_view s2) const noexcept {
        return cp1251_mb8tolowermb((unsigned char*)(s1.data())) < cp1251_mb8tolowermb((unsigned char*)(s2.data()));
    }
};
//------------------------
struct iless_equal_ansi_8_chars {         //  <=
//                              сравнивает chars ( char8_t* -> char - compare
    [[nodiscard]] constexpr 
    auto operator() (const char8_t* const p1, const char8_t* const p2) const noexcept {
        return cp1251_mb8tolowermb((unsigned char*)p1) <= cp1251_mb8tolowermb((unsigned char*)p2);
    }

    [[nodiscard]] constexpr 
    bool operator() (const std::u8string_view s1, const std::u8string_view s2) const noexcept {
        return cp1251_mb8tolowermb((unsigned char*)(s1.data())) <= cp1251_mb8tolowermb((unsigned char*)(s2.data()));
    }
};
#define iless_equal_symb_utf8_ansi iless_equal_ansi_8_chars
//////////////////////////////////////////////////////////////////////////////////////////////////////////////
//                          Сравнение UTF-8 (ANSI)  СТРОК  без учета регистра
//////////////////////////////////////////////////////////////////////////////////////////////////////////////
[[nodiscard]] constexpr 
int32_t icompare_ansi_strings_impl_8(const std::u8string_view s1, const std::u8string_view s2, 
                                    const size_t n1, const size_t n2) {
    const char8_t* p1 = s1.data();
    const char8_t* p2 = s2.data();
    for(size_t i = 0; i < n1; ++i) {
        if(!iequal_to_ansi_8_chars()(p1, p2)) return iless_ansi_8_chars()(p1, p2) ? -1 : 1;
        p1 += utf8len(*p1); p2 += utf8len(*p2);
    }
    return n1 < n2 ? -1 : 0;    // <=
}
//---------------------
[[nodiscard]] constexpr 
int32_t icompare_ansi_strings_8(const std::u8string_view s1, const std::u8string_view s2) {
    const auto n1 = u8size(s1);    // in symbols
    const auto n2 = u8size(s2);
    return n1 <= n2? icompare_ansi_strings_impl_8(s1, s2, n1, n2) : -icompare_ansi_strings_impl_8(s2, s1, n2, n1);
}
//---------------------
struct iless_ansi8 {                          // предикат
    [[nodiscard]] constexpr 
    bool operator() (const std::u8string_view s1, const std::u8string_view s2) const noexcept {
        return icompare_ansi_strings_8(s1, s2) < 0;
    }
};
struct iless_equal_to_ansi8 {                          // предикат
    [[nodiscard]] constexpr 
    bool operator() (const std::u8string_view s1, const std::u8string_view s2) const noexcept {
        return !(icompare_ansi_strings_8(s1, s2) > 0);
    }
};
struct iequal_to_ansi8 {                         // предикат
    [[nodiscard]] constexpr 
    bool operator() (const std::u8string_view s1, const std::u8string_view s2) const noexcept {
        return icompare_ansi_strings_8(s1, s2) == 0;
    }
};
//////////////////////////////////////  SORT    ////////////////////////////////////////////////////////////
//                            Сортировка для  UTF8 ANSI строк
#define isort_ansi8(x)          sort<iless_ansi8>(x)
#define isort_parallel_ansi8(x) sort_parallel<iless_ansi8>(x)
///////////////////////////////////////////////////////////////////////////////////////////////////////////////
//                                      UTF-8   =>  wstring                                                 //
//////////////////////////////////////////////////////////////////////////////////////////////////////////////
//                                      with _locale_t
[[nodiscard]] inline 
std::wstring utf8_to_wstring(const std::u8string_view str, _locale_t lc) {
    std::wstring result;
    if(!str.empty()) {
        size_t n(0);
        bool   blc = false;
        if(lc == nullptr) { blc = true; lc = _create_locale(LC_ALL, "ru_RU.UTF-8"); } //   for cp1251
        if(_mbstowcs_s_l(&n, NULL, 0, (const char*)str.data(), 0, lc) == 0 && n > 0) {
            result.resize(n);
            if(_mbstowcs_s_l(&n, &result.at(0), n, (const char*)str.data(), n, lc) == 0)
                result.resize(n - 1); // truncate terminating NULL
            else  result.clear();
        }
        if(blc) _free_locale(lc);
    }
    return result;
}
//------------------------------------------------------------------------------------------------------------
//                                      without _locale_t
[[nodiscard]] inline 
std::wstring utf8_to_wstring_ansi_o(const std::u8string_view str) {
    std::wstring result;
    if(!str.empty()) {
        static const _locale_t lc { _create_locale(LC_ALL, "ru_RU.UTF-8") };  //   for cp1251
        size_t n(0);

        if(_mbstowcs_s_l(&n, NULL, 0, (const char*)str.data(), 0, lc) == 0 && n > 0) {
            result.resize(n);
            if(_mbstowcs_s_l(&n, &result.at(0), n, (const char*)str.data(), n, lc) == 0)
                result.resize(n - 1); // truncate terminating NULL
            else  result.clear();
        }
    }
    return result;
} 
//-------------------------------------------------------------------------------------------------------------
// эта пошустрее предыдущей 
[[nodiscard]] inline
std::wstring utf8_to_wstring(const std::u8string_view srcw) {
    if(srcw.empty()) return std::wstring{};

    size_t            count(0);
    const char* const src = (const char*)srcw.data();
    const char* const e   = src + srcw.size();
    const char*       p   = src;
  
    while(p < e) {
        if(!(*p & 0x80)) p++;
        else {
            int len; const unsigned ucs = utf8_decode(p, e, &len);
            p += len;
            if(ucs >= 0x10000) ++count;
        }
        ++count;
    }
  
    const size_t   dstlen(count);
    std::wstring   dstw(dstlen, L'\0');
    wchar_t* const dst = (wchar_t*)dstw.data(); 
    count = 0;   p = src;
  
    for(;;) {
        if(p >= e) break;
        if(!(*p & 0x80)) { // ascii
        dst[count] = static_cast<unsigned short>(*p++);
        } else {
            int len; const unsigned ucs = utf8_decode(p, e, &len);
            p += len;
            if(ucs < 0x10000) {
                dst[count] = static_cast<unsigned short>(ucs);
            } else {
                // сделать суррогатную пару
                if(count+2 >= dstlen) break;
                dst[count] = (((ucs-0x10000u)>>10)&0x3ff) | 0xd800;
                dst[++count] = (ucs&0x3ff) | 0xdc00;
            }
        }
        if(++count == dstlen) break;
    }
    return dstw;
}
////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//                                      wstring   =>  UTF-8                                                   //
////////////////////////////////////////////////////////////////////////////////////////////////////////////////
[[nodiscard]] inline 
std::u8string wstring_to_utf8_o(const std::wstring_view str) {
    std::u8string result;
    if(!str.empty()) {
        static const _locale_t lc { _create_locale(LC_ALL, "ru_RU.UTF-8")};
        size_t    n(0);
        //std::cout << "wstring_to_utf8\n";
        if(_wcstombs_s_l(&n, NULL, 0, (const wchar_t*)str.data(), 0, lc) == 0 && n > 0) {
            result.resize(n);
            if(_wcstombs_s_l(&n, (char*)&result.at(0), n, (const wchar_t*)str.data(), n, lc) == 0)
                result.resize(n - 1); // truncate terminating NULL
            else  result.clear();
        }
    }
    return result;
}
//-------------------------------------------------------------------------------------------------------------
//                                  эта пошустрее предыдущей 
[[nodiscard]] inline 
std::u8string wstring_to_utf8_ansi(const std::wstring_view srcw) {       // for ANSI
    if(srcw.empty()) return std::u8string{};//u8""s;
  
    unsigned              i(0), count(0);
    const size_t          srclen(srcw.size());
    const wchar_t* const  src = srcw.data();
    // подсчитаем  длину utf8 строки
    while(i < srclen) {
        const unsigned ucs = src[i++];
        if(ucs < 0x80U) {
            count++;
        } else if(ucs < 0x800U) { /* 2 bytes */
            count += 2;
        } else if(ucs >= 0x10000 && ucs <= 0x10ffff) {
            count += 4;
        } else {
            count += 3;
        }
    }
  
    const unsigned dstlen(count+1); // +1 for eof
    std::u8string dstw(count, u8'\0');
    char* const dst = (char*)dstw.data();

    i=count=0;
    for(;;) {
        if(i >= srclen) break;
        unsigned ucs = src[i++];
        if(ucs < 0x80U) {
        dst[count++] = static_cast<char>(ucs);
        if(count >= dstlen) { break;}
        } else if(ucs < 0x800U) { /* 2 bytes */
            if(count+2 >= dstlen) break;
            dst[count++] = static_cast<char>(0xc0 | (ucs >> 6));
            dst[count++] = 0x80 | (ucs & 0x3F);
        } else if(ucs >= 0x10000) {
            if(ucs > 0x10ffff) { ucs = 0xfffd; goto J1; }
            if(count+4 >= dstlen) break;
            dst[count++] = static_cast<char>(0xf0 | (ucs >> 18));
            dst[count++] = 0x80 | ((ucs >> 12) & 0x3F);
            dst[count++] = 0x80 | ((ucs >> 6) & 0x3F);
            dst[count++] = 0x80 | (ucs & 0x3F);
        } else {
J1:
            // все остальные имеют 3 байта
            if(count+3 >= dstlen) break;
            dst[count++] = static_cast<char>(0xe0 | (ucs >> 12));
            dst[count++] = 0x80 | ((ucs >> 6) & 0x3F);
            dst[count++] = 0x80 | (ucs & 0x3F);
        }
    }
    return dstw;
}
//--------------------------------------------------------------------------------------------------
//                                      wchar_t   =>  UTF-8 string
//--------------------------------------------------------------------------------------------------
[[nodiscard]] inline
std::u8string wchar_to_utf8_ansi(const wchar_t c) {
    unsigned i(0), count(0);
    {
        const unsigned ucs = c;
        if(ucs < 0x80U)     ++count;
        else 
        if(ucs < 0x800U)    count += 2;   // 2 bytes
        else 
        if(ucs >= 0x10000 && ucs <= 0x10ffff) count += 4;
        else                count += 3;
            //count2 = 1 + (ucs[i] >= 0x80) + (ucs[i] >= 0x800) + (ucs[i] >= 0x10000);
    }
  
    const unsigned dstlen(count+1); // +1 for eof
    std::u8string dstw(count, u8'\0');
    char* const dst = (char*)dstw.data();

    i=count=0;
    for(;;) {
        if(i >= 1) break;
        unsigned ucs = c; ++i;
        if(ucs < 0x80U) {
            dst[count++] = static_cast<char>(ucs);
            if(count >= dstlen)  break;
        } else 
        if(ucs < 0x800U) { /* 2 bytes */
            if(count+2 >= dstlen) break;
            dst[count++] = static_cast<char>(0xc0 | (ucs >> 6));
            dst[count++] = 0x80 | (ucs & 0x3F);
        } else 
        if(ucs >= 0x10000) {
            if(ucs > 0x10ffff) { ucs = 0xfffd; goto J1; }
            if(count+4 >= dstlen) break;
            dst[count++] = static_cast<char>(0xf0 | (ucs >> 18));
            dst[count++] = 0x80 | ((ucs >> 12) & 0x3F);
            dst[count++] = 0x80 | ((ucs >> 6) & 0x3F);
            dst[count++] = 0x80 | (ucs & 0x3F);
        } else {
J1:
            // все остальные имеют 3 байта
            if(count+3 >= dstlen) break;
            dst[count++] = static_cast<char>(0xe0 | (ucs >> 12));
            dst[count++] = 0x80 | ((ucs >> 6) & 0x3F);
            dst[count++] = 0x80 | (ucs & 0x3F);
        }
    }
    return dstw;
}
//-------------------------------------------------------------------------------------------------------------
[[nodiscard]] inline 
std::u8string wstring_to_utf8(const std::wstring_view str, _locale_t lc) {
    std::u8string result;
    if(!str.empty()) {
        size_t n(0);
        bool blc = false;
        if(lc == nullptr) { blc = true; lc = _create_locale(LC_ALL, "ru_RU.UTF-8"); }
        if(_wcstombs_s_l(&n, NULL, 0, (const wchar_t*)str.data(), 0, lc) == 0 && n > 0) {
            result.resize(n);
            if(_wcstombs_s_l(&n, (char*)&result.at(0), n, (const wchar_t*)str.data(), n, lc) == 0)
                result.resize(n - 1); // truncate terminating NULL
            else  result.clear();
        }
        if(blc) _free_locale(lc);
    }
    return result;
}
/////////////////////////////////////////////////////////////////////////////////////////////////////////////
[[nodiscard]] inline
std::string utf8_to_string_ansi_o(const std::u8string_view str) {  // current locale ANSI
    return  wstring_to_string_ansi(utf8_to_wstring(str));
}
//-------------------------------------------------------------------------------------------------------------
// эта в 2 раза шустрее предыдущей
[[nodiscard]] inline // constexpr
std::string utf8_to_string_ansi(const std::u8string_view src) {  // for ANSI
    //const unsigned char* p    = (const unsigned char*)src.data();
    const size_t   size =  u8size(src);
    std::string    dst(size, '\0');
  
    std::span<unsigned char> r = {(unsigned char*) dst.data(), size};
    for(const unsigned char* p    = (const unsigned char*)src.data(); unsigned char& x: r) {
        p += cp1251_mb8tomb(x, p);
    }
    return dst;
}
//----------------------------------------------------------------------------------------
template<class T1, class T2>    // не для всей строки   n1 = позиция, n2 = кол-во символов
                                //                    без проверок диапазонов !!!!!
[[nodiscard]] inline
std::string utf8_to_string_ansi(const std::u8string_view src, const T1 _pos, const T2 n2) {  // for ANSI
    const unsigned char*     p = (const unsigned char*)src.data();
    std::string              dst(static_cast<size_t>(n2), '\0');

    for(size_t n = static_cast<size_t>(_pos); n > 0; --n) p += utf8len(*p);   // p - на позицию _pos
    for(std::span<unsigned char> r = {(unsigned char*) dst.data(), static_cast<size_t>(n2)};
                                                                            unsigned char& x: r) 
        p += cp1251_mb8tomb(x, p);
    return dst;
}
//----------------------------------------------------------------------------------------
                            // pos = in SYMBOLS
template<class T>
[[nodiscard]] constexpr 
char utf8_to_char_ansi(const std::u8string_view src, T pos) {  // for ANSI
    const unsigned char* p    = (const unsigned char*)src.data();

    for(; pos>0; --pos) p += utf8len(*p);
    unsigned char x;
    cp1251_mb8tomb(x, p);
    return static_cast<char>(x);
}
//----------------------------------------------------------------------------------------
                            // Символ в позиции 0
[[nodiscard]] constexpr 
char utf8_to_char_ansi(const char8_t* const p) {  // for ANSI
    unsigned char x;
    cp1251_mb8tomb(x, (unsigned char*) p);
    return static_cast<char>(x);
}
//////////////////////////////////////////////////////////////////////////////////////////////////////////////
[[nodiscard]] inline
std::string utf8_to_string(const std::u8string_view str, const _locale_t lc) {
    return  wstring_to_string(utf8_to_wstring(str, lc), lc);
}
[[nodiscard]] inline
std::string _from_utf8(const std::u8string_view str, [[maybe_unused]] const _locale_t lc = NULL) {
    return  lc? utf8_to_string(str, lc) : utf8_to_string_ansi(str);
}
////////////////////////////////////////////////////////////////////////////////////////////////////////////////
[[nodiscard]] inline
std::u8string string_to_utf8_ansi_d(const std::string_view src) {
    std::u8string dstx(u8size(src), u8'\0');
    for(char8_t* p = dstx.data(); const unsigned char& c: src) {
        copy_plus(p, table_utf8+table_indexes[c].pos, (int32_t)(table_indexes[c].length)); // copy_plus перемещает p
    }

    return dstx;
}

#if defined(ansi_tabs_hpp)
#  define string_to_utf8_ansi(x) string_to_utf8_ansi_tab_char(x)
#else  
#  define string_to_utf8_ansi(x) string_to_utf8_ansi_d(x)
#endif
//-------------------------------------------------------------------------------------------------------------
[[nodiscard]] inline
std::u8string string_to_utf8(const std::string_view str, const _locale_t lc) {
      return wstring_to_utf8(string_to_wstring(str, lc), lc);
}
//                                          for string
[[nodiscard]] inline //constexpr 
std::u8string _to_utf8 (const std::string_view str, [[maybe_unused]] const _locale_t lc = NULL) {
    return lc? string_to_utf8(str, lc) : string_to_utf8_ansi(str);
}
[[nodiscard]] inline //constexpr 
std::u8string _to_utf8 (const unsigned char* const ptr, [[maybe_unused]] const _locale_t lc = NULL) {
    return _to_utf8((const char* const)ptr, lc);
}
//                                          for wstring
[[nodiscard]] inline //constexpr 
std::u8string _to_utf8 (const std::wstring_view str, [[maybe_unused]] const _locale_t lc = NULL) {
    return lc? wstring_to_utf8(str, lc) : wstring_to_utf8_ansi(str);
}
//////////////////////////////////////////////////////////////////////////////////////////////////////////////
//                    переданный ptr перемещается на следующий CoPo(utf8 sequence, CodePointer)
template <class byte_pointer, 
            std::enable_if_t<std::is_pointer_v<byte_pointer> || std::is_iterator_v<byte_pointer>, int> = 0>
constexpr 
void u8_next(byte_pointer& it) noexcept {
    if constexpr(std::is_class_v<byte_pointer>) {
        using Noptr = std::remove_const_t<typename byte_pointer::value_type>;
        using Nptr  = std::remove_const_t<typename byte_pointer::pointer>;
        static_assert(std::is_same_v<Noptr, char8_t> || 
                      std::is_same_v<Nptr, char8_t*> ||
                      //std::is_same_v<Nptr, const char8_t*> ||
                      std::is_same_v<Noptr, uint8_t>, "ONLY FOR char8_t*");
        auto& it_ptr = it._Ptr;                   // не применимо для iter8
        it_ptr += utf8len(*it_ptr);
    }
    else {
        using Noptr = std::remove_const_t<std::remove_pointer_t<byte_pointer>>;
        static_assert(std::is_same_v<Noptr, char8_t> || std::is_same_v<Noptr, uint8_t>, "ONLY FOR char8_t*");
        it += utf8len(*it);
    }
}
//------------------------------------------------------------------------------
//                    возвращается указатель на следующий CoPo без перемещения переданного)
template <class byte_pointer, 
            std::enable_if_t<std::is_pointer_v<byte_pointer> || std::is_iterator_v<byte_pointer>, int> = 0>
[[nodiscard]] constexpr 
auto u8_peek_next(const byte_pointer it) noexcept {
    if constexpr(std::is_class_v<byte_pointer>) {
        using Noptr = std::remove_const_t<typename byte_pointer::value_type>;
        using Nptr  = std::remove_const_t<typename byte_pointer::pointer>;
        static_assert(std::is_same_v<Noptr, char8_t> || 
                      std::is_same_v<Nptr, char8_t*> ||
                      std::is_same_v<Nptr, const char8_t*> ||
                      std::is_same_v<Noptr, uint8_t>, "ONLY FOR char8_t*");
        const auto it_ptr = it._Ptr;           // не применимо для iter8
        return it_ptr + utf8len(*it_ptr);
    }
    else {
        using Noptr = std::remove_const_t<std::remove_pointer_t<byte_pointer>>;
        static_assert(std::is_same_v<Noptr, char8_t> || std::is_same_v<Noptr, uint8_t>, "ONLY FOR char8_t*");
        return it + utf8len(*it);
    }
}
///////////////////////////////////////////////////////////////////////////////////////////////////////////////
//                          дистанция В СИМВОЛАХ между двумя code pointers  
//                                    ==========
template <class byte_pointer>
requires std::is_pointer_v<byte_pointer> || std::is_iterator_v<byte_pointer>
[[nodiscard]] constexpr 
ptrdiff_t u8_distance(byte_pointer first, byte_pointer last) noexcept {
    ptrdiff_t dist = 0;
    if constexpr(std::is_class_v<byte_pointer>) {           // iterator           // iter8
        using Noptr = std::remove_const_t<typename byte_pointer::value_type>;     
        using Nptr  = std::remove_const_t<typename byte_pointer::pointer>;        
        static_assert(std::is_same_v<Noptr, char8_t> || 
                      std::is_same_v<Nptr, char8_t*> ||
                      std::is_same_v<Nptr, const char8_t*> ||
                      std::is_same_v<Noptr, uint8_t>, "ONLY FOR value_type = char8_t");
        auto f_ptr = first.operator->(), l_ptr = last.operator->(); 
        if(f_ptr <= l_ptr)
            dist = u8size(f_ptr, l_ptr);
        else
            dist = -(ptrdiff_t)u8size(l_ptr, f_ptr);
    }
    else {
        using Noptr = std::remove_const_t<std::remove_pointer_t<byte_pointer>>;
        static_assert(std::is_same_v<Noptr, char8_t> || std::is_same_v<Noptr, uint8_t>, "ONLY FOR char8_t*");

        if(first <= last)
            dist = u8size(first, last);
        else
            dist = -(ptrdiff_t)u8size(last , first);
    }
    
    return dist;
}
//-------------------------------------------------------------------------------------------------------------
//               Преобразование позиции, выраженной в СИМВОЛАХ, в позицию в БАЙТАХ
//               дистанция в БАЙТАХ между code pointer и позицией pos(В СИМВОЛАХ)  
//                           ======                                   ========== 
template <class byte_pointer, 
            std::enable_if_t<std::is_pointer_v<byte_pointer> || std::is_iterator_v<byte_pointer>, int> = 0>
[[nodiscard]] constexpr 
size_t u8_distance(byte_pointer cp, const ptrdiff_t pos) noexcept {
    if constexpr(std::is_class_v<byte_pointer>) {                // iterator
        using Noptr = std::remove_const_t<typename byte_pointer::value_type>;
        using Nptr  = std::remove_const_t<typename byte_pointer::pointer>;
        static_assert(std::is_same_v<Noptr, char8_t> || 
                      std::is_same_v<Nptr, char8_t*> ||
                      std::is_same_v<Nptr, const char8_t*> ||
                      std::is_same_v<Noptr, uint8_t>, "ONLY FOR char8_t*");
        const auto   cp_ptr = cp.operator->();//_Ptr;//ptr();
        auto p              = cp.operator->();//_Ptr;//ptr();
        ptrdiff_t size      = 0;

        if(pos >= 0) {
            while(size != pos) size += u8_is_first(*p++);
            while(u8_is_trail_b(*p)) ++p;
            return (size_t)(p - cp_ptr);
        } else {
            while(u8_is_trail_b(*(--p)));
            for(;;--p) { size += u8_is_first_b(*(p));  if(size == -pos) break; }
            return  (size_t)(cp_ptr - p);
        }
    }
    else {
        using Noptr = std::remove_const_t<std::remove_pointer_t<byte_pointer>>;
        static_assert(std::is_same_v<Noptr, char8_t> || std::is_same_v<Noptr, uint8_t>, "ONLY FOR char8_t*");
        const byte_pointer p = cp;
        ptrdiff_t size          = 0;
        if(pos == 0) return 0;
        if(pos > 0) {
            while(size != pos) size += u8_is_first(*cp++);
            while(u8_is_trail_b(*cp)) ++cp;
            return (size_t)(cp - p);
        } else {
            while(u8_is_trail_b(*(--cp)));
            for(;;--cp) { size += u8_is_first_b(*(cp));  if(size == -pos) break; }
            return  (size_t)(p - cp);
        }
    }
}
///////////////////////////////////////////////////////////////////////////////////////////////////////////////
//                      переданный CODE POINTER перемещается на дистанцию pos(В СИМВОЛАХ)
//                                                                           ==========
template <class byte_pointer,
            std::enable_if_t<std::is_pointer_v<byte_pointer> || std::is_iterator_v<byte_pointer>, int> = 0>
inline
void u8_advance(byte_pointer& it, const ptrdiff_t pos) noexcept {
    if constexpr(std::is_class_v<byte_pointer>) {
        using Noptr = std::remove_const_t<typename byte_pointer::value_type>;  // для utf8string -> Orig<4>
        using Nptr  = std::remove_const_t<typename byte_pointer::pointer>;
        static_assert(std::is_same_v<Noptr, char8_t> || 
                      std::is_same_v<Nptr, char8_t*> ||
                      std::is_same_v<Nptr, const char8_t*> ||
                      std::is_same_v<Noptr, uint8_t>, "ONLY FOR char8_t*");
        if(pos >= 0)
            it += u8_distance(it._Ptr, pos);                  // _Ptr не явл-ся членом iter8. it's good!
        else
            it -= u8_distance(it._Ptr, pos);
    }
    else {
        using Noptr = std::remove_const_t<std::remove_pointer_t<byte_pointer>>;
        static_assert(std::is_same_v<Noptr, char8_t> || std::is_same_v<Noptr, uint8_t>, "ONLY FOR char8_t*");
        if(pos >= 0)
            it += u8_distance(it, pos);
        else
            it -= u8_distance(it, pos);
     }
}
///////////////////////////////////////////////////////////////////////////////////////////////////////////////
//               Return ptr to code pointer на дистанции pos(В СИМВОЛАХ) от переданного CoPo(CodePointer)
//                                                           ==========   
template <class byte_pointer, 
            std::enable_if_t<std::is_pointer_v<byte_pointer> || std::is_iterator_v<byte_pointer>, int> = 0>
[[nodiscard]] constexpr 
auto u8_fwd_n(byte_pointer bp, const ptrdiff_t pos) noexcept {
    u8_advance(bp, pos);
    return bp;
}
///////////////////////////////////////////////////////////////////////////////////////////////////////////////
//                 Преобразование позиции, выраженной в БАЙТАХ, в позицию в СИМВОЛАХ
//                                                      =======             ========
template<class byte_pointer, 
            std::enable_if_t<std::is_pointer_v<byte_pointer>, int> = 0>
[[nodiscard]] constexpr                                         // pos in BYTES
auto convert_pos_in_bytes_to_pos_in_symbols(byte_pointer bp, const size_t pos) noexcept {
    using Noptr = std::remove_const_t<std::remove_pointer_t<byte_pointer>>;
    static_assert(std::is_same_v<Noptr, char8_t> || std::is_same_v<Noptr, uint8_t>, "ONLY FOR char8_t*");
    if(pos == std::string::npos) return pos;
    size_t          pos_s   = 0, size = 0;

    while(pos_s++ < pos) { size += u8_is_first(*bp++); }
    return size;
}
//-------------------------
template<class byte_pointer, 
            std::enable_if_t<std::is_pointer_v<byte_pointer>, int> = 0>
[[nodiscard]] constexpr 
auto convert_pos_in_bytes_to_pos_in_symbols(const byte_pointer first, const byte_pointer last) noexcept {
    return convert_pos_in_bytes_to_pos_in_symbols(first, last-first);
}
#define cnv_pos_byte_sym    convert_pos_in_bytes_to_pos_in_symbols
///////////////////////////////////////////////////////////////////////////////////////////////////////////////
//                                  previous ptr  to code pointer
template <class byte_pointer, 
std::enable_if_t<std::is_pointer_v<byte_pointer> || std::_Is_iterator_v<byte_pointer>, int> = 0>
constexpr 
void u8_prev(byte_pointer& _p) noexcept {
    if constexpr(std::is_class_v<byte_pointer>) {
        using Noptr = std::remove_const_t<typename byte_pointer::value_type>;
        using Nptr  = std::remove_const_t<typename byte_pointer::pointer>;
        static_assert(std::is_same_v<Noptr, char8_t> || 
                      std::is_same_v<Nptr, char8_t*> ||
                      std::is_same_v<Nptr, const char8_t*> ||
                      std::is_same_v<Noptr, uint8_t>, "ONLY FOR char8_t*");
        auto&  p_ptr = _p._Ptr;               // не применимо для iter8
        while(u8_is_trail(*(--p_ptr)));
    }
    else {
        using Noptr = std::remove_const_t<std::remove_pointer_t<byte_pointer>>;
        static_assert(std::is_same_v<Noptr, char8_t> || std::is_same_v<Noptr, uint8_t>, "ONLY FOR char8_t*");
        while(u8_is_trail(*(--_p)));
    }
}
//----------------------------------------------------------------------------------------------------
template <class byte_pointer, 
std::enable_if_t<std::is_pointer_v<byte_pointer> || std::_Is_iterator_v<byte_pointer>, int> = 0>
[[nodiscard]] constexpr 
auto u8_peek_prev(byte_pointer _p) noexcept {
    if constexpr(std::is_class_v<byte_pointer>) {
        using Noptr = std::remove_const_t<typename byte_pointer::value_type>;
        using Nptr  = std::remove_const_t<typename byte_pointer::pointer>;
        static_assert(std::is_same_v<Noptr, char8_t> || 
                      std::is_same_v<Nptr, char8_t*> ||
                      std::is_same_v<Nptr, const char8_t*> ||
                      std::is_same_v<Noptr, uint8_t>, "ONLY FOR char8_t*");
        auto   p_pt  = _p;
        auto&  p_ptr = p_pt._Ptr;                   // не применимо для iter8
        while(u8_is_trail(*(--p_ptr)));
        return p_pt;
    }
    else {
        using Noptr = std::remove_const_t<std::remove_pointer_t<byte_pointer>>;
        static_assert(std::is_same_v<Noptr, char8_t> || std::is_same_v<Noptr, uint8_t>, "ONLY FOR char8_t*");
        auto  p_ptr = _p;
        while(u8_is_trail(*(--p_ptr)));
        return p_ptr;
    }
}
//////////////////////////////////////////////////////////////////////////////////////////////////////////////
//                                  CHAR to u8string
[[nodiscard]] inline
std::u8string char_to_utf8_ansi_tab_char(const char c) noexcept {
    return {(char8_t*)tab_char::tab_utf8[(unsigned char)c].s8, tab_char::tab_utf8[(unsigned char)c].len};
}
//----------------------------------------------------------------------------------------------------
//                                  _count символов c в u8string
[[nodiscard]] inline
std::u8string char_to_utf8_ansi_tab_char(size_t count, const char c) {
    auto len = len_ch_to_ch8((unsigned char)c);
    std::u8string dst(count * len, u8'\0');
    char8_t* p = dst.data();
    const char8_t* pt = (char8_t*)tab_char::tab_utf8[(unsigned char)c].s8;
    for(; count>0; --count) {
        scopy(p, pt, len);  p += len;
    }
    return dst;
}
//                                  _count символов wc в u8string
[[nodiscard]] inline
std::u8string wchar_to_utf8_ansi(const size_t count, const wchar_t wc) {
    return _to_utf8(std::wstring(count, wc));
}
//////////////////////////////////////////////////////////////////////////////////////////////////////////////
//                                  (w)string с позиции pos длиной count в u8string
[[nodiscard]] inline
std::u8string string_to_utf8_ansi_tab_char(const std::string_view src, const size_t pos, const size_t count=std::string::npos) {
    if(pos >= src.size()) return std::u8string{};
    const size_t size_from = (std::min)(count, src.size()-pos); 

    return _to_utf8(std::string_view((char*)(src.data()+pos), size_from));
}

template<class T> requires std::is_any_of_v<T, char, unsigned char>
[[nodiscard]] inline
std::u8string string_to_utf8_ansi_tab_char(const T* const p, const size_t pos, const size_t count=std::string::npos) {
    const auto len = std::char_traits<char>::length((char*)p);
    if(pos >= len) return std::u8string{};

    return _to_utf8(std::string_view((char*)(p+pos), (std::min)(count, len-pos)));
}

#define substr_string_to_utf8 string_to_utf8_ansi_tab_char
//--------------------------------------------------------
[[nodiscard]] inline
std::u8string wstring_to_utf8_ansi(const std::wstring_view src, const size_t pos, const size_t count=std::wstring::npos) {
    if(pos >= src.size()) return std::u8string{};
    return _to_utf8(std::wstring_view((wchar_t*)(src.data()+pos), (std::min)(count, src.size()-pos)));
}

[[nodiscard]] inline
std::u8string wstring_to_utf8_ansi(const wchar_t* const p, const size_t pos, const size_t count=std::wstring::npos) {
    const auto len = std::char_traits<wchar_t>::length((wchar_t*)p);
    if(pos >= len) return std::u8string{};

    return _to_utf8(std::wstring_view(p+pos, (std::min)(count, len-pos)));
}
#define substr_wstring_to_utf8 wstring_to_utf8_ansi
//--------------------------------------------------
[[nodiscard]] inline                                      // count in SYMBOLS
std::u8string utf8_string_ansi_substr(const char8_t* const pbeg, const size_t count=std::string::npos) {
    return { pbeg, u8_distance(pbeg, (std::min)(count, u8size(pbeg))) }; 
}
//////////////////////////////////////////////////////////////////////////////////////////////////////////////
//                                      substr строки utf8 символов
[[nodiscard]] inline                            // pos and count in SYMBOLS
std::u8string utf8_string_ansi_substr(const std::u8string_view src, size_t pos, size_t count=std::string::npos) {
    const size_t len = u8size(src);

    if(pos >= len) return std::u8string{};
    count = (std::min)(count, len - pos); 
    if(!pos) return { src.data(), u8_distance(src.data(), count) };
    
    pos   = u8_distance(src.data(), pos);                        // from in SYMBOLS to im BYTES
    const auto pbeg = src.data() + pos;

    return  { src.data() + pos, u8_distance(pbeg, count) };
}
#define substr_utf8_string_ansi utf8_string_ansi_substr
//////////////////////////////////////////////////////////////////////////////////////////////////////////////
///////////////////////           one utf8 symbol FROM u8string from position pos          ///////////////////
template<class Tsize> 
[[nodiscard]] inline
std::u8string assign_symbol_string8_to_string8(const std::u8string_view src, const Tsize pos=0) {
    if(static_cast<size_t>(pos) >= u8size(src)) return std::u8string{};
    const auto ptr = u8_fwd_n(src.data(), pos);
    return {ptr, ptr+utf8len(*ptr)};
}
//---------------------           ONE utf8 symbol FROM pointer to utf8 sequnce TO u8string
[[nodiscard]] inline
std::u8string ret_string(const char8_t* const p) {
        return {p, p+utf8len(*p)};
}
//--------------------            между двумя указателями  
[[nodiscard]] inline               // не промахнись, Асунта!!!!  (с указателями-то)
std::u8string ret_string(const char8_t* const p1, const char8_t* const p2) {
        return {p1, static_cast<size_t>(p2 - p1)};
}
///////////////////////////////////////////////////////////////////////////////////////////////////////////////

