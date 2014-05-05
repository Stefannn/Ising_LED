#include <iostream>
#include <cassert>
#include "Simulation.hpp"

//one test function per class
//

namespace Testing {


 template<typename S>
 struct nine_setter {
    void operator()(S &s) {s = 9;}
  };

 class TestModel {
  public:
  TestModel () {std::cout << "TestModel default constructor\n";}
  void update_model(Sim::Lattice<> & l) {std::cout << "testmodel update_model\n";}
  };

  class TestView {
  public:
  TestView(){std::cout << "Testview default const\n";}
  void update_view(const Sim::Lattice<> & l){std::cout <<"testview update_view\n";}
  };


template <typename Model,typename View>
void SimulationTest() {

  //Check the get_latticesite function
  //Make sure it returns by value and you can't change the lattice via a returned reference
  Sim::Simulation<Model,View> sim;
  nine_setter<typename Sim::Simulation<Model,View>::spin_t> ns;
  sim.init_lattice(ns);
  assert(sim.get_latticesite(3,0) == 9 && "get_latticsite failed");
  typename Sim::Simulation<Model,View>::spin_t xx = sim.get_latticesite(0,0);
  xx = 0;
  assert(sim.get_latticesite(0,0) == 9 && "Sim returned a reference instead of a copy");


}

void LatticeTest() {
  typedef int spintype;
  Sim::Lattice<spintype> lattice;

  //test the apply method
  nine_setter<int> ns;
  lattice.apply(ns);
  assert(lattice(0,0) == 9 && "Lattice apply failed");


  //test the operator() method
  lattice(Sim::HEIGHT-1,Sim::WIDTH-1) = 4;
  assert(lattice(Sim::HEIGHT-1,Sim::WIDTH-1) == 4 && "Setting of spin failed");

}

} //end namespace testing


int main(int argc, char **argv) {
  Testing::LatticeTest();
  Testing::SimulationTest<Testing::TestModel,Testing::TestView>();

  return 0;
}
