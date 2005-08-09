#pragma once

#include "../../Render/Render.hpp"
#include "../../Physics/Physics.hpp"

class MassSpringNode :
	public Edge::DiffSystemNode
{
public:
	typedef boost::shared_ptr<MassSpringNode> MassSpringNodePtr;

	MassSpringNode(void);
	virtual ~MassSpringNode(void);
	void Setup(Edge::MassSpringSystem::MassSpringSystemPtr pMS);
	void ApplyVisitor(Edge::NodeVisitor* pVisitor);	
	void UpdateTranslateNodesFromState();

private:
	std::vector<Edge::Translate::TranslatePtr> m_TranslateNodes;
};
