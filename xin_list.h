#pragma once

#include<initializer_list>
#include<xin_iterator.h>
#include<xin_memory.h>
#include<xin_functional.h>
#include<xin_utility.h>
#include<xin_exceptdef.h>

namespace XinSTL{
    template<class T>
    struct list_node_base;

    template<class T>
    struct list_node;

    template<class T>
    struct node_traits{
        typedef list_node_base<T>* base_ptr;
        typedef list_node<T>* node_ptr;
    };

    //list的节点结构
    template<class T>
    struct list_node_base{
        typedef typename node_traits<T>::base_ptr base_ptr;
        typedef typename node_traits<T>::node_ptr node_ptr;

        base_ptr prev;//前一个节点
        base_ptr next;//下一节点

        list_node_base() = default;

        node_ptr as_node(){
            return static_cast<node_ptr>(self());
        }

        void unlink(){
            prev = next = self();
        }

        base_ptr self(){
            return static_cast<base_ptr>(&*this);
        }
    };

    template<class T>
    struct list_node : public list_node_base<T>{
        typedef typename node_traits<T>::base_ptr base_ptr;
        typedef typename node_traits<T>::node_ptr node_ptr;

        T value;//数据域

        list_node() = default;
        list_node(const T& v) : value(v){}
        list_node(T&& v) : value(XinSTL::move(v)){}

        base_ptr as_base(){
            return static_cast<base_ptr>(&*this);
        }

        node_ptr self(){
            return static_cast<node_ptr>(&*this);
        }
    };

    //list的迭代器设计
    template<class T>
    struct list_iterator : public XinSTL::iterator<XinSTL::bidirectional_iterator_tag,T>{
        using value_type = T;
        using pointer = T*;
        using reference = T&;
        using base_ptr = typename node_traits<T>::base_ptr;
        using node_ptr = typename node_traits<T>::node_ptr;
        using self = list_iterator<T>;

        base_ptr node_;//指向当前节点

        //构造函数
        list_iterator() = default;
        list_iterator(base_ptr x) : node_(x){}
        list_iterator(node_ptr x) : node_(x->as_base()){}
        list_iterator(const list_iterator& rhs) : node_(rhs.node_){}

        //重载操作符
        reference operator*()const{
            return node_->as_node()->value;
        }

        pointer operator->()const{
            return &(operator*());
        }

        self& operator++(){
            XINSTL_DEBUG(node_ != nullptr);
            node_ = node_->next;
            return *this;
        }

        self operator++(int){
            self temp = *this;
            ++*this;
            return temp;
        }

        self& operator--(){
            XINSTL_DEBUG(node_ != nullptr);
            node_ = node_->prev;
            return *this;
        }

        self operator--(int){
            self temp = *this;
            --*this;
            return temp;
        }

        //重载比较操作符
        bool operator==(const self& rhs)const{
            return node_ == rhs.node_;
        }

        bool operator!=(const self& rhs)const{
            return node_ != rhs.node_;
        }
    };

    template<class T>
    struct list_const_iterator : public iterator<bidirectional_iterator_tag,T>{
        using value_type = T;
        using pointer = const T*;
        using reference = const T&;
        using base_ptr = typename node_traits<T>::base_ptr;
        using node_ptr = typename node_traits<T>::node_ptr;
        using self = list_const_iterator<T>;

        base_ptr node_;

        list_const_iterator() = default;
        list_const_iterator(base_ptr x) : node_(x){}
        list_const_iterator(node_ptr x) : node_(x->as_base()){}
        list_const_iterator(const list_iterator<T>& rhs) : node_(rhs.node_){}
        list_const_iterator(const list_const_iterator& rhs) : node_(rhs.node_){}

        reference operator*()const{
            return node_->as_node()->value;
        }

        pointer operator->()const{
            return &(operator*());
        }

        self& operator++(){
            XINSTL_DEBUG(node_ != nullptr);
            node_ = node_->next;
            return *this;
        }

        self& operator--(){
            XINSTL_DEBUG(node_ != nullptr);
            node_ = node_->prev;
            return *this;
        }

        self operator--(int){
            self temp = *this;
            --*this;
            return temp;
        }

        //重载比较操作符
        bool operator==(const self& rhs)const{
            return node_ == rhs.node_;
        }

        bool operator!=(const self& rhs)const{
            return node_ != rhs.node_;
        }
    };

