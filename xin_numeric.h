#pragma once

#include<xin_iterator.h>
//数值算法

namespace XinSTL{
    //***************************************************************************
    //accumulate
    //版本一：以初值init对每个元素进行累加
    //版本二：以初值init对每个元素进行二元操作

    //版本一
    template<class InputIterator,class T>
    T accumulate(InputIterator first,InputIterator last,T init){
        for(;first != last;first++){
            init += *first;
        }
        return init;
    }

    //版本二
    template<class InputIterator,class T,class BinaryOp>
    T accumulate(InputIterator first,InputIterator last,T init,BinaryOp binary_op){
        for(;first != last;first++){
            init = binary_op(init,*first);
        }
        return init;
    }

    //***************************************************************************
    //adjacent_difference
    //版本一：计算相邻元素的差值，结果保存到以result为起始的区间上
    //版本二：自定义相邻元素的二元操作

    //版本一
    template<class InputIterator,class OutputIterator>
    OutputIterator adjacent_difference(InputIterator first,InputIterator last,OutputIterator result){
        if(first == last){
            return result;
        }
        *result = *first;//记录第一个元素
        auto value = *first;
        while(++first != last){
            auto temp = *first;
            *++result = temp - value;
            value = temp;
        }
        return ++result;
    }

    //版本二
    template<class InputIterator,class OutputIterator,class BinaryOp>
    OutputIterator adjacent_difference(InputIterator first,InputIterator last,OutputIterator result,BinaryOp binary_op){
        if(first == last){
            return result;
        }
        *result = *first;//记录第一个元素
        auto value = *first;
        while(++first != last){
            auto temp = *first;
            *++result = binary_op(temp,value);
            value = temp;
        }
        return ++result;
    }

    //**************************************************************************
    //inner_product
    //版本一：以init为初值，计算两个区间的内积
    //版本二：自定义operator+和operator*

    //版本一
    template<class InputIterator1,class InputIterator2,class T>
    T inner_product(InputIterator1 first1,InputIterator1 last1,InputIterator2 first2,T init){
        for(;first1 != last1;first1++,first2++){
            init += (*first1 * *first2);
        }
        return init;
    }

    //版本二
    template<class InputIterator1,class InputIterator2,class T,class BinaryOp1,class BinaryOp2>
    T inner_product(InputIterator1 first1,InputIterator1 last1,InputIterator2 first2,T init,BinaryOp1 binary_op1,BinaryOp2 binary_op2){
        for(;first1 != last1;first1++,first2++){
            init = binary_op1(init,binary_op2(*first1,*first2));
        }
        return init;
    }

    //***************************************************************************
    //iota
    //填充[first,last)，以value为初值开始递增
    template<class ForwardIterator,class T>
    void iota(ForwardIterator first,ForwardIterator last,T value){
        while(first != last){
            *first++ = value;
            value++;
        }
    }

    //**************************************************************************
    //partial_sum
    //版本一：计算局部累计求和，结果保存到以result为起始的区间上
    //版本二：进行局部进行自定义二元操作
    
    //版本一
    template<class InputIterator,class OutputIterator>
    OutputIterator partial_sum(InputIterator first,InputIterator last,OutputIterator result){
        if(first == last){
            return result;
        }
        *result = *first;//记录第一个元素
        auto value = *first;
        while(++first != last){
            value = value + *first;
            *++result = value;
        }
        return ++result;
    }

    //版本二
    template<class InputIterator,class OutputIterator,class BinaryOp>
    OutputIterator partial_sum(InputIterator first,InputIterator last,OutputIterator result,BinaryOp binary_op){
        if(first == last){
            return result;
        }
        *result = *first;//记录第一个元素
        auto value = *first;
        while(++first != last){
            value = binary_op(value,*first);
            *++result = value;
        }
        return ++result;
    }
}