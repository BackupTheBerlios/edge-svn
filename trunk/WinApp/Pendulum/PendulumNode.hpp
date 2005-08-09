#pragma once

#include "../../Render/Render.hpp"
#include "../../Physics/Particle.hpp"
#include "../../Physics/Chain.hpp"

class PendulumNode :
	public Edge::DiffSystemNode
{
public:
	typedef boost::shared_ptr<PendulumNode> PendulumNodePtr;

	PendulumNode(void);
	virtual ~PendulumNode(void);	
	void SetParticle(int Idx, Edge::ParticlePtr pParticle);
	void PendulumNode::SetForce(int Idx, const bnu::vector<double>& Force);
	void ApplyVisitor(Edge::NodeVisitor* pVisitor);	
	void UpdateTranslateNodesFromState();
	void Setup(Edge::Chain::ChainPtr);
	void AddConnect(int, int);
	int GetNumTranslateNodes() {return m_TranslateNodes.size();}	
	const std::vector<int>& GetConnect() {return m_Connect;}

private:
	std::vector<int> m_Connect;
	std::vector<Edge::TranslatePtr> m_TranslateNodes;	
};
