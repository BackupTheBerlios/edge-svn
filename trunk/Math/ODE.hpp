#pragma once

namespace Edge
{
	class DEStateSource;

	/**
	Represents a abstract base class (i.e. no instance can be created)
	for a differential equation solver. If you want to implement
	your own algorithm then inherit from this class.
	*/
	class ODE
	{
	public:
		ODE(void);
		virtual ~ODE(void);

		/**
		Steps the state of the differential system by the given time step.
		@param Source The differentiable system that we want a numerical solution for.
		@param dt The time step to use. Smaller time steps result in a more accurate approximation.
		*/
		virtual void StepState(DEStateSource& Source, double dt)=0;
	};
}
