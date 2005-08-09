//#include "StdAfx.h"
#include "./PointLight.hpp"
#include "./RenderVisitor.hpp"

using namespace Edge;
using namespace boost;

PointLight::PointLight(int ID) :
	Light(ID),
	m_Position(3)
{
}

PointLight::~PointLight(void)
{
}

void PointLight::SetPosition(double x, double y, double z)
{
	m_Position[0] = x;
	m_Position[1] = y;
	m_Position[2] = z;
}

const bnu::vector<double>& PointLight::GetPosition() const
{
	return m_Position;
}

void PointLight::ApplyVisitor(Edge::NodeVisitor* pVisitor)
{
	PointLightPtr pThis(dynamic_pointer_cast<PointLight>(shared_from_this()));
	pVisitor->Visit(pThis);
	Node::ApplyVisitor(pVisitor);

	/*const RenderVisitor* pOGL = dynamic_cast<const RenderVisitor*>(pVisitor);
	if (pOGL)
	{
		PointLightPtr pThis(dynamic_pointer_cast<PointLight>(shared_from_this()));
		pOGL->SetLight(pThis);
		Node::ApplyVisitor(pVisitor);
	}
	else
	{
		Node::ApplyVisitor(pVisitor);
	}*/
}
