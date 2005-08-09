//#include "StdAfx.h"
#include "./FixedVector.hpp"

using namespace Edge;
using Edge::ParticlePtr;
//using MassSpringSystem::TDPSPTuple;
//using MassSpringSystem::TDPSPTupleContainer;

FixedVector::FixedVector(ParticlePtr pP0, ParticlePtr pP1, 
					 int MatrixCol0, int MatrixCol1, int NumParticles) :
	m_pP0(pP0),
	m_pP1(pP1),
	m_MatrixCol0(MatrixCol0),
	m_MatrixCol1(MatrixCol1),
	m_Vector(pP0->GetPosition() - pP1->GetPosition())
{
	ResizeDJacobian(NumParticles);
	ResizeJacobian(NumParticles);
	ResizeConPos();
	ResizeConVel();
}

FixedVector::~FixedVector(void)
{
}

int FixedVector::GetRowSize() const
{
	return 3;
}

void FixedVector::CalculateJacobian()
{
	bnu::matrix_range < bnu::sparse_matrix<double> > mr0(m_Jacobian, 
		bnu::range(0, 3), //row range
		bnu::range(m_MatrixCol0*3, m_MatrixCol0*3+3)); //column range
	mr0 = bnu::identity_matrix<double>(3,3);	

	bnu::matrix_range < bnu::sparse_matrix<double> > mr1(m_Jacobian, 
		bnu::range(0, 3), //row range
		bnu::range(m_MatrixCol1*3, m_MatrixCol1*3+3)); //column range
	mr1 = -bnu::identity_matrix<double>(3,3);	
}

void FixedVector::CalculateDJacobian()
{	
	
}

void FixedVector::ResizeJacobian(int NumParticles)
{
	m_Jacobian = bnu::zero_matrix<double>(3, NumParticles*3);	
}

void FixedVector::ResizeDJacobian(int NumParticles)
{
	m_DJacobian = bnu::zero_matrix<double>(3, NumParticles*3);
}

void FixedVector::CalculateDriftForce()
{
	m_DriftCorrection.CalculateForce();
}

const bnu::vector<double>& FixedVector::GetDriftForce(Edge::ParticlePtr pParticle) const
{
	return m_DriftCorrection.GetForce(pParticle);	
}

bool FixedVector::InConstraint(Edge::ParticlePtr pParticle) const
{
	return  m_pP0 == pParticle || m_pP1 == pParticle;
}

void FixedVector::CalculateConVel()
{
	m_ConVel = m_pP0->GetVelocity() - m_pP1->GetVelocity();
}

void FixedVector::CalculateConPos()
{	
	m_ConPos = m_pP0->GetPosition() - m_pP1->GetPosition() - m_Vector;
}

void FixedVector::ResizeConPos()
{
	m_ConPos.resize(3);
}

void FixedVector::ResizeConVel()
{
	m_ConVel.resize(3);
}
