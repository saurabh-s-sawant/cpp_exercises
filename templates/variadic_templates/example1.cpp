/* Simple print() with variadic templates.
 * - use of sizeof... operator.
 * - difference in using run-time if and compile-time if with sizeof...
 */

#include<iostream>


// Example 1
/* This non-template overload of print function is needed 
 * to end the recursion of the variadic print function
 */
void print() {}

/* typename...  Type is a template parameter pack 
 * Types... args is a function parameter pack 
 */
template<typename T, typename... Types>
void print(T firstarg, Types... args) {

    std::cout << firstarg;

    /* when args parameter pack is empty, print() is called */
    print(args...);
}


// Example 2
void print_runtimeif() {}

template<typename T, typename... Types>
void print_runtimeif(T firstarg, Types... args) {

    std::cout << firstarg;

    /* Run-time if:
     * Here we still need print_runtimeif() to terminate the recursion.
     * If it is not declared, then we get compilation error.
     * This is because both branches of all if statements are instantiated,
     * i.e. even if sizeof... (args) == 0, print_runtimeif() will be instantiated.
     */
    if (sizeof... (args) > 0) {
        print_runtimeif(args...);
    }
}


// Example 3

template<typename T, typename... Types>
void print_compiletimeif(T firstarg, Types... args) {

    std::cout << firstarg;

    /* Compile-time if:
     * This behaves as expected without print_compiletimeif()
     */
    if constexpr(sizeof... (args) > 0) {
        print_compiletimeif(args...);
    }
}

int main() {

    print("ex1 (without if): 22/7=", 22/7., 
          " is a good approximtion for Pi, while 355/113=", 355/113., 
          " is even better!\n");

    print_runtimeif("ex2 (run-time if): 22/7=", 22/7., 
                    " is a good approximtion for Pi, while 355/113=", 355/113., 
                    " is even better!\n");

    print_compiletimeif("ex3 (compile-time if): 22/7=", 22/7., 
                        " is a good approximtion for Pi, while 355/113=", 355/113., 
                        " is even better!\n");

}
