//Ϊ������ԣ�������xin_type_traits�汾

#pragma once
//type traits������Ҫͨ���ж��ͱ�����ԣ�ѡ����Ч����ߵĴ�ʩ
//�Ƿ�߱�non-trivial default ctor
//�Ƿ�߱�non-trivial copy ctor
//�Ƿ�߱�non-trivial assignment operator
//�Ƿ�߱�non-trivial dtor
 
//���class�ں�ָ���Ա�����Ҷ��������ڴ涯̬����
//��ô���class����Ҫʵ���Լ���non_trivial_xxx

namespace XinSTL {
	//������ֻ���class object��ʽ�Ĳ����������Ƶ�
	//�����ȡ�Ľ��Ӧ���������հ׵�classes
	//�ȿ��Ա�ʾ��٣��ֲ���������⸺��
	struct _true_type{};
	struct _false_type{};

	//�����汾��type_traits������Ϊ_false_type
	//�ڲ�ȷ�����ػ��汾�ǲ�����صİ汾
	template<class type>
	struct _type_traits {
		typedef _false_type has_trivial_default_constructor;
		typedef _false_type has_trivial_copy_constructor;
		typedef _false_type has_trivial_assignment_operator;
		typedef _false_type has_trivial_destructor;
		typedef _false_type is_POD_type;
	};

	//����bool���͵���ģ���ػ��汾
	template<> struct _type_traits<bool>
	{
		typedef _true_type		has_trivial_default_constructor;
		typedef _true_type		has_trivial_copy_constructor;
		typedef _true_type		has_trivial_assignment_operator;
		typedef _true_type		has_trivial_destructor;
		typedef _true_type		is_POD_type;
	};

	//����char���͵���ģ���ػ��汾
	template<> struct _type_traits<char> {
		typedef _true_type has_trivial_default_constructor;
		typedef _true_type has_trivial_copy_constructor;
		typedef _true_type has_trivial_assignment_operator;
		typedef _true_type has_trivial_destructor;
		typedef _true_type is_POD_type;
	};

	//����signed char���͵���ģ���ػ��汾
	template<> struct _type_traits<signed char> {
		typedef _true_type has_trivial_default_constructor;
		typedef _true_type has_trivial_copy_constructor;
		typedef _true_type has_trivial_assignment_operator;
		typedef _true_type has_trivial_destructor;
		typedef _true_type is_POD_type;
	};

	//����unsigned char���͵���ģ���ػ��汾
	template<> struct _type_traits<unsigned char> {
		typedef _true_type has_trivial_default_constructor;
		typedef _true_type has_trivial_copy_constructor;
		typedef _true_type has_trivial_assignment_operator;
		typedef _true_type has_trivial_destructor;
		typedef _true_type is_POD_type;
	};

	//����wchar_t���͵���ģ���ػ��汾
	template<> struct _type_traits<wchar_t>
	{
		typedef _true_type		has_trivial_default_constructor;
		typedef _true_type		has_trivial_copy_constructor;
		typedef _true_type		has_trivial_assignment_operator;
		typedef _true_type		has_trivial_destructor;
		typedef _true_type		is_POD_type;
	};

	//����short���͵���ģ���ػ��汾
	template<> struct _type_traits<short> {
		typedef _true_type has_trivial_default_constructor;
		typedef _true_type has_trivial_copy_constructor;
		typedef _true_type has_trivial_assignment_operator;
		typedef _true_type has_trivial_destructor;
		typedef _true_type is_POD_type;
	};

	//����unsigned short���͵���ģ���ػ��汾
	template<> struct _type_traits<unsigned short> {
		typedef _true_type has_trivial_default_constructor;
		typedef _true_type has_trivial_copy_constructor;
		typedef _true_type has_trivial_assignment_operator;
		typedef _true_type has_trivial_destructor;
		typedef _true_type is_POD_type;
	};

	//����int���͵���ģ���ػ��汾
	template<> struct _type_traits<int> {
		typedef _true_type has_trivial_default_constructor;
		typedef _true_type has_trivial_copy_constructor;
		typedef _true_type has_trivial_assignment_operator;
		typedef _true_type has_trivial_destructor;
		typedef _true_type is_POD_type;
	};

	//����unsigned int���͵���ģ���ػ��汾
	template<> struct _type_traits<unsigned int> {
		typedef _true_type has_trivial_default_constructor;
		typedef _true_type has_trivial_copy_constructor;
		typedef _true_type has_trivial_assignment_operator;
		typedef _true_type has_trivial_destructor;
		typedef _true_type is_POD_type;
	};

	//����long���͵���ģ���ػ��汾
	template<> struct _type_traits<long> {
		typedef _true_type has_trivial_default_constructor;
		typedef _true_type has_trivial_copy_constructor;
		typedef _true_type has_trivial_assignment_operator;
		typedef _true_type has_trivial_destructor;
		typedef _true_type is_POD_type;
	};

	//����unsigned long���͵���ģ���ػ��汾
	template<> struct _type_traits<unsigned long> {
		typedef _true_type has_trivial_default_constructor;
		typedef _true_type has_trivial_copy_constructor;
		typedef _true_type has_trivial_assignment_operator;
		typedef _true_type has_trivial_destructor;
		typedef _true_type is_POD_type;
	};

