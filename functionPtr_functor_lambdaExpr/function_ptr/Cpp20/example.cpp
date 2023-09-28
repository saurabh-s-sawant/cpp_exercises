/** Function Pointers, Function Objects, Lambda Expressions
 *
 * Functions are so-called `first-class' functions -> can be treated as normal variables,
 * i.e. passed as argument to another function, returned from a function, assigned to a variable.
 * 
 * callbacks: something that can be called, e.g. function pointer, an object with an overloaded operator (), 
 * or an inline lambda expression
 *
 * functor or function object: it's a class that can overload an operator ()
 *
 * Function pointers: 
 * -can be created using type aliases
 * -typed according to parameter type and return type of compatible functions
 *
 * higher-order functions: functions that accept other functions as parameter 
 */

import <vector>;
import <span>;
import <iostream>;
//import <format>;
import <functional>;

using namespace std;
using Compare_f = bool (*) (int, int);
using TruthHandler_f = void (*) (size_t, int, int);

void compare_and_handle(span<const int> vecA, span<const int> vecB,
        Compare_f compare_f, TruthHandler_f truthHandler_f) 
{
    if(vecA.size() != vecB.size()) { return; } 

    for(size_t i {0}; i < vecA.size(); ++i) 
    {
        if( compare_f(vecA[i], vecB[i]) ) 
        {
            truthHandler_f(i, vecA[i], vecB[i]);
        }
    }
}

bool is_divisible(int A, int B) { return A % B == 0; }

void print_divisibles(size_t index, int A, int B) 
{ 
//    cout << format("At index {}, {} is divisible by {}", index, A, B) << "\n";
    cout << "printing index: " << index << ", A: " << A << ", B:" << B << "\n";
}

int main()
{
    vector vecA{ 1, 4, 5, 4342, 256, 151, 235, 64687 };
    vector vecB{ 2, 3, 6, 7, 24, 2, 5, 6 };

    compare_and_handle(vecA, vecB, is_divisible, print_divisibles);
}
