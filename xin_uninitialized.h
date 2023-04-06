#pragma once

//该头文件用于对未初始化空间构造元素

#include<xin_algobase.h>
#include<xin_construct.h>
#include<xin_utility.h>
#include<xin_type_traits.h>
#include<xin_iterator.h>

namespace XinSTL{
    //***************************************************************************
    //unintialized_copy
    //把[first,last)上的元素复制到以result为起始的空间
    //返回复制接受的位置
    template<class InputIterator,class ForwardIterator>
    ForwardIterator unchecked_unitialized_copy(InputIterator first,InputIterator last,ForwardIterator result,std::true_type){
        return XinSTL::copy(first,last,result);
    }

    template<class InputIterator,class ForwardIterator>
    ForwardIterator unchecked_unitialized_copy()
}