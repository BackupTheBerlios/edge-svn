#include "StdAfx.h"
#include "./SoftBodyNode.hpp"
#include "./SoftBodyController.hpp"
#include "./SoftBodyRenderer.hpp"
#include <boost/bind.hpp>

using namespace Edge;
using namespace boost;
using namespace boost::numeric;
using MassSpringSystem::TDPSPTuple;
using MassSpringSystem::TDPSPTupleContainer;
using namespace std;

SoftBodyNode::SoftBodyNode(void)
{
}

SoftBodyNode::~SoftBodyNode(void)
{
}

void SoftBodyNode::Setup(void)
{
	BuildSurface();
	SoftBodyController::SoftBodyControllerPtr pSBC(new SoftBodyController);
	bnu::vector<double> Force(3);
	Force[1] = -1;
	pSBC->BuildSystem(m_Vertices, m_Connect, m_Normals, Force);	
	SetDEStateSource(pSBC);
}

void SoftBodyNode::ApplyVisitor(Edge::NodeVisitor* pVisitor)
{
	SoftBodyRenderer* pRenderer = dynamic_cast<SoftBodyRenderer*>(pVisitor);
	if (pRenderer)
	{
		SoftBodyNodePtr pThis(boost::dynamic_pointer_cast<SoftBodyNode>(shared_from_this()));
		pRenderer->Draw(pThis);
	}
	DiffSystemNode::ApplyVisitor(pVisitor);
	UpdateVerticesFromState();
	CalcVertexNormals();
}

void SoftBodyNode::UpdateVerticesFromState()
{
	DEStateSource::DEStateSourcePtr pDE(GetDEStateSource());
	DEStateSource::StateType State;
	DEStateSource::StateType::iterator itState;
	pDE->GetState(State);
	TDVertexContainer::iterator itVertex;
	assert(m_Vertices.size() == State.size()/2);
	for (itVertex = m_Vertices.begin(), itState = State.begin(); 
		itVertex != m_Vertices.end(); 
		++itVertex, itState += 2)
	{
		*itVertex = *itState;
	}
}	

void SoftBodyNode::BuildSurface()
{
	//start with a set of vertices and recursively subdivide them	
	m_Vertices.resize(6);	
	for (int i = 0; i < 6; ++i)
	{
		m_Vertices[i].resize(3);
	}
	m_Vertices[0][0] = 0;		m_Vertices[0][1] = 1;		m_Vertices[0][2] = 0.5;
	m_Vertices[1][0] = 0.5;		m_Vertices[1][1] = 1;		m_Vertices[1][2] = 0;	
	m_Vertices[2][0] = 0;		m_Vertices[2][1] = 1.5;		m_Vertices[2][2] = 0;
	m_Vertices[3][0] = -0.5;	m_Vertices[3][1] = 1;		m_Vertices[3][2] = 0;
	m_Vertices[4][0] = 0;		m_Vertices[4][1] = 0.5;		m_Vertices[4][2] = 0;
	m_Vertices[5][0] = 0;		m_Vertices[5][1] = 1;		m_Vertices[5][2] = -0.5;	

	m_Connect.resize(8);
	for (int i = 0; i < 8; ++i)
	{
		m_Connect[i].resize(3);
	}	
	m_Connect[0][0] = 0; m_Connect[0][1] = 1; m_Connect[0][2] = 2;
	m_Connect[1][0] = 0; m_Connect[1][1] = 2; m_Connect[1][2] = 3;
	m_Connect[2][0] = 0; m_Connect[2][1] = 3; m_Connect[2][2] = 4;
	m_Connect[3][0] = 0; m_Connect[3][1] = 4; m_Connect[3][2] = 1;
	m_Connect[4][0] = 5; m_Connect[4][1] = 2; m_Connect[4][2] = 1;
	m_Connect[5][0] = 5; m_Connect[5][1] = 3; m_Connect[5][2] = 2;
	m_Connect[6][0] = 5; m_Connect[6][1] = 4; m_Connect[6][2] = 3;
	m_Connect[7][0] = 5; m_Connect[7][1] = 1; m_Connect[7][2] = 4;	
	
	int Steps=2;
	DivideSurface(Steps);
	ResizeNormals();
	CalcVertexNormals();
}