    //模板类list
    //模板参数T代表数据类型
    template<class T>
    class list{
    public:
        //list的嵌套类型定义
        using allocator_type = XinSTL::allocator<T>;
        using data_allocator = XinSTL::allocator<T>;
        using base_allocator = XinSTL::allocator<list_const_iterator<T>>;
        using node_allocator = XinSTL::allocator<list_node<T>>;

        using value_type = typename allocator_type::value_type;
        using pointer = typename allocator_type::pointer;
        using const_pointer = typename allocator_type::const_pointer;
        using reference = typename alllcator_type::reference;
        using const_reference = typename alllcator_type::const_reference;
        using size_type = typename allocator_type::size_type;
        using difference_type = typename allocator_type::difference_type;

        using iterator = list_iterator<T>;
        using const_iterator = list_const_iterator<T>;
        using reverse_iterator = XinSTL::reverse_iterator<iterator>;
        using const_reverse_iterator = XinSTL::reverse_iterator<const_iterator>;

        using base_ptr = typename node_traits<T>::base_ptr;
        using node_ptr = typename node_traits<T>::node_ptr;

        allocator_type get_allocator(){
            return node_allocator();
        }

    private:
        base_ptr node_;//指向末尾节点
        size_type size_;//大小

    public:
        //构造、复制、移动、析构函数
        list(){
            fill_init(0,value_type());
        }

        explicit list(size_type n){
            fill_init(n,value_type());
        }

        list(size_type n,const T& value){
            fill_init(n,value);
        }

        template<class Iterator,typename std::enable_if<XinSTL::is_input_iterator<Iterator>::value,int>::type = 0>
        list(Iterator first,Iterator last){
            copy_init(first,last);
        }

        list(std::initializer_list<T> ilist){
            copy_init(ilist.begin(),ilist.end());
        }

        list(const list& rhs){
            copy_init(ilist.begin(),ilist.end());
        }

        list(const list& rhs){
            copy_init(rhs.cbegin(),rhs.cend());
        }

        list(list&& rhs)noexcept : node_(rhs.node_),size_(rhs.size_){
            rhs.node_ = nullptr;
            rhs.size_ = 0;
        }

        list& operator=(const list& rhs){
            if(this != &rhs){
                assign(rhs.begin(),rhs.end());
            }
            return *this;
        }

        list& operator=(list&& rhs)noexcept{
            clear();
            splice(end(),rhs);
            return *this;
        }

        list& operator=(std::initializer_list<T> ilist){
            list temp(ilist.begin(),ilist.end());
            swap(temp);
            return *this;
        }

        ~list(){
            if(node_){
                clear();
                base_allocator::deallocate(node_);
                node_ = nullptr;
                size_ = 0;
            }
        }

    public:
        //迭代器相关操作
        iterator begin()noexcept{
            return node_->next;
        }
        const_iterator begin()const noexcept{
            return node_->next;
        }
        iterator end()noexcept{
            return node_;
        }
        const_iterator end()const noexcept{
            return node_;
        }


        reverse_iterator rbegin()noexcept{
            return reverse_iterator(end());
        }
        const_reverse_iterator rbegin()const noexcept{
            return reverse_iterator(end());
        }
        reverse_iterator rend()noexcept{
            return reverse_iterator(begin());
        }
        const_reverse_iterator rend()const noexcept{
            return reverse_iterator(begin());
        }

        const_iterator cbegin()const noexcept{
            return begin();
        }
        const_iterator cend()const noexcept{
            return end();
        }
        const_reverse_iterator crbegin()const noexcept{
            return rbegin();
        }
        const_reverse_iterator crend()const noexcept{
            return rend();
        }

        //容量相关操作
        bool empty()const noexcept{
            return node_->next = node_;
        }
        size_type size()const noexcept{
            return size_;
        }
        size_type max_size()const noexcept{
            return static_cast<size_type>(-1);
        }

        //访问元素相关操作
        reference front(){
            XINSTL_DEBUG(!empty());
            return *begin();
        }
        const_reference front()const{
            XINSTL_DEBUG(!empty());
            return *begin();
        }
        reference back(){
            XINSTL_DEBUG(!empty());
            return *(--end());
        }
        const_reference back()const{
            XINSTL_DEBUG(!empty());
            return *(--end());
        }

        
    };
}