/** Function objects or functors
 *
 * Benefit over a function: a functor keeps state between calls to itself.
 *  
 */

#include <vector>
#include <iostream>

using namespace std;

class c_IsDivisibleBy 
{
   public:
   explicit c_IsDivisibleBy(int arg) : m_val {arg} {}

   /*NOTE*/
   bool operator() (int val1, int val2) const  //const is not necessary for this example
   {                                           //but for many STL algorithms, operator must be const.
       return (val1%m_val == 0) && (val2%m_val == 0);
   }

   private:
   int m_val;
};

template<typename Compare_f, typename Print_f> 
void compare_and_handle(const vector<int>& vecA, const vector<int>& vecB, Compare_f compare_f, Print_f print_f) 
{
    if(vecA.size() != vecB.size()) { return; }

    for(size_t i {0}; i < vecA.size(); ++i)
    {
        if( compare_f(vecA[i], vecB[i]) )
        {
            print_f(i, vecA[i], vecB[i]);
        }
    }
}

void print_divisibles(size_t index, int A, int B)
{
    cout << "At index: " << index << ", " << A << " and " << B << " are divisible! " << "\n";

    /*(?) How to make this function print the divisor, m_val? */
}


int main()
{
    vector vecA{ 1, 4, 5, 4242, 256, 151, 235, 64687 };
    vector vecB{ 2, 3, 6, 35, 24, 2, 5, 6 };

    compare_and_handle(vecA, vecB, c_IsDivisibleBy {7}, print_divisibles);


}
