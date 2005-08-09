#include "./ChangeApplyMode.hpp"

using namespace Edge;

ChangeApplyMode::ChangeApplyMode(TextureMap::ApplyMode AM) :
	m_AM(AM)
{
}

ChangeApplyMode::~ChangeApplyMode(void)
{
}

void 
ChangeApplyMode::Visit(TextureMapPtr pTex)
{
	pTex->SetApplyMode(m_AM);
}

void 
ChangeApplyMode::Visit(MeshPtr pMesh)
{

}

void 
ChangeApplyMode::Visit(TranslatePtr pTrans)
{

}

void 
ChangeApplyMode::Visit(ScalePtr pScale)
{

}

void 
ChangeApplyMode::Visit(RotatePtr pRotate)
{

}

void 
ChangeApplyMode::Visit(BoundingBoxPtr pBox)
{

}

void 
ChangeApplyMode::Visit(MaterialPtr pMat)
{

}

void 
ChangeApplyMode::Visit(DirectionLightPtr pL)
{

}

void 
ChangeApplyMode::Visit(PointLightPtr pL)
{

}

void 
ChangeApplyMode::Visit(AmbientLightPtr pL)
{

}

void 
ChangeApplyMode::PopMatrix()
{

}
