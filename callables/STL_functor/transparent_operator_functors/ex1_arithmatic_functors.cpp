/** STL functors: transparent operator functors
 *C++ provides functor class templates for aithmatic operators: plus, minus, multiplies, divides, modulus, unary_negate
 *Benefit of arithmatic function objects over operators: you can pass them as callbacks to other functions
 */

#include <vector>
#include <iostream>

using namespace std;

template<typename Iter, typename InitVal, typename Operation> 
auto operateOnData(Iter begin, Iter end, InitVal init_val, Operation op) 
{
    /*Here op is a callback*/

    auto result {init_val};

    for(Iter iter {begin}; iter != end; ++iter) 
    {
        result = op(result, *iter);    
    }
    return result;
}

int main()
{
    vector vec{ 1, 2, 3, 4, 5};

    double result { operateOnData(cbegin(vec), cend(vec), 0.5, multiplies<>{} ) };
    
    /*same as using multiplies<double>{}, where as multiplies<int>{} gives 0*/
    /*recommended to always use transparent operator functors*/

    cout << result << "\n"; //ans should be 60 = 0.5*1*2*3*4*5
}
