#pragma once
#include "../../Render/Render.hpp"
#include "../../Physics/Physics.hpp"

class DoublePendulumNode :
	public Edge::DiffSystemNode
{
public:
	typedef boost::shared_ptr<DoublePendulumNode> DoublePendulumNodePtr;

	DoublePendulumNode(void);
	virtual ~DoublePendulumNode(void);
	
	void ApplyVisitor(Edge::NodeVisitor* pVisitor);	
	void UpdateTranslateNodesFromState();
	void Setup(Edge::DoublePendulum::DoublePendulumPtr);
	int GetNumTranslateNodes() {return m_TranslateNodes.size();}
	void ConvertAngularToCartesian(double& x0, double& y0, double& x1, double& y1);

private:
	std::vector<Edge::Translate::TranslatePtr> m_TranslateNodes;	
};
