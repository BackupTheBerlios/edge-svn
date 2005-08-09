#pragma once
#include "./NodeVisitor.hpp"
#include "./TextureMap.hpp"

class ChangeApplyMode :
	public Edge::NodeVisitor
{
public:
	ChangeApplyMode(Edge::TextureMap::ApplyMode AM);
	virtual ~ChangeApplyMode(void);

	virtual void Visit(Edge::MeshPtr pMesh);
	virtual void Visit(Edge::TranslatePtr pTrans);
	virtual void Visit(Edge::ScalePtr pScale);
	virtual void Visit(Edge::RotatePtr pRotate);
	virtual void Visit(Edge::BoundingBoxPtr pBox);						
	virtual void Visit(Edge::MaterialPtr pMat);
	virtual void Visit(Edge::DirectionLightPtr pL);		
	virtual void Visit(Edge::PointLightPtr pL);
	virtual void Visit(Edge::AmbientLightPtr pL);
	virtual void Visit(Edge::TextureMapPtr pT);		
	virtual void PopMatrix();

private:
	Edge::TextureMap::ApplyMode m_AM;
};
