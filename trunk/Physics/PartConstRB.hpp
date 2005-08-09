#pragma once

#include "../Math/Math.hpp"
#include "./Particle.hpp"
#include "./Force.hpp"
#include "./ConstraintSystem.hpp"


namespace Edge
{
	/**
	Particle Based Constraint Rigid Body. Base class intended to support the
	simulation of a rigid body using constrained dynamics. For example the BridgeLinked
	class is derived from this.
	*/
	class PartConstRB :
		public DEStateSource
	{
	public:

		PartConstRB(void);
		virtual ~PartConstRB(void);

		/**
		Set the force on a particle
		@param Idx The index of the particle
		@param Force The force on the particle
		*/
		void SetForce(int Idx, const bnu::vector<double>& Force);

		/**
		Set particle at the given index
		@param Idx The index of the particle
		@param pParticle The force on the particle
		*/
		void SetParticle(int Idx, Edge::ParticlePtr pParticle);

		/**
		Get the force on a particle. If you want to avoid the copying
		at the return point (return by value) use the GetForce(int Idx, bnu::vector<double>& Force)
		@param Idx The index of the particle
		@return The force on the particle at index Idx
		*/
		bnu::vector<double> GetForce(int Idx);

		/**
		Get the force on a particle. 
		@param Idx The index of the particle
		@param Force Output parameter that contains the force on the particle at the given Idx
		*/
		void GetForce(int Idx, bnu::vector<double>& Force);

		const ConstraintSystem& GetConstraintSystem() { return m_CS; }

		void SetForceAllParticles(const bnu::vector<double>& Force);
		
		/**
		Pure virtual function. Derived classes implement this to setup the constraint system
		(m_CS) and the particles.
		*/
		virtual void BuildSystem()=0;

		void ResizeForceContainer(TDForceVector::size_type Size);
		void ResizeDampingContainer(TDForceVector::size_type Size);
		void ResizeParticleContainer(TDForceVector::size_type Size);
		void ResizeCollisionForceContainer(TDForceVector::size_type Size);
		void ResizeFrictionForceContainer(TDForceVector::size_type Size);

		/**
		Resize the constraint system
		@param n The new number of particles in the system.
		*/
//		virtual void Resize(int n);

		/**
		@param State The current state of the system.
		*/
		virtual void GetState(StateType& State);

		/**
		@param StateDerivative The state derivative of the system.
		*/
		virtual void GetStateDerivative(StateType& StateDerivative);

		/**
		@param State Set the state of the system
		*/
		virtual void SetState(const StateType& State);			
		
		void SetParticles(const Edge::TDParticleVector& Particles);

		void AddParticle(const Edge::ParticlePtr pParticle);

		void SetDampingConstant(double Damping);

		void CalculateDamping();

		bool CheckForCollision(Edge::ParticlePtr pP, double dt, double& t);

		void CalculateCollisionForce();
		void CalculateFrictionForce();
		//void CalculateFrictionForce(const bnu::vector<double>&);

		TDParticleVector::iterator GetParticleBegin() {return m_Particles.begin();}
		TDParticleVector::const_iterator GetParticleBegin() const {return m_Particles.begin();}
		TDForceVector::iterator GetForceBegin() {return m_Forces.begin();}
		TDForceVector::const_iterator GetForceBegin() const {return m_Forces.begin();}

		TDParticleVector::iterator GetParticleEnd() {return m_Particles.end();}
		TDParticleVector::const_iterator GetParticleEnd() const {return m_Particles.end();}
		TDForceVector::iterator GetForceEnd() {return m_Forces.end();}
		TDForceVector::const_iterator GetForceEnd() const {return m_Forces.end();}

		TDForceVector::iterator GetDampingBegin();
		TDForceVector::iterator GetDampingEnd();
		TDForceVector::const_iterator GetDampingBegin() const;
		TDForceVector::const_iterator GetDampingEnd() const;

	protected:
		TDForceVector m_Forces; /**Force per particle*/
		TDForceVector m_Damping; /**Damping force per particle*/
		TDParticleVector m_Particles; /**Particles in the system*/
		ConstraintSystem m_CS;	/**Constraints on particles*/
		TDForceVector m_Collision; /**Collision force*/
		TDForceVector m_Friction; /**Friction force*/
		double m_DampingConstant;
		double m_StaticFriction;
		double m_MovingFriction;
	};
}
