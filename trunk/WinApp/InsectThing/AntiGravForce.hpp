#pragma once

#include "../../Physics/Particle.hpp"

class AntiGravForce
{
public:
	AntiGravForce();
	virtual ~AntiGravForce();

	void SetParticle(Edge::ParticlePtr pParticle);
	void SetIdealHeight(double h);
	void CalcForce();
	const bnu::vector<double>& GetForce();

	Edge::ParticlePtr m_pParticle;
	double m_IdealHeight;
	bnu::vector<double> m_Force;
};

