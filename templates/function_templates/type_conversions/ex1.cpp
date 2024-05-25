/* Type conversions during type deduction
 * -automatic type conversions are limited
 * -when declaring call parameters by reference two arguments declared with template parameter T must match exactly.
 * -when declaring call parameters by value, only trivial conversions that decay are supported.
 *  -const and volatile are ignored
 *  -references convert to referenced type
 *  -raw arrays or functions convert to the corresponding pointer type
 *  -the decayed types must match
 */

#include <iostream>
#include <boost/type_index.hpp>

using namespace std;



template<typename T>
void func(T a, T b) 
{
    std::cout << "type of a: " << boost::typeindex::type_id_with_cvr<decltype(a)>().pretty_name() << "\n"
              << "type of b: " << boost::typeindex::type_id_with_cvr<decltype(b)>().pretty_name() << "\n";
}

template<typename T1, typename T2 = std::string>
void func2(T1 a, T2="") 
{
    std::cout << "type of a: " << boost::typeindex::type_id_with_cvr<T1>().pretty_name() << "\n"
              << "type of b: " << boost::typeindex::type_id_with_cvr<T2>().pretty_name() << "\n";
}

template<typename T>
void func3(T& a, T& b) 
{
    std::cout << "type of a: " << boost::typeindex::type_id_with_cvr<decltype(a)>().pretty_name() << "\n"
              << "type of b: " << boost::typeindex::type_id_with_cvr<decltype(b)>().pretty_name() << "\n";
}

template<typename T>
void func4(const T& a, const T& b) 
{
    std::cout << "type of a: " << boost::typeindex::type_id_with_cvr<decltype(a)>().pretty_name() << "\n"
              << "type of b: " << boost::typeindex::type_id_with_cvr<decltype(b)>().pretty_name() << "\n";
}


int main()
{

 
    int i {0};
    int const j {1};

    cout << "For pass-by-value arguments: \n";
    cout << "arguments: int and int const: \n"; 
    func(i,j);

    int& iref {i};
    cout << "\narguments: int and int&: \n"; 
    func(i,iref);

    int const& jref {j};
    cout << "\narguments: int& and int const&: \n"; 
    func(iref,jref);

    int p[1] {5};
    cout << "\narguments: int[1] and int&: \n"; 
    func(p,&iref);

    std::string world="world";
    //func("hello",world); //doesn't compile: "hello" is const char[6]
    cout << "\narguments: \"hello\"s and std::string: \n"; 
    func("hello"s,world); 

    cout << "\n\nFor pass-by-value arguments with default second argument of type std::string: \n";
    cout << "arguments: int and int: \n"; 
    func2(1,2);

    cout << "\narguments: int and none: \n"; 
    func2(1);

    int const& l {j};
    cout << "\n\nFor pass-by-reference arguments: \n";
    cout << "arguments: int const& and int const: \n"; 
    func3(l,j);

    int q[1] {1};
    cout << "\narguments: int[1] and int[1]: \n"; 
    func3(p,q);

    int* r = p;
    int* s = r;
    cout << "\narguments: int* and int*: \n"; 
    func3(r,s);

    //cout << "\narguments: \"hello\"s and std::string: \n"; 
    //func3("hello"s,world); //doesn't compile because cannot bind non-const l-value reference to r-value

    cout << "\narguments: \"hello\"s and std::string: \n"; 
    func4("hello"s,world); //compiles because can bind const l-value reference to r-value

}

