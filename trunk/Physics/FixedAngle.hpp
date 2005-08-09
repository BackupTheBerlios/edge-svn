#pragma once

#include "./Constraint.hpp"

class FixedAngle :
	public Edge::Constraint
{
	/**
	Maintain a fixed angle between 2 vectors
	*/

public:
	typedef boost::shared_ptr<FixedAngle> FixedAnglePtr;

	FixedAngle(Edge::ParticlePtr pP0, Edge::ParticlePtr pP1, Edge::ParticlePtr pP2,
				int MatrixCol0, int MatrixCol1, int MatrixCol2, 
				int NumParticles);

	virtual ~FixedAngle(void);

	virtual void CalculateJacobian();

	virtual void CalculateDJacobian();

	virtual void CalculateConVel();

	virtual void CalculateConPos();

	virtual int GetRowSize() const;

	void SetP0(Edge::ParticlePtr pP) {m_pP0 = pP;}
	
	void SetP1(Edge::ParticlePtr pP) {m_pP1 = pP;}

	void SetP2(Edge::ParticlePtr pP) {m_pP2 = pP;}

	void SetMatrixCol0(int Idx) {m_MatrixCol0 = Idx;}
	
	void SetMatrixCol1(int Idx) {m_MatrixCol1 = Idx;}

	void SetMatrixCol2(int Idx) {m_MatrixCol2 = Idx;}

	Edge::ParticlePtr GetP1() {return m_pP0;}		
	
	Edge::ParticlePtr GetP2() {return m_pP1;}		

	Edge::ParticlePtr GetP3() {return m_pP2;}		

	virtual void ResizeJacobian(int NumParticles);

	virtual void ResizeDJacobian(int NumParticles);

	virtual void ResizeConPos();

	virtual void ResizeConVel();

	double GetAngle() const;

	virtual bool InConstraint(Edge::ParticlePtr pParticle) const;

private:
	double m_Angle; /**The angle to maintain*/
	Edge::ParticlePtr m_pP0;
	Edge::ParticlePtr m_pP1;
	Edge::ParticlePtr m_pP2;
	int m_MatrixCol0;
	int m_MatrixCol1;
	int m_MatrixCol2;
	
};
