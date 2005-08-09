//#include "StdAfx.h"
#include "./Scale.hpp"
#include "./RenderVisitor.hpp"

using namespace Edge;
using namespace boost;

Scale::Scale(void) :
	m_Scale(3)
{
	m_Scale[0] = 1; m_Scale[1] = 1; m_Scale[2] = 1;
}

Scale::~Scale(void)
{
}

void Scale::ApplyVisitor(NodeVisitor* pVisitor)
{
	ScalePtr pThis(dynamic_pointer_cast<Scale>(shared_from_this()));
	pVisitor->Visit(pThis);
	Node::ApplyVisitor(pVisitor);
	pVisitor->Leave(pThis);

	/*const RenderVisitor* pOGL = dynamic_cast<const RenderVisitor*>(pVisitor);
	if (pOGL)
	{
		ScalePtr pThis(dynamic_pointer_cast<Scale>(shared_from_this()));
		pOGL->PushScale(pThis);
		Node::ApplyVisitor(pVisitor);
		pOGL->PopMatrix();
	}	
	else
	{
		Node::ApplyVisitor(pVisitor);
	}*/
}

void Scale::SetScale(double x, double y, double z)
{
	m_Scale[0] = x;
	m_Scale[1] = y;
	m_Scale[2] = z;
}

void Scale::GetScale(double& x, double& y, double& z) const
{
	x = m_Scale[0];
	y = m_Scale[1];
	z = m_Scale[2];
}
