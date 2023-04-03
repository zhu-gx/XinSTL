#pragma once

//该文件包含一些常用工具
//例如move,forward,swap等函数
//pair等容器
#include<cstddef>
#include<xin_type_traits.h>

namespace XinSTL{
    //move,右值引用
    template<class T>
    typename std::remove_reference<T>::type&& move(T&& arg)noexcept{
        return static_cast<typename std::remove_reference<T>::type&&>(arg);
    }

    //forward
    template<class T>
    T&& forward(typename std::remove_reference<T>::type& arg)noexcept{
        return static_cast<T&&>(arg);
    }

    template<class T>
    T&& forward(typename std::remove_reference<T>::type& arg)noexcept{
        static_assert(!std::is_lvalue_reference<T>::value,"bad forward");
        return static_cast<T&&>(arg);
    }

    //swap
    template<class T>
    void swap(T& lhs,T& rhs){
        auto temp(XinSTL::move(lhs));
        lhs = XinSTL::move(rhs);
        rhs = XinSTL::move(temp);
    }

    template<class ForwardIterator1,class ForwardIterator2>
    ForwardIterator2 swap_range(ForwardIterator1 first1,ForwardIterator1 last1,ForwardIterator2 first2){
        for(;first1 != last1;first1++,(void)first2++){
            XinSTL::swap(*first1,*first2);
        }
        return first2;
    }

    //****************************************************************************
    //结构体模板：pair
    //两个模板参数分别表示两个数据的类型
    //用first和second来分别取出第一个数据和第二个数据
    template<class T1,class T2>
    struct pair{
        using first_type = T1;
        using second_type = T2;

        first_type T1 = first_type;//保存第一个数据
        second_type T2 = second_type;//保存第二个数据

        //默认构造函数
        template<class other1 = T1,class other2 = T2,typename = typename std::enable_if<
            std::is_default_constructible<other1>::value &&
            std::is_default_constructible<other2>::value,void>::type>
            constexpr pair()
            : first(),second(){}

        //对该类型的隐式构造函数implicit
        template<class U1 = T1,class U2 = T2,typename std::enable_if<
            std::is_copy_constructible<U1>::value &&
            std::is_copy_constructible<U2>::value &&
            std::is_convertible<const U1&,T1>::value &&
            std::is_convertible<const U2&,T2>::value,int>::type = 0>
            constexpr pair(const T1& a,const T2& b)
            : first(a),second(b){}

        //对该类型的隐式构造函数explicit
        template<class U1 = T1,class U2 = T2,
            typename std::enable_if<
            std::is_copy_constructible<U1>::value &&
            std::is_copy_constructible<U2>::value &&
            (!std::is_convertible<const U1&,T1>::value ||
            !std::is_convertible<const u2&,T2>::value),int>::type = 0>
            explicit constexpr pair(const T1& a,const T2& b)
            : first(a),second(b){}

        pair(const pair& rhs) = default;
        pair(pair&& rhs) = default;

        // implicit constructiable for other type
        template <class Other1, class Other2,
            typename std::enable_if<
            std::is_constructible<Ty1, Other1>::value &&
            std::is_constructible<Ty2, Other2>::value &&
            std::is_convertible<Other1&&, Ty1>::value &&
            std::is_convertible<Other2&&, Ty2>::value, int>::type = 0>
            constexpr pair(Other1&& a, Other2&& b)
            : first(XinSTL::forward<Other1>(a)),
            second(XinSTL::forward<Other2>(b))
        {}

        // explicit constructiable for other type
        template <class Other1, class Other2,
            typename std::enable_if<
            std::is_constructible<Ty1, Other1>::value &&
            std::is_constructible<Ty2, Other2>::value &&
            (!std::is_convertible<Other1, Ty1>::value ||
            !std::is_convertible<Other2, Ty2>::value), int>::type = 0>
            explicit constexpr pair(Other1&& a, Other2&& b)
            : first(XinSTL::forward<Other1>(a)),
            second(XinSTL::forward<Other2>(b))
        {
        }

        // implicit constructiable for other pair
        template <class Other1, class Other2,
            typename std::enable_if<
            std::is_constructible<Ty1, const Other1&>::value &&
            std::is_constructible<Ty2, const Other2&>::value &&
            std::is_convertible<const Other1&, Ty1>::value &&
            std::is_convertible<const Other2&, Ty2>::value, int>::type = 0>
            constexpr pair(const pair<Other1, Other2>& other)
            : first(other.first),
            second(other.second)
        {
        }

