#pragma once
#include "./Node.hpp"
#include "./Translate.hpp"
#include "./Rotate.hpp"
#include "./Mesh.hpp"

namespace Edge
{
	class BoneNode :
		public Node
	{
	public:
		BoneNode(void);
		virtual ~BoneNode(void);
		
		void SetMesh(Edge::MeshPtr pMesh);		

		void SetTranslate(Edge::TranslatePtr pTranslate);
		
		virtual void ApplyVisitor(NodeVisitor* pVisitor);

	private:
		Edge::TranslatePtr m_pTranslate;
		Edge::MeshPtr m_pMesh;
	};
	typedef boost::shared_ptr<BoneNode> BoneNodePtr;
}