//#include "StdAfx.h"
#include "./BoundsCalculator.hpp"
#include "../Math/MatrixSupport.hpp"

using namespace Edge;

BoundsCalculator::BoundsCalculator(void)
{	
	m_TransformStack.push_back(bnu::identity_matrix<double>(4));
	BoundingBoxPtr pBox(new BoundingBox);
	m_BoundingBoxes.push_back(pBox);
}

BoundsCalculator::~BoundsCalculator(void)
{
}

BoundingBoxPtr 
BoundsCalculator::GetBoundingBox()
{
	TDBoundingBoxContainer::iterator it;
	it = m_BoundingBoxes.end();
	it--;
	return *it;
}

const bnu::matrix<double>& 
BoundsCalculator::GetTransform()
{
	TDMatrixContainer::iterator it;
	it = m_TransformStack.end();
	it--;
	return *it;
}

void 
BoundsCalculator::Visit(Edge::MeshPtr pMesh)
{
	const bnu::matrix<double>& Transform(GetTransform());

	BoundingBoxPtr pCurrentBox(GetBoundingBox());
	//find the minx, y z and max
	double MinX;
	double MinY;
	double MinZ;
	double MaxX;
	double MaxY;
	double MaxZ;
	Mesh::TDVertexContainer::iterator itVertex;
	int size = m_TransformStack.size();
	for (itVertex = pMesh->GetBeginVertex(); 
		itVertex != pMesh->GetEndVertex();
		++itVertex)
	{
		double x((*itVertex)[0]);
		double y((*itVertex)[1]);
		double z((*itVertex)[2]);

		if (itVertex == pMesh->GetBeginVertex())
		{
			//set them the 1st time
			MinX = x;
			MinY = y;
			MinZ = z;
			MaxX = x;
			MaxY = y;
			MaxZ = z;
		}
		else
		{
			MinX = (MinX < x) ? MinX:x;
			MinY = (MinY < y) ? MinY:y;
			MinZ = (MinZ < z) ? MinZ:z;
			MaxX = (MaxX > x) ? MaxX:x;
			MaxY = (MaxY > y) ? MaxY:y;
			MaxZ = (MaxZ > z) ? MaxZ:z;
		}
	}
	bnu::vector<double> Max(4);
	bnu::vector<double> Min(4);
	Max[0] = MaxX;
	Max[1] = MaxY;
	Max[2] = MaxZ;
	Max[3] = 1;
	Min[0] = MinX;
	Min[1] = MinY;
	Min[2] = MinZ;
	Min[3] = 1;
	Max = bnu::prod(Transform, Max);
	Min = bnu::prod(Transform, Min);
//	if (pCurrentBox->m_SetOnce)
//	{
		pCurrentBox->m_MaxX = (Max[0] > pCurrentBox->m_MaxX) ? Max[0]:pCurrentBox->m_MaxX;
		pCurrentBox->m_MaxY = (Max[1] > pCurrentBox->m_MaxY) ? Max[1]:pCurrentBox->m_MaxY;
		pCurrentBox->m_MaxZ = (Max[2] > pCurrentBox->m_MaxZ) ? Max[2]:pCurrentBox->m_MaxZ;
		pCurrentBox->m_MinX = (Min[0] < pCurrentBox->m_MinX) ? Min[0]:pCurrentBox->m_MinX;
		pCurrentBox->m_MinY = (Min[1] < pCurrentBox->m_MinY) ? Min[1]:pCurrentBox->m_MinY;
		pCurrentBox->m_MinZ = (Min[2] < pCurrentBox->m_MinZ) ? Min[2]:pCurrentBox->m_MinZ;
	//}
	//else
	//{
	//	pCurrentBox->m_MaxX = Max[0];
	//	pCurrentBox->m_MaxY = Max[1];
	//	pCurrentBox->m_MaxZ = Max[2];
	//	pCurrentBox->m_MinX = Min[0];
	//	pCurrentBox->m_MinY = Min[1];
	//	pCurrentBox->m_MinZ = Min[2];
	//	pCurrentBox->m_SetOnce = true;
	//}

}

void
BoundsCalculator::Leave(MeshPtr pMesh)
{
}

void 
BoundsCalculator::Visit(Edge::TranslatePtr pTrans)
{
	bnu::matrix<double> Transform(bnu::identity_matrix<double>(4));
	const bnu::matrix<double>& CurTrans(GetTransform());
	double x, y, z;
	pTrans->GetTranslate(x, y, z);
	Transform(0, 3) = x;
	Transform(1, 3) = y;
	Transform(2, 3) = z;
	Transform = bnu::prod(CurTrans, Transform);
	m_TransformStack.push_back(Transform);
}

