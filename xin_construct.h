//构造和析构的基本工具
#pragma once

#include<new> //placement new
#include<xin_type_traits.h>
#include<xin_iterator.h>
#include<xin_utility.h>

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

    template<class T,class... Args>
    void construct(T* ptr,Args&&... args){
        ::new ((void*)ptr) T(XinSTL::forward<Args>(args)...);
    }

    //destroy将对象析构
    //以下是类类型的析构，根据type_traits采取不同的析构偏特化
    //普通类型，无需采用多余的析构函数
    template<class T>
    void destroy_one(T*,std::true_type){}

    //需要调用指定的析构函数
    template<class T>
    void destroy_one(T* pointer,std::false_type){
        if(pointer != nullptr){
            pointer->~T();
        }
    }

    template<class T>
    void destroy(T* pointer){
        destroy_one(pointer,std::is_trivially_destructible<T>{});
    }

    //以下是迭代器的析构，也是根据type_traits的不同采取不同的析构策略
    template<class ForwardIterator>
    void destroy_category(ForwardIterator,ForwardIterator,std::true_type){}

    template<class ForwardIterator>
    void destroy_category(ForwardIterator first,ForwardIterator last,std::false_type){
        for(;first != last;first++){
            destroy(&*first);
        }
    }

    template<class ForwardIterator>
    void destroy(ForwardIterator first,ForwardIterator last){
        destroy_category(first,last,std::is_trivially_destructible<ForwardIterator>::value_type{});
    }

}
