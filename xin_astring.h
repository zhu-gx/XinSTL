#pragma once

// 定义了 string, wstring, u16string, u32string 类型

#include<xin_basic_string.h>

namespace XinSTL{

using string    = XinSTL::basic_string<char>;
using wstring   = XinSTL::basic_string<wchar_t>;
using u16string = XinSTL::basic_string<char16_t>;
using u32string = XinSTL::basic_string<char32_t>;

}