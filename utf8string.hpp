//
//                  utf8string.hpp  -- Copyright (C) by Alexander Prilepsky 2022
//
// Этот заголовочный файл содержит класс  std::utf8string, класс back_insert_iterator<std::utf8string>
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
#ifndef _utf8string_hpp_
#define _utf8string_hpp_
#include "str8_utils.hpp"

//////////////////////////////////////////////////////////////////////////////////////////////////////////////
namespace std {
// Вообще не знаю, нужен ли этот u8char_traits, не придумал ничего интересного, так, ерунда всякая
// проверял работу utf8string с ПУСТЫМ u8char_traits. Хотя его содержимое не должно повлиять на ut8string, 
// потому, что нигде не использовал
struct u8char_traits : char_traits<char8_t> {
    using _Elem = char8_t;
    using Base_traits = char_traits<_Elem>;

    // это уже есть в utf8string
    [[nodiscard]] static constexpr 
    size_t clamp_suffix_size(const size_t len, const size_t pos, const size_t count) noexcept {
        // trims count to the longest it can be assuming a string at/after pos
        return (std::min)(count, len - pos);
    }
    // это уже есть в utf8string
    [[nodiscard]] static constexpr               // count in SYMBOLS
    auto retPosSize(const _Elem* const p, size_t count) noexcept {
        const size_t len = ::u8size(p);    // in symbols
        count = clamp_suffix_size(len, 0, count);
        return u8_distance(p, count);
    }

    static constexpr20                                                    // count in SYMBOLS
    _Elem* copy(_Elem* const first1, const _Elem* const first2, const size_t count) noexcept  {
        return Base_traits::copy(first1, first2, retPosSize(first1, count));
    }
   
    static constexpr20
    _Elem* move(_Elem* const first1, const _Elem* const first2, const size_t count) noexcept  {
        // copy [first2, first2 + count) to [first1, ...), allowing overlap
        return Base_traits::move(first1, first2, retPosSize(first1, count));
    }

    static constexpr20
    _Elem* _Copy_s(_Elem* const first1, const size_t dest_size, const _Elem* const first2, const size_t count) {
        // copy [first2, first2 + count) to [first1, first1 + dest_size)
        ASSERTM(count <= dest_size, "invalid argument");
        return copy(first1, first2, count);
    }

    [[nodiscard]] static constexpr                                  // count in SYMBOLS
    int compare(const _Elem* const first1, const _Elem* const first2, const size_t count) noexcept {
        // compare [first1, first1 + count) with [first2, ...)
        return Base_traits::compare(first1, first2, retPosSize(first1, count));
    }
    
    using Base_traits::length;

    [[nodiscard]] static constexpr
    size_t length8(const _Elem* const first) noexcept {
        return ::u8size(first);
    }

    [[nodiscard]] static constexpr
    const _Elem* find(const _Elem* const first, const size_t count, const _Elem& ch) noexcept {
        // look for ch in [first, first + count)
        return Base_traits::find(first, retPosSize(first, count), ch);
    }

    static  constexpr20
    _Elem* assign(_Elem* const first, size_t count, const _Elem ch) noexcept {
        // assign count * ch to [first, ...)
        return Base_traits::assign(first, count, ch);
    }

    static constexpr
    void assign(_Elem& _Left, const _Elem& _Right) noexcept {
        _Left = _Right;
    }

    [[nodiscard]] static constexpr
    bool eq(const _Elem& left, const _Elem& right) noexcept {
        return left == right;
    }

    [[nodiscard]] static constexpr
    bool eq(const _Elem* const left, const _Elem* right) noexcept {
        return 0 == compare_two_utf8_symbols(left, right);
    }

    [[nodiscard]] static constexpr
    bool lt(const _Elem& left, const _Elem& right) noexcept {
        return static_cast<unsigned char>(left) < static_cast<unsigned char>(right);
    }

    [[nodiscard]] static constexpr
    bool lt(const _Elem* const left, const _Elem* right) noexcept {
        return compare_two_utf8_symbols(left, right) < 0;
    }

    using Base_traits::to_char_type;    // переделать ???????????????
    //_NODISCARD static constexpr _Elem to_char_type(const int_type& _Meta) noexcept {
    //    return static_cast<_Elem>(_Meta);
    //}
    using Base_traits::to_int_type;     // переделать ????????????
    //_NODISCARD static constexpr int_type to_int_type(const _Elem& _Ch) noexcept {
    //    return static_cast<unsigned char>(_Ch);
    //}
    using Base_traits::eq_int_type;
    //_NODISCARD static constexpr bool eq_int_type(const int_type& _Left, const int_type& _Right) noexcept {
    //    return _Left == _Right;
    //}
    using Base_traits::not_eof;
    //_NODISCARD static constexpr int_type not_eof(const int_type& _Meta) noexcept {
    //    return _Meta != eof() ? _Meta : !eof();
    //}
    using Base_traits::eof;
    //_NODISCARD static constexpr int_type eof() noexcept {
    //    return static_cast<int_type>(EOF);
    //}
}; // struct u8char_traits

template<class Traits = std::u8char_traits>
class basic_utf8string;

using utf8string = basic_utf8string<std::u8char_traits>;

template<class T>
concept convertible_to_utf8 = std::is_convertible_v<T, std::utf8string>;

template<class _Iter>
concept is_iterator_only_v = is_iterator_v<_Iter> && (!is_pointer_v<remove_cvref_t<_Iter>>);
} // namespace std;

#include "u8_iterator.hpp"
//////////////////////////////////////////////////////////////////////////////////////////////////////////////
namespace std {

//  Во избежание ошибок, в функциях utf8string лучше используйте итераторы utf8string вместо указателей
//WARN("To avoid ERRORS,\n    it is better to use utf8string iterators INSTEAD of pointers in utf8string functions.\n");

template<class Traits>
class basic_utf8string : public u8string {
    using _Traits = Traits;
    using _Alloc = allocator<char8_t>;
    
    template<class>
    friend class Iterator8;
    template<class>
    friend class Retu8;

public:
    using Base                  = u8string;//basic_string<char8_t, char_traits<char8_t>, allocator<char8_t>>;
    using Base_view             = u8string_view;
    using traits_type           = _Traits;
    using value_type            = char8_t;
    using _Elem                 = char8_t;
    using pointer               = _Elem*;
    using const_pointer         = const _Elem*;
    using reference             = _Elem*;
    using const_reference       = const _Elem*;
    using Bstring               = string;

    using iterator              = Iterator8<basic_utf8string>;
    using const_iterator        = Iterator8<basic_utf8string>;
    using reverse_iterator      = std::reverse_iterator<iterator>;
    using const_reverse_iterator= std::reverse_iterator<const_iterator>;

    using typename Base::size_type;//             = size_t;      // in win32 == uint32, in win64 == uint64
    using typename Base::difference_type;//       = ptrdiff_t;   // in win32 == int32, in win64 == int64

    //  ----------------------------------  CONSTRUCT -----------------------------------------------------
    using Base::Base;             // прокидывание конструкторов базового класса
    using Base::npos;

protected:
    iterator* iterators_chain = nullptr;      // цепочка итераторов

public:
    template<class _Iter>
    using is_iterator8 = 
            std::bool_constant<is_any_of_v<remove_cvref_t<_Iter>, iterator, const_iterator>>;
    
    using it_ret = Retu8<basic_utf8string>;

    template <class Tsview>
    using Is_string_view =
        enable_if_t<conjunction_v<is_convertible<const Tsview&, Base_view>,
                         negation<is_convertible<const Tsview&, const _Elem*>>>, int>;
    //---------------------------------------------------------------------------------------------
    // конструкторы и операторы копирования и перемещения
    constexpr20
    basic_utf8string(const it_ret& re) : Base(re.string8()) {}
    //---------------------------------------------------------------------------------------------
    constexpr20
    basic_utf8string(const basic_utf8string& var) : Base(var) {}

    template<class Tsview, Is_string_view<Tsview> = 0>    // u8string_view, u8string
    constexpr20
    basic_utf8string(const Tsview& right) :  Base(right) {}
    //--------------------------------------------------------------------------------------------------------
    //                      string, string_view, wstring, wstring_view
    template<class T> requires is_class_v<T> && has_type_value_type<T>
                                             && is_same_v<T, basic_string<typename T::value_type>> 
                                             && is_any_of_v<typename T::value_type, char, wchar_t>
    constexpr20
    basic_utf8string(const T& var) : Base(_to_utf8(var)) {}
    
    template<class T> requires is_any_of_v<T, char, unsigned char, wchar_t>
    constexpr20
    basic_utf8string(const T* const p) : Base(_to_utf8(p)) {}
//============================================================================================================
    constexpr20                                                  // pos in SYMBOLS
    basic_utf8string(const basic_utf8string& right, const size_type pos)
                : Base(*(Base*)(&right), retPosSize(right.data(), pos) ) {
        // construct from right [pos, <end>)
    }
    constexpr20                                      // pos in SYMBOLS
    basic_utf8string(const Base& right, const size_type pos)
                : Base(right, retPosSize(right.data(), pos) ) {}
    // Base_view через basic_utf8string
          //--------------------------------------------------------------------------------------------------------
    template<class T> requires is_convertible_v<T, string_view>
    constexpr20                                                     // string, string_view
    basic_utf8string(const T& right, const size_type pos)
              : Base(string_to_utf8_ansi_tab_char(right, pos)) {}

    template<class T> requires is_convertible_v<T, wstring_view>
    constexpr20                                                     // wstring, wstring_view
    basic_utf8string(const T& right, const size_type pos)
              : Base(wstring_to_utf8_ansi(right, pos)) {}
//============================================================================================================
    template<class Ty, enable_if_t<is_convertible_v<const Ty&, Base_view>, int> = 0>
    constexpr20 explicit                                         // pos and count in SYMBOLS
    basic_utf8string(const Ty& right, const pair<const size_type, const size_type>& pa) 
                : Base(right, pa.first, pa.second) {
        // construct from right [first, first + second)
    }
    constexpr20                                                  // pos and count in SYMBOLS
    basic_utf8string(const basic_utf8string& right, const size_type pos, const size_type count) 
                :basic_utf8string(*((Base*)(&right)), retPosSize(right.data(), pos, count)) {
        // construct from right [pos, pos + count)
    }
    constexpr20                                      // pos and count in SYMBOLS
    basic_utf8string(const Base& right, const size_type pos, const size_type count) 
                : basic_utf8string(right, retPosSize(right.data(), pos, count)) {
        // construct from right [pos, pos + count)
    }
    
    template<class Ty, enable_if_t<is_convertible_v<const Ty&, Base_view> && !(is_same_v<Ty, Base>), int> = 0>
    constexpr20                                                                  // char8_t*, u8string_view
    basic_utf8string(const Ty& right, const size_type pos, const size_type count)
                : basic_utf8string(right, retPosSize(std::data(right), pos, count)) {}
    //----------------------------------------------------------------------------------------------------
    template<class T> requires is_convertible_v<T, string_view>
    constexpr20                                                                     // string, string_view
    basic_utf8string(const T& var, const size_type pos, const size_type count) 
              : Base(string_to_utf8_ansi_tab_char(var, pos, count)) {}

