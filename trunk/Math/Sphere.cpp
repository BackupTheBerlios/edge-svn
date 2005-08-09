//#include "StdAfx.h"
#include "./Sphere.hpp"

using namespace Edge;

Sphere::Sphere(void)
{
}

Sphere::~Sphere(void)
{
}

void Sphere::SetCenter(const bnu::vector<double>& Center)
{
	m_Center = Center;
}

void Sphere::SetRadius(const double& Radius)
{
	m_Radius = Radius;
}

double Sphere::GetRadius() const
{
	return m_Radius;
}

const bnu::vector<double>& Sphere::GetCenter() const
{
	return m_Center;
}
