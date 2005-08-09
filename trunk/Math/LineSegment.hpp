#pragma once

#include <boost/numeric/ublas/vector.hpp>
#include <boost/shared_ptr.hpp>
#include <boost/numeric/ublas/matrix.hpp>
#include <boost/numeric/ublas/matrix_sparse.hpp>

namespace bnu = boost::numeric::ublas;

namespace Edge
{

	class LineSegment
	{
	public:
		LineSegment(void);
		virtual ~LineSegment(void);

		bnu::vector<double> m_P0;
		bnu::vector<double> m_P1;
	};

}
