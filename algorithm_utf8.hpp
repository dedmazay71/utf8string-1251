//
// algorithm_utf8.hpp   -- Alexander Prilepsky 2022
//
// ���� ������������ ���� �������� ������� �� ���������� ���������� ��� ������ � UTF-8 �������� ��
// �����  std::utf8string � ��� �����������.
//
// ����������� ����������� ��������������� "��� ����", ��� �����-���� ��������,
// ����� ��� ���������������, �������, �� �� ������������� ����������
// �������� �����������, ����������� ��� ������������ ���� � ���������� ���������.
// �� � ���� ������ ����� �� ����� ��������������� �� �����-���� ���������, ������ ���
// ���� ���������������, ���� �� � ���� ��������, ������� ��� ���� �������,
// ���������� ��, ��� � ����� � ����������� ������������ ��� �������������� ���
// ������ ������ � ����������� ������������.
//
//                      c++ Visual Studio 2022 version 17.2
//
///////////////////////////////////////////////////////////////////////////////////////////////////////////////
#pragma once

namespace std {
//////////////////////////////////////////////////////////////////////////////////////////////////////////////
[[nodiscard]] constexpr
std::iter_diff_t<iter8> distance(const iter8& first, const iter8& last) {
        return last - first;
}
//////////////////////////////////////////////////////////////////////////////////////////////////////////////
//                                      ���������� ��������
[[nodiscard]] inline
iter8 prev_iter(const iter8& it_src, std::iter_diff_t<iter8>const pos = 1) { 
    return  it_src - pos; 
}
//                                      ��������� ��������
[[nodiscard]] inline
iter8 next_iter(const iter8& it_src, std::iter_diff_t<iter8>const pos = 1) { 
    return it_src + pos;
}
//--------------------------------------------------------------------------------------
[[nodiscard]] inline
iter8 next(const iter8& first, std::iter_diff_t<iter8> pos = 1) { // increment iterator
    return next_iter(first, pos);
}
[[nodiscard]] inline
iter8 prev(const iter8& first, std::iter_diff_t<iter8> pos = 1) { // decrement iterator
    return prev_iter(first, pos);
}
////////////////////////////////////////////////  ����������  /////////////////////////////////////////////////
//                                                quick_sort                                                   
//                                  ������� ���������� �� ������ ���������� STL
//-------------------------------------------------------------------------------------------------------------
template< typename BidirectionalIterator, typename Compare> constexpr
std::enable_if_t<(std::is_class_v<BidirectionalIterator>  &&
                  std::is_any_of_v<typename BidirectionalIterator::pointer, char8_t*, const char8_t*>
                 )>  // �� ��������� void
quick_sort(BidirectionalIterator first, BidirectionalIterator last, Compare&& cmp) {
    if(first < last) {
        BidirectionalIterator left  = first;
        BidirectionalIterator right = last;
        BidirectionalIterator pivot = left++;

        while(left < right) {
            if(cmp(*left, *pivot) ) ++left;
            else {
                while( (*left < *(--right)) && cmp(*pivot, *right) );
                std::iter_swap(left, right);
            }
        }
        --left;
        std::iter_swap(first, left);
        quick_sort(first, left, cmp);
        quick_sort(right, last, cmp);
    }
}
//-----------------------------------------------------  Sort Case  --------------------------------
template<class BidirectionalIterator> constexpr
std::enable_if_t<(std::is_class_v<BidirectionalIterator>  &&
                  std::is_any_of_v<typename BidirectionalIterator::pointer, char8_t*, const char8_t*>
                 )>  // �� ��������� void
quick_sort(const BidirectionalIterator& first, const BidirectionalIterator& last) {
    std::quick_sort(first, last, std::less<>{});
}
inline
void quick_sort(std::utf8string& str) {
    std::quick_sort(str.begin(), str.end(), std::less<>{});
}
//--------------------------------------------------------------------------------------------------
inline
void sort(std::utf8string& str) {
    std::sort(str.begin(), str.end(), std::less<>{});
}
//-----------------------------------------------------  Sort NoCase  ------------------------------
template<class BidirectionalIterator> constexpr
std::enable_if_t<(std::is_class_v<BidirectionalIterator>  &&
                  std::is_any_of_v<typename BidirectionalIterator::pointer, char8_t*, const char8_t*>
                 )>  // �� ��������� void
iquick_sort(const BidirectionalIterator& first, const BidirectionalIterator& last) {
    std::quick_sort(first, last, iless_ansi_8_chars());
}
//------------------------------------------------------------------------------------------------------------
inline
void iquick_sort(std::utf8string& str) {
    std::quick_sort(str.begin(), str.end(), iless_ansi_8_chars());

}
inline
void isort(std::utf8string& str) {
    std::sort(str.begin(), str.end(), iless_ansi_8_chars());
}
///////////////////////////////////////////////////////////////////////////////////////////////////////////////
} // namespace std
