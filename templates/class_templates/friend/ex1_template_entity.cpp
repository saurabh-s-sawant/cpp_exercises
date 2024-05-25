/* Here we define a template entity to print the elements of our class.
 * Template entity is an ordinary function instantiated with the class template, if needed.
 * Below we overload operator << as friend, which is a nonmember function.
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

    friend std::ostream& operator<< (std::ostream& strm, Stack<T> const& s) 
    {
    /* This function is not a template function, 
     * but an ordinary function instantiated with class template if needed,
     * also known as template entity.
     * If we were to declare this, how to do that?--see two approaches in ex2, ex3.
     */
        s.Print(strm);
        return strm;
    }
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
