#pragma once

#include <boost/numeric/ublas/vector.hpp>
#include <boost/numeric/ublas/matrix.hpp>

namespace bnu = boost::numeric::ublas;

namespace Edge
{
	class Camera
	{
	public:
		Camera(void);
		virtual ~Camera(void);
		
		void SetPosition(const bnu::vector<double>& Pos);
		void SetAxes(const bnu::vector<double>& Left, const bnu::vector<double>& Up, const bnu::vector<double>& LookAt);
		void Translate(const bnu::vector<double>& T);
		void Translate(double dx, double dy, double dz);
		bnu::vector<double> GetDir();
		bnu::vector<double> GetUp();
		bnu::vector<double> GetLeft();
		bnu::vector<double> GetPostion();		
		void MoveForward();
		void MoveBackward();
		void TurnLeft();
		void TurnRight();
		void TurnUp();
		void TurnDown();
		void StrafeRight();
		void StrafeLeft();
		//ublas::matrix<double> CreateRotation(const ublas::vector<double>& Axis, double Angle);

	private:
		bnu::vector<double> m_Position;
		bnu::matrix<double> m_Axes;
		double m_FwdSpeed;
		double m_RotSpeed;
	};
}
