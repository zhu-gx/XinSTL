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
	template<class T>
	inline T* _allocate(ptrdiff_t size, T*) {
		set_new_handler(0);
		T* tmp = (T*)(::operator new((size_t)(size * sizeof(T))));
		if (tmp == 0) {
			std::cerr << "out of memory" << std::endl;
			exit(1);
		}
		return tmp;
	}

	template<class T>
	inline void _deallocate(T* buffer) {
		::operator delete(buffer);
	}

	template<class T1,class T2>
	inline void _construct(T1* p, const T2& value) {
		new(p) T1(value);//placement new 调用T1的构造函数
	}

	template<class T>
	inline void _destroy(T* ptr) {
		ptr->~T();
	}

	template<class T>
	class allocator {
	public:
		typedef T			value_type;
		typedef T*			pointer;
		typedef const T*	const_pointer;
		typedef T&			reference;
		typedef const T&	const_reference;
		typedef size_t		size_type;
		typedef ptrdiff_t	difference_type;

		//rebind实现两个不同但是两者互相有关的类型
		//使用同一种内存分配方法
		template<class U>
		struct rebind {
			typedef allocator<U> other;
		};

		//hint used for locality
		pointer allocate(size_type n, const void* hint = 0) {
			return _allocate((difference_type)n, (pointer)0);
		}

		void deallocate(pointer p, size_type n) {
			_deallocate(p);
		}

		pointer address(reference x) {
			return (pointer)&x;
		}

		const_pointer const_address(const_reference x) {
			return (const_pointer)&x;
		}

		size_type max_size()const {
			return size_type(UINT_MAX / sizeof(T));
		}
	};
}

#endif //__XinSTLalloc__