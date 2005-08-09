//#include "StdAfx.h"
#include "./SceneGraph.hpp"
#include "./Node.hpp"

using namespace std;
using namespace boost;
using namespace Edge;

SceneGraph::SceneGraph(void) 
{	
	
}

SceneGraph::~SceneGraph(void)
{
}


void SceneGraph::AddNodePairs(vector<NodePairType> NodePairs)
{
	int n = NodePairs.size();
	for (int i=0; i<n; ++i)
	{
		VDescType VDesc0, VDesc1;
		VNameMapType VNameMap;
		VDesc0 = add_vertex(m_Graph);
		VDesc1 = add_vertex(m_Graph);
		VNameMap = get(vertex_name, m_Graph);
		put(VNameMap, VDesc0, NodePairs[i].first);	
		put(VNameMap, VDesc1, NodePairs[i].second);
		add_edge(VDesc0, VDesc1, m_Graph);
	}
}

void SceneGraph::ApplyVisitor(OpenGLVisitor& OGLVisitor)
{
	//typedef adjacency_list < vecS, listS, directedS > graph_t;
	//graph_t g;
	//depth_first_search(g, visitor(OGLVisitor));
}
