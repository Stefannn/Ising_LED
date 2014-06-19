
#ifndef LATTICE_HPP_
#define LATTICE_HPP_

#include <array>
#include <cassert>



namespace Sim{


  template <typename SpinType= int>
  class Lattice {
    public:

      const static int HEIGHT = 16;
      const static int WIDTH  = 32;

      typedef SpinType spin_t;

      inline spin_t& operator()(std::size_t row ,std::size_t col) { //row major
        assert(row < HEIGHT && col < WIDTH);
        return data_[row*WIDTH + col];
      }

      //function which returns a const copy of the spin at the specified position
      inline spin_t operator()(std::size_t row ,std::size_t col) const { //row major
        assert(row < HEIGHT && col < WIDTH);
        return data_[row*WIDTH + col];
      }

      template<typename UnaryFunction>
      void apply(UnaryFunction uf){
        for (spin_t& spin : data_) {
          uf(spin);
        }
      }

    private:
      std::array<spin_t,HEIGHT*WIDTH> data_; //Store in row-major

  };



} //end namespace
#endif
