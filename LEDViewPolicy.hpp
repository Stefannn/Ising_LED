// May 14


#ifndef LEDVIEWPOLICY_HPP_
#define LEDVIEWPOLICY_HPP_



#include "LED/gpio.h"
#include "LED/led-matrix.h"
#include "Lattice.hpp" //for the WIDTH & HEIGHT params


#include <cstdlib>
#include <memory>
#include <thread>
#include <iostream>
#include <chrono>
#include <csignal>
#include <functional>

namespace Sim{
	class LEDViewPolicy {
	
	public:
		LEDViewPolicy() { 
			
			//check gpio before creating a new RGBMatrix with it
			if (!io.Init()) {
				std::cerr << "couldnt initialize gpio, calling std::exit(1) \n";
        std::exit(1);
			}
			matrix_ = std::unique_ptr<RGBMatrix>(new RGBMatrix(&io));
      
		}
		
    
		template<typename SpinType>
		void update_view(const Sim::Lattice<SpinType>& lattice) {
      for (int r = 0;r < matrix_->height();++r) {
        for (int c = 0;c < matrix_->width();++c) {
          if (lattice(r,c) > 0) {
            matrix_->SetPixel(c,r,255,255,0);
          } else {
            matrix_->SetPixel(c,r,0,0,255);
          }
          
        }
        
      }
      matrix_->UpdateScreen();
      
		}
    
    //Clears the LED-panel
    void OnSIGINT() {
      std::cout << "Cleaning Up...\n\n";
      matrix_->ClearScreen();
      matrix_->UpdateScreen();     
			std::_Exit(EXIT_FAILURE);
    }
  
    ~LEDViewPolicy() {
      matrix_->ClearScreen();
      matrix_->UpdateScreen();
    }
    
    std::function<void()> return_sigHandler() {
      return [this]() {this->OnSIGINT();};
    }
	
	private:
		GPIO io;
    std::unique_ptr<RGBMatrix> matrix_; //somehow needs to be a shared_ptr for std::bind to work in main.cpp
    
    
    
	};
		

		
} // end namespace Sim

#endif  //header guards
