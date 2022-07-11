//#define _ITERATOR_DEBUG_LEVEL 1
#include "stdc++.h"
#include "my_adds.hpp"
//#define CDEBUG(x)
//#define _ITERATOR_DEBUG_LEVEL 1
//#define NOTDEBUG
#include "utf8string.hpp"
#include "algorithm_utf8.hpp"

//#ifdef NOTDEBUG
//#undef NOTDEBUG
//#endif
//////////////////////////////////////////////////////////////////////////////////////////////////////////////
    using std::u8string, std::utf8string, std::string, std::u8string_view, std::string_view, std::wstring;
    using std::iter8, std::wstring_view;
//////////////////////////////////////////////////////////////////////////////////////////////////////////////
int main([[maybe_unused]]int argc, [[maybe_unused]]char** argv) {
    set_ust();
    
    string s      =   "987№АБВГДЕЖЗИЙКЛМНОПРСТУФХЦЧШЩЪЫЬЭЮЯабвгдежзийклмнопрстуфхцчшщъыьэюя ABCDEFGHIJKLMNOPQRSTUVWXYZ[]^_`abcdefghijklmnopqrstuvwxyz{|}~0123456789";
    watch(s);
    watch(s.size());
    PERFORM_PT(utf8string s8 = s, s8);
    watch(s8.size());
    watch(s8.u8size());

    {
    wstring    ws8 = s8;
    wcout << "ws8 = " << ws8 << endl;
    }
    
    cout << endl;
    PERFORM_PT(utf8string ss8 = s8.substr(2, 5), ss8);
    ASSERT(ss8 == u8"7№АБВ");
    
    PERFORM_PT(ss8.assign(s8, 1, 7), ss8);
    ASSERT(ss8 == u8"87№АБВГ");
    
    PERFORM_PT(ss8.replace(1, 3, s, 10, 2), ss8);
    ASSERT(ss8 == u8"8ЖЗБВГ");

    PERFORM_PT(ss8.append(u8"QweRty"), ss8);
    ASSERT(ss8 == u8"8ЖЗБВГQweRty");
    
    PERFORM_PT(ss8.replace(2, 3, u8"зБв"), ss8);
    ASSERT(ss8 == u8"8ЖзБвГQweRty"_s8);

    PERFORM_PT(std::isort(ss8), ss8);
    ASSERT(ss8 == u8"8eQRtwyБвГЖз"sv);
    
    PERFORM_PT(std::sort(ss8), ss8);
    ASSERT(ss8 == u8"8QRetwyБГЖвз"s);

    PERFORM_PT(auto bsrt = std::is_sorted(std::begin(ss8), std::end(ss8)), bsrt);
    ASSERT(bsrt == true);

    PERFORM_PT(auto xfind = ss8.substr(5, 3), xfind);
    ASSERT(xfind == u8"wyБ"s);

    PERFORM_PT(auto pos = ss8.find(xfind), pos);
    ASSERT(pos == 5);

    PERFORM_PT(pos = ss8.find_first_of(u8"взЖБ"), pos);
    ASSERT(pos == 7);
    
    PERFORM_PT(std::reverse(ss8.begin(), ss8.end()), ss8);
    ASSERT(ss8 == u8"звЖГБywteRQ8");
    {
    cout << endl;
    PERFORM_PT(iter8 it8 = ss8.begin(), it8);
    PERFORM_PT(auto it82 = it8+4, it82);
    PERFORM_PT(it82.iter_swap(it8), it82);
    watch(it8);
    watch(ss8);
    PERFORM_PT(std::iter_swap(it8, it82), it8);
    watch(it82);
    watch(ss8);
    }

    {
    cout << endl;
    std::random_device rd;
    std::mt19937 g(rd());
    PERFORM_PT(std::shuffle(ss8.begin()+1, ss8.end()-1, g), ss8);
    std::vector<u8string> vu8 (ss8.begin(), ss8.end());
    for(auto& x: vu8) cout << x << ' '; cout << endl;
    }
    
    {
    cout << endl;
    utf8string s8_true;
    utf8string s8_false;
    std::partition_copy(ss8.begin(), ss8.end(), 
                        std::back_inserter(s8_true), std::back_inserter(s8_false),
                        [] (const auto& x) { return x < u8"Й"s;});
    std::cout << "After partition_copy:\n";
    watch(s8_true);
    watch(s8_false);
    }

    string s1, s2, s3;
    {
    cout << endl;
    PERFORM_PT(string s = "ЙгйГяЮЯюЩчщ", s);

    auto middle1 = std::partition(s.begin(), s.end(), [](const unsigned char x){ return x <= (unsigned char)'Я'; });
    std::cout << "After partition:\n";
    PERFORM_PT(s1 = s.substr(0, std::distance(s.begin(), middle1)), s1);
    std::copy(s.begin(), middle1, std::ostream_iterator<char>(std::cout, "")); cout << endl;
    watch(s);
    auto p08 = std::partition_point(s.begin(), s.end(), [](const unsigned char x){ return x < (unsigned char)'Я'; });
 
    PERFORM_PT(s2 = s.substr(0, std::distance(s.begin(), p08)), s2);
    PERFORM_PT(s3 = s.substr(std::distance(s.begin(), p08), std::distance(p08, s.end())), s3);
    std::cout << "Before partition:\n";
    std::copy(s.begin(), p08, std::ostream_iterator<char>(std::cout, " "));
    std::cout << "\nAfter partition:\n";
    std::copy(p08, s.end(), std::ostream_iterator<char>(std::cout, " ")); cout << endl;
    }
    {
    cout << endl;
    utf8string s18, s28, s38;
    PERFORM_PT(utf8string s8u = u8"ЙгйГяЮЯюЩчщ"sv, s8u);

    auto middle1 = std::partition(s8u.begin(), s8u.end(), [](const auto &s){ return s <= u8"Я"s; });
    PERFORM_PT(s18 = s8u.substr(0, std::distance(s8u.begin(), middle1)), s18);
    ASSERT(s18 == utf8string(s1));
    std::cout << "After partition:\n";
    std::copy(s8u.begin(), middle1, std::ostream_iterator<std::utf8string>(std::cout, "")); cout << endl;
    watch(s8u);
    auto p08 = std::partition_point(s8u.begin(), s8u.end(), [](const auto &s){ return s < u8"Я"; });
 
    PERFORM_PT(s28 = s8u.substr(0, std::distance(s8u.begin(), p08)), s28);
    ASSERT(s28 == utf8string(s2));
    PERFORM_PT(s38 = s8u.substr(std::distance(s8u.begin(), p08), std::distance(p08, s8u.end())), s38);
    ASSERT(s38 == utf8string(s3));
    std::cout << "Before partition:\n";
    std::copy(s8u.begin(), p08, std::ostream_iterator<std::utf8string>(std::cout, " "));
    std::cout << "\nAfter partition:\n";
    std::copy(p08, s8u.end(), std::ostream_iterator<std::utf8string>(std::cout, " ")); cout << endl;
    }
  
}   


