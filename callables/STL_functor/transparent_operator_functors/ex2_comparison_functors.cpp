/** STL functors: transparent operator functors
 *C++ also provides comaprison functors: equal_to, not_equal_to, less, greater, less_equal, greater_equal.
 */

#include <queue>
#include <set>
#include <string>
#include <iostream>

using namespace std;

void print_queue(auto& q) 
{
    while(!q.empty()) {
        cout << q.top() << " ";
        q.pop();
    }
    cout << "\n";
}


int main()
{

    priority_queue<int> q; /*default implementation with lesser*/

    q.push(5);
    q.push(7);
    q.push(2);

    print_queue(q);

    priority_queue<int, vector<int>, greater<>> q_greater;

    q_greater.push(5);
    q_greater.push(7);
    q_greater.push(2);

    print_queue(q_greater);

    /*heterogeneous lookups*/
    //set<string,less<>> my_set({"a","qr","da"}); //this is more efficient than set<string> s;
                                                  //no string is constructed and no memory is allocated
    //auto s = my_set.find("a"); 
    //cout << "key is found: " << *s << "\n";
}
