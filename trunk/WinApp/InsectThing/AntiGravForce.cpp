#include "StdAfx.h"
#include "./AntiGravForce.hpp"

AntiGravForce::AntiGravForce(void) :
	m_Force(3),
	m_IdealHeight(0)
{
}

AntiGravForce::~AntiGravForce(void)
{
}

void 
AntiGravForce::SetParticle(Edge::ParticlePtr pParticle)
{
	m_pParticle = pParticle;
}

void 
AntiGravForce::SetIdealHeight(double h)
{
	m_IdealHeight = h;
}
	
void 
AntiGravForce::CalcForce()
{
	const bnu::vector<double>& Pos(m_pParticle->GetPosition());
	double Diff(Pos[1] - m_IdealHeight);
	m_Force[1] = -10*Diff;
}

const bnu::vector<double>& 
AntiGravForce::GetForce()
{
	return m_Force;
}
