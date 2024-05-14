#include "algorithm.H"

void computeAlgorithm(std::vector<Algorithm> const& materials) {
    for(auto const& material: materials) {
        computeStep1(material);
    }
}

void computeStep1(Algorithm const& material) {
    material.pimpl->computeStep1();
}
