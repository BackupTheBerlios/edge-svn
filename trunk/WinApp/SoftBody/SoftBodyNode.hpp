#pragma once

#include "../../Render/Render.hpp"
#include "../../Physics/MassSpringSystem.hpp"

class SoftBodyNode :
	public Edge::DiffSystemNode
{
public:
	typedef boost::shared_ptr<SoftBodyNode> SoftBodyNodePtr;
	typedef bnu::vector<double> TDVertex;
	typedef std::vector<unsigned> TDConnect;	
	typedef std::vector < TDVertex > TDVertexContainer;
	typedef std::vector < TDConnect > TDConnectContainer;
	typedef std::vector <Edge::TDNormalPtr> TDNormalContainer;
	typedef std::pair<TDConnect::value_type, TDConnect::value_type> TDEdgeIdx;

	SoftBodyNode(void);
	virtual ~SoftBodyNode(void);

	void ApplyVisitor(Edge::NodeVisitor* pVisitor);	
	void Setup();
	void UpdateVerticesFromState();
	void BuildSurface();
	void DivideTriangle(TDVertexContainer& Vertices, 
						TDConnectContainer& Connections, 
						TDConnectContainer::iterator itFace);
	void DivideSurface(int nSteps);

	const TDVertexContainer& GetVertices() {return m_Vertices;}
	const TDConnectContainer& GetConnect() {return m_Connect;}
	const TDNormalContainer& GetNormals() {return m_Normals;}
	void ResizeNormals();
	void CalcVertexNormals();
	void BuildMassSpringSystem(void);	

private:
	TDVertexContainer m_Vertices;	
	TDConnectContainer m_Connect;
	TDNormalContainer m_Normals;
	Edge::MassSpringSystem m_MS;
	
	class EdgeIdxCompare: 
		public std::binary_function<TDEdgeIdx, TDEdgeIdx, bool> 
	{
	public:
		result_type operator( ) (first_argument_type a, second_argument_type b)
		{
			return (a.first == b.first && a.second == b.second) || 
				(a.first == b.second && a.second == b.first);
		}
	};
};
