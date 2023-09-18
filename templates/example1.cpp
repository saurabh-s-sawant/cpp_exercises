template<typename T>
void print(const T& coll) 
{
    for(const auto& elem : coll ) 
    {
        cout << elem << "\n";
    }
}

void print(const auto& coll) 
{
    for(const auto& elem : coll ) 
    {
        cout << elem << "\n";
    }
}

template<typename T>
concept SupportsLessThan = requires (T x) {x < x; };

template<typename T>
requires std::copyable<T> && SupportsLessThan<T>
T mymax (T a, T b)
{
    return b < a ? a : b;   
}

template<typename T1, typename T2>
auto mymax(T1 a, T2 b)
{
    return b < a ? a : b;
}
 

//conditional support

//std::array
template<typename T, size_t SZ>
struct array {
T elems[SZ];
};

//variadic templates

template<typename T, typename... Types>
void print(T firstArg, Types... args) {

    std::cout << firstArg << "\n";

    if constexpr(sizeof... (args) > 0) //if args is not empty
    {
        print(args... );
    }
}