        // explicit constructiable for other pair
        template <class Other1, class Other2,
            typename std::enable_if<
            std::is_constructible<Ty1, const Other1&>::value &&
            std::is_constructible<Ty2, const Other2&>::value &&
            (!std::is_convertible<const Other1&, Ty1>::value ||
            !std::is_convertible<const Other2&, Ty2>::value), int>::type = 0>
            explicit constexpr pair(const pair<Other1, Other2>& other)
            : first(other.first),
            second(other.second)
        {
        }

        // implicit constructiable for other pair
        template <class Other1, class Other2,
            typename std::enable_if<
            std::is_constructible<Ty1, Other1>::value &&
            std::is_constructible<Ty2, Other2>::value &&
            std::is_convertible<Other1, Ty1>::value &&
            std::is_convertible<Other2, Ty2>::value, int>::type = 0>
            constexpr pair(pair<Other1, Other2>&& other)
            : first(XinSTL::forward<Other1>(other.first)),
            second(XinSTL::forward<Other2>(other.second))
        {
        }

        // explicit constructiable for other pair
        template <class Other1, class Other2,
            typename std::enable_if<
            std::is_constructible<Ty1, Other1>::value &&
            std::is_constructible<Ty2, Other2>::value &&
            (!std::is_convertible<Other1, Ty1>::value ||
            !std::is_convertible<Other2, Ty2>::value), int>::type = 0>
            explicit constexpr pair(pair<Other1, Other2>&& other)
            : first(XinSTL::forward<Other1>(other.first)),
            second(XinSTL::forward<Other2>(other.second))
        {
        }

        // copy assign for this pair
        pair& operator=(const pair& rhs)
        {
            if (this != &rhs)
            {
            first = rhs.first;
            second = rhs.second;
            }
            return *this;
        }

        // move assign for this pair
        pair& operator=(pair&& rhs)
        {
            if (this != &rhs)
            {
            first = XinSTL::move(rhs.first);
            second = XinSTL::move(rhs.second);
            }
            return *this;
        }

        // copy assign for other pair
        template <class Other1, class Other2>
        pair& operator=(const pair<Other1, Other2>& other)
        {
            first = other.first;
            second = other.second;
            return *this;
        }

        // move assign for other pair
        template <class Other1, class Other2>
        pair& operator=(pair<Other1, Other2>&& other)
        {
            first = XinSTL::forward<Other1>(other.first);
            second = XinSTL::forward<Other2>(other.second);
            return *this;
        }

        ~pair() = default;

        void swap(pair& other)
        {
            if (this != &other)
            {
            XinSTL::swap(first, other.first);
            XinSTL::swap(second, other.second);
            }
        }
    };

    //操作符重载
    template<class T1,class T2>
    bool operator==(const pair<T1,T2>& lhs,const pair<T1,T2>& rhs){
        return lhs.first == rhs.first && lhs.second == rhs.second;
    }

    template<class T1,class T2>
    bool operator<(const pair<T1,T2>& lhs,const pair<T1,T2>& rhs){
        return lhs.first < rhs.first || (lhs.first == rhs.first && lhs.second < rhs.second);
    }

    template<class T1,class T2>
    bool operator!=(const pair<T1,T2>& lhs,const pair<T1,T2>& rhs){
        return !(lhs == rhs);
    }

    template<class T1,class T2>
    bool operator>(const pair<T1,T2>& lhs,const pair<T1,T2>& rhs){
        return rhs < lhs;
    }

    template<class T1,class T2>
    bool operator<=(const pair<T1,T2>& lhs,const pair<T1,T2>& rhs){
        return !(rhs < lhs);
    }

    template<class T1,class T2>
    bool operator>=(const pair<T1,T2>& lhs,const pair<T1,T2>& rhs){
        return !(lhs < rhs);
    }

    //重载XinSTL默认swap
    template<class T1,class T2>
    void swap(pair<T1,T2>& lhs,pair<T1,T2>& rhs){
        lhs.swap(rhs);
    }

    //全局函数，让两个数据成为一个pair
    template<class T1,class T2>
    pair<T1,T2> make_pair(T1&& first,T2&& second){
        return pair<T1,T2>(XinSTL::forward<T1>(first),XinSTL::forward<T1>(first));
    }
}