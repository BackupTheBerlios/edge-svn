//#include "StdAfx.h"
#include "./IdealGasSystem.hpp"

using namespace Edge;
using std::pair;

IdealGasSystem::IdealGasSystem(void) :
	m_R(8.31441417),
	m_T(1) 
{
}

IdealGasSystem::~IdealGasSystem(void)
{
}

void IdealGasSystem::AddParticle(Edge::ParticlePtr pP)
{
	m_Particles.push_back(pP);
}

void IdealGasSystem::CalculateForce()
{
	//calc the volume
	assert(m_Particles.size() == m_Normals.size());
	double Volume(CalcVolume());	
	TDParticleVector::iterator itParticle;
	TDNormalContainer::iterator itNormal;
	for (itParticle = m_Particles.begin(), itNormal = m_Normals.begin(); 
			itParticle != m_Particles.end(); 
			++itParticle, ++itNormal)
	{
		double P(1.0/Volume*m_R*m_T);
		TDParticleToForce::iterator itMap;
		itMap = m_PToF.find(*itParticle);
		bnu::vector<double> Normal(**itNormal);
		bnu::vector<double> PForce(P*Normal);
		itMap->second = PForce;
	}
}

void IdealGasSystem::SetNormalContainer(const TDNormalContainer& Normals)
{
	m_Normals = Normals;
}

double IdealGasSystem::CalcVolume()
{
	//use ellipse volume formula 4/3*rx*ry*rz*pi
	double MaxX(0);
	double MaxY(0);
	double MaxZ(0);
	double MinX(0);
	double MinY(0);
	double MinZ(0);
	TDParticleVector::iterator itParticle;
	for (itParticle = m_Particles.begin(); itParticle != m_Particles.end(); ++itParticle)
	{
		const bnu::vector<double>& Pos((*itParticle)->GetPosition());
		MaxX = (MaxX < Pos[0]) ? Pos[0] : MaxX;
		MaxY = (MaxY < Pos[1]) ? Pos[1] : MaxY;
		MaxZ = (MaxZ < Pos[2]) ? Pos[2] : MaxZ;
		MinX = (MinX > Pos[0]) ? Pos[0] : MinX;
		MinY = (MinY > Pos[1]) ? Pos[1] : MinX;
		MinZ = (MinZ > Pos[2]) ? Pos[2] : MinZ;
	}
	double rx(fabs(MaxX - MinX)*0.5);
	double ry(fabs(MaxY - MinY)*0.5);
	double rz(fabs(MaxZ - MinZ)*0.5);
	return 4.0/3.0*rx*ry*rz*Pi;
}

const IdealGasSystem::TDForce&
IdealGasSystem::GetForce(Edge::ParticlePtr pParticle) const
{
	return m_PToF.find(pParticle)->second;
}

void IdealGasSystem::ResizeParticleForceMap()
{
	TDParticleVector::const_iterator it;	
	for (it = m_Particles.begin(); it != m_Particles.end(); ++it)
	{
		Edge::ParticlePtr pP0(*it);
		pair < Edge::ParticlePtr, bnu::vector<double> > PToF;
		bnu::vector<double> F(3);
		F[0] = 0; F[1] = 0; F[2] = 0;
		m_PToF[pP0] = F;		
	}
}

void IdealGasSystem::SetParticleContainer(const TDParticleVector& Particles)
{
	m_Particles = Particles;
	ResizeParticleForceMap();
}

