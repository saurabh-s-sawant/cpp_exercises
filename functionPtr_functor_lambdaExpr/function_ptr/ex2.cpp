/** example using templates instead of type aliases
 */

#include <vector>
#include <iostream>

using namespace std;

//type aliases
//using Compare_f = bool (*) (int, int);
//using TruthHandler_f = void (*) (size_t, int, int);

template<typename Compare_f, typename TruthHandler_f>
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
}
