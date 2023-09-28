/** STL functors: transparent operator functors
 * C++20 provides support for transparent operators for unordered associative containers such as 
 * unordered_map and unordered_set, however, implementation requires one to specify a custom hash
 * functor containing is_transparent type defined as void.
 */

#include <unordered_set>
#include <string>
#include <iostream>

using namespace std;

class Hasher
{
    public:
    using is_transparent = void;
    size_t operator() (string_view sv) const {return hash<string_view>{}(sv); }
};

int main()
{

    unordered_set<string, Hasher, equal_to<>> my_set({"Value","Key","Random"});
    auto a {my_set.find("Key")}; //no string is constructed and no memory is allocated
                                 //same goes for a{my_set.find("Key"sv)};
    cout << *a << "\n";                                 
}
