#pragma once
#include "./MassSpringSystem.hpp"
#include "./Particle.hpp"

namespace Edge
{
	/**
	An abstract class used to represent a geometric constraint. Note that the constraints
	derived from this class are holonomic and do not depend on time. For a overview 
	on how constrained dynamics works and how to implement it see	
	http://www-2.cs.cmu.edu/afs/cs.cmu.edu/user/baraff/www/sigcourse/
	and the thesis here
	http://www.cs.auckland.ac.nz/GG/robin_otte.html
	*/

	class Constraint
	{
	public:

		Constraint(void);
		virtual ~Constraint(void);
		
		/**
		Return the Jacobian of the system (i.e. the matrix that contains
		the derivatives of the constraint with respect to the particles.
		Use Constraint::CalculateJacobian to calculate the matrix and this function to
		return the matrix.
		@return A matrix representing the Jacobian of the system
		*/
		const bnu::matrix<double>& GetJacobian() const;

		/**
		Return the derivative of the Jacobian with respect to time.
		Use Constraint::CalculateDJacobian to calculate the matrix and this function to
		return the matrix.
		@return A matrix representing the derivative of the Jacobian.
		*/
		const bnu::matrix<double>& GetDJacobian() const;

		/**
		Return the constraint position. The legal position of the particle.
		@return A vector representing the legal position of the particle
		*/
		const bnu::vector<double>& GetConPos() const {return m_ConPos;}

		/**
		Return the constraint velocity. 
		@return A vector representing the legal velocity of the particle
		*/
		const bnu::vector<double>& GetConVel() const {return m_ConVel;}

		/**
		Set the size of the Jacobian. The row size of the Jacobian depends on whether 
		the constraint is a vector or a scalar. The columns depends on the number
		of particles in affected by the constraint.
		*/
		virtual void ResizeJacobian(int NumParticles)=0;

		/**
		Same size as the Jacobian.
		*/
		virtual void ResizeDJacobian(int NumParticles)=0;

		virtual void ResizeConPos()=0;

		virtual void ResizeConVel()=0;

		/**
		Calculate the Jacobian of the system.
		*/
		virtual void CalculateJacobian()=0;
		
		/**		
		Calculate the derivative of the Jacobian with respect to time.
		*/
		virtual void CalculateDJacobian()=0;

		/**
		Get the row size of the Jacobian. The row size depends on whether the constraint
		is a scalar or a vector.
		@return Row size of the Jacobian matrix.
		*/
		virtual int GetRowSize() const = 0;
		
		/**
		Is the given particle used by this constraint?
		@param pParticle The particle in question.
		*/
		virtual bool InConstraint(Edge::ParticlePtr pParticle) const=0;
		
		/**
		Calculate the constraint velocity.
		*/
		virtual void CalculateConVel()=0;

		/**
		Calculate the constraint position.
		*/
		virtual void CalculateConPos()=0;

	protected:		
		bnu::matrix<double> m_Jacobian; //dC/dq
		bnu::matrix<double> m_DJacobian; //D[dC/dq]/dt
		bnu::vector<double> m_ConPos;
		bnu::vector<double> m_ConVel;
	};

	typedef boost::shared_ptr<Constraint> ConstraintPtr;
	typedef std::vector<Edge::ConstraintPtr> TDConstraintVector;
}
