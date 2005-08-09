#pragma once
#include "../../Render/Render.hpp"
#include "../../Physics/IdealGasSystem.hpp"
#include "./SoftBodyNode.hpp"

class SoftBodyController :
	public Edge::DEStateSource
{
public:
	typedef std::vector<Edge::ParticlePtr> TDParticleContainer;
	typedef std::vector< bnu::vector<double> > TDForceContainer;
	typedef boost::shared_ptr<SoftBodyController> SoftBodyControllerPtr;

	SoftBodyController(void);
	virtual ~SoftBodyController(void);	
	void BuildSystem(const SoftBodyNode::TDVertexContainer& Vertices, 
		const SoftBodyNode::TDConnectContainer& Connect,
		const SoftBodyNode::TDNormalContainer& Normals,
		const bnu::vector<double>& Force);
	void BuildMassSpringSystem(const SoftBodyNode::TDConnectContainer& Connect);
	virtual void SetState(const StateType& State);
	virtual void GetState(StateType& State);
	virtual void GetStateDerivative(StateType& StateDerivative);
	void CalculateCollisionForce();
	void ResizeCollisionForceContainer(TDForceContainer::size_type Size);
	bool CheckForCollision(Edge::ParticlePtr pP, double dt, double& t);
	void AddCollisionForces(const TDForceContainer& Force);
	void ZeroCollisionForces();

	TDParticleContainer::iterator GetBeginParticle() {return m_Particles.begin();}
	TDParticleContainer::const_iterator GetBeginParticle() const {return m_Particles.end();}
	TDParticleContainer::iterator GetEndParticle() {return m_Particles.end();}
	TDParticleContainer::const_iterator GetEndParticle() const {return m_Particles.end();}
	TDParticleContainer::size_type GetNumParticles() const {return m_Particles.size(); }

private:
	TDParticleContainer m_Particles;
	TDForceContainer m_Forces;
	TDForceContainer m_Collision; /**Collision force*/
	Edge::MassSpringSystem m_MS;
	Edge::IdealGasSystem m_IS;
};
