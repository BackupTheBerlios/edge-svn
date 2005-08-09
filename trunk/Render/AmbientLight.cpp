//#include "StdAfx.h"
#include "./AmbientLight.hpp"
#include "./RenderVisitor.hpp"

using namespace Edge;
using boost::dynamic_pointer_cast;

AmbientLight::AmbientLight(int ID) :
	Light(ID)
{
}

AmbientLight::~AmbientLight(void)
{
}

void AmbientLight::ApplyVisitor(NodeVisitor* pVisitor)
{
	AmbientLightPtr pThis(dynamic_pointer_cast<AmbientLight>(shared_from_this()));
	pVisitor->Visit(pThis);
	Node::ApplyVisitor(pVisitor);

	//const RenderVisitor* pOGL = dynamic_cast<const RenderVisitor*>(pVisitor);
	//if (pOGL)
	//{
	//	AmbientLightPtr pThis(dynamic_pointer_cast<AmbientLight>(shared_from_this()));
	//	//DirectionLightPtr pThis(enable_shared_from_this<DirectionLight>::shared_from_this());
	//	pOGL->SetLight(pThis);
	//	Node::ApplyVisitor(pVisitor);		
	//}
	//else
	//{
	//	Node::ApplyVisitor(pVisitor);
	//}
}
