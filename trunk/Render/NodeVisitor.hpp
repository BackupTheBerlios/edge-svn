#pragma once

#include "./Mesh.hpp"
#include "./Translate.hpp"
#include "./Scale.hpp"
#include "./Rotate.hpp"
#include "./BoundNode.hpp"
#include "./Material.hpp"
#include "./DirectionLight.hpp"
#include "./PointLight.hpp"
#include "./AmbientLight.hpp"
#include "./TextureMap.hpp"

namespace Edge
{		
	class NodeVisitor
	{
	public:
		NodeVisitor(void);
		virtual ~NodeVisitor(void);

		//virtual void Visit(Edge::MeshPtr pMesh);
		virtual void Visit(Edge::MeshPtr pMesh) = 0;		
		virtual void Visit(Edge::TranslatePtr pTrans)= 0;
		virtual void Visit(Edge::ScalePtr pScale) = 0;
		virtual void Visit(Edge::RotatePtr pRotate) = 0;
		virtual void Visit(Edge::BoundNodePtr pBox) = 0;						
		virtual void Visit(Edge::MaterialPtr pMat) = 0;
		virtual void Visit(Edge::DirectionLightPtr pL) = 0;		
		virtual void Visit(Edge::PointLightPtr pL) = 0;
		virtual void Visit(Edge::AmbientLightPtr pL) = 0;
		virtual void Visit(Edge::TextureMapPtr pT) = 0;		

		virtual void Leave(Edge::MeshPtr pMesh) = 0;
		virtual void Leave(Edge::TranslatePtr pTrans)= 0;
		virtual void Leave(Edge::ScalePtr pScale) = 0;
		virtual void Leave(Edge::RotatePtr pRotate) = 0;
		virtual void Leave(Edge::BoundNodePtr pBox) = 0;
		virtual void Leave(Edge::MaterialPtr pMat) = 0;
		virtual void Leave(Edge::DirectionLightPtr pL) = 0;
		virtual void Leave(Edge::PointLightPtr pL) = 0;
		virtual void Leave(Edge::AmbientLightPtr pL) = 0;
		virtual void Leave(Edge::TextureMapPtr pT) = 0;
	};
}


