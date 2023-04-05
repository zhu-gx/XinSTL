#pragma once

//该头文件包含了XinSTL的基本算法

#include<cstring>
#include<xin_iterator.h>
#include<xin_utility.h>

namespace XinSTL{
    
#ifdef max
#pragma message("#undefing marco max")
#undef max
#endif //max

#ifdef min
#pragma message("#undefing marco min")
#undef min
#endif //min

    //******************************************************************************
    //max
    template<class T>
    const T& max(const T& lhs,const T& rhs){
        return lhs > rhs ? lhs : rhs;
    }

    //使用自定义比较方法comp进行max判断
    //采用函数对象实现
    template<class T,class Compare>
    const T& max(const T& lhs,const T& rhs,Compare comp){
        return comp(lhs,rhs) ? rhs : lhs;
    }

    //****************************************************************************
    //min
    template<class T>
    const T& min(const T& lhs,const T& rhs){
        return lhs < rhs ? lhs : rhs;
    }

    //用函数对象实现
    template<class T,class Compare>
    const T& min(const T& lhs,const T& rhs,Compare comp){
        return comp(lhs,rhs) ? lhs : rhs;
    }

    //***************************************************************************
    //iter_swap,交换迭代器所指的内容
    template<class ForwardIterator1,class ForwardIterator2>
    void iter_swap(ForwardIterator1 lhs,ForwardIterator2 rhs){
        XinSTL::swap(*lhs,*rhs);
    }

    //***************************************************************************
    //copy
    //input_iterator_tag版本
    template<class InputIterator,class OutputIterator>
    OutputIterator unchecked_copy_category(InputIterator first,InputIterator last,OutputIterator result,XinSTL::input_iterator_tag){
        for(;first != last;first++,result++){
            *result = *first;
        }
        return result;
    }

    //random_access_iterator版本
    template<class RandomAccessIterator,class OutputIterator>
    OutputIterator unchecked_copy_category(RandomAccessIterator first,RandomAccessIterator last,OutputIterator result,XinSTL::random_access_iterator_tag){
        for(auto n = last - first;n > 0;n--,first++,result++){
            *result = *first;
        }
        return result;
    }

    //type_traits技巧
    template<class InputIterator,class OutputIterator>
    OutputIterator unchecked_copy(InputIterator first,InputIterator last,OutputIterator result){
        return unchecked_copy_category(first,last,result,iterator_category(first));
    }

    //为trivially_copy_assignable提供特化版本
    template <class Tp, class Up>
    typename std::enable_if<
    std::is_same<typename std::remove_const<Tp>::type, Up>::value &&
    std::is_trivially_copy_assignable<Up>::value,
    Up*>::type
    unchecked_copy(Tp* first, Tp* last, Up* result)
    {
    const auto n = static_cast<size_t>(last - first);
    if (n != 0)
        std::memmove(result, first, n * sizeof(Up));
    return result + n;
    }

    template <class InputIter, class OutputIter>
    OutputIter copy(InputIter first, InputIter last, OutputIter result)
    {
    return unchecked_copy(first, last, result);
    }

    //***************************************************************************
    //copy_backward
    template<class BidirectionalIterator1,class BidirectionalIterator2>
    BidirectionalIterator2 unchencked_copy_backward_category(BidirectionalIterator1 first,BidirectionalIterator1 last,BidirectionalIterator2 result,XinSTL::bidirectional_iterator_tag){
        while(first != last){
            result--;
            last--;
            *result = *last;
        }
        return result;
    }

    template<class RandomIterator,class BidirectionalIterator>
    BidirectionalIterator unchencked_copy_backward_category(RandomIterator first,RandomIterator last,BidirectionalIterator result,XinSTL::random_access_iterator_tag){
        for(auto n = last - first;n > 0;n--){
            result--;
            last--;
            *result = *last;
        }
        return result;
    }

    template<class BidirectionalIterator1,class BidirectionalIterator2>
    BidirectionalIterator2 unchencked_copy_backward(BidirectionalIterator1 first,BidirectionalIterator1 last,BidirectionalIterator2 result){
        return unchencked_copy_backward_category(first,last,result);
    }

    //trivially_copy_assignable特化版本
    template<class T,class U>
    typename std::enable_if<std::is_same<typename std::remove_const<T>::type,U>::value && std::is_trivially_copy_assignable<U>::value,U>::type
    unchencked_copy_backward(T* first,T* last,U* result){
        const auto n = static_cast<size_t>(last - first);
        if(n != 0){
            result -= n;
            std::memmove(result,first,n * sizeof(U));
        }
        return result;
    }

    template<class BidiretionalIterator1,class BidirectionalIterator2>
    BidirectionalIterator2 copy_backward(BidiretionalIterator1 first,BidiretionalIterator1 last,BidirectionalIterator2 result){
        return unchencked_copy_backward(first,last,result);
    }

    //***************************************************************************
    //copy_n
    //把[first,first+n)复制到[result,reuslt+n)上
    template<class InputIterator,class Size,class OutputIterator>
    XinSTL::pair<InputIterator,OutputIterator> unchecked_copy_n(InputIterator first,Size n,OutputIterator result,XinSTL::input_iterator_tag){
        for(;n > 0;--n,++first,++result){
            *result = *first;
        }
        return XinSTL::pair<InputIterator,OutputIterator>(first,result);
    }

    template<class RamdomIterator,class Size,class OutputIterator>
    XinSTL::pair<RamdomIterator,OutputIterator> unchecked_copy_n(RamdomIterator first,Size n,OutputIterator result,XinSTL::random_access_iterator_tag){
        auto last = first + n;
        return XinSTL::pair<RamdomIterator,OutputIterator>(last,XinSTL::copy(first,last,result));
    }

    template<class InputIterator,class Size,class OutputIterator>
    XinSTL::pair<InputIterator,OutputIterator> copy_n(InputIterator first,Size n,OutputIterator result){
        return unchecked_copy_n(first,n,result,iterator_category(first));
    }

    //****************************************************************************
    //move

    //input_iterator_tag
    template<class InputIterator,class OutputIterator>
    OutputIterator unchecked_move_category(InputIterator first,InputIterator last,OutputIterator result,XinSTL::input_iterator_tag){
        for(;first != last;first++,result++){
            *result = XinSTL::move(*first);
        }
        return result;
    }

    //random_access_iterator_tag
    template<class RandomIterator,class OutputIterator>
    OutputIterator unchecked_move_category(RandomIterator first,RandomIterator last,OutputIterator result,XinSTL::random_access_iterator_tag){
        for (auto n = last - first; n > 0; --n, ++first, ++result){
            *result = XinSTL::move(*first);
        }
        return result;
    }

    template<class InputIterator,class OutputIterator>
    OutputIterator unchecked_move(InputIterator first,InputIterator last,OutputIterator result){
        return unchecked_move_category(first,last,result,iterator_category(first));
    }

    // 为 trivially_copy_assignable 类型提供特化版本
    template <class Tp, class Up>
    typename std::enable_if<
        std::is_same<typename std::remove_const<Tp>::type, Up>::value &&
        std::is_trivially_move_assignable<Up>::value,
        Up*>::type
    unchecked_move(Tp* first, Tp* last, Up* result){
        const size_t n = static_cast<size_t>(last - first);
        if (n != 0){
            std::memmove(result, first, n * sizeof(Up));
        }
        return result + n;
    }

    template<class InputIterator,class OutputIterator>
    OutputIterator move(InputIterator first,InputIterator last,OutputIterator result){
        return unchecked_move(first,last,result);
    }

    //***************************************************************************
    //move_forward

}