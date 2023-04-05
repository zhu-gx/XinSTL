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
    //move_backward
    //将[first,last)区间的元素移动到[result-(last-first),result)内

    //bidirectional_iterator_tag
    template<class BidirectionalIterator1,class BidirectionalIterator2>
    BidirectionalIterator2 unchencked_move_backward_category(BidirectionalIterator1 first,BidirectionalIterator1 last,BidirectionalIterator2 result,XinSTL::bidirectional_iterator_tag){
        while(first != last){
            *(--result) = XinSTL::move(*(--last));
        }
        return result;
    }

    //random_access_iterator_tag
    template<class RandomIterator1,class RandomIterator2>
    RandomIterator2 unchencked_move_backward_category(RandomIterator1 first,RandomIterator1 last,RandomIterator2 result,XinSTL::random_access_iterator_tag){
        for(auto n = last - first;n > 0;n--){
            *(--result) = XinSTL::move(*--last);
        }
        return result;
    }

    template<class BidirectionalIterator1,class BidirectionalIterator2>
    BidirectionalIterator2 unchencked_move_backward(BidirectionalIterator1 first,BidirectionalIterator1 last,BidirectionalIterator2 result){
        return unchencked_move_backward_category(first,last,result,iterator_category(first));
    }

    //为trivially_copy_assgnable提供特化版本
    //具备普通拷贝赋值运算符的操作对象 采用memmove进行拷贝复制
    template<class T,class U>
    typename std::enable_if<
        std::is_same<typename std::remove_const<T>::type,U>::value &&
        std::is_trivially_move_assignable<U>::value,U*>::type
    unchencked_move_backward(T* first,T* last,U* result){
        const size_t n = static_cast<size_t>(last - first);
        if(n != 0){
            result -= n;
            std::memmove(result,first,n * sizeof(U));
        }
        return result;
    }

    template<class BidirectionalIterator1,class BidirectonalIterator2>
    BidirectonalIterator2 move_backward(BidirectionalIterator1 first,BidirectonalIterator2 last,BidirectonalIterator2 result){
        return unchencked_move_backward(first,last,result);
    }

    //****************************************************************************
    //equal
    //比较在[first,last)区间上的元素是否相等
    template<class InputIterator1,class InputIterator2>
    bool equal(InputIterator1 first1,InputIterator1 last1,InputIterator2 first2){
        for(;first1 != last1;++first1,++first2){
            if(*first1 != *first2){
                return false;
            }
        }
        return true;
    }

    //comp重载版本
    template<class InputIterator1,class InputIterator2,class Compare>
    bool equal(InputIterator1 first1,InputIterator1 last1,InputIterator2 first2,InputIterator2 last2,Compare comp){
        for(;first1 != last1;++first1,++first2){
            if(!comp(*first1,*first2)){
                return false;
            }
        }
        return true;
    }

    //***************************************************************************
    //fill_n
    //从first位置开始填充n个值
    template<class OutputIterator,class Size,class T>
    OutputIterator unchecked_fill_n(OutputIterator first,Size n,const T& value){
        for(;n > 0;--n,++first){
            *first = value;
        }
        return first;
    }

    //为one-byte类型提供特化版本
    template<class T,class Size,class U>
    typename std::enable_if<
        std::is_integral<T>::value && sizeof(T) == 1 &&
        !std::is_same<T,bool>::value &&
        std::is_integral<U>::value && sizeof(U) == 1,T*>::type
    unchecked_fill_n(T* first,Size n,U value){
        if(n > 0){
            std::memset(first,(unsigned char)value,(size_t)(n));
        }
        return first + n;
    }

    template<class OutputIterator,class Size,class T>
    OutputIterator fill_n(OutputIterator first,Size n,const T& value){
        return unchecked_fill_n(first,n,value);
    }

    //***************************************************************************
    //fill
    //为[first,last)区间内的所有元素填充新值

    template<class ForwardIterator,class T>
    void fill_category(ForwardIterator first,ForwardIterator last,const T& value,XinSTL::forward_iterator_tag){
        for(;first != last;++first){
            *first = value;
        }
    }

    template<class RandomIterator,class T>
    void fill_category(RandomIterator first,RandomIterator last,const T& value,XinSTL::random_access_iterator_tag){
        fill_n(first,last - first,value);
    }

    template<class ForwardIterator,class T>
    void fill(ForwardIterator first,ForwardIterator last,const T& value){
        fill_category(first,last,value,iterator_category(first));
    }

    //****************************************************************************
    //lexicographical_compare
    //以字典序排列对两个序列进行比较，当在某一位置发现第一组不相等元素时：
    //1 如果第一序列的元素较小，返回true，否则返回false
    //2 如果到达last1而未到last2 返回true
    //3 如果到达last2而未到last1 返回false
    //4 如果同时到达last1和last2 返回false
    template<class InputIterator1,class InputIterator2>
    bool lexicographical_compare(InputIterator1 first1,InputIterator1 last1,InputIterator2 first2,InputIterator2 last2){
        for(;first1 != last1 && first2 != last2;++first1,++first2){
            if(*first1 < *first2){
                return true;
            }else if(*first1 > *first2){
                return false;
            }
        }
        return first1 == last1 && first2 != last2;
    }

    // 重载版本使用函数对象 comp 代替比较操作
    template <class InputIter1, class InputIter2, class Compred>
    bool lexicographical_compare(InputIter1 first1, InputIter1 last1,
                                InputIter2 first2, InputIter2 last2, Compred comp)
    {
        for (; first1 != last1 && first2 != last2; ++first1, ++first2)
        {
            if (comp(*first1, *first2)){
                return true;
            }else if (comp(*first2, *first1)){
                return false;
            }
        }
        return first1 == last1 && first2 != last2;
    }

    // 针对 const unsigned char* 的特化版本
    bool lexicographical_compare(const unsigned char* first1,
                                const unsigned char* last1,
                                const unsigned char* first2,
                                const unsigned char* last2){
        const auto len1 = last1 - first1;
        const auto len2 = last2 - first2;
        // 先比较相同长度的部分
        const auto result = std::memcmp(first1, first2, XinSTL::min(len1, len2));
        // 若相等，长度较长的比较大
        return result != 0 ? result < 0 : len1 < len2;
    }

    //***************************************************************************
    //平行比较两组序列 找到失配的两对元素 返回一对迭代器
    template <class InputIter1, class InputIter2>
    XinSTL::pair<InputIter1, InputIter2> 
    mismatch(InputIter1 first1, InputIter1 last1, InputIter2 first2){
        while (first1 != last1 && *first1 == *first2)
        {
            ++first1;
            ++first2;
        }
        return XinSTL::pair<InputIter1, InputIter2>(first1, first2);
    }

    // 重载版本使用函数对象 comp 代替比较操作
    template <class InputIter1, class InputIter2, class Compred>
    XinSTL::pair<InputIter1, InputIter2> 
    mismatch(InputIter1 first1, InputIter1 last1, InputIter2 first2, Compred comp){
        while (first1 != last1 && comp(*first1, *first2)){
            ++first1;
            ++first2;
        }
        return XinSTL::pair<InputIter1, InputIter2>(first1, first2);
    }

}