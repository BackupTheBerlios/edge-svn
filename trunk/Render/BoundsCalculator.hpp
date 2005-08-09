#pragma once
#include "./NodeVisitor.hpp"
#include "./BoundingBox.hpp"

namespace Edge
{
	class BoundsCalculator :
		public NodeVisitor
	{
	public:
		typedef boost::shared_ptr<BoundsCalculator> BoundsCalculatorPtr;
		typedef std::list< bnu::matrix<double> > TDMatrixContainer;
		typedef std::list<Edge::BoundingBoxPtr> TDBoundingBoxContainer;

		BoundsCalculator(void);
		virtual ~BoundsCalculator(void);
		
		void SetBoundingBox(Edge::BoundingBoxPtr pBox);
		Edge::BoundingBoxPtr GetBoundingBox(void);
		void PopTransform();
		void PushTransform(const bnu::matrix<double>& Matrix);
		void PushBox(Edge::BoundingBoxPtr pBox);
		void PopBox(Edge::BoundingBoxPtr pBox);
		const bnu::matrix<double>& GetTransform();

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


	private:
		TDMatrixContainer		m_TransformStack;
		TDBoundingBoxContainer	m_BoundingBoxes;
	};
}
