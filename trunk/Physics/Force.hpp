#pragma once

#include <vector>
#include <list>
#include <boost/numeric/ublas/vector.hpp>
#include <boost/numeric/ublas/matrix.hpp>
#include <boost/numeric/ublas/triangular.hpp>
#include <boost/numeric/ublas/io.hpp>
#include <boost/shared_ptr.hpp>

namespace bnu=boost::numeric::ublas;

namespace Edge
{
	//these typedefs will do for now...might make a class later
	typedef bnu::vector<double> TDForce;
	typedef std::vector< bnu::vector<double> > TDForceVector;
	typedef std::list< bnu::vector<double> > TDForceList;
}