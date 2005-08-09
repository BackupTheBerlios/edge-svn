#pragma once

#include "../../Physics/Particle.hpp"
#include "../../Math/Math.hpp"
#include "../../Physics/Force.hpp"
#include "./SideForce.hpp"

class AntiGravForce;

class InsectEngine
{
public:
	typedef boost::shared_ptr<InsectEngine> InsectEnginePtr;	
	typedef double TDAngle;
	typedef std::list<TDAngle> TDAngleContainer;
	typedef std::list<double> TDSpringConstContainer;
	typedef std::multimap<Edge::ParticlePtr, SideForce> TDParticleSFMap;
	typedef std::map<Edge::ParticlePtr, AntiGravForce> TDParticleAGFMap;
	typedef std::map<Edge::ParticlePtr, bool> TDParticleStepperMap;

	enum
	{
		EnumParticle,
		EnumSpring,
		EnumTarget
	};

	InsectEngine(void);
	virtual ~InsectEngine(void);

	void 
	Setup(Edge::TDParticleVector& Particles);

	void
	CalcForce(Edge::ParticlePtr pP);
	
	bnu::vector<double> 
	GetForce(Edge::ParticlePtr pParticle);

	void 
	SetParticles(const Edge::TDParticleVector& Particles);
	
	void 
	AddSideForce(Edge::ParticlePtr pP0, const SideForce& SF);

	void
	AddAntiGravForce(Edge::ParticlePtr pP, const AntiGravForce& AGF);

	void 
	ChangeAngDisp(Edge::ParticlePtr pP0, TDAngle Disp);

	TDParticleSFMap::size_type 
	GetSFCount() const;

	void
	SetConstant(Edge::ParticlePtr pP, double Const);

	void
	SetStepping(bool b);

	void
	SetupSpringForces(Edge::TDParticleVector& Particles);

private:
	bool m_IsStepping;
	Edge::TDParticleVector m_Particles;
	Edge::TDForceVector m_Forces;
	TDParticleSFMap m_ParticleSF;
	TDParticleAGFMap m_ParticleAGF;
	TDParticleStepperMap m_ParticleStep0;
	TDParticleStepperMap m_ParticleStep1;
	Edge::MassSpringSystem m_Springs;
};
