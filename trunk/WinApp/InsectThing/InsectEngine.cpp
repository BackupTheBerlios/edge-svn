#include "StdAfx.h"
#include "../../Physics/MassSpringSystem.hpp"
#include "../../Physics/PartConstRB.hpp"
#include "./InsectEngine.hpp"
#include "./AntiGravForce.hpp"
#include <strstream>

using namespace std;
using std::pair;
using Edge::ParticlePtr;
using Edge::TDForce;
using Edge::TDParticleVector;
using Edge::MassSpringSystem::TDPSPTuple;
using Edge::MassSpringSystem::TDPSPTupleContainer;
using Edge::PartConstRB::StateType;
using InsectEngine::TDParticleSFMap;
using InsectEngine::TDParticleAGFMap;
using Edge::Spring;
using Edge::MassSpringSystem;

InsectEngine::InsectEngine(void) :
	m_IsStepping(true)
{
	
}

InsectEngine::~InsectEngine(void)
{
}

void
InsectEngine::SetupSpringForces(TDParticleVector& Particles)
{
	//leg set 0
	SideForce SF;
	SF.SetParticle0(Particles[0]);
	SF.SetParticle1(Particles[1]);
	SF.SetParticle2(Particles[2]);
	SF.SetRestAngle(SF.GetAngle()+Pi/8.0);
	pair<Edge::ParticlePtr, SideForce> SFKeyValPair;
	SFKeyValPair.first = Particles[0];
	SFKeyValPair.second = SF;
	m_ParticleSF.insert(SFKeyValPair);	
	SFKeyValPair.first = Particles[2];
	m_ParticleSF.insert(SFKeyValPair);	

	SF.SetParticle0(Particles[1]);
	SF.SetParticle1(Particles[2]);
	SF.SetParticle2(Particles[3]);
	SF.SetRestAngle(SF.GetAngle());
	SFKeyValPair.first = Particles[1];
	SFKeyValPair.second = SF;
	m_ParticleSF.insert(SFKeyValPair);	
	SFKeyValPair.first = Particles[3];
	m_ParticleSF.insert(SFKeyValPair);	

	SF.SetParticle0(Particles[4]);
	SF.SetParticle1(Particles[3]);
	SF.SetParticle2(Particles[2]);
	SF.SetRestAngle(SF.GetAngle());
	SFKeyValPair.first = Particles[4];
	SFKeyValPair.second = SF;
	m_ParticleSF.insert(SFKeyValPair);	
	SFKeyValPair.first = Particles[2];
	m_ParticleSF.insert(SFKeyValPair);	

	SF.SetParticle0(Particles[5]);
	SF.SetParticle1(Particles[4]);
	SF.SetParticle2(Particles[3]);
	SF.SetRestAngle(SF.GetAngle()+Pi/8.0);
	SFKeyValPair.first = Particles[5];
	SFKeyValPair.second = SF;
	m_ParticleSF.insert(SFKeyValPair);	
	SFKeyValPair.first = Particles[3];	
	m_ParticleSF.insert(SFKeyValPair);	

	//leg set 1
	SF.SetParticle0(Particles[6]);
	SF.SetParticle1(Particles[7]);
	SF.SetParticle2(Particles[8]);
	SF.SetRestAngle(SF.GetAngle()+Pi/8.0);
	SFKeyValPair.first = Particles[6];
	SFKeyValPair.second = SF;
	m_ParticleSF.insert(SFKeyValPair);	
	SFKeyValPair.first = Particles[8];
	m_ParticleSF.insert(SFKeyValPair);	
	
	SF.SetParticle0(Particles[7]);
	SF.SetParticle1(Particles[8]);
	SF.SetParticle2(Particles[9]);
	SF.SetRestAngle(SF.GetAngle());
	SFKeyValPair.first = Particles[7];
	SFKeyValPair.second = SF;
	m_ParticleSF.insert(SFKeyValPair);	
	SFKeyValPair.first = Particles[9];
	m_ParticleSF.insert(SFKeyValPair);	

	SF.SetParticle0(Particles[10]);
	SF.SetParticle1(Particles[9]);
	SF.SetParticle2(Particles[8]);
	SF.SetRestAngle(SF.GetAngle());
	SFKeyValPair.first = Particles[10];
	SFKeyValPair.second = SF;
	m_ParticleSF.insert(SFKeyValPair);	
	SFKeyValPair.first = Particles[8];
	m_ParticleSF.insert(SFKeyValPair);	

	SF.SetParticle0(Particles[11]);
	SF.SetParticle1(Particles[10]);
	SF.SetParticle2(Particles[9]);
	SF.SetRestAngle(SF.GetAngle()+Pi/8.0);
	SFKeyValPair.first = Particles[11];
	SFKeyValPair.second = SF;
	m_ParticleSF.insert(SFKeyValPair);	
	SFKeyValPair.first = Particles[9];
	m_ParticleSF.insert(SFKeyValPair);	

	//leg set 2
	SF.SetParticle0(Particles[12]);
	SF.SetParticle1(Particles[13]);
	SF.SetParticle2(Particles[14]);
	SF.SetRestAngle(SF.GetAngle()+Pi/8.0);
	SFKeyValPair.first = Particles[12];
	SFKeyValPair.second = SF;
	m_ParticleSF.insert(SFKeyValPair);	
	SFKeyValPair.first = Particles[14];
	m_ParticleSF.insert(SFKeyValPair);	

	SF.SetParticle0(Particles[13]);
	SF.SetParticle1(Particles[14]);
	SF.SetParticle2(Particles[15]);
	SF.SetRestAngle(SF.GetAngle());
	SFKeyValPair.first = Particles[13];
	SFKeyValPair.second = SF;
	m_ParticleSF.insert(SFKeyValPair);	
	SFKeyValPair.first = Particles[15];
	m_ParticleSF.insert(SFKeyValPair);	

	SF.SetParticle0(Particles[16]);
	SF.SetParticle1(Particles[15]);
	SF.SetParticle2(Particles[14]);
	SF.SetRestAngle(SF.GetAngle());
	SFKeyValPair.first = Particles[16];
	SFKeyValPair.second = SF;
	m_ParticleSF.insert(SFKeyValPair);
	SFKeyValPair.first = Particles[14];
	m_ParticleSF.insert(SFKeyValPair);	

	SF.SetParticle0(Particles[17]);
	SF.SetParticle1(Particles[16]);
	SF.SetParticle2(Particles[15]);
	SF.SetRestAngle(SF.GetAngle()+Pi/8.0);
	SFKeyValPair.first = Particles[17];
	SFKeyValPair.second = SF;
	m_ParticleSF.insert(SFKeyValPair);
	SFKeyValPair.first = Particles[15];
	m_ParticleSF.insert(SFKeyValPair);	
}

