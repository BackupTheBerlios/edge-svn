#pragma once
#include "./ODE.hpp"

namespace Edge
{
	/**
	A class that implements the Euler Solver. In general this is a bad solver to use
	since it diverges from the solution rapidly.
	*/
	class EulerSolver :
		public ODE
	{
	public:
		EulerSolver(void);
		virtual ~EulerSolver(void);

		/**			
		Steps the state of the differential system by the given time step using the 
		Euler method
		@param Source The differentiable system that we want a numerical solution for.
		@param dt The time step to use. Smaller time steps result in a more accurate approximation.		
		*/
		virtual void StepState(DEStateSource& rSource, double dt);
	};
}