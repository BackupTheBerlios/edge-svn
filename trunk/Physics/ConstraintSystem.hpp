#pragma once

#include "./Constraint.hpp"
#include "./MassSpringSystem.hpp"

namespace Edge
{
	/**
	A class that can be used in a particle system where certain particles are governed
	by constraints. For a overview 
	on how constrained dynamics works and how to implement it see	
	http://www-2.cs.cmu.edu/afs/cs.cmu.edu/user/baraff/www/sigcourse/
	and the thesis here
	http://www.cs.auckland.ac.nz/GG/robin_otte.html	
	*/
	class ConstraintSystem
	{
	public:

		ConstraintSystem(void);
		virtual ~ConstraintSystem(void);

		void AddConstraint(Edge::ConstraintPtr);
		void AddParticle(Edge::ParticlePtr);
		void SetParticles(const TDParticleVector& Particles);
		void SetForceAllParticles(const bnu::vector<double>& Force);		
		void SetForce(const bnu::vector<double>& Forces);
		void SetForce(int Idx, const bnu::vector<double>& Forces);
		const bnu::sparse_matrix<double>& GetJacobian() const {return m_Jacobian;}
		const bnu::sparse_matrix<double>& GetDJacobian() const {return m_DJacobian;}
		const bnu::sparse_matrix<double>& GetMassMatrix() const {return m_Mass;}
		const bnu::sparse_matrix<double>& GetMassInvMatrix() const {return m_MassInv;}
		const bnu::vector<double>& GetLambda() const {return m_Lambda;}		
		const bnu::vector<double>& GetVelocities() const {return m_Velocities;}
		const bnu::vector<double>& GetConstraintForce() const {return m_CForce;}
		void AllowRecalculationMassMatrix() {m_MassCalculated = false;}
		void AllowRecalculationMassInvMatrix() {m_MassInvCalculated = false;}		
		void SetParticle(int Idx, Edge::ParticlePtr pPart);
		void SetConstraint(int Idx, Edge::ConstraintPtr pCons);
		void ClearConstraints();
		void ClearParticles();		
		void RemoveConstraint(TDConstraintVector::size_type Idx);
		bnu::sparse_matrix<double>::size_type ConstraintSystem::CalculateRowSize();		
		void CalculateJacobian();
		void CalculateDJacobian();
		void CalculateConstraintForce();
		void CalculateMassInvMatrix();
		void CalculateVelocities();
		void CalculateMassMatrix();
		void CalculateLambda();
		void CalculateConVel();
		void CalculateConPos();
		void ResizeConPos();
		void ResizeConVel();
		void ResizeParticles(TDParticleVector::size_type Size);
		void ResizeConstraints(TDConstraintVector::size_type Size);
		void ResizeJacobian();
		void ResizeDJacobian();
		void ResizeLambda();
		void ZeroDriftForce();

		TDConstraintVector::iterator GetConstraintBegin() { return m_Constraints.begin(); }
		TDConstraintVector::iterator GetConstraintEnd() { return m_Constraints.end(); }
		TDConstraintVector::const_iterator GetConstraintBegin() const { return m_Constraints.begin(); }
		TDConstraintVector::const_iterator GetConstraintEnd() const { return m_Constraints.end(); }

	private:		
		void ResizeVelocities();		
		void ResizeMass();
		void ResizeMassInv();
		void ResizeForce();
		void ResizeConstraintForce();
		
		bnu::sparse_matrix<double> m_Jacobian;
		bnu::sparse_matrix<double> m_DJacobian;
		bnu::vector<double> m_Lambda;
		bnu::vector<double> m_Velocities;
		bnu::vector<double> m_CForce;
		bnu::sparse_matrix<double> m_MassInv;
		bnu::sparse_matrix<double> m_Mass;
		bnu::vector<double> m_ConVel;
		bnu::vector<double> m_ConPos;
		bool m_MassInvCalculated;
		bool m_MassCalculated;
		TDConstraintVector m_Constraints;
		TDParticleVector m_Particles;
		bnu::vector<double> m_Forces;			
		double m_kd;
		double m_ks;
	};
		
}
	
