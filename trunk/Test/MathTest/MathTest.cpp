#include <iostream>
#include <numeric>
#include <boost/test/unit_test.hpp>
#include <boost/test/floating_point_comparison.hpp>
#include "../../Math/Math.hpp"

using namespace std;
using namespace boost::unit_test_framework;
using namespace Edge;

void MatrixSupportTest()
{
	const double Pi = 3.141592;
	bnu::vector<double> v(3);
	bnu::matrix<double> m;
	v[0] = 0; v[1] = 1; v[2] = 0;
	m = AxisAngle(v, Pi/2.0);
	//if we rotate about the axis of rotation we expect the axis of rotation to remain unchanged
	v = bnu::prod(m,v);	
	BOOST_CHECK_CLOSE(v[0], 0.0, 1e-5);
	BOOST_CHECK_CLOSE(v[1], 1.0, 1e-5);
	BOOST_CHECK_CLOSE(v[2], 0.0, 1e-5);			
}

void RK4SolverTest()
{
	ExpDE Exp;	
	RK4Solver RK4;
	
	RK4.StepState(Exp, 1);	
	ExpDE::StateType State;
	Exp.GetState(State);
	//from Fundamentals of Differential Eqns, 4th ed, Nagle and Saaf, pg 133
	BOOST_CHECK_CLOSE(State[0][0], 2.70833, 1e-5);

	//from Fundamentals of Differential Eqns, 4th ed, Nagle and Saaf, pg 277
	DDy_3Dy_2y Eqn;
	double StepSize = 0.125;
	int NumSteps = 1.0/StepSize;	
	Eqn.GetState(State);
	BOOST_CHECK_CLOSE(State[0][0], 1.0, 1e-5);
	BOOST_CHECK_CLOSE(State[1][0], 1.0, 1e-5);
	for (int i = 0; i < NumSteps; i++)
	{
		RK4.StepState(Eqn, StepSize);
		Eqn.GetState(State);
		switch (i)
		{
		case 0:
			BOOST_CHECK_CLOSE(State[0][0], 1.08987, 1e-5);
			BOOST_CHECK_CLOSE(State[1][0], 0.46774, 1e-5);
			break;

		case 1:
			BOOST_CHECK_CLOSE(State[0][0], 1.12332, 1e-5);
			BOOST_CHECK_CLOSE(State[1][0], 0.08977, 1e-4); //fails if using 1e-5 since State[1][0] 0.0897676 which is too accurate
			break;
		
		case 2:
			BOOST_CHECK_CLOSE(State[0][0], 1.11711, 1e-5);
			BOOST_CHECK_CLOSE(State[1][0], -0.17235, 1e-4);
			break;

		}		
	}
	Eqn.GetState(State);	
	//check the final result
	BOOST_CHECK_CLOSE(State[0][0], 0.83295, 1e-5);
	BOOST_CHECK_CLOSE(State[1][0], -0.56226, 1e-5);
}

void EulerSolverTest()
{
	ExpDE Exp;
	EulerSolver E;

	E.StepState(Exp, 1);
	ExpDE::StateType State;	
	Exp.GetState(State);
	//from Fundamentals of Differential Eqns, 4th ed, Nagle and Saaf, pg 120
	BOOST_CHECK_CLOSE(State[0][0], 2.00000, 1e-5);

	double StepSize = 1e-1;
	int NumSteps = 1.0/StepSize;
	ExpDE Exp1;
	for (int i = 0; i < NumSteps; i++)
	{
		E.StepState(Exp1, StepSize);
	}
	Exp1.GetState(State);
	BOOST_CHECK_CLOSE(State[0][0], 2.59374, 1e-5);
}

