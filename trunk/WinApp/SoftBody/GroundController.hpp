#pragma once

#include "../../Physics/Physics.hpp"
#include "../../Render/Render.hpp"


class GroundController :
	public Edge::DEStateSource
{
public:
	typedef std::vector<Edge::Particle::ParticlePtr> TDParticleContainer;
	typedef std::vector< bnu::vector<double> > TDForceContainer;
	typedef boost::shared_ptr<GroundController> GroundControllerPtr;

	GroundController(void);
	virtual ~GroundController(void);
	virtual void SetState(const StateType& State);
	virtual void GetState(StateType& State);
	virtual void GetStateDerivative(StateType& StateDerivative);

private:
	TDParticleContainer m_Particles;
};
