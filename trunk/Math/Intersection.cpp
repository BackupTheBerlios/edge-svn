//#include "stdafx.h"
#include "./Intersection.hpp"
#include "./MatrixSupport.hpp"


using namespace Edge;
using namespace std;

bool Edge::LineSegPlaneInt(const bnu::vector<double>& P0, 
		const bnu::vector<double>& P1, 
		const bnu::vector<double>& Normal, 
		double d,
		double& t)
{
	double Denom(bnu::inner_prod(Normal, P1 - P0));
	if (fabs(Denom) >= 1e-5)
	{
		t = (d - bnu::inner_prod(Normal, P0))/Denom;
		if (t >= 0.0 && t <= 1.0)
			return true;
		return false;
	}
	return false;
}

void Edge::FindIntersection(const Ray& R, const Sphere& S, vector < bnu::vector<double> >& Points)
{	
	Points.clear();
	const double a(bnu::inner_prod(R.GetDirection(), R.GetDirection()));
	const double b(2*bnu::inner_prod(R.GetDirection(), (R.GetOrigin() - S.GetCenter())));
	const double c(bnu::inner_prod((R.GetOrigin() - S.GetCenter()),(R.GetOrigin() - S.GetCenter())) - S.GetRadius()*S.GetRadius());
	const double Discriminant(b*b - 4*a*c);
	double t0,t1;
	if (Discriminant < 0.0)
		return;
	if (fabs(Discriminant)  <= 1e-7)
	{
		//1 point of intersection
		t0 = -b/2*a;
		bnu::vector<double> Point;
		Point = R.GetOrigin() + t0*R.GetDirection();
		Points.push_back(Point);
		return;
	}
	t0 = (-b+sqrt((b*b)-(4*a*c)))/(2*a);
	t1 = (-b-sqrt((b*b)-(4*a*c)))/(2*a);
	//only allow +ve
	if (t0 >= 0.0)
	{
		bnu::vector<double> Point(R.GetOrigin() + t0*R.GetDirection());
		Points.push_back(Point);
	}
	if (t1 >= 0.0)
	{
		bnu::vector<double> Point(R.GetOrigin() + t1*R.GetDirection());	
		Points.push_back(Point);
	}
}

void 
Edge::FindIntersection(const LineSegment& LS, const Triangle& T, std::vector < bnu::vector<double> >& Points)
{
	Points.clear();
	bnu::vector<double> u(T.m_P0 - T.m_P1);
	bnu::vector<double> v(T.m_P2 - T.m_P1);
	bnu::vector<double> N(3);
	Cross(u, v, N);
	double d = bnu::inner_prod(T.m_P0, N);
	double t;
	bool Intersect = LineSegPlaneInt(LS.m_P0, LS.m_P1, N, d, t);
	if (Intersect)
	{
		bnu::vector<double> Dir(LS.m_P1 - LS.m_P0);
		bnu::vector<double> Pt(LS.m_P0 + t*Dir);
		bnu::vector<double> P0P1(T.m_P1 - T.m_P0);
		bnu::vector<double> P1P2(T.m_P2 - T.m_P1);
		bnu::vector<double> P2P0(T.m_P0 - T.m_P2);
		bnu::vector<double> P0Pt(Pt - T.m_P0);
		bnu::vector<double> P1Pt(Pt - T.m_P1);
		bnu::vector<double> P2Pt(Pt - T.m_P2);
		bnu::vector<double> C0(3);
		bnu::vector<double> C1(3);
		bnu::vector<double> C2(3);		
		Cross(P0P1, P0Pt, C0);
		Cross(P1P2, P1Pt, C1);
		Cross(P2P0, P2Pt, C2);
		Normalize(C0, C0);
		Normalize(C1, C1);
		Normalize(C2, C2);
		if (EqualTest(C0, C1, 1e-5) && EqualTest(C0, C2, 1e-5))		
		{
			Points.push_back(Pt);
		}
	}	
}
