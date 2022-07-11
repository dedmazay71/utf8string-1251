//
//                  u8_iterator.hpp  -- Copyright (C) by Alexander Prilepsky 2022
//
// Этот заголовочный файл содержит класс  Iterator8<std::utf8string>
//
// ПРОГРАММНОЕ ОБЕСПЕЧЕНИЕ ПРЕДОСТАВЛЯЕТСЯ "КАК ЕСТЬ", БЕЗ КАКИХ-ЛИБО ГАРАНТИЙ,
// ЯВНЫЕ ИЛИ ПОДРАЗУМЕВАЕМЫЕ, ВКЛЮЧАЯ, НО НЕ ОГРАНИЧИВАЯСЬ ГАРАНТИЯМИ
// ТОВАРНАЯ ПРИГОДНОСТЬ, ПРИГОДНОСТЬ ДЛЯ ОПРЕДЕЛЕННОЙ ЦЕЛИ И ОТСУТСТВИЕ НАРУШЕНИЙ.
// НИ В КОЕМ СЛУЧАЕ АВТОР НЕ НЕСEТ ОТВЕТСТВЕННОСТИ ЗА КАКИЕ-ЛИБО ПРЕТЕНЗИИ, УБЫТКИ ИЛИ
// ИНАЯ ОТВЕТСТВЕННОСТЬ, БУДЬ ТО В СИЛУ ДОГОВОРА, ДЕЛИКТА ИЛИ ИНЫМ ОБРАЗОМ,
// ВЫТЕКАЮЩИЕ ИЗ, ИЛИ В СВЯЗИ С ПРОГРАММНЫМ ОБЕСПЕЧЕНИЕМ ИЛИ ИСПОЛЬЗОВАНИЕМ ИЛИ
// ДРУГИЕ СДЕЛКИ С ПРОГРАММНЫМ ОБЕСПЕЧЕНИЕМ.
//
// ВАЖНО помнить, что когда вы получаете итератор к коллекции, а после этого модифицируете коллекцию, то этот 
// итератор становится уже непригодным к использованию. 
//
//                      c++ Visual Studio 2022 version 17.2
//
///////////////////////////////////////////////////////////////////////////////////////////////////////////////
#pragma once
//                              for ANSI
#pragma warning(disable : 5101)
//5101

namespace std {

#ifndef scopy_xxx
#define scopy_xxx
template<class T> constexpr
void scopy(T* const dest, const T* const src, size_t size) {
    std::char_traits<T>::copy(dest, src, size);
}
#endif

template<class>
class Iterator8;
///////////////////////////////////////////////////////////////////////////////////////////////////////////////
template<size_t N=4>
struct Orig {
    using Base              = u8string;
    using value_type        = typename Base::value_type;
    using size_type         = typename Base::size_type;
    using difference_type   = typename Base::difference_type;
    using pointer           = typename Base::pointer;
    using const_pointer     = const pointer;
    using reference         = value_type&;
    using const_reference   = const value_type&;

    constexpr Orig() noexcept = default;
    constexpr Orig(const Orig& r) noexcept = default;

    Orig(nullptr_t) = delete;

    constexpr Orig(const value_type* const r) { 
        scopy(dat, r, size(r));
    }
    template<size_t P>
    constexpr Orig(const array<value_type, P>& r) { 
        scopy(dat, r.data(), size(r.data()));
    }
    template<size_t P> requires (P!=N)
    constexpr Orig(const value_type (&r)[P]) { 
        scopy(dat, (value_type*)r, size((value_type*)r));
    }
    
    constexpr 
    operator const value_type*() const noexcept { 
        return (const value_type*)(dat); 
    } 

    [[nodiscard]] constexpr 
    const value_type* data() const noexcept { 
        return (const value_type*)dat; 
    } 
    [[nodiscard]] constexpr 
    value_type* data() noexcept { 
        return (value_type*)dat; 
    }
    
    [[nodiscard]] constexpr
    size_t size() const noexcept {
        return len8(*dat);
    }
    [[nodiscard]] constexpr
    size_t size(const value_type* p) const noexcept {
        return len8(*p);
    }
    constexpr
    u8string string8() const noexcept { 
        return u8string((value_type*)dat, size()); 
    }
    
