/** std::function defined in <functional>
 * -used to create a type that can point to anything that is callable, i.e. a function pointer,
 *  function object, or a lambda expression
 * 
 * An instance of std::function can be 
 * -used as function pointer or a parameter of a function to implement callbacks
 * -stored, copied, moved, executed
 *
 * std::function<R(ArgTypes...)>
 * R: return type
 * ArgTypes: comma separated list of parameter types for a function
 */
#include <vector>
#include <iostream>
#include <functional>
using namespace std;

using Compare_f = function<bool(int, int)>;
//same as: using Compare_f = bool (*) (int, int);
using TruthHandler_f = function<void (size_t, int, int)>;

void compare_and_handle(const vector<int>& vecA, const vector<int>& vecB,
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
    cout << "At index: " << index << ", " << A << " is divisible by " << B << "\n";
}

int main()
{
    vector vecA{ 1, 4, 5, 4342, 256, 151, 235, 64687 };
    vector vecB{ 2, 3, 6, 7, 24, 2, 5, 6 };

    compare_and_handle(vecA, vecB, is_divisible, print_divisibles);

    //another example
    function<void(size_t,int,int)> fp {print_divisibles};
    fp(6,235,5);

    //if use do auto fp {print_divisibles}; 
    //then the type of tp is void (*fp) (size_t, int, int) instead of std::function
}
