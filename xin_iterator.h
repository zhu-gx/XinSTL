#pragma once

#include<cstddef>
#include<xin_TypeTraits.h>

namespace XinSTL{

//定义五种迭代器类型
//这种迭代器所指的对象，不允许外界改变，只读,支持++
struct input_iterator_tag{};
//只写，支持++
struct output_iterator_tag{};
//允许写入型算法的操作，因此继承自只读，支持++
struct forward_iterator_tag : public input_iterator_tag{};
//双向移动迭代器，当然属于前向迭代器的一种，支持++和--
struct bidirectional_iterator_tag : public forward_iterator_tag{};
//随机访问迭代器，支持++/--/+n/-n/...
struct random_access_iterator_tag : public bidirectional_iterator_tag{};





}