//#include "StdAfx.h"
#include "./FixedPlane.hpp"

using namespace Edge;
using Edge::ParticlePtr;

FixedPlane::FixedPlane(const bnu::vector<double>& Normal, double Distance, Edge::ParticlePtr pP,
					   int MatrixCol, int NumParticles) :	
	m_MatrixCol(MatrixCol),
	m_Normal(Normal),
	m_Distance(Distance),
	m_pP(pP)
{
	ResizeJacobian(NumParticles);
	ResizeDJacobian(NumParticles);	
	ResizeConPos();
	ResizeConVel();
}

FixedPlane::~FixedPlane(void)
{
}

void FixedPlane::ResizeJacobian(int NumParticles)
{
	m_Jacobian = bnu::zero_matrix<double>(1, NumParticles*3);	
}

void FixedPlane::ResizeDJacobian(int NumParticles)
{
	m_DJacobian = bnu::zero_matrix<double>(1, NumParticles*3);
}

void FixedPlane::ResizeConPos()
{
	m_ConPos.resize(1);
}

void FixedPlane::ResizeConVel()
{
	m_ConVel.resize(1);
}

void FixedPlane::CalculateJacobian()
{
	//for a fixed plane contraint the result of dC/dq the transpose of the plane normal	
	bnu::matrix_range < bnu::sparse_matrix<double> > mr(m_Jacobian, 
		bnu::range(0, 1), //row range
			bnu::range(m_MatrixCol*3, m_MatrixCol*3+3)); //column range
	mr(0,0) = m_Normal[0];
	mr(0,1) = m_Normal[1];
	mr(0,2) = m_Normal[2];
}
	
void FixedPlane::CalculateDJacobian()
{	
}

int FixedPlane::GetRowSize() const
{
	return 1;
}

void FixedPlane::SetNormal(const bnu::vector<int>& Normal)
{
	m_Normal = Normal;
}

const bnu::vector<double>& FixedPlane::GetNormal()
{
	return m_Normal;
}

void FixedPlane::CalculateDriftForce()
{
	double d = bnu::inner_prod(m_Normal, m_pP->GetPosition()) - m_Distance;
	m_DriftForce = -(d*m_Normal + 0.5*m_pP->GetVelocity());
}

const bnu::vector<double>& FixedPlane::GetDriftForce(Edge::ParticlePtr pParticle) const
{
	return m_DriftForce;
}

bool FixedPlane::InConstraint(Edge::ParticlePtr pParticle) const 
{
	return  m_pP == pParticle;
}

void FixedPlane::CalculateConVel()
{
	m_ConVel[0] = bnu::inner_prod(m_Normal, m_pP->GetVelocity());
}

void FixedPlane::CalculateConPos()
{
	m_ConPos[0] = bnu::inner_prod(m_Normal, m_pP->GetPosition()) - m_Distance;
}
