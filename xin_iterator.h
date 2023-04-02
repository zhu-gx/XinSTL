#pragma once

#include<cstddef>
#include<xin_type_traits.h>

namespace XinSTL{

//定义五种迭代器类型
//这种迭代器所指的对象，不允许外界改变，只读,支持++
struct input_iterator_tag{};
//只写，支持++
struct output_iterator_tag{};
//允许写入型算法的操作，因此继承自只读，支持++
struct forward_iterator_tag : public input_iterator_tag{};
//双向移动迭代器，当然属于前向迭代器的一种，支持++和--
struct bidirectional_iterator_tag : public forward_iterator_tag{};
//随机访问迭代器，支持++/--/+n/-n/...
struct random_access_iterator_tag : public bidirectional_iterator_tag{};

//iterator模板
template<class Category,class T,class Distance = ptrdiff_t,class Pointer = T*,class Reference = T&>
struct iterator{
    using iterator_category = Category;//迭代器类型
    using value_type = T;//迭代器所指类型对象
    using pointer = Pointer;//迭代器所指对象的类型指针
    using reference = Reference;//迭代器所指对象的类型引用
    using difference_type = Distance;//两个迭代器之间的距离
};

//iterator traits
//该函数用来返回iterator_traits_helper的第二个参数
//如果是false则没必要萃取
//如果是true则继承iterator_traits_implement
//用来判断是否有迭代器型别iterator category
template<class T>
struct has_iterator_category{
private:
    //two的大小是2
    struct two {
        char a;
        char b;
    };

    template<class U>
    static two test(...);

    template<class U>
    static char test(typename U::iterator_category* = 0);

public:
    static const bool value = sizeof(test<T>(0)) == sizeof(char);
};

//如果不能隐式转换为input型或者output型
//没有迭代器类型
//模板元编程技法，需要定义一个一般情况，保证该类能通过编译器的编译
template<class Iterator,bool>
struct iterator_traits_implement{};

//可以隐式转换
template<class Iterator>
struct iterator_traits_implement<Iterator,true>{
    using iterator_category = typename Iterator::iterator_category;
    using value_type = typename Iterator::value_type;
    using pointer = typename Iterator::pointer;
    using reference = typename Iterator::reference;
    using difference_type = typename Iterator::difference_type;
};

//没有迭代器类型
template<class Iteartor,bool>
struct iterator_traits_helper{};

//如果可以隐式转换为input或者output
//公有继承
template<class Iterator>
struct iterator_traits_helper<Iterator,true>
 : public iterator_traits_implement<Iterator,
 std::is_convertible<typename Iterator::iterator_category,input_iterator_tag>::value ||
 std::is_convertible<typename Iterator::iterator_category,output_iterator_tag>::value>
{};

//萃取迭代器的特性
template<class Iterator>
struct iterator_traits : 
public iterator_traits_helper<Iterator,has_iterator_category<Iterator>::value>{};

//针对原生指针的偏特化版本
template<class T>
struct iterator_traits<T*>{
    using iterator_category = random_access_iterator_tag;
    using value_type = T;
    using pointer = T*;
    using reference = T&;
    using difference_type = ptrdiff_t;
};

template<class T>
struct iterator_traits<const T*>{
    using iterator_category = random_access_iterator_tag;
    using value_type = T;
    using pointer = const T*;
    using reference = const T&;
    using difference = ptrdiff_t;
};

//T型迭代器可隐式转换为U型迭代器
template<class T,class U,bool = has_iterator_category<iterator_traits<T>>::value>
struct has_iterator_category_of
 : public m_bool_constant<std::is_convertible<typename iterator_traits<T>::iterator_category,U>::value>
 {};

 //T型迭代器不可隐式转换为U型迭代器
 template<class T,class U>
 struct has_iterator_category_of<T,U,false> : public m_false_type{};