    template<class T> requires is_convertible_v<T, wstring_view>
    constexpr20                                                                     // wstring, wstring_view
    basic_utf8string(const T& var, const size_type pos, const size_type count) 
              : Base(wstring_to_utf8_ansi(var, pos, count)) {}

    template<class T> requires is_any_of_v<T, char, unsigned char>
    constexpr20
    basic_utf8string (const T* const p, const size_type pos, const size_type count) : 
                                Base(substr_string_to_utf8(p, pos, count)) {}

    template<class T> requires is_any_of_v<T, wchar_t>
    constexpr20
    basic_utf8string (const T* const p, const size_type pos, const size_type count) : 
                                Base(substr_wstring_to_utf8(p, pos, count)) {}
//============================================================================================================
    //                          ПУСТЬ ПОЛЬЗУЮТСЯ, ЕСЛИ ХОТЯТ  !!!!!!!!!!!!!!!!
    constexpr20                                          // count in SYMBOLS!!!!!!!!!
    basic_utf8string(const _Elem* const p, const size_type count)   // правильно ли установили ук-ль???
                : Base(p, retPosSize(p, count) ) {
        // construct from p [p, p+count)
        /*static_assert(is_integral_v<Tsize> && !(is_integral_v<Tsize>), 
        "!!!  To avoid ERRORS, do NOT use (pointer, count)."
        "\n                                      Use (pointer, pos, count) instead  !!!");
        */
    }
    template<class T> requires is_any_of_v<T, char, unsigned char>
    constexpr20
    basic_utf8string (const T* const p, const size_type count) : 
                                Base(substr_string_to_utf8(p, 0, count)) {}

    template<class T> requires is_any_of_v<T, wchar_t>
    constexpr20
    basic_utf8string (const T* const p, const size_type count) : 
                                Base(substr_wstring_to_utf8(p, 0, count)) {}
    //========================================================================================================
    //                                        char's
    constexpr20
    basic_utf8string(const _Elem ch) noexcept : Base(1, ch) {}
    
    template<class T> requires is_any_of_v<T, char, unsigned char>
    constexpr20
    basic_utf8string(const T ch) : Base(char_to_utf8_ansi_tab_char(ch)) {}

    constexpr20
    basic_utf8string(const wchar_t ch) : Base(wchar_to_utf8_ansi(ch)) {}
    
    template<class T> requires is_any_of_v<T, char, unsigned char>
    constexpr20
    basic_utf8string(const size_type count, const T ch) : Base(char_to_utf8_ansi_tab_char(count, ch)) {}

    constexpr20
    basic_utf8string(const size_type count, const wchar_t ch) : Base(wchar_to_utf8_ansi(count, ch)) {}
    //========================================================================================================
    constexpr20     //                                      utf8string::iterator 
    basic_utf8string(const iterator& first, const iterator& last) {
        _Adl_verify_range(first, last);
        Base::assign(first.rc_ptr(), static_cast<size_type>(diff_ptr(first, last)));
    }

    constexpr20                         //   one symbol from one iterator
    basic_utf8string(const const_iterator& one) : Base(one.rc_ptr(), utf8len(one.rc_ptr())) {}
    
    template<class _Iter, class Type = typename _Iter::value_type> 
    requires  is_iterator_v<_Iter> && is_class_v<_Iter> && is_any_of_v<Type, char, wchar_t>
    constexpr20 
    basic_utf8string(const _Iter first, const _Iter last) : Base(_to_utf8(basic_string<Type>(first, last))) {}
    
    template<class _Iter, class Type = typename _Iter::value_type> 
    requires  is_iterator_v<_Iter> && is_class_v<_Iter> && is_same_v<Type, unsigned char>
    constexpr20 
    basic_utf8string(const _Iter first, const _Iter last) : Base(_to_utf8(basic_string<char>(first, last))) {}
    
    template<class Iter, class Type = typename Iter::value_type> 
    requires  is_iterator_v<Iter> && is_class_v<Iter> && (!is_char_v<Type>) && convertible_to_utf8<Type>
    constexpr20   // итераторы контейнеров с char8_t*, char*, u8string, u8string_view, utf8string, string...
    basic_utf8string(Iter first, Iter last) {
        _Adl_verify_range(first, last);
        //ASSERT(first <= last);       // у list нет операторов <, <=, >, >=
        //            Iter=std::_List_iterator<std::_List_val<std::_List_simple_types<const char8_t *>>>
        if constexpr(std::is_pointer_v<Type> || std::is_class_v<Type>) {
            for(; first != last; ++first) {
                append(*first);
            }
        }
        else
            static_assert(is_integral_v<Iter> && (!is_integral_v<Iter>), "and what is it?");
    }
    //========================================================================================================
    constexpr20
    basic_utf8string(basic_utf8string&& right) noexcept : Base(std::move(right)) {}

    constexpr20
    basic_utf8string(Base&& right) :  Base(std::move(right)) {}
    //========================================================================================================
    constexpr20
    basic_utf8string& operator=(basic_utf8string&& right) noexcept {
        if(this != std::addressof(right)) {
            if(iterators_chain) iterators_chain = nullptr; // все итераторы к этому объекту становятся недействительными !!!
            *((Base*)this) = std::move(*(Base*)(&right));
        }
        return *this;
    }

    constexpr20
    basic_utf8string& assign(basic_utf8string&& right) noexcept(noexcept(*this = std::move(right))) {
        *this = std::move(right);
        return *this;
    }

    constexpr20
    basic_utf8string& operator=(Base&& right) {
         if(iterators_chain) iterators_chain = nullptr; // все итераторы к этому объекту становятся недействительными !!!
         *((Base*)this) = std::move(right);
        return *this;
    }
    //========================================================================================================
    //                                              initializer_list
    template<class T>
    requires convertible_to_utf8<T> && (!is_any_of_v<T, iterator, const iterator>)
    constexpr20
    basic_utf8string(initializer_list<T> _Ilist) noexcept {
        if constexpr(is_char_v<remove_const_t<T>>) { // char | uchar | wchar_t | const char8_t(not char8_t)
            *this = basic_utf8string(std::data(_Ilist), 0, std::size(_Ilist));
        }
        else { // (const)char*(const), (const)uchar*(const), (const)wchar_t*(const), (const)char8_t*(const),
               // string, string_view, wstring, wstring_view, u8string, u8string_view, utf8string, 
            for(const auto& x : _Ilist) append(x);
        }
    }

    constexpr20                                                  // а то будет неоднозначность
    basic_utf8string& assign(initializer_list<_Elem> _Ilist) {   
        if(iterators_chain) iterators_chain = nullptr; // все итераторы к этому объекту становятся недействительными !!!
        Base::assign(_Ilist);

        return *this;
    }

    constexpr20                                                    // а то будет неоднозначность
    basic_utf8string& append(std::initializer_list<_Elem> _Ilist) {
        Base::append(_Ilist);
        if(iterators_chain) iterators_chain = nullptr; // все итераторы к этому объекту становятся недействительными !!!
        return *this;
    }
 
    constexpr
    iterator insert(const const_iterator& where, const std::initializer_list<_Elem> _Ilist) {
        where._Compat(this->data());                 // it is in this string
        const auto save_capacity  = this->capacity();
        const difference_type pos = where.diff_ptr_in_symbols_;         // in symbols
        
        Base::insert(where.get_u8iterator(), _Ilist); 
        if(iterators_chain) iterators_chain = nullptr; // все итераторы к этому объекту становятся недействительными !!!
        
        if(this->capacity() == save_capacity)  return where;
        return begin() + pos;                 // O(n)
    }

    constexpr20
    basic_utf8string& replace(const const_iterator& first, const const_iterator& last, const initializer_list<_Elem> il) {
        // replace with initializer_list
        first._Compat(this->data());                 // first is in this string
        _Adl_verify_range(first, last);             // first and last owned the same line
        Base::replace(static_cast<size_type>(first.rc_ptr() - this->data()),      // off
                               static_cast<size_type>(diff_ptr(first, last)),     // count
                               std::data(il), std::size(il));
        if(iterators_chain) iterators_chain = nullptr; // все итераторы к этому объекту становятся недействительными !!!
        return *this;
    }
    //========================================================================================================
    constexpr20
    basic_utf8string& operator=(const basic_utf8string& right) {
        if(this != std::addressof(right)) {
            *((Base*)this) = *(Base*)(&right);
            if(iterators_chain) iterators_chain = nullptr; // все итераторы к этому объекту становятся недействительными !!!
        }
        return *this;
    }

    template<class Tsview, Is_string_view<Tsview> = 0>        // u8string, u8string_view, il<_Elem>
    constexpr20
    basic_utf8string& operator=(const Tsview& right) {
        if(iterators_chain) iterators_chain = nullptr; // все итераторы к этому объекту становятся недействительными !!!
        Base::operator=(right);
        return *this;
    }

    constexpr20
    basic_utf8string& operator=(const _Elem* const p) {
         if(iterators_chain) iterators_chain = nullptr; // все итераторы к этому объекту становятся недействительными !!!
         *((Base*)this) = p;
        return *this;
    }
    //-----------------------------------------------------------------------------
    template<class T> requires is_any_of_v<T, char, unsigned char, wchar_t>
    constexpr20
    basic_utf8string& operator=(const T* const p) { 
        *this = basic_utf8string(p); 
        return *this;
    }

#if _HAS_CXX23
    basic_utf8string& operator=(nullptr_t) = delete;
#endif // _HAS_CXX23
    
    constexpr20
    basic_utf8string& operator=(const _Elem ch) {
        if(iterators_chain) iterators_chain = nullptr; // все итераторы к этому объекту становятся недействительными !!!
        (*((Base*)this)) = ch;
        return *this;
    }
    
    template<class T> requires is_any_of_v<T, char, unsigned char, wchar_t>
    constexpr20
    basic_utf8string& operator=(const T ch) { 
        *this = basic_utf8string(ch); 
        return *this;
    }

    // char8_t*, One iterator, utf8string, u8string, u8string_view, wstring, wstring_view, string, string_view, 
    // initializer_list<_Elem>
    template<class Tstringv> 
    constexpr20
    basic_utf8string& operator+=(const Tstringv& s) noexcept { 
        return this->append(s); 
    }

