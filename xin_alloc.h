#pragma once
#ifndef __XinSTLalloc__
#define __XinSTLalloc__
#include<new>//����placement new
#include<cstddef>//����ptrdiff_t size_t
#include<cstdlib>//����exit()
#include<climits>//����UINT_MAX
#include<iostream>//����cerr
using namespace std;

namespace XinSTL {
	//��һ��������ֱ��ʹ��malloc()��free()
	//�ڶ�����������
	//�������鳬��128bytesʱ->�㹻��->���õ�һ��������
	//��������С��128bytesʱ->��С->memory pool����ʽ

	//�ӿڰ�װ������STL���
	//SGI STL��������ȫ��ʹ������ӿ�
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

	//��һ��������
	//allocate()ֱ��ʹ��malloc()��deallocate()ֱ��ʹ��free()
	//ģ��C++��set_new_handler()�Դ����ڴ治������
	#if !defined(__throw_bad_alloc)
	#include<iostream>
	#define __throw_bad_alloc cerr<<"out of memory"<<endl; exit(1)
	#endif
	template<int inst>
	class __malloc_alloc_template{
	private:
		//����������ĺ������������ڴ治������
		//oom:out of memory
		//ǰ������ָ�뺯���������ķ���ֵ��ָ��
		static void *oom_malloc(size_t);
		static void *oom_realloc(void *,size_t);
		//����ָ�룬ָ������ָ��
		static void (* __malloc_alloc_oom_handler)();

	public:
		static void * allocate(size_t n){
			//��һ��������ֱ��ʹ��malloc()
			//malloc()���ص�������������ָ��void*
			//ʹ��ʱ��Ҫ����ǿ������ת��
			void *result = malloc(n);
			//�޷���������ʱ������oom_malloc()
			if(0 == result){
				result = oom_malloc(n);
			}
			return result;
		}

		static void deallocate(void *p,size_t /**/)
	};

	//�ڶ���������
	//ά��16����������free lists���ڴ��memory pool��malloc()���ö���
	//����ڴ治�㣬��ת���õ�һ��������
	//��������������128bytes���͵��õ�һ��������

}

#endif //__XinSTLalloc__