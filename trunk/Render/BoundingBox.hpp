#pragma once

#include <boost/shared_ptr.hpp>

namespace Edge
{
	class BoundingBox
	{	
	public:
		BoundingBox(void);
		virtual ~BoundingBox(void);

		double m_MaxX;
		double m_MaxY;
		double m_MaxZ;
		double m_MinX;
		double m_MinY;
		double m_MinZ;
		bool m_SetOnce;
	};
	typedef boost::shared_ptr<BoundingBox> BoundingBoxPtr;
}
