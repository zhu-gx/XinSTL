#pragma once

//该头文件包含模板类allocator
//用于管理内存的分配、释放、对象的构造、析构

#include<xin_construct.h>
#include<xin_utility.h>

namespace XinSTL{
    //模板类allocator
    //模板类代表数据类型
    template<class T>
    class allocator{
    public:
        using value_type = T;//数据类型
        using pointer = T*;//数据类型指针
        using const_pointer = const T*;//const数据类型指针
        using reference = T&;//数据类型引用
        using const_reference = const T&;//const数据类型引用
        using size_type = size_t;//数据类型指针大小
        using difference_type = ptrdiff_t;//数据类型指针距离

    public:
        //分配内存
        static T* allocate();
        static T* allocate(size_type n);
        
        //回收内存
        static void deallocate(T* ptr);
        static void deallocate(T* ptr,size_type n);

        //对象构造
        static void construct(T* ptr);
        static void construct(T* ptr,const T& value);
        static void construct(T* ptr,T&& value);

        template<class.. Args>
        static void construct(T* ptr,Args&& ...args);

        //对象析构
        static void destroy(T* ptr);
        static void destroy(T* first,T* last);
    };

    //内存分配直接使用operator new
    template<class T>
    T* allocator<T>::allocate(){
        return static_cast<T*>(::operator new(sizeof(T)));
    }

    template<class T>
    T* allocator<T>::allocate(size_type n){
        if(!n){
            return nullptr;
        }
        return static_cast<T*>(::operator new(n*sizeof(T)));
    }

    //内存回收直接使用operator delete
    template<class T>
    void allocator<T>::deallocate(T* ptr){
        if(ptr == nullptr){
            return;
        }
        ::operator delete(ptr);
    }

    template<class T>
    void allocator<T>::deallocate(T* ptr,size_type){
        if(ptr == nullptr){
            return;
        }
        ::operator delete(ptr);
    }

    //对象构造
    template<class T>
    void allocator<T>::construct(T* ptr,const T& value){
        XinSTL::construct(ptr,value);
    }

    template<class T>
    void allocator<T>::construct(T* ptr){
        XinSTL::construct(ptr);
    }

    template<class T>
    void allocator<T>::construct(T* ptr,T&& value){
        XinSTL::construct(ptr,XinSTL::move(value));
    }

    template<class T,class... Args>
    void XinSTL::construct(T* ptr,Args&&... args){
        XinSTL::construct(ptr,XinSTL::move(value));
    }

    //对象析构
    template<class T>
    void allocator<T>::destroy(T* ptr){
        XinSTL::destroy(ptr);
    }

    template<class T>
    void allocator<T>::destroy(T* first,T* last){
        XinSTL::destroy(first,last);
    }
    
}