    template<class T> requires is_any_of_v<T, char, unsigned char, wchar_t, char8_t>
    constexpr20
    basic_utf8string& operator+=(const T c) { 
        return this->append(c); 
    }
    //=========================================================================================================
    template<class T, class U = typename std::remove_const_t<std::remove_pointer_t<std::remove_const_t<T>>>>
    requires std::is_same_v<U, char8_t>
    constexpr20       //(char*, char const*, char const* const, ...)
    basic_utf8string(T* first, T* last) {
        static_assert(is_integral_v<T> && !(is_integral_v<T>),
            "To avoid ERRORS,"
            "\n                       do not use pointers to char8_t.\n"
            "                         Use std::utf8string::iterator's");
    }
    //========================================================================================================
    template<class T, class U = typename std::remove_const_t<std::remove_pointer_t<std::remove_const_t<T>>>>
    requires std::is_any_of_v<U, char, wchar_t, unsigned char> ||
                (std::is_class_v<T> && std::convertible_to_utf8<typename T::value_type>)
    constexpr20       //(char*, char const*, char const* const, ...)
    basic_utf8string(T* first, T* last) {
        _Adl_verify_range(first, last);
        
        if constexpr(std::is_char_v<T>) {  // il<char, wchar_t, ...>  char*, char const*, wchar_t*...
            using type_value = std::remove_const_t<T>;
            if constexpr(is_same_v<type_value, unsigned char>) {  // initializer_list<unsigned char>
                *this = (basic_string<char>( (char*)first, (char*)last) );
            }
            else {   // initializer_list<char | wchar_t>, 
                *this = basic_string<type_value>((type_value*)first, (type_value*)last);
            }
        }
        else {                  // T = char* , char const*,  uchar*, uchar const*, wchar_t*, wchar_t const*, ...
            for(; first < last; ++first) {
                if constexpr(is_class_v<T>) {
                    append(*first);
                }
                else { // initializer_list<char* | wchar_t* | const char* | const wchar_t* | unsigned char*>
                    append(std::data(*first));
                }
            }
        }
    }
    ///////////////////////////////////     Fill ...    //////////////////////////////////////////////////////
    constexpr20 
    basic_utf8string& fill(const size_type nums, const char c) noexcept { 
        return *this = char_to_utf8_ansi_tab_char(nums, c); 
    }
    /////////////////////////////////////////  MIX ///////////////////////////////////////////////////////////
    [[nodiscard]] inline
    std::string string()          const noexcept {
                return utf8_to_string_ansi(this->data());
    }

    operator Bstring()            const noexcept {  
        return string(); 
    }

    [[nodiscard]] inline
    std::wstring   wstring()        const noexcept { 
        return utf8_to_wstring(*this); 
    }
    
    operator  std::wstring()        const noexcept { 
        return wstring(); 
    }
    //                                      размер std::string строки (в символах)
    [[nodiscard]] constexpr 
    size_type u8size()              const noexcept { return ::u8size(this->data()); }

    [[nodiscard]] constexpr
    auto    size8()                 const noexcept  { return u8size(); }


    static constexpr 
    void check_offset(const size_type len, const size_type pos) { 
    // checks whether pos is in the bounds of [0, len()]
        ASSERTM(pos <= len, "invalid string position");
    }
    
    [[nodiscard]] static constexpr 
    size_type clamp_suffix_size(const size_type len, const size_type pos, const size_type count) noexcept {
        // trims count to the longest it can be assuming a string at/after pos
        return (std::min)(count, len - pos);
    }

    [[nodiscard]] static constexpr                        // from in SYMBOLS to in BYTES
    auto retPosSize(const _Elem* const p, size_type pos, size_type count) {
        const size_type len = ::u8size(p);   // in SYMBOLS
        check_offset(len, pos);
        count = clamp_suffix_size(len, pos, count);
        pos   = u8_distance(p, pos);                        // from in SYMBOLS to im BYTES

        return pair(pos, u8_distance(p+pos, count));        // in BYTES
    }
  
    [[nodiscard]] static constexpr               // count in SYMBOLS
    auto retPosSize(const _Elem* const p, size_type count) noexcept {
        const size_type len = ::u8size(p);    // in symbols
        count = clamp_suffix_size(len, 0, count);
        return u8_distance(p, count);
    }

private:
    [[nodiscard]] constexpr      // pos  in SYMBOLS
    auto retPosSize(const size_type pos) const {      
        const size_type len = u8size();    // in symbols
        check_offset(len, pos);
        return u8_distance(this->data(), pos);
    }
    [[nodiscard]] constexpr      // pos  in SYMBOLS
    auto retPosSize_nocheck(const size_type pos) const {      
        return u8_distance(this->data(), pos);
    }

public:
    //////////////////////////////////////  APPEND     ////////////////////////////////////////////////////
    constexpr20
    basic_utf8string& append(const basic_utf8string& s) {
        if(iterators_chain) iterators_chain = nullptr; // все итераторы к этому объекту становятся недействительными !!!
        Base::append(*(Base*)(&s));
        return *this;
    }
    
    constexpr20                                       // pos and count in symbols
    basic_utf8string& append(const basic_utf8string& right, size_type pos, size_type count = npos) {
        // append right [pos, pos + count)
        tie(pos, count) = retPosSize(right.data(), pos, count);
        
        if(iterators_chain) iterators_chain = nullptr; // все итераторы к этому объекту становятся недействительными !!!
        Base::append(*(Base*)(&right), pos, count);
        return *this;
    }

    template<class Tsview, Is_string_view<Tsview> = 0>
    constexpr20
    basic_utf8string& append(const Tsview& right) {    // u8string, u8string_view
        if(iterators_chain) iterators_chain = nullptr; // все итераторы к этому объекту становятся недействительными !!!
        Base::append(right);
        return *this;
    }

    template<class Tsview, Is_string_view<Tsview> = 0>  //u8string, u8string_view
    constexpr20                                       // pos and count in symbols
    basic_utf8string& append(const Tsview& right, size_type pos, size_type count = npos) {
        // append right [pos, pos + count)
        tie(pos, count) = retPosSize(right.data(), pos, count);
        
        if(iterators_chain) iterators_chain = nullptr; // все итераторы к этому объекту становятся недействительными !!!
        Base::append(right, pos, count);
        return *this;
    }

    template<class T> requires is_any_of_v<T, char, unsigned char, wchar_t, char8_t>
    constexpr20                                                            // count in symbols
    basic_utf8string& append(const T* const p, const size_type count) {    
        // append [p, p + count)
        if constexpr(is_same_v<T, char8_t>) {
            if(iterators_chain) iterators_chain = nullptr; // все итераторы к этому объекту становятся недействительными !!!
            Base::append(p, retPosSize(p, count));
            return *this;
        }
        return this->append(basic_utf8string(p, count));
    }

    template<class T> requires is_any_of_v<T, char, unsigned char, wchar_t, char8_t>
    constexpr20
    basic_utf8string& append(const T* const p) {  // append [p, <null>)
        if constexpr(is_same_v<T, char8_t>) {
            if(iterators_chain) iterators_chain = nullptr; // все итераторы к этому объекту становятся недействительными !!!
            Base::append(p);
            return *this;
        }
        return this->append(basic_utf8string(p));
    }

    template<class T> requires is_char_v<T> //is_any_of_v<T, char, unsigned char, wchar_t, char8_t>
    constexpr20
    basic_utf8string& append(const size_type count, const T ch) {
        // append count * ch
        if constexpr(is_same_v<T, char8_t>) {
            if(iterators_chain) iterators_chain = nullptr; // все итераторы к этому объекту становятся недействительными !!!
            Base::append(count, ch);
            return *this;
        }
        else
            return this->append(basic_utf8string(count, ch));
    }
    
    template<class T> requires is_char_v<T> //is_any_of_v<T, char, unsigned char, wchar_t, char8_t>
    constexpr20
    basic_utf8string& append(const T c) {
        if constexpr(is_same_v<T, char8_t>) {
            if(iterators_chain) iterators_chain = nullptr; // все итераторы к этому объекту становятся недействительными !!!
            Base::append(1, c);
            return *this;
        }
        else
            return this->append(basic_utf8string(c));
    }

    constexpr20
    basic_utf8string& append(const const_iterator& first, const const_iterator& last) {
        // append [first, last), input iterators
        _Adl_verify_range(first, last);
        if(iterators_chain) iterators_chain = nullptr; // все итераторы к этому объекту становятся недействительными !!!
        Base::append(first.rc_ptr(), static_cast<size_type>(diff_ptr(first, last)));
        return *this;
    }
    
    template<class T>
    requires std::is_same_v<T, char8_t>
    constexpr20
    basic_utf8string& append(const T* const first, const T* const last) {
        // append [first, last), poiters
        static_assert(is_integral_v<T> && !(is_integral_v<T>), 
        "!!!  To avoid ERRORS,"
        "\n                                      do NOT use (pointer, pointer)."
        "\n                                      Use (iterator8, iterator8) instead  !!!");
        //Base::append(first, last);
        //return *this;
    }
    
    template<class InputIterator> 
    requires is_iterator_only_v<InputIterator> && 
                (!is_any_of_v<remove_cvref_t<InputIterator>, iterator, const_iterator>)
    constexpr20
    basic_utf8string& append(const InputIterator first, const InputIterator last) {
        static_assert((!same_as<typename InputIterator::value_type, _Elem>), 
        "\n                             USE utf8string::iterator's INSTEAD OF (u8string,u8string_view)::iterator's");
        _Adl_verify_range(first, last);
        return this->append(basic_utf8string (first, last));
    }

    constexpr20
    basic_utf8string& append(const const_iterator& first) {
        if(iterators_chain) iterators_chain = nullptr; // все итераторы к этому объекту становятся недействительными !!!
        Base::append(first.rc_ptr());
        return *this;
    }
    
    constexpr20                              // count in SYMBOLS
    basic_utf8string& append(const const_iterator& it, const size_type count) {    
        if(iterators_chain) iterators_chain = nullptr; // все итераторы к этому объекту становятся недействительными !!!
        Base::append(it.rc_ptr(), retPosSize(it.rc_ptr(), count));
        return *this;
    }
    
    template<class _Iter>         //string, string_view, u8string...
    requires is_iterator_only_v<_Iter> && 
             (!is_any_of_v<remove_cvref_t<_Iter>, iterator, const_iterator>)
    constexpr20                     // count in bytes
    basic_utf8string& append(const _Iter it, const size_type count) {
        static_assert((!same_as<typename _Iter::value_type, _Elem>), 
        "\n                             USE utf8string::iterator's INSTEAD OF (u8string,u8string_view)::iterator's");

        if(iterators_chain) iterators_chain = nullptr; // все итераторы к этому объекту становятся недействительными !!!
        Base::append(basic_utf8string(it, it+count));
        return *this;
    }
    //////////////////////////////////////////  ASSIGN ////////////////////////////////////////////////////////
    constexpr20
    basic_utf8string& assign(const basic_utf8string& right) {
        *this = right;
        return *this;      
    }

    constexpr20             // pos and count in SYMBOLS
    basic_utf8string& assign(const basic_utf8string& right, size_type pos, size_type count = npos) {
        // assign right [pos, pos + count)
        tie(pos, count) = retPosSize(right.data(), pos, count);

        if(iterators_chain) iterators_chain = nullptr; // все итераторы к этому объекту становятся недействительными !!!
        Base::assign(*(Base*)(&right), pos, count);
        return *this;
    }

    template<class Tsview, Is_string_view<Tsview> = 0>
    constexpr20
    basic_utf8string& assign(const Tsview& right) {    // u8string, u8string_view
        if(iterators_chain) iterators_chain = nullptr; // все итераторы к этому объекту становятся недействительными !!!
        Base::assign(right);
        return *this;
    }

    constexpr20 
    basic_utf8string& assign(Base&& s) noexcept {
        if(iterators_chain) iterators_chain = nullptr; // все итераторы к этому объекту становятся недействительными !!!
        Base::assign(std::move(s));               // определен конструктор move
        return *this;
    }

