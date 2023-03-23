//构造和析构的基本工具
#ifndef __XinSTLconstruct__
#define __XinSTLconstruct__

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
        //value_type是一个iterator中的函数，返回迭代器指向类型的指针
    }

    //判断元素的type_traits是否有trivial destruct
    template<class ForwardIterator,class T>
    inline void __destroy(ForwardIterator first,ForwardIterator last,T*){
        typedef typename __type_traits<T>::has_trivial_destruct trivial_destructor;
        __destroy_aux(first,last,trivial_destructor);
    }

    //根据type_traits中的trivial_destructor采用效率更高的destroy
    //当type_traits表示有non-trivial destructor,即需要调用析构析构
    template<class ForwardIterator>
    inline void __destroy_aux(ForwardIterator first,ForwardIterator last,_false_type){
        for(;first < last;++first){
            destroy(&*first);
        }
    }

    //当type_traits表示没有non-trivial destructor
    //那么不需要一次次调用析构函数
    template<class ForwardIterator>
    inline void __destroy_aux(ForwardIterator,ForwardIterator,_true_type){}

}

#endif //__XinSTLconstruct__