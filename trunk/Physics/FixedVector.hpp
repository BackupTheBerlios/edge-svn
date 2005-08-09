#pragma once

#include "./Particle.hpp"
#include "./Constraint.hpp"


namespace Edge
{
	class FixedVector :
		public Edge::Constraint
	{
		/** 
		A fixed vector constraint preserves the initial difference between the particles
		*/
	public:
		typedef boost::shared_ptr<FixedVector> FixedVectorPtr;

		FixedVector(Edge::ParticlePtr pP0, Edge::ParticlePtr pP1, 
			int MatrixCol0, int MatrixCol1,
			int NumParticles);
		virtual ~FixedVector(void);
		virtual void CalculateJacobian();
		virtual void CalculateDJacobian();
		virtual int GetRowSize() const;
		void SetP0(Edge::ParticlePtr pP0) {m_pP0 = pP0;}
		void SetP1(Edge::ParticlePtr pP1) {m_pP1 = pP1;}
		void SetMatrixCol0(int Idx) {m_MatrixCol0 = Idx;}
		void SetMatrixCol1(int Idx) {m_MatrixCol1 = Idx;}
		Edge::ParticlePtr GetP0() {return m_pP0;}		
		Edge::ParticlePtr GetP1() {return m_pP1;}		
		virtual void ResizeJacobian(int NumParticles);
		virtual void ResizeDJacobian(int NumParticles);
		virtual void ResizeConPos();
		virtual void ResizeConVel();
		virtual void CalculateDriftForce();
		virtual const bnu::vector<double>& GetDriftForce(Edge::ParticlePtr) const;
		const bnu::vector<double>& GetPosition() const;
		virtual bool InConstraint(Edge::ParticlePtr) const;
		virtual void CalculateConVel();
		virtual void CalculateConPos();

	private:
		MassSpringSystem m_DriftCorrection;
		Edge::ParticlePtr m_pP0;
		Edge::ParticlePtr m_pP1;
		int m_MatrixCol0;
		int m_MatrixCol1;
		const bnu::vector<double> m_Vector;
	};
}