    template<class Tsview, Is_string_view<Tsview> = 0>  //u8string, u8string_view
    constexpr20                                          // pos and count in symbols
    basic_utf8string& assign(const Tsview& right, size_type pos, size_type count = npos) {
        // assign right [pos, pos + count)
        tie(pos, count) = retPosSize(right.data(), pos, count);
        
        if(iterators_chain) iterators_chain = nullptr; // все итераторы к этому объекту становятся недействительными !!!
        Base::assign(right, pos, count);
        return *this;
    }

    constexpr20                                  // count in symbols
    basic_utf8string& assign(const _Elem* const ptr, const size_type count) {
        // assign [ptr, ptr + count)
        if(iterators_chain) iterators_chain = nullptr; // все итераторы к этому объекту становятся недействительными !!!
        Base::assign(ptr, retPosSize(ptr, count));
        return *this;
    }

    template<class T> requires std::is_any_of_v<T, char, unsigned char, wchar_t>
    constexpr20 
    basic_utf8string& assign(const T* const ptr, const size_type count) {
        *this = basic_utf8string(ptr, count);
        return *this;
    }

    constexpr20 
    basic_utf8string& assign(const _Elem* const ptr) {
        if(iterators_chain) iterators_chain = nullptr; // все итераторы к этому объекту становятся недействительными !!!
        Base::assign(ptr);
        return *this;
    } 

    template<class T> requires std::is_any_of_v<T, char, unsigned char, wchar_t>
    constexpr20 
    basic_utf8string& assign(const T* const ptr) {
        *this = basic_utf8string(ptr);
        return *this;//->assign(ptr, std::char_traits<char>::length((char*)ptr));
    }
    
    constexpr20      // pos and count in symbols
    basic_utf8string& assign(const _Elem* const ptr, size_type pos, size_type count) {
        tie(pos, count) = retPosSize(ptr, pos, count);
        
        if(iterators_chain) iterators_chain = nullptr; // все итераторы к этому объекту становятся недействительными !!!
        Base::assign(ptr + pos, count);
        return *this;
    }

    template<class T> requires std::is_any_of_v<T, char, unsigned char, wchar_t>
    constexpr20 
    basic_utf8string& assign(const T* const ptr, const size_type pos, const size_type count) {
        *this = basic_utf8string(ptr, pos, count);
        return *this;
    }

    constexpr20 
    basic_utf8string& assign(const size_type count, const char8_t ch) {
        if(iterators_chain) iterators_chain = nullptr; // все итераторы к этому объекту становятся недействительными !!!
        Base::assign(count, ch);
        return *this;
    }

    template<class T> requires std::is_any_of_v<T, char, unsigned char, wchar_t>
    constexpr20 
    basic_utf8string& assign(const size_type count, const T ch) {
        return this->assign(basic_utf8string(count, ch));
    }

    constexpr20 
    basic_utf8string& assign(const const_iterator& first, const const_iterator& last) {
        _Adl_verify_range(first, last);
        
        if(iterators_chain) iterators_chain = nullptr; // все итераторы к этому объекту становятся недействительными !!!
        Base::assign(first.rc_ptr(), static_cast<size_type>(diff_ptr(first, last)));
        return *this;
    }

    template<class InputIterator> 
    requires is_iterator_only_v<InputIterator>
    constexpr20 
    basic_utf8string& assign(const InputIterator first, const InputIterator last) {
        static_assert((!same_as<typename InputIterator::value_type, _Elem>), 
        "\n                             USE utf8string::iterator's INSTEAD OF (u8string,u8string_view)::iterator's");
       _Adl_verify_range(first, last);

        return this->assign(basic_utf8string (first, last));
    }

    template<class T>
    requires std::is_same_v<T, char8_t>
    constexpr20
    basic_utf8string& assign(const T* const first, const T* const last) {
        static_assert(is_integral_v<T> && !(is_integral_v<T>), 
        "!!!  To avoid ERRORS,"
        "\n                                      do NOT use (pointer, pointer)."
        "\n                                      Use (iterator8, iterator8) instead  !!!");
    }

    template<class T> requires std::is_any_of_v<T, char, unsigned char, wchar_t>
    constexpr20 
    basic_utf8string& assign(const T* const first, const T* const last) {
        *this = basic_utf8string(first, last);
        return *this;
    }
    //////////////////////////////////////  END ASSIGN ////////////////////////////////////////////////////
private:
    [[nodiscard]] constexpr                       // from in SYMBOLS to in BYTES
    auto retPosSize(size_type pos1, const _Elem* const p, size_type pos2, size_type count2) const noexcept {
        pos1 = retPosSize(pos1);
        tie(pos2, count2) = retPosSize(p, pos2, count2);
        return tuple(pos1, pos2, count2);
    }

public:
    //////////////////////////////////////  INSERT /////////////////////////////////////////////////////////
    template<class T> requires is_convertible_v<T, Base_view>
    constexpr20                // utf8string, u8string, u8string_view, char8_t*
    basic_utf8string& insert(size_type pos, const T& right) {        // pos in SYMBOLS
        // insert right at pos
        pos = retPosSize(pos);     // from SYMBOLS to in BYTES

        if(iterators_chain) iterators_chain = nullptr; // все итераторы к этому объекту становятся недействительными !!!

        if constexpr(is_any_of_v<T, Base_view, Base>)
            Base::insert(pos, right);
        else  
        if constexpr(is_same_v<T, basic_utf8string>) {
            Base::insert(pos, *((Base*)&right));
        }
        else // char8_t*
            Base::insert(pos, right);

        return *this;
    }

    template<class T>       //  utf8string, u8string, u8string_view, char8_t*
    requires (!is_pointer_v<T>) && is_convertible_v<T, Base_view>
    constexpr20                   // pos, rpos, count in SYMBOLS
    basic_utf8string& insert(size_type pos, const T& right, size_type rpos, size_type count = npos) {
        // insert right [rpos, rpos + count) at pos
        tie(pos, rpos, count) = retPosSize(pos, std::data(right), rpos, count);

        if(iterators_chain) iterators_chain = nullptr; // все итераторы к этому объекту становятся недействительными !!!

        if constexpr(is_any_of_v<T, Base_view, Base>) {
            Base::insert(pos, right, rpos, count);
        }
        else
        if constexpr(is_same_v<T, basic_utf8string>) {
            Base::insert(pos, *((Base*)&right), rpos, count);
        }
        else 
            static_assert(is_integral_v<T> && (!is_integral_v<T>), "And what is it?");
        return *this;
    }
    //                      string_view, string, char*
    template<class T> requires is_convertible_v<T, string_view> && (!is_same_v<T, basic_utf8string>)
    constexpr20 
    basic_utf8string& insert(const size_type pos, const T& right, const size_type rpos, 
                                                               const size_type count = npos) {
        if constexpr(is_same_v<T, string_view>)
            return this->insert(pos, basic_utf8string(right, rpos, count));
        else
            return this->insert(pos, basic_utf8string(string_view(right), rpos, count));
    }
    //                      wstring_view, wstring, wchar_t*
    template<class T> requires is_convertible_v<T, std::wstring_view> && (!std::is_same_v<T, basic_utf8string>)
    constexpr20 
    basic_utf8string& insert(const size_type pos, const T& right, const size_type rpos, 
                                                               const size_type count = npos) {
        if constexpr(is_same_v<T, wstring_view>)
            return this->insert(pos, basic_utf8string(right, rpos, count));
        else
            return this->insert(pos, basic_utf8string(wstring_view(right), rpos, count));
    }
    
    constexpr20 
    basic_utf8string& insert(const size_type pos, const _Elem* p, const size_type count) {
        // insert [p, p + count) at pos
        if(iterators_chain) iterators_chain = nullptr; // все итераторы к этому объекту становятся недействительными !!!
        Base::insert(retPosSize(pos), p, retPosSize(p, count));
        return *this;
    }

    template<class T> requires is_any_of_v<T, char, unsigned char, wchar_t, char8_t>
    constexpr20 
    basic_utf8string& insert(const size_type pos, const size_type count, const T ch) {
        // insert count * ch at pos
        if constexpr(is_same_v<T, char8_t>) {
            if(iterators_chain) iterators_chain = nullptr; // все итераторы к этому объекту становятся недействительными !!!
            Base::insert(retPosSize(pos), count, ch);
            return *this;
        }
        else
            return this->insert(pos, basic_utf8string(count, ch));//, npos);
    }
    //--------------------------------------------------------------------------------------------
    //                                  with iterators
    template<class T> requires std::is_char_v<T>
    constexpr
    iterator insert(const const_iterator& where, const T ch) {   // insert ch at where
        where._Compat(this->data());                 // where is in this string
        if constexpr(is_same_v<T, char8_t>) {
            const auto save_capacity  = this->capacity();
            const difference_type pos = where.diff_ptr_in_symbols_;      // in symbols

            if(iterators_chain) iterators_chain = nullptr; // все итераторы к этому объекту становятся недействительными !!!
            Base::insert(where.get_u8iterator(), ch); 

            if(this->capacity() == save_capacity) return where;
            return begin() + pos;
        }
        else {
            const basic_utf8string tmp(ch);          // char, wchar_t
            return this->insert(where, tmp.cbegin(), tmp.cend());
        }
    }

    template<class T> requires std::is_char_v<T>
    constexpr
    iterator insert(const const_iterator& where, const size_type count, const T ch) {
        // insert count * is_char at where
        where._Compat(this->data());                 // where is in this string
        if constexpr(is_same_v<T, char8_t>) {
            const auto save_capacity  = this->capacity();
            const difference_type pos = where.diff_ptr_in_symbols_;      // in symbols

            if(iterators_chain) iterators_chain = nullptr; // все итераторы к этому объекту становятся недействительными !!!
            Base::insert(where.get_u8iterator(), count, ch); 

            if(this->capacity() == save_capacity) return where;
            return begin() + pos;
        }
        else {
            const basic_utf8string tmp(count, ch);
            return this->insert(where, tmp.cbegin(), tmp.cend());
        }
    }
        
