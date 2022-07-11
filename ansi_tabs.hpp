//                                          (для ANSI=cp1251)
#pragma once
#if !defined(ansi_tabs_hpp)
#define ansi_tabs_hpp
#include "cp1251.hpp"
//////////////////////////////////////////////////////////////////////////////////////////////////////////////
//          таблица длин utf8 символов по первому символу utf-8 последовательности (cp1251)
//////////////////////////////////////////////////////////////////////////////////////////////////////////////
inline constexpr unsigned tab_len8[] = {
     // 0             7               15              23              31
     // 0 1 2 3 4 5 6 7 8 9 A B C D E F 0 1 2 3 4 5 6 17  9 A B C D E 1F               HEX
        1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,  // 32        20
        1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,  // 64        40
        1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,  // 96        60
        1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,  // 128       80
        0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,  // 160       A0
        0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,  // 192       C0
        0,0,2,0,0,0,0,0,0,0,0,0,0,0,0,0,2,2,2,0,0,0,0,0,0,0,0,0,0,0,0,0,  // 224       E0
        0,0,3,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0   // 256      100
};

#define c8len(x)    tab_len8[(x)]

[[nodiscard]] constexpr
unsigned utf8len(const char8_t x)        { return tab_len8[x]; }
[[nodiscard]] constexpr
unsigned utf8len(const char8_t* const p) { return tab_len8[*p]; }
[[nodiscard]] constexpr
unsigned u8len  (const char8_t x)        { return tab_len8[x]; }
[[nodiscard]] constexpr
unsigned u8len  (const char8_t* const p) { return tab_len8[*p]; }
#define len8(x)     utf8len(x)
///////////////////////////////////////////////////////////////////////////////////////////////////////////////
//                      таблица наличия символа utf8 (для подсчета длины строки utf8 в символах) (cp1251)            //
///////////////////////////////////////////////////////////////////////////////////////////////////////////////
inline constexpr int tab_len8m[] = {
     // 0             7               15              23              31
     // 0 1 2 3 4 5 6 7 8 9 A B C D E F 0 1 2 3 4 5 6 17  9 A B C D E 1F               HEX
        1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,  // 32        20
        1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,  // 64        40
        1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,  // 96        60
        1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,  // 128       80
        0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,  // 160       A0
        0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,  // 192       C0
        0,0,1,0,0,0,0,0,0,0,0,0,0,0,0,0,1,1,1,0,0,0,0,0,0,0,0,0,0,0,0,0,  // 224       E0
        0,0,1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0   // 256      100
};
///////////////////////////////////////////////////////////////////////////////////////////////////////////////
//                          таблица длин utf8 символов по ANSI символу
////////////////////////////////////////////////////////////////////////////////////////////////////////////
inline constexpr 
unsigned tab_len_ch_to_len_ch8[] = {
     // 0             7               15              23              31
     // 0 1 2 3 4 5 6 7 8 9 A B C D E F 0 1 2 3 4 5 6 17  9 A B C D E 1F               HEX
        1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,  // 32        20
        1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,  // 64        40
        1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,  // 96        60
        1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,  // 128       80
        2,2,3,2,3,3,3,3,3,3,2,3,2,2,2,2,2,3,3,3,3,3,3,3,2,3,2,3,2,2,2,2,  // 160       A0
        2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,3,2,2,2,2,2,2,  // 192       C0
        2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,  // 224       E0
        2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2   // 256      100
};
////////////////////////////////////////////////////////////////////////////////////////////////////////////
//                длина в байтах символа ANSI(cp1251) при переводе в char8_t                       
////////////////////////////////////////////////////////////////////////////////////////////////////////////
#define lenc_to_c8(x)      tab_len_ch_to_len_ch8[(x)]
#define len_c_to_c8(x)     tab_len_ch_to_len_ch8[(x)]
#define len_ch_to_ch8(x)   tab_len_ch_to_len_ch8[(x)]
////////////////////////////////////////////////////////////////////////////////////////////////////////////
//                           Возвращает кол-во символов (NOT bytes) в UTF8 строке
///////////////////////////////////////////////////////////////////////////////////////////////////////////////
[[nodiscard]] constexpr 
size_t u8size_old(const std::u8string_view x) { // in symbols, not in bytes
    return std::reduce(x.cbegin(), x.cend(), size_t{}, [] (const size_t a, const unsigned char b) {
        return a + tab_len8m[b];
    });
} 
//////////////////////////////////////////////////////////////////////////////////////////////////////////////
[[nodiscard]] constexpr
int u8_is_lead(const uint8_t c) noexcept {
    return (((~c) & 0xC0) == 0);
}
[[nodiscard]] constexpr
bool u8_is_lead_b(const uint8_t c) noexcept {
    return (((~c) & 0xC0) == 0);
}
[[nodiscard]] constexpr
int u8_is_trail(const unsigned c) noexcept {
    //return ( ((c&0x80) != 0) && ((c&0x40) == 0) );
    return ( (c&0xC0) == 0x80);
}
[[nodiscard]] constexpr
bool u8_is_trail_b(const uint8_t c) noexcept {
    //return ( ((c&0x80) != 0) && ((c&0x40) == 0) );
    return ( (c&0xC0) == 0x80 );
}
[[nodiscard]] constexpr
int u8_is_first(const unsigned c) noexcept {
    return ( ((c&0x80) == 0) || ((c&0x40) != 0) );
    //return ( (c&0xC0) != 0x80 );
}
[[nodiscard]] constexpr
bool u8_is_first_b(const uint8_t c) noexcept {
    return ( ((c&0x80) == 0) || ((c&0x40) != 0) );
    //return ( (c&0xC0) != 0x80 );
}
//////////////////////////////////////////////////////////////////////////////////////////////////////////////
[[nodiscard]] constexpr 
size_t u8size(const std::u8string_view x) {
    size_t size(0);
    for(const auto& c: x) size += u8_is_first(c);
    return size;
}
[[nodiscard]] constexpr 
size_t u8size(const char8_t* p1, const char8_t* const p2) {
    size_t size(0);
    while(p1 < p2) size += u8_is_first(*p1++);
    return size;
}
[[nodiscard]] constexpr                   // len in BYTES
size_t u8size(const char8_t* p1, const size_t len) {
     return u8size(p1, p1 + len);
}
///////////////////////////////////////////////////////////////////////////////////////////////////////////////
//          Длина строки символов STRING в кол-ве байт, занимаемых этими символами в utf8 кодировке
//////////////////////////////////////////////////////////////////////////////////////////////////////////////
[[nodiscard]] constexpr
size_t u8size(const std::string_view x) { // in symbols, not in bytes
    return std::reduce(x.cbegin(), x.cend(), size_t{}, [] (const size_t a, const unsigned char b) {
        return a + len_ch_to_ch8(b);
    });
}
////////////////////////////////////////////////////////////////////////////////////////////////////////////
//                                   char -> char8_t   c -> mb8 
////////////////////////////////////////////////////////////////////////////////////////////////////////////
// реверс int для помещения в строку символов
[[nodiscard]] constexpr
unsigned reversem(unsigned n, unsigned len=4) {
  unsigned m = n & 0x0ff;
  n >>= 8;
  while(--len) {
    m = (m<<8) + (n&0x0ff);//m*256+n%256;
    n >>= 8;
  };
  return m;
}
//--------------------------------------------------------------------------------------------------------
//                                   char -> char8_t   c -> mb8 
constexpr 
int cp1251_mbtoc8(unsigned char *pc, const unsigned char c, const unsigned n) {
  if(c < 0x80) { *pc = c; return 1; }
  if(n < 4) return -1;
  auto len = len_ch_to_ch8(c); 
  *(unsigned*)pc = reversem(cp1251_2utf8[c-0x80], len);
  return (int)len;
}
//--------------------------------------------------------------------------------------------------------
constexpr 
int cp1251_mbtoc8(unsigned char *pc, const unsigned char c) {    // без проверки n
  if(c < 0x80) { *pc = c; return 1; }
  auto len = len_ch_to_ch8(c); 
  *(unsigned*)pc = reversem(cp1251_2utf8[c-0x80], len);
  return len;
}
//////////////////////////////////////////////////////////////////////////////////////////////////////////////
//                           utf8 symbol -> lower utf8 symbol
//////////////////////////////////////////////////////////////////////////////////////////////////////////////
constexpr 
unsigned cp1251_mb8_tolower_mb8 (char8_t* const r, const char8_t* const p8) {    // c8 -> c8  mb8 -> mb8
    const unsigned c8 = *p8;
    if(c8 >= 0x3f && c8 <= 0x5a) { *r = static_cast<char8_t>(c8 + 0x020);  return 1; } // ASCII
  
    unsigned c = 0;
    const unsigned c81 = (c8<<8) | p8[1];
    if(c81 >= 0xd081 && c81 <= 0xd08f) c = c81 + 0x0110;
    else
    if(c81 == 0xd290)  c = 0xd291; //c81 + 1;
    else
    if(c81 >= 0xd090 && c81 <= 0xd09f) c = c81 + 0x20;   // d090 - d0af (А-Я)
    else                                               // d0b0 - d0bf (а-п) d180-d18f(р-я)
    if(c81 >= 0xd0a0 && c81 <= 0xd0af) c = c81 + 0xe0;   // d0b0-d090 = 0x20 d0bf-d09f=0x20 d180-d0a0=0xe0  d18f-d0af=0xe0
    else { 
        auto nn = utf8len(*p8);             // не надо переводить в нижний регмстр
        //for(unsigned i=0; i<nn; ++i) r[i] = p8[i];
        std::char_traits<char8_t>::copy(r, p8, nn);
        return nn; 
    }
  
    r[1] = c & 0x0ff;  r[0] = ( (c>>8) & 0x0ff );
    return 2;
}
///----------------------------------------------------------------------------------------------------------
//                                              inplace
constexpr 
unsigned cp1251_mb8_tolower_mb8 (char8_t* const r) {   // c8 -> c8  mb8 -> mb8  tolower
  const unsigned c8 = *r;
  if(c8 >= 0x41 && c8 <= 0x5a) { *r = static_cast<char8_t>(c8 + 0x020);  return 1; }  // ASCII
  
  unsigned c = 0;
  const unsigned c81 = (c8<<8) | r[1];
  if(c81 >= 0xd081 && c81 <= 0xd08f) c = c81 + 0x0110;
  else
  if(c81 == 0xd290)  c = 0xd291; //c81 + 1;
  else
  if(c81 >= 0xd090 && c81 <= 0xd09f) c = c81 + 0x20;   // d090 - d0af (А-Я)
  else                                               // d0b0 - d0bf (а-п) d180-d18f(р-я)
  if(c81 >= 0xd0a0 && c81 <= 0xd0af) c = c81 + 0xe0;   // d0b0-d090 = 0x20 d0bf-d09f=0x20 d180-d0a0=0xe0  d18f-d0af=0xe0
  else { auto nn = utf8len(*r); return nn; }
  
  r[1] = c & 0x0ff;  r[0] = ( (c>>8) & 0x0ff );
  return 2;
}
//////////////////////////////////////////////////////////////////////////////////////////////////////////////
struct tab_char {
  typedef struct {
    unsigned  len; 
    char8_t       s8[4];
  }  struct_u8;