void BiCGSolverTest()
{
	bnu::matrix<double> A(bnu::identity_matrix<double>(2));
	bnu::vector<double> b(2);
	bnu::vector<double> x(bnu::zero_vector<double>(2));	
	b[0] = 1;
	b[1] = 1;
	bool Breakdown;
	BiCGSolve(A, x, b, Breakdown); 
	//x should == b
	BOOST_CHECK_CLOSE(x[0], b[0], 1e-5);
	BOOST_CHECK_CLOSE(x[1], b[1], 1e-5);
	A(0,0) = 2; A(0,1) = 1; 
	A(1,0) = 1; A(1,1) = 2;
	x = bnu::zero_vector<double>(2);
	BiCGSolve(A, x, b, Breakdown);
	//x should be [1/3, 1/3]
	BOOST_CHECK_CLOSE(x[0], 0.333333, 1e-5);
	BOOST_CHECK_CLOSE(x[1], 0.333333, 1e-5);

	A.resize(4,4);
	A(0,0) = 1; A(0,1) = 2; A(0,2) = 0; A(0,3) = 5;
	A(1,0) = 2; A(1,1) = 0; A(1,2) = 1; A(1,3) = 2;
	A(2,0) = 0; A(2,1) = 1; A(2,2) = 0; A(2,3) = 0;
	A(3,0) = 5; A(3,1) = 2; A(3,2) = 0; A(3,3) = 0;
	b.resize(4);
	b[0] = 1; b[1] = 2; b[2] = 3; b[3] = 4;
	x.resize(4);
	x = bnu::zero_vector<double>(4);
	BiCGSolve(A, x, b, Breakdown);
	//solution from Mathematica, x = [-2/5, 3, 116/25, -23/25]
	BOOST_CHECK_CLOSE(x[0], -2.0/5.0, 1e-5);
	BOOST_CHECK_CLOSE(x[1], 3.0, 1e-5);
	BOOST_CHECK_CLOSE(x[2], 116.0/25.0, 1e-5);
	BOOST_CHECK_CLOSE(x[3], -23.0/25.0, 1e-5);

	A.resize(3,3);
	A(0,0) = 1; A(0,1) = 0; A(0,2) = 3;
	A(1,0) = 0; A(1,1) = 3; A(1,2) = 4;
	A(2,0) = 7; A(2,1) = 8; A(2,2) = 0;
	b.resize(3);
	b[0] = 2; b[1] = 3; b[2] = 3;
	x.resize(3);
	x = bnu::zero_vector<double>(3);
	BiCGSolve(A, x, b, Breakdown);
	BOOST_CHECK_CLOSE(x[0], 0.2, 1e-5);
	BOOST_CHECK_CLOSE(x[1], 0.2, 1e-5);
	BOOST_CHECK_CLOSE(x[2], 0.6, 1e-5);	

	//try a singular matrix
	A(0,0) = 1; A(0,1) = 0; A(0,2) = 3;
	A(1,0) = 0; A(1,1) = 3; A(1,2) = 4;
	A(2,0) = 0; A(2,1) = 0; A(2,2) = 0;
	x = bnu::zero_vector<double>(3);
	BiCGSolve(A, x, b, Breakdown);

}

void LineSegTriIntTest()
{
	LineSegment LS;
	Triangle T;
	LS.m_P0 = bnu::vector<double>(3);
	LS.m_P1 = bnu::vector<double>(3);
	LS.m_P0[0] = 0; LS.m_P0[1] = 0; LS.m_P0[2] = 0;
	LS.m_P1[0] = 1; LS.m_P1[1] = 0; LS.m_P1[2] = 0;
	T.m_P0 = bnu::vector<double>(3);
	T.m_P1 = bnu::vector<double>(3);
	T.m_P2 = bnu::vector<double>(3);
	T.m_P0[0] = -1; T.m_P0[1] = 1; T.m_P0[2] = 0;
	T.m_P1[0] = -1; T.m_P1[1] = 0; T.m_P1[2] = 1;
	T.m_P2[0] = -1; T.m_P2[1] = 0; T.m_P2[2] = -1;
	std::vector < bnu::vector<double> > Points;
	FindIntersection(LS, T, Points);
	BOOST_CHECK(Points.empty(), true);
	
	Points.clear();
	BOOST_CHECK(Points.empty(), true);
	T.m_P0[0] = 0.5; T.m_P0[1] = 1; T.m_P0[2] = 0;
	T.m_P1[0] = 0.5; T.m_P1[1] = -1; T.m_P1[2] = 1;
	T.m_P2[0] = 0.5; T.m_P2[1] = -1; T.m_P2[2] = -1;
	FindIntersection(LS, T, Points);
	BOOST_CHECK(Points.size(), 1);	
}

