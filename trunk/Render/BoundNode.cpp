//#include "StdAfx.h"
#include "./BoundNode.hpp"
#include "./BoundsCalculator.hpp"
#include "./BoundingBox.hpp"

using namespace Edge;
using namespace boost;

BoundNode::BoundNode(void) :
	m_pBox(new BoundingBox)
{
}

BoundNode::~BoundNode(void)
{
}

BoundingBoxPtr 
BoundNode::GetBoundingBox()
{
	return m_pBox;
}

void 
BoundNode::ApplyVisitor(Edge::NodeVisitor* pVisitor)
{
	BoundNodePtr pThis(dynamic_pointer_cast<BoundNode>(shared_from_this()));
	pVisitor->Visit(pThis);
	Node::ApplyVisitor(pVisitor);
	pVisitor->Leave(pThis);
}
