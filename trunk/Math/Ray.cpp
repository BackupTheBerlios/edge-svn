//#include "StdAfx.h"
#include "./Ray.hpp"

using namespace Edge;

Ray::Ray(void)
{
}

Ray::~Ray(void)
{
}

const bnu::vector<double>& Ray::GetDirection() const
{
	return m_Direction;
}

const bnu::vector<double>& Ray::GetOrigin() const
{
	return m_Origin;
}

void Ray::SetDirection(const bnu::vector<double>& Dir)
{
	m_Direction = Dir;
}

void Ray::SetOrigin(const bnu::vector<double>& Origin)
{
	m_Origin = Origin;
}
