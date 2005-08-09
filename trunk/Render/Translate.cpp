//#include "StdAfx.h"
#include "./Translate.hpp"
#include "./RenderVisitor.hpp"

using namespace Edge;
using namespace boost;
using namespace std;

Translate::Translate(void) :
	m_Translate(3)
{
}

Translate::~Translate(void)
{
}

void Translate::ApplyVisitor(NodeVisitor* pVisitor)
{
	TranslatePtr pThis(dynamic_pointer_cast<Translate>(shared_from_this()));
	pVisitor->Visit(pThis);
	Node::ApplyVisitor(pVisitor);
	pVisitor->Leave(pThis);
	/*const RenderVisitor* pOGL = dynamic_cast<const RenderVisitor*>(pVisitor);
	if (pOGL)
	{
		TranslatePtr pThis(dynamic_pointer_cast<Translate>(shared_from_this()));
		pOGL->PushTranslate(pThis);
		Node::ApplyVisitor(pVisitor);
		pOGL->PopMatrix();
	}	
	else
	{
		Node::ApplyVisitor(pVisitor);
	}*/
}

void Translate::SetTranslate(double x, double y, double z)
{
	m_Translate[0] = x;
	m_Translate[1] = y;
	m_Translate[2] = z;
}

void Translate::SetTranslate(bnu::vector<double> Trans)
{
	m_Translate = Trans;
}

void Translate::GetTranslate(double& x, double& y, double& z) const
{
	x = m_Translate[0];
	y = m_Translate[1];
	z = m_Translate[2];
}
