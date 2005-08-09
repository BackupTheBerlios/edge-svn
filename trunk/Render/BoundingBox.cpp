//#include "StdAfx.h"
#include "./BoundingBox.hpp"

using namespace Edge;

BoundingBox::BoundingBox(void) :
	m_MaxX(0),
	m_MaxY(0),
	m_MaxZ(0),
	m_MinX(0),
	m_MinY(0),
	m_MinZ(0),
	m_SetOnce(false)
{
}

BoundingBox::~BoundingBox(void)
{
}