void SoftBodyNode::ResizeNormals()
{
	m_Normals.resize(m_Vertices.size());
	TDNormalContainer::iterator itNormal;
	for (itNormal = m_Normals.begin(); itNormal != m_Normals.end(); ++itNormal)
	{
		TDNormalPtr pN(new TDNormal(3));
		*itNormal = pN;
	}
}

void SoftBodyNode::CalcVertexNormals()
{
	assert(m_Normals.size() == m_Vertices.size());
	TDVertexContainer::iterator itVert;	
	TDNormalContainer::iterator itNormal;
	for (itVert = m_Vertices.begin(), itNormal = m_Normals.begin(); 
		itVert != m_Vertices.end(); 
		++itVert, ++itNormal)
	{
		TDNormal Normal(3);
		TDConnectContainer::iterator itFace;
		unsigned nIdx = m_Vertices.size() - (m_Vertices.end() - itVert);
		for (itFace = m_Connect.begin(); itFace != m_Connect.end(); ++itFace)
		{
			TDConnect::iterator itIdx;
			if ((itIdx = find(itFace->begin(), itFace->end(), nIdx)) != itFace->end()) 
			{
				const bnu::vector<double>& A(m_Vertices[*(itFace->begin())]);
				const bnu::vector<double>& B(m_Vertices[*(itFace->begin() + 1)]);
				const bnu::vector<double>& C(m_Vertices[*(itFace->begin() + 2)]);
				bnu::vector<double> AB(B - A);
				bnu::vector<double> AC(C - A);
				bnu::vector<double> ABCrossAC(3);
				Cross(AB, AC, ABCrossAC);
				Normal = Normal + ABCrossAC;
			}
		}		
		Normal = (1.0/bnu::norm_2(Normal)) * Normal;
		**itNormal = Normal;
	}
}


