/** Simple example using method pointer
 * 
 * pointer to a non-static data members or methods must be dereferenced in the context of an object.
 * pointer to a static data member or static method can be dereferenced without an object.
 *
 */

#include <iostream>

using namespace std;

class A 
{
public:

   A(int a) : m_val {a} {}

   int getVal() const 
   { return m_val; }

private:

   int m_val;
};

int main()
{
    A object{1};

    int (A::*method_ptr) () const { &A::getVal };

    //Another approach
    //using my_alias = int (A::*) () const;
    //auto method_ptr { &A::getVal };

    cout << "Dereferencing a method_ptr of in the context of object to obtain value: " 
         << (object.*method_ptr) () 
         << "\n";
    //paranthesis are needed around object.*method_ptr because () has higher precedence than *.
}
