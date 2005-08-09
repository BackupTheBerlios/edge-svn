#pragma once

namespace Edge
{
	class RGBColor
	{
	public:
		double m_Red, m_Green, m_Blue;
		RGBColor(double r = 0.5, double g = 0.5, double b = 0.5);
		~RGBColor(void);
	};
}
