//该头文件用于类型萃取type_traits
//使用stl库的type_traits作为辅助
#pragma once
#include<type_traits>

namespace XinSTL{
//主要沿用GNU STL的标准type_traits
//方便测试

template<class T,T v>
struct m_intergral_constant
{
    static constexpr T value = v;
};

template<bool b>
using m_bool_constant = m_intergral_constant<bool,b>;

using m_true_type = m_bool_constant<true>;
using m_false_type = m_bool_constant<false>;

//为is_pair类型设定type_traits

template<class T1,class T2>
struct pair;

template<class T>
struct is_pair : XinSTL::m_false_type{};

template<class T1,class T2>
struct is_pair<XinSTL::pair<T1,T2>> : XinSTL::m_true_type{};


}