//为方便测试，最后采用xin_type_traits版本

#pragma once
//type traits技法主要通过判断型别的特性，选用最效率最高的措施
//是否具备non-trivial default ctor
//是否具备non-trivial copy ctor
//是否具备non-trivial assignment operator
//是否具备non-trivial dtor
 
//如果class内含指针成员，并且对它进行内存动态配置
//那么这个class就需要实现自己的non_trivial_xxx

namespace XinSTL {
	//编译器只会对class object形式的参数做类型推导
	//因此萃取的结果应该是两个空白的classes
	//既可以表示真假，又不会带来额外负担
	struct _true_type{};
	struct _false_type{};

	//泛化版本的type_traits都定义为_false_type
	//在不确定其特化版本是采用最保守的版本
	template<class type>
	struct _type_traits {
		typedef _false_type has_trivial_default_constructor;
		typedef _false_type has_trivial_copy_constructor;
		typedef _false_type has_trivial_assignment_operator;
		typedef _false_type has_trivial_destructor;
		typedef _false_type is_POD_type;
	};

	//关于bool类型的类模板特化版本
	template<> struct _type_traits<bool>
	{
		typedef _true_type		has_trivial_default_constructor;
		typedef _true_type		has_trivial_copy_constructor;
		typedef _true_type		has_trivial_assignment_operator;
		typedef _true_type		has_trivial_destructor;
		typedef _true_type		is_POD_type;
	};

	//关于char类型的类模板特化版本
	template<> struct _type_traits<char> {
		typedef _true_type has_trivial_default_constructor;
		typedef _true_type has_trivial_copy_constructor;
		typedef _true_type has_trivial_assignment_operator;
		typedef _true_type has_trivial_destructor;
		typedef _true_type is_POD_type;
	};

	//关于signed char类型的类模板特化版本
	template<> struct _type_traits<signed char> {
		typedef _true_type has_trivial_default_constructor;
		typedef _true_type has_trivial_copy_constructor;
		typedef _true_type has_trivial_assignment_operator;
		typedef _true_type has_trivial_destructor;
		typedef _true_type is_POD_type;
	};

	//关于unsigned char类型的类模板特化版本
	template<> struct _type_traits<unsigned char> {
		typedef _true_type has_trivial_default_constructor;
		typedef _true_type has_trivial_copy_constructor;
		typedef _true_type has_trivial_assignment_operator;
		typedef _true_type has_trivial_destructor;
		typedef _true_type is_POD_type;
	};

	//关于wchar_t类型的类模板特化版本
	template<> struct _type_traits<wchar_t>
	{
		typedef _true_type		has_trivial_default_constructor;
		typedef _true_type		has_trivial_copy_constructor;
		typedef _true_type		has_trivial_assignment_operator;
		typedef _true_type		has_trivial_destructor;
		typedef _true_type		is_POD_type;
	};

	//关于short类型的类模板特化版本
	template<> struct _type_traits<short> {
		typedef _true_type has_trivial_default_constructor;
		typedef _true_type has_trivial_copy_constructor;
		typedef _true_type has_trivial_assignment_operator;
		typedef _true_type has_trivial_destructor;
		typedef _true_type is_POD_type;
	};

	//关于unsigned short类型的类模板特化版本
	template<> struct _type_traits<unsigned short> {
		typedef _true_type has_trivial_default_constructor;
		typedef _true_type has_trivial_copy_constructor;
		typedef _true_type has_trivial_assignment_operator;
		typedef _true_type has_trivial_destructor;
		typedef _true_type is_POD_type;
	};

	//关于int类型的类模板特化版本
	template<> struct _type_traits<int> {
		typedef _true_type has_trivial_default_constructor;
		typedef _true_type has_trivial_copy_constructor;
		typedef _true_type has_trivial_assignment_operator;
		typedef _true_type has_trivial_destructor;
		typedef _true_type is_POD_type;
	};

	//关于unsigned int类型的类模板特化版本
	template<> struct _type_traits<unsigned int> {
		typedef _true_type has_trivial_default_constructor;
		typedef _true_type has_trivial_copy_constructor;
		typedef _true_type has_trivial_assignment_operator;
		typedef _true_type has_trivial_destructor;
		typedef _true_type is_POD_type;
	};

	//关于long类型的类模板特化版本
	template<> struct _type_traits<long> {
		typedef _true_type has_trivial_default_constructor;
		typedef _true_type has_trivial_copy_constructor;
		typedef _true_type has_trivial_assignment_operator;
		typedef _true_type has_trivial_destructor;
		typedef _true_type is_POD_type;
	};

	//关于unsigned long类型的类模板特化版本
	template<> struct _type_traits<unsigned long> {
		typedef _true_type has_trivial_default_constructor;
		typedef _true_type has_trivial_copy_constructor;
		typedef _true_type has_trivial_assignment_operator;
		typedef _true_type has_trivial_destructor;
		typedef _true_type is_POD_type;
	};

