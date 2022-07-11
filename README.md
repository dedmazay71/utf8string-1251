utf8string
==========

Производный от std::u8string класс std::utf8string и класс итераторов к utf8string, для работы с 
мультибайтовыми UTF-8 строками в кодировке Windows.1251.

Класс utf8string содержит все(ну, почти все) методы класса std::u8string,
работает с библиотекой STL.

README.md               
algorithm_utf8.hpp      некоторые дополнение к алгоритмам библиотеки STL.
str8_utils.hpp          функции, используемые в обработке строк с символами char8_t 
sample.cpp              пример
utf8string.hpp          класс utf8string
u8_iterator.hpp         класс итераторов к utf8string
my_adds.hpp             некоторые дополнительные процедуры
ansi_tabs.hpp           таблицы и процедуры
cp1251.hpp              таблицы и процедуры
stdc++.h                заголовочный файл для sample.cpp

Испытано и проверено в windows 10, с компилятором от Visual Studio 2022 версия 17.2

компиляция с редактированием
cl.exe /permissive- /clr:nostdlib /openmp /Ot /O2 /Ox /sdl /std:c++latest /D WIN32 /D NDEBUG /diagnostics:caret /Zc:lambda,hiddenFriend,implicitNoexcept,externConstexpr /analyze- /nologo /W4 /MP /GS /Gm- /GF /GL /Gw /Oi /Zc:preprocessor,strictStrings-,forScope,__cplusplus,inline,wchar_t,auto /GA /GR /EHac /Qpar /Qpar-report:1 /fp:precise /TP test.cpp
                                или
cl.exe -permissive- /analyze- /Ot /O2 /Ox /sdl /std:c++latest /D WIN32 /D NDEBUG /await:strict /diagnostics:caret /Zc:implicitNoexcept,externConstexpr /MT /openmp /nologo /W4 /MP /GS /Gm- /GF /GL /Gw /Oi /Zc:preprocessor,strictStrings-,forScope,__cplusplus,inline,wchar_t,auto /GA /GR /EHac /Qpar /Qpar-report:1 /fp:precise /TP test.cpp