void SoftBodyNode::DivideTriangle(TDVertexContainer& Vertices, 
								TDConnectContainer& Connections, 
								TDConnectContainer::iterator itFace)
{
	TDConnect::iterator itIdx = itFace->begin();
	TDConnect::size_type nIdxA = *itIdx;
	TDConnect::size_type nIdxB = *(itIdx + 1);
	TDConnect::size_type nIdxC = *(itIdx + 2);
	TDConnect::size_type nIdxD = -1;
	TDConnect::size_type nIdxE = -1;
	TDConnect::size_type nIdxF = -1;
	TDVertex A(m_Vertices[nIdxA]);
	TDVertex B(m_Vertices[nIdxB]);
	TDVertex C(m_Vertices[nIdxC]);
	TDVertex AB(B-A);
	TDVertex AC(C-A);
	TDVertex BC(C-B);
	//TDVertex Normal(AB.Cross(AC));

	//normals of the edges - I don't know if this idea makes sense - I've never come
	//across an edge normal before in computer graphics literature. In this program I've defined the normal of
	//an edge to be the sum of the normals of the adjoining vertices. This is not true in general. 
	//It should be the sum of the normals of the adjoining faces, ie the faces that the edge border, 
	//but in this case because the kernel polyhedron is an octahedron, we may be able to get away with it.
	TDVertex NormalAB(*(m_Normals[nIdxA]) + *(m_Normals[nIdxB]));
	TDVertex NormalAC(*(m_Normals[nIdxA]) + *(m_Normals[nIdxC]));
	TDVertex NormalBC(*(m_Normals[nIdxB]) + *(m_Normals[nIdxC]));
		
	NormalAB = 1.0f/bnu::norm_2(NormalAB) * NormalAB;
	NormalAC = 1.0f/bnu::norm_2(NormalAC) * NormalAC;
	NormalBC = 1.0f/bnu::norm_2(NormalBC) * NormalBC;

	TDVertex MidAB(A + 0.5*AB);
	TDVertex MidAC(A + 0.5*AC);
	TDVertex MidBC(B + 0.5*BC);
	
	Sphere BoundSphere;
	bnu::vector<double> Center(3);
	Center[0] = 0.0; Center[1] = 1.0; Center[2] = 0.0;
	BoundSphere.SetCenter(Center);
	BoundSphere.SetRadius(0.5);
	
	Ray Ray;
	Ray.SetOrigin(MidAB);
	Ray.SetDirection(NormalAB);
	TDVertexContainer Points;	
	FindIntersection(Ray, BoundSphere, Points);
	assert(Points.size() == 1);
	TDVertex D(Points[0]);

	Ray.SetOrigin(MidBC);
	Ray.SetDirection(NormalBC);
	FindIntersection(Ray, BoundSphere, Points);
	assert(Points.size()== 1);
	TDVertex E(Points[0]);

	Ray.SetOrigin(MidAC);
	Ray.SetDirection(NormalAC);
	FindIntersection(Ray, BoundSphere, Points);
	assert(Points.size()== 1);
	TDVertex F(Points[0]);

	TDVertexContainer::iterator itVec;
	if ((itVec = find_if(Vertices.begin(), Vertices.end(), bind(Equal<TDVertex>(), _1, D))) == Vertices.end())
	{
		Vertices.push_back(D);
		nIdxD = Vertices.size() - 1;
	}
	else
	{
		nIdxD = Vertices.size() - (Vertices.end() - itVec);
	}
	if ((itVec = find_if(Vertices.begin(), Vertices.end(), bind(Equal<TDVertex>(), _1, E))) == Vertices.end())
	{
		Vertices.push_back(E);
		nIdxE = Vertices.size() - 1;
	}
	else
	{
		nIdxE = Vertices.size() - (Vertices.end() - itVec);
	}
	if ((itVec = find_if(Vertices.begin(), Vertices.end(), bind(Equal<TDVertex>(), _1, F))) == Vertices.end())
	{
		Vertices.push_back(F);
		nIdxF = Vertices.size() - 1;
	}
	else
	{
		nIdxF = Vertices.size() - (Vertices.end() - itVec);
	}
	
	vector<unsigned> vecFace;
	vecFace.push_back(nIdxA);
	vecFace.push_back(nIdxD);
	vecFace.push_back(nIdxF);
	Connections.push_back(vecFace);
	
	vecFace.clear();
	vecFace.push_back(nIdxD);
	vecFace.push_back(nIdxB);
	vecFace.push_back(nIdxE);
	Connections.push_back(vecFace);

	vecFace.clear();
	vecFace.push_back(nIdxF);
	vecFace.push_back(nIdxE);
	vecFace.push_back(nIdxC);
	Connections.push_back(vecFace);

	vecFace.clear();
	vecFace.push_back(nIdxD);
	vecFace.push_back(nIdxE);
	vecFace.push_back(nIdxF);
	Connections.push_back(vecFace);
}

void SoftBodyNode::DivideSurface(int nSteps)
{
	if (nSteps == 0)
		return;

	TDConnectContainer NewConnect;
	TDVertexContainer NewVertex;
	TDConnectContainer::iterator itFace;
	
	NewVertex.resize(m_Vertices.size());
	NewVertex.assign(m_Vertices.begin(), m_Vertices.end());
	ResizeNormals();
	CalcVertexNormals();
	for (itFace = m_Connect.begin(); itFace != m_Connect.end(); ++itFace)
	{
		DivideTriangle(NewVertex, NewConnect, itFace);
	}
	
	m_Connect.resize(NewConnect.size());
	m_Connect.assign(NewConnect.begin(), NewConnect.end());
	m_Vertices.resize(NewVertex.size());
	m_Vertices.assign(NewVertex.begin(), NewVertex.end());

	DivideSurface(--nSteps);
}

