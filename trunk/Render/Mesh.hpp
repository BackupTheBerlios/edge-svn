#pragma once

#include <boost/numeric/ublas/vector.hpp>
#include "./Node.hpp"
#include "./RGBColor.hpp"
//#include "./NodeVisitor.hpp"

namespace bnu = boost::numeric::ublas;

namespace Edge
{	
	class Mesh :
		public Edge::Node
	{
	public:
		typedef std::vector < bnu::vector<double> > TDVertexContainer;
		typedef std::vector < bnu::vector<double> > NormalArrayType;
		typedef std::vector < bnu::vector<double> > TexCoordArrayType;
		typedef std::vector<RGBColor> ColorArrayType;
		typedef std::vector< std::vector<int> > TDConnectContainer;
		typedef bnu::vector<double> VertexType;
		typedef bnu::vector<double> NormalType;
		typedef bnu::vector<double> TexCoordType;
		typedef RGBColor ColorType;
		typedef std::vector<int> TDConnect;

		Mesh(void);
		virtual ~Mesh(void);
		
		const TDConnectContainer& GetConnect() const {return m_Connect;}
		const NormalArrayType& GetNormals() const {return m_Normals;}
		const TDVertexContainer& GetVertices() const {return m_Vertices;}		
		const TexCoordArrayType& GetTexCoords() const {return m_TexCoord;}
		TDVertexContainer::size_type NumVertices() {return m_Vertices.size();}
		void PushVertex(const VertexType& V) {m_Vertices.push_back(V);}
		void PushConnect(const TDConnect& C);
		void PushNormal(const NormalType& N) {m_Normals.push_back(N);}
		void PushTexCoord(double u, double v);
		void PushTexCoord(const TexCoordType& t);
		// load an *.obj file into the mesh
		void LoadObj(const std::string& FileName);
		bool GoodTexCoords() {return m_GoodTexCoords;}
		virtual void ApplyVisitor(NodeVisitor* pVisitor);

		TDVertexContainer::iterator GetBeginVertex() { return m_Vertices.begin(); }
		TDVertexContainer::iterator GetEndVertex() { return m_Vertices.end(); }
		TDVertexContainer::const_iterator GetBeginVertex() const { return m_Vertices.begin(); }
		TDVertexContainer::const_iterator GetEndVertex() const { return m_Vertices.end(); }

		TDConnectContainer::iterator GetBeginConnect() { return m_Connect.begin(); }
		TDConnectContainer::iterator GetEndConnect() { return m_Connect.end(); }
		TDConnectContainer::const_iterator GetBeginConnect() const { return m_Connect.begin(); }
		TDConnectContainer::const_iterator GetEndConnect() const { return m_Connect.end(); }


	private:		
		TDVertexContainer		m_Vertices;
		NormalArrayType			m_Normals;
		ColorArrayType			m_Color;
		TDConnectContainer		m_Connect;
		TexCoordArrayType		m_TexCoord;
		bool m_GoodNormals;
		bool m_GoodTexCoords;
	};

	typedef boost::shared_ptr<Mesh> MeshPtr;
}

