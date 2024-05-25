/* How to declare a template entity--an ordinary function instantiated with the class template?
 * -Approach 1: implicitly declare a new function template as friend with a different template parameter
 * -Approach 2: 
 *  -forward declare a function template for the class (so forward declare template class as well)
 *  -inside the class, declare a specialization of nonmember function template as friend
 *
 *  Here Approach 2 is described.
 */

#include <iostream>
#include <cassert>
#include <vector>

using namespace std;

/*forward declaration*/
template<typename T>
class Stack;

template<typename T>
std::ostream& operator << (std::ostream& strm, Stack<T> const& s);


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

    /* Declare a specialization of nonmember function template as a friend
     * Note <T> after operator <<.
     * Without this <T>, we would have declared a new nontemplate function!
     */
    friend std::ostream& operator<< <T> (std::ostream& strm, Stack<T> const& s);

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
/* This function is not a template function, 
 * but an ordinary function instantiated with class template if needed,
 * also known as template entity.
 * If we were to declare this, how to do that?--see two approaches in ex2, ex3.
 */
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
