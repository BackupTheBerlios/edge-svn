//#include "StdAfx.h"
#include "./FixedPosition.hpp"

using namespace Edge;
//using MassSpringSystem::TDParticleToForce;
//using MassSpringSystem::TDPSPTuple;
//using MassSpringSystem::TDPSPTupleContainer;

FixedPosition::FixedPosition(Edge::ParticlePtr pP0, int MatrixCol, 
							 int NumParticles, double SpringConst) :
	m_MatrixCol(MatrixCol),
	m_pP0(pP0),
	m_Position(pP0->GetPosition())
{
	ResizeJacobian(NumParticles);
	ResizeDJacobian(NumParticles);
	ResizeConPos();
	ResizeConVel();
}

FixedPosition::~FixedPosition(void)
{
}

void FixedPosition::ResizeConPos()
{
	m_ConPos.resize(3);
}

void FixedPosition::ResizeConVel()
{
	m_ConVel.resize(3);
}

void FixedPosition::ResizeJacobian(int NumParticles)
{
	//m_Jacobian.resize(1, NumParticles*3);	
	m_Jacobian = bnu::zero_matrix<double>(3, NumParticles*3);	
}

void FixedPosition::ResizeDJacobian(int NumParticles)
{
//	m_DJacobian.resize(1, NumParticles*3);
	m_DJacobian = bnu::zero_matrix<double>(3, NumParticles*3);
}

void FixedPosition::CalculateJacobian()
{
	//for a fixed position contraint the result of dC/dq is I where the particle
	//is and 0 every else
	//create a reference to a range/sub-matrix of our main matrix
	bnu::matrix_range < bnu::sparse_matrix<double> > mr(m_Jacobian, 
		bnu::range(0, 3), //row range
			bnu::range(m_MatrixCol*3, m_MatrixCol*3+3)); //column range
	mr = bnu::identity_matrix<double>(3,3);
}
	
void FixedPosition::CalculateDJacobian()
{	
}

int FixedPosition::GetRowSize() const
{
	return 3;
}

void FixedPosition::CalculateDriftForce()
{
	m_DriftCorrection.CalculateForce();
}

const bnu::vector<double>& FixedPosition::GetDriftForce(Edge::ParticlePtr pParticle) const
{
	return m_DriftCorrection.GetForce(pParticle);
}

const bnu::vector<double>& FixedPosition::GetPosition() const
{
	return m_Position;
}

bool FixedPosition::InConstraint(Edge::ParticlePtr pParticle) const
{
	return m_pP0 == pParticle;
}

void FixedPosition::CalculateConVel()
{
	m_ConVel = m_pP0->GetVelocity();
}

void FixedPosition::CalculateConPos()
{	
	m_ConPos = m_pP0->GetPosition() - m_Position;
}
