//2d IsingModel
//May 14




#ifndef ISING_POLICY_
#define ISING_POLICY_



#include "Lattice.hpp" //for the WIDTH & HEIGHT params

#include <random>
#include <vector>
#include <cassert>

namespace Sim {
	
	//Different Coupling types which can be passed to instantiate an IsingPolicy
	
	//returns a specific coupling J_ij = J_rcs: also for non-neighbouring spins
	template <int J = 1>
	struct UniformCoupling {
	constexpr int operator()(std::size_t row1, std::size_t col1, 
													 std::size_t row2, std::size_t col2 ) const{
			return J;
		}
	};


	///////////////////////////////////////////////////////////////////////////
	// IsingPolicy 	//only nearest neighbour couplings on 2dmesh, no externa field
	template <typename Coupling = UniformCoupling<1>>
	class IsingPolicy {
	public:
		
		typedef double real_t;
		
		
		IsingPolicy(): engine_(11),row_distr_(0, Sim::Lattice<int>::HEIGHT-1),col_distr_(0,Sim::Lattice<int>::WIDTH-1), 
									 accept_distr_(0.,1.), width_(Sim::Lattice<int>::WIDTH),height_(Sim::Lattice<int>::HEIGHT),
									 inv_temperature_(1.), E_is_initialized(false), M_is_initialized(false)
									 {
									 }
		
		
		template<typename SpinType>
		void update_model(Sim::Lattice<SpinType>& lattice) {

			
			//Choose a random spin
			std::size_t row = row_distr_(engine_);
			std::size_t col = col_distr_(engine_);
			
			//compute energy change for the system with the flipped spin
			//For every neighbour, dE = 2*J_ij TODO check
			real_t dE = 2 * (-1) * compute_4_neighbour_E(row,col,lattice);
											
			
			//accept with p=min(1,exp(-T*dE))
			if (dE <= 0 || accept_distr_(engine_) < std::exp(-inv_temperature_*dE)) {
				lattice(row,col) *= -1;
				E_ += dE;
                M_ += 2*(lattice(row,col));
			}
			
		}
		
		//energy calculation: could be optimized for certain types of couplings
		//TODO --> delegate computation to Coupling template
		
		void set_invtemperature (real_t inv_T) {
			inv_temperature_ = inv_T;
		}
		
		
		template<typename SpinType>
		real_t compute_Energy(const Sim::Lattice<SpinType>& lattice) {
			if (!E_is_initialized) {
				E_ = 0;
				for (std::size_t i = 0;i< height_;i++) {
					for (std::size_t j = 0; j < width_;j++) {
						E_ += compute_4_neighbour_E(i,j,lattice);
					}
				}
				E_ = E_/2; // count every interaction only once
				E_is_initialized = true;
			}
			return get_Energy();
		}
        
		template <typename SpinType>
		real_t compute_Magnetization(const Sim::Lattice<SpinType>& lattice) {
			if (!M_is_initialized) {
				M_ = 0;
				for (std::size_t i = 0;i< height_;i++) {
					for (std::size_t j = 0; j < width_;j++) {
						M_ += lattice(i,j);
					}
				}
				M_is_initialized = true;
			}
			return get_Magnetization();
		}
		
		
		
	private:
		real_t inv_temperature_;
		real_t E_; bool E_is_initialized;
		real_t M_; bool M_is_initialized;
		Coupling J_;
		std::mt19937 engine_;
		std::uniform_int_distribution<std::size_t> row_distr_;
		std::uniform_int_distribution<std::size_t> col_distr_;
		std::uniform_real_distribution<real_t> accept_distr_;
		const std::size_t width_;
		const std::size_t height_;
		
		
		real_t get_Energy() const {
			return E_;
		}
        
		real_t get_Magnetization() const {
			return M_/(width_*height_);
		}
        
// computes the local energy contribution of the spin at position (row,col)
		template<typename SpinType>
		real_t compute_4_neighbour_E(std::size_t row, std::size_t col, const Sim::Lattice<SpinType>& lattice)  {
			auto spin_middle = lattice(row,col);
			return  			 spin_middle*(J_(row,col,row,(col+width_-1)%width_)	 *lattice(row,(col+width_-1)%width_)     // left
																+ J_(row,col,row,(col+1)%width_)         *lattice(row,(col+1)%width_)				// right
																+ J_(row,col,(row+height_-1)%height_,col)*lattice((row+height_-1)%height_,col)  // down
																+ J_(row,col,(row+1)%height_,col)        *lattice((row+1)%height_,col));			// up
		}
		
		

	
	};



} //end namespace Sim


#endif //header guards
