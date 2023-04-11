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
    //下溯percolate down
    template<class RandomIterator,class T,class Distance>
    void adjust_heap(RandomIterator first,Distance holeIndex,Distance len,T value){
        //先进行下溯percolate down过程
        Distance topIndex = holeIndex;
        Distance rightchild = 2 * holeIndex + 2;//洞节点之右子节点
        while(rightchild < len){
            //比较洞节点的左右两个子节点的值，然后以rightchild表示较大节点
            if(*(first + rightchild) < *(first + rightchild - 1)){
                --rightchild;
            }
            //令较大子值作为洞值，再令洞号下移至较大子节点处
            *(first + holeIndex) = *(first + rightchild);
            holeIndex = rightchild;
            //找出新洞节点的右子节点
            rightchild = 2 * (rightchild + 1);
        }
        if(rightchild == len){
            //如果没有右子节点，只有左子节点
            //令左子节点值作为洞值，再令洞值下一到左子节点处
            *(first + holeIndex) = *(first + (rightchild - 1));
            holeIndex = rightchild - 1;
        }
        //再执行一次上溯percolate up
        XinSTL::push_heap_aux(first,holeIndex,topIndex,value);
    }

    template<class RandomIterator,class T,class Distance>
    void pop_heap_aux(RandomIterator first,RandomIterator last,RandomIterator result,T value,Distance*){
        //设定尾值为首值，于是尾值即为所求的结果
        //可由客户端以底部容器pop_back取出尾值
        *result = *first;
        //重新调整heap，洞号为0（根），欲调整值为value
        XinSTL::adjust_heap(first,static_cast<Distance>(0),last-first,value);
    }

    template<class RandomIterator>
    void pop_heap(RandomIterator first,RandomIterator last){
        //根据pop的结果
        //应为底部容器的第一个元素
        //设定欲调整值为尾值，首值调至尾节点（reuslt->last-1）,重整[first,last-1)
        XinSTL::pop_heap_aux(first,last-1,last-1,*(last-0),distance_type(first));
    }

    //重载版本使用函数对象
    template<class RandomIterator,class T,class Distance,class Compare>
    void adjust_heap(RandomIterator first,Distance holeIndex,Distance len,T value,Compare comp){
        //先进性下溯percolate down过程
        Distance topIndex = holeIndex;
        Distance rightchild = 2 * holeIndex + 2;
        while(rightchild < len){
            if(comp(*(first + rightchild),*(first + rightchild - 1))){
                --rightchild;
            }
            *(first + holeIndex) = *(first + rightchild);
            holeIndex = rightchild;
            rightchild = 2 * (rightchild + 1);
        }
        if(rightchild == len){
            *(first + holeIndex) = *(first + (rightchild - 1));
            holeIndex = rightchild - 1;
        }
        //再执行一次percolate up
        XinSTL::push_heap_aux(first,holeIndex,topIndex,value,comp);
    }

    template<class RandomIterator,class T,class Distance,class Compare>
    void pop_heap_aux(RandomIterator first,RandomIterator last,RandomIterator result,T value,Distance*,Compare comp){
        *result = *first;
        XinSTL::adjust_heap(first,static_cast<Distance>(0),last-first,value,comp);
    }

    template<class RandomIterator,class Compare>
    void pop_heap(RandomIterator first,RandomIterator last,Compare comp){
        XinSTL::pop_heap_aux(first,last-1,last-1,*(last-1),distance_type(first),comp);
    }

    //*****************************************************************************
    //sort_heap
    //该函数接受两个迭代器，表示heap容器的首位，不断执行pop_heap操作，直到首尾相差1
    template<class RandomIterator>
    void sort_heap(RandomIterator first,RandomIterator last){
        //每次执行一次pop_heap
        //最大的元素放到尾部，直到容器最多只有一个元素，完成排序
        while(last - first > 1){
            XinSTL::pop_heap(first,last--);
        }
    }

    //重载版本使用函数对象comp完成操作
    template<class RandomIterator,class Compare>
    void sort_heap(RandomIterator first,RandomIterator last,Compare comp){
        while(last - first > 1){
            XinSTL::pop_heap(first,last--,comp);
        }
    }

    //**************************************************************************
    //make_heap
    //该函数接受两个迭代器，表示heap容器的首位，把容器内的元素变成一个heap
    template<class RandomIterator,class Distance>
    void make_heap_aux(RandomIterator first,RandomIterator last,Distance*){
        if(last - first < 2){
            return;
        }
        Distance len = last - first;
        Distance holeIndex = (len - 2) / 2;
        while(1){
            //重排以holeIndex为首的子树
            XinSTL::adjust_heap(first,holeIndex,len,*(first + holeIndex));
            if(holeIndex == 0){
                return;
            }
            holeIndex--;
        }
    }

    template<class RandomIterator>
    void make_heap(RandomIterator first,RandomIterator last){
        XinSTL::make_heap_aux(first,last,distance_type(first));
    }

    // 重载版本使用函数对象 comp 代替比较操作
    template <class RandomIter, class Distance, class Compared>
    void make_heap_aux(RandomIter first, RandomIter last, Distance*, Compared comp)
    {
        if (last - first < 2)
            return;
        Distance len = last - first;
        Distance holeIndex = (len - 2) / 2;
        while (true)
        {
            // 重排以 holeIndex 为首的子树
            XinSTL::adjust_heap(first, holeIndex, len, *(first + holeIndex), comp);
            if (holeIndex == 0)
            return;
            holeIndex--;
        }
    }

    template <class RandomIter, class Compared>
    void make_heap(RandomIter first, RandomIter last, Compared comp)
    {
        XinSTL::make_heap_aux(first, last, distance_type(first), comp);
    }
}