
#ifndef LATTICE_HPP_
#define LATTICE_HPP_

#include <array>
#include <cassert>



namespace Sim{

  const short HEIGHT = 16;
  const short WIDTH = 32;

  template <typename SpinType= int>
  class Lattice {
    public:
      typedef SpinType spin_t;
      spin_t& operator()(std::size_t row ,std::size_t col) { //row major
        assert(row < Sim::HEIGHT && col < Sim::WIDTH);
        return data_[row*Sim::WIDTH + col];
      }

      //function which returns a const copy of the spin at the specified position
      spin_t operator()(std::size_t row ,std::size_t col) const { //row major
        assert(row < Sim::HEIGHT && col < Sim::WIDTH);
        return data_[row*Sim::WIDTH + col];
      }

      template<typename UnaryFunction>
      void apply(UnaryFunction uf) {
        for (spin_t& spin : data_) {
          uf(spin);
        }
      }

    private:
      std::array<spin_t,Sim::HEIGHT*Sim::WIDTH> data_; //Store in row-major

  };

} //end namespace
#endif
