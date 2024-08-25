/* 
 * Write a function to check if an array or a vector is a 2D array.
 **/
#include <iostream>
#include <vector>
#include <array>
#include <type_traits>

using namespace std;

template<typename T>
struct is_2D_array: std::false_type{};

template<typename T, std::size_t N, std::size_t M>
struct is_2D_array<std::array<std::array<T,N>,M>> : std::true_type{};

template<typename T, std::size_t N, std::size_t M>
struct is_2D_array<T[N][M]> : std::true_type{};


template <typename T>
int check_if_2D_array() {
    if constexpr (is_2D_array<T>::value) {
        return true;
    }
    return false;
}

template<typename T>
struct is_2D_vector : std::false_type{};

template<typename T, typename Alloc>
struct is_2D_vector<std::vector<std::vector<T>, Alloc>> : std::true_type{};


template <typename T>
int check_if_2D_vector() {
    if constexpr (is_2D_vector<T>::value) {
        return true;
    }
    return false;
}

int main() {
    std::array<std::array<int,4>,3> arr2D = {{{1,2,3,4},{5,6,7,8},{9,10,11,12}}};
    std::array<int,5> arr1D = {1,2,3,4,5};

    int carr2D[4][3];
    int carr1D[5];
    
    std::vector<std::vector<int>> vec2D = {{1,2,3},{4,5,6}};
    std::vector<int> vec1D = {1,2,3,4,5};

    std::cout << "is arr2D a 2D array? : "  << check_if_2D_array<decltype(arr2D)>() << "\n";
    std::cout << "is arr1D a 2D array? : "  << check_if_2D_array<decltype(arr1D)>() << "\n";
    
    std::cout << "is carr2D a 2D array? : " << check_if_2D_array<decltype(carr2D)>() << "\n";
    std::cout << "is carr1D a 2D array? : " << check_if_2D_array<decltype(carr1D)>() << "\n";

    std::cout << "is vec2D a 2D vector? : " << check_if_2D_vector<decltype(vec2D)>() << "\n";
    std::cout << "is vec1D a 2D vector? : " << check_if_2D_vector<decltype(vec1D)>() << "\n";

    return 0;
}
