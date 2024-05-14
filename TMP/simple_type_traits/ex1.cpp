/* Simplest metaprogram demonstration:
 * 1) We create a template print function that takes an argument to be printed by value, 
 * where the argument can be a pointer. If it is a pointer then it is dereferenced 
 * while printing, instead of printing the address of the argument. 
 * Whether the argument is a pointer is determined at compile-time. 
 * This is demonstrated using an implementation of is_pointer, similar to 
 * std::is_pointer class template.
 *
 * 2) Next, if the argument is a floating type, then we want it to be printed with
 * 6 decimal precision. Again the floating type passed may be a pointer. 
 * We can check whether T is a floating type using std::is_floating_point<T>::value.
 * However, if T is a pointer then we need to obtain T_without_pointer from T.
 * This is done using strip_pointer class template.
 **/

#include <iostream>
#include <cmath>
#include <iomanip>
#include <type_traits>

template<typename T>
struct is_pointer 
{
    /* We implement our own is_pointer similar to std::is_pointer.
     * static: so that we can use without making an instance of is_pointer.
     * constexpr: so that we can use it at compile-time.
     * default value: false
     **/
    static constexpr bool value = false;
};

template<typename T>
struct is_pointer<T*> 
{
    /* Here we create a partial specialization of is_pointer
     * and define value to be true.
     **/
    static constexpr bool value = true;
};

template<typename T>
struct strip_pointer 
{
    /* type is a member type alias set to T.
     **/
    using type = T;
};

template<typename T>
struct strip_pointer<T*> 
{
    /* Again a partial specialization of strip_pointer class template,
     * where type is set to T.
     **/
    using type = T;
};

template<typename T>
void print1(T t) 
{
    /* We could have used std::is_pointer<T>::value below,
     * but we have implemented our own is_pointer.
     **/
    if constexpr(is_pointer<T>::value) 
    {
        /* Here we first need to strip off pointer from T* to obtain T_without_pointer,
         * before using it in std::is_floating_point<T>::value.
         **/
        using T_without_pointer = typename strip_pointer<T>::type;
        if constexpr(std::is_floating_point<T_without_pointer>::value) 
        {
            std::cout << std::fixed << std::setprecision(6) << *t;
        } 
        else 
        {
            std::cout << *t;
        }
    }
    else 
    {
        /* Here we know that T is not a pointer so we simply use 
         * std::is_floating_point<T>::value to determine whether it is 
         * a floating type.
         **/
        if constexpr(std::is_floating_point<T>::value) 
        {
            std::cout << std::fixed << std::setprecision(6) << t;
        }
        else { 
            std::cout << t;
        }
    }
}

template<typename A, typename B>
void print2(A a, B b) 
{
    print1(a);
    std::cout << ", ";
    print1(b);
    std::cout << "\n";
}

int main()
{
    print2(1, 2);
    std::string hi = "hi";
    
    /* 1) Both calls below print "hi, 2",
     * although in the second call, we pass a pointer to hi
     **/
    print2(hi, 2); 
    print2(&hi, 1);

    /* 2) Both calls below print "hi, 3.141593",
     * although in the second call, we pass pointer to float.
     **/
    double b = M_PI;
    print2(hi, b);
    print2(&hi, &b);
}
