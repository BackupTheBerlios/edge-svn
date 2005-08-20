//#include "StdAfx.h"
#include "./MatrixSupport.hpp"
#include <sstream>
#include <boost/numeric/ublas/matrix_proxy.hpp>

using namespace std;
using namespace Edge;
const double Pi = 3.14159265;

bnu::matrix<double> Edge::AxisAngle(const bnu::vector<double>& Axis, double Theta)
{
	//see pg 15 3d game engine design for formula to generate a rotation matrix 
	//from axis angle pair
//	assert(Theta >= 0.0);
	bnu::matrix<double> S(3,3);
	bnu::vector<double> NormAxis(Axis/bnu::norm_2(Axis));
	bnu::identity_matrix<double> I(3,3);
	//S[0][0] = 0;			S[0][1] = -NormAxis[2]; S[0][2] = NormAxis[1];
	//S[1][0] = NormAxis[2];	S[1][1] = 0;			S[1][2] = -NormAxis[0];
	//S[2][0] = -NormAxis[1];	S[2][1] = NormAxis[0];	S[2][2] = 0;

	S(0,0) = 0;			S(0,1) = -NormAxis[2]; S(0,2) = NormAxis[1];
	S(1,0) = NormAxis[2];	S(1,1) = 0;			S(1,2) = -NormAxis[0];
	S(2,0) = -NormAxis[1];	S(2,1) = NormAxis[0];	S(2,2) = 0;

	return I + (sin(Theta)*S) + ((1 - cos(Theta))*prod(S,S));
}

bool Edge::EqualTest(double x, double y, double Tolerance)
{
	return (fabs(x - y) <= Tolerance);
}

bool Edge::EqualTest(bnu::vector<double> x, bnu::vector<double> y, double Tolerance)
{
	if (x.size() != y.size())
		return false;
	int n = x.size();
	for (int i = 0; i < n; ++i)
	{
		if (!EqualTest(x[i], y[i], Tolerance))
			return false;
	}
	return true;
}

bool Edge::EqualTest(bnu::matrix<double> x, bnu::matrix<double> y, double Tolerance)
{
	if (x.size1() != y.size1())
		return false;
	if (x.size2() != y.size2())
		return false;
	int n = x.size1();	
	for (int i = 0; i < n; ++i)
	{
		bnu::matrix_row < bnu::matrix < double> > mc_x(x, i);
		bnu::matrix_row < bnu::matrix < double> > mc_y(y, i);
		if (!EqualTest(mc_x, mc_y, Tolerance))
			return false;
	}
	return true;
}

void Edge::BiCGSolve(const bnu::matrix<double> A, bnu::vector<double>& x, 
				   const bnu::vector<double>& b, bool& Breakdown)
{
	//Biconjugate Gradient solver
	//see numerical recipes in C http://www.library.cornell.edu/nr/bookcpdf.html section 2.7
	assert(A.size1() == A.size2()); //A must be square	
	bnu::zero_vector<double> zero(x.size());
	bnu::vector<double> r(zero);
	bnu::vector<double> rbar(zero);
	bnu::vector<double> p(zero);
	bnu::vector<double> pbar(zero);
	bnu::vector<double> Nextr(zero);
	bnu::vector<double> Nextrbar(zero);
	bnu::vector<double> Nextp(zero);
	bnu::vector<double> Nextpbar(zero);
	double alpha;
	double beta;
	int N = A.size1();		
	r = b;// - bnu::prod(A, x);
	rbar = r;
	p = r;
	pbar = rbar;
	if (EqualTest(A, bnu::zero_matrix<double>(A.size1(), A.size2()), 1e-15))
	{		
		Breakdown = true;
		return;
	}
	for (int i = 0; i < N; ++i)
	{		
		if (EqualTest(r, zero, 1e-1)) //no error therefore solution is correct
			break;
		double Temp1(bnu::inner_prod(bnu::prod(pbar, A), p));
		if (EqualTest(Temp1, 0, 1e-10))
		{
			//denominator is 0
			Breakdown = true;
			return;
		}
		alpha = bnu::inner_prod(rbar, r)/Temp1;
		Nextr = r - alpha*bnu::prod(A, p);
		Nextrbar = rbar - alpha*(bnu::prod(bnu::trans(A), pbar));
		double Temp2(bnu::inner_prod(rbar, r));
		if (EqualTest(Temp2, 0, 1e-10))
		{
			Breakdown = true;
			return;
		}
		beta = bnu::inner_prod(Nextrbar, Nextr)/Temp2;
		Nextp = Nextr + beta*p;
		Nextpbar = Nextrbar + beta*pbar;
		x = x + alpha*p;
		r = Nextr;
		rbar = Nextrbar;
		p = Nextp;
		pbar = Nextpbar;
	}
	Breakdown = false;
}

void Edge::VectorWrite(fstream& File, const bnu::vector<double>& v)
{
	int n = v.size();
	for (int i = 0; i < n; ++i)
	{
		/*strstream str;
		str << v[i] << ' ';
		File << str.str();*/
		File << v[i] << '\n';
	}
	File << "\n\n";
}

void Edge::MatrixWrite(fstream& File, const bnu::matrix<double>& M)
{
	int m = M.size1();
	int n = M.size2();
	for (int i = 0; i < m; ++i)
	{
		for (int j = 0; j < n; ++j)
		{
			File << M(i,j) << ' ';
		}
		File << "\n";
	}
	File << "\n\n";
}

void Edge::Cross(const bnu::vector<double>& u, const bnu::vector<double>& v, bnu::vector<double>& x)
{
	assert(u.size() == 3);
	assert(v.size() == 3);
	assert(x.size() == 3);
	x[0] = u[1]*v[2] - v[1]*u[2];
	x[1] = -u[0]*v[2] + v[0]*u[2];
	x[2] = u[0]*v[1] - v[0]*u[1];
}


double Edge::RadiansToDegrees(double Radians)
{
	return Radians*180.0/Pi;
}

double Edge::DegreesToRadians(double Degrees)
{
	return Degrees*Pi/180.0;
}

void Edge::Normalize(const bnu::vector<double>& u, bnu::vector<double>& Result)
{
	Result = u/bnu::norm_2(u);
}
