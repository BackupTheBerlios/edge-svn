#include <vector>
#include <numeric>
#include <boost/test/unit_test.hpp>
#include <boost/test/floating_point_comparison.hpp>
#include <boost/graph/graph_traits.hpp>
#include <boost/graph/adjacency_list.hpp>
#include <boost/graph/property_iter_range.hpp>
#include "../../Render/SceneGraph.hpp"
#include "../../Math/MatrixSupport.hpp"
#include "../../Render/Translate.hpp"
#include "../../Render/Node.hpp"
#include "../../Render/OpenGLVisitor.hpp"

using namespace boost::unit_test_framework;
using namespace Edge;
using namespace std;

void SceneGraphTest()
{
	SceneGraph SG;
	Node::NodePtr pRoot(new Node);
	Translate::TranslatePtr pTr(new Translate);
	vector<SceneGraph::NodePairType> Pairs;	
	Pairs.push_back(SceneGraph::NodePairType(pRoot, pTr));
	SG.AddNodePairs(Pairs);
	BOOST_CHECK_EQUAL(SG.GetNumVertices(), 2);
	OpenGLVisitor OGLVis;
	SG.ApplyVisitor(OGLVis);	
}

test_suite* init_unit_test_suite( int argc, char* argv[] )
{
    test_suite* test= BOOST_TEST_SUITE("Render test" );
	test->add(BOOST_TEST_CASE(&SceneGraphTest));
    return test;
}
