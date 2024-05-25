/* How to declare a template entity--an ordinary function instantiated with the class template?
 * -Approach 1: implicitly declare a new function template as friend with a different template parameter
 * -Approach 2: 
 *  -forward declare a function template for the class (so forward declare template class as well)
 *  -inside the class, declare a specialization of nonmember function template as friend
 *
 *  Here Approach 1 is described.
 */

#include <iostream>
#include <cassert>
#include <vector>

using namespace std;

template<typename T>
class Stack
{
private:
    std::vector<T> m_elems;

public:
    void push(T const& elem);
    T pop();

    void Print(std::ostream& strm) const 
    {
        for(T const& elem: m_elems) 
        {
            strm << elem << " ";
        }    
    }

    /* Approach 1 implementation */
    template<typename U>
        friend std::ostream& operator<< (std::ostream& strm, Stack<U> const& s);

};

template<typename T>
void
Stack<T>::push(T const& elem) 
{
    m_elems.push_back(elem);
}

template<typename T>
T 
Stack<T>::pop() 
{
    assert(!m_elems.empty()); // "Can't pop elements of an empty container!");
    T elem = m_elems.back();
    m_elems.pop_back();
    return elem;
}

template<typename T>
std::ostream& operator<< (std::ostream& strm, Stack<T> const& s) 
{
    s.Print(strm);
    return strm;
}

int main()
{
    Stack<int> a;

    a.push(10);
    a.push(20);
    a.push(30);

    cout << "a: " << a << "\n";
    cout << "a.pop(): " << a.pop() << "\n";
    cout << "a: " << a << "\n";
}
