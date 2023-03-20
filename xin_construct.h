//构造和析构的基本工具
#ifndef __xin__construct__
#define __xin__construct__

#include<new> //placement new
#include<xin_TypeTraits.h>

namespace XinSTL{
    template<class T1,class T2>
    inline void construct(T1* p,const T2& value){
        new(p) T1(value);//placement new 调用T1:T1(value)
    }

    //以下是destroy()的第一个版本，接受一个指针
    template<class T>
    inline void destroy(T* pointer){
        pointer->~T();//调用dtor ~T()
    }
    
    //以下是destroy()的第二个版本，接受两个迭代器
    //此函数设法找出元素的type_traits，以采用最适当措施
    template<class ForwardIterator>
    inline void destroy(ForwardIterator first,ForwardIterator last){
        __destroy(first,last,value_type(first));
    }

    
}

#endif