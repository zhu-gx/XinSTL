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
    
    //****************************************************************************
    //adjacent_find
    //找出第一对匹配的相邻元素，缺省使用operator==比较
    //如果找到返回一个迭代器，指向这对元素的第一个元素
    template<class ForwardIterator>
    ForwardIterator adjacent_find(ForwardIterator first,ForwardIterator last){
        if(first == last){
            return last;
        }
        auto next = first;
        while(++next != last){
            if(*first == *next){
                return first;
            }
            first = next;
        }
        return last;
    }

    //重载版本使用函数对象comp代替比较操作
    template<class ForwardIterator,class Compare>
    ForwardIterator adjacent_find(ForwardIterator first,ForwardIterator last,Compare comp){
        if(first == last){
            return last;
        }
        auto next = first;
        while(++next != last){
            if(comp(*first,*next)){
                return first;
            }
            first = next;
        }
        return last;
    }

    //****************************************************************************
    //lower_bound
    //在有序数列[first,last)中查找第一个不小于value的元素
    //并返回指向它的迭代器，若没有，则返回last
    //版本一采用operator<，版本二采用仿函数comp
    //lower_bound需要辅助函数实现细节，实际上使用二分查找实现

    //版本一的forward_iterator_tag版本
    template<class ForwardIterator,class T>
    ForwardIterator __lower_bound(ForwardIterator first,ForwardIterator last,const T& value,forward_iterator_tag){
        auto len = XinSTL::distance(first,last);//求取整个区间的长度len
        auto half = len;//主要用于确定half的size_type
        ForwardIterator middle;
        while(len > 0){
            half = len >> 1;//除以2
            //下面两行令middle指向
            middle = first;
            XinSTL::advance(middle,half);
            if(*middle < value){//如果中间位置的元素值<value
                first = middle;//令first指向中点的下一个位置，重复
                first++;
                len = len - half - 1;//修正len，在右半区域查找
            }else{
                len = half;//同样修正len，在左半区域查找
            }
        }
        return first;
    }

    //版本二的random_access_iterator_tag版本
    template<class RandomIterator,class T>
    RandomIterator __lower_bound(RandomIterator first,RandomIterator last,const T& value,random_access_iterator_tag){
        auto len = last - first;//此处体现与forwar_iterator的不同
        auto half = len;
        RandomIterator middle;
        while(len > 0){
            half = len >> 1;
            middle = first + half;//此处体现与forwar_iterator的不同
            if(*middle < value){
                first = middle + 1;
                len = len - half - 1;
            }else{
                len = half;
            }
        }
        return first;
    }

    template<class ForwardIterator,class T>
    ForwardIterator lower_bound(ForwardIterator first,ForwardIterator last,const T& value){
        return XinSTL::__lower_bound(first,last,value,iterator_category(first));
    }

    //重载版本使用函数对象comp代替比较操作
    //版本二的辅助函数
    //forwar_iterator_tag版本
    template<class ForwardIterator,class T,class Compare>
    ForwardIterator __lower_bound(ForwardIterator first,ForwardIterator last,const T& value,forward_iterator_tag,Compare comp){
        auto len = XinSTL::distance(first,last);
        auto half = len;
        ForwardIterator middle;
        while(len > 0){
            half = len >> 1;
            middle = first;
            XinSTL::advance(middle,half);
            if(comp(*middle,value)){
                first = middle;
                first++;
                len = len - half - 1;
            }else{
                len = half;
            }
        }
        return first;
    }

    //辅助函数的random_access_iterator_tag版本
    template<class RandomIterator,class T,class Compare>
    RandomIterator __lower_bound(RandomIterator first,RandomIterator last,const T& value,random_access_iterator_tag,Compare comp){
        auto len = last - first;
        auto half = len;
        RandomIterator middle;
        while(len > 0){
            half = len >> 1;
            middle = first + half;
            if(comp(*middle,value)){
                first = middle + 1;
                len = len - half - 1;
            }else{
                len = half;
            }
        }
        return first;
    }

    template<class ForwardIterator,class T,class Compare>
    ForwardIterator lower_bound(ForwardIterator first,ForwardIterator last,const T& value,Compare comp){
        return XinSTL::__lower_bound(first,last,value,iterator_category(first),comp);
    }

    //****************************************************************************
    //upper_bound
    //在[first,last)中查找第一个大于value的元素
    //并返回指向它的迭代器，若没有则返回last

    //辅助函数的forward_iterator_tag版本
    template<class ForwardIterator,class T>
    ForwardIterator __upper_bound(ForwardIterator first,ForwardIterator last,const T& value,forward_iterator_tag){
        auto len = XinSTL::distance(first,last);
        auto half = len;
        ForwardIterator middle;
        while(len > 0){
            half = len >> 1;
            middle = first;
            XinSTL::advance(middle,half);
            if(value < *middle){
                len = half;
            }else{
                first = middle;
                first++;
                len = len - half - 1;
            }
        }
        return first;
    }

    //辅助函数的random_access_iterator_tag版本
    template<class RandomIterator,class T>
    RandomIterator __upper_bound(RandomIterator first,RandomIterator last,const T& value,random_access_iterator_tag){
        auto len = last - first;
        auto half = len;
        RandomIterator middle;
        while(len > 0){
            half = len >> 1;
            middle = first + half;
            if(value < *middle){
                len = half;
            }else{
                first = middle + 1;
                len = len - half - 1;
            }
        }
        return first;
    }

    template<class ForwardIterator,class T>
    ForwardIterator upper_bound(ForwardIterator first,ForwardIterator last,const T& value){
        return XinSTL::__upper_bound(first,last,value,iterator_category(first));
    }

    //重载版本使用函数对象comp代替比较操作
    //辅助函数的forward_iterator_tag
    template<class ForwardIterator,class T,class Compare>
    ForwardIterator __upper_bound(ForwardIterator first,ForwardIterator last,const T& value,forward_iterator_tag,Compare comp){
        auto len = XinSTL::distance(first,last);
        auto half = len;
        ForwardIterator middle;
        while(len > 0){
            half = len >> 1;
            middle = first;
            XinSTL::advance(middle,half);
            if(comp(value,*middle)){
                len = half;
            }else{
                first = middle;
                first++;
                len = len - half - 1;
            }
        }
        return first;
    }

    //辅助函数的random_iterator_tag版本
    template<class RandomIterator,class T,class Compare>
    RandomIterator __upper_bound(RandomIterator first,RandomIterator last,const T& value,random_access_iterator_tag,Compare comp){
        auto len = last - first;
        auto half = len;
        RandomIterator middle;
        while(len > 0){
            half = len >> 1;
            middle = first + half;
            if(comp(value,*middle)){
                len = half;
            }else{
                first = middle + 1;
                len = len - half - 1;
            }
        }
        return first;
    }

    template<class ForwardIterator,class T,class Compare>
    ForwardIterator upper_bound(ForwardIterator first,ForwardIterator last,const T& value,Compare comp){
        return XinSTL::__upper_bound(first,last,value,iterator_category(first),comp);
    }

    //***************************************************************************
    //binary_search
    //二分查找，如果在[first,last)内有等同于value的元素
    //返回true，否则，返回false
    template<class ForwardIterator,class T>
    bool binary_search(ForwardIterator first,ForwardIterator last,const T& value){
        auto i = XinSTL::lower_bound(first,last,value);
        return i != last && !(value < *i);
    }

    //重载版本使用函数对象comp代替比较操作
    template<class ForwardIterator,class T,class Compare>
    bool binary_search(ForwardIterator first,ForwardIterator last,const T& value,Compare comp){
        auto i = XinSTL::lower_bound(first,last,value);
        return i != last && !comp(value,*i);
    }

    //****************************************************************************
    //equal_range
    //查找[first,last)区间中与value相等的元素所形成的区间
    //返回一对迭代器指向区间首位
    //第一个迭代器指向第一个不小于value的元素，第一个迭代器指向第一个大于value的元素

    //辅助函数的forward_iterator_tag版本
    template<class ForwardIterator,class T>
    XinSTL::pair<ForwardIterator,ForwardIterator>
    equal_range_dispatch(ForwardIterator first,ForwardIterator last,const T& value,forward_iterator_tag){
        auto len = XinSTL::distance(first,last);
        auto half = len;
        ForwardIterator middle,left,right;
        while(len > 0){
            half = len >> 1;
            middle = first;
            XinSTL::advance(middle,half);
            if(*middle < value){
                first = middle;
                first++;
                len = len - half - 1;
            }else if(value < *middle){
                len = half;
            }else{
                left = XinSTL::lower_bound(first,last,value);
                XinSTL::advance(first,len);
                middle++;
                right = XinSTL::upper_bound(middle,first,value);
                return XinSTL::pair<ForwardIterator,ForwardIterator>(left,right);
            }
        }
        return XinSTL::pair<ForwardIterator,ForwardIterator>(last,last);
    }

    //辅助函数equal_range_dispatch的random_access_iterator_tag版本
    template<class RandomIterator,class T>
    XinSTL::pair<RandomIterator,RandomIterator>
    equal_range_dispatch(RandomIterator first,RandomIterator last,const T& value,random_access_iterator_tag){
        auto len = last - first;
        auto half = len;
        RandomIterator middle,left,right;
        while(len > 0){
            half = len >> 1;
            middle = first + half;
            if(*middle < value){
                first = middle + 1;
                len = len - half - 1;
            }else if(value < *middle){
                len = half;
            }else{
                left = XinSTL::lower_bound(first,middle,value);
                middle++;
                right = XinSTL::upper_bound(middle,first+len,value);
                return XinSTL::pair<RandomIterator,RandomIterator>(left,right);
            }
        }
        return XinSTL::pair<RandomIterator,RandomIterator>(last,last);
    }

    template<class ForwardIterator,class T>
    XinSTL::pair<ForwardIterator,ForwardIterator>
    equal_range(ForwardIterator first,ForwardIterator last,const T& value){
        return XinSTL::equal_range_dispatch(first,last,iterator_category(first));
    }

    //重载版本使用函数对象comp代替比较操作
    //辅助函数的forward_iterator版本
    template<class ForwardIterator,class T,class Compare>
    XinSTL::pair<ForwardIterator,ForwardIterator>
    equal_range_dispatch(ForwardIterator first,ForwardIterator last,const T& value,forward_iterator_tag,Compare comp){
        auto len = XinSTL::distance(first,last);
        auto half = len;
        ForwardIterator middle,left,right;
        while(len > 0){
            half = len >> 1;
            middle = first;
            XinSTL::advance(middle,half);
            if(comp(*middle,value)){
                first = middle;
                first++;
                len = len - half - 1;
            }else if(comp(value,*middle)){
                len = half;
            }else{
                left = XinSTL::lower_bound(first,last,value,comp);
                XinSTL::advance(first,len);
                middle++;
                right = XinSTL::upper_bound(middle,first,value,comp);
                return XinSTL::pair<ForwardIterator,ForwardIterator>(left,right);
            }
        }
        return XinSTL::pair<ForwardIterator,ForwardIterator>(last,last);
    }

    //辅助函数的random_access_iterator版本
    template<class RandomIterator,class T,class Compare>
    XinSTL::pair<RandomIterator,RandomIterator>
    equal_range_dispatch(RandomIterator first,RandomIterator last,const T& value,random_access_iterator_tag,Compare comp){
        auto len = last - first;
        auto half = len;
        RandomIterator middle,left,right;
        while (len > 0){
           half = len >> 1;
           middle = first + half;
           if(comp(*middle,value)){
                first = middle + 1;
                len = len - half - 1;
           }else if(comp(value,*middle)){
                len = half;
           }else{
                left = XinSTL::lower_bound(first,middle,value,comp);
                middle++;
                right = XinSTL::upper_bound(middle,first+len,value,comp);
                return XinSTL::pair<RandomIterator,RandomIterator>(left,right);
           }
        }
        return XinSTL::pair<RandomIterator,RandomIterator>(last,last);
    }

    template<class ForwardIterator,class T,class Compare>
    XinSTL::pair<ForwardIterator,ForwardIterator>
    equal_range_dispatch(ForwardIterator first,ForwardIterator last,const T& value,Compare comp){
        return XinSTL::equal_range_dispatch(first,last,value,iterator_category(first),comp);
    }

    //****************************************************************************
    //generate
    //将函数对象gen的运算结果对[first,last)内的每个元素赋值
    template<class ForwardIterator,class Generator>
    void generate(ForwardIterator first,ForwardIterator last,Generator gen){
        for(;first != last;first++){
            *first = gen();
        }
    }

    //***************************************************************************
    //generate_n
    //用函数对象gen连续对n个元素赋值
    template<class ForwardIterator,class Size,class Generator>
    void generate_n(ForwardIterator first,Size n,Generator gen){
        for(;n > 0;n--,first++){
            *first = gen();
        }
    }

    //****************************************************************************
    //includes
    //判断序列1是否包含序列2
    template<class InputIterator1,class InputIterator2>
    bool includes(InputIterator1 first1,InputIterator1 last1,InputIterator2 first2,InputIterator2 last2){
        while(first1 != last1 && first2 != last2){
            if(*first2 < *first1){
                return false;
            }else if(*first1 < *first2){
                first1++;
            }else{
                first1++;
                first2++;
            }
        }
        return first2 == last2;
    }

    //重载版本使用函数对象comp代替比较操作
    template<class InputIterator1,class InputIterator2,class Compare>
    bool includes(InputIterator1 first1,InputIterator1 last1,InputIterator2 first2,InputIterator2 last2,Compare comp){
        while(first1 != last1 && first2 != last2){
            if(comp(*first2,*first1)){
                return false;
            }else if(comp(*first1,*first2)){
                first1++;
            }else{
                first1++;
                first2++;
            }
        }
        return first2 == last2;
    }

    //***************************************************************************
    //is_heap
    //检查[first,last)内的元素是否为一个堆，如果是，返回true
    template<class RandomIterator>
    bool is_heap(RandomIterator first,RandomIterator last){
        auto n = XinSTL::distance(first,last);
        auto parent = 0;
        for(auto child = 1;child < n;child++){
            if(first[parent] < first[child]){
                return false;
            }
            if((child & 1) == 0){
                //该孩子与父母均已核验完成
                //核验下一节点
                parent++;
            }
        }
        return true;
    }

    //重载版本使用函数对象comp代替比较操作
    template<class RandomIterator,class Compare>
    bool is_heap(RandomIterator first,RandomIterator last,Compare comp){
        auto n = XinSTL::distance(first,last);
        auto parent = 0;
        for(auto child = 1;child < n;child++){
            if(comp(first[parent],first[child])){
                return false;
            }
            if((child & 1) == 0){
                parent++;
            }
        }
        return true;
    }

    //***************************************************************************
    //is_sorted
    //检查[first,last)内的元素是否升序，如果是升序，返回true
    template<class ForwardIterator>
    bool is_sorted(ForwardIterator first,ForwardIterator last){
        if(first == last){
            return true;
        }
        auto next = first;
        next++;
        for(;next != last;first = next,next++){
            if(*next < *first){
                return false;
            }
        }
        return true;
    }

    //重载版本使用函数对象comp代替比较操作
    template<class ForwardIterator,class Compare>
    bool is_sorted(ForwardIterator first,ForwardIterator last,Compare comp){
        if(first == last){
            return true;
        }
        auto next = first;
        next++;
        for(;next != last;first = next,next++){
            if(comp(*next,*first)){
                return false;
            }
        }
        return true;
    }

    //***************************************************************************
    //median
    //找出三个值的中间值
    template<class T>
    const T& median(const T& left,const T& mid,const T& right){
        if(left < mid){
            if(mid < right){
                return mid;//left<mid<right
            }else if(left < right){
                return right;//left < right <= mid
            }else{
                return left;//right <= left < mid
            }
        }else if(left < right){
            return left;//mid <= left < right
        }else if(mid < right){
            return right;//mid < right <= left
        }
        return mid;//right <= mid <= left
    }

    //重载版本使用函数对象comp代替比较操作
    template<class T,class Compare>
    const T& median(const T& left,const T& mid,const T& right,Compare comp){
        if(comp(left,mid)){
            if(comp(mid,right)){
                return mid;
            }else if(comp(left,right)){
                return right;
            }else{
                return left;
            }
        }else if(comp(left,right)){
            return left;
        }else if(comp(mid,right)){
            return right;
        }else{
            return mid;
        }
    }

    //****************************************************************************
    //max_element
    //返回一个迭代器，指向序列中最大的元素
    template<class ForwardIteraor>
    ForwardIteraor max_element(ForwardIteraor first,ForwardIteraor last){
        if(first == last){
            return first;
        }
        auto result = first;
        while(++first != last){
            if(*result < *first){
                result = first;
            }
        }
        return result;
    }

    //重载版本使用函数对象comp代替比较操作
    template<class ForwardIterator,class Compare>
    ForwardIterator max_element(ForwardIterator first,ForwardIterator last,Compare comp){
        if(first == last){
            return first;
        }
        auto result = first;
        while(++first != last){
            if(comp(*result,*first)){
                result = first;
            }
        }
        return result;
    }

    //***************************************************************************
    //min_element
    //返回一个迭代器，指向序列中最小的元素
    template<class ForwardIterator>
    ForwardIterator min_element(ForwardIterator first,ForwardIterator last){
        if(first == last){
            return first;
        }
        auto result = first;
        while(++first != last){
            if(*first < *result){
                result = first;
            }
        }
        return result;
    }

    //重载版本使用函数对象comp代替比较操作
    template<class ForwardIterator,class Compare>
    ForwardIterator min_element(ForwardIterator first,ForwardIterator last,Compare comp){
        if(first == last){
            return first;
        }
        auto result = first;
        while(++first != last){
            if(comp(*first,*result)){
                result = first;
            }
        }
        return result;
    }

    //***************************************************************************
    //swap_ranges
    //将[first1,last1)从first2开始，交换相同个数元素
    //交换的区间长度必须相同，两个序列不能互相重叠
    //返回一个迭代器指向序列二最后一个被交换元素的下一位
    template<class ForwardIterator1,class ForwardIterator2>
    ForwardIterator2 swap_ranges(ForwardIterator1 first1,ForwardIterator1 last1,ForwardIterator2 first2){
        for(;first1 != last1;first1++,first2++){
            XinSTL::iter_swap(first1,first2);
        }
        return first2;
    }

    //***************************************************************************
    //transform
    //第一个版本以函数对象unary_op作用于[first,last)中的每个元素
    //并将结果保存至result中
    //第二个版本以函数对象binary_op作用域两个序列[first1,last1)、[first2,last2)的相同位置
    template<class InputIterator,class OutputIterator,class UnaryOperation>
    OutputIterator transform(InputIterator first,InputIterator last,OutputIterator result,UnaryOperation unary_op){
        for(;first != last;first++,result++){
            *result = unary_op(*first);
        }
        return result;
    }

    template<class InputIterator1,class InputIterator2,class OutputIterator,class BinaryOperation>
    OutputIterator transform(InputIterator1 first1,InputIterator1 last1,InputIterator2 first2,OutputIterator result,BinaryOperation binary_op){
        for(;first1 != last1;first1++,first2++,result++){
            *result = binary_op(*first1,*first2);
        }
        return result;
    }

    //**************************************************************************
    //remove_copy
    //移除区间内指定value相等的元素
    //并将结果复制到以result标示起始位置的容器上
    template<class InputIterator,class OutputIterator,class T>
    OutputIterator remove_copy(InputIterator first,InputIterator last,OutputIterator result,const T& value){
        for(;first != last;first++){
            if(*first != value){
                *result++ = *first;
            }
        }
        return result;
    }

    //***********************************************************************
    //remove
    //移除所有与指定value相等的元素
    //并不从容器中删除这些元素，所以remove和remove_if不适用于array
    template<class ForwradIterator,class T>
    ForwradIterator remove(ForwradIterator first,ForwradIterator last,const T& value){
        first = XinSTL::find(first,last,value);//利用find找出第一个匹配的地方
        auto next = first;
        return first == last ? first : XinSTL::remove_copy(++next,last,first,value);
    }

    //*************************************************************************
    //remove_copy_if
    //移除区间内所有令一元操作unary_pred为true的元素
    //并将结果复制到以result为起始位置的容器上
    template<class InputIterator,class OutputIterator,class UnaryPredicate>
    OutputIterator remove_copy_if(InputIterator first,InputIterator last,OutputIterator result,UnaryPredicate unary_pred){
        for(;first != last;first++){
            if(!unary_pred(*first)){
                *result = *first;
                result++;
            }
        }
        return result;
    }

    //**************************************************************************
    //remove_if
    //移除区间内所有令一元操作unary_pred为true的元素
    template<class ForwardIterator,class UnaryPredicate>
    ForwardIterator remove_if(ForwardIterator first,ForwardIterator last,UnaryPredicate unary_pred){
        first = XinSTL::find_if(first,last,unary_pred);//利用find_if找出第一个匹配的地方
        auto next = first;
        return first == last ? first : XinSTL::remove_copy_if(++next,last,first,unary_pred);
    }

    //***************************************************************************
    //replace
    //将区间内所有的old_value都以new_value替代
    template<class ForwadIterator,class T>
    void replace(ForwadIterator first,ForwadIterator last,const T& old_value,const T& new_value){
        for(;first != last;first++){
            if(*first = old_value){
                *first = new_value;
            }
        }
    }

    //***************************************************************************
    //replace_copy
    //行为与replace类似
    //不同的是将结果复制到result所指的容器中，原序列没有改变
    template<class InputIterator,class OutputIterator,class T>
    OutputIterator replace_copy(InputIterator first,InputIterator last,OutputIterator result,const T& old_value,const T& new_value){
        for(;first != last;first++,result++){
            *result = *first == old_value ? new_value : *first;
        }
        return result;
    }

    //**************************************************************************
    //replace_copy_if
    //行为与replace_if类似，不同的是将结果复制到result所指的容器中
    //原序列没有改变
    template<class InputIterator,class OutputIterator,class UnaryPredicate,class T>
    OutputIterator replace_copy_if(InputIterator first,InputIterator last,OutputIterator result,UnaryPredicate unary_pred,const T& new_value){
        for(;first != last;first++,result++){
            *result = unary_pred(*first) ? new_value : *first;
        }
        return result;
    }

    //*************************************************************************
    //replace_if
    //将区间内所有令一元操作unary_pred为true的元素都用new_value替代
    template<class ForwardIterator,class UnaryPrediacate,class T>
    void replace_if(ForwardIterator first,ForwardIterator last,UnaryPrediacate unary_pred,const T& new_value){
        for(;first != last;first++){
            if(unary_pred(*first)){
                *first = new_value;
            }
        }
    }

    //***************************************************************************
    //reverse
    //将[first,last)区间内的元素反转

    //reverse_dispatch的bidirectional_iterator_tag版本
    template<class BidirectionalIterator>
    void reverse_dispatch(BidirectionalIterator first,BidirectionalIterator last,bidirectional_iterator_tag){
        while(true){
            if(first == last || first == --last){
                return;
            }
            XinSTL::iter_swap(first++,last);
        }
    }

    //reverse_dispatch的random_access_iterator_tag版本
    template<class RandomIterator>
    void reverse_dispatch(RandomIterator first,RandomIterator last,random_access_iterator_tag){
        while(first < last){
            XinSTL::iter_swap(first++,--last);
        }
    }

    template<class Bidirectional>
    void reverse(Bidirectional first,Bidirectional last){
        XinSTL::reverse_dispatch(first,last,iterator_category(first));
    }

    //**************************************************************************
    //reverse_copy
    //行为与reverse类似，不同的是将结果复制到result所指容器中
    template<class BidirectionalIterator,class OutputIterator>
    OutputIterator reverse_copy(BidirectionalIterator first,BidirectionalIterator last,OutputIterator result){
        while(first != last){
            last--;
            *result = *last;
            ++result;
        }
        return result;
    }

    //***************************************************************************
    //random_shuffle
    //将[first,last)内的元素次序随机重排
    //重载版本使用一个产生随机数的函数对象rand
    template<class RandomIterator>
    void random_shuffle(RandomIterator first,RandomIterator last){
        if(first == last){
            return;
        }
        srand((unsigned)time(0));
        for(auto i = first + 1;i != first;i++){
            XinSTL::iter_swap(i,first + (rand()%(i-first+1)));
        }
    }

    //重载版本使用一个产生随机数的函数对象rand
    template<class RandomIterator,class RandomNumberGenerator>
    void random_shuffle(RandomIterator first,RandomIterator last,RandomNumberGenerator& rand){
        if(first == last){
            return;
        }
        auto len = XinSTL::distance(first,last);
        for(auto i = first + 1;i != last;i++){
            XinSTL::iter_swap(i,first+(rand(i-first+1)%len));
        }
    }

    //**************************************************************************
    //rotate
    //将[first,middle)内的元素和[middle,last)内的元素互换
    //可以交换两个长度不同的区间
    //返回交换后middle的位置
    
    //rotate_dispatch的forward_iterator_tag版本
    template<class ForwardIterator>
    ForwardIterator rotate_dispatch(ForwardIterator first,ForwardIterator middle,ForwardIterator last,forward_iterator_tag){
        auto first2 = middle;
        do{
            XinSTL::swap(*first++,*first2++);
            if(first == middle){
                middle = first2;
            }
        }while(first2 != last);//后半段移到前面

        auto new_middle = first;//迭代器后面的位置
        first2 = middle;
        while(first2 != last){
            //调整剩余元素
            XinSTL::swap(*first++,*first2++);
            if(first == middle){
                middle = first2;
            }else if(first2 == last){
                first2 = middle;
            }
        }
        return new_middle;
    }

    //rotate_dispatch的bidirectional_iterator_tag版本
    template<class BidirectionalIterator>
    BidirectionalIterator rotate_dispatch(BidirectionalIterator first,BidirectionalIterator middle,BidirectionalIterator last,bidirectional_iterator_tag){
        XinSTL::reverse_dispatch(first,middle,bidirectional_iterator_tag());
        XinSTL::reverse_dispatch(middle,last,bidirectional_iterator_tag());
        while(first != middle && middle != last){
            XinSTL::swap(*first++,*--last);
        }
        if(first == middle){
            XinSTL::reverse_dispatch(middle,last,bidirectional_iterator_tag());
            return last;
        }else{
            XinSTL::reverse_dispatch(first,middle,bidirectional_iterator_tag());
            return first;
        }
    }

    //求最大公因子
    template<class EuclideanRingElement>
    EuclideanRingElement rgcd(EuclideanRingElement m,EuclideanRingElement n){
        while(n != 0){
            auto t = m % n;
            m = n;
            n = t;
        }
        return m;
    }

    //rotate_dispatch的random_access_iterator_tag版本
    template<class RandomIterator>
    RandomIterator rotate_dispatch(RandomIterator first,RandomIterator middle,RandomIterator last,random_access_iterator_tag){
        //因为是random access iterator，我们可以确定每个元素的位置
        auto n = last - first;
        auto l = middle - first;
        auto r = n - 1;
        auto result = first + (last - middle);
        if(l == r){
            XinSTL::swap_ranges(first,middle,middle);
            return result;
        }
        auto cycle_times = rgcd(n,1);
        for(auto i = 0;i < cycle_times;i++){
            auto temp = *first;
            auto p = first;
            if(l < r){
                for(auto j = 0; j < r/cycle_times;j++){
                    if(p > first + r){
                        *p = *(p - r);
                        p -= r;
                    }
                    *p = *(p-r);
                    p -= r;
                }
                *p = *(p+1);
                p += 1;
            }else{
                for(auto j = 0;j < 1/cycle_times-1;j++){
                    if(p < last - 1){
                        *p = *(p-1);
                        p += 1;
                    }
                    *p = *(p-r);
                    p -= r;
                }
            }
            *p = temp;
            first++;
        }
        return result;
    }

    template<class ForwardIterator>
    ForwardIterator rotate(ForwardIterator first,ForwardIterator middle,ForwardIterator last){
        if(first == middle){
            return last;
        }
        if(middle == last){
            return first;
        }
        return XinSTL::rotate_dispatch(first,middle,last,iterator_category(first));
    }
    
    //**************************************************************************
    //rotate_copy
    //行为与rotate类似，不同的是将结果复制到result所指的容器中
    template<class ForwardIterator,class OutputIterator>
    ForwardIterator rotate_copy(ForwardIterator first,ForwardIterator middle,ForwardIterator last,OutputIterator result){
        return XinSTL::copy(first,middle,XinSTL::copy(middle,last,result));
    }

    //***************************************************************************
    //is_permutation
    //判断[first1,last1)是否为[first2,last2)的排列组合
    template<class ForwardIterator1,class ForwardIterator2,class BinaryPred>
    bool is_permutation_aux(ForwardIterator1 first1,ForwardIterator1 last1,ForwardIterator2 first2,ForwardIterator2 last2,BinaryPred pred){
        constexpr bool is_ra_it = XinSTL::is_random_access_iterator<ForwardIterator1>::value && XinSTL::is_random_access_iterator<ForwardIterator2>::value;
        if(is_ra_it){
            auto len1 = last1 - first1;
            auto len2 = last2 - first2;
            if(len1 != len2){
                return false;
            }
        }

        //先找出相同的前缀段
        for(;first1 != last1 && first2 != last2;first1++,(void)first2++){
            if(!pred(*first1,*first2)){
                break;
            }
        }
        if(is_ra_it){
            if(first1 == last1){
                return true;
            }
        }else{
            auto len1 = XinSTL::distance(first1,last1);
            auto len2 = XinSTL::distance(first2,last2);
            if(len1 == 0 && len2 == 0){
                return true;
            }
            if(len1 != len2){
                return false;
            }
        }

        //判断剩余部分
        for(auto i = first1;i != last1;i++){
            bool is_repeated = false;
            for(auto j = first1;j != i;j++){
                if(pred(*i,*j)){
                    is_repeated = true;
                    break;
                }
            }

            if(!is_repeated){
                //计算*i在[first2,last2)的数目
                auto c2 = 0;
                for(auto j = first2;j != last2;j++){
                    if(pred(*i,*j)){
                        c2++;
                    }
                }
                if(c2 == 0){
                    return false;
                }

                //计算*i在[first1,last1)的数目
                auto c1 = 1;
                auto j = i;
                for(j++;j != last1;j++){
                    if(pred(*i,*j)){
                        c1++;
                    }
                }
                if(c1 != c2){
                    return false;
                }
            }
        }
        return true;
    }

    template<class ForwardIterator1,class ForwardIterator2,class BinaryPred>
    bool is_permutation(ForwardIterator1 first1,ForwardIterator1 last1,ForwardIterator2 first2,ForwardIterator2 last2,BinaryPred pred){
        return is_permutation_aux(first1,last1,first2,last2,pred);
    }

    template<class ForwardIterator1,class ForwardIterator2>
    bool is_permutation(ForwardIterator1 first1,ForwardIterator1 last1,ForwardIterator2 first2,ForwardIterator2 last2){
        typedef typename iterator_traits<ForwardIterator1>::value_type v1;
        typedef typename iterator_traits<ForwardIterator2>::value_type v2;
        static_assert(std::is_same<v1,v2>::value,"the type should be same in XinSTL::is_permutation");
        return is_permutation_aux(first1,last1,first2,last2,XinSTL::equal_to<v1>());
    }

    //***************************************************************************
    //next_permutation
    //取得[first,last)所标示序列的下一个排列组合
    //如果没有下一个排序组合，返回false，否则返回true
    template<class BidirentionalIterator>
    bool next_permutation(BidirentionalIterator first,BidirentionalIterator last){
        auto i = last;
        if(first == last || first == --i){
            return false;
        }

        while(true){
            auto j = i;
            if(*--i < *j){
                //找到第一对小于关系的元素
                auto k = last;
                while(!(*i<*--k)){}
                XinSTL::iter_swap(i,k);//交换i,k所指元素
                XinSTL::reverse(j,last);//将j之后的所有元素反转
                return true;
            }
            if(i == first){
                XinSTL::reverse(first,last);
                return false;
            }
        }
    }

    //重载版本使用函数对象comp代替比较操作
    template <class BidirectionalIter, class Compared>
    bool next_permutation(BidirectionalIter first, BidirectionalIter last, Compared comp)
    {
        auto i = last;
        if (first == last || first == --i)
            return false;
        for (;;)
        {
            auto ii = i;
            if (comp(*--i, *ii))
            {
                auto j = last;
                while (!comp(*i, *--j)) {}
                XinSTL::iter_swap(i, j);       // 交换 i，j 所指元素
                XinSTL::reverse(ii, last);     // 将 ii 之后的所有元素反转
                return true;
            }
            if (i == first)
            {
            XinSTL::reverse(first, last);
            return false;
            }
        }
    }

    //******************************************************************************
    // prev_permutation
    // 取得[first, last)所标示序列的上一个排列组合
    //如果没有上一个排序组合，返回 false，否则返回 true
    template <class BidirectionalIter>
    bool prev_permutation(BidirectionalIter first, BidirectionalIter last)
    {
        auto i = last;
        if (first == last || first == --i)
            return false;
        for (;;)
        {
            auto ii = i;
            if (*ii < *--i)
            {                 // 找到第一对大于关系的元素
                auto j = last;
                while (!(*--j < *i)) {}
                XinSTL::iter_swap(i, j);       // 交换i，j
                XinSTL::reverse(ii, last);     // 将 ii 之后的所有元素反转
                return true;
            }
            if (i == first)
            {
                XinSTL::reverse(first, last);
                return false;
            }
        }
    }

    // 重载版本使用函数对象 comp 代替比较操作
    template <class BidirectionalIter, class Compared>
    bool prev_permutation(BidirectionalIter first, BidirectionalIter last, Compared comp)
    {
        auto i = last;
        if (first == last || first == --i)
            return false;
        for (;;)
        {
            auto ii = i;
            if (comp(*ii, *--i))
            {
                auto j = last;
                while (!comp(*--j, *i)) {}
                XinSTL::iter_swap(i, j);       // 交换i，j
                XinSTL::reverse(ii, last);     // 将 ii 之后的所有元素反转
                return true;
            }
            if (i == first)
            {
                XinSTL::reverse(first, last);
                return false;
            }
        }
    }

    //*************************************************************************
    //merge
    //将两个经过排序的集合S1和S2合并起来置于另一段空间
    //返回一个迭代器指向最后一个元素的下一个位置
    template<class InputIterator1,class InputIteartor2,class OutputIterator>
    OutputIterator merge(InputIterator1 first1,InputIterator1 last1,InputIteartor2 first2,InputIteartor2 last2,OutputIterator result){
        while(first1 != last1 && first2 != last2){
            if(*first2 < *first1){
                *result = *first2;
                first2++;
            }else{
                *result = *first1;
                first2++;
            }
            result++;
        }
        return XinSTL::copy(first2,last2,XinSTL::copy(first1,last1,result));
    }

    //重载版本使用函数对象comp代替比较操作
    template<class InputIterator1,class InputIterator2,class OutputIterator,class Compare>
    OutputIterator merge(InputIterator1 first1,InputIterator1 last1,InputIterator2 first2,InputIterator2 last2,OutputIterator result,Compare comp){
        while(first1 != last1 && first2 != last2){
            if(comp(*first2,*first1)){
                *result = *first2;
                first2++;
            }else{
                *result = *first1;
                first1++;
            }
            result++;
        }
        return XinSTL::copy(first2,last2,XinSTL::copy(first1,last1,result));
    }

    //**************************************************************************
    //inplace_merge
    //把连接在一起的两个有序序列结合成单一序列并保持有序

    //没有缓冲器的情况下合并
    template<class BidirectionalIterator,class Distance>
    void merge_without_buffer(BidirectionalIterator first,BidirectionalIterator middle,BidirectionalIterator last,Distance len1,Distance len2){
        if(len1 == 0 || len2 == 0){
            return;
        }

        if(len1 + len2 == 2){
            if(*middle < *first){
                XinSTL::iter_swap(first,middle);
            }
            return;
        }

        auto first_cut = first;
        auto second_cut = middle;
        Distance len11 = 0;
        Distance len22 = 0;
        if(len1 > len2){
            //序列一较长，找到序列一的中点
            len11 = len1 >> 1;
            XinSTL::advance(first_cut,len11);
            second_cut = XinSTL::lower_bound(middle,last,*first_cut);
            len22 = XinSTL::distance(middle,second_cut);
        }else{
            //序列二较长，找到序列二的重点
            len22 = len2 >> 1;
            XinSTL::advance(second_cut,len22);
            first_cut = XinSTL::upper_bound(first,middle,*second_cut);
            len11 = XinSTL::distance(first,first_cut);
        }
        auto new_middle = XinSTL::rotate(first_cut,middle,second_cut);
        XinSTL::merge_without_buffer(first,first_cut,new_middle,len11,len22);
        XinSTL::merge_without_buffer(new_middle,second_cut,last,len1-len11,len2-len22);
    }

    template<class BidirectionalIterator1,class BidirectionalIterator2>
    BidirectionalIterator1 merge_backward(BidirectionalIterator1 first1,BidirectionalIterator1 last1,BidirectionalIterator2 first2,BidirectionalIterator2 last2,BidirectionalIterator1 result){
        if(first1 == last1){
            return XinSTL::copy_backward(first2,last2,result);
        }
        if(first2 == last2){
            return XinSTL::copy_backward(first1,last1,result);
        }
        --last1;
        --last2;
        while(true){
            if(*last2 < *last1){
                *--result = -last1;
                if(first1 == last1){
                    return XinSTL::copy_backward(first2,++last2,result);
                }
                --last1;
            }else{
                *--result = *last2;
                if(first2 == last2){
                    return XinSTL::copy_backward(first1,++last1,result);
                }
                --last2;
            }
        }
    }

    template <class BidirectionalIter1, class BidirectionalIter2, class Distance>
    BidirectionalIter1
    rotate_adaptive(BidirectionalIter1 first, BidirectionalIter1 middle,
                    BidirectionalIter1 last, Distance len1, Distance len2,
                    BidirectionalIter2 buffer, Distance buffer_size)
    {
        BidirectionalIter2 buffer_end;
        if (len1 > len2 && len2 <= buffer_size)
        {
            buffer_end = XinSTL::copy(middle, last, buffer);
            XinSTL::copy_backward(first, middle, last);
            return XinSTL::copy(buffer, buffer_end, first);
        }
        else if (len1 <= buffer_size)
        {
            buffer_end = XinSTL::copy(first, middle, buffer);
            XinSTL::copy(middle, last, first);
            return XinSTL::copy_backward(buffer, buffer_end, last);
        }
        else
        {
            return XinSTL::rotate(first, middle, last);
        }
    }

    // 有缓冲区的情况下合并
    template <class BidirectionalIter, class Distance, class Pointer>
    void merge_adaptive(BidirectionalIter first, BidirectionalIter middle,
                        BidirectionalIter last, Distance len1, Distance len2,
                        Pointer buffer, Distance buffer_size)
        {
        // 区间长度足够放进缓冲区
        if (len1 <= len2 && len1 <= buffer_size)
        {
            Pointer buffer_end = XinSTL::copy(first, middle, buffer);
            XinSTL::merge(buffer, buffer_end, middle, last, first);
        }
        else if (len2 <= buffer_size)
        {
            Pointer buffer_end = XinSTL::copy(middle, last, buffer);
            XinSTL::merge_backward(first, middle, buffer, buffer_end, last);
        }
        else
        {  // 区间长度太长，分割递归处理
            auto first_cut = first;
            auto second_cut = middle;
            Distance len11 = 0;
            Distance len22 = 0;
            if (len1 > len2)
            {
                len11 = len1 >> 1;
                XinSTL::advance(first_cut, len11);
                second_cut = XinSTL::lower_bound(middle, last, *first_cut);
                len22 = XinSTL::distance(middle, second_cut);
            }
            else
            {
                len22 = len2 >> 1;
                XinSTL::advance(second_cut, len22);
                first_cut = XinSTL::upper_bound(first, middle, *second_cut);
                len11 = XinSTL::distance(first, first_cut);
            }
            auto new_middle = XinSTL::rotate_adaptive(first_cut, middle, second_cut,
                                                    len1 - len11, len22, buffer, buffer_size);
            XinSTL::merge_adaptive(first, first_cut, new_middle, len11, len22, buffer, buffer_size);
            XinSTL::merge_adaptive(new_middle, second_cut, last, len1 - len11,
                                len2 - len22, buffer, buffer_size);
        }
    }        

    template <class BidirectionalIter, class T>
    void inplace_merge_aux(BidirectionalIter first, BidirectionalIter middle,
                    BidirectionalIter last, T*)
    {
        auto len1 = XinSTL::distance(first, middle);
        auto len2 = XinSTL::distance(middle, last);
        temporary_buffer<BidirectionalIter, T> buf(first, last);
        if (!buf.begin())
        {
            XinSTL::merge_without_buffer(first, middle, last, len1, len2);
        }
        else
        {
            XinSTL::merge_adaptive(first, middle, last, len1, len2, buf.begin(), buf.size());
        }
    }

    template<class BidirectionalIterator>
    void inplace_merge(BidirectionalIterator first,BidirectionalIterator middle,BidirectionalIterator last){
        if(first == middle || middle == last){
            return;
        }
        XinSTL::inplace_merge_aux(first,middle,last,value_type(first));
    }

    //重载版本使用函数对象comp代替比较操作
    //没有缓冲区的情况下合并
    template<class BidirectionalIterator,class Distance,class Compare>
    void merge_without_buffer(BidirectionalIterator first,BidirectionalIterator middle,BidirectionalIterator last,Distance len1,Distance len2,Compare comp){
        if(len1 == 0 || len2 == 0){
            return;
        }
        if(len1 + len2 == 2){
            if(comp(*middle,*first)){
                XinSTL::iter_swap(first,middle);
            }
            return;
        }
        auto first_cut = first;
        auto second_cut = middle;
        Distance len11 = 0;
        Distance len22 = 0;
        if(len1 > len2){
            len11 = len1 >> 1;
            XinSTL::advance(first_cut,len11);
            second_cut = XinSTL::lower_bound(middle,last,*first_cut,comp);
            len22 = XinSTL::distance(middle,second_cut);
        }else{
            len22 = len2 >> 1;
            XinSTL::advance(second_cut,len22);
            first_cut = XinSTL::upper_bound(first,middle,*second_cut,comp);
            len11 = XinSTL::distance(first,first_cut);
        }
        auto new_middle = XinSTL::rotate(first_cut,middle,second_cut);
        XinSTL::merge_without_buffer(first,first_cut,new_middle,len11,len22,comp);
        XinSTL::merge_without_buffer(new_middle,second_cut,last,len1-len11,len2-len22,comp);
    }

    template<class BidirectionalIterator1,class BidirectionalIterator2,class Compare>
    BidirectionalIterator1 merge_backward(BidirectionalIterator1 first1,BidirectionalIterator1 last1,BidirectionalIterator2 first2,BidirectionalIterator2 last2,BidirectionalIterator1 result,Compare comp){
        if(first1 == last1){
            return XinSTL::copy_backward(first2,last2,result);
        }
        if(first2 == last2){
            return XinSTL::copy_backward(first1,last1,result);
        }
        last1--;
        last2--;
        while(true){
            if(comp(*last2,*last1)){
                *--result = *last1;
                if(first1 == last1){
                    return XinSTL::copy_backward(first2,++last2,result);
                }
                --last1;
            }else{
                *--result = *last2;
                if(first2 == last2){
                    return XinSTL::copy_backward(first1,++last1,result);
                }
                --last2;
            }
        }
    }

    //有缓冲区的情况下合并
    template<class BidirectionalIterator,class Distance,class Pointer,class Compare>
    void merge_adaptive(BidirectionalIterator first,BidirectionalIterator middle,BidirectionalIterator last,Distance len1,Distance len2,Pointer buffer,Distance buffer_size,Compare comp){
        //区间长度足够放进缓冲区
        if(len1 <= len2 && len1 <= buffer_size){
            Pointer buffer_end = XinSTL::copy(first,middle,buffer);
            XinSTL::merge(buffer,buffer_end,middle,last,first,comp);
        }else if(len2 <= buffer_size){
            Pointer buffer_end = XinSTL::copy(middle,last,buffer);
            XinSTL::merge_backward(first,middle,buffer,buffer_end,last,comp);
        }else{
            //区间长度太长，分割递归处理
            auto first_cut = first;
            auto second_cut = middle;
            Distance len11 = 0;
            Distance len22 = 0;
            if(len1 > len2){
                len11 = len1 >> 1;
                XinSTL::advance(first_cut,len11);
                second_cut = XinSTL::lower_bound(middle,last,*first_cut,comp);
                len22 = XinSTL::distance(middle,second_cut);
            }else{
                len22 = len2 >> 1;
                XinSTL::advance(second_cut,len22);
                first_cut = XinSTL::upper_bound(first,middle,*second_cut,comp);
                len11 = XinSTL::distance(first,first_cut);
            }
            auto new_middle = XinSTL::rotate_adaptive(first_cut,middle,second_cut,len1-len11,len22,buffer,buffer_size);
            XinSTL::merge_adaptive(first,first_cut,new_middle,len11,len22,buffer,buffer_size,comp);
            XinSTL::merge_adaptive(new_middle,second_cut,last,len1-len11,len2-len22,buffer,buffer_size,comp);
        }
    }

    template<class BidirectionalIterator,class T,class Compare>
    void inplace_merge_aux(BidirectionalIterator first,BidirectionalIterator middle,BidirectionalIterator last,T*,Compare comp){
        auto len1 = XinSTL::distance(first,middle);
        auto len2 = XinSTL::distance(middle,last);
        temporary_buffer<BidirectionalIterator,T> buf(first,last);
        if(!buf.begin()){
            XinSTL::merge_without_buffer(first,middle,last,len1,len2,comp);
        }else{
            XinSTL::merge_adaptive(first,middle,last,len1,len2,buf.begin(),buf.size(),comp);
        }
    }

    template<class BidirectionalIterator,class Compare>
    void inplace_merge(BidirectionalIterator first,BidirectionalIterator middle,BidirectionalIterator last,Compare comp){
        if(first == middle || middle == last){
            return;
        }
        XinSTL::inplace_merge_aux(first,middle,last,value_type(first),comp);
    }

    //***************************************************************************
    //partial_sort
    //对整个序列做部分排序
    //保证较小的N个元素以递增顺序置于[first,first+N)中

    //原理
    //将[first,first+N)设置成heap，然后依次判断后面的元素是否应该加入heap
    //最后sort_heap,将前N个元素按序排列
    template<class RandomIterator>
    void partial_sort(RandomIterator first,RandomIterator middle,RandomIterator last){
        XinSTL::make_heap(first,middle);
        for(auto i = middle;i < last;i++){
            if(*i < *first){
                XinSTL::pop_heap_aux(first,middle,i,*i,distance_type(first));
            }
        }
        XinSTL::sort_heap(first,middle);
    }

    //重载版本使用函数对象comp代替比较操作
    template<class RandomIterator,class Compare>
    void partial_sort(RandomIterator first,RandomIterator middle,RandomIterator last,Compare comp){
        XinSTL::make_heap(first,middle,comp);
        for(auto i = middle;i < last;i++){
            if(comp(*i,*first)){
                XinSTL::pop_heap_aux(first,middle,i,*i,distance_type(first),comp);
            }
        }
        XinSTL::sort_heap(first,middle,comp);
    }

    //***************************************************************************
    //partial_sort_copy
    //行为与partial_sort类似，不同的是把排序结果复制到result容器中
    template<class InputIterator,class RandomIterator,class Distance>
    RandomIterator psort_copy_aux(InputIterator first,InputIterator last,RandomIterator result_first,RandomIterator result_last,Distance*){
        if(result_first == result_last){
            return result_last;
        }
        auto result_iter = result_first;
        while(first != last && result_iter != result_last){
            *result_iter = *first;
            ++result_iter;
            ++first;
        }
        XinSTL::make_heap(result_first,result_iter);
        while(first != last){
            if(*first < *result_first){
                XinSTL::adjust_heap(result_first,static_cast<Distance>(0),result_iter - result_first,*first);
            }
            ++first;
        }
        XinSTL::sort_heap(result_first,result_iter);
        return result_iter;
    }

    template<class InputIterator,class RandomIterator>
    RandomIterator partial_sort_copy(InputIterator first,InputIterator last,RandomIterator result_first,RandomIterator result_last){
        return XinSTL::psort_copy_aux(first,last,result_first,result_last,distance_type(result_first));
    }

    //重载版本使用函数对象comp代替比较操作
    template<class InputIterator,class RandomIterator,class Distance,class Compare>
    RandomIterator psort_copy_aux(InputIterator first,InputIterator last,RandomIterator result_first,RandomIterator result_last,Distance*,Compare comp){
        if(result_first == result_last){
            return result_last;
        }
        auto result_iter = result_first;
        while(first != last && result_iter != result_last){
            *result_iter = *first;
            ++result_iter;
            ++first;
        }
        XinSTL::make_heap(result_first,result_iter,comp);
        while(first != last){
            if(comp(*first,*result_first)){
                XinSTL::adjust_heap(result_first,static_cast<Distance>(0),result_iter - result_first,*first,comp);
            }
            ++first;
        }
        XinSTL::sort_heap(result_first,result_iter,comp);
        return result_iter;
    }

    template<class InputIterator,class RandomIterator,class Compare>
    RandomIterator partial_sort_copy(InputIterator first,InputIterator last,RandomIterator result_first,RandomIterator result_last,Compare comp){
        return XinSTL::psort_copy_aux(first,last,result_first,result_last,distance_type(result_first),comp);
    }

    //**************************************************************************
    //partition
    //对区间内的元素重排，被一元条件运算判定为true的元素会放到区间的前段
    //该函数不保证元素的原始相对位置
    template<class BidirectionalIterator,class UnaryPredicate>
    BidirectionalIterator partition(BidirectionalIterator first,BidirectionalIterator last,UnaryPredicate unary_pred){
        while(true){
            while(first != last && unary_pred(*first)){
                ++first;
            }
            if(first == last){
                break;
            }
            --last;
            while(first != last && !unary_pred(*last)){
                --last;
            }
            if(first == last){
                break;
            }
            XinSTL::iter_swap(first,last);
            ++first;
        }
        return first;
    }

    //partition_copy
    //行为与partition类似，不同的是，将被一元操作符判定为true的放到result_true的输出区间
    //其余放到result_false的输出区间，并返回一个XinSTL::pair指向这两个区间的尾部
    template<class InputIterator,class OutputIterator1,class OutputIterator2,class UnaryPredicate>
    XinSTL::pair<OutputIterator1,OutputIterator2>
    partial_copy(InputIterator first,InputIterator last,OutputIterator1 result_true,OutputIterator2 result_false,UnaryPredicate unary_pred){
        for(;first != last;first++){
            if(unary_pred(*first)){
                *result_true++ = *first;
            }else{
                *result_false++ = *first;
            }
        }
        return XinSTL::pair<OutputIterator1,OutputIterator2>(result_true,result_false);
    }

    //***************************************************************************
    //sort将[first,last)内的元素以递增的方式排序

    //threshold
    //小型区间的大小，在这个大小内使用插入排序
    constexpr static size_t kSmallSectionSize = 128;

    //找出lgk <= n的k的最大值
    template<class Size>
    Size slg2(Size n){
        Size k = 0;
        for(;n > 1;n >> 1){
            k++;
        }
        return k;
    }

    //分割函数
    //用于控制分割恶化的情况
    //用于quick_sort
    //当!(first < last)时，表示序列调整完毕
    //first为轴，左半部分小于等于pivot，右半部分大于等于pivot
    template<class RandomIterator,class T>
    RandomIterator unchecked_partition(RandomIterator first,RandomIterator last,const T& pivot){
        while(true){
            while(*first < pivot){
                first++;
            }
            last--;
            while(pivot < *last){
                last--;
            }
            if(!(first < last)){
                return first;
            }
            XinSTL::iter_swap(first,last);
            first++;
        }
    }

    //不恰当的pivot选择，可能导致quick_sort退化为O(N^2)
    //内省式算法
    //有恶化倾向时，改用heap sort
    template<class RandomIterator,class Size>
    void intro_sort(RandomIterator first,RandomIterator last,Size depth_limit){
        while(static_cast<size_t>(last-first) > kSmallSectionSize){
            if(depth_limit == 0){
                //到达最大分割深度限制
                //改用heap_sort
                XinSTL::partial_sort(first,last,last);
                return;
            }
            depth_limit--;
            auto mid = XinSTL::median(*(first),*(first+(last-first)/2),*(last-1));
            auto cut = XinSTL::unchecked_partition(first,last,mid);
            XinSTL::intro_sort(cut,last,depth_limit);
            last = cut;
        }
    }

    //insertion_sort以双层循环的形式进行
    //外循环遍历整个序列，每次迭代器决定出一个子区间
    //内循环遍历子区间，将逆转对倒转过来

    //插入排序辅助函数
    //unchencked_linear_insert
    template<class RandomIterator,class T>
    void unchecked_linear_insert(RandomIterator last,const T& value){
        auto next = last;
        next--;
        //insertion sort的内循环
        //一旦不再出现逆转对，循环结束
        while(value < *next){//逆转对存在
            *last = *next;//调整
            last = next;//调整迭代器
            next--;//左移一个位置
        }
        *last = value;//value的正确落脚处
    }

    //插入排序函数
    //unchecked_insertion_sort
    template<class RandomIterator>
    void unchecked_insertion_sort(RandomIterator first,RandomIterator last){
        for(auto i = first;i != last;i++){
            XinSTL::unchecked_linear_insert(i,*i);
        }
    }

    //插入排序函数
    //insertion_sort
    template<class RandomIterator>
    void insertion_sort(RandomIterator first,RandomIterator last){
        if(first == last){
            return;
        }
        for(auto i = first + 1;i != last;i++){
            auto value = *i;
            if(value < *first){
                XinSTL::copy_backward(first,i,i+1);
                *first = value;
            }else{
                XinSTL::unchecked_linear_insert(i,value);
            }
        }
    }

    //对于“几近排序但尚未彻底排序”的子序列做一次完整的排序，其效率更好
    //final_insertion_sort
    template<class RandomIterator>
    void final_insertion_sort(RandomIterator first,RandomIterator last){
        if(static_cast<size_t>(last-first) > kSmallSectionSize){
            XinSTL::insertion_sort(first,first+kSmallSectionSize);
            XinSTL::unchecked_insertion_sort(first+kSmallSectionSize,last);
        }else{
            XinSTL::insertion_sort(first,last);
        }
    }

    template<class RandomIterator>
    void sort(RandomIterator first,RandomIterator last){
        if(first != last){
            //内省式排序，将区间分为一个个小区间，然后对整体进行插入排序
            XinSTL::intro_sort(first,last,slg2(last-first)*2);
            XinSTL::final_insertion_sort(first,last);
        }
    }

    //重载版本使用函数对象comp代替比较操作
    //分割函数unchecked_partition
    template<class RandomIterator,class T,class Compare>
    RandomIterator unchecked_partition(RandomIterator first,RandomIterator last,const T& pivot,Compare comp){
        while(true){
            while(comp(*first,pivot)){
                first++;
            }
            last--;
            while(comp(pivot,*last)){
                last--;
            }
            if(!(first < last)){
                return first;
            }
            XinSTL::iter_swap(first,last);
            first++;
        }
    }

    //内省式排序
    //重载版本
    template<class RandomIterator,class Size,class Compare>
    void intro_sort(RandomIterator first,RandomIterator last,Size depth_limit,Compare comp){
        while(static_cast<size_t>(last-first) > kSmallSectionSize){
            if(depth_limit == 0){
                //到达最大分割深度限制
                XinSTL::partial_sort(first,last,last,comp);
                return;
            }
            depth_limit--;
            auto mid = XinSTL::median(*(first),*(first+(last-first)/2),*(last-1));
            auto cut = XinSTL::unchecked_partition(first,last,mid,comp);
            XinSTL::intro_sort(cut,last,depth_limit,comp);
            last = cut;
        }
    }

    //插入排序辅助函数
    //unchecked_linear_insert
    template<class RandomIterator,class T,class Compare>
    void unchecked_linear_insert(RandomIterator last,const T& value,Compare comp){
        auto next = last;
        next--;
        while(comp(value,*next)){
            //从尾部开始寻找第一个可插入位置
            *last = *next;
            last = next;
            next--;
        }
        *last = value;
    }

    //插入排序函数
    //unchecked_insertion_sort
    template<class RandomIterator,class Compare>
    void unchecked_insertion_sort(RandomIterator first,RandomIterator last,Compare comp){
        for(auto i = first;i != last;i++){
            XinSTL::unchecked_linear_insert(i,*i,comp);
        }
    }

    //插入排序函数
    //insertion_sort
    template<class RandomIterator,class Compare>
    void insertion_sort(RandomIterator first,RandomIterator last,Compare comp){
        if(first == last){
            return;
        }
        for(auto i = first+1;i != last;i++){
            auto value = *i;
            if(comp(value,*first)){
                XinSTL::copy_backward(first,i,i+1);
                *first = value;
            }else{
                XinSTL::unchecked_linear_insert(i,value,comp);
            }
        }
    }

    //最终插入排序函数
    //final_insertion_sort
    template<class RandomIterator,class Compare>
    void final_insertion_sort(RandomIterator first,RandomIterator last,Compare comp){
        if(static_cast<size_t>(last-first) > kSmallSectionSize){
            XinSTL::insertion_sort(first,first+kSmallSectionSize,comp);
            XinSTL::unchecked_insertion_sort(first+kSmallSectionSize,last,comp);
        }else{
            XinSTL::insertion_sort(first,last,comp);
        }
    }

    template<class RandomIterator,class Compare>
    void sort(RandomIterator first,RandomIterator last,Compare comp){
        if(first != last){
            //内省式排序
            //将区间分为一个个小区间，然后对整体进行插入排序
            XinSTL::intro_sort(first,last,slg2(last-first)*2,comp);
            XinSTL::final_insertion_sort(first,last,comp);
        }
    }

    //**************************************************************************
    //nth_element
    //对序列重排，使得所有小于第n个元素的元素出现在它的前面
    //大于它的出现在它的后面
    template<class RandomIterator>
    void nth_element(RandomIterator first,RandomIterator nth,RandomIterator last){
        if(nth == last){
            return;
        }
        while(last - first > 3){
            auto cut = XinSTL::unchecked_partition(first,last,XinSTL::median(*first,*(first+(last-first)/2),*(last-1)));
            if(cut <= nth){
                //如果nth位于右段
                //对右段进行分割
                first = cut;
            }else{
                //对左段进行分割
                last = cut;
            }
        }
        XinSTL::insertion_sort(first,last);
    }

    //重载版本使用函数对象comp代替比较操作
    template<class RandomIterator,class Compare>
    void nth_element(RandomIterator first,RandomIterator nth,RandomIterator last,Compare comp){
        if(nth == last){
            return;
        }
        while(last - first > 3){
            auto cut = XinSTL::unchecked_partition(first,last,XinSTL::median(*first,*(first+(last-first)/2),*(last-1)),comp);
            if(cut <= nth){
                //如果nth位于右段
                //对右段进行分割
                first = cut;
            }else{
                //对左段进行分割
                last = cut;
            }
        }
        XinSTL::insertion_sort(first,last,comp);
    }

    //**************************************************************************
    //unique_copy
    //从[first,last)中将元素复制到result上
    //序列必须有序，如果有重复的元素，只会复制一次

    //unique_copy_dispatch的forward_iterator_tag版本
    template<class InputIterator,class ForwardIterator>
    ForwardIterator unique_copy_dispatch(InputIterator first,InputIterator last,ForwardIterator result,forward_iterator_tag){
        *result = *first;
        while(++first != last){
            if(*result != *first){
                *++result = *first;
            }
        }
        return ++result;
    }

    //output_iterator_tag版本
    //由于output_iterator只有只读操作
    //所以不能有*result != *first这样的判断
    template<class InputIterator,class OutputIterator>
    OutputIterator unique_copy_dispatch(InputIterator first,InputIterator last,OutputIterator result,output_iterator_tag){
        auto value = *first;
        *result = value;
        while(++first != last){
            if(value = *first){
                value = *first;
                *++result = value;
            }
        }
        return ++result;
    }

    template<class InputIterator,class OutputIteartor>
    OutputIteartor unique_copy(InputIterator first,InputIterator last,OutputIteartor result){
        if(first == last){
            return result;
        }
        return XinSTL::unique_copy_dispatch(first,last,result,iterator_category(result));
    }

    //重逢版本使用函数对象comp代替比较操作
    //unique_copy_dispatch的forwrad_iterator_tag版本
    template<class InputIterator,class ForwardIterator,class Compare>
    ForwardIterator unique_copy_dispatch(InputIterator first,InputIterator last,ForwardIterator result,forward_iterator_tag,Compare comp){
        *result = *first;
        while(++first != last){
            if(!(comp(*result,*first))){
                *++result = *first;
            }
        }
        return ++result;
    }

    //output_iterator_tag版本
    template<class InputIterator,class OutputIterator,class Compare>
    OutputIterator unique_copy_dispatch(InputIterator first,InputIterator last,OutputIterator result,output_iterator_tag,Compare comp){
        auto value = *first;
        *result = value;
        while(++first != last){
            if(!comp(value,*first)){
                value = *first;
                *++result = value;
            }
        }
        return ++result;
    }

    template<class InputIterator,class OutputIterator,class Compare>
    OutputIterator unique_copy(InputIterator first,InputIterator last,OutputIterator result,Compare comp){
        if(first == last){
            return result;
        }
        return XinSTL::unique_copy_dispatch(first,last,result,iterator_category(result),comp);
    }

    //***************************************************************************
    //unique
    //移除[first,last)内重复的元素
    //序列必须有序，和remove类似，它也不能真正的删除重复元素
    template<class ForwardIterator>
    ForwardIterator unique(ForwardIterator first,ForwardIterator last){
        first = XinSTL::adjacent_find(first,last);
        return XinSTL::unique_copy(first,last,first);
    }

    //重载版本使用函数对象comp代替比较操作
    template<class ForwardIterator,class Compare>
    ForwardIterator unique(ForwardIterator first,ForwardIterator last,Compare comp){
        first = XinSTL::adjacent_find(first,last,comp);
        return XinSTL::unique_copy(first,last,first,comp);
    }
} 

