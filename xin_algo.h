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

    
}

