#pragma once

#include<cstddef>
#include<ctime>
#include<xin_algobase.h>
#include<xin_memory.h>
#include<xin_heap_algo.h>
#include<xin_functional.h>

namespace XinSTL{
    //***************************************************************************
    //all_of
    //检查[first,last)内是否全部元素都满足一元操作unary_pred为true的情况
    //否则返回false
    template<class InputIterator,class UnaryPredicate>
    bool all_of(InputIterator first,InputIterator last,UnaryPredicate unary_pred){
        for(;first != last;++first){
            if(!unary_pred(*first)){
                return false;
            }
        }
        return true;
    }

    //****************************************************************************
    //any_of
    //检查[first,last)内是否存在某个元素满足一元操作unary_pred为true的情况
    //否则返回false
    template<class InputIterator,class UnaryPredicate>
    bool any_of(InputIterator first,InputIterator last,UnaryPredicate unary_pred){
        for(;first != last;++first){
            if(unary_pred(*first)){
                return true;
            }
        }
        return false;
    }

    //****************************************************************************
    //none_of
    //检查[first,last)内是否全部元素不满足一元函数unary_pred为true的情况
    //否则返回false
    template<class InputIterator,class UnaryPredicate>
    bool none_of(InputIterator first,InputIterator last,UnaryPredicate unary_pred){
        return !any_of(first,last,unary_pred);
    }

    //****************************************************************************
    //count
    //对[first,last)区间内的元素与给定值进行比较，缺省使用operator==,返回元素相等的个数
    template<class InputIterator,class T>
    size_t count(InputIterator first,InputIterator last,const T& value){
        size_t n = 0;
        for(;first != last;++first){
            if(*first == value){
                n++;
            }
        }
        return n;
    }

    //****************************************************************************
    //count_if
    //对[first,last)区间内的每个元素进行一元unary_pred操作，返回结果为true的个数
    template<class InputIterator,class UnaryPredicate>
    size_t count_if(InputIterator first,InputIterator last,UnaryPredicate unary_pred){
        size_t n = 0;
        for(;first != last;first++){
            if(unary_pred(*first)){
                n++;
            }
        }
        return n;
    }

    //****************************************************************************
    //find
    //在[first,last)区间内找到等于value的元素，返回指向该元素的迭代器
    template<class InputIterator,class T>
    InputIterator find(InputIterator first,InputIterator last,const T& value){
        while(first != last && *first != value){
            first++;
        }
        return first;
    }

    //****************************************************************************
    //find_if
    //在[first,last)区间内找到第一个令一元操作unary_pred为true的元素并返回指向该元素的迭代器
    template<class InputIterator,class UnaryPredicate>
    InputIterator find_if(InputIterator first,InputIterator last,UnaryPredicate unary_pred){
        while(first != last && !unary_pred(*first)){
            first++;
        }
        return first;
    }

    //****************************************************************************
    //find_if_not
    //在[first,last)区间内找到第一个令一元操作unary_pred为false的元素并返回指向该元素的迭代器
    template<class InputIterator,class UnaryPredicate>
    InputIterator find_if_not(InputIterator first,InputIterator last,UnaryPredicate unary_pred){
        while(first != last && unary_pred(*first)){
            first++;
        }
        return first;
    }

    //****************************************************************************
    //search
    //在[first1,last1)中查找[first2,last2)的首次出现点
    template<class ForwardIterator1,class ForwardIterator2>
    ForwardIterator1 search(ForwardIterator1 first1,ForwardIterator1 last1,ForwardIterator2 first2,ForwardIterator2 last2){
        auto d1 = XinSTL::distance(first1,last1);
        auto d2 = XinSTL::distance(first2,last2);

        if(d1 < d2){
            return last1;
        }
        auto current1 = first1;
        auto current2 = first2;
        while(current2 != last2){
            if(*current1 == *current2){
                current1++;
                current2++;
                //一直到current2 == last2表示匹配成功
            }else{
                if(d1 == d2){
                    return last1;
                    //长度如果一样，并且一旦匹配失败，则表示不可能存在子序列
                }else{
                    //以当前first1起始的子序列不匹配，first1++
                    first1++;
                    current1 = first1;
                    current2 = first2;
                    d1--;
                }
            }
        }
        return first1;
    }