    constexpr
    iterator insert(const const_iterator& where, const const_iterator& first, const const_iterator& last) {
        // insert [first, last) at where, input iterators
        where._Compat(this->data());                 // where is in this string
        _Adl_verify_range(first, last);         // first and last is same string, first <= last
        
        const auto save_capacity  = this->capacity();
        const auto off            = static_cast<size_type>(where.rc_ptr() - this->data());      // in bytes
        const difference_type pos = where.diff_ptr_in_symbols_;                                 // in symbols

        if(iterators_chain) iterators_chain = nullptr; // все итераторы к этому объекту становятся недействительными !!!
        Base::insert(off, first.rc_ptr(), static_cast<size_type>(diff_ptr(first, last))); 
        
        if(this->capacity() == save_capacity) return where;
        return begin() + pos;         // O(n)
    }
    //                      InputIterator  string, string_view, wstring, wstring_view
    template<class InputIterator, class value_type=typename InputIterator::value_type> 
    requires is_iterator_v<InputIterator> && is_class_v<InputIterator> && is_char_v<value_type>
    constexpr
    iterator insert(const const_iterator& where, const InputIterator first, const InputIterator last) {
        // insert [first, last) at where, input iterators
        static_assert((!is_any_of_v<InputIterator, Base::iterator, Base_view::iterator, 
                                                Base::const_iterator, Base_view::const_iterator>),
        "\n                     USE utf8string::iterator  INSTEAD OF (u8string,u8string_view)::iterators");
        where._Compat(this->data());                 // it is in this string
        _Adl_verify_range(first, last);

        const auto save_capacity  = this->capacity();
        const difference_type pos = where.diff_ptr_in_symbols_;         // in symbols

        this->insert(pos, basic_utf8string(first, last)); 
        
        if(this->capacity() == save_capacity) return where;
        return begin() + pos;         // O(n)
    }
    // -                        вст. 1-го символа, заданного итератором2,  в позицию, заданную итераторм1.
    constexpr
    iterator insert(const const_iterator& where, const const_iterator& other) {
        return this->insert(where, other, other+1);   
    }
    //----------------------------------------------------------------------------
    constexpr
    iterator insert(const const_iterator& where, const basic_utf8string& str) {
        return this->insert(where, str.cbegin(), str.cend()); 
    }
    /////////////////////////////////////   ERASE   ///////////////////////////////////////////////////////
    // при erase выполняется move symbols
private:
    [[nodiscard]] constexpr                              //  from in symbols to in bytes
    auto retPosSize(size_type pos1, size_type count1, const _Elem* const p, 
                                                size_type pos2, size_type count2) const noexcept {
        tie(pos1, count1) = retPosSize(this->data(), pos1, count1);
        tie(pos2, count2) = retPosSize(p, pos2, count2);
        return tuple(pos1, count1, pos2, count2);
    }

public:
    constexpr20        // pos in symbols
    basic_utf8string& erase(const size_type pos = 0)  { // erase elements [pos, ...)
        if(pos >= u8size()) return *this;   // предотвратить выход за предел строки
        
        if(iterators_chain) iterators_chain = nullptr; // все итераторы к этому объекту становятся недействительными !!!
        Base::erase(retPosSize(pos));
        return *this;
    }

    constexpr20           // pos1 and count1 in SYMBOLS
    basic_utf8string& erase(size_type pos, size_type count) { // erase elements [pos, count)
        tie(pos, count) = retPosSize(this->data(), pos, count);  // from in symbols to in bytes

        if(iterators_chain) iterators_chain = nullptr; // все итераторы к этому объекту становятся недействительными !!!
        Base::erase(pos, count);
        return *this;
    }

    constexpr                                                    //   erase 1 element
    iterator erase(const iterator& where) {
        where._Compat(this->data());                 // where is in this string
        if(iterators_chain) iterators_chain = nullptr; // все итераторы к этому объекту становятся недействительными !!!
        Base::erase(static_cast<size_type>(where.rc_ptr() - this->data()),        // off
                            utf8len(where.rc_ptr()));                                      // count
        return where;
    }
    
    constexpr
    iterator erase(const iterator& first, const iterator& last) {
        first._Compat(this->data());                 // first is in this string
        _Adl_verify_range(first, last);             // first and last owned the same line

        if(iterators_chain) iterators_chain = nullptr; // все итераторы к этому объекту становятся недействительными !!!
        Base::erase(static_cast<size_type>(first.rc_ptr() - this->data()),             // off
                             static_cast<size_type>(diff_ptr(first, last)));                    // count
        return first;
    }
    //////////////////////////////////////////   REPLACE   //////////////////////////////////////////////////
    constexpr20                                               // pos count  in SYMBOLS
    basic_utf8string& replace(size_type pos, size_type count, const basic_utf8string& right) { 
        // replace [pos, pos + count) with right
        tie(pos, count) = retPosSize(this->data(), pos, count);

        if(iterators_chain) iterators_chain = nullptr; // все итераторы к этому объекту становятся недействительными !!!
        Base::replace(pos, count, *(Base*)(&right));
        return *this;
    }

    constexpr20        // pos, count, rpos, rcount  in SYMBOLS 
    basic_utf8string& replace(size_type pos, size_type count, const basic_utf8string& right, 
                              size_type rpos, size_type rcount = npos) {
        // replace [pos, pos + count) with right [rpos, rpos + rcount)
        tie(pos, count, rpos, rcount) = retPosSize(pos, count, right.data(), rpos, rcount);

        if(iterators_chain) iterators_chain = nullptr; // все итераторы к этому объекту становятся недействительными !!!
        Base::replace(pos, count, right.data() + rpos, rcount);
        return *this;
    }

    template<class Tsview, Is_string_view<Tsview> = 0>    // u8string_view, u8string
    constexpr20                                               // _pos1, n1  in symbols 
    basic_utf8string& replace(size_type pos, size_type count, const Tsview& right) { 
        // replace [pos, pos + count) with right
        tie(pos, count) = retPosSize(this->data(), pos, count);

        if(iterators_chain) iterators_chain = nullptr; // все итераторы к этому объекту становятся недействительными !!!
        Base::replace(pos, count, right);
        return *this;
    }

    template<class Tsview, Is_string_view<Tsview> = 0>    // u8string_view, u8string
    constexpr20        // pos1, n1, pos2, m2  in symbols 
    basic_utf8string& replace(size_type pos, size_type count, 
                                const Tsview& right, size_type rpos, size_type rcount = npos) {
        // replace [pos, pos + count) with right [rpos, rpos + count)
        tie(pos, count, rpos, rcount) = retPosSize(pos, count, std::data(right), rpos, rcount);

        if(iterators_chain) iterators_chain = nullptr; // все итераторы к этому объекту становятся недействительными !!!
        Base::replace(pos, count, std::data(right) + rpos, rcount);
        return *this;
    }

    constexpr20                      // pos count rcount in symbols 
    basic_utf8string& replace(size_type pos, size_type count, const _Elem* const p, size_type rcount) { 
        // replace [pos, pos + count) with [p, p + rcount)
        tie(pos, count) = retPosSize(this->data(), pos, count);

        if(iterators_chain) iterators_chain = nullptr; // все итераторы к этому объекту становятся недействительными !!!
        Base::replace(pos, count, p, retPosSize(p, rcount));
        return *this;
    }

    constexpr20                     // pos and count in symbols 
    basic_utf8string& replace(size_type pos, size_type count, const _Elem* const p) { 
        // replace [pos, pos + count) with [p, <null>)
        tie(pos, count) = retPosSize(this->data(), pos, count);

        if(iterators_chain) iterators_chain = nullptr; // все итераторы к этому объекту становятся недействительными !!!
        Base::replace(pos, count, p);
        return *this;
    }

