utf8string
==========

����������� �� std::u8string ����� std::utf8string � ����� ���������� � utf8string, ��� ������ � 
��������������� UTF-8 �������� � ��������� Windows.1251.

����� utf8string �������� ���(��, ����� ���) ������ ������ std::u8string,
�������� � ����������� STL.

README.md               
algorithm_utf8.hpp      ��������� ���������� � ���������� ���������� STL.
str8_utils.hpp          �������, ������������ � ��������� ����� � ��������� char8_t 
sample.cpp              ������
utf8string.hpp          ����� utf8string
u8_iterator.hpp         ����� ���������� � utf8string
my_adds.hpp             ��������� �������������� ���������
ansi_tabs.hpp           ������� � ���������
cp1251.hpp              ������� � ���������
stdc++.h                ������������ ���� ��� sample.cpp

�������� � ��������� � windows 10, � ������������ �� Visual Studio 2022 ������ 17.2

���������� � ���������������
cl.exe /permissive- /clr:nostdlib /openmp /Ot /O2 /Ox /sdl /std:c++latest /D WIN32 /D NDEBUG /diagnostics:caret /Zc:lambda,hiddenFriend,implicitNoexcept,externConstexpr /analyze- /nologo /W4 /MP /GS /Gm- /GF /GL /Gw /Oi /Zc:preprocessor,strictStrings-,forScope,__cplusplus,inline,wchar_t,auto /GA /GR /EHac /Qpar /Qpar-report:1 /fp:precise /TP test.cpp
                                ���
cl.exe -permissive- /analyze- /Ot /O2 /Ox /sdl /std:c++latest /D WIN32 /D NDEBUG /await:strict /diagnostics:caret /Zc:implicitNoexcept,externConstexpr /MT /openmp /nologo /W4 /MP /GS /Gm- /GF /GL /Gw /Oi /Zc:preprocessor,strictStrings-,forScope,__cplusplus,inline,wchar_t,auto /GA /GR /EHac /Qpar /Qpar-report:1 /fp:precise /TP test.cpp