    //重载版本使用函数对象comp代替比较操作
    template<class ForwardIterator1,class ForwardIterator2,class Compare>
    ForwardIterator1 search(ForwardIterator1 first1,ForwardIterator1 last1,ForwardIterator2 first2,ForwardIterator2 last2,Compare comp){
        auto d1 = XinSTL::distance(first1,last1);
        auto d2 = XinSTL::distance(first2,last2);
        if(d1 < d2){
            return last1;
        }
        auto current1 = first1;
        auto current2 = first2;
        while(current2 != last2){
            if(comp(*current1,*current2)){
                current1++;
                current2++;
            }else{
                if(d1 == d2){
                    return last1;
                }else{
                    first1++;
                    current1 = first1;
                    current2 = first2;
                    d1--;
                }
            }
        }
        return first1;
    }

    //***************************************************************************
    //search_n
    //在[first,last)中查找连续n个value所形成的子序列，返回一个迭代器指向该子序列的起始处
    template<class ForwardIterator,class Size,class T>
    ForwardIterator search_n(ForwardIterator first,ForwardIterator last,Size n,const T& value){
        if(n <= 0){
            return first;
        }else{
            first = XinSTL::find(first,last,value);
            while(first != last){
                auto m = n-1;
                auto i = first;
                i++;
                while(i != value && m != 0 && *i == value){
                    i++;
                    m--;
                }
                if(m == 0){
                    return first;
                }else{
                    first = XinSTL::find(i,last,value);
                }
            }
            return last;
        }
    }

    //重逢版本使用函数对象comp代替比较操作
    template<class ForwardIterator,class Size,class T,class Compare>
    ForwardIterator search_n(ForwardIterator first,ForwardIterator last,Size n,const T& value,Compare comp){
        if(n <= 0){
            return first;
        }
        while(first != last){
            if(comp(*first,value)){
                break;
            }
            first++;
        }
        while(first != last){
            auto m = n-1;
            auto i = first;
            i++;
            while(i != last && m != 0 && comp(*i,value)){
                i++;
                m--;
            }
            if(m == 0){
                return first;
            }else{
                while(i != first){
                    if(comp(*i,value)){
                        break;
                    }
                    i++;
                }
                first = i;
            }
        }
        return last;
    }

    //***************************************************************************
    //find_end
    //在[first1,last1)区间中查找[first2,last2)最后一次出现的地方，若不存在返回last1
    //find_end_dispatch的forward_iterator_tag版本
    template<class ForwardIterator1,class ForwardIterator2>
    ForwardIterator1 find_end_dispatch(ForwardIterator1 first1,ForwardIterator1 last1,ForwardIterator2 first2,ForwardIterator2 last2,forward_iterator_tag,forward_iterator_tag){
        if(first2 == last2){
            return last1;
        }

        auto result = last1;
        while(true){
            //利用search查找某个子序列的首次出现点，找不到则返回last1
            auto new_result = XinSTL::search(first1,last1,first2,last2);
            if(new_result == last1){
                return result;
            }else{
                result = new_result;
                first1 = new_result;
                first1++;
            }
        }
    }

    //find_end_dispatch的bidirectional_iteartor_tag版本
    template<class BidirectionalIterator1,class BidirectionalIterator2>
    BidirectionalIterator1 find_end_dispatch(BidirectionalIterator1 first1,BidirectionalIterator1 last1,BidirectionalIterator2 first2,BidirectionalIterator2 last2,bidirectional_iterator_tag,bidirectional_iterator_tag){
        using reviter1 = reverse_iterator<BidirectionalIterator1>;
        using reviter2 = reverse_iterator<BidirectionalIterator2>;

        reviter1 rlast1(first1);
        reviter2 rlast2(first2);
        reviter1 rresult = XinSTL::search(reviter1(last1),rlast1,reviter2(last2),rlast2);
        if(rresult == rlast1){
            return rlast1;
        }else{
            auto result = rresult.base();
            XinSTL::advance(result,-XinSTL::distance(first2,last2));
            return result;
        }
    }

