//#include "StdAfx.h"
#include "./FixedAngle.hpp"

using namespace Edge;
//using MassSpringSystem::TDParticleToForce;
//using MassSpringSystem::TDPSPTuple;
//using MassSpringSystem::TDPSPTupleContainer;

FixedAngle::FixedAngle(Edge::ParticlePtr pP0, Edge::ParticlePtr pP1, Edge::ParticlePtr pP2,
					   int MatrixCol0, int MatrixCol1, int MatrixCol2, 
					   int NumParticles) :
	m_MatrixCol0(MatrixCol0),
	m_MatrixCol1(MatrixCol1),
	m_MatrixCol2(MatrixCol2),
	m_pP0(pP0),
	m_pP1(pP1),
	m_pP2(pP2)	
{
	//the angle to maintain
	m_Angle = bnu::inner_prod(m_pP0->GetPosition() - m_pP1->GetPosition(), m_pP2->GetPosition() - m_pP1->GetPosition());
	ResizeJacobian(NumParticles);
	ResizeDJacobian(NumParticles);
	ResizeConPos();
	ResizeConVel();	
}

FixedAngle::~FixedAngle(void)
{
}


void FixedAngle::ResizeConVel()
{
	m_ConVel.resize(1);
}

void FixedAngle::ResizeConPos()
{
	m_ConPos.resize(1);
}

void FixedAngle::ResizeJacobian(int NumParticles)
{
	//m_Jacobian.resize(1, NumParticles*3);	
	m_Jacobian = bnu::zero_matrix<double>(1, NumParticles*3);	
}

void FixedAngle::ResizeDJacobian(int NumParticles)
{
//	m_DJacobian.resize(1, NumParticles*3);
	m_DJacobian = bnu::zero_matrix<double>(1, NumParticles*3);
}

void FixedAngle::CalculateJacobian()
{
	bnu::vector<double> Pos0(3);
	bnu::vector<double> Pos1(3);
	bnu::vector<double> Pos2(3);
	Pos0 = m_pP0->GetPosition();
	Pos1 = m_pP1->GetPosition();
	Pos2 = m_pP2->GetPosition();
	bnu::matrix_range < bnu::sparse_matrix<double> > mr0(m_Jacobian, 
		bnu::range(0, 1), 
		bnu::range(m_MatrixCol0*3, m_MatrixCol0*3+3));
	mr0(0,0) = Pos2[0] - Pos1[0];
	mr0(0,1) = Pos2[1] - Pos1[1];
	mr0(0,2) = Pos2[2] - Pos1[2];

	bnu::matrix_range < bnu::sparse_matrix<double> > mr1(m_Jacobian, 
		bnu::range(0, 1), 
		bnu::range(m_MatrixCol1*3, m_MatrixCol1*3+3));
	mr1(0,0) = -(Pos0[0] - Pos1[0]) - (Pos2[0] - Pos1[0]);
	mr1(0,1) = -(Pos0[1] - Pos1[1]) - (Pos2[1] - Pos1[1]);
	mr1(0,2) = -(Pos0[2] - Pos1[2]) - (Pos2[2] - Pos1[2]);

	bnu::matrix_range < bnu::sparse_matrix<double> > mr2(m_Jacobian, 
		bnu::range(0, 1), 
		bnu::range(m_MatrixCol2*3, m_MatrixCol2*3+3));
	mr2(0,0) = Pos0[0] - Pos1[0];
	mr2(0,1) = Pos0[1] - Pos1[1];
	mr2(0,2) = Pos0[2] - Pos1[2];
}
	
void FixedAngle::CalculateDJacobian()
{	
	bnu::vector<double> Vel0(3);
	bnu::vector<double> Vel1(3);
	bnu::vector<double> Vel2(3);
	Vel0 = m_pP0->GetVelocity();
	Vel1 = m_pP1->GetVelocity();
	Vel2 = m_pP2->GetVelocity();
	bnu::matrix_range < bnu::sparse_matrix<double> > mr0(m_DJacobian, 
		bnu::range(0, 1), 
		bnu::range(m_MatrixCol0*3, m_MatrixCol0*3+3));
	mr0(0,0) = Vel2[0] - Vel1[0];
	mr0(0,1) = Vel2[1] - Vel1[1];
	mr0(0,2) = Vel2[2] - Vel1[2];

	bnu::matrix_range < bnu::sparse_matrix<double> > mr1(m_DJacobian, 
		bnu::range(0, 1), 
		bnu::range(m_MatrixCol1*3, m_MatrixCol1*3+3));
	mr1(0,0) = -(Vel0[0] - Vel1[0]) - (Vel2[0] - Vel1[0]);
	mr1(0,1) = -(Vel0[1] - Vel1[1]) - (Vel2[1] - Vel1[1]);
	mr1(0,2) = -(Vel0[2] - Vel1[2]) - (Vel2[2] - Vel1[2]);

	bnu::matrix_range < bnu::sparse_matrix<double> > mr2(m_DJacobian, 
		bnu::range(0, 1), 
		bnu::range(m_MatrixCol2*3, m_MatrixCol2*3+3));
	mr2(0,0) = Vel0[0] - Vel1[0];
	mr2(0,1) = Vel0[1] - Vel1[1];
	mr2(0,2) = Vel0[2] - Vel1[2];
}

int FixedAngle::GetRowSize() const
{
	return 1;
}


double FixedAngle::GetAngle() const
{
	return m_Angle;
}

bool FixedAngle::InConstraint(Edge::ParticlePtr pParticle) const
{
	return m_pP0 == pParticle || m_pP1 == pParticle || m_pP2 == pParticle;
}

void FixedAngle::CalculateConVel()
{
	assert(m_ConVel.size() == 1);
	const bnu::vector<double>& Pos0(m_pP0->GetPosition());
	const bnu::vector<double>& Pos1(m_pP1->GetPosition());
	const bnu::vector<double>& Pos2(m_pP2->GetPosition());
	const bnu::vector<double>& Vel0(m_pP0->GetVelocity());
	const bnu::vector<double>& Vel1(m_pP1->GetVelocity());
	const bnu::vector<double>& Vel2(m_pP2->GetVelocity());
	m_ConVel[0] = bnu::inner_prod(Vel0 - Vel1, Pos2 - Pos1) + bnu::inner_prod(Vel2 - Vel1, Pos0 - Pos1);
}

void FixedAngle::CalculateConPos()
{	
	assert(m_ConPos.size() == 1);
	const bnu::vector<double>& Pos0(m_pP0->GetPosition());
	const bnu::vector<double>& Pos1(m_pP1->GetPosition());
	const bnu::vector<double>& Pos2(m_pP2->GetPosition());
	m_ConPos[0] = bnu::inner_prod(Pos0 - Pos1, Pos2 - Pos1) - m_Angle;
}
