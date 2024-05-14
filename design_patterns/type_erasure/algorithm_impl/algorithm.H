#pragma once

/* Author: Saurabh S. Sawant
 *
 * An example of a type erasure.
 *
 * A type erasure contains:
 * - a templated constructor
 * - a non-virtual interface
 * - combination of three design patterns:
 *   - External polymorphism
 *   - Bridge
 *   - Prototype
 *
 * see blog: 
 */

#include <memory>
#include <vector>
#include "../materials/materials.H"
#include "materials_impl.H"

class Algorithm {
private:
    /* External polymorphism design pattern: allows C++ classes
     * unrelated by inheritance and/or having no virtual methods to be
     * created polymorphically.
     * We create a interface, AlgorithmConcept, and 
     * a derived class template, AlgorithmModel.
     * Algorithm model has a constructor that takes some T and uses it.
     */
    struct AlgorithmConcept {
        virtual ~AlgorithmConcept() {}
        
        virtual void computeStep1() const = 0;
        
        /* Prototype design pattern: clone function will return a copy of
         * whatever stored in the derived class in the form of a unique_ptr
         * to AlgorithmConcept.
         */
        virtual std::unique_ptr<AlgorithmConcept> clone () const = 0;       
    };

    template<typename T>
    struct AlgorithmModel : public AlgorithmConcept {
        AlgorithmModel(T&& args) : object{std::forward<T>(args)} {}
        
        std::unique_ptr<AlgorithmConcept> clone() const override {
            return std::make_unique<AlgorithmModel>(*this);
        }

        void computeStep1() const override {
            ::computeStep1(object); // Affordances required by type T
        }
        
        T object;
    };

    /* friend functions */
    friend void computeStep1(Algorithm const& material);

    std::unique_ptr<AlgorithmConcept> pimpl = nullptr;

public:
    /* This templated constructor acts as a bridge:
     * It instantiates algorithm model for whatever T object you pass
     * and we store it as pointer to base so we have erased the type!!
     * We can have infinite number of derived classes but we dont have to
     * write them, the compiler will generate them for us.
     */
    template<typename T>
    Algorithm(T&& args) : pimpl{ std::make_unique<AlgorithmModel<T>>(std::forward<T>(args)) } {}

    /* How to copy an object when we have erased the type of the object? 
     * Using clone functions! Prototype design pattern.
     */
    Algorithm(Algorithm const& that) {
        if (that.pimpl) {
            pimpl = that.pimpl->clone();
        } else {
            pimpl.reset(); // Reset pimpl to nullptr if 'that' has no pimpl
        }
    }
    Algorithm& operator=(Algorithm const& that) noexcept {
        if (this != &that) {
            if(that.pimpl) {
                auto temp = that.pimpl->clone();
                std::swap(pimpl, temp);
                temp.reset();
            }
        }
        return *this;
    }

    /* Move constructor is straighforward to implement. */
    Algorithm(Algorithm&& that) = default;
    Algorithm& operator=(Algorithm&& that) noexcept;
};

void computeAlgorithm(std::vector<Algorithm> const& materials);
