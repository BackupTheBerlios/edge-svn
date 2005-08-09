#pragma once
#include "../../Render/Render.hpp"

class DoublePenulumNode :
	public DiffSystemNode
{
public:
	DoublePenulumNode(void);
	virtual ~DoublePenulumNode(void);
	
	void ApplyVisitor(Edge::NodeVisitor* pVisitor);	
	void UpdateTranslateNodesFromState();
	void Setup(Edge::DoublePendulum::DoublePendulumPtr);
	int GetNumTranslateNodes() {return m_TranslateNodes.size();}

private:
	std::vector<Edge::Translate::TranslatePtr> m_TranslateNodes;

};
