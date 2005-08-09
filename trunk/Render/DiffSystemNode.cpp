#include "./DiffSystemNode.hpp"
#include "./TimeStepVisitor.hpp"

using namespace Edge;
using namespace boost;

DiffSystemNode::DiffSystemNode(void)
{
}

DiffSystemNode::~DiffSystemNode(void)
{
}

void DiffSystemNode::ApplyVisitor(NodeVisitor* pVisitor)
{
	TimeStepVisitor<RK4Solver>* pTSV = dynamic_cast<TimeStepVisitor<RK4Solver>*>(pVisitor);	
	DiffSystemNodePtr pThis(dynamic_pointer_cast<DiffSystemNode>(shared_from_this()));
	if (pTSV)
	{
		pTSV->Step(pThis);
	}
	Node::ApplyVisitor(pVisitor);
}