	//关于long long类型的类模板特化版本
	template<> struct _type_traits<long long>
	{
		typedef _true_type		has_trivial_default_constructor;
		typedef _true_type		has_trivial_copy_constructor;
		typedef _true_type		has_trivial_assignment_operator;
		typedef _true_type		has_trivial_destructor;
		typedef _true_type		is_POD_type;
	};

	//关于unsigned long long类型的类模板特化版本
	template<> struct _type_traits<unsigned long long>
	{
		typedef _true_type		has_trivial_default_constructor;
		typedef _true_type		has_trivial_copy_constructor;
		typedef _true_type		has_trivial_assignment_operator;
		typedef _true_type		has_trivial_destructor;
		typedef _true_type		is_POD_type;
	};

	//关于float类型的类模板特化版本
	template<> struct _type_traits<float> {
		typedef _true_type has_trivial_default_constructor;
		typedef _true_type has_trivial_copy_constructor;
		typedef _true_type has_trivial_assignment_operator;
		typedef _true_type has_trivial_destructor;
		typedef _true_type is_POD_type;
	};

	//关于double类型的类模板特化版本
	template<> struct _type_traits<double> {
		typedef _true_type has_trivial_default_constructor;
		typedef _true_type has_trivial_copy_constructor;
		typedef _true_type has_trivial_assignment_operator;
		typedef _true_type has_trivial_destructor;
		typedef _true_type is_POD_type;
	};

	//关于long double类型的类模板特化版本
	template<> struct _type_traits<long double> {
		typedef _true_type has_trivial_default_constructor;
		typedef _true_type has_trivial_copy_constructor;
		typedef _true_type has_trivial_assignment_operator;
		typedef _true_type has_trivial_destructor;
		typedef _true_type is_POD_type;
	};

	//关于原生指针类型的类模板偏特化版本
	template<class T> struct _type_traits<T*> {
		typedef _true_type has_trivial_default_constructor;
		typedef _true_type has_trivial_copy_constructor;
		typedef _true_type has_trivial_assignment_operator;
		typedef _true_type has_trivial_destructor;
		typedef _true_type is_POD_type;
	};

	//关于常量原生指针类型的类模板偏特化版本
	template<class T>
	struct _type_traits<const T*>
	{
		typedef _true_type		has_trivial_default_constructor;
		typedef _true_type		has_trivial_copy_constructor;
		typedef _true_type		has_trivial_assignment_operator;
		typedef _true_type		has_trivial_destructor;
		typedef _true_type		is_POD_type;
	};

	//关于char*类型的类模板偏特化版本
	template<> struct _type_traits<char*>
	{
		typedef _true_type		has_trivial_default_constructor;
		typedef _true_type		has_trivial_copy_constructor;
		typedef _true_type		has_trivial_assignment_operator;
		typedef _true_type		has_trivial_destructor;
		typedef _true_type		is_POD_type;
	};

	//关于unsigned char*类型的类模板特化版本
	template<> struct _type_traits<unsigned char*>
	{
		typedef _true_type		has_trivial_default_constructor;
		typedef _true_type		has_trivial_copy_constructor;
		typedef _true_type		has_trivial_assignment_operator;
		typedef _true_type		has_trivial_destructor;
		typedef _true_type		is_POD_type;
	};

	//关于signed char*类型的类模板特化版本
	template<> struct _type_traits<signed char*>
	{
		typedef _true_type		has_trivial_default_constructor;
		typedef _true_type		has_trivial_copy_constructor;
		typedef _true_type		has_trivial_assignment_operator;
		typedef _true_type		has_trivial_destructor;
		typedef _true_type		is_POD_type;
	};

	//关于const char*类型的类模板特化版本
	template<> struct _type_traits<const char*>
	{
		typedef _true_type		has_trivial_default_constructor;
		typedef _true_type		has_trivial_copy_constructor;
		typedef _true_type		has_trivial_assignment_operator;
		typedef _true_type		has_trivial_destructor;
		typedef _true_type		is_POD_type;
	};

	//关于const unsigned char*类型的类模板特化版本
	template<> struct _type_traits<const unsigned char*>
	{
		typedef _true_type		has_trivial_default_constructor;
		typedef _true_type		has_trivial_copy_constructor;
		typedef _true_type		has_trivial_assignment_operator;
		typedef _true_type		has_trivial_destructor;
		typedef _true_type		is_POD_type;
	};

	//关于const signed char*类型的类模板特化版本
	template<> struct _type_traits<const signed char*>
	{
		typedef _true_type		has_trivial_default_constructor;
		typedef _true_type		has_trivial_copy_constructor;
		typedef _true_type		has_trivial_assignment_operator;
		typedef _true_type		has_trivial_destructor;
		typedef _true_type		is_POD_type;
	};
}