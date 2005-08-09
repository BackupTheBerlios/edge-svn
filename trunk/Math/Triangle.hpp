#pragma once

#include <boost/numeric/ublas/vector.hpp>

namespace bnu=boost::numeric::ublas;


class Triangle
{
public:
	Triangle(void);
	virtual ~Triangle(void);
	
	bnu::vector<double> m_P0;
	bnu::vector<double> m_P1;
	bnu::vector<double> m_P2;
};