	//����long long���͵���ģ���ػ��汾
	template<> struct _type_traits<long long>
	{
		typedef _true_type		has_trivial_default_constructor;
		typedef _true_type		has_trivial_copy_constructor;
		typedef _true_type		has_trivial_assignment_operator;
		typedef _true_type		has_trivial_destructor;
		typedef _true_type		is_POD_type;
	};

	//����unsigned long long���͵���ģ���ػ��汾
	template<> struct _type_traits<unsigned long long>
	{
		typedef _true_type		has_trivial_default_constructor;
		typedef _true_type		has_trivial_copy_constructor;
		typedef _true_type		has_trivial_assignment_operator;
		typedef _true_type		has_trivial_destructor;
		typedef _true_type		is_POD_type;
	};

	//����float���͵���ģ���ػ��汾
	template<> struct _type_traits<float> {
		typedef _true_type has_trivial_default_constructor;
		typedef _true_type has_trivial_copy_constructor;
		typedef _true_type has_trivial_assignment_operator;
		typedef _true_type has_trivial_destructor;
		typedef _true_type is_POD_type;
	};

	//����double���͵���ģ���ػ��汾
	template<> struct _type_traits<double> {
		typedef _true_type has_trivial_default_constructor;
		typedef _true_type has_trivial_copy_constructor;
		typedef _true_type has_trivial_assignment_operator;
		typedef _true_type has_trivial_destructor;
		typedef _true_type is_POD_type;
	};

	//����long double���͵���ģ���ػ��汾
	template<> struct _type_traits<long double> {
		typedef _true_type has_trivial_default_constructor;
		typedef _true_type has_trivial_copy_constructor;
		typedef _true_type has_trivial_assignment_operator;
		typedef _true_type has_trivial_destructor;
		typedef _true_type is_POD_type;
	};

	//����ԭ��ָ�����͵���ģ��ƫ�ػ��汾
	template<class T> struct _type_traits<T*> {
		typedef _true_type has_trivial_default_constructor;
		typedef _true_type has_trivial_copy_constructor;
		typedef _true_type has_trivial_assignment_operator;
		typedef _true_type has_trivial_destructor;
		typedef _true_type is_POD_type;
	};

	//���ڳ���ԭ��ָ�����͵���ģ��ƫ�ػ��汾
	template<class T>
	struct _type_traits<const T*>
	{
		typedef _true_type		has_trivial_default_constructor;
		typedef _true_type		has_trivial_copy_constructor;
		typedef _true_type		has_trivial_assignment_operator;
		typedef _true_type		has_trivial_destructor;
		typedef _true_type		is_POD_type;
	};

	//����char*���͵���ģ��ƫ�ػ��汾
	template<> struct _type_traits<char*>
	{
		typedef _true_type		has_trivial_default_constructor;
		typedef _true_type		has_trivial_copy_constructor;
		typedef _true_type		has_trivial_assignment_operator;
		typedef _true_type		has_trivial_destructor;
		typedef _true_type		is_POD_type;
	};

	//����unsigned char*���͵���ģ���ػ��汾
	template<> struct _type_traits<unsigned char*>
	{
		typedef _true_type		has_trivial_default_constructor;
		typedef _true_type		has_trivial_copy_constructor;
		typedef _true_type		has_trivial_assignment_operator;
		typedef _true_type		has_trivial_destructor;
		typedef _true_type		is_POD_type;
	};

	//����signed char*���͵���ģ���ػ��汾
	template<> struct _type_traits<signed char*>
	{
		typedef _true_type		has_trivial_default_constructor;
		typedef _true_type		has_trivial_copy_constructor;
		typedef _true_type		has_trivial_assignment_operator;
		typedef _true_type		has_trivial_destructor;
		typedef _true_type		is_POD_type;
	};

	//����const char*���͵���ģ���ػ��汾
	template<> struct _type_traits<const char*>
	{
		typedef _true_type		has_trivial_default_constructor;
		typedef _true_type		has_trivial_copy_constructor;
		typedef _true_type		has_trivial_assignment_operator;
		typedef _true_type		has_trivial_destructor;
		typedef _true_type		is_POD_type;
	};

	//����const unsigned char*���͵���ģ���ػ��汾
	template<> struct _type_traits<const unsigned char*>
	{
		typedef _true_type		has_trivial_default_constructor;
		typedef _true_type		has_trivial_copy_constructor;
		typedef _true_type		has_trivial_assignment_operator;
		typedef _true_type		has_trivial_destructor;
		typedef _true_type		is_POD_type;
	};

	//����const signed char*���͵���ģ���ػ��汾
	template<> struct _type_traits<const signed char*>
	{
		typedef _true_type		has_trivial_default_constructor;
		typedef _true_type		has_trivial_copy_constructor;
		typedef _true_type		has_trivial_assignment_operator;
		typedef _true_type		has_trivial_destructor;
		typedef _true_type		is_POD_type;
	};
}