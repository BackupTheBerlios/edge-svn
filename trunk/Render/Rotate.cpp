//#include "StdAfx.h"
#include "./Rotate.hpp"
#include "./RenderVisitor.hpp"

using namespace Edge;
using namespace boost;

Rotate::Rotate(void) :
	m_Axis(3),
	m_Angle(0)

{
}

Rotate::~Rotate(void)
{
}

void Rotate::ApplyVisitor(NodeVisitor* pVisitor)
{
	RotatePtr pThis(dynamic_pointer_cast<Rotate>(shared_from_this()));
	pVisitor->Visit(pThis);
	Node::ApplyVisitor(pVisitor);
	pVisitor->Leave(pThis);

	/*const RenderVisitor* pOGL = dynamic_cast<const RenderVisitor*>(pVisitor);
	if (pOGL)
	{
		RotatePtr pThis(dynamic_pointer_cast<Rotate>(shared_from_this()));
		pOGL->PushRotate(pThis);
		Node::ApplyVisitor(pVisitor);
		pOGL->PopMatrix();
	}	
	else
	{
		Node::ApplyVisitor(pVisitor);
	}*/
}

void Rotate::SetAngleAxis(double Angle, const bnu::vector<double>& Axis)
{
	m_Angle = Angle;
	m_Axis = Axis;
}

void Rotate::SetAxis(double x, double y , double z)
{
	m_Axis[0] = x;
	m_Axis[1] = y;
	m_Axis[2] = z;
}
