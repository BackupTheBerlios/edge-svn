#include "./TextureMap.hpp"
#include "./RenderVisitor.hpp"
#include "./PreRenderVisitor.hpp"
#include "./ChangeApplyMode.hpp"

using namespace Edge;
using namespace boost;

TextureMap::TextureMap(void) :
	m_AM(ApplyMode::AM_MODULATE)
{
}

TextureMap::~TextureMap(void)
{
}

bool TextureMap::LoadBMP(const std::string& Filename)
{	
	m_pBitmap = auxDIBImageLoad(Filename.c_str());
	if (!m_pBitmap) return false;
	return true;

}

void TextureMap::ApplyVisitor(Edge::NodeVisitor* pVisitor)
{
	TextureMapPtr pThis(dynamic_pointer_cast<TextureMap>(shared_from_this()));
	pVisitor->Visit(pThis);
	Node::ApplyVisitor(pVisitor);

	/*PreRenderVisitor* pOGLTexGen = dynamic_cast<PreRenderVisitor*>(pVisitor);
	RenderVisitor* pRV = dynamic_cast<RenderVisitor*>(pVisitor);
	ChangeApplyMode* pCAM = dynamic_cast<ChangeApplyMode*>(pVisitor);
	TextureMapPtr pThis(dynamic_pointer_cast<TextureMap>(shared_from_this()));
	if (pOGLTexGen)
	{	
		pOGLTexGen->GenerateTexture(pThis);
	}
	else if (pRV)
	{
		pRV->SetTextureMap(pThis);
	}
	else if (pCAM)
	{
		pCAM->SetApplyMode(pThis);
	}
	Node::ApplyVisitor(pVisitor);*/
}

void TextureMap::SetApplyMode(TextureMap::ApplyMode AM)
{
	m_AM = AM;
}