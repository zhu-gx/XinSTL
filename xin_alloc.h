#pragma once
#ifndef __XinSTLalloc__
#define __XinSTLalloc__
#include<new>//����placement new
#include<cstddef>//����ptrdiff_t size_t
#include<cstdlib>//����exit()
#include<climits>//����UINT_MAX
#include<iostream>//����cerr
using namespace std;

#if !defined(__throw_bad_alloc)
#include<iostream>
#define __throw_bad_alloc cerr<<"out of memory"<<endl; exit(1)
#endif

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
		static void deallocate(T *p){��
			Alloc::deallocate(p,sizeof(T));
		}
	};

	//��һ��������
	//allocate()ֱ��ʹ��malloc()��deallocate()ֱ��ʹ��free()
	//ģ��C++��set_new_handler()�Դ����ڴ治������
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

		static void deallocate(void *p,size_t n){
			//��һ����������ֱ��ʹ��free
			free(p);
		}

		static void * reallocate(void *p,size_t old_sz,size_t new_sz){
			//��һ����������ֱ��ʹ��realloc
			void * result = realloc(p,new_sz);
			//���ʧ�ܣ�����NULL
			//�����ڴ治��Ĵ�������ool_realloc
			if(0 == result){
				result = oom_realloc(p,new_sz);
			}
			return result;
		}

		//���·���C++��set_new_handler
		//��Ϊû����::operator new�����Բ���ֱ������new_handler
		//ͨ������ĺ���������ָ���Լ���out_of_memory handler
		//�������⿴->void(*f)()->һ������ָ�룬����Ϊ�գ���������ҲΪ��
		//��p=void(*f)()->static void (*set_malloc_handler(p))()
		//�ȼ���:
		//using PF = staitc void (*)()
		//PF set_malloc_handler(p);
		//����set_malloc_handler�Ĳ���Ϊ*void()()���ͣ��β���Ϊf
		//����ֵҲ��*void()()����
		static void (* set_malloc_handler(void(*f)())) (){
			void (*old) () = __malloc_alloc_oom_handler;
			__malloc_alloc_oom_handler = f;
			return old;
		}
	};

	//out_of_memory handling��ֵ�趨
	//��ֵ�趨Ϊ0
	//�ú�����oom_malloc()�б����ã���Ϊ�ڴ治��ʱ�ͷ��ڴ������
	template<int inst>
	void(* __malloc_alloc_template<inst>::__malloc_alloc_oom_handler)()=0;

	//oom_malloc����
	template<int inst>
	void * __malloc_alloc_template<inst>::oom_malloc(size_t n){
		void (*my_malloc_handler)();
		void *result;

		for(;;){
			//�����ͷ�->����->�ͷ�->����->...
			my_malloc_handler = __malloc_alloc_oom_handler;
			//���û���趨�������̣��׳�����
			if(0 == my_malloc_handler){
				__throw_bad_alloc;
			}
			//���ô������̣���ͼ�ͷ��ڴ�
			(*my_malloc_handler)();
			//�ٴγ��������ڴ�
			result = malloc(n);
			if(result){
				return(result);
			}
		}
	}

	//oom_realloc()����
	template<int inst>
	void * __malloc_alloc_template<inst>::oom_realloc(void *p,size_t n){
		void (*my_malloc_handler)();
		void * result;
		for(;;){
			//�����ͷ�->����->�ͷ�->����->...
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

	//����instΪ0��oom_realloc()������
	typedef __malloc_alloc_template<0> malloc_alloc;


	//�ڶ���������
	//ά��16����������free lists���ڴ��memory pool��malloc()���ö���
	//����ڴ治�㣬��ת���õ�һ��������
	//��������������128bytes���͵��õ�һ��������
	template<int inst>
	class _default_alloc_template{
	private:
		enum ALIGN {_align = 8};//С��������ϵ��߽�
		enum MAX_BYTES {_max_bytes = 128};//С�����������
		enum NFREELISTS {
			_nfreelists = MAX_BYTES::_max_bytes/ALIGN::_align
		};//free-lists����

		//ROUND_UP()��bytes�ϵ���8�ı���
		//�ȼ���((bytes+7)/8)*8
		static size_t ROUND_UP(size_t bytes){
			return ((bytes+ALIGN::_align-1) & ~(ALIGN::_align-1));
		}

		//free-lists�Ľڵ㹹��
		union obj{
			//obj��Ϊһ��ָ�룬ָ����ͬ��ʽ����һ��obj
			union obj * free_list_link;
			//obj��Ϊһ��ָ�룬ָ��ʵ������
			char client_data[1];
		}

		//free_list����16��
		static obj * free_list[NFREELISTS::_nfreelists]={
			0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,
		};
		//���������С��ѡ���Ӧ��free-list,n��0��ʼ
		//�ȼ���(bytes+7)/8-1
		static size_t NFREELISTS_INDEX(size_t bytes){
			return (((bytes) + ALIGN::_align-1) / ALIGN::_align-1);
		}

		//����һ����СΪn�Ķ��󣬲����ܼ����СΪn���������鵽free-list
		static void *refill(size_t n);
		//����һ���ռ䣬������nobjs����СΪsize������
		//�������nobjs�����鲻�㣬��nobjs�п��ܽ���
		static char *chunk_alloc(size_t size,int &nobjs);

		//chunk����״̬
		static char *start_free=0;//�ڴ����ʼλ�ã�ֻ��chunk_alloc()�б仯
		static char *end_free=0;//�ڴ�ؽ���λ�ã�ֻ��chunk_alloc()�б仯
		static size_t heap_size=0;

	public:
		static void * allocate(size_t bytes);
		static void deallocate(void *p,size_t bytes);
		static void * reallocate(void *p,size_t old_sz,size_t new_sz);

		//�ڶ����������ı�׼�ӿں���allocate()
		//����n�Ĵ�С��ȡ��Ӧ���ڴ����ò���
		static void * allocate(size_t n){//n>0
			obj * volatile my_free_list;
			obj * result;
			//n>128�����õ�һ��������
			if(n>(size_t)MAX_BYTES::_max_bytes){
				return (malloc_alloc::allocate(n));
			}
			//Ѱ��16��free-list���ʵ���һ��
			my_free_list = free_list + NFREELISTS_INDEX(n);
			//resultָ�򼴽������õ�chunk
			result = *my_free_list;
			if(result == 0){
				//û���ҵ����õ�free list��׼���������free_list
				void *r = refill(ROUND_UP(n));
				return r;
			}
			//����free list
			//�ñ�ŵ�free listԭ��ָ��ĵ�һ��chunk������
			//��Ϊָ����һ��chunk����δ�����õ��Ǹ�
			*my_free_list = result -> free_list_link;
			return result;
		}

		//�ڶ����������ı�׼�ӿں���deallocate()
		//����n�Ĵ�С��ȡ��Ӧ���ڴ���ղ���
		//����Ӧ���ڴ���ս��ڴ��
		static void deallocate(void *p,size_t n){ //p��������0
			//ָ��qָ������ڴ��ַ
			//ָ��my_free_listָ���Ӧ��ŵ��ڴ�ص�ַ
			obj *q = (obj*) p;
			obj * volatile * my_free_list;
			//����128�����õ�һ��������
			if(n > (size_t)MAX_BYTES::_max_bytes){
				malloc_alloc::deallocate(p,n);
				return;
			}
			//���Ҷ�Ӧ��ŵ�free list
			my_free_list = free_list + NFREELISTS_INDEX(n);
			//����free list����������
			//��qָ��ǰ��Ӧ����ڴ���еĵ�һ��chunk
			q -> free_list_link = *my_free_list;
			//���Ӧ�ڴ��ָ���¼����chunk
			*my_free_list = q;
		}

		//��free list��û�п�������ʱ
		//����refill()
		//ͨ��chunk_alloc()������ȱʡ����20������
		void * refill(size_t n){
			int nobjs = 20;
			//����chunk_alloc()
			//����ȡ��nobjs��chunk��Ϊfree list���½ڵ�
			//����nobjs��pass by reference
			char * chunk = chunk_alloc(n,nobjs);
			objs * volatile * my_free_list;
			obj * result;
			obj * current_obj,obj * next_obj;
			int i;

			//���ֵ���һ��chunk�������chunk���ظ�������
			//free listû���½ڵ�
			if(1 == nobjs){
				return(chunk);
			}
			//����׼������free list
			my_free_list = free_list + NFREELISTS_INDEX(n);

			//������chunk�ռ��ڽ���free list
			result = (obj*)chunk;//��һ��׼�����ظ�������
			//��������free listָ���µĿռ�
			*my_free_list = next_obj = (obj*)(chunk+n);
			//���½�free list�ĸ��ڵ㴮������
			for(int i = 1;;i++){//��1��ʼ����0�����ص���
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