void
BoundsCalculator::Leave(TranslatePtr pTrans)
{
	m_TransformStack.pop_back();
}

void 
BoundsCalculator::Visit(Edge::ScalePtr pScale)
{
	bnu::matrix<double> Transform(bnu::identity_matrix<double>(4));
	const bnu::matrix<double>& CurTrans(GetTransform());
	double x, y, z;
	pScale->GetScale(x, y, z);
	Transform(0, 0) = x;
	Transform(1, 1) = y;
	Transform(2, 2) = z;
	Transform = bnu::prod(CurTrans, Transform);
	m_TransformStack.push_back(Transform);
}

void
BoundsCalculator::Leave(ScalePtr pScale)
{
	m_TransformStack.pop_back();
}

void 
BoundsCalculator::Visit(Edge::RotatePtr pRotate)
{
	bnu::matrix<double> MRotate(AxisAngle(pRotate->GetAxis(), pRotate->GetAngle()));
	//MRotate = bnu::trans(MRotate);
	bnu::matrix<double> Transform(bnu::identity_matrix<double>(4));
	const bnu::matrix<double>& CurTrans(GetTransform());
	Transform(0, 0) = MRotate(0, 0); Transform(0, 1) = MRotate(0, 1);	Transform(0, 2) = MRotate(0, 2);
	Transform(1, 0) = MRotate(1, 0); Transform(1, 1) = MRotate(1, 1);	Transform(1, 2) = MRotate(1, 2);
	Transform(2, 0) = MRotate(2, 0); Transform(2, 1) = MRotate(2, 1);	Transform(2, 2) = MRotate(2, 2);
	Transform = bnu::prod(CurTrans, Transform);
	m_TransformStack.push_back(Transform);
}

void
BoundsCalculator::Leave(RotatePtr pRotate)
{
	m_TransformStack.pop_back();
}


void 
BoundsCalculator::Visit(BoundNodePtr pBNode)
{
	//check if the current bboxes extents need updating 
	BoundingBoxPtr pBox(pBNode->GetBoundingBox());
	BoundingBoxPtr pCurrentBox(GetBoundingBox());
	pCurrentBox->m_MaxX = (pBox->m_MaxX > pCurrentBox->m_MaxX) ? pBox->m_MaxX:pCurrentBox->m_MaxX;
	pCurrentBox->m_MaxY = (pBox->m_MaxY > pCurrentBox->m_MaxY) ? pBox->m_MaxY:pCurrentBox->m_MaxY;
	pCurrentBox->m_MaxZ = (pBox->m_MaxZ > pCurrentBox->m_MaxZ) ? pBox->m_MaxZ:pCurrentBox->m_MaxZ;
	pCurrentBox->m_MinX = (pBox->m_MinX < pCurrentBox->m_MinX) ? pBox->m_MinX:pCurrentBox->m_MinX;
	pCurrentBox->m_MinY = (pBox->m_MinY < pCurrentBox->m_MinY) ? pBox->m_MinY:pCurrentBox->m_MinY;
	pCurrentBox->m_MinZ = (pBox->m_MinZ < pCurrentBox->m_MinZ) ? pBox->m_MinZ:pCurrentBox->m_MinZ;
	m_BoundingBoxes.push_back(pBox);
}

void
BoundsCalculator::Leave(BoundNodePtr pBNode)
{
	m_BoundingBoxes.pop_back();
}

void 
BoundsCalculator::Visit(MaterialPtr pMat)
{

}

void
BoundsCalculator::Leave(MaterialPtr pMat)
{
}


void 
BoundsCalculator::Visit(DirectionLightPtr pL)
{

}

void
BoundsCalculator::Leave(DirectionLightPtr pDLight)
{
}


void 
BoundsCalculator::Visit(PointLightPtr pL)
{

}

void
BoundsCalculator::Leave(PointLightPtr pL)
{
}


void 
BoundsCalculator::Visit(AmbientLightPtr pL)
{

}

void
BoundsCalculator::Leave(AmbientLightPtr pALight)
{

}


void 
BoundsCalculator::Visit(TextureMapPtr pT)
{
}

void
BoundsCalculator::Leave(TextureMapPtr pTexMap)
{
}