    constexpr
    operator u8string() const noexcept { 
        return this->string8(); 
    }
    
    constexpr 
    u8string_view string8_view() const noexcept { 
        return u8string_view((value_type*)dat, size()); 
    }
    constexpr 
    operator u8string_view() const noexcept { 
        return this->string8_view(); 
    }

    using ar_const = const value_type(&)[N];
    using ar = value_type(&)[N];

    constexpr 
    operator ar()  noexcept { 
        return dat; 
    } 
    constexpr 
    operator ar_const () const noexcept { 
        return dat; 
    }
    
    [[nodiscard]] constexpr
    int compare(const Orig& right) const noexcept {
        return compare_two_utf8_symbols(data(), right.data());
    }

    value_type dat[N] = {value_type{}};

    template<class Ch, class Tr>
    friend auto& operator<<(basic_ostream<Ch, Tr>& os, const Orig& r) noexcept {
        return os << r.string8();
    }
    using comparison_category = strong_ordering;
}; // Orig

template<std::size_t N> 
Orig(const char8_t(&)[N]) -> Orig<N>;
///////////////////////////////////////////////////////////////////////////////////////////////////////////////
//                                          COMPARE
template <size_t N>
[[nodiscard]] constexpr bool operator==(const Orig<N>& left, const Orig<N>& right) noexcept {
    return left.compare(right) == 0;
}
template <size_t N>
[[nodiscard]] constexpr typename Orig<N>::comparison_category operator<=> (const Orig<N>& left, 
                                                                        const Orig<N>& right) {
    return static_cast<typename Orig<N>::comparison_category>(left.compare(right) <=> 0);
}
//-----------------------------------------------------------------------------------
template<size_t N>
struct hash<Orig<N>> {
    [[nodiscard]] size_t operator()(const Orig<N> Keyval) const noexcept {
        return _Hash_array_representation(Keyval.data(), Keyval.size());
    }
};

template<class Tstr>//=utf8string>
class Retu8  {
    template<class> 
    friend class Iterator8;
    
    template<class> 
    friend class basic_utf8string;
    
    using ITER = Iterator8<Tstr>;

    using RET  = Retu8<Tstr>;

public:
    using type_orig         = Orig<4>;
    using type_base         = Tstr;
    using ret_type          = Retu8;
    using value_type        = ret_type;
    //using value_type        = u8string;
    
    using difference_type   = typename Tstr::difference_type;
    using pointer           = typename Tstr::pointer;
    using const_pointer     = typename Tstr::const_pointer;
    using reference         = ret_type&;                         //Retu8;
    using const_reference   = const ret_type&;
    using ovalue_type       = typename Tstr::value_type;

#define RLIST ptr_to_string->iterators_chain   
//private:
//    void print_chain() {}

private:
    constexpr
    void set_chain() {
        if(RLIST)                              //уже там есть чего-то
            next_ = RLIST;
        RLIST = (ITER*)this;
        //print_chain();
    }

    constexpr
    void remove_from_chain() {
        auto prev = (ITER*)nullptr;
        for(ITER* ptr=RLIST; ptr != (ITER*)nullptr; prev = ptr, ptr = (ITER*)(ptr->next_)) {
            if(this == ptr) {
                if(ptr == RLIST) RLIST = (ITER*)next_; 
                else             prev->next_ = next_;    
                next_ = (ITER*)nullptr;      // необязательно вроде бы
                break;
            } 
        }
        //print_chain();
    }
public:
    explicit 
    constexpr Retu8(const ovalue_type* p, const type_base& base_string) noexcept : 
                                                           Ptr((ovalue_type*)p), 
                                                           ptr_to_string((type_base*)(&base_string)) {
        diff_ptr_in_symbols_ = u8_distance(ptr_to_string->data(), Ptr);
        set_chain();
    }

public:
    constexpr
    Retu8() noexcept : Ptr() {}

    constexpr 
    Retu8(const ITER& it) noexcept : Ptr(it.Ptr), ptr_to_string(it.ptr_to_string), 
                                    diff_ptr_in_symbols_(it.diff_ptr_in_symbols_) {
        set_chain();
    }

