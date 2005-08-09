#ifdef _WIN32
#include <windows.h>
#endif

#include <GL/gl.h>
#include <GL/glu.h>
#include "./PreRenderVisitor.hpp"

using namespace Edge;

PreRenderVisitor::PreRenderVisitor(void)
{
	
}

PreRenderVisitor::~PreRenderVisitor(void)
{
}

void PreRenderVisitor::InitGL()
{
	glEnable(GL_NORMALIZE);
	glEnable(GL_LIGHTING);
	glEnable(GL_TEXTURE_2D);
	glEnable(GL_CULL_FACE);
	glCullFace(GL_CW);
	//glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
	glShadeModel(GL_SMOOTH);
	glEnable(GL_DEPTH_TEST);
	glDepthFunc(GL_LEQUAL);							
	glHint(GL_PERSPECTIVE_CORRECTION_HINT, GL_NICEST);
}

void 
PreRenderVisitor::Visit(TextureMapPtr pTexMap)
{
	GLuint ID;	
	glGenTextures(1, &ID);
	glBindTexture(GL_TEXTURE_2D, ID);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP);
	assert(GL_NO_ERROR == glGetError());
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
	int Width = pTexMap->GetWidth();
	int Height = pTexMap->GetHeight();
	const GLvoid* pData = static_cast<const GLvoid*>(pTexMap->GetDataPtr());
	/*glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, Width, Height,
		0, GL_RGBA, GL_UNSIGNED_BYTE, pData);*/
	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, Width, Height,
		0, GL_RGB, GL_UNSIGNED_BYTE, pData);	
	m_TextureToID[pTexMap] = ID;
	assert(GL_NO_ERROR == glGetError());
}

void
PreRenderVisitor::Leave(TextureMapPtr pTexMap)
{
}

void 
PreRenderVisitor::Visit(Edge::MeshPtr pMesh) 
{

}

void
PreRenderVisitor::Leave(MeshPtr pMesh)
{
}

void 
PreRenderVisitor::Visit(Edge::TranslatePtr pTrans) 
{

}

void
PreRenderVisitor::Leave(TranslatePtr pTrans)
{

}

void 
PreRenderVisitor::Visit(Edge::ScalePtr pScale) 
{

}

void
PreRenderVisitor::Leave(ScalePtr pScale)
{
}

void 
PreRenderVisitor::Visit(Edge::RotatePtr pRotate) 
{

}

void
PreRenderVisitor::Leave(RotatePtr pRotate)
{
}

void 
PreRenderVisitor::Visit(BoundNodePtr pBNode) 
{

}

void
PreRenderVisitor::Leave(BoundNodePtr pBNode)
{
}

void 
PreRenderVisitor::Visit(MaterialPtr pMat) 
{

}

void
PreRenderVisitor::Leave(MaterialPtr pMat)
{
}

void 
PreRenderVisitor::Visit(DirectionLightPtr pL) 
{

}

void
PreRenderVisitor::Leave(DirectionLightPtr pDLight)
{
}

void 
PreRenderVisitor::Visit(PointLightPtr pL) 
{

}

void
PreRenderVisitor::Leave(PointLightPtr pL)
{
}

void 
PreRenderVisitor::Visit(AmbientLightPtr pL) 
{

}

void
PreRenderVisitor::Leave(AmbientLightPtr pALight)
{

}

