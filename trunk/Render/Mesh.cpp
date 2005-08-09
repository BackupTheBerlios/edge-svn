//#include "StdAfx.h"
#include <fstream>
#include "./Mesh.hpp"
#include "./RenderVisitor.hpp"
#include "./Translate.hpp"
#include "./BoundsCalculator.hpp"

using namespace Edge;
using std::string;
using namespace boost::numeric;
using namespace boost;

Mesh::Mesh(void) :
	m_GoodNormals(false),
	m_GoodTexCoords(false)
{	
}

Mesh::~Mesh(void)
{
}

// load an *.obj file into the mesh
void Mesh::LoadObj(const std::string& FileName)
{	
	std::ifstream fWavefront;
	std::string strToken;
	m_Connect.erase(m_Connect.begin(), m_Connect.end());
	m_Vertices.erase(m_Vertices.begin(), m_Vertices.end());
	m_Normals.erase(m_Normals.begin(), m_Normals.end());
	fWavefront.open(FileName.c_str());
	while (fWavefront)
	{
		fWavefront >> strToken;
		if (strToken[0] == 'v' && strToken.length() == 1)
		{
			//read the floating point values that represent vertices
			ublas::vector<double> Temp(3);
			fWavefront >> Temp[0] >> Temp[1] >> Temp[2];
			m_Vertices.push_back(Temp);
		}
		if (strToken[0] == 'f' && strToken.length() == 1)
		{
			//read the face connect stuff
			std::vector<int> Temp(3); //assume triangulated for now
			std::string strDummy;
			Temp.resize(3);			
			fWavefront >> Temp[0] >> strDummy >> Temp[1] >> strDummy >> Temp[2];
			//subtract 1 because we like to use a 0 based index. 
			//obj uses a 1 based index. goddamn stupid bug got me.
			Temp[0] -= 1;
			Temp[1] -= 1;
			Temp[2] -= 1;
			m_Connect.push_back(Temp);
		}
		if (strToken == "vn")
		{
			//read vertex normal data
			ublas::vector<double> Temp(3);
			fWavefront >> Temp[0] >> Temp[1] >> Temp[2];
			m_Normals.push_back(Temp);
		}
	}
}

void Mesh::ApplyVisitor(NodeVisitor* pVisitor)
{	
	MeshPtr pThis(dynamic_pointer_cast<Mesh>(shared_from_this()));
	pVisitor->Visit(pThis);
	Node::ApplyVisitor(pVisitor);

	//const RenderVisitor* pOGL = dynamic_cast<const RenderVisitor*>(pVisitor);
	//if (pOGL)
	//{
	//	MeshPtr pThis(dynamic_pointer_cast<Mesh>(shared_from_this()));
	//	pOGL->DrawMesh(pThis);
	//	Node::ApplyVisitor(pVisitor);
	//}
	//else if (BoundsCalculator* pCalc = dynamic_cast<BoundsCalculator*>(pVisitor))
	//{
	//	const bnu::matrix<double>& Transform(pCalc->GetTransform());
	//	
	//	//find the minx, y z and max
	//	double MinX;
	//	double MinY;
	//	double MinZ;
	//	double MaxX;
	//	double MaxY;
	//	double MaxZ;
	//	TDVertexContainer::iterator itVertex;
	//	for (itVertex = m_Vertices.begin(); 
	//		itVertex != m_Vertices.end();
	//		++itVertex)
	//	{
	//		double x((*itVertex)[0]);
	//		double y((*itVertex)[1]);
	//		double z((*itVertex)[2]);
	//		
	//		if (itVertex == m_Vertices.begin())
	//		{
	//			//set them the 1st time
	//			MinX = x;
	//			MinY = y;
	//			MinZ = z;
	//			MaxX = x;
	//			MaxY = y;
	//			MaxZ = z;
	//		}
	//		else
	//		{
	//			MinX = (MinX < x) ? MinX:x;
	//			MinY = (MinY < y) ? MinY:y;
	//			MinZ = (MinZ < z) ? MinZ:z;
	//			MaxX = (MaxX > x) ? MaxX:x;
	//			MaxY = (MaxY > y) ? MaxY:y;
	//			MaxZ = (MaxZ > z) ? MaxZ:z;
	//		}
	//	}
	//	bnu::vector<double> Max(3);
	//	bnu::vector<double> Min(3);
	//	Max[0] = MaxX;
	//	Max[1] = MaxY;
	//	Max[2] = MaxZ;
	//	Min[0] = MinX;
	//	Min[1] = MinY;
	//	Min[2] = MinZ;
	//	Max = bnu::prod(Transform, Max);
	//	Min = bnu::prod(Transform, Min);
	//	BoundingBox::BoundingBoxPtr pCurrentBox(pCalc->GetBoundingBox());
	//	pCurrentBox->m_MaxX = (MaxX > pCurrentBox->m_MaxX) ? MaxX:pCurrentBox->m_MaxX;
	//	pCurrentBox->m_MaxY = (MaxY > pCurrentBox->m_MaxY) ? MaxY:pCurrentBox->m_MaxY;
	//	pCurrentBox->m_MaxZ = (MaxZ > pCurrentBox->m_MaxZ) ? MaxZ:pCurrentBox->m_MaxZ;
	//	pCurrentBox->m_MinX = (MinX < pCurrentBox->m_MinX) ? MinX:pCurrentBox->m_MinX;
	//	pCurrentBox->m_MinY = (MinY < pCurrentBox->m_MinY) ? MinY:pCurrentBox->m_MinY;
	//	pCurrentBox->m_MinZ = (MinZ < pCurrentBox->m_MinZ) ? MinZ:pCurrentBox->m_MinZ;
	//}
	//else
	//{
	//	Node::ApplyVisitor(pVisitor);
	//}
}

void Mesh::PushTexCoord(double u, double v)
{
	bnu::vector<double> temp(2);
	temp[0] = u;
	temp[1] = v;
	m_TexCoord.push_back(temp);
	m_GoodTexCoords = true;
}

void Mesh::PushTexCoord(const TexCoordType& t)
{
	m_TexCoord.push_back(t);
	m_GoodTexCoords = true;
}

void Mesh::PushConnect(const TDConnect& C) 
{
	assert(C.size() == 3); //only allow triangles
	m_Connect.push_back(C);
}

