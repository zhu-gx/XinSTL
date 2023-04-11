#pragma once

#include<xin_iterator.h>
//该头文件包括了heap的四个算法：push_heap/pop_heap/sort_heap/make_heap
//heap的作用是作为priority queue（取出时一定是优先值最高的值）的助手
//binary heap是一种完全二叉树

namespace XinSTL{
    //*****************************************************************************
    //push_heap
    //该函数接受两个迭代器，表示一个heap容器的首尾
    //并且新元素已经插入到底部容器的最尾端，调整heap
    //插入一个元素后，执行一段percolate up程序，直到不需要调换为止
    template<class RandomIterator>
    void push_heap(RandomIterator first,RandomIterator last){
        //此函数被调用时，新元素应该已置于底部容器的最尾端
        XinSTL::push_heap_d(first,last,distance_type(first));
    }

    template<class RandomIterator,class Distance,class T>
    void push_heap_aux(RandomIterator first,Distance holeIndex,Distance topIndex,T value){
        Distance parent = (holeIndex - 1)/2;//找出父节点
        while(holeIndex > topIndex && *(first + parent) < value){
            //尚未到达顶端，且父节点小于新值，需要继续执行percolate up
            //由于以上使用operator<，因此这是一个max_heap
            *(first + holeIndex) = *(first + parent);//令洞值为父值
            holeIndex = parent;//调整洞号，向上提升至父节点
            parent = (holeIndex - 1) / 2;//新洞的父节点
        }//持续到顶端 或这 满足条件
        *(first + holeIndex) = value;//令洞值为插入值，完成插入操作
    }

    template<class RandomIterator,class Distance>
    void push_heap_d(RandomIterator first,RandomIterator last,Distance*){
        //新值一定位于底部
        //容器的最尾端，第一个洞号，(last-first)-1
        XinSTL::push_heap_aux(first,(last-first)-1,static_cast<Distance>(0),*(last-1));
    }

    //重载版本
    //使用函数对象comp代替比较操作
    template<class RandomIterator,class Distance,class T,class Compare>
    void push_heap_aux(RandomIterator first,Distance holeIndex,Distance topIndex,T value,Compare comp){
        Distance parent = (holeIndex - 1) / 2;
        while(holeIndex > topIndex && comp(*(first + parent),value)){
            *(first + holeIndex) = *(first + parent);
            holeIndex = parent;
            parent = (holeIndex - 1) / 2;
        }
        *(first + holeIndex) = value;
    }

    template<class RandomIterator,class Compare,class Distance>
    void push_heap_d(RandomIterator first,RandomIterator last,Distance*,Compare comp){
        XinSTL::push_heap_aux(first,(last-first)-1,static_cast<Distance>(0),*(last-1),comp);
    }

    template<class RandomIterator,class Compare>
    void push_heap(RandomIterator first,RandomIterator last,Compare comp){
        XinSTL::push_heap_d(first,last,distance_type(first),comp);
    }

    //*****************************************************************************
    //pop_heap
    //该函数接受两个迭代器，表示heap容器的首位，将heap的根节点取出放到容器尾部，调整heap
    

}