 //判断是否input_iterator
 template<class Iterator>
 struct is_input_iterator : public has_iterator_category_of<Iterator,input_iterator_tag>{};

//判断是否output_iterator
template<class Iterator>
struct is_output_itearator : public has_iterator_category_of<Iterator,output_iterator_tag>{};

//判断是否forward_iterator
template<class Iterator>
struct is_bidirectional_iterator : public has_iterator_category_of<Iterator,forward_iterator_tag>{};

//判断是否random_access_iterator
template<class Iterator>
struct is_random_access_iterator : public has_iterator_category_of<Iterator,random_access_iterator_tag>{};

//判断是否迭代器
//所有迭代器都继承自input和output
template<class Iterator>
struct is_iterator : public m_bool_constant<is_input_iterator<Iterator>::value || 
is_output_itearator<Iterator>::value>{};

//萃取某个迭代器的category
template<class Iterator>
typename iterator_traits<Iterator>::iterator_category
iterator_category(const Iterator&){
    using Category = iterator_traits<Iterator>::iterator_category;
    return Category();
}

//萃取某个迭代器的distance_type
template<class Iterator>
typename iterator_traits<Iterator>::difference_type* distance_type(const Iterator&){
    return static_cast<typename iterator_traits<Iterator>::difference_type*>(0);
}

//萃取某个迭代器的value_type
template<class Iterator>
typename iterator_traits<Iterator>::value_type*
value_type(const Iterator&){
    return static_cast<typename iterator_traits<Iterator>::value_type*>(0);
}

//******************************************
//以下函数用于计算迭代器之间的距离
//input版本
template<class InputIterator>
typename iterator_traits<InputIterator>::difference_type
distance_dispatch(InputIterator first,InputIterator last,input_iterator_tag){
    typename iterator_traits<InputIterator>::difference_type n = 0;
    while(first != last){
        ++first;
        ++n;
    }
    return n;
}

//random_access版本
template<class RandomIterator>
typename iterator_traits<RandomIterator>::difference_type
distance_dispatch(RandomIterator first,RandomIterator last,random_access_iterator_tag){
    return last - first;
}

//计算迭代器距离的函数，根据迭代器类型的不同采取不同的底层实现
template<class Iterator>
typename iterator_traits<Iterator>::difference_type
distance(Iterator first,Iterator last){
    return distance_dispatch(first,last,iterator_category(first));
}

//****************************************
//以下函数用于让迭代器前进n个距离
//根据迭代器类型的不同采用不同的底层实现
template<class InputIterator,class Distance>
void advance(InputIterator& i,Distance n){
    advance_dispatch(i,n,iterator_category());
}

//input_iterator版本
template<class InputIterator,class Distance>
void advance_dispatch(InputIterator& i,Distance n,input_iterator_tag){
    while(n--){
        ++i;
    }
}

//bidirectional_iterator版本
template<class BidirectionalIterator,class Distance>
void advance_dispatch(BidirectionalIterator& i,Distance n,bidirectional_iterator_tag){
    if(n >= 0){
        while(n--){
            ++i;
        }
    }else{
        while(n++){
            --i;
        }
    }
}

//random_access_iterator版本
template<class RandomIterator,class Distance>
void advance_dispatch(RandomIterator& i,Distance n,random_access_iterator_tag){
    i += n;
}

//***********************************************
//模板类，reverse_iterator
//代表反向迭代器，使前进为后退，后退为前进
template<class Iterator>
class reverse_iterator{
private:
    Iterator current;//记录原本对应的正向迭代器

public:
    //反向迭代器的五种相应型别
    using iterator_category = typename iterator_traits<Iterator>::iterator_category;
    using value_type = typename iterator_traits<Iterator>::value_type;
    using difference_type = typename iterator_traits<Iterator>::difference_type;
    using pointer = typename iterator_traits<Iterator>::pointer;
    using reference = typename iterator_traits<Iterator>::reference;
    
    using iterator_type = Iterator;
    using self = reverse_iterator<Iterator>;

public:
    //构造函数
    reverse_iterator(){};
    //防止隐式转换
    explicit reverse_iterator(iterator_type i):current(i){}
    //如果传入的就是反向迭代器
    reverse_iterator(const self& rhs):current(rhs.current){}

    //取出对应的正向迭代器
    iterator_type base()const{
        return current;
    }

    //重载操作符
    //取值操作，对应的是正向迭代器的前一个位置
    reference operator*()const{
        auto temp = current;
        return *--temp;
    }

    //迭代器对应值取地址操作
    pointer operator->()const{
        return &(operator*());
    }

    //++变--
    self& operator++(){
        --current;
        return *this;
    }

    //--变++
    self& operator--(int){
        self tmp = *this;
        ++current;
        return tmp;
    }

    self& operator+=(difference_type n){
        current -= n;
        return *this;
    }

    self operator+(difference_type n)const{
        return self(current-n);
    }

    self& operator-=(difference_type n){
        current += n;
        return *this;
    }

    self operator-(difference_type n)const{
        return self(current + n);
    }

    self operator[](difference_type n)const{
        return *(*this+n);
    }
};

//重载operator-
template<class Iterator>
typename reverse_iterator<Iterator>::difference_type
operator-(const reverse_iterator<Iterator>& lhs,const reverse_iterator<Iterator>& rhs){
    return rhs.base() - lhs.base();
}

//重载比较操作符
template<class Iterator>
bool operator==(const reverse_iterator<Iterator>& lhs,const reverse_iterator<Iterator>& rhs){
    return lhs.base() == rhs.base();
}

template<class Iterator>
bool operator<(const reverse_iterator<Iterator>& lhs,const reverse_iterator<Iterator>& rhs){
    return lhs.base() > rhs.base();
}

template<class Iterator>
bool operator!=(const reverse_iterator<Iterator>& lhs,const reverse_iterator<Iterator>& rhs){
    return !(rhs==lhs);
}

template<class Iterator>
bool operator>(const reverse_iterator<Iterator>& lhs,const reverse_iterator<Iterator>& rhs){
    return rhs < lhs;//调用已经重定义的<运算符
}

template<class Iterator>
bool operator>=(const reverse_iterator<Iterator>& lhs,reverse_iterator<Iterator>& rhs){
    return !(rhs < lhs);
}

template<class Iterator>
bool operator<=(const reverse_iterator<Iterator>& lhs,const reverse_iterator<Iterator>& rhs){
    return !(lhs < rhs);
}


}