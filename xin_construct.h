//构造和析构的基本工具
#pragma once

#include<new> //placement new
#include<xin_type_traits.h>
#include<xin_iterator.h>

namespace XinSTL{
    //construct构造对象
    template<class T>
    void construct(T* ptr){
        ::new ((void*)ptr) T();
    }

    template<class T1,class T2>
    void construct(T1* ptr,const T2& value){
        ::new ((void*)ptr) T1(value);
    }

    template

}
