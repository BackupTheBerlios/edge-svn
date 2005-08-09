#include "StdAfx.h"
#include "../../Math/MatrixSupport.hpp"
#include "./SideForce.hpp"

using namespace Edge;
const double Pi = 3.14159265;

void 
SideForce::CalcForce()
{
	const bnu::vector<double>& P0(m_pP0->GetPosition());
	const bnu::vector<double>& P1(m_pP1->GetPosition());
	const bnu::vector<double>& P2(m_pP2->GetPosition());
	bnu::vector<double> u(P0 - P1);
	bnu::vector<double> v(P2 - P1);
	bnu::vector<double> uxv(3);
	u = u/bnu::norm_2(u);
	v = v/bnu::norm_2(v);
	Cross(u, v, uxv);
	bnu::vector<double> uxuxv(3);
	Cross(u, uxv, uxuxv);
	m_Force = uxuxv/bnu::norm_2(uxuxv);
	double CurAngle(GetAngle());
	m_Force = m_Stiffness*(m_RestAngle-CurAngle)*m_Force;
}

double
SideForce::GetExtension() const
{
	const bnu::vector<double>& Pos0(m_pP0->GetPosition());
	const bnu::vector<double>& Pos2(m_pP2->GetPosition());
	bnu::vector<double> Dir(Pos0 - Pos2);
	return bnu::norm_2(Dir);
}

double
SideForce::GetMaxExtension() const
{
	const bnu::vector<double>& Pos0(m_pP0->GetPosition());
	const bnu::vector<double>& Pos1(m_pP1->GetPosition());
	const bnu::vector<double>& Pos2(m_pP2->GetPosition());
	bnu::vector<double> Dir0(Pos0 - Pos1);
	bnu::vector<double> Dir1(Pos1 - Pos2);
	return bnu::norm_2(Dir0) + bnu::norm_2(Dir1);
}

double
SideForce::GetMinExtension() const
{
	double Max(GetMaxExtension());
	return Max - (0.2*Max);
}

const bnu::vector<double>&
SideForce::GetForce()
{
	return m_Force;
}

SideForce::SideForce():
	m_Force(3),
	m_RestAngle(Pi/2.0),
	m_Stiffness(10)
{
}

SideForce::~SideForce()
{

}

const SideForce& 
SideForce::operator =(const SideForce& SF)
{
	m_pP0 = SF.m_pP0;
	m_pP1 = SF.m_pP1;
	m_pP2 = SF.m_pP2;
	m_Force = SF.m_Force;
	m_RestAngle = SF.m_RestAngle;
	m_Stiffness = SF.m_Stiffness;
	return *this;
}

double
SideForce::GetAngle()
{
	const bnu::vector<double>& P0(m_pP0->GetPosition());
	const bnu::vector<double>& P1(m_pP1->GetPosition());
	const bnu::vector<double>& P2(m_pP2->GetPosition());
	bnu::vector<double> u(P0 - P1);
	bnu::vector<double> v(P2 - P1);
	u = u/bnu::norm_2(u);
	v = v/bnu::norm_2(v);
	return acos(bnu::inner_prod(u, v));
}

