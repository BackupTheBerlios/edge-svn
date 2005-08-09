//#include "StdAfx.h"
#include "./FixedDist.hpp"

using namespace Edge;
//using Edge::ParticlePtr;
//using MassSpringSystem::TDPSPTuple;
//using MassSpringSystem::TDPSPTupleContainer;

FixedDist::FixedDist(ParticlePtr pP0, ParticlePtr pP1, 
					 int sparse_matrixCol0, int sparse_matrixCol1, int NumParticles) :
	m_pP0(pP0),
	m_pP1(pP1),
	m_MatrixCol0(sparse_matrixCol0),
	m_MatrixCol1(sparse_matrixCol1),
	m_Dist(bnu::norm_2(pP1->GetPosition() - pP0->GetPosition()))
{
	ResizeDJacobian(NumParticles);
	ResizeJacobian(NumParticles);
	ResizeConPos();
	ResizeConVel();
}

FixedDist::~FixedDist(void)
{
}

int FixedDist::GetRowSize() const
{
	return 1;
}

void FixedDist::CalculateJacobian()
{
	bnu::vector<double> Pos0(3);
	bnu::vector<double> Pos1(3);
	Pos0 = m_pP0->GetPosition();
	Pos1 = m_pP1->GetPosition();
	bnu::matrix_range < bnu::sparse_matrix<double> > mr0(m_Jacobian, 
		bnu::range(0, 1), 
		bnu::range(m_MatrixCol0*3, m_MatrixCol0*3+3));
	mr0(0,0) = Pos0[0] - Pos1[0];
	mr0(0,1) = Pos0[1] - Pos1[1];
	mr0(0,2) = Pos0[2] - Pos1[2];

	bnu::matrix_range < bnu::sparse_matrix<double> > mr1(m_Jacobian, 
		bnu::range(0, 1), 
		bnu::range(m_MatrixCol1*3, m_MatrixCol1*3+3));
	mr1(0,0) = Pos1[0] - Pos0[0];
	mr1(0,1) = Pos1[1] - Pos0[1];
	mr1(0,2) = Pos1[2] - Pos0[2];
}

void FixedDist::CalculateDJacobian()
{	
	bnu::vector<double> Vel0(3);
	bnu::vector<double> Vel1(3);
	Vel0 = m_pP0->GetVelocity();
	Vel1 = m_pP1->GetVelocity();
	bnu::matrix_range < bnu::sparse_matrix<double> > mr0(m_DJacobian, 
		bnu::range(0, 1), 
		bnu::range(m_MatrixCol0*3, m_MatrixCol0*3+3));
	mr0(0,0) = Vel0[0] - Vel1[0];
	mr0(0,1) = Vel0[1] - Vel1[1];
	mr0(0,2) = Vel0[2] - Vel1[2];

	bnu::matrix_range < bnu::sparse_matrix<double> > mr1(m_DJacobian, 
		bnu::range(0, 1), 
		bnu::range(m_MatrixCol1*3, m_MatrixCol1*3+3));
	mr1(0,0) = Vel1[0] - Vel0[0];
	mr1(0,1) = Vel1[1] - Vel0[1];
	mr1(0,2) = Vel1[2] - Vel0[2];
}

void FixedDist::ResizeJacobian(int NumParticles)
{
	m_Jacobian = bnu::zero_matrix<double>(1, NumParticles*3);	
}

void FixedDist::ResizeDJacobian(int NumParticles)
{
	m_DJacobian = bnu::zero_matrix<double>(1, NumParticles*3);
}

void FixedDist::ResizeConPos()
{
	m_ConPos.resize(1);
}

void FixedDist::ResizeConVel()
{
	m_ConVel.resize(1);
}

void FixedDist::CalculateDriftForce()
{
	m_DriftCorrection.CalculateForce();
}

const bnu::vector<double>& FixedDist::GetDriftForce(Edge::ParticlePtr pParticle) const
{
	return m_DriftCorrection.GetForce(pParticle);	
}

/*
double FixedDist::GetDistance() const
{
	TDPSPTupleContainer::const_iterator it(m_DriftCorrection.PSPBegin());
	Spring::SpringPtr pS(it->get<1>());
	return pS->GetRestLen();
}
*/

bool FixedDist::InConstraint(Edge::ParticlePtr pParticle) const
{
	return  m_pP0 == pParticle || m_pP1 == pParticle;
}

void FixedDist::CalculateConVel()
{
	bnu::vector<double> r(m_pP1->GetPosition() - m_pP0->GetPosition());
	bnu::vector<double> v(m_pP1->GetVelocity() - m_pP0->GetVelocity());
	m_ConVel[0] = bnu::inner_prod(r,v);
}

void FixedDist::CalculateConPos()
{
	bnu::vector<double> v(m_pP1->GetPosition() - m_pP0->GetPosition());
	//m_ConPos[0] = 0.5*bnu::inner_prod(v,v) - m_Dist*m_Dist;
	m_ConPos[0] = bnu::norm_2(v) - m_Dist;
}