void
InsectEngine::Setup(TDParticleVector& Particles)
{
	SetupSpringForces(Particles);
	TDParticleVector::iterator itParticle;
	//m_SF.reserve(NumParticles);	
	pair<ParticlePtr, bool> ParticleStepPair;
	for (itParticle = Particles.begin();
		itParticle != Particles.end();
		++itParticle)
	{
		
		
	/*	pair<ParticlePtr, bool> ParticleStepPair;
		ParticleStepPair.first = *itParticle;
		ParticleStepPair.second = false;
		m_ParticleStep0.insert(ParticleStepPair);
		AntiGravForce AGF;
		TDForce Force(3);
		Force[1] = 1.5;
		AGF.m_Force = Force;
		AGF.m_pParticle = *itParticle;
		AGF.m_IdealHeight = (*itParticle)->GetPosition()[1];
		pair<Edge::ParticlePtr, AntiGravForce> KeyValPair;
		KeyValPair.first = *itParticle;
		KeyValPair.second = AGF;
		m_ParticleAGF.insert(KeyValPair);		*/
	}
}


TDParticleSFMap::size_type
InsectEngine::GetSFCount() const
{
	return m_ParticleSF.size();
}

/**
Calculate the direction of the force for each particle
thats in the insect.
*/
void
InsectEngine::CalcForce(Edge::ParticlePtr pP)
{
	TDParticleSFMap::iterator itCurrent(m_ParticleSF.lower_bound(pP));
	TDParticleSFMap::iterator itEnd(m_ParticleSF.upper_bound(pP));		
	while (itCurrent != itEnd)
	{
		SideForce& SF(itCurrent->second);
		SF.CalcForce();
		++itCurrent;
	}
	TDParticleAGFMap::iterator itAGF;
	itAGF = m_ParticleAGF.find(pP);
	if (itAGF != m_ParticleAGF.end())
	{
		itAGF->second.CalcForce();
	}
}

/**
Get the force on the given particle
*/
bnu::vector<double>
InsectEngine::GetForce(Edge::ParticlePtr pParticle)
{
	TDParticleSFMap::iterator itParticleSF;
	itParticleSF = m_ParticleSF.find(pParticle);
	bnu::vector<double> SFForce(3);
	TDParticleSFMap::iterator itCurrent(m_ParticleSF.lower_bound(pParticle));
	TDParticleSFMap::iterator itEnd(m_ParticleSF.upper_bound(pParticle));
	OutputDebugString("GetForceBegin\n");
	int i = 0;
	while (itCurrent != itEnd)
	{
		stringstream str;
		const bnu::vector<double>& Force(itCurrent->second.GetForce());
		str << Force[0] << " " << Force[1] << " " << Force[2] << "\n";
		OutputDebugString(str.str().c_str());		
		if (m_IsStepping)
		{
			stringstream str;
			str << i << "\n";
			OutputDebugString(str.str().c_str());
			SFForce = SFForce + itCurrent->second.GetForce();
		}
		++itCurrent;
		++i;
		
	}	
	OutputDebugString("GetForceEnd\n");
	
	TDParticleAGFMap::iterator itParticleAGF;
	bnu::vector<double> AGFForce(3);
	itParticleAGF = m_ParticleAGF.find(pParticle);
	TDParticleStepperMap::iterator itParticleStep;	
	if (itParticleAGF != m_ParticleAGF.end())
	{
		//cancel the anitgrav force if the particle is involved
		//in a step motion and the insect is stepping.
		itParticleStep = m_ParticleStep0.find(pParticle);
		AGFForce = itParticleAGF->second.GetForce();
		if (m_IsStepping && itParticleStep->second)
		{
			AGFForce = bnu::zero_vector<double>(3);
		}		
	}
	else
	{
		AGFForce = bnu::zero_vector<double>(3);
	}
	return SFForce;// + AGFForce;
}

void
InsectEngine::AddAntiGravForce(Edge::ParticlePtr pP, const AntiGravForce& AGF)
{
	pair<Edge::ParticlePtr, AntiGravForce> KeyValPair;
	KeyValPair.first = pP;
	KeyValPair.second = AGF;
	m_ParticleAGF.insert(KeyValPair);
}

void 
InsectEngine::AddSideForce(Edge::ParticlePtr pP0, const SideForce& SF)
{
	pair<Edge::ParticlePtr, SideForce> KeyValPair;
	KeyValPair.first = pP0;
	KeyValPair.second = SF;
	m_ParticleSF.insert(KeyValPair);
}

void
InsectEngine::SetStepping(bool b)
{
	m_IsStepping = b;
}