  inline static unsigned char tab_lo[256];
  inline static unsigned char tab_hi[256];
  
  inline static struct_u8     tab_utf8[256];
  
  tab_char(const std::locale& L = std::locale("ru")) {
    const std::ctype<char>& ct = std::use_facet<std::ctype<char>>(L);
    for(unsigned i=0; i<256; ++i) tab_lo[i] = tab_hi[i] = static_cast<unsigned char>(i);
    _SOME some;
    for(int i=0; i<256; ++i) {
      some.ui = 0;
      tab_utf8[i].len = cp1251_mbtoc8( (unsigned char *)some.uc, tab_lo[i]);  // без проверки длины принимающей
      memcpy(tab_utf8[i].s8, some.uc, tab_utf8[i].len);
    }
    ct.tolower((char*)tab_lo, (char*)tab_lo+256);
    ct.toupper((char*)tab_hi, (char*)tab_hi+256);
  }
  // low
  struct lt_lo {
    constexpr bool operator() (const unsigned char x, const unsigned char y) const noexcept { 
      return tab_lo[x] < tab_lo[y]; 
    }
  };
  struct eq_lo {
    constexpr bool operator() (const unsigned char x, const unsigned char y) const noexcept { 
    return tab_lo[x] == tab_lo[y]; }
  };
  struct gt_lo {
    constexpr bool operator() (const unsigned char x, const unsigned char y) const noexcept { 
    return tab_lo[x] > tab_lo[y]; }
  };
  // high
  struct lt_hi {
    constexpr bool operator() (const unsigned char x, const unsigned char y) const noexcept { 
      return tab_hi[x] < tab_hi[y]; 
    }
  };
  struct eq_hi {
    constexpr bool operator() (const unsigned char x, const unsigned char y) const noexcept { 
    return tab_hi[x] == tab_hi[y]; }
  };
  struct gt_hi {
    constexpr bool operator() (const unsigned char x, const unsigned char y) const noexcept { 
    return tab_hi[x] > tab_hi[y]; }
  };
} inline const var_iless;
//////////////////////////////////////////////////////////////////////////////////////////////////////////////
//                             Сравнение ANSI СИМВОЛОВ без учета регистра
///////////////////////////////////////////////////////////////////////////////////////////////////////////////
[[nodiscard]] constexpr 
bool iequ_ansi_chars(const unsigned char x, const unsigned char y) noexcept {
    return tab_char::eq_lo()(x, y);
}
//--------------------------
struct iequal_to_ansi_chars {
  constexpr bool operator() (const unsigned char x, const unsigned char y) const noexcept {
    return tab_char::eq_lo()(x, y);
  }
};
//---------------------
[[nodiscard]] constexpr 
bool ilt_ansi_chars(const unsigned char x, const unsigned char y) noexcept {
    return tab_char::lt_lo()(x, y);
}
//---------------------
struct iless_ansi_chars {
  constexpr bool operator() (const unsigned char x, const unsigned char y) const noexcept {
    return tab_char::lt_lo()(x, y);
  }
};
//////////////////////////////////////////////////////////////////////////////////////////////////////////////
//                             Сравнение ANSI строк без учета регистра
//////////////////////////////////////////////////////////////////////////////////////////////////////////////
[[nodiscard]] constexpr 
int icompare_ansi_strings_long_impl(const std::string_view s1, const std::string_view s2) {
  const auto& [first, second] = std::mismatch(s1.begin(), s1.end(), s2.begin(), s2.end(), iequal_to_ansi_chars());
  // 
  if(first == s1.end()) {                       // Если условие истинно, 
    if(second == s2.end()) return 0;            // либо s1 и s2 равны. 
    else                   return -1;           // либо s1 короче s2 
  }
  return iless_ansi_chars() (*first, *second) ? -1: 1; 
}
//---------------
[[nodiscard]] constexpr 
int icompare_ansi_strings_long(const std::string_view s1, const std::string_view s2) {
  return s1.size() <= s2.size()? icompare_ansi_strings_long_impl(s1, s2) : -icompare_ansi_strings_long_impl(s2, s1);
}
//----------------------------            функторы            -------------------------------------------
struct iless_ansi_strings_long {
  constexpr bool operator() (const std::string_view s1, const std::string_view s2) const noexcept {
    return icompare_ansi_strings_long(s1, s2) < 0;
  }
};
//---------------------------------
struct iequal_to_ansi_strings_long {
  constexpr bool operator() (const std::string_view s1, const std::string_view s2) const noexcept {
    return icompare_ansi_strings_long(s1, s2) == 0;
  }
};
//-----------------------------------------------------------------------------------------------------------
[[nodiscard]] constexpr 
int icompare_ansi_strings_impl(const std::string_view s1, const std::string_view s2) {
    for(size_t i = 0; i < s1.size(); ++i) {
        if(!iequal_to_ansi_chars()(s1[i], s2[i])) return iless_ansi_chars()(s1[i], s2[i]) ? -1:1;
    }
    return s1.size() < s2.size() ? -1 : 0;    // <=
}

