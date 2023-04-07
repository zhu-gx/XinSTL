#pragma once

//该头文件用于对未初始化空间构造元素

#include<xin_algobase.h>
#include<xin_construct.h>
#include<xin_utility.h>
#include<xin_type_traits.h>
#include<xin_iterator.h>

namespace XinSTL{
    //***************************************************************************
    //unintialized_copy
    //把[first,last)上的元素复制到以result为起始的空间
    //返回复制结束的位置
    template<class InputIterator,class ForwardIterator>
    ForwardIterator unchecked_unitialized_copy(InputIterator first,InputIterator last,ForwardIterator result,std::true_type){
        return XinSTL::copy(first,last,result);
    }

    template<class InputIterator,class ForwardIterator>
    ForwardIterator unchecked_unitialized_copy(InputIterator first,InputIterator last,ForwardIterator result,std::false_type){
        ForwardIterator cur = result;
        try{
            for(;first != last;++first,++cur){
                XinSTL::construct(&*cur,*first);
            }
        }catch(...){
            for(;result != cur;--cur){
                XinSTL::destroy(&*cur);
            }
        }
        return cur;
    }

    template<class InputIterator,class ForwardIterator>
    ForwardIterator unitialized_copy(InputIterator first,InputIterator last,ForwardIterator result){
        return XinSTL::unchecked_unitialized_copy(first,last,std::is_trivially_assignable<typename iterator_traits<ForwardIterator>::value_type>{});
    }

    //****************************************************************************
    //uninitialized_copy_n
    //把[first,first+n)上的内容复制到以result为起始处的空间
    //返回复制结束的地址
    template<class InputIterator,class Size,class ForwardIterator>
    ForwardIterator unchecked_unitialized_copy_n(InputIterator first,Size n,ForwardIterator result,std::true_type){
        return XinSTL::copy_n(first,n,result).second;
    }

    template<class InputIterator,class Size,class ForwardIterator>
    ForwardIterator unchecked_unitialized_copy_n(InputIterator first,Size n,ForwardIterator result,std::false_type){
        ForwardIterator cur = result;
        try{
            for(;n > 0;n--,cur++,first++){
                XinSTL::construct(&*cur,*first);
            }
        }catch(...){
            for(; result != cur;cur--){
                XinSTL::destroy(&*cur);
            }
        }
        return cur;
    }

    template<class InputIterator,class Size,class ForwardIterator>
    ForwardIterator unitialized_copy_n(InputIterator first,Size n,ForwardIterator result){
        return XinSTL::unchecked_unitialized_copy_n(first,n,result,std::is_trivially_copy_assignable<typename iterator_traits<InputIterator>::value_type>{});
    }

    //****************************************************************************
    //uninitialized_fill
    //在[first,last)区间内填充元素值
    template<class ForwardIterator,class T>
    void unchecked_uninitialized_fill(ForwardIterator first,ForwardIterator last,const T& value,std::true_type){
        XinSTL::fill(first,last,value);
    }

    template<class ForwardIterator,class T>
    void unchecked_uninitialized_fill(ForwardIterator first,ForwardIterator last,const T& value,std::false_type){
        auto cur = first;
        try{
            for(;cur != last;++cur){
                XinSTL::construct(&*cur,value);
            }
        }catch(...){
            for(;first != cur;cur++){
                XinSTL::destroy(&*first);
            }
        }
    }

    template<class ForwardIterator,class T>
    void uninitialized_fill(ForwardIterator first,ForwardIterator last,const T& value){
        XinSTL::unchecked_uninitialized_fill(first,last,value,std::is_trivially_copy_assignable<typename iterator_traits<ForwardIterator>::value_type>{});
    }

    //****************************************************************************
    //uninitialized_fill_n
    //从first位置开始，填充n个元素值，返回填充结束的位置
    template<class ForwardIterator,class Size,class T>
    ForwardIterator unchecked_uninitialized_fill_n(ForwardIterator first,ForwardIterator last,const T& value,std::true_type){
        return XinSTL::fill_n(first,n,value);
    }

    template<class ForwardIterator,class Size,class T>
    ForwardIterator unchecked_uninitialized_fill_n(ForwardIterator first,Size n,const T& value,std::false_type){
        auto cur = first;
        try{
            for(;n > 0;n--,++cur){
                XinSTL::construct(&*cur,value);
            }
        }catch(...){
            for(;first != cur;++first){
                XinSTL::destroy(&*first);
            }
        }
        return cur;
    }

    template<class ForwardIterator,class Size,class T>
    ForwardIterator uninitialized_fill_n(ForwardIterator first,Size n, const T& value){
        return XinSTL::unchecked_uninitialized_fill_n(first,n,value,std::is_trivially_assignable<typename iterator_traits<ForwardIterator>::value_type>{});
    }

    //****************************************************************************
    //uninitialized_move
    //把[first, last)上的内容移动到以 result 为起始处的空间
    //返回移动结束的位置
    template<class InputIterator,class ForwardIterator>
    ForwardIterator unchecked_uninitialized_move(InputIterator first,InputIterator last,ForwardIterator result,std::true_type){
        auto cur = result;
        try{
            for(;first != last;first++,cur++){
                XinSTL::construct(&*cur,XinSTL::move(*first));
            }
        }catch(...){
            XinSTL::destroy(result,cur);
        }
        return cur;
    }

    template<class InputIterator,class ForwardIterator>
    ForwardIterator uninitialized_move(InputIterator first,InputIterator last,ForwardIterator result){
        return XinSTL::unchecked_uninitialized_move(first,last,result,std::is_trivially_move_assignable<typename iterator_traits<InputIterator>::value_type>{});
    }

    //****************************************************************************
    //uninitialized_move_n
    // 把[first, first + n)上的内容移动到以 result 为起始处的空间
    //返回移动结束的位置
    template<class InputIterator,class Size,class ForwardIterator>
    ForwardIterator unchecked_uninitialized_move_n(InputIterator first,Size n,ForwardIterator result,std::true_type){
        return XinSTL::move(first,first+n,result);
    }

    template<class InputIterator,class Size,class ForwardIterator>
    ForwardIterator unchecked_uninitialized_move_n(InputIterator first,Size n,ForwardIterator result,std::false_type){
        auto cur = result;
        try{
            for(;n > 0;n--,first++,cur++){
                XinSTL::construct(&*cur,XinSTL::move(*first));
            }
        }catch(...){
            for(;result != cur;++result){
                XinSTL::destroy(&*result);
            }
            throw;
        }
        return cur;
    }

    template<class InputIterator,class Size,class ForwardIterator>
    ForwardIterator uninitialized_move_n(InputIterator first,Size n,ForwardIterator result){
        return XinSTL::unchecked_uninitialized_move_n(first,n,result,std::is_trivially_move_assignable<typename iterator_traits<InputIterator>::value_type>{});
    }
}