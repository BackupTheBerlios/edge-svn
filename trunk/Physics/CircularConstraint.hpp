#pragma once

#include "../Math/DEStateSource.hpp"
#include "./Particle.hpp"

namespace Edge
{
	using namespace boost::numeric;

	/**
	A class that implements a circular constraint on a particle. It allows one to specify
	a particle's position, velocity and the external forces on a particle and subsequently
	calculate the constraint force required to keep the particle on the circle. A more generic
	implementation is given in ConstraintSystem.
	*/

	class CircularConstraint :
		public DEStateSource
	{
	public:
		CircularConstraint();
		
		/**
		Constructor to inititialize particles state (i.e. position and velocity)
		@param x Particles x position
		@param y Particles y position
		@param z Particles y position
		@param dx Particles x velocity
		@param dy Particles y velocity
		@param dz Particles z velocity
		*/
		CircularConstraint(double x, double y, double z, double dx, double dy, double dz);

		virtual ~CircularConstraint(void);
		
		/**
		Set the external force on a particle.
		@param Force The external force applied to the particle.
		*/
		void SetForce(const ublas::vector<double>& Force) {m_Force = Force; }

		/**
		Get the state of the system. In this specific instance this means the particles
		position and velocity.
		@param State The position and velocity of the particle
		*/
		void GetState(DEStateSource::StateType& State) const;

		/**
		Get the derivative of the system. In this instance this is the velocity and acceleration
		of the particle.
		@param StateDerivative The velocity and acceleration of the particle		
		*/
		void GetStateDerivative(DEStateSource::StateType& StateDerivative) const;

		/**
		Explictly change the state of the system (i.e. change the position 
		and velocity of the particle)
		@param State The position and velocity of the particle
		*/
		void SetState(const StateType& State);

		/**
		Calculate and return the constraint force required to keep the particle on the circle
		@return Constraint force.
		*/
		ublas::vector<double> CircularConstraint::GetConstraintForce() const;

	private:
		
		ublas::vector<double> m_Force;
		Edge::Particle m_Particle;
		double m_Radius;
	};
}
