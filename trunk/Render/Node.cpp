//#include "StdAfx.h"
#include "./Node.hpp"

using namespace Edge;
using namespace std;

Node::Node(void) :
	m_Name("")
{
}

Node::~Node(void)
{
}

void Node::SetParent(NodePtr pNode)
{
	m_pParent = pNode;
}


void Node::AddChild(NodePtr pChild)
{
	m_Children.push_back(pChild);
	pChild->m_pParent = shared_from_this();
}

void Node::RemoveChild(NodePtr pChild)
{
	m_Children.remove(pChild);
	pChild->m_pParent.reset();
}

void Node::ApplyVisitor(NodeVisitor* pVis)
{
	list<NodePtr>::iterator it;
	for (it = m_Children.begin(); it != m_Children.end(); ++it)
	{
		(*it)->ApplyVisitor(pVis);
	}
}

void Node::SetNumChildren(int n)
{
	m_Children.resize(n);
}
