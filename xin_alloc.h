#pragma once
#ifndef __XinSTLalloc__
#define __XinSTLalloc__
#include<new>//用于placement new
#include<cstddef>//用于ptrdiff_t size_t
#include<cstdlib>//用于exit()
#include<climits>//用于UINT_MAX
#include<iostream>//用于cerr
using namespace std;

#if !defined(__throw_bad_alloc)
#include<iostream>
#define __throw_bad_alloc cerr<<"out of memory"<<endl; exit(1)
#endif

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
		static void deallocate(T *p){·
			Alloc::deallocate(p,sizeof(T));
		}
	};

	//第一级配置器
	//allocate()直接使用malloc()，deallocate()直接使用free()
	//模拟C++的set_new_handler()以处理内存不足的情况
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

		static void deallocate(void *p,size_t n){
			//第一级配置器，直接使用free
			free(p);
		}

		static void * reallocate(void *p,size_t old_sz,size_t new_sz){
			//第一季配置器，直接使用realloc
			void * result = realloc(p,new_sz);
			//如果失败，返回NULL
			//调用内存不足的处理函数，ool_realloc
			if(0 == result){
				result = oom_realloc(p,new_sz);
			}
			return result;
		}

		//以下仿真C++的set_new_handler
		//因为没有用::operator new，所以不能直接运用new_handler
		//通过下面的函数，可以指定自己的out_of_memory handler
		//从内向外看->void(*f)()->一个函数指针，参数为空，返回类型也为空
		//设p=void(*f)()->static void (*set_malloc_handler(p))()
		//等价于:
		//using PF = staitc void (*)()
		//PF set_malloc_handler(p);
		//函数set_malloc_handler的参数为*void()()类型，形参名为f
		//返回值也是*void()()类型
		static void (* set_malloc_handler(void(*f)())) (){
			void (*old) () = __malloc_alloc_oom_handler;
			__malloc_alloc_oom_handler = f;
			return old;
		}
	};

	//out_of_memory handling初值设定
	//初值设定为0
	//该函数在oom_malloc()中被调用，作为内存不足时释放内存的例程
	template<int inst>
	void(* __malloc_alloc_template<inst>::__malloc_alloc_oom_handler)()=0;

	//oom_malloc定义
	template<int inst>
	void * __malloc_alloc_template<inst>::oom_malloc(size_t n){
		void (*my_malloc_handler)();
		void *result;

		for(;;){
			//不断释放->配置->释放->配置->...
			my_malloc_handler = __malloc_alloc_oom_handler;
			//如果没有设定配置例程，抛出错误
			if(0 == my_malloc_handler){
				__throw_bad_alloc;
			}
			//调用处理例程，试图释放内存
			(*my_malloc_handler)();
			//再次尝试配置内存
			result = malloc(n);
			if(result){
				return(result);
			}
		}
	}

	//oom_realloc()定义
	template<int inst>
	void * __malloc_alloc_template<inst>::oom_realloc(void *p,size_t n){
		void (*my_malloc_handler)();
		void * result;
		for(;;){
			//不断释放->配置->释放->配置->...
			my_malloc_handler = __malloc_alloc_oom_handler;
			if(0 == my_malloc_handler){
				__throw_bad_alloc;
			}
			result = realloc(p,n);
			if(result){
				return(result);
			}
		}
	}

	//参数inst为0的oom_realloc()重命名
	typedef __malloc_alloc_template<0> malloc_alloc;


	//第二级配置器
	//维护16个自由链表free lists，内存池memory pool以malloc()配置而成
	//如果内存不足，则转调用第一级配置器
	//如果需求区块大于128bytes，就调用第一级配置器
	template<int inst>
	class _default_alloc_template{
	private:
		enum ALIGN {_align = 8};//小型区块的上调边界
		enum MAX_BYTES {_max_bytes = 128};//小型区块的上限
		enum NFREELISTS {
			_nfreelists = MAX_BYTES::_max_bytes/ALIGN::_align
		};//free-lists个数

		//ROUND_UP()将bytes上调至8的倍数
		//等价于((bytes+7)/8)*8
		static size_t ROUND_UP(size_t bytes){
			return ((bytes+ALIGN::_align-1) & ~(ALIGN::_align-1));
		}

		//free-lists的节点构造
		union obj{
			//obj视为一个指针，指向相同形式的另一个obj
			union obj * free_list_link;
			//obj视为一个指针，指向实际区块
			char client_data[1];
		}

		//free_list共有16个
		static obj * free_list[NFREELISTS::_nfreelists]={
			0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,
		};
		//根据区块大小，选择对应的free-list,n从0开始
		//等价于(bytes+7)/8-1
		static size_t NFREELISTS_INDEX(size_t bytes){
			return (((bytes) + ALIGN::_align-1) / ALIGN::_align-1);
		}

		//返回一个大小为n的对象，并可能加入大小为n的其他区块到free-list
		static void *refill(size_t n);
		//配置一大块空间，可容纳nobjs个大小为size的区块
		//如果配饰nobjs个区块不便，则nobjs有可能降低
		static char *chunk_alloc(size_t size,int &nobjs);

		//chunk分配状态
		static char *start_free=0;//内存池起始位置，只在chunk_alloc()中变化
		static char *end_free=0;//内存池结束位置，只在chunk_alloc()中变化
		static size_t heap_size=0;

	public:
		static void * allocate(size_t bytes);
		static void deallocate(void *p,size_t bytes);
		static void * reallocate(void *p,size_t old_sz,size_t new_sz);

		//第二级配置器的标准接口函数allocate()
		//根据n的大小采取对应的内存配置操作
		static void * allocate(size_t n){//n>0
			obj * volatile my_free_list;
			obj * result;
			//n>128，调用第一级配置器
			if(n>(size_t)MAX_BYTES::_max_bytes){
				return (malloc_alloc::allocate(n));
			}
			//寻找16个free-list中适当的一个
			my_free_list = free_list + NFREELISTS_INDEX(n);
			//result指向即将被调用的chunk
			result = *my_free_list;
			if(result == 0){
				//没有找到可用的free list，准备重新填充free_list
				void *r = refill(ROUND_UP(n));
				return r;
			}
			//调整free list
			//该编号的free list原本指向的第一个chunk被调用
			//改为指向下一个chunk，即未被调用的那个
			*my_free_list = result -> free_list_link;
			return result;
		}

		//第二级配置器的标准接口函数deallocate()
		//根据n的大小采取对应的内存回收操作
		//将对应的内存回收进内存池
		static void deallocate(void *p,size_t n){ //p不可以是0
			//指针q指向回收内存地址
			//指针my_free_list指向对应编号的内存池地址
			obj *q = (obj*) p;
			obj * volatile * my_free_list;
			//大于128，调用第一级配置器
			if(n > (size_t)MAX_BYTES::_max_bytes){
				malloc_alloc::deallocate(p,n);
				return;
			}
			//查找对应编号的free list
			my_free_list = free_list + NFREELISTS_INDEX(n);
			//调整free list，回收区块
			//让q指向当前对应编号内存池中的第一个chunk
			q -> free_list_link = *my_free_list;
			//令对应内存池指向新加入的chunk
			*my_free_list = q;
		}

		//当free list中没有可用区块时
		//调用refill()
		//通过chunk_alloc()函数，缺省分配20个区块
		void * refill(size_t n){
			int nobjs = 20;
			//调用chunk_alloc()
			//尝试取得nobjs个chunk作为free list的新节点
			//其中nobjs是pass by reference
			char * chunk = chunk_alloc(n,nobjs);
			objs * volatile * my_free_list;
			obj * result;
			obj * current_obj,obj * next_obj;
			int i;

			//如果值获得一个chunk，则这个chunk返回给调用者
			//free list没有新节点
			if(1 == nobjs){
				return(chunk);
			}
			//否则，准备纳入free list
			my_free_list = free_list + NFREELISTS_INDEX(n);

			//以下在chunk空间内建立free list
			result = (obj*)chunk;//这一块准备返回给调用者
			//以下引导free list指向新的空间
			*my_free_list = next_obj = (obj*)(chunk+n);
			//以下将free list的各节点串接起来
			for(int i = 1;;i++){//从1开始，第0个返回调用
				current_obj = next_obj;
				next_obj = (obj*)((char*)next_obj+n);
				if(nobjs-1 == i){
					current_obj -> free_list_link = 0;
					break;
				}else{
					current_obj -> free_list_link = next_obj;
				}
			}
			return(result);
		}
	};
}

#endif //__XinSTLalloc__
