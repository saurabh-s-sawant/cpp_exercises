/* This code demonstrates a type-dispatch mechanism using variadic macros, templates, 
 * and enum-based scalar types. It allows dynamic selection of an example kernel 
 * implementations based on the `ScalarType` enum, which represents supported 
 * scalar data types such as `Float`, `Double`, and `Int`.
 */

#include <iostream>
#include <string>
#include <type_traits>

// supported scalar types by the code
enum class ScalarType { Float, Double, Int };

// Helper to convert ScalarType to string
std::string toString(ScalarType type) {
    switch (type) {
        case ScalarType::Float: return "Float";
        case ScalarType::Double: return "Double";
        case ScalarType::Int: return "Int";
        default: return "Unknown";
    }
}

// Helper to map ScalarType to actual C++ types
template <ScalarType T>
struct ScalarTypeToCPPType;

template <>
struct ScalarTypeToCPPType<ScalarType::Float> {
    using type = float;
};

template <>
struct ScalarTypeToCPPType<ScalarType::Double> {
    using type = double;
};

template <>
struct ScalarTypeToCPPType<ScalarType::Int> {
    using type = int;
};

// Macro for dispatching to specific implementations based on ScalarType
#define DISPATCH_CASE(enum_type, ...)                       \
    case enum_type: {                                       \
        using scalar_t = typename ScalarTypeToCPPType<enum_type>::type; \
        return __VA_ARGS__();                               \
    }

// Main dispatch macro
#define DISPATCH_SCALAR_TYPE(scalar_type, ...)               \
    switch (scalar_type) {                                   \
        DISPATCH_CASE(ScalarType::Float, __VA_ARGS__)        \
        DISPATCH_CASE(ScalarType::Double, __VA_ARGS__)       \
        DISPATCH_CASE(ScalarType::Int, __VA_ARGS__)          \
        default:                                             \
            throw std::runtime_error("Unsupported ScalarType"); \
    }

// Example kernel functions
template <typename T>
void exampleKernel(ScalarType scalarType) {
    // Use toString to print a human-readable name for ScalarType
    std::cout << "Running kernel for type: " << toString(scalarType)
              << " (C++ type: " << typeid(T).name() << ")\n";
}

// Wrapper to use dispatch
void runKernel(ScalarType scalarType) {
    DISPATCH_SCALAR_TYPE(scalarType, [&]() {
        exampleKernel<scalar_t>(scalarType);
    });
}

int main() {
    runKernel(ScalarType::Float);  
    runKernel(ScalarType::Double); 
    runKernel(ScalarType::Int);    
    return 0;
}
