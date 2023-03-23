#pragma once
#ifndef __XinSTLalloc__
#define __XinSTLalloc__
#include<new>//用于placement new
#include<cstddef>//用于ptrdiff_t size_t
#include<cstdlib>//用于exit()
#include<climits>//用于UINT_MAX
#include<iostream>//用于cerr
using namespace std;

namespace XinSTL {
	//第一级配置器直接使用malloc()和free()
	//第二级配置器：
	//配置区块超过128bytes时->足够大->调用第一级配置器
	//配置区块小于128bytes时->过小->memory pool整理方式

	//接口包装，符合STL规格
	//SGI STL的配置器全部使用这个接口
	template<class T,class Alloc>
	class simple_alloc{
	public:
		static T *allocate(size_t n){
			return 0 == n ? 0 : (T*)Alloc::allocate(n*sizeof(T));
		}
		static T *allocate(void){
			return (T*)Alloc::allocate(sizeof(T));
		}
		static void deallocate(T *p,size_t n){
			if(0 != n) Alloc::deallocate(p,n*sizeof(T));
		}
		static void deallocate(T *p){
			Alloc::deallocate(p,sizeof(T));
		}
	};

	//第一级配置器
	//allocate()直接使用malloc()，deallocate()直接使用free()
	//模拟C++的set_new_handler()以处理内存不足的情况
	#if !defined(__throw_bad_alloc)
	#include<iostream>
	#define __throw_bad_alloc cerr<<"out of memory"<<endl; exit(1)
	#endif
	template<int inst>
	class __malloc_alloc_template{
	private:
		//以下所代表的函数用来处理内存不足的情况
		//oom:out of memory
		//前两者是指针函数，函数的返回值是指针
		static void *oom_malloc(size_t);
		static void *oom_realloc(void *,size_t);
		//函数指针，指向函数的指针
		static void (* __malloc_alloc_oom_handler)();

	public:
		static void * allocate(size_t n){
			//第一级配置器直接使用malloc()
			//malloc()返回的类型是无类型指针void*
			//使用时需要进行强制类型转换
			void *result = malloc(n);
			//无法满足需求时，调用oom_malloc()
			if(0 == result){
				result = oom_malloc(n);
			}
			return result;
		}

		static void deallocate(void *p,size_t /**/)
	};

	//第二级配置器
	//维护16个自由链表free lists，内存池memory pool以malloc()配置而成
	//如果内存不足，则转调用第一级配置器
	//如果需求区块大于128bytes，就调用第一级配置器

}

#endif //__XinSTLalloc__