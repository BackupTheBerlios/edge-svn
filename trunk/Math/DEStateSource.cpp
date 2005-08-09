//#include "StdAfx.h"
#include "./DEStateSource.hpp"

using namespace Edge;

DEStateSource::DEStateSource(void) :
	m_Time(0)
{
}

DEStateSource::~DEStateSource(void)
{
}

void DEStateSource::IncTime(double dt)
{
	m_Time += dt;
}

double DEStateSource::GetTime() const
{
	return m_Time;
}

void DEStateSource::SetTime(double t)
{
	m_Time = t;
}
