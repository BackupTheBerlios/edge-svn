#pragma once
#include "./ODE.hpp"

namespace Edge
{
	/**
	A class that implements the classical 4th order Runge-Kutta algorithm. For the algorithm
	see any book on differential equations.
	*/
	class RK4Solver :
		public ODE
	{
	public:
		RK4Solver(void);
		virtual ~RK4Solver(void);

		/**
		Steps the state of the differential system by the given time step using the 4th order
		Runge-Kutta algorithm.
		@param Source The differentiable system that we want a numerical solution for.
		@param dt The time step to use. Smaller time steps result in a more accurate approximation.
		*/
		virtual void StepState(DEStateSource& Source, double dt);
	};
}