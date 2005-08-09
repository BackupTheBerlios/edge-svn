//#include "StdAfx.h"
#include "./Material.hpp"
#include "./RenderVisitor.hpp"

using namespace Edge;
using namespace boost;

Material::Material(void)
{
}

Material::~Material(void)
{
}

void Material::SetDiffuse(const RGBColor& Col)
{
	m_Diffuse = Col;
}

void Material::SetAmbient(const RGBColor& Col)
{
	m_Ambient = Col;
}

const RGBColor& Material::GetAmbient() const
{
	return m_Ambient;
}

const RGBColor& Material::GetDiffuse() const
{
	return m_Diffuse;
}

void Material::ApplyVisitor(Edge::NodeVisitor* pVisitor)
{
	MaterialPtr pThis(dynamic_pointer_cast<Material>(shared_from_this()));
	pVisitor->Visit(pThis);
	Node::ApplyVisitor(pVisitor);

	/*const RenderVisitor* pOGL = dynamic_cast<const RenderVisitor*>(pVisitor);
	if (pOGL)
	{
		MaterialPtr pThis(dynamic_pointer_cast<Material>(shared_from_this()));
		pOGL->SetMaterial(pThis);
		Node::ApplyVisitor(pVisitor);		
	}
	else
	{
		Node::ApplyVisitor(pVisitor);
	}*/
}
