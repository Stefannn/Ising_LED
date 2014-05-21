#include <iostream>
#include "Simulation.hpp"
#include "AsciiViewer.hpp"
#include "Lattice.hpp"

struct tmp {

  tmp() :cnt_(0) {;}

  template<class SpinType>
  void update_model(Sim::Lattice<SpinType>& l) {l(cnt_++,0) = cnt_;
  }
  private:
    int cnt_;
      };

int main(int argc, char ** argv) {
  std::cout << "main called\n";
  Sim::Simulation<tmp,Sim::AsciiViewer> s;
  s.init_lattice([=](int &x) {x=1;});
  for (int i = 0; i < 10;++i) {s.update();}

  return 0;
}