[[nodiscard]] constexpr 
int icompare_ansi_strings(const std::string_view s1, const std::string_view s2) {
  return s1.size() <= s2.size()? icompare_ansi_strings_impl(s1, s2) : -icompare_ansi_strings_impl(s2, s1);
}
//----------------------------            функторы            -------------------------------------------
struct iless_ansi_strings {
  constexpr bool operator() (const std::string_view s1, const std::string_view s2) const noexcept {
    return icompare_ansi_strings(s1, s2) < 0;
  }
};
#define Nocase_ansi_strings iless_ansi_strings
//-----------------------------
struct iequal_to_ansi_strings {
  constexpr bool operator() (const std::string_view s1, const std::string_view s2) const noexcept {
    return icompare_ansi_strings(s1, s2) == 0;
  }
};
//---------------------
[[nodiscard]] constexpr 
bool iequal_to_ansi_strings_funcs(const std::string_view x, const std::string_view y) noexcept {
    return icompare_ansi_strings(x, y) == 0;
}
//---------------------
[[nodiscard]] constexpr 
bool iless_ansi_strings_funcs(const std::string_view x, const std::string_view y) noexcept {
    return icompare_ansi_strings(x, y) < 0;
}
//////////////////////////////////////////////////////////////////////////////////////////////////////////////
//                           utf8 symbol -> lower char ANSI symbol
//////////////////////////////////////////////////////////////////////////////////////////////////////////////
[[nodiscard]] constexpr 
unsigned char cp1251_mb8tolowermb (const unsigned char* const p8) {
  const unsigned c8 = *p8;
  if(c8 <= 0x7f) { return tab_char::tab_lo[c8]; }   // ASCII
  
  unsigned char c = 0;
  const unsigned c81 = (c8<<8) | p8[1];

  if(c8 < 0xe2) {
    if(c81 >= 0xd090 && c81 <= 0xd0bf) c = (c81&0x0ff) + 0x30;     // А - Я, а-п
    else
    if(c81 >= 0xd180 && c81 <= 0xd18f) c = (c81&0x0ff) + 0x70;     // р-я
    else 
    if(c8 == 0xc2) c = c81&0x0ff;
    else           c = cp1251_page04_8[ ((c8&0x02) << 3) + (c81&0x3f) ];
  }
  else 
  if(c8 == 0xe2) {
    const unsigned c82 = p8[2];
    if((c81&0x0ff) == 0x084 && c82 == 0x0a2) c = 0x99;
    else                                   c = cp1251_page20_8[ c82 & 0x03f ];//- 0x10 ]; //доступный для чтения объем равен "43" байт, однако считать можно только "48" байт
  }
  return tab_char::tab_lo[c];
}
//------------------------------------------------------------------
constexpr 
unsigned cp1251_mb8tolowermb (unsigned char& c, const unsigned char* const p8) {
  const unsigned c8 = *p8;
  if(c8 <= 0x7f) { c = tab_char::tab_lo[c8]; return 1; }   // ASCII

  const unsigned c81 = (c8<<8) | p8[1];
  unsigned nn(2);
  if(c8 < 0xe2) {
    if(c81 >= 0xd090 && c81 <= 0xd0bf) c = (c81&0x0ff) + 0x30;     // А - Я, а-п
    else
    if(c81 >= 0xd180 && c81 <= 0xd18f) c = (c81&0x0ff) + 0x70;     // р-я
    else 
    if(c8 == 0xc2) c = c81&0x0ff;
    else           c = cp1251_page04_8[ ((c8&0x02) << 3) + (c81&0x3f) ];
  }
  else 
  if(c8 == 0xe2) {
    nn = 3;
    const unsigned c82 = p8[2];
    if((c81&0x0ff) == 0x084 && c82 == 0x0a2) c = 0x99;
    else                                   c = cp1251_page20_8[ c82 & 0x03f ];//- 0x10 ]; // доступный для чтения объем равен "43" байт, однако считать можно только "48" байт
  } else nn = 1;
  c = tab_char::tab_lo[c];
  return nn;
}
//---------------------------------------------------------------------------------
constexpr 
unsigned cp1251_mb8tolowermb(unsigned char* const r, const unsigned char* const p8) {
    return cp1251_mb8tolowermb(*r, p8);
}
///////////////////////////////////////////////////////////////////////////////////////////////////////////////
//                              ANSI string to lower ANSI string
///////////////////////////////////////////////////////////////////////////////////////////////////////////////
[[nodiscard]] inline //constexpr 
std::string  string_to_lower_ansi_tab_char(const std::string_view src) {
  std::string dst(src);
  for(auto& x: dst) x = tab_char::tab_lo[static_cast<unsigned char>(x)];
  return dst;
}
//////////////////////////////////////////////////////////////////////////////////////////////////////////////
//                              ANSI string to higher ANSI string
///////////////////////////////////////////////////////////////////////////////////////////////////////////////
[[nodiscard]] inline //constexpr 
std::string  string_to_higher_ansi_tab_char(const std::string_view src) {
  std::string dst(src);
  for(auto& x: dst) x = tab_char::tab_hi[static_cast<unsigned char>(x)];
  return dst;
}
//////////////////////////////////////////////////////////////////////////////////////////////////////////////
//                           ANSI  std::string, char*, std::string_view -> std::u8string
//////////////////////////////////////////////////////////////////////////////////////////////////////////////
constexpr
void copy_plus(unsigned char*& to, const unsigned char* from, int len) {
    while(len-- > 0) *to++ = *from++;
}
constexpr
void copy_plus(char8_t*& to, char8_t const* from, int len) {
    while(len-- > 0) *to++ = *from++;
}
//------------------
[[nodiscard]] inline //constexpr
std::u8string string_to_utf8_ansi_tab_char(const std::string_view src) {
    std::u8string dst(u8size(src), u8'\0');

    for(char8_t* p = dst.data(); const unsigned char& x: src) { 
         copy_plus(p, tab_char::tab_utf8[x].s8, tab_char::tab_utf8[x].len); 
    }
    return dst;
} 
//////////////////////////////////////////////////////////////////////////////////////////////////////////////
struct tab_wchar {
  inline static wchar_t tab   [256];
  inline static wchar_t tab_lo[256];
  inline static wchar_t tab_hi[256];

