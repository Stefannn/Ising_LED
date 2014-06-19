// May 14
//
#ifndef SIMULATION_HPP_
#define SIMULATION_HPP_


#include <iostream>
#include <array>
#include <algorithm>
#include <cassert>
#include "Lattice.hpp"

namespace Sim {

typedef double real_t;
typedef int count_t;


//Concepts required for ModelPolicy:
// - Constructor: default
// - must have void update_model(Lattice&) function
// - must have (convertible to double) compute_Energy(Lattice&) function

//Concepts required for VisualizationPolicy:
// - Constructor: default
// - must have void update_view(const Lattice&) function
template <typename ModelPolicy, typename VisualizationPolicy>
class Simulation: public ModelPolicy, public VisualizationPolicy  {
public:

    typedef int spin_t;

    Simulation() : x(10) {
        std::cout << "Default-Constructor Called\n";
    }


    void thermalize() {
        int thermalizationSteps = 1e6;
        for (int i = 0; i<thermalizationSteps; ++i) {
            ModelPolicy::update_model(lattice_);
        }
    }

    void update() {
        ModelPolicy::update_model(lattice_);
        VisualizationPolicy::update_view(lattice_);
    }

    double get_energy()  {
        return ModelPolicy::compute_Energy(lattice_);
    }

    template <typename UnaryFunction>
    void init_lattice(UnaryFunction uf) {
        lattice_.apply(uf);
    }

    spin_t get_latticesite(std::size_t row, std::size_t col) const {
        return lattice_(row,col);
    }

    void set_latticesite(std::size_t row, std::size_t col, spin_t val) {
        lattice_(row,col) = val;
    }

    const Sim::Lattice<spin_t>& get_lattice() {
        return lattice_;
    }


private:
    Sim::Lattice<spin_t> lattice_;
    count_t x;
};


} //end namespace Sim
#endif //guards end
