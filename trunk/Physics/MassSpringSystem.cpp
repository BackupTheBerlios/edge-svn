//#include "StdAfx.h"
#include <limits>
#include "./MassSpringSystem.hpp"

using namespace boost;
using std::numeric_limits;
using std::pair;
using namespace Edge;
using std::list;
using std::vector;

MassSpringSystem::MassSpringSystem() :
	m_Damping(10)
{
}

MassSpringSystem::MassSpringSystem(const TDPSPTupleContainer& PSPContainer) :
	m_PSPContainer(PSPContainer)
{
	ResizeParticleForceMap();
}


void MassSpringSystem::ZeroForcesOnParticles()
{
	MassSpringSystem::TDParticleToForce::iterator it;	
	bnu::vector<double> F(3);
	F[0] = 0; F[1] = 0; F[2] = 0;
	for (it = m_PToF.begin(); it != m_PToF.end(); ++it)
	{
		it->second = F;
	}
}

void MassSpringSystem::ResizeParticleForceMap()
{
	MassSpringSystem::TDPSPTupleContainer::const_iterator it;	
	for (it = m_PSPContainer.begin(); it != m_PSPContainer.end(); ++it)
	{
		Edge::ParticlePtr pP0(it->get<0>());
		Edge::ParticlePtr pP1(it->get<2>());
		pair < Edge::ParticlePtr, bnu::vector<double> > PToF;
		bnu::vector<double> F(3);
		F[0] = 0; F[1] = 0; F[2] = 0;
		m_PToF[pP0] = F;
		m_PToF[pP1] = F;
	}
}

MassSpringSystem::~MassSpringSystem(void)
{
}

int MassSpringSystem::CountParticles(void) const
{
	//Count the distinct particles	
	list<Edge::ParticlePtr> Found;	
	MassSpringSystem::TDPSPTupleContainer::const_iterator it;
	int Count = 0;
	for (it = m_PSPContainer.begin(); it != m_PSPContainer.end(); ++it)
	{
		if (find(Found.begin(), Found.end(), (*it).get<0>()) == Found.end())
		{
			Found.push_back((*it).get<0>());
			Count++;
		}
		if (find(Found.begin(), Found.end(), (*it).get<2>()) == Found.end())
		{
			Found.push_back((*it).get<2>());
			Count++;
		}
	}
	return Count;
}


void MassSpringSystem::CalculateForce()
{
	MassSpringSystem::TDPSPTupleContainer::iterator it;
	ZeroForcesOnParticles();
	//caculate the force on each particle
	for (it = m_PSPContainer.begin(); it != m_PSPContainer.end(); ++it)
	{
		Edge::ParticlePtr pP0(it->get<0>());
		Spring::SpringPtr pS(it->get<1>());
		Edge::ParticlePtr pP1(it->get<2>());
		//r = p1 - p0
		const bnu::vector<double>& Pos1(pP1->GetPosition());
		const bnu::vector<double>& Pos0(pP0->GetPosition());
		bnu::vector<double> r(Pos1 - Pos0);
		if (r[0] == 0 && r[1] == 0 && r[2] == 0)
			continue;
		//stretch = |r| - rest len
		double Stretch = (bnu::norm_2(r) - pS->GetRestLen());
		bnu::vector<double> dr(pP1->GetVelocity() - pP0->GetVelocity());
		//Damping = kd*((r.dr)/|r|))
		double Damping = m_Damping*(bnu::inner_prod(r, dr))/bnu::norm_2(r);
		//F = F + (ks*Stretch + Damping)*(r/|r|)
		MassSpringSystem::TDParticleToForce::iterator it;
		it = m_PToF.find(pP0);
		bnu::vector<double> F((pS->GetConstant()*Stretch + Damping)*(r/bnu::norm_2(r)));	
		bnu::vector<double>& F1(it->second);
		F1 += F;
		it = m_PToF.find(pP1);
		bnu::vector<double>& F2(it->second);
		F2 -= F;
	}
}

MassSpringSystem::TDParticleToForce::iterator MassSpringSystem::PToFBegin()
{
	return m_PToF.begin();
}

MassSpringSystem::TDParticleToForce::iterator MassSpringSystem::PToFEnd()
{
	return m_PToF.end();
}

MassSpringSystem::TDParticleToForce::const_iterator MassSpringSystem::PToFBegin() const
{
	return m_PToF.begin();
}

MassSpringSystem::TDParticleToForce::const_iterator MassSpringSystem::PToFEnd() const
{
	return m_PToF.end();
}

MassSpringSystem::TDPSPTupleContainer::iterator MassSpringSystem::PSPBegin()
{
	return m_PSPContainer.begin();
}

MassSpringSystem::TDPSPTupleContainer::iterator MassSpringSystem::PSPEnd()
{
	return m_PSPContainer.end();
}

MassSpringSystem::TDPSPTupleContainer::const_iterator MassSpringSystem::PSPBegin() const
{
	return m_PSPContainer.begin();
}

MassSpringSystem::TDPSPTupleContainer::const_iterator MassSpringSystem::PSPEnd() const
{
	return m_PSPContainer.end();
}

void MassSpringSystem::SetPSPContainer(const MassSpringSystem::TDPSPTupleContainer& Container)
{
	m_PSPContainer = Container;
	ResizeParticleForceMap();
}

const bnu::vector<double>& MassSpringSystem::GetForce(Edge::ParticlePtr pParticle) const
{
	assert(!m_PToF.empty());
	MassSpringSystem::TDParticleToForce::const_iterator it(m_PToF.find(pParticle));	
	assert(it != m_PToF.end());
	return it->second;
}

bool MassSpringSystem::IsParticleInSystem(const Edge::ParticlePtr pParticle)
{
	return m_PToF.find(pParticle) != m_PToF.end();
}
