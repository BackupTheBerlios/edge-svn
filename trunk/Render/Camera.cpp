//#include "StdAfx.h"
#include "./Camera.hpp"
#include "../Math/MatrixSupport.hpp"

using namespace Edge;
using namespace boost::numeric;

Camera::Camera(void) :
	m_Position(ublas::zero_vector<double>(3)),
	m_Axes(ublas::identity_matrix<double>(3)),
	m_FwdSpeed(0.2),
	m_RotSpeed(0.05)
{
	
}

Camera::~Camera(void)
{
}

void Camera::SetPosition(const ublas::vector<double>& Pos)
{
	m_Position = Pos;
}

void Camera::SetAxes(const ublas::vector<double>& Left, 
					 const ublas::vector<double>& Up, 
					 const ublas::vector<double>& Dir)
{
	m_Axes[0] = Left;
	m_Axes[1] = Up;
	m_Axes[2] = Dir;
}

void Camera::Translate(double dx, double dy, double dz)
{
	m_Position[0] += dx;
	m_Position[1] += dy;
	m_Position[2] += dz;
}

void Camera::Translate(const ublas::vector<double>& T)
{
	m_Position = m_Position + T;
}

ublas::vector<double> Camera::GetDir()
{
	return m_Axes[2];
}

ublas::vector<double> Camera::GetUp()
{
	return m_Axes[1];
}

ublas::vector<double> Camera::GetLeft()
{
	return m_Axes[0];
}

ublas::vector<double> Camera::GetPostion()
{
	return m_Position;
}
void Camera::MoveForward()
{
	m_Position += m_FwdSpeed*m_Axes[2];	
}

void Camera::MoveBackward()
{
	m_Position -= m_FwdSpeed*m_Axes[2];	
}

void Camera::TurnLeft()
{
	ublas::matrix<double> R(3,3);
	R = AxisAngle(m_Axes[1], m_RotSpeed);
	m_Axes[0] = ublas::prod(R, m_Axes[0]);
	m_Axes[2] = ublas::prod(R, m_Axes[2]);
}

void Camera::StrafeLeft()
{
	m_Position = m_Position + m_FwdSpeed*m_Axes[0];
}

void Camera::StrafeRight()
{
	m_Position = m_Position - m_FwdSpeed*m_Axes[0];
}

void Camera::TurnUp()
{		
	ublas::matrix<double> R(3,3);
	R = AxisAngle(m_Axes[0], m_RotSpeed);
	m_Axes[1] = ublas::prod(R, m_Axes[1]);
	m_Axes[2] = ublas::prod(R, m_Axes[2]);
}

void Camera::TurnDown()
{	
	ublas::matrix<double> R(3,3);
	R = AxisAngle(m_Axes[0], -m_RotSpeed);
	m_Axes[1] = ublas::prod(R, m_Axes[1]);
	m_Axes[2] = ublas::prod(R, m_Axes[2]);
}

void Camera::TurnRight()
{	
	ublas::matrix<double> R(3,3);
	R = AxisAngle(m_Axes[1], -m_RotSpeed);
	m_Axes[0] = ublas::prod(R, m_Axes[0]);
	m_Axes[2] = ublas::prod(R, m_Axes[2]);
}

