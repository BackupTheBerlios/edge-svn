#pragma once
#include "./NodeVisitor.hpp"
//#include "./Translate.hpp"
//#include "./Mesh.hpp"
//#include "./Material.hpp"
//#include "./DirectionLight.hpp"
//#include "./Rotate.hpp"
//#include "./PointLight.hpp"
//#include "./Scale.hpp"
//#include "./TextureMap.hpp"
//#include "./AmbientLight.hpp"
//#include "./BoundNode.hpp"
#include <map>


namespace Edge
{
	class RenderVisitor :
		public Edge::NodeVisitor
	{
	public:
		RenderVisitor(void);
		virtual ~RenderVisitor(void);
		
		virtual void Visit(Edge::MeshPtr pMesh);		
		virtual void Visit(Edge::TranslatePtr pTrans);
		virtual void Visit(Edge::ScalePtr pScale);
		virtual void Visit(Edge::RotatePtr pRotate);
		virtual void Visit(Edge::BoundNodePtr pBox);						
		virtual void Visit(Edge::MaterialPtr pMat);
		virtual void Visit(Edge::DirectionLightPtr pL);		
		virtual void Visit(Edge::PointLightPtr pL);
		virtual void Visit(Edge::AmbientLightPtr pL);
		virtual void Visit(Edge::TextureMapPtr pT);		

		virtual void Leave(Edge::MeshPtr pMesh);		
		virtual void Leave(Edge::TranslatePtr pTrans);
		virtual void Leave(Edge::ScalePtr pScale);
		virtual void Leave(Edge::RotatePtr pRotate);
		virtual void Leave(Edge::BoundNodePtr pBox);
		virtual void Leave(Edge::MaterialPtr pMat);
		virtual void Leave(Edge::DirectionLightPtr pL);		
		virtual void Leave(Edge::PointLightPtr pL);
		virtual void Leave(Edge::AmbientLightPtr pL);
		virtual void Leave(Edge::TextureMapPtr pT);		

		void SetTextureToID(const std::map<Edge::TextureMapPtr, GLuint>& TextureToID) {m_TextureToID = TextureToID;}

	private:
		static const int m_MaxLights = 8;
		bool m_LightSlots[m_MaxLights];
		std::map<Edge::TextureMapPtr, GLuint> m_TextureToID;
	};
}