    template<class ForwardIterator1,class ForwardIteartor2>
    ForwardIterator1 find_end(ForwardIterator1 first1,ForwardIterator1 last1,ForwardIteartor2 first2,ForwardIteartor2 last2){
        using Category1 = iterator_traits<ForwardIterator1>;
        using Category2 = iterator_traits<ForwardIteartor2>;
        return XinSTL::find_end_dispatch(first1,last1,first2,last2,Category1(),Category2());
    }

    //重载版本使用函数对象comp代替比较操作
    //find_end_dispatch的forward_iterator_tag版本
    template<class ForwardIteartor1,class ForwardIterator2,class Compare>
    ForwardIteartor1 find_end_dispatch(ForwardIteartor1 first1,ForwardIteartor1 last1,ForwardIterator2 first2,ForwardIterator2 last2,forward_iterator_tag,forward_iterator_tag,Compare comp){
        if(first2 == last2){
            return last1;
        }

        auto result = last1;
        while(true){
            //利用search查找某个子序列的首次出现点
            //找不到则返回last1
            auto new_result = XinSTL::search(first1,last1,first2,last2,comp);
            if(new_result == last1){
                return result;
            }else{
                result = new_result;
                first1 = new_result;
                first1++;
            }
        }
    }

    //find_end_dispatch的bidirectional_iterator_tag版本
    template<class BidirectionalIterator1,class BidirectionalIterator2,class Compare>
    BidirectionalIterator1 find_end_dispatch(BidirectionalIterator1 first1,BidirectionalIterator1 last1,BidirectionalIterator2 first2,BidirectionalIterator2 last2,bidirectional_iterator_tag,bidirectional_iterator_tag,Compare comp){
        using reviter1 = reverse_iterator<BidirectionalIterator1>;
        using reviter2 = reverse_iterator<BidirectionalIterator2>;
        reviter1 rlast1(first1);
        reviter2 rlast2(first2);
        reviter1 rresult = XinSTL::search(reviter1(last1),rlast1,reviter2(last2),rlast2,comp);
        if(rresult == rlast1){
            return last1;
        }
            
        auto result = rresult.base();
        XinSTL::advance(result,-XinSTL::distance(first2,last2));
        return result;
    }

    template<class ForwardIterator1,class ForwardIterator2,class Compare>
    ForwardIterator1 find_end(ForwardIterator1 first1,ForwardIterator1 last1,ForwardIterator2 first2,ForwardIterator2 last2,Compare comp){
        using Category1 = typename iterator_traits<ForwardIterator1>::iterator_category;
        using Category2 = typename iterator_traits<ForwardIterator2>::iterator_category;
        return XinSTL::find_end_dispatch(first1,last1,first2,last2,Category1(),Category2(),comp);
    }

    //****************************************************************************
    //find_first_of
    //在[first1,last1)中查找[first2,last2)中的某些元素
    //返回指向第一次出现的元素的迭代器
    template<class InputIterator,class ForwardIterator>
    InputIterator find_first_of(InputIterator first1,InputIterator last1,ForwardIterator first2,ForwardIterator last2){
        for(;first1 != last1;first1++){
            for(auto it = first2;it != last2;it++){
                if(*first1 == *it){
                    return first1;
                }
            }
        }
        return last1;
    }

    //重载版本使用函数对象comp代替比较操作
    template<class InputIterator,class ForwardIterator,class Compare>
    InputIterator find_first_of(InputIterator first1,InputIterator last1,ForwardIterator first2,ForwardIterator last2,Compare comp){
        for(;first1 != last1;first1++){
            for(auto it = first2;it != last2;it++){
                if(comp(*first1,*it)){
                    return first1;
                }
            }
        }
        return last1;
    }

    //***************************************************************************
    //for_each
    //使用一个函数对象f对[first,last)区间内的每一个元素执行一个operator()操作
    //但不改变元素内容
    //f()可返回一个值，但该值会被忽略
    template<class InputIterator,class Function>
    Function for_each(InputIterator first,InputIterator last,Function f){
        for(;first != last;++first){
            f(*first);
        }
        return f;
    }
    
}