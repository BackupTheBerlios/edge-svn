//#include "StdAfx.h"
#include "./Constraint.hpp"

using namespace Edge;
using namespace std;
using namespace boost::numeric;

Constraint::Constraint(void)
{
}

Constraint::~Constraint(void)
{
}

const bnu::sparse_matrix<double>& Constraint::GetDJacobian() const 
{
	return m_DJacobian;
}


const bnu::sparse_matrix<double>& Constraint::GetJacobian() const 
{
	return m_Jacobian;
}