    template<class T> requires is_any_of_v<T, char, unsigned char, wchar_t, char8_t>
    constexpr20    //        pos, count in symbols
    basic_utf8string& replace(size_type pos, size_type count, const size_type rcount, const T ch) {
        // replace [pos, pos + count) with rcount * ch
        if constexpr(is_same_v<T, char8_t>) {
            tie(pos, count) = retPosSize(this->data(), pos, count);
            
            if(iterators_chain) iterators_chain = nullptr; // все итераторы к этому объекту становятся недействительными !!!
            Base::replace(pos, count, rcount, ch);
            return *this;  
        }
        else
            return this->replace(pos, count, basic_utf8string(rcount,  ch));
    }
    //-------------------------------------------------------------------------------------------------
private:
#if _ITERATOR_DEBUG_LEVEL >= 1
    constexpr void _Verify_ptr(const _Elem* const ptr) const {
        ASSERTM(ptr, "cannot seek value-initialized pointer");    // nullptr
        ASSERTM(ptr >= data() && ptr <= data() + size(),
                                              "cannot seek pointer because the pointer was"
                             " invalidated (e.g. reallocation occurred, or the string was destroyed)"
                             " or it is out of range");
#else // ^^^ _ITERATOR_DEBUG_LEVEL >= 1 ^^^ // vvv _ITERATOR_DEBUG_LEVEL == 0 vvv
    constexpr void _Verify_ptr() const noexcept {
#endif // _ITERATOR_DEBUG_LEVEL >= 1
    }
protected:     // используются в классе итераторов( used in class Iterator8 )
    //                                                          когда известны длины СИМВОЛОВ !!!!
    constexpr20                 // size_where and size_pntr в БАЙТАХ!!!!
    basic_utf8string& replace(const _Elem* const where, const size_type size_where, const _Elem* const pntr, 
                                                        const size_type size_pntr) { 
        if(size_where == size_pntr) 
            std::char_traits<char8_t>::copy((char8_t*)where, pntr, size_where); 
        else
            Base::replace(where - this->data(), size_where, pntr, size_pntr);
        return *this;
    }
    //              замена одного символа  символом , на который указывает указатель
    constexpr20                     // Oh there may be mistakes, so we will do private.
    basic_utf8string& replace(const _Elem* const where, const _Elem* const pntr) { 
        return this->replace(where, len8(*where), pntr, len8(*pntr));
    }

public:

    template<class Tsview, Is_string_view<Tsview> = 0>    // utf8string, u8string_view, u8string
    constexpr20
    basic_utf8string& replace(const const_iterator& first, const const_iterator& last, const Tsview& right) {
        // replace [first, last) with right
        first._Compat(this->data());
        _Adl_verify_range(first, last);

        if(iterators_chain) iterators_chain = nullptr; // все итераторы к этому объекту становятся недействительными !!!
        Base::replace(static_cast<size_type>(first.rc_ptr() - this->data()),  // off
                               static_cast<size_type>(diff_ptr(first, last)),        // count
                               right.data());
        return *this;
    }

    constexpr20
    basic_utf8string& replace(const const_iterator& first, const const_iterator& last, 
                                        const _Elem* const p, size_type count) {
        // replace [_First, _Last) with [p, p + count)
        first._Compat(this->data());
        _Adl_verify_range(first, last);

        count    = retPosSize(p, count);

        if(iterators_chain) iterators_chain = nullptr; // все итераторы к этому объекту становятся недействительными !!!
        Base::replace(static_cast<size_type>(first.rc_ptr() - this->data()),      // off in bytes
                               static_cast<size_type>(diff_ptr(first, last)),     // count in bytes
                                 p, count);
        return *this;
    }

    constexpr20
    basic_utf8string& replace(const const_iterator& first, const const_iterator& last, 
                                        const _Elem* const p) {
        // replace [first, last) with [p, <null>)
        first._Compat(this->data());
        _Adl_verify_range(first, last);

        if(iterators_chain) iterators_chain = nullptr; // все итераторы к этому объекту становятся недействительными !!!
        Base::replace(static_cast<size_type>(first.rc_ptr() - this->data()),      // off in bytes
                               static_cast<size_type>(diff_ptr(first, last)),     // count in bytes
                                 p);
        return *this;
    }

    template<class T> requires is_any_of_v<T, char, unsigned char, wchar_t, char8_t>
    constexpr20
    basic_utf8string& replace(const const_iterator& first, const const_iterator& last, 
                                                              const size_type count, const T ch) {
        // replace [first, last) with count * ch
        first._Compat(this->data());
        _Adl_verify_range(first, last);

        if constexpr(is_same_v<T, char8_t>) {
            if(iterators_chain) iterators_chain = nullptr; // все итераторы к этому объекту становятся недействительными !!!
            Base::replace(static_cast<size_type>(first.rc_ptr() - this->data()),      // off in bytes
                               static_cast<size_type>(diff_ptr(first, last)),     // count in bytes
                               count, ch);
           return *this;  
        }
        return this->replace(first, last, basic_utf8string(count, ch));
    }

    template<class InputIterator> requires is_iterator_v<InputIterator>  && (!is_iterator8<InputIterator>::value)
    constexpr20
    basic_utf8string& replace(const const_iterator& first, const const_iterator& last, 
                                                                const InputIterator j1, const InputIterator j2) {
        // replace [first, last) with [j1, j2), input iterators
        static_assert((!is_any_of_v<InputIterator, Base::iterator, Base_view::iterator, 
                                                Base::const_iterator, Base_view::const_iterator>),
        "\n                     USE utf8string::iterator  INSTEAD OF (u8string,u8string_view)::iterators");
       _Adl_verify_range(j1, j2);
       return this->replace(first, last, basic_utf8string (j1, j2));
    }

    constexpr20
    basic_utf8string& replace(const const_iterator& first, const const_iterator& last, 
                              const const_iterator& j1,    const const_iterator& j2) {
        // replace [first, last) with [j1, j2), iterators
       first._Compat(this->data());
       _Adl_verify_range(first, last);
       _Adl_verify_range(j1, j2);
       
        if(iterators_chain) iterators_chain = nullptr; // все итераторы к этому объекту становятся недействительными !!!
        Base::replace(static_cast<size_type>(first.rc_ptr() - this->data()),      // off in bytes
                               static_cast<size_type>(diff_ptr(first, last)),     // count in bytes
                                 j1.rc_ptr(), static_cast<size_type>(diff_ptr(j1, j2)));
    }
    //------------------------------------------------------------------------------------------------
    //                      - замена символа, заданного итератором 1, символом, заданным итератором 2.
    constexpr20
    basic_utf8string& replace(const const_iterator& i1, const const_iterator& i2) {
        i1._Compat(this->data());
        // i2 может быть совсем от другой строки, поэтому проверить нет возможности
        return this->replace(i1.rc_ptr(), i2.rc_ptr());
    }

    constexpr20                    // pos in symbols 
    basic_utf8string& replace(const size_type pos, const const_iterator& ci) {
        return this->replace(this->data()+retPosSize(pos), ci.rc_ptr());
    }
    //////////////////////////////////////  END REPLACES  /////////////////////////////////////////////////
    [[nodiscard]] constexpr 
    iterator begin() noexcept {
        return iterator(this->data(), *this);
    }
    [[nodiscard]] constexpr 
    iterator begin() const noexcept {
        return iterator(this->data(), *this);
    }
    [[nodiscard]] constexpr 
    const_iterator cbegin() const noexcept {
        return const_iterator(std::remove_const_t<pointer>(this->data()), *this);
    }
    [[nodiscard]] constexpr 
    iterator end() noexcept {
        return iterator(this->data() + this->size(), *this);
    }
    [[nodiscard]] constexpr 
    iterator end() const noexcept {
        return iterator(this->data() + this->size(), *this);
    }
    [[nodiscard]] constexpr 
    const_iterator cend() const noexcept {
        return const_iterator(std::remove_const_t<pointer>(this->data()) + this->size(), *this);
    }
    
    [[nodiscard]] constexpr
    reverse_iterator rbegin() noexcept {
        return reverse_iterator{end()};
    } 
    [[nodiscard]] constexpr 
    reverse_iterator rbegin() const noexcept {
        return reverse_iterator{end()};
    }
    [[nodiscard]] constexpr 
    const_reverse_iterator crbegin() const noexcept {
        return const_reverse_iterator{cend()};
    }
    [[nodiscard]] constexpr 
    reverse_iterator rend() noexcept {
        return reverse_iterator{begin()};
    } 
    [[nodiscard]] constexpr 
    reverse_iterator rend() const noexcept {
        return reverse_iterator{begin()};
    }
    [[nodiscard]] constexpr 
    const_reverse_iterator crend() const noexcept {
        return const_reverse_iterator{cbegin()};
    } 
    //---------------------------------------------------------------------------------------------------------
    [[nodiscard]]                    //pos in SYMBOLS
    const_reference at(const size_type pos) const {       // const _Elem*
       return this->data() + retPosSize(pos);   //
    }
    [[nodiscard]]                               //pos in SYMBOLS
    basic_utf8string  operator[] (const size_type pos) const { 
#if _CONTAINER_DEBUG_LEVEL > 0
        ASSERTM(pos <= u8size(), "string subscript out of range");
#endif // _CONTAINER_DEBUG_LEVEL > 0
       return ret_string(this->data() + retPosSize_nocheck(pos));   //
    } 
    //=========================================================================================================
    template<class T> requires is_any_of_v<T, char, unsigned char, wchar_t, char8_t>
    constexpr20
    basic_utf8string& push_back(const T c) noexcept {                     // insert element at end
        return this->append(c); 
    }
    
    constexpr20
    void pop_back() noexcept {
        const size_type _Old_size = size();
#if _ITERATOR_DEBUG_LEVEL >= 1
        ASSERTM(_Old_size != 0, "invalid to pop_back empty string");
#endif // _ITERATOR_DEBUG_LEVEL >= 1
        Base::resize(_Old_size - u8_distance(this->data() + this->size(), -1));
        if(iterators_chain) iterators_chain = nullptr; // все итераторы к этому объекту становятся недействительными !!!
    }

    [[nodiscard]] constexpr20
    const_reference front() const {   // const char8_t*
#if _CONTAINER_DEBUG_LEVEL > 0
        ASSERTM(size() != 0, "front() called on empty string");
#endif // _CONTAINER_DEBUG_LEVEL > 0
        return at(0);
    }
 
    [[nodiscard]] constexpr20
    const_reference back() const {      // const char8_t*
#if _CONTAINER_DEBUG_LEVEL > 0
        ASSERTM(size() != 0, "back() called on empty string");
#endif // _CONTAINER_DEBUG_LEVEL > 0
        return at(size8()-1);
    }
    //========================================================================================================
    constexpr20                             // count , pos in SYMBOLS
    size_type copy(_Elem* const ptr, size_type count, size_type pos = 0) const {
        // copy [pos, pos + count) to [ptr, ptr + count)
        tie(pos, count) = retPosSize(this->data(), pos, count);
        return Base::copy(ptr, count, pos);  // in BYTES
    }

    constexpr20 
    void swap(basic_utf8string& right) noexcept {
        if(this != std::addressof(right)) {
            (*((Base*)this)).swap(*(Base*)(&right));
            right.iterators_chain = this-> iterators_chain = nullptr;
        }
     }
    // char8_t*, One iterator, utf8string, u8string, u8string_view, wstring, wstring_view, string, string_view, 
    // initializer_list<_Elem>
    template<class Tstringv> 
    constexpr20
    basic_utf8string& push_back(const Tstringv& s) noexcept { 
        return this->append(s);
    }
    //////////////////////////////////////  FIND   ////////////////////////////////////////////////////////
private:
    [[nodiscard]] constexpr
    auto f_retPosSize(const size_type pos) const noexcept {      // pos = in symbols => pos in bytes
        return u8_distance(this->data(), pos);
    }

public:
    [[nodiscard]] static constexpr
    auto f_retPosSize(const _Elem* const p, const size_type pos) noexcept {      // pos = in symbols => pos in bytes
        return u8_distance(p, pos);
    }

private:
    [[nodiscard]] constexpr          // from in_SYMBOLS to in_BYTES
    auto f_retPosSize(const _Elem* const p, size_type pos, size_type count) const noexcept {
        if(count > ::u8size(p)) count = npos;
        else                    count = u8_distance(p, count);
        pos  = u8_distance(this->data(), pos);
        return pair(pos, count);
    }
    [[nodiscard]] constexpr                     // from in_SYMBOLS to in_BYTES
    auto f_retPosSize_r(const _Elem* const p, size_type pos, size_type count) const {
        //count = (std::min)(count, ::u8size(p));
        count = u8_distance(p, count);          // count in symbols -> in bytes
        
        if(pos > this->u8size()) pos = npos;
        else                     pos =  u8_distance(this->data(), pos);   // pos  in symbols -> in bytes  
        return pair(pos, count);
    }
public:
    // Implicitly converts t to a string view sv as if by std::basic_string_view<CharT, Traits> sv = t;, then 
    // finds the first substring equal to sv.
    template<class Tsview, Is_string_view<Tsview> = 0>    // u8string_view, u8string, utf8string
    [[nodiscard]] constexpr
    size_type find(const Tsview& right, size_type pos = 0) const 
                                       noexcept(is_nothrow_convertible_v<const Tsview&, u8string_view>) {
        // look for right beginning at or after pos
        if((pos + ::u8size(right)) > this->u8size()) return npos;

        pos = f_retPosSize(pos);
        const Base_view as_view = right;
        return cnv_pos_byte_sym(data(), Base::find(as_view, pos));      // from size in BYTES to size in SYMBOLS
    }

    [[nodiscard]] constexpr     // pos and count in SYMBOLS
    size_type find(const _Elem* const ptr, size_type pos, size_type count) const noexcept {
        // look for [ptr, ptr + count) beginning at or after pos
        if(pos + count > this->u8size()) return npos;
        
        tie(pos, count) = f_retPosSize(ptr, pos, count);
        return cnv_pos_byte_sym(data(), Base::find(ptr, pos, count));      // from size in BYTES to size in SYMBOLS
    }

    [[nodiscard]] constexpr 
    size_type find(const _Elem* const ptr, size_type pos = 0) const noexcept {
        // look for [ptr, <null>) beginning at or after pos
        if(pos + ::u8size(ptr) > this->u8size()) return npos;

        pos = f_retPosSize(pos);
        return cnv_pos_byte_sym(data(), Base::find(ptr, pos));      // from size in BYTES to size in SYMBOLS
    }

    [[nodiscard]] constexpr
    size_type find(const _Elem ch, size_type pos = 0) const noexcept {
        if(pos >= this->u8size()) return npos;

        pos = f_retPosSize(pos);
        return cnv_pos_byte_sym(data(), Base::find(ch, pos));      // from size in BYTES to size in SYMBOLS
        
    }

    template<class Tsview, Is_string_view<Tsview> = 0>    // u8string_view, u8string, utf8string
    [[nodiscard]] constexpr                             // pos in SYMBOLS
    size_type rfind(const Tsview& right, size_type pos = npos) const 
                                       noexcept(is_nothrow_convertible_v<const Tsview&, u8string_view>)    {
        // look for right beginning before pos
        if(right.empty()) {
            if(pos == npos) return this->u8size();
            return pos;
        }
        if(pos == 0 || ::u8size(right) > this->u8size()) return npos;
        if(pos != npos) pos = f_retPosSize(pos);
        
        const Base_view as_view = right;
        return cnv_pos_byte_sym(data(), Base::rfind(as_view, pos));      // from size in BYTES to size in SYMBOLS
    }

    [[nodiscard]] constexpr
    size_type rfind(const _Elem* const ptr, size_type pos, size_type count) const noexcept {
        if(pos == 0 || count > this->u8size()) return npos;
        tie(pos, count) = f_retPosSize_r(ptr, pos, count);
        return cnv_pos_byte_sym(data(), Base::rfind(ptr, pos, count));      // from size in BYTES to size in SYMBOLS
    } 

    [[nodiscard]] constexpr 
    size_type rfind(const _Elem* const ptr, size_type pos = npos) const noexcept {
        if(ptr == nullptr || *ptr == _Elem{}) {                // empty
            if(pos == npos) return this->u8size();
            return pos;
        }
        
        if(pos == 0 || ::u8size(ptr) > this->u8size()) return npos;
        if(pos != npos) pos = f_retPosSize(pos);

        return cnv_pos_byte_sym(data(), Base::rfind(ptr, pos));      // in symbols
    }
 
    [[nodiscard]] constexpr
    size_type rfind(const _Elem ch, size_type pos = npos) const noexcept {
        if(pos == 0) return npos;

        if(pos != npos) pos = f_retPosSize(pos);
        return cnv_pos_byte_sym(data(), Base::rfind(ch, pos));      // from size in BYTES to size in SYMBOLS
    }
    //---------------------------------------------------------------------------------
    // u8string::find_first_of не ищет символы, размером больше 1-го байта !!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!
    // !!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!
    //------------------------------------------------------------------------------------------
    template<class Tsview, Is_string_view<Tsview> = 0>    // u8string_view, u8string, utf8string
    [[nodiscard]] static constexpr                     // count in SYMBOLS
    int matching_one_symbol_and_sequence(const _Elem* const p, const size_type count,  const Tsview& s)
        noexcept(is_nothrow_convertible_v<const Tsview&, u8string_view>) {
        const Base_view as_view = s;
        return cnv_pos_byte_sym(as_view.data(), as_view.find(p, 0, count));      // from size in BYTES to size in SYMBOLS
    }

private:
    [[nodiscard]] constexpr                     // off in BYTES
    auto mfind_first_of(const size_type off, const Base_view right)  const noexcept {
        for(auto match_ptr = data()+off;  match_ptr < data() + size(); ) {
            const auto len = u8len(match_ptr);
            if(matching_one_symbol_and_sequence(match_ptr, len, right) != npos) {
                return cnv_pos_byte_sym(data(), match_ptr - data());
            }
            match_ptr += len;
        }
        return npos; // no match
    }

public:

    template<class Tsview, Is_string_view<Tsview> = 0>    // u8string_view, u8string, utf8string
    [[nodiscard]] constexpr                             // pos in SYMBOLS
    size_type find_first_of(const Tsview& right, size_type pos = 0) const
                                        noexcept(is_nothrow_convertible_v<const Tsview&, u8string_view>) {
        // look for one of right at or after pos
        if(pos >= this->u8size() || right.empty()) return npos;

        return mfind_first_of(f_retPosSize(pos), right);
    }

    [[nodiscard]] constexpr                             // pos and count in SYMBOLS
    size_type find_first_of(const _Elem* const ptr, size_type pos, size_type count) const noexcept {
        // look for one of [ptr, ptr + count) at or after pos
        if(ptr == nullptr || *ptr == _Elem{}) return npos;       // empty
        const auto count8 = ::u8size(ptr);
        if(pos >= this->u8size()) return npos;
        
        tie(pos, count) = f_retPosSize(ptr, pos, (std::min)(count, count8));  // count from ptr, pos from this->data()

        return mfind_first_of(pos, {ptr, count});
    }

    [[nodiscard]] constexpr                             // pos in SYMBOLS
    size_type find_first_of(const _Elem* const ptr, size_type pos = 0) const noexcept  {
        // look for one of [ptr, <null>) at or after pos
        if(ptr == nullptr || *ptr == _Elem{}) return npos; // empty
        if(pos >= this->u8size()) return npos;

        return mfind_first_of(f_retPosSize(pos), ptr);
    }

    [[nodiscard]] constexpr               // pos in SYMBOLS
    size_type find_first_of(const _Elem ch, size_type pos = 0) const noexcept {
        // look for _Ch at or after _Off
        if(pos >= this->u8size()) return npos;

        return cnv_pos_byte_sym(data(), Base::find_first_of(ch, f_retPosSize(pos)));      // from size in BYTES to size in SYMBOLS
    }
    
    //  --------------------------------------
    static constexpr void u8prev(pointer& p) noexcept {
        while(u8_is_trail(*(--p)));
    } 

private:
    [[nodiscard]] constexpr         // off in BYTES
    auto mfind_last_of(const size_type off, const Base_view right)  const noexcept {
        for(pointer match_ptr = (pointer)data() + off;  match_ptr >= data(); ) {
            const auto len = u8len(match_ptr);
            if(matching_one_symbol_and_sequence(match_ptr, len, right) != npos) {
                return cnv_pos_byte_sym(data(), match_ptr - data());
            }
            u8_prev(match_ptr);
        }
        return npos; // no match
    }
public:

    template<class Tsview, Is_string_view<Tsview> = 0>
    [[nodiscard]] constexpr                             // pos in SYMBOLS
    size_type find_last_of(const Tsview& right, size_type pos = npos) const
        noexcept(is_nothrow_convertible_v<const Tsview&, Base_view>) {
        // look for one of right before pos
        if(right.empty()) return npos;;
        
        if(right.size() > size()) {
            if(pos == 0) return 0;
        }
        
        if(pos == npos) pos = size()-1;
        else {
            pos = f_retPosSize(pos);            // in BYTES
            if(pos >= size()) pos = size()-1;
        }
        
        return mfind_last_of(pos, right);
    }

    [[nodiscard]] constexpr                             // pos and count in SYMBOLS
    size_type find_last_of(const _Elem* const ptr, size_type pos, size_type count) const noexcept  {
        // look for one of [ptr, ptr + count) before pos
        if(ptr == nullptr || *ptr == _Elem{}) return npos; // empty
        
        const auto count8 = ::u8size(ptr);
        if(count > count8) count = count8;

        Base_view as_view(ptr, f_retPosSize(ptr, count));
        return find_last_of(as_view, pos);
    }

    [[nodiscard]] constexpr                             // pos in SYMBOLS
    size_type find_last_of(const _Elem* const ptr, const size_type pos = npos) const noexcept  {
        // look for one of [ptr, <null>) before pos
        Base_view as_view(ptr);
        return find_last_of(as_view, pos);
    }

    [[nodiscard]] constexpr                             // pos in SYMBOLS
    size_type find_last_of(const _Elem ch, size_type pos = npos) const noexcept {
        // look for ch before pos
        if(pos == 0) return npos;
        if(pos >= this->u8size() && pos != npos) pos = npos;

        if(pos != npos) pos = f_retPosSize(pos);   // in BYTES
        return cnv_pos_byte_sym(data(), Base::find_last_of(ch, pos));      // from size in BYTES to size in SYMBOLS
    }

private:
    [[nodiscard]] constexpr                     // off in BYTES
    auto mfind_first_not_of(const size_type off, const Base_view right)  const noexcept {
        for(auto match_ptr = data()+off;  match_ptr < data() + size(); ) {
            const auto len = u8len(match_ptr);
            if(matching_one_symbol_and_sequence(match_ptr, len, right) == npos) {
                return cnv_pos_byte_sym(data(), match_ptr - data());
            }
            match_ptr += len;
        }
        return npos; // no match
    }
public:

    template <class Tsview, Is_string_view<Tsview> = 0>
    [[nodiscard]] constexpr                             // pos in SYMBOLS
    size_type find_first_not_of(const Tsview& right, const size_type pos = 0) const
        noexcept(is_nothrow_convertible_v<const Tsview&, u8string_view>) {
        // look for none of right at or after pos
        if(pos >= this->u8size()) return npos;
        if(right.empty()) {
            if(pos == 0) return 0;
            return pos;
        }
        return mfind_first_not_of(f_retPosSize(pos), right);
    }

    [[nodiscard]] constexpr                             // pos and count in SYMBOLS
    size_type find_first_not_of(const _Elem* const ptr, const size_type pos, size_type count) const noexcept {
        // look for none of [ptr, ptr + count) at or after pos
        count  =  f_retPosSize(ptr, (std::min)(count, ::u8size(ptr)));
        Base_view as_view (ptr, count);
        return find_first_not_of(as_view, pos);
    }

    [[nodiscard]] constexpr                             // pos in SYMBOLS
    size_type find_first_not_of(const _Elem* const ptr, const size_type pos = 0) const noexcept {
        // look for one of [ptr, <null>) at or after pos
        Base_view as_view (ptr);
        return find_first_not_of(as_view, pos);
    }

    [[nodiscard]] constexpr                             // pos in SYMBOLS
    size_type find_first_not_of(const _Elem ch, const size_type pos = 0) const noexcept {
        // look for non-ch at or after pos
        if(pos >= this->u8size()) return npos;
        
        return cnv_pos_byte_sym(data(), Base::find_first_not_of(ch, f_retPosSize(pos)));      // from size in BYTES to size in SYMBOLS
    }
    //----------------------------------------------------------------------------------------
private:
    [[nodiscard]] constexpr         // off in BYTES
    auto mfind_last_not_of(const size_type off, const Base_view right)  const noexcept {
        for(pointer match_ptr = (pointer)data() + off;  match_ptr >= data(); ) {
            const auto len = u8len(match_ptr);
            if(matching_one_symbol_and_sequence(match_ptr, len, right) == npos) {
                return cnv_pos_byte_sym(data(), match_ptr - data());
            }
            u8_prev(match_ptr);
        }
        return npos; // no match
    }
public:
    
    template<class Tsview, Is_string_view<Tsview> = 0>
    [[nodiscard]] constexpr                             // pos in SYMBOLS
    size_type find_last_not_of(const Tsview& right, size_type pos = npos) const
        noexcept(is_nothrow_convertible_v<const Tsview&, u8string_view>) {
        // look for none of right before pos
        if(right.empty()) {
            return (std::min)(pos, size8()-1);
        }
        
        if(right.size() > size()) return npos;
        
        if(pos == npos) pos = size()-1;
        else  {
            pos = f_retPosSize(pos);            // in BYTES
            if(pos >= size()) pos = size()-1;
        }
        
        return mfind_last_not_of(pos, right);
    } 

    [[nodiscard]] constexpr            // pos and count in SYMBOLS
    size_type find_last_not_of(const _Elem* const ptr, const size_type pos, size_type count) const noexcept {
        // look for none of [ptr, ptr + count) before pos
        count  =  f_retPosSize(ptr, (std::min)(count, ::u8size(ptr)));
        Base_view as_view (ptr, count);
        return find_last_not_of(as_view, pos);
    }

    [[nodiscard]] constexpr            // pos in SYMBOLS
    size_type find_last_not_of(const _Elem* const ptr, const size_type pos = npos) const noexcept {
        // look for none of [ptr, <null>) before pos
        Base_view as_view (ptr);
        return find_last_not_of(as_view, pos);
    }

    [[nodiscard]] constexpr            // pos in SYMBOLS
    size_type find_last_not_of(const _Elem ch, size_type pos = npos) const noexcept {
        // look for non-ch before pos
        if(pos == 0) return pos;   // in BYTES

        if(pos >= u8size()) pos = npos;   // in BYTES
        if(pos != npos) pos = f_retPosSize(pos);   // in BYTES
        return cnv_pos_byte_sym(data(), Base::find_last_not_of(ch, pos));      // from size in BYTES to size in SYMBOLS
    }
    //////////////////////////////////////  SUBSTR /////////////////////////////////////////////////////////
    //                          substr
    [[nodiscard]] constexpr20                          //  pos and count in SYMBOLS
    basic_utf8string substr(const size_type pos = 0, const size_type count = npos) const { 
        // return [pos, pos + count) as new string
        return basic_utf8string(*this, pos, count);
    }
    //////////////////////////////////////  COMPARE /////////////////////////////////////////////////////////
    template<class Tsview, Is_string_view<Tsview> = 0>
    [[nodiscard]] constexpr 
    int compare(const Tsview& right) const 
                noexcept(is_nothrow_convertible_v<const Tsview&, u8string_view>) {
        // compare [0, size()) with right
        return Base::compare(right);
    } 

    template<class Tsview, Is_string_view<Tsview> = 0>
    [[nodiscard]] constexpr                               // pos and count in SYMBOLS
    int compare(size_type pos, size_type count, const Tsview& right) const {
        // compare [pos, pos + count) with right
        tie(pos, count) = retPosSize(this->data(), pos, count);

        return Base::compare(pos, count, right);
    }

    template<class Tsview, Is_string_view<Tsview> = 0>
    [[nodiscard]] constexpr                               // pos and count in SYMBOLS
    int compare(size_type pos, size_type count, const Tsview& right, size_type rpos, size_type rcount = npos) const {
        // compare [pos, pos + count) with right [rpos, rpos + rcount)
        tie(pos, count, rpos, rcount) = retPosSize(pos, count, right.data(), rpos, rcount);

        return Base::compare(pos, count, right, rpos, rcount);
    }
    //
    [[nodiscard]] constexpr
    int compare(const _Elem* const ptr) const noexcept {
        // compare [0, size()) with [ptr, <null>)
        return Base::compare(ptr);
    } 
    //
    [[nodiscard]] constexpr                    // pos and count in SYMBOLS
    int compare(size_type pos, size_type count, const _Elem* const ptr) const  {
        // compare [pos, pos + count) with [ptr, <null>)
        tie(pos, count) = retPosSize(this->data(), pos, count);
        return Base::compare(pos, count, ptr);
    }
    //
    [[nodiscard]] constexpr        // pos, count, count2 in SYMBOLS
    int compare(size_type pos, size_type count, const _Elem* const ptr, const size_type count2) const {
        // compare [pos, pos + count) with [ptr, ptr + count2)
        tie(pos, count) = retPosSize(this->data(), pos, count);
        return Base::compare(pos, count, ptr, retPosSize(ptr, count2));
    }
    // сравнение 2-х символов
    [[nodiscard]] constexpr                                 // pos in SYMBOLS
    int compare_one_symbol(const _Elem* const ptr, const size_type pos=0) const {
        // compare [pos, pos + 1) with [ptr, ptr + 1)     in SYMBOLS
        return compare_two_utf8_symbols(this->data(), ptr + retPosSize(ptr, pos));
    }
    //                                  - срав. с C строкой.
    [[nodiscard]] constexpr      // pos1, count1 in SYMBOLS
    int compare(const size_type pos1, const size_type count1, const std::string_view str, const size_type count2) const {
        // compare [pos1, pos1 + count1) with str [0, count2]
        return -str.compare(0, count2, substr(pos1, count1).string());
    }
    
    //////////////////////////////////////  END COMPARE  ////////////////////////////////////////////////
    ////////////////////          Сравнение без учета регистра             ////////////////////////////////////
    [[nodiscard]] constexpr 
    int icompare(const basic_utf8string& s2) const noexcept {
         return utf8_tolower_ansi(*this).compare(utf8_tolower_ansi(s2));
    }
    [[nodiscard]] constexpr 
    bool ieq(const basic_utf8string& s2) const noexcept {
         return this->icompare(s2) == 0;
    }
    [[nodiscard]] constexpr 
    bool ilt(const basic_utf8string& s2) const noexcept {
        return this->icompare(s2) < 0;
    }
    [[nodiscard]] constexpr 
    bool igt(const basic_utf8string& s2) const noexcept {
         return this->icompare(s2) > 0;
    }

  //////////////////////////////////////////   INPUT   ///////////////////////////////////////////////////////
    inline std::basic_istream<char8_t>& operator >> (std::basic_istream<char8_t>& is) {
        return  std::operator>>(is, *this);
    }
}; // basic_utf8string
///////////////////////////////////////////////////////////////////////////////////////////////////////////////
template<>
class back_insert_iterator<std::utf8string> { // wrap pushes to back of container as output iterator
public:
    using iterator_category = output_iterator_tag;
    using value_type        = void;
    using pointer           = void;
    using reference         = void;
    using _Container        = std::utf8string;
    using container_type    = _Container;

