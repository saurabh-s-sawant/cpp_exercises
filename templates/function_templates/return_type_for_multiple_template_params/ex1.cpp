/* Return type in template functions
 * -return type can be a template parameter with an optional default value.
 * -return type can be deduced by compiler
 * -type trait common_type_t can be used
 */

#include <iostream>
#include <boost/type_index.hpp>
#include <type_traits>

using namespace std;


//template<typename T1, typename T2,typename RT=std::decay_t<decltype(true?T1():T2())>> //requires default constructors
template<typename T1, typename T2,typename RT=std::common_type_t<T1,T2>>
RT func1_RTasTemplateParam(T1 a, T2 b) 
{
    return a > b ? a : b;
}

template<typename T1, typename T2>
//auto func2_RTdeduced(T1 a, T2 b) -> typename std::decay<decltype(true?a:b)>::type //C++11 onward
//auto func2_RTdeduced(T1 a, T2 b) -> std::decay_t<decltype(true?a:b)> //C++14 onward
auto func2_RTdeduced(T1 a, T2 b) //C++17 onward
{
    return a > b ? a : b;
}

template<typename T1, typename T2>
std::common_type_t<T1,T2> func3_RTasCommonType(T1 a, T2 b) 
{
    return a > b ? a : b;
}


int main()
{
    unsigned a = 3;
    float b = 7;

    cout << "a: " << a << " type:" << boost::typeindex::type_id_with_cvr<decltype(a)>().pretty_name() << "\n";
    cout << "b: " << b << " type:" << boost::typeindex::type_id_with_cvr<decltype(b)>().pretty_name() << "\n";

    cout << "\nfunc1_RTasTemplateParam: \n"; 
    auto ans1a = func1_RTasTemplateParam<unsigned,float,double>(b,a);
    cout << "returned value (b>a): " << ans1a 
         << " type: " << boost::typeindex::type_id_with_cvr<decltype(ans1a)>().pretty_name() << "\n";

    auto ans1b = func1_RTasTemplateParam(a,b);
    cout << "returned value (a>b): " << ans1b 
         << " type: " << boost::typeindex::type_id_with_cvr<decltype(ans1b)>().pretty_name() << "\n";

    cout << "\nfunc2_RTdeduced: \n"; 
    auto ans2a = func2_RTdeduced(b,a);
    cout << "returned value (b>a): " << ans2a 
         << " type: " << boost::typeindex::type_id_with_cvr<decltype(ans2a)>().pretty_name() << "\n";

    auto ans2b = func2_RTdeduced(a,b);
    cout << "returned value (a>b): " << ans2b 
         << " type: " << boost::typeindex::type_id_with_cvr<decltype(ans2b)>().pretty_name() << "\n";

    cout << "\nfunc3_RTasCommonType: \n"; 
    auto ans3a = func3_RTasCommonType(b,a);
    cout << "returned value (b>a): " << ans3a 
         << " type: " << boost::typeindex::type_id_with_cvr<decltype(ans3a)>().pretty_name() << "\n";

    auto ans3b = func3_RTasCommonType(a,b);
    cout << "returned value (a>b): " << ans3b 
         << " type: " << boost::typeindex::type_id_with_cvr<decltype(ans3b)>().pretty_name() << "\n";
}

