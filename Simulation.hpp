//Stefan Hegglin, May 14
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
// - must have void update_model(Lattice) function

//Concepts required for VisualizationPolicy:
// - Constructor: default
// - must have void update_view(const Lattice) function
template <typename ModelPolicy, typename VisualizationPolicy>
class Simulation: public ModelPolicy, public VisualizationPolicy  {
  public:

    typedef int spin_t;

    Simulation() : x(10){
      std::cout << "Default-Constructor Called\n";
    }



    void update() {
      ModelPolicy::update_model(lattice_);
      VisualizationPolicy::update_view(lattice_);
    }

    template <typename UnaryFunction>
    void init_lattice(UnaryFunction uf) {
      lattice_.apply(uf);
    }

    spin_t get_latticesite(std::size_t row, std::size_t col) const {
      return lattice_(row,col);
    }


  private:
    Sim::Lattice<spin_t> lattice_;
    count_t x;
};





} //end namespace Sim
#endif //guards end
