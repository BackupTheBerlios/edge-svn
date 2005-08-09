//#include "StdAfx.h"
#include "./DirectionLight.hpp"
#include "./RenderVisitor.hpp"

using namespace Edge;
using namespace boost;

DirectionLight::DirectionLight(int ID) :
	Light(ID),
	m_Direction(3)
{
}

DirectionLight::~DirectionLight(void)
{
}

void DirectionLight::SetDirection(double x, double y, double z)
{
	m_Direction[0] = x;
	m_Direction[1] = y;
	m_Direction[2] = z;
}

void DirectionLight::ApplyVisitor(Edge::NodeVisitor* pVisitor)
{
	DirectionLightPtr pThis(dynamic_pointer_cast<DirectionLight>(shared_from_this()));
	pVisitor->Visit(pThis);
	Node::ApplyVisitor(pVisitor);
}