    using it_ret            = typename _Container::it_ret;
#ifdef __cpp_lib_concepts
    using difference_type = ptrdiff_t;

    constexpr back_insert_iterator() noexcept = default;
#else // ^^^ __cpp_lib_concepts / !__cpp_lib_concepts vvv
    using difference_type = void;
#endif // __cpp_lib_concepts

    constexpr explicit back_insert_iterator(_Container& _Cont) noexcept /* strengthened */
        : container(std::addressof(_Cont)) {}

    constexpr 
    back_insert_iterator& operator=(const _Container& _Val) {
        container->push_back(_Val);
        return *this;
    }

    constexpr back_insert_iterator& operator=(_Container&& _Val) noexcept {
        container->push_back(std::move(_Val));
        return *this;
    }
    constexpr 
    back_insert_iterator& operator=(const it_ret& right) noexcept {
        u8string tmp = right.string8();
        container->push_back(std::move(tmp));
        return *this;
    }
    constexpr 
    back_insert_iterator& operator=(it_ret&& right) noexcept {
        u8string tmp = right.string8();
        container->push_back(std::move(tmp));
        return *this;
    }
    constexpr 
    back_insert_iterator& operator=(const u8string& right) noexcept {
        container->push_back(std::remove_cvref_t<u8string>(right));
        return *this;
    }
    [[nodiscard]] constexpr 
    back_insert_iterator& operator*() noexcept  {
        return *this;
    }

