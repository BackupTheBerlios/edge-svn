#pragma once
#include "./Constraint.hpp"
#include "./Particle.hpp"
#include "./MassSpringSystem.hpp"

namespace Edge
{	
	class FixedDist :
		public Edge::Constraint
	{

		/**
		Implements a fixed distance constraint. A fixed distance constraint keeps particles
		a fixed distance appart. The initial distance is the difference between the particles initial
		positions.
		*/
	public:
		typedef boost::shared_ptr<FixedDist> FixedDistPtr;

		FixedDist(Edge::ParticlePtr pP0, Edge::ParticlePtr pP1, 
			int sparse_matrixCol0, int sparse_matrixCol1, int NumParticles);
		virtual ~FixedDist(void);

		double GetDistance() const;
		void SetP0(Edge::ParticlePtr pP0) {m_pP0 = pP0;}
		void SetP1(Edge::ParticlePtr pP1) {m_pP1 = pP1;}
		void SetIdx0(int Idx) {m_MatrixCol0 = Idx;}
		void SetIdx1(int Idx) {m_MatrixCol1 = Idx;}
		Edge::ParticlePtr GetP0() {return m_pP0;}
		Edge::ParticlePtr GetP1() {return m_pP1;}
		virtual void ResizeJacobian(int NumParticles);
		virtual void ResizeDJacobian(int NumParticles);
		virtual void ResizeConPos();
		virtual void ResizeConVel();
		virtual void CalculateJacobian();
		virtual void CalculateDJacobian();
		virtual int GetRowSize() const;
		virtual void CalculateDriftForce();
		virtual const bnu::vector<double>& GetDriftForce(Edge::ParticlePtr) const;
		virtual bool InConstraint(Edge::ParticlePtr) const;
		virtual void CalculateConVel();
		virtual void CalculateConPos();
		
	private:		
		MassSpringSystem m_DriftCorrection;
		Edge::ParticlePtr m_pP0;
		Edge::ParticlePtr m_pP1;
		int m_MatrixCol0;
		int m_MatrixCol1;
		const double m_Dist;
	};
}