    Retu8(ITER&& it) = delete;

    using iterator          = typename u8string::iterator;
    using const_iterator    = typename u8string::const_iterator;

    [[nodiscard]] constexpr 
#if _ITERATOR_DEBUG_LEVEL >= 1
    type_base& get_string() { 
        ASSERTM(ptr_to_string, "cannot return non-initialized string");
#else
    type_base& get_string() noexcept { 
#endif
        return *ptr_to_string; 
    }

    [[nodiscard]] constexpr 
#if _ITERATOR_DEBUG_LEVEL >= 1
    const type_base& get_string() const { 
        ASSERTM(ptr_to_string, "cannot return non-initialized string");
#else
    const type_base& get_string() const noexcept { 
#endif
        return *ptr_to_string; 
    }

    [[nodiscard]] constexpr 
#if _ITERATOR_DEBUG_LEVEL >= 1
    reference operator*() const {
        ASSERTM(Ptr, "cannot dereference value-initialized string iterator");
        ASSERTM(Ptr >= ptr_to_string->data() && 
                Ptr <= ptr_to_string->data() + ptr_to_string->size(),
            "cannot dereference string iterator because it is out of range (e.g. an end iterator)");
#else
    reference operator*() const noexcept {
#endif // _ITERATOR_DEBUG_LEVEL >= 1

        return const_cast<reference>(*this);
    }
//----------------------------------------------------------------------------------------------------------
    [[nodiscard]] constexpr 
    size_t operator/(const size_t divisor) const noexcept {
        const auto              len = len8(*Ptr);
        size_t                  hx(0);
        
        if(len == 1) hx = Ptr[0];
        else
        if(len == 2) hx = (Ptr[0] << 8) | Ptr[1];
        else
        if(len == 3) hx = (Ptr[1] << 8) | Ptr[2];
        return hx/divisor; 
    }

    using comparison_category = strong_ordering;
    
    [[nodiscard]] constexpr
    int compare(const Retu8& right) const noexcept {
        return compare_two_utf8_symbols(Ptr, right.Ptr);
    }
    
    template<size_t N>
    [[nodiscard]] constexpr
    int compare(const Orig<N>& right) const noexcept {
        return compare_two_utf8_symbols(Ptr, right.data());
    }
    
    [[nodiscard]] constexpr
    int compare(const Tstr& right) const noexcept {
        return compare_two_utf8_symbols(Ptr, right.data());
    }
    [[nodiscard]] constexpr
    int compare(const u8string& right) const noexcept {
        return compare_two_utf8_symbols(Ptr, right.data());
    }
    [[nodiscard]] constexpr
    int compare(const char8_t* const right) const noexcept {
        return compare_two_utf8_symbols(Ptr, right);
    }

//--------------------------------
    [[nodiscard]] constexpr
    u8string string8() const noexcept {
        return u8string( Ptr, len8(*Ptr) );
    }
    
    constexpr
    operator u8string() const noexcept { 
        return u8string( Ptr, len8(*Ptr) );
    }
    //  operator utf8string() const noexcept;    определяется после определения класса utf8string
    [[nodiscard]] constexpr 
    u8string_view string8_view() const noexcept { 
        return u8string_view(Ptr, len8(*Ptr)); 
    }

    constexpr 
    operator u8string_view() const noexcept { 
        return this->string8_view(); 
    }

    using type_pointer = remove_const_t<pointer>;
    // неконстантный указатель
    [[nodiscard]] constexpr 
    type_pointer rc_ptr() const noexcept { return (type_pointer)Ptr; }

    [[nodiscard]] constexpr 
    type_orig get_orig() const noexcept { 
        return type_orig(Ptr); 
    }

    constexpr
    operator type_orig() const noexcept { 
        return type_orig(Ptr);
    }

    constexpr
    reference operator=(const Tstr& right) noexcept {
        copy_right(right.data());
        return *this;
    }
    
    template<size_t N=4> constexpr
    reference operator=(const Orig<N>& right) noexcept {
        copy_right(right.data());
        return *this;
    }
    
    constexpr
    reference operator=(const Retu8& right) noexcept {
        replace_ref(right.Ptr, len8(*Ptr), len8(*(right.Ptr)));
        return *this;                            
    }

private:
    constexpr
    void corr_realloc(auto& p) {
        Ptr = u8_fwd_n(ptr_to_string->data(), diff_ptr_in_symbols_);         // NEW
        p = (ITER*)next_;
    }
    constexpr
    void viewing_the_chain_corr_realloc(auto p) {
        while(p) p->corr_realloc(p);
    }
    constexpr
    void corr_diff_ptrs(auto& p, const auto* ptr, const auto diff) {
        if(Ptr > ptr) Ptr += diff;
        p = (ITER*)next_;
    }
    constexpr
    void viewing_the_chain_corr_diff_ptrs(auto p, const auto* ptr, const auto diff) {
        while(p) p->corr_diff_ptrs(p, ptr, diff);
    }
public:
    ~Retu8() { 
        if(ptr_to_string) remove_from_chain();
    };

//-----------------------------------------------------------------------------------------------------------
    template<class Ch, class Tr>
    friend auto& operator<<(std::basic_ostream<Ch, Tr>& os, const Retu8& r) {
        //#ifndef NOTDEBUG
        //return os << utf8_to_char_ansi(r.Ptr, 0) << '(' << *(r.ptr_to_string) << ')';
        //#else
        return os << utf8_to_char_ansi(r.Ptr, 0);
        //#endif
    }
//-----------------------------------------------------------------------------------------------------------

    pointer          Ptr;
private:
    type_base*       ptr_to_string          = nullptr;//(type_base*)(&pusto);
    difference_type  diff_ptr_in_symbols_   = 0;
    Retu8*           next_                  = nullptr;
private:
    void replace(const ovalue_type* const p, const auto size_left, const auto size_right) {
        ptr_to_string->replace((type_pointer)Ptr, size_left, p, size_right);
    }

    void replace_ref(const ovalue_type* const right, const auto len_left, const auto len_right) noexcept {
        if(len_left == len_right) { 
            scopy(Ptr, right, len_right); 
            return;
        }
        // если не будет реэллокейта, то надо буде только смещение(в байтах) для ук-лей правее этого.
        // для указателей левее этого ничего не меняется
        // если будет реэллокейт, то надо будет инфо о смещении ук-лей, до выполнения реэллокации, 
        // причем для всех итераторов, а не только для тех, что правее
        // 
        const ovalue_type* beg          = ptr_to_string->data();
        const difference_type diff_ptr  = len_right - len_left;   // смещение в байтах для ук-лей правее
                                                                  // этого 
        replace(right, len_left, len_right);     // может поменяться _Ptr
        if(beg == ptr_to_string->data()) {                  // не было реэллокейт
            viewing_the_chain_corr_diff_ptrs(RLIST, Ptr, diff_ptr);  
        }
        else {                                          // была реэллокация
            viewing_the_chain_corr_realloc(RLIST);
        }
    }

    constexpr
    void copy_right(const ovalue_type* const right) {                  // ???
        replace_ref(right, utf8len(*Ptr), utf8len(*right));
    }
}; // class Retu8
////////////////////////////////////////////////////////////////////////////////////////////////////////////////

template <class Tstr>
[[nodiscard]] constexpr bool operator==(const Retu8<Tstr>& left, const Retu8<Tstr>& right) noexcept {
    return left.compare(right) == 0;
}
template <size_t N, class Tstr>
[[nodiscard]] constexpr bool operator==(const Retu8<Tstr>& left, const Orig<N>& right) noexcept {
    return left.compare(right) == 0;
}
template <class Tstr>
[[nodiscard]] constexpr bool operator==(const Retu8<Tstr>& left, const Tstr& right) noexcept {
    return left.compare(right) == 0;
}
template <class Tstr>
[[nodiscard]] constexpr bool operator==(const Retu8<Tstr>& left, const u8string& right) noexcept {
    return left.compare(right) == 0;
}
template <class Tstr>
[[nodiscard]] constexpr bool operator==(const Retu8<Tstr>& left, const char8_t* const right) {
    return left.compare(right) == 0;
}
//template <size_t N, class Tstr>
//[[nodiscard]] constexpr bool operator==(const Retu8<Tstr>& left, const char8_t (&right)[N]) noexcept {
//[[nodiscard]] constexpr bool operator==(const Retu8<Tstr>& left, const char8_t right[N]) noexcept {
//    return left.compare(right) == 0;
//} 
//--------------------
template <class Tstr>
[[nodiscard]] constexpr typename Retu8<Tstr>::comparison_category operator<=>(
                                                    const Retu8<Tstr>& left, const Retu8<Tstr>& right) {
    return static_cast<typename Retu8<Tstr>::comparison_category>(left.compare(right) <=> 0);
}

template <size_t N, class Tstr>
[[nodiscard]] constexpr typename Retu8<Tstr>::comparison_category operator<=>(
                                                    const Retu8<Tstr>& left, const Orig<N>& right) {
    return static_cast<typename Retu8<Tstr>::comparison_category>(left.compare(right) <=> 0);
}

template <class Tstr>
[[nodiscard]] constexpr typename Retu8<Tstr>::comparison_category operator<=>(
                                                    const Retu8<Tstr>& left, const Tstr& right) {
    return static_cast<typename Retu8<Tstr>::comparison_category>(left.compare(right) <=> 0);
}
template <class Tstr>
[[nodiscard]] constexpr typename Retu8<Tstr>::comparison_category operator<=>(
                                                    const Retu8<Tstr>& left, const u8string& right) {
    return static_cast<typename Retu8<Tstr>::comparison_category>(left.compare(right) <=> 0);
}
template <class Tstr>
[[nodiscard]] constexpr typename Retu8<Tstr>::comparison_category operator<=>(
                                                    const Retu8<Tstr>& left, const char8_t* const right) {
    return static_cast<typename Retu8<Tstr>::comparison_category>(left.compare(right) <=> 0);
}
//--------------------------
template<class Tstr>
struct hash<Retu8<Tstr>> {
    [[nodiscard]] constexpr
    std::size_t operator()(Retu8<Tstr> const& s) const noexcept {
        return std::hash<u8string>{}(s);
    }
};
////////////////////////////////////////////////////////////////////////////////////////////////////////////////
template<class Tstr>
class Iterator8 : public Retu8<Tstr> {
public:
    using Base = Retu8<Tstr>;
    using typename Base::type_base;
    using typename Base::ovalue_type;
    using  typename Base::ret_type;
    using  typename Base::type_orig;

#ifdef __cpp_lib_concepts
    using iterator_concept  = contiguous_iterator_tag;
#endif // __cpp_lib_concepts
    using iterator_category = random_access_iterator_tag;
    using value_type        = type_orig;
    using difference_type   = typename Tstr::difference_type;
    using pointer           = typename Tstr::pointer;
    using const_pointer     = typename Tstr::const_pointer;

    using reference         = ret_type&;
    using const_reference   = const ret_type&;
    
    using Base::ptr_to_string;
    using Base::Ptr;
    using Base::next_;
    using Base::diff_ptr_in_symbols_;

    using Base::Base;

public:
    constexpr 
    Iterator8(const Iterator8& it) noexcept : Base(it) {}

    using  typename Base::iterator;
    using  typename Base::const_iterator;
    using  typename Base::type_pointer;

    constexpr 
    Iterator8& operator=(const Iterator8& rhs) noexcept {
        if(this != &rhs) { 
            if(!rhs.Ptr) {          // неинициализированный итератор справа
                if(ptr_to_string) this->remove_from_chain();
                ptr_to_string        = rhs.ptr_to_string;                  // nullptr
            }
            else
            if(ptr_to_string != rhs.ptr_to_string) {    // другая строка
                this->remove_from_chain();
                ptr_to_string  = rhs.ptr_to_string;
                this->set_chain();
            }
            Ptr                  = rhs.Ptr;
            diff_ptr_in_symbols_ = rhs.diff_ptr_in_symbols_;
        }
        
        return *this;
    }

protected:    
    constexpr 
    Iterator8&  operator=(const type_base& rhs) noexcept {
        if(Ptr)  this->remove_from_chain();

        ptr_to_string = (type_base*)(&rhs);
        this->set_chain();
        Ptr = ptr_to_string->data();         // NEW
        diff_ptr_in_symbols_ = 0;

        return *this;
    }
    
public:
    [[nodiscard]] constexpr 
#if _ITERATOR_DEBUG_LEVEL >= 1
    iterator get_u8iterator() const {
        _Verify_ptr();
#else     
    iterator get_u8iterator() const noexcept { 
#endif
        ptrdiff_t diff = Ptr - ptr_to_string->data();
        return ((u8string*)ptr_to_string)->begin()+diff; 
    }

    [[nodiscard]] constexpr 
#if _ITERATOR_DEBUG_LEVEL >= 1
    pointer operator->() const {
        _Verify_ptr();
#else
    pointer operator->() const noexcept {
#endif
        return const_cast<ovalue_type*>(Ptr);
    }

    constexpr 
#if _ITERATOR_DEBUG_LEVEL >= 1
    Iterator8& operator++() {
        ASSERTM(Ptr, "cannot increment value-initialized string iterator"); // nullptr
        ASSERTM(Ptr >= ptr_to_string->data() && Ptr <= ptr_to_string->data() + ptr_to_string->size(),
                                              "cannot seek string iterator because the iterator was"
                             " invalidated (e.g. reallocation occurred, or the string was destroyed)"
                             " or it is out of range");
        ASSERTM(1 <= static_cast<difference_type>(::u8size(Ptr)), "cannot increment string iterator past end");
#else
    Iterator8& operator++() noexcept {
#endif // _ITERATOR_DEBUG_LEVEL >= 1
        
        u8_next(Ptr);
        ++diff_ptr_in_symbols_;
        return *this;
    }

    constexpr 
    Iterator8  operator++(int) noexcept {
        Iterator8 Tmp(*this);
        ++*this;
        return Tmp;
    }
    
    constexpr 
#if _ITERATOR_DEBUG_LEVEL >= 1
    Iterator8& operator--() {
        ASSERTM(Ptr, "cannot increment value-initialized string iterator"); // nullptr
        ASSERTM(Ptr >= ptr_to_string->data() && Ptr <= ptr_to_string->data() + ptr_to_string->size(),
                                              "cannot seek string iterator because the iterator was"
                             " invalidated (e.g. reallocation occurred, or the string was destroyed)"
                             " or it is out of range");
        ASSERTM(diff_ptr_in_symbols_-1 >= 0, "cannot increment string iterator past end");
#else
    Iterator8& operator--() noexcept {

#endif // _ITERATOR_DEBUG_LEVEL >= 1
        u8_prev(Ptr);
        --diff_ptr_in_symbols_;
        return *this;
    }
    
    constexpr 
    Iterator8 operator--(int) noexcept {
        Iterator8 Tmp(*this);
        --*this;
        return Tmp;
    }

#if _ITERATOR_DEBUG_LEVEL >= 1
    constexpr void _Verify_ptr() const {
        ASSERTM(Ptr, "cannot seek value-initialized pointer");    // nullptr
        ASSERTM(Ptr >= ptr_to_string->data() && Ptr <= ptr_to_string->data() + ptr_to_string->size(),
                                              "cannot seek pointer because the pointer was"
                             " invalidated (e.g. reallocation occurred, or the string was destroyed)"
                             " or it is out of range");
#else // ^^^ _ITERATOR_DEBUG_LEVEL >= 1 ^^^ // vvv _ITERATOR_DEBUG_LEVEL == 0 vvv
    constexpr void _Verify_ptr() const noexcept {
#endif // _ITERATOR_DEBUG_LEVEL >= 1
    }


#if _ITERATOR_DEBUG_LEVEL >= 1
    constexpr void _Verify_offset(const difference_type Off) const {  // Off = in symbols
        if(Off == 0) return;

        ASSERTM(Ptr, "cannot seek value-initialized string iterator");    // nullptr
        ASSERTM(Ptr >= ptr_to_string->data() && Ptr <= ptr_to_string->data() + ptr_to_string->size(),
                                              "cannot seek string iterator because the iterator was"
                             " invalidated (e.g. reallocation occurred, or the string was destroyed)"
                             " or it is out of range");
        if(Off < 0) ASSERTM(diff_ptr_in_symbols_ + Off >= 0, "cannot seek string iterator before begin");

        if(Off > 0) ASSERTM(Off <= static_cast<difference_type>(::u8size(Ptr)),
                                                            "cannot seek string iterator after end");
#else // ^^^ _ITERATOR_DEBUG_LEVEL >= 1 ^^^ // vvv _ITERATOR_DEBUG_LEVEL == 0 vvv
    constexpr void _Verify_offset(const difference_type Off) const noexcept {  // Off = in symbols
        (void) Off;
#endif // _ITERATOR_DEBUG_LEVEL >= 1
    }

    constexpr 
#if _ITERATOR_DEBUG_LEVEL >= 1
    Iterator8& operator+=(const difference_type Off) {     // Off = in symbols
        _Verify_offset(Off);
#else
    Iterator8& operator+=(const difference_type Off) noexcept {     // Off = in symbols
#endif // _ITERATOR_DEBUG_LEVEL >= 1
        
        u8_advance(Ptr, Off);
        diff_ptr_in_symbols_ += Off;
        return *this;
    }

    [[nodiscard]] constexpr 
    Iterator8 operator+(const difference_type Off) const noexcept {  // Off = in symbols
        Iterator8 Tmp(*this);
        Tmp += Off;
        return Tmp;
    }

    constexpr 
    Iterator8& operator-=(const difference_type Off) noexcept {      // Off = in symbols
        return *this += -Off;
    }

    [[nodiscard]] constexpr 
    Iterator8 operator-(const difference_type Off) const noexcept {   // Off = in symbols
        Iterator8 Tmp(*this);
        Tmp -= Off;
        return Tmp;
    }

    //difference_type operator-(const Iterator8& _Right) const noexcept {
    [[nodiscard]] constexpr 
    difference_type operator-(const Iterator8& _Right) const {    // (*this - _Right)
        _Compat(_Right);
        return u8_distance(_Right.Ptr, Ptr); // (Ptr - _Right.Ptr) в СИМВОЛАХ  !!!!!!!!!!!
    }

    template<class Size>
    [[nodiscard]] constexpr 
    reference operator[](const Size Off) const noexcept {
        static_assert(!is_integral_v<Size> && is_integral_v<Size>, "DO NOT USE operator[], USE operator*");
    }

    [[nodiscard]] constexpr 
    bool operator==(const Iterator8& rhs) const noexcept {
        _Compat(rhs);
        return Ptr == rhs.Ptr;
    }

#if _HAS_CXX20
    [[nodiscard]] constexpr 
    strong_ordering operator<=>(const Iterator8& rhs) const noexcept {
        _Compat(rhs);
        return Ptr <=> rhs.Ptr;
    }
#else //_HAS_CXX20

    [[nodiscard]] constexpr 
    bool operator!=(const Iterator8& other) const noexcept {
        return !(*this == other);
    }
    [[nodiscard]] constexpr 
    bool operator<(const Iterator8& _Right) const noexcept {
        _Compat(_Right);
        return Ptr < Right.Ptr;
    }
    [[nodiscard]] constexpr 
    bool operator>(const Iterator8& _Right) const noexcept {
        return _Right < *this;
    }
    [[nodiscard]] constexpr 
    bool operator<=(const Iterator8& _Right) const noexcept {
        return !(_Right < *this);
    }
    [[nodiscard]] constexpr 
    bool operator>=(const Iterator8& _Right) const noexcept {
        return !(*this < _Right);
    }
#endif //_HAS_CXX20

    constexpr
#if _ITERATOR_DEBUG_LEVEL >= 1
    void _Compat(const Iterator8& _Right) const { // test for compatible iterator
        ASSERTM(ptr_to_string->data() == _Right.ptr_to_string->data(), "string iterators incompatible (e.g."
                                                           " point to different string instances)");
#else // ^^^ _ITERATOR_DEBUG_LEVEL >= 1 ^^^ // vvv _ITERATOR_DEBUG_LEVEL == 0 vvv
    void _Compat(const Iterator8& _Right) const noexcept { // test for compatible iterator
#endif // _ITERATOR_DEBUG_LEVEL
        (void) _Right;
    }
    
    constexpr      // например, where._Compat(this->data) where compatible iterator with string
#if _ITERATOR_DEBUG_LEVEL >= 1
    void _Compat(const pointer p) const { // test for compatible pointer
        ASSERTM(ptr_to_string->data() == p, "string iterators incompatible (e.g."
                                                           " point to different string instances)");
#else // ^^^ _ITERATOR_DEBUG_LEVEL >= 1 ^^^ // vvv _ITERATOR_DEBUG_LEVEL == 0 vvv
    void _Compat(const pointer p) const noexcept { // test for compatible pointer
#endif // _ITERATOR_DEBUG_LEVEL
        (void) p;
    }

#if _ITERATOR_DEBUG_LEVEL >= 1
    friend constexpr void _Verify_range(const Iterator8& _First, const Iterator8& _Last) {
        ASSERTM(_First.ptr_to_string->data() == _Last.ptr_to_string->data(), "string iterators in range are from different containers");
        ASSERTM(_First.Ptr <= _Last.Ptr, "string iterator range transposed");
#else // ^^^ _ITERATOR_DEBUG_LEVEL >= 1 ^^^ // vvv _ITERATOR_DEBUG_LEVEL == 0 vvv
    friend constexpr void _Verify_range(const Iterator8& _First, const Iterator8& _Last) noexcept {
        (void) (_First, _Last);
#endif // _ITERATOR_DEBUG_LEVEL >= 1
    }

    using _Prevent_inheriting_unwrap = Iterator8;

    [[nodiscard]] constexpr 
    const Iterator8 _Unwrapped() const noexcept {                    // ?????????????
        return *this;
    }

    constexpr
    void _Seek_to(const Iterator8& _It) noexcept {                //?????????????????
        *this = (Iterator8)(_It);
    }

    constexpr
    void iter_swap(const Iterator8& other) noexcept {
        if(other.Ptr == Ptr) return;

        value_type tmp = other;
        *other = **this;
        **this = tmp;
    }

    // print iterator
    template<class Ch, class Tr>
    friend auto& operator<<(std::basic_ostream<Ch, Tr>& os, const Iterator8& r) {
        //#ifndef NOTDEBUG
        //return os << '[' << u8string(r.Ptr) << "] {" << *(r.ptr_to_string) << '}';
        //#else
        return os << (r.Ptr ? r.Ptr : u8"nullptr");
        //#endif
    }
};  //class Iterator8
#undef RLIST

///////////////////////////////////////////////////////////////////////////////////////////////////////////////
template<class Tstr>
[[nodiscard]] constexpr 
Iterator8<Tstr> operator+(typename Iterator8<Tstr>::difference_type _Off, const Iterator8<Tstr>& _Next) noexcept {
    return _Next += _Off;
}
template<class Tstr>
[[nodiscard]] constexpr
#if _ITERATOR_DEBUG_LEVEL >= 1
auto diff_ptr(const Iterator8<Tstr>& first, const Iterator8<Tstr>& last) {
    first._Compat(last);
#else // ^^^ _ITERATOR_DEBUG_LEVEL >= 1 ^^^ // vvv _ITERATOR_DEBUG_LEVEL == 0 vvv
auto diff_ptr(const Iterator8<Tstr>& first, const Iterator8<Tstr>& last) noexcept {
#endif // _ITERATOR_DEBUG_LEVEL

    return static_cast<typename Iterator8<Tstr>::difference_type>(last.rc_ptr()-first.rc_ptr());
}

#if _HAS_CXX20
template <class Tstr>
struct pointer_traits<Iterator8<Tstr>> {
    using pointer         = Iterator8<Tstr>;
    using element_type    = typename pointer::ovalue_type;
    using difference_type = typename pointer::difference_type;
    using ret_type        = typename pointer::ret_type;

    [[nodiscard]] static constexpr 
    ret_type* to_address(const pointer& Iter) noexcept {    // ?????????????????? 

        const auto Rawptr = std::to_address(Iter);
        return Rawptr;
    }
};
#endif // _HAS_CXX20
///////////////////////////////////////////////////////////////////////////////////////////////////////////////
        using iter8 = Iterator8<utf8string>;
} // namespace std
///////////////////////////////////////////////////////////////////////////////////////////////////////////////
//#pragma warning(pop)