    constexpr back_insert_iterator& operator++() noexcept  {
        return *this;
    }
    constexpr back_insert_iterator& operator--() noexcept  {   // может пригодиться
        return *this;                                                            // для move_backward
    }                                                                            // dest как back_inserter
                                                                                 // тогда получается в обратном порядке
    constexpr back_insert_iterator operator++(int) noexcept {
        return *this;
    }

    [[nodiscard]] inline
    _Container get() const noexcept { return *container; }
    
    template<class Ch, class Tr> friend inline
    basic_ostream<Ch, Tr>& operator<<(basic_ostream<Ch, Tr>& os, const back_insert_iterator<_Container>& x) {
        return os << x.get();
    }
protected:
    _Container* container = nullptr;
};  // back_insert_iterator
//-------------------------------------------------------------------------------------------------------------
// FUNCTION TEMPLATE back_inserter
template<>
[[nodiscard]] constexpr 
back_insert_iterator<std::utf8string> back_inserter<std::utf8string>(std::utf8string& _Cont) noexcept /* strengthened */ {
    return back_insert_iterator<std::utf8string>(_Cont);
}
///////////////////////////////////////////////////////////////////////////////////////////////////////////////
template<class T>
inline constexpr bool is_iter8_v = std::is_iterator_v<T> &&
                                   std::is_any_of_v<T, utf8string::iterator, utf8string::const_iterator>;
///////////////////////////////////////////////////////////////////////////////////////////////////////////////
//                                          вывод
inline std::basic_ostream<char>& operator << (std::basic_ostream<char>& os, const utf8string& s8) {
        return os << s8.string();
}
inline std::basic_ostream<wchar_t>& operator << (std::basic_ostream<wchar_t>& os, const utf8string& s8) {
        return os << s8.wstring();
}
inline std::basic_ostream<char>& operator << (std::basic_ostream<char>& os, const std::u8string& s8) {
     return os << *((std::utf8string*)&s8);
} 
inline std::basic_ostream<char>& operator << (std::basic_ostream<char>& os, const std::vector<std::u8string>& v8) {
     for(const auto& x: v8) os << x << "|"; os << std::endl;
     return os;
} 
inline std::basic_ostream<char>& operator << (std::basic_ostream<char>& os, const std::u8string_view s8v) {
     return os << std::u8string(s8v);
} 
inline std::basic_ostream<char>& operator << (std::basic_ostream<char>& os, char8_t* sp8) {
     return os << std::utf8string(sp8);
} 
inline std::basic_ostream<char>& operator << (std::basic_ostream<char>& os, const char8_t* sp8) {
     return os << std::utf8string(sp8);
} 
inline std::basic_ostream<char>& operator << (std::basic_ostream<char>& os, const char8_t c8) {
     return os << std::utf8string(c8);
}
inline std::basic_ostream<wchar_t>& operator << (std::basic_ostream<wchar_t>& os, const std::u8string& s8) {
     return os << std::utf8string(s8);
}
inline std::basic_ostream<wchar_t, std::char_traits<wchar_t>>& operator << (
            std::basic_ostream<wchar_t, std::char_traits<wchar_t>>& os, const std::vector<std::u8string>& v8) {
     for(const auto& x: v8) os << x << L"| "; os << std::endl;
     return os;
} 
inline std::basic_ostream<wchar_t>& operator << (std::basic_ostream<wchar_t>& os, const std::u8string_view s8v) {
     return os << std::utf8string(s8v).wstring();
}
inline std::basic_ostream<wchar_t>& operator << (std::basic_ostream<wchar_t>& os, char8_t* sp8) {
     return os << std::utf8string(sp8).wstring();
} 
inline std::basic_ostream<wchar_t>& operator << (std::basic_ostream<wchar_t>& os, const char8_t* sp8) {
     return os << std::utf8string(sp8).wstring();
} 
///////////////////////////////////////////////////////////////////////////////////////////////////////////////
inline namespace literals {
inline namespace string_literals {

[[nodiscard]] inline
std::utf8string operator "" _s8 (const char8_t* const _ptr_, [[maybe_unused]] const size_t _len_) noexcept {
    return (std::utf8string(_ptr_));
}
//------------------
[[nodiscard]] inline
std::utf8string operator "" _s8 (const char* const _ptr_, [[maybe_unused]] const size_t _len_) noexcept {
    return (std::utf8string(_ptr_));
}
}}  // namespace string_literals}  // namespace literals}  // namespace
//----------------------------------------------------------------------
template<> 
inline void iter_swap<iter8, iter8>(iter8 left, iter8 right) noexcept {
        left.iter_swap(right);  
}
} // namespace std
///////////////////////////////////////////////////////////////////////////////////////////////////////////////
constexpr                                    // pos in SYMBOLS!
void u8_advance(std::iter8& it, const ptrdiff_t pos) noexcept {
     it += pos;
} 
///////////////////////////////////////////////////////////////////////////////////////////////////////////////
//                      конвертирует строку в число, в случае ошибки возвращает 0
template<class T=int> 
[[nodiscard]] constexpr
T NumFromString(const std::utf8string& str) {
    T res{};
    std::from_chars((const char*)str.data(), (const char*)(str.data() + str.size()), res);
    return res;       // 0 при ошибке
}
///////////////////////////////////////////////////////////////////////////////////////////////////////////////
#endif // _utf8string_hpp_

