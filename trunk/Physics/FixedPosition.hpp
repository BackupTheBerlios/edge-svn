#pragma once
#include "./Constraint.hpp"

namespace Edge
{
	class FixedPosition :
		public Edge::Constraint
	{
		/**
		Keeps a particle in a fixed position.
		*/
	public:
		typedef boost::shared_ptr<FixedPosition> FixedPositionPtr;

		FixedPosition(Edge::ParticlePtr pP0, int MatrixCol, 
			int NumParticles, double SpringConst);
		virtual ~FixedPosition(void);
		virtual void CalculateJacobian();
		virtual void CalculateDJacobian();
		virtual void CalculateConVel();
		virtual void CalculateConPos();
		virtual int GetRowSize() const;
		void SetP0(Edge::ParticlePtr pP0) {m_pP0 = pP0;}
		void SetMatrixCol(int Idx) {m_MatrixCol = Idx;}
		Edge::ParticlePtr GetP0() {return m_pP0;}		
		virtual void ResizeJacobian(int NumParticles);
		virtual void ResizeDJacobian(int NumParticles);
		virtual void ResizeConPos();
		virtual void ResizeConVel();
		virtual void CalculateDriftForce();
		virtual const bnu::vector<double>& GetDriftForce(Edge::ParticlePtr) const;
		const bnu::vector<double>& GetPosition() const;
		virtual bool InConstraint(Edge::ParticlePtr) const;

	private:
		MassSpringSystem m_DriftCorrection;
		Edge::ParticlePtr m_pP0;
		int m_MatrixCol;
		const bnu::vector<double> m_Position;
	};
}