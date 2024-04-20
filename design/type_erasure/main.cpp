#include "materials/materials.H"
#include "algorithm_impl/algorithm.H"

#include <vector>

int main() {
    using Materials = std::vector<Algorithm>;

    Materials materials;

    materials.emplace_back( CNT{4} );
    materials.emplace_back( Graphene{2} );

    computeAlgorithm(materials);
}
