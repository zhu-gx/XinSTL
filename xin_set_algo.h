#pragma once

#include<xin_algobase.h>
#include<xin_iterator.h>

//这个头文件包含set的四种算法：
//union，intersection，difference，symmetric_difference
//所有函数都要求序列有序

namespace XinSTL{
    //**************************************************************************
    //set_union
    //计算S1∪S2的结果并保存到result中
    //返回一个迭代器指向输出结果的尾部
    template<class InputIterator1,class InputIterator2,class OutputIterator>
    OutputIterator set_union(InputIterator1 first1,InputIterator1 last1,InputIterator2 first2,InputIterator2 last2,OutputIterator result){
        while(first1 != last1 && first2 != last2){
            if(*first1 < *first2){
                *result = *first1;
                first1++;
            }else if(*first2 < *first1){
                *result = *first2;
                first2++;
            }else{
                *result = *first1;
                first1++;
                first2++;
            }
            result++;
        }
        //将剩余元素拷贝到result
        return XinSTL::copy(first2,last2,XinSTL::copy(first1,last1,result));
    }

    //重载版本使用函数对象comp代替比较操作
    template<class InputIterator1,class InputIterator2,class OutputIterator,class Compare>
    OutputIterator set_union(InputIterator1 first1,InputIterator1 last1,InputIterator2 first2,InputIterator2 last2,OutputIterator result,Compare comp){
        while(first1 != last1 && first2 != last2){
            if(comp(*first1,*first2)){
                *result = *first1;
                first1++;
            }else if(comp(*first2,*first1)){
                *result = *first2;
                first2++;
            }else{
                *result = *first1;
                first1++;
                first2++;
            }
            result++;
        }
        return XinSTL::copy(first2,last2,XinSTL::copy(first1,last1,result));
    }

    //***************************************************************************
    //set_intersection
    //计算S1∩S2的结果并保存到result中
    //返回一个迭代器指向输出结果的尾部
    template<class InputIterator1,class InputIterator2,class OutputIterator>
    OutputIterator set_intersection(InputIterator1 first1,InputIterator1 last1,InputIterator2 first2,InputIterator2 last2,OutputIterator result){
        while(first1 != last1 && first2 != last2){
            if(*first1 < *first2){
                first1++;
            }else if(*first2 < *first1){
                first2++;
            }else{
                *result = *first1;
                first1++;
                first2++;
                result++;
            }
        }
        return result;
    }

    //重载版本使用函数对象comp代替比较操作
    template<class InputIterator1,class InputIterator2,class OutputIterator,class Compare>
    OutputIterator set_intersection(InputIterator1 first1,InputIterator1 last1,InputIterator2 first2,InputIterator2 last2,OutputIterator result,Compare comp){
        while(first1 != last1 && first2 != last2){
            if(comp(*first1,*first2)){
                first1++;
            }else if(comp(*first2,*first1)){
                first2++;
            }else{
                *result = *first1;
                first1++;
                first2++;
                result++;
            }
        }
        return result;
    }

    //***************************************************************************
    //set_difference
    //计算S1-S2的结果并保存到result中
    //返回一个迭代器指向输出结果的尾部
    template<class InputIterator1,class InputIterator2,class OutputIterator>
    OutputIterator set_difference(InputIterator1 first1,InputIterator1 last1,InputIterator2 first2,InputIterator2 last2,OutputIterator result){
        while(first1 != last1 && first2 != last2){
            if(*first1 < *first2){
                *result = *first1;
                first1++;
                result++;
            }else if(*first2 < *first1){
                first2++;
            }else{
                first1++;
                first2++;
            }
        }
        return XinSTL::copy(first1,last1,result);
    }

    //重载版本使用函数对象comp代替比较操作
    template<class InputIterator1,class InputIterator2,class OutputIterator,class Compare>
    OutputIterator set_difference(InputIterator1 first1,InputIterator1 last1,InputIterator2 first2,InputIterator2 last2,OutputIterator result,Compare comp){
        while(first1 != last1 && first2 != last2){
            if(comp(*first1,*first2)){
                *result = *first1;
                first1++;
                result++;
            }else if(comp(*first2,*first1)){
                first2++;
            }else{
                first1++;
                first2++;
            }
        }
        return XinSTL::copy(first1,last1,result);
    }

    //***************************************************************************
    //set_symmetric_difference
    //计算(S1-S2)∪(S2-S1)的结果并保存到 result 中
    //返回一个迭代器指向输出结果的尾部
    template<class InputIterator1,class InputIteartor2,class OutputIterator>
    OutputIterator set_symmetric_difference(InputIterator1 first1,InputIterator1 last1,InputIteartor2 first2,InputIteartor2 last2,OutputIterator result){
        while(first1 != last1 && first2 != last2){
            if(*first1 < *first2){
                *result = *first1;
                first1++;
                result++;
            }else if(*first2 < *first1){
                *result = *first2;
                first2++;
                result++;
            }else{
                first1++;
                first2++;
            }
        }
        return XinSTL::copy(first2,last2,XinSTL::copy(first1,last1,result));
    }

    //重载版本使用函数对象comp代替比较操作
    template<class InputIterator1,class InputIterator2,class OutputIterator,class Compare>
    OutputIterator set_symmetric_difference(InputIterator1 first1,InputIterator1 last1,InputIterator2 first2,InputIterator2 last2,OutputIterator result,Compare comp){
        while(first1 != last1 && first2 != last2){
            if(comp(*first1,*first2)){
                *result = *first1;
                first1++;
                result++;
            }else if(comp(*first2,*first1)){
                *result = *first2;
                first2++;
                result++;
            }else{
                first1++;
                first2++;
            }
        }
        return XinSTL::copy(first2,last2,XinSTL::copy(first1,last1,result));
    }
}