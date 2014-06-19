//Code for LED-acces (subfolder LED) from:
//https://github.com/hzeller/rpi-rgb-led-matrix

// June 14



#include <iostream>
#include <chrono>
#include <functional>

#include "Simulation.hpp"
#include "AsciiViewer.hpp"
#include "LEDViewPolicy.hpp"
#include "IsingPolicy.hpp"
#include "Lattice.hpp"
#include "LED/gpio.h"
#include "LED/led-matrix.h"


////////////////////////////////////SIGINT handling...///////////////////////////
std::function<void(void)> SIGINT_handler;

void SIGINT_callback(int sig) {
  std::cout << "Programm was aborted using SIGINT, clearing the LED-matrix\n";
  SIGINT_handler();
}

////////////////////////////////////////////////////////////////////////////////


// Runs an Ising Model for nSeconds at specified invT
// Using a uniform -1 coupling
int runIsingSimulation(int nSeconds,double invT) {
	
  std::cout << "Running an Ising Model with coupling -1 at invT= " << invT
            << " for " << nSeconds << " seconds\n";
  std::cout << "Performs ~185 steps per second (limited by the LEDs)\n";
  
  
  //Setup simulation
  typedef Sim::UniformCoupling<-1> unif1;
  typedef Sim::Simulation<Sim::IsingPolicy<unif1>,Sim::LEDViewPolicy> IsingLED;
  IsingLED s;
  s.init_lattice([=](int &x) {x=1;});
  s.set_invtemperature(invT);
  
  //create signal handling in case of a ctrl-c
  // => clear the LED
  struct sigaction new_action,old_action;
  SIGINT_handler = s.return_sigHandler(); //free function... needed for sigaction api....
  new_action.sa_handler = SIGINT_callback;
  sigaction(SIGINT,&new_action,&old_action);
  
  
  std::cout << "Thermalization period for 1e6 steps (no visualization)\n";
  s.thermalize();
  
  
  //Run it for nSeconds
  std::cout << "Starting visualization\n";
  std::chrono::steady_clock::time_point end = std::chrono::steady_clock::now() + std::chrono::seconds(nSeconds);
  int counter = 0;
  while (std::chrono::steady_clock::now() < end) {
    s.update();
    ++counter;
  }
  return counter;
}


int main(int argc, char ** argv) {

  
  if (argc < 3) {
    std::cout << "Usage: sudo ./isingLED RuntimeInSeconds(int) InverseTemperature(float)\n" 
              << "Choose 0 < invT < 1.5\n";
    return 1;
  } 
  int nSeconds = std::atoi(argv[1]);
  double invT = std::atof(argv[2]);
  
  
  // Start Simulation
	int nSteps = runIsingSimulation(nSeconds,invT);
  std::cout << "Performed " << nSteps << " steps in " << nSeconds << " seconds.\n\n";
  

  return 0;
}
