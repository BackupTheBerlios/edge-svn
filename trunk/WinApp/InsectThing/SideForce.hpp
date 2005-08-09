#pragma once

#include "../../Physics/Particle.hpp"
#include "../../Physics/Force.hpp"

class SideForce
{
public:
	SideForce();
	virtual ~SideForce(void);

	void 
	CalcForce();
	
	const Edge::TDForce&
	GetForce();

	const SideForce& 
	operator =(const SideForce& SF);

	double
	GetAngle();

	void SetParticle0(Edge::ParticlePtr pParticle) {m_pP0 = pParticle;}
	void SetParticle1(Edge::ParticlePtr pParticle) {m_pP1 = pParticle;}
	void SetParticle2(Edge::ParticlePtr pParticle) {m_pP2 = pParticle;}

	double GetExtension() const;
	double GetMinExtension() const;
	double GetMaxExtension() const;

	void SetRestAngle(double a) {m_RestAngle = a;}

private:
	Edge::ParticlePtr m_pP0;
	Edge::ParticlePtr m_pP1;
	Edge::ParticlePtr m_pP2;
	Edge::TDForce m_Force;
	double m_RestAngle;
	double m_Stiffness;
};
