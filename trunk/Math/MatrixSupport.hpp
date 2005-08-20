#pragma once

#include <boost/numeric/ublas/vector.hpp>
#include <boost/shared_ptr.hpp>
#include <boost/numeric/ublas/matrix.hpp>
#include <boost/numeric/ublas/matrix_sparse.hpp>
#include <fstream>

namespace bnu=boost::numeric::ublas;

namespace Edge
{
	typedef bnu::vector<double> TDNormal;
	typedef boost::shared_ptr<TDNormal> TDNormalPtr;

	/**
	Generate a matrix representing a rotation by the a given angle (in radians) about 
	a given axis.
	@param Axis The axis of rotation
	@param Theta The angle (in radians) to rotate
	*/
	bnu::matrix<double> AxisAngle(const bnu::vector<double>& Axis, double Theta);
	
	/**
	Solve a system Ax = b using the biconjugate gradient method. See 
	numerical recipes in C http://www.library.cornell.edu/nr/bookcpdf.html section 2.7
	for the algorithm. This is an example of an algorithm that is easy to implement but
	hard to understand why it works. The matrix A need not be positive definite or symmetric.
	@param A The matrix
	@param x the variable to solve for
	@param b the answer
	*/
	void BiCGSolve(const bnu::matrix<double> A, bnu::vector<double>& x, 
				 const bnu::vector<double>& b, bool& Breakdown);

	void Cross(const bnu::vector<double>& u, const bnu::vector<double>& v, bnu::vector<double>& x);

	void Normalize(const bnu::vector<double>& u, bnu::vector<double>& Result);

	void VectorWrite(std::fstream& File, const bnu::vector<double>& v);
	void MatrixWrite(std::fstream& File, const bnu::matrix<double>& M);

	bool EqualTest(double x, double y, double Tolerance);
	bool EqualTest(bnu::vector<double> x, bnu::vector<double> y, double Tolerance);
	bool EqualTest(bnu::matrix<double> x, bnu::matrix<double> y, double Tolerance);

	double RadiansToDegrees(double Radians);
	double DegreesToRadians(double Degrees);

	template<class Type>
	class Equal : 
		public std::binary_function<Type, Type, bool> 
	{
	public:
		typedef std::binary_function<Type, Type, bool> parent_type;

		typename parent_type::result_type 
		operator() (typename parent_type::first_argument_type a, typename parent_type::second_argument_type b)
		{
			return EqualTest(a, b, 1.0e-7);
		}
	};
};


