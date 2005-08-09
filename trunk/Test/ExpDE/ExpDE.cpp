#include <boost/test/unit_test.hpp>
#include "../../Physics/ExpDE.hpp"
using namespace boost::unit_test_framework;
using namespace Edge;

void ExpDETest()
{
	//ExpDE models y' = y, y(0) = 1
	ExpDE Exp;	
	ExpDE::StateType State;
	//Check that after construction the initial state is 1
	//and that there is only one element in the state space.
	Exp.GetState(State);
	BOOST_CHECK_EQUAL(State.size(), 1);
	BOOST_CHECK_EQUAL(State[0][0], 1.0);

	//check that the state derivative is the same as the state
	ExpDE::StateType DState;
	Exp.GetStateDerivative(DState);
	BOOST_CHECK_EQUAL(DState.size(), 1);
	BOOST_CHECK_EQUAL(DState[0][0], State[0][0]);	
}

test_suite* init_unit_test_suite( int argc, char* argv[] )
{
    test_suite* test= BOOST_TEST_SUITE( "ExpDE test" );
	test->add(BOOST_TEST_CASE(&ExpDETest));
    return test;
}