void IntersectionTest()
{
	bnu::vector<double> P0(3);
	bnu::vector<double> P1(3);
	bnu::vector<double> Normal(3);
	double d = 0;

	P0[0] = 0; P0[1] = 1; P0[2] = 0;
	P1[0] = 0; P1[1] = -1; P1[2] = 0;
	Normal[0] = 0; Normal[1] = 1; Normal[2] = 0;
	d = 0;
	double t = 0;
	LineSegPlaneInt(P0, P1, Normal, d, t);
	BOOST_CHECK_CLOSE(t, 0.5, 1e-5);
}

void CrossProductTest()
{
	bnu::vector<double> u(3);
	bnu::vector<double> v(3);
	bnu::vector<double> x(3);
	u[0] = 1; u[1] = 0; u[2] = 0;
	v[0] = 0; v[1] = 1; v[2] = 0;
	Cross(u, v, x);
	BOOST_CHECK_CLOSE(x[0], 0.0, 1e-5);
	BOOST_CHECK_CLOSE(x[1], 0.0, 1e-5);
	BOOST_CHECK_CLOSE(x[2], 1.0, 1e-5);

	u[0] = 1; u[1] = 0; u[2] = 3;
	v[0] = 1; v[1] = 2; v[2] = 3;
	Cross(u, v, x);
	BOOST_CHECK_CLOSE(x[0], -6.0, 1e-5);
	BOOST_CHECK_CLOSE(x[1], 0.0, 1e-5);
	BOOST_CHECK_CLOSE(x[2], 2.0, 1e-5);
}

void RaySphereIntersectionTest()
{
	Ray R;
	Sphere S;
	bnu::vector<double> Dir(3);
	bnu::vector<double> Origin(3);
	Dir[0] = 1; Dir[1] = 0; Dir[2] = 0;
	Origin[0] = 0; Origin[1] = 0; Origin[2] = 0;
	R.SetDirection(Dir);
	R.SetOrigin(Origin);
	S.SetCenter(Origin);
	S.SetRadius(1);
	vector< bnu::vector<double> > Points;
	FindIntersection(R, S, Points);
	BOOST_CHECK_EQUAL(Points.size(),1);
	BOOST_CHECK_CLOSE(Points[0][0], 1.0, 1e-5);
	BOOST_CHECK_CLOSE(Points[0][1], 0.0, 1e-5);
	BOOST_CHECK_CLOSE(Points[0][2], 0.0, 1e-5);

	//tangent
	Dir[0] = 1; Dir[1] = 0; Dir[2] = 0;
	Origin[0] = -3; Origin[1] = 1; Origin[2] = 0;
	R.SetDirection(Dir);
	R.SetOrigin(Origin);
	FindIntersection(R, S, Points);
	BOOST_CHECK_EQUAL(Points.size(),1);
	BOOST_CHECK_CLOSE(Points[0][0], 0.0, 1e-5);
	BOOST_CHECK_CLOSE(Points[0][1], 1.0, 1e-5);
	BOOST_CHECK_CLOSE(Points[0][2], 0.0, 1e-5);
}

test_suite* init_unit_test_suite( int argc, char* argv[] )
{
    test_suite* test= BOOST_TEST_SUITE("Render test" );
	test->add(BOOST_TEST_CASE(&MatrixSupportTest));
	test->add(BOOST_TEST_CASE(&RK4SolverTest));
	test->add(BOOST_TEST_CASE(&EulerSolverTest));
	test->add(BOOST_TEST_CASE(&BiCGSolverTest));
	test->add(BOOST_TEST_CASE(&IntersectionTest));	
	test->add(BOOST_TEST_CASE(&CrossProductTest));
	test->add(BOOST_TEST_CASE(&RaySphereIntersectionTest));
	test->add(BOOST_TEST_CASE(&LineSegTriIntTest));
    return test;
}