  tab_wchar(const std::locale& L = std::locale("ru")) {
    const std::ctype<wchar_t>& ct = std::use_facet<std::ctype<wchar_t>>(L);
    for(int i=0; i<256; ++i) tab[i] = tab_lo[i] = tab_hi[i] = ct.widen(static_cast<char>(i));
    ct.tolower(tab_lo, tab_lo + 256);
    ct.toupper(tab_hi, tab_hi + 256);
  }
  //                              cp1251_wctomb  // wc -> mb  wchar_t -> char
  // norm
  /*struct lt_wchar {
    constexpr bool operator() (const wchar_t x, const wchar_t y) const noexcept { 
        return tab[cp1251_wctomb(x)] < tab[cp1251_wctomb(y)]; 
    }
  };
  struct eq_wchar {
    constexpr bool operator() (const wchar_t x, const wchar_t y) const noexcept { 
    return tab[cp1251_wctomb(x)] == tab[cp1251_wctomb(y)]; }
  };
  struct gt_wchar {
    constexpr  bool operator() (const wchar_t x, const wchar_t y) const noexcept { 
    return tab[cp1251_wctomb(x)] > tab[cp1251_wctomb(y)]; }
  };*/
  // lower
  struct lt_lo {
    constexpr  bool operator() (const wchar_t x, const wchar_t y) const noexcept { 
        return tab_lo[cp1251_wctomb(x)] < tab_lo[cp1251_wctomb(y)]; 
    }
  };
  struct eq_lo {
    constexpr  bool operator() (const wchar_t x, const wchar_t y) const noexcept { 
        return tab_lo[cp1251_wctomb(x)] == tab_lo[cp1251_wctomb(y)]; 
    }
  };
  struct gt_lo {
    constexpr bool operator() (const wchar_t x, const wchar_t y) const noexcept { 
        return tab_lo[cp1251_wctomb(x)] > tab_lo[cp1251_wctomb(y)]; 
    }
  };
  // high
  struct lt_hi {
    constexpr  bool operator() (const wchar_t x, const wchar_t y) const noexcept { 
        return tab_hi[cp1251_wctomb(x)] < tab_hi[cp1251_wctomb(y)]; 
    }
  };
  struct eq_hi {
    constexpr  bool operator() (const wchar_t x, const wchar_t y) const noexcept { 
        return tab_hi[cp1251_wctomb(x)] == tab_hi[cp1251_wctomb(y)]; 
    }
  };
  struct gt_hi {
    constexpr bool operator() (const wchar_t x, const wchar_t y) const noexcept { 
        return tab_hi[cp1251_wctomb(x)] > tab_hi[cp1251_wctomb(y)]; 
    }
  };
} inline const tabilessm;
///////////////////////////////////////////////////////////////////////////////////////////////////////////////
struct iless_wc {
  constexpr bool operator() (const std::wstring_view x, const std::wstring_view y) const {
    const size_t _nums = (std::min)(x.size(), y.size()); 
    for(size_t i = 0; i < _nums; ++i) {
      if(!tab_wchar::eq_lo()(x[i], y[i])) return tab_wchar::lt_lo()(x[i], y[i]);
    }
    if(x.size() >= y.size()) return false;
    return true;
  }
};
///////////////////////////////////////////////////////////////////////////////////////////////////////////////
[[nodiscard]] inline
std::wstring wstring_to_lower_ansi_tab_wchar(const std::wstring_view src) {
  std::wstring dst(src);
  for(wchar_t& x: dst)  x = tab_wchar::tab_lo[cp1251_wctomb(x)]; 
  return dst;
} 
#define wstring_to_lower_ansi(x) wstring_to_lower_ansi_tab_wchar(x)
///////////////////////////////////////////////////////////////////////////////////////////////////////////////
[[nodiscard]] inline
std::wstring string_to_wstring_ansi_tab_wchar(const std::string_view src) {
  std::wstring dst(src.size(), L'\0');
  for(wchar_t* p = (wchar_t*) dst.data(); const unsigned char& x: src) *p++ = tab_wchar::tab[x];
  return dst;
}
///////////////////////////////////////////////////////////////////////////////////////////////////////////////
//              utf8 -> wchar_t символ в Windows  mb8 -> wc
///////////////////////////////////////////////////////////////////////////////////////////////////////////////
unsigned utf8_decode(const char* const p, int* len=nullptr) {    // utf8_mbtowc  (char8_t -> wchar_t)
  const unsigned char c = *(const unsigned char*)p;
  if(c < 0x80) { if(len) *len = 1; return c; } 
  else if(c < 0xc2) goto FAIL;

  if(c < 0xe0) {
    if(len) *len = 2; 
     return  ((p[0] & 0x1f) << 6) + ((p[1] & 0x3f));
  } 
  else if(c == 0xe0) {
    if(((const unsigned char*)p)[1] < 0xa0) goto FAIL;
    goto UTF8_3;
  } else if(c < 0xf0) {
  UTF8_3:
    if(len) *len = 3;
    return    ((p[0] & 0x0f) << 12) +  ((p[1] & 0x3f) << 6) +  ((p[2] & 0x3f));
  } else if(c == 0xf0) {
    if(((const unsigned char*)p)[1] < 0x90) goto FAIL;
    goto UTF8_4;
  } else if(c < 0xf4) {
  UTF8_4:
    if(len) *len = 4;
    return  ((p[0] & 0x07) << 18) + ((p[1] & 0x3f) << 12) + ((p[2] & 0x3f) << 6) + ((p[3] & 0x3f));
  } else if(c == 0xf4) {
    if(((const unsigned char*)p)[1] > 0x8f) goto FAIL; // после 0x10ffff
    goto UTF8_4;
  } else {
  FAIL:
    if(len) *len = 1;
    return c;
  }
}
///////////////////////////////////////////////////////////////////////////////////////////////////////////////
//              utf8 -> wchar_t символ в Windows  mb8 -> wc
//              тоже, что и предыдущая, только есть еще end для использования в некоторых ф-ях
///////////////////////////////////////////////////////////////////////////////////////////////////////////////
unsigned utf8_decode(const char* const p, const char* const end, int* len) {
  const unsigned char c = *(const unsigned char*)p;
  if(c < 0x80) { if(len) *len = 1;  return c; } 
  else if(c < 0xc2) goto FAIL;

  if( (end && p+1 >= end) || (p[1]&0xc0) != 0x80) goto FAIL;
  if(c < 0xe0) { if (len) *len = 2;  return  ((p[0] & 0x1f) << 6) + ((p[1] & 0x3f)); } 
  else if(c == 0xe0) {
    if(((const unsigned char*)p)[1] < 0xa0) goto FAIL;
    goto UTF8_3;
  } else if(c < 0xf0) {
  UTF8_3:
    if( (end && p+2 >= end) || (p[2]&0xc0) != 0x80) goto FAIL;
    if(len) *len = 3;
    return  ((p[0] & 0x0f) << 12) + ((p[1] & 0x3f) << 6) + ((p[2] & 0x3f));
  } else if(c == 0xf0) {
    if(((const unsigned char*)p)[1] < 0x90) goto FAIL;
    goto UTF8_4;
  } else if(c < 0xf4) {
  UTF8_4:
    if( (end && p+3 >= end) || (p[2]&0xc0) != 0x80 || (p[3]&0xc0) != 0x80) goto FAIL;
    if(len) *len = 4;
    return   ((p[0] & 0x07) << 18) + ((p[1] & 0x3f) << 12) + ((p[2] & 0x3f) << 6) + ((p[3] & 0x3f));
  } else if(c == 0xf4) {
    if(((const unsigned char*)p)[1] > 0x8f) goto FAIL; // after 0x10ffff
    goto UTF8_4;
  } else {
  FAIL:
    if(len) *len = 1;
    return c;
  }
}
///////////////////////////////////////////////////////////////////////////////////////////////////////////////
//                                   wchar_t -> char8_t   wc -> mb8
///////////////////////////////////////////////////////////////////////////////////////////////////////////////
constexpr 
int utf8_wctomb (unsigned char* const r, unsigned wc) { 
  int count(0);
  if(wc < 0x80)               count = 1;
  else if (wc < 0x800)        count = 2;
  else if (wc < 0x10000)      count = 3;
  else if (wc < 0x200000)     count = 4;
  else if (wc < 0x4000000)    count = 5;
  else if (wc <= 0x7fffffff)  count = 6;
  else                        return -1;

  switch (count) { /* note: code falls through cases! */
    case 6: r[5] = 0x80 | (wc & 0x3f); wc = wc >> 6; wc |= 0x4000000;
    case 5: r[4] = 0x80 | (wc & 0x3f); wc = wc >> 6; wc |= 0x200000;
    case 4: r[3] = 0x80 | (wc & 0x3f); wc = wc >> 6; wc |= 0x10000;
    case 3: r[2] = 0x80 | (wc & 0x3f); wc = wc >> 6; wc |= 0x800;
    case 2: r[1] = 0x80 | (wc & 0x3f); wc = wc >> 6; wc |= 0xc0;
    case 1: r[0] = static_cast<char>(wc);
  }
  return count;
}
//---------------------------------------------------------
constexpr 
unsigned len_wc_to_ch8(const unsigned wc) { 
  unsigned count(0);
  
  if(wc < 0x80)               count = 1;
  else if (wc < 0x800)        count = 2;
  else if (wc < 0x10000)      count = 3;
  else if (wc < 0x200000)     count = 4;
  else if (wc < 0x4000000)    count = 5;
  else                        count = 6;
  //else if (wc <= 0x7fffffff)  count = 6;
  //else                        return -1;
  return count;
}
///////////////////////////////////////////////////////////////////////////////////////////////////////////////
/*template<class T> constexpr 
uint32_t utf8len(const T c) {     // char || char8_t
  if(!(c & 0x80)) return 1;
  if(c & 0x40) {
    if(c & 0x20) {
      if(c & 0x10) {                          
        if(c & 0x08) {
          if(c & 0x04)  return 6;
          return 5;
        }
        return 4;
      }
      return 3;
    }
    return 2;
  }
  return 0;
}
#define c8len  utf8len
*/
/*
[[nodiscard]] constexpr         Тоже неплохая
unsigned utf8len_with_trail_bytes(uint8_t c) {
    return 1 + (c >= 0xc0) + (c >= 0xe0);// + (c >= 0xf0);
 */
//////////////////////////////////////////////////////////////////////////////////////////////////////////////
#endif  // ansi_tabs_hpp
