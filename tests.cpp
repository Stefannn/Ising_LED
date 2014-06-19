#include <iostream>
#include <cassert>
#include "Simulation.hpp"
#include "IsingPolicy.hpp"
#include "AsciiViewer.hpp"
#include "LEDViewPolicy.hpp"

#include "LED/gpio.h"
#include "LED/led-matrix.h"




namespace Testing {


 template<typename S>
 struct nine_setter {
    void operator()(S &s) {s = 9;}
  };
	
	template<typename S>
	struct one_setter {
		void operator() (S&s) {s=1;}
	};
	
 class TestModel {
  public:
  TestModel () {std::cout << "TestModel default constructor\n";}
  void update_model(Sim::Lattice<> & l) {std::cout << "testmodel update_model\n";}
  };

  class TestView {
  public:
  TestView(){std::cout << "Testview default const\n";}
  void update_view(const Sim::Lattice<> & l){/*std::cout <<"testview update_view\n";*/}
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
	for (int i = 0; i < 8; ++i) {sim.update();}

}

template<typename Model, typename View>
void IsingTest() {
	Sim::Simulation<Model,View> sim;
	one_setter<typename Sim::Simulation<Model,View>::spin_t> ones;
	sim.init_lattice(ones);
	for (int i = 0; i < 1000; ++i) {sim.update();std::cout << sim.get_energy() << "\n";
		if (sim.get_energy() < -1024) {
			std::cout << "energy lower than theoretical minimum, aborting\n";
			break;
		}
	}
}

// the critical temperature should be around 0.45, verify by plotting M vs T
template<typename Model, typename View>
void CriticalTemperatureTest() {
    Sim::Simulation<Model,View> sim;
    one_setter<typename Sim::Simulation<Model,View>::spin_t> ones;
    sim.init_lattice(ones);
    std::cout << "T, <M>\n";
    for (double T = 0.01;T <=1.3;T += 0.1) {
        double averageM = 0;
        int equilibrateSteps = 100000;
        sim.set_invtemperature(T);
        for (int i = 0; i < equilibrateSteps; ++i) {
            sim.update();
        }
        int nSteps = 1000000;
        for (int i = 0; i < nSteps; ++i) {
            sim.update();
            averageM += std::abs(sim.compute_Magnetization(sim.get_lattice())); //highly correlated...
        }
        std::cout << T << ", " << averageM /(nSteps) <<"\n";
    }
   

}


void LatticeTest() {
  typedef int spintype;
  Sim::Lattice<spintype> lattice;

  //test the apply method
  nine_setter<int> ns;
  lattice.apply(ns);
  assert(lattice(0,0) == 9 && "Lattice apply failed");


  //test the operator() method
  lattice(lattice.HEIGHT-1,lattice.WIDTH-1) = 4;
  assert(lattice(lattice.HEIGHT-1,lattice.WIDTH-1) == 4 && "Setting of spin failed");

}





//LED-test
void LEDTest() {
  GPIO io;
  if (!io.Init())
    std::cout << "io.Init failed\n";
  RGBMatrix m(&io);
  m.SetPixel(1,1,255,255,255);
  std::cout << " SetPixel called\n";
  m.UpdateScreen();
  std::cout << " UpdateScreen called\n";
  
  std::cin.ignore();
  m.UpdateScreen();
  std::cin.ignore();
  
  m.ClearScreen();
  m.UpdateScreen();
  std::cout << "io initialized\n";
}



} //end namespace testing


int main(int argc, char **argv) {
  //Testing::LatticeTest();
  //Testing::SimulationTest<Testing::TestModel,Testing::TestView>();
	typedef Sim::UniformCoupling<-1> unif4;
	//Testing::SimulationTest<Sim::IsingPolicy<unif4>,Sim::AsciiViewer>();
	//Testing::SimulationTest<Sim::IsingPolicy<unif4>,Testing::TestView>();
  //Testing::IsingTest<Sim::IsingPolicy<unif4>,Testing::TestView>();
	//Testing::CriticalTemperatureTest<Sim::IsingPolicy<unif4>,Sim::LEDViewPolicy>();
  Testing::CriticalTemperatureTest<Sim::IsingPolicy<unif4>,Testing::TestView>();
	

  return 0;
}
