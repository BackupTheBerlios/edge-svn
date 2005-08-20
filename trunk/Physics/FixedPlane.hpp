#pragma once

#include "./Particle.hpp"
#include "./Constraint.hpp"

namespace Edge
{
	class FixedPlane :
		public Constraint
	{
		/**
		A fixed plane constraint tells the particle to remain on the plave. The plane is described using its
		normal and distance from origin.
		*/
	public:
		typedef boost::shared_ptr<FixedPlane> FixedPlanePtr;

		FixedPlane(const bnu::vector<double>& Normal, double Distance, Edge::ParticlePtr pP0,
			int MatrixCol, int NumParticles);
		virtual ~FixedPlane(void);
		virtual void CalculateJacobian();
		virtual void CalculateDJacobian();
		virtual int GetRowSize() const;
		void SetMatrixCol(int Col) {m_MatrixCol = Col;}
		void SetNormal(const bnu::vector<double>& Normal);
		const bnu::vector<double>& GetNormal();
		virtual void ResizeJacobian(int NumParticles);
		virtual void ResizeDJacobian(int NumParticles);
		virtual void ResizeConPos();
		virtual void ResizeConVel();
		virtual void CalculateDriftForce();
		virtual const bnu::vector<double>& GetDriftForce(Edge::ParticlePtr) const;
		double GetDistance() { return m_Distance; }		
		virtual bool InConstraint(Edge::ParticlePtr) const;
		virtual void CalculateConVel();
		virtual void CalculateConPos();


	private:
		Edge::ParticlePtr m_pP;
		int m_MatrixCol;
		bnu::vector<double> m_Normal;
		bnu::vector<double> m_DriftForce;
		const double m_Distance;

	};
}