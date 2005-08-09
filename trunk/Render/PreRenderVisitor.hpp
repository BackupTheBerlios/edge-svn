#pragma once

#include "./NodeVisitor.hpp"
#include "./TextureMap.hpp"
#include <map>

namespace Edge
{
	/**
	Visitor class to traverse the scenegraph and do tasks that only need to be done once,
	like loading files, generating textures etc...
	*/
	class PreRenderVisitor :
		public NodeVisitor
	{
	public:
		PreRenderVisitor(void);
		virtual ~PreRenderVisitor(void);

		virtual void Visit(Edge::MeshPtr pMesh);
		virtual void Visit(Edge::TranslatePtr pTrans);
		virtual void Visit(Edge::ScalePtr pScale);
		virtual void Visit(Edge::RotatePtr pRotate);
		virtual void Visit(Edge::BoundNodePtr pBNode);						
		virtual void Visit(Edge::MaterialPtr pMat);
		virtual void Visit(Edge::DirectionLightPtr pL);		
		virtual void Visit(Edge::PointLightPtr pL);
		virtual void Visit(Edge::AmbientLightPtr pL);
		virtual void Visit(Edge::TextureMapPtr pT);
		
		virtual void Leave(Edge::MeshPtr pMesh);		
		virtual void Leave(Edge::TranslatePtr pTrans);
		virtual void Leave(Edge::ScalePtr pScale);
		virtual void Leave(Edge::RotatePtr pRotate);
		virtual void Leave(Edge::BoundNodePtr pBNode);
		virtual void Leave(Edge::MaterialPtr pMat);
		virtual void Leave(Edge::DirectionLightPtr pL);		
		virtual void Leave(Edge::PointLightPtr pL);
		virtual void Leave(Edge::AmbientLightPtr pL);
		virtual void Leave(Edge::TextureMapPtr pT);		

		void GenerateTexture(Edge::TextureMapPtr pTexMap);		
		void GetTextureToID(std::map<Edge::TextureMapPtr, GLuint>& TextureToID) {TextureToID = m_TextureToID;}
		void InitGL();

	private:
		std::map<Edge::TextureMapPtr, GLuint> m_TextureToID;
	};
}
