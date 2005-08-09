#pragma once
#include "./Particle.hpp"
#include "../Math/DEStateSource.hpp"

namespace Edge
{

	class DoublePendulum :
		public DEStateSource
	{
	public:
		DoublePendulum(void);
		virtual ~DoublePendulum(void);

		/**
		@param State The current state of the system.
		*/
		virtual void GetState(StateType& State) const =0;

		/**
		@param StateDerivative The state derivative of the system.
		*/
		virtual void GetStateDerivative(StateType& StateDerivative) const =0;

		/**
		@param State Set the state of the system
		*/
		virtual void SetState(const StateType& State)=0;
	
	private:
		Edge::ParticlePtr pP0;
		Edge::ParticlePtr pP1;
	};
}
