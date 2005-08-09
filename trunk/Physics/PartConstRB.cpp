//#include "StdAfx.h"
#include "./PartConstRB.hpp"


using namespace Edge;
using namespace boost;
using Edge::TDForceVector;
using Edge::TDParticleVector;
using namespace std;

PartConstRB::PartConstRB() :
	m_DampingConstant(0.5),
	m_StaticFriction(0.9),
	m_MovingFriction(0.8)
{	
}

PartConstRB::~PartConstRB(void)
{
}

void
PartConstRB::GetState(StateType& State)
{
	TDParticleVector::size_type n = m_Particles.size();	
	if (State.size() != n*2)
	{
		State.resize(n*2);
	}	
	for (TDParticleVector::size_type i = 0; i < n; ++i)
	{
		State[i*2] = m_Particles[i]->GetPosition();
		State[i*2+1] = m_Particles[i]->GetVelocity();
	}
}

void
PartConstRB::SetState(const StateType& State)
{
	vector<Edge::ParticlePtr>::size_type n = m_Particles.size();
	assert(State.size() == n*2);
	for (TDParticleVector::size_type i = 0; i < n; ++i)
	{
		m_Particles[i]->SetPosition(State[i*2]);
		m_Particles[i]->SetVelocity(State[i*2+1]);
	}	
}

void
PartConstRB::GetStateDerivative(StateType& DState) 
{
	//for each partcle work out the velocity and acceleration
	TDParticleVector::size_type n = m_Particles.size();
	if (DState.size() != n*2)
	{
		DState.resize(n*2);
		for (TDParticleVector::size_type i = 0; i < n*2; ++i)
			DState[i].resize(3);
	}

	CalculateDamping();
	CalculateCollisionForce();
	CalculateFrictionForce();
	bnu::vector<double> TotalForce(m_Forces.size()*3);
	for (TDParticleVector::size_type i = 0; i < m_Forces.size(); ++i)
	{
		double fx(m_Forces[i][0]);
		double fy(m_Forces[i][1]);
		double fz(m_Forces[i][2]);
		double fricx(m_Friction[i][0]);
		double fricy(m_Friction[i][1]);
		double fricz(m_Friction[i][2]);
		double dx(m_Damping[i][0]);
		double dy(m_Damping[i][1]);
		double dz(m_Damping[i][2]);
		double cx(m_Collision[i][0]);
		double cy(m_Collision[i][1]);
		double cz(m_Collision[i][2]);
		double tx(fx + dx + cx);
		double ty(fy + dy + cy);
		double tz(fz + dz + cz);
		//double tx(fx + dx + cx + fricx);
		//double ty(fy + dy + cy + fricy);
		//double tz(fz + dz + cz + fricz);
		/*double tx(fx + dx);
		double ty(fy + dy);
		double tz(fz + dz);*/
	
		TotalForce[i*3] = tx;
		TotalForce[i*3+1] = ty;
		TotalForce[i*3+2] = tz;
	}
	m_CS.SetForce(TotalForce);
	m_CS.CalculateConstraintForce();
	const bnu::vector<double>& CForce(m_CS.GetConstraintForce());
	for (TDParticleVector::size_type i = 0; i < n; ++i)
	{
		DState[i*2] = m_Particles[i]->GetVelocity();
		double d0(m_Damping[i][0]);
		double d1(m_Damping[i][1]);
		double d2(m_Damping[i][2]);
		//DState[i*2+1][0] = (TotalForce[i*3] + CForce[i*3] + DriftForce[i*3])/m_Particles[i]->GetMass();
		//DState[i*2+1][1] = (TotalForce[i*3+1] + CForce[i*3+1] + DriftForce[i*3+1])/m_Particles[i]->GetMass();
		//DState[i*2+1][2] = (TotalForce[i*3+2] + CForce[i*3+2] + DriftForce[i*3+2])/m_Particles[i]->GetMass();		
		DState[i*2+1][0] = (TotalForce[i*3] + CForce[i*3])/m_Particles[i]->GetMass();
		DState[i*2+1][1] = (TotalForce[i*3+1] + CForce[i*3+1])/m_Particles[i]->GetMass();
		DState[i*2+1][2] = (TotalForce[i*3+2] + CForce[i*3+2])/m_Particles[i]->GetMass();					
	}
}

void
PartConstRB::CalculateDamping()
{
	//calculate a damping forced proportial to the velocity
	assert(m_Damping.size() == m_Particles.size());
	TDForceVector::iterator it;
	TDParticleVector::iterator itPart;
	for (it = m_Damping.begin(), itPart = m_Particles.begin();
		it != m_Damping.end();
		++it, ++itPart)
	{
		const bnu::vector<double>& Vel((*itPart)->GetVelocity());
		double Len(bnu::norm_2(Vel));
		if (Len > 0.0)
		{
			double v0(Vel[0]);
			double v1(Vel[1]);
			double v2(Vel[2]);
			(*it)[0] = -m_DampingConstant * (Vel[0]/Len);
			(*it)[1] = -m_DampingConstant * (Vel[1]/Len);
			(*it)[2] = -m_DampingConstant * (Vel[2]/Len);
		}
	}
}

bnu::vector<double>
PartConstRB::GetForce(int Idx)
{
	return m_Forces[Idx];
}

void
PartConstRB::GetForce(int Idx, bnu::vector<double>& Force)
{
	Force = m_Forces[Idx];
}

void
PartConstRB::SetForceAllParticles(const bnu::vector<double>& Force)
{
	assert(m_Forces.size() == m_Particles.size());
	TDForceVector::size_type n = m_Forces.size();
	TDForceVector::size_type i = 0;
	for (i = 0; i < n; ++i)
	{
		m_Forces[i] = Force;
	}	
	m_CS.SetForceAllParticles(Force);
}

void
PartConstRB::SetParticle(int Idx, Edge::ParticlePtr pParticle)
{
	m_Particles[Idx] = pParticle;
	m_CS.SetParticle(Idx, pParticle);
}

void
PartConstRB::SetParticles(const Edge::TDParticleVector& Particles)
{
	m_Particles = Particles;
	m_CS.SetParticles(Particles);
}

void
PartConstRB::AddParticle(Edge::ParticlePtr pParticle)
{
	m_Particles.push_back(pParticle);
	m_CS.AddParticle(pParticle);
}

void
PartConstRB::ResizeForceContainer(TDForceVector::size_type Size)
{
	m_Forces.resize(Size);	
}

void
PartConstRB::ResizeDampingContainer(TDForceVector::size_type Size)
{
	m_Damping.resize(Size);
	TDForceVector::size_type i;
	for (i = 0; i < Size; ++i)
	{
		m_Damping[i].resize(3);
	}
}

void 
PartConstRB::ResizeFrictionForceContainer(TDForceVector::size_type Size)
{	
	m_Friction.resize(Size);
	TDForceVector::size_type i;
	for (i = 0; i < Size; ++i)
	{
		m_Friction[i].resize(3);
	}
}

void
PartConstRB::ResizeParticleContainer(TDForceVector::size_type Size)
{
	m_Particles.resize(Size);
	m_CS.ResizeParticles(Size);	
}

void
PartConstRB::BuildSystem()
{
	m_CS.ResizeDJacobian();
	m_CS.ResizeJacobian();
	m_CS.ResizeLambda();
	m_CS.ResizeConPos();
	m_CS.ResizeConVel();
}

void
PartConstRB::SetForce(int Idx, const bnu::vector<double>& Force)
{
	m_Forces[Idx] = Force;
	m_CS.SetForce(Idx, Force);
}

void
PartConstRB::SetDampingConstant(double Damping)
{
	m_DampingConstant = Damping;
}

TDForceVector::iterator 
PartConstRB::GetDampingBegin()
{
	return m_Damping.begin();
}
		
TDForceVector::iterator 
PartConstRB::GetDampingEnd()
{
	return m_Damping.end();
}

TDForceVector::const_iterator 
PartConstRB::GetDampingBegin() const
{
	return m_Damping.begin();
}

TDForceVector::const_iterator 
PartConstRB::GetDampingEnd() const
{
	return m_Damping.end();
}

bool PartConstRB::CheckForCollision(Edge::ParticlePtr pP, double dt, double& t)
{
	//check if particle will collide with ground plane at the next time step. Not exact.
	const bnu::vector<double>& Pos0(pP->GetPosition());
	bnu::vector<double> Pos1(Pos0 + (pP->GetVelocity()*dt));
	bnu::vector<double> Normal(3); //normal of plane perpendicular to y axis
	Normal[1] = 1;
	LineSegPlaneInt(Pos0, Pos1, Normal, -1, t);
	if (t >= 0.0 && t <= 1.0)
		return true;
	return false;
}

//TODO: Clean this shit up - checking for a collision shouldn't be hardcoded to plane.
void PartConstRB::CalculateCollisionForce()
{
	TDParticleVector::iterator itParticle;
	int i = 0;
	for (itParticle = m_Particles.begin(); itParticle != m_Particles.end(); ++itParticle, ++i)
	{
		Edge::ParticlePtr pP(*itParticle);
		double t;
		double dt = 0.01;
		double ks = 1000;
		if (CheckForCollision(pP, dt, t) || pP->GetPosition()[1] <= -1)
		{
			const bnu::vector<double>& Pos0(pP->GetPosition());
			bnu::vector<double> Pos1(Pos0 + (pP->GetVelocity()*dt));
			bnu::vector<double> Acceleration(3);
			const bnu::vector<double>& Velocity(pP->GetVelocity());
			bnu::vector<double> Normal(3);
			Normal[0] = 0; Normal[1] = 1; Normal[2] = 0; 
			double d = bnu::inner_prod(Pos1, Normal) + 1; //plane is at -1 y - get penetration depth
			//penalty force
			double fx(-ks*d*Normal[0]); 
			double fy(-ks*d*Normal[1]);
			double fz(-ks*d*Normal[2]);
			m_Collision[i][0] = fx;
			m_Collision[i][1] = fy;
			m_Collision[i][2] = fz;
		}
		else
		{
			m_Collision[i][0] = 0;
			m_Collision[i][1] = 0;
			m_Collision[i][2] = 0;
		}
	}
}

void PartConstRB::ResizeCollisionForceContainer(TDForceVector::size_type Size)
{
	m_Collision.resize(Size);
	for (TDParticleVector::size_type i = 0; i < Size; ++i)
	{
		m_Collision[i].resize(3);		
	}
}

//void
//PartConstRB::CalculateFrictionForce(const bnu::vector<double>& TotalForce)
//{
//	TDParticleVector::const_iterator itParticle;
//	TDForceVector::const_iterator itForce;
//	TDForceVector::iterator itFrictionForce;
//	int ForceIdx;
//	for (itParticle = m_Particles.begin(), ForceIdx=0, itFrictionForce = m_Friction.begin();
//		itParticle != m_Particles.end(); 
//		++itParticle, ++ForceIdx, ++itFrictionForce)
//	{
//		const bnu::vector<double>& Pos((*itParticle)->GetPosition());
//		const bnu::vector<double>& Vel((*itParticle)->GetVelocity());
//		bnu::vector<double> Force(3);
//		Force[0] = TotalForce[ForceIdx*3];
//		Force[1] = TotalForce[ForceIdx*3+1];
//		Force[2] = TotalForce[ForceIdx*3+2];
//		bnu::vector<double>& FrictionForce(*itFrictionForce);
//		bnu::vector<double> Normal(3);
//		Normal[0] = 0; Normal[1] = 1; Normal[2] = 0; 
//		double d = bnu::inner_prod(Pos, Normal) + 1; //plane is at -1 y 
//		//apply friction if particle is close enough to the ground plane
//		if (fabs(d) <= 0.001)
//		{
//			bnu::vector<double> FPerp(bnu::inner_prod(Normal, Force)*Normal);
//			bnu::vector<double> FPar(Force - FPerp);
//			bnu::vector<double> VelPerp(bnu::inner_prod(Normal, Vel)*Normal);
//			bnu::vector<double> VelPar(Vel - VelPerp);
//			double NormForceSize = bnu::inner_prod(m_StaticFriction*FPerp, m_StaticFriction*FPerp);
//			double PlaneForceSize = bnu::inner_prod(FPar, FPar);
//			//if the particles velocity is zero
//			if (fabs(bnu::inner_prod(Vel, Vel)) <= 0.0001)
//			{
//				if (NormForceSize > PlaneForceSize)
//				{
//					//stay still
//					FrictionForce = -FPar;
//				}
//				else
//				{
//					FrictionForce = -m_MovingFriction*FPerp;
//				}	
//			}
//			else 
//			{
//				FrictionForce = -bnu::norm_2(-m_MovingFriction*FPerp)*(VelPar/bnu::norm_2(VelPar));
//			}
//		}
//	}
//}

void
PartConstRB::CalculateFrictionForce()
{
	//assert(m_Forces.size() == m_Particles.size()*3);
	TDParticleVector::const_iterator itParticle;
	TDForceVector::const_iterator itForce;
	TDForceVector::iterator itFrictionForce;
	for (itParticle = m_Particles.begin(), itForce = m_Forces.begin(), itFrictionForce = m_Friction.begin();
		itParticle != m_Particles.end(); 
		++itParticle, ++itForce, ++itFrictionForce)
	{
		const bnu::vector<double>& Pos((*itParticle)->GetPosition());
		const bnu::vector<double>& Vel((*itParticle)->GetVelocity());
		const bnu::vector<double>& Force(*itForce);
		bnu::vector<double>& FrictionForce(*itFrictionForce);
		bnu::vector<double> Normal(3);
		Normal[0] = 0; Normal[1] = 1; Normal[2] = 0; 
		double d = bnu::inner_prod(Pos, Normal) + 1; //plane is at -1 y 
		//apply friction if particle is close enough to the ground plane
		if (fabs(d) <= 0.05)
		{
			bnu::vector<double> FPerp(bnu::inner_prod(Normal, Force)*Normal);
			bnu::vector<double> FPar(Force - FPerp);
			bnu::vector<double> VelPerp(bnu::inner_prod(Normal, Vel)*Normal);
			bnu::vector<double> VelPar(Vel - VelPerp);
			double NormForceSize = bnu::inner_prod(m_StaticFriction*FPerp, m_StaticFriction*FPerp);
			double PlaneForceSize = bnu::inner_prod(FPar, FPar);
			//if the particles velocity is zero
			if (fabs(bnu::inner_prod(Vel, Vel)) <= 0.0001)
			{
				if (NormForceSize > PlaneForceSize)
				{
					//stay still
					FrictionForce = -FPar;
				}
				else
				{
					//FrictionForce = -FPar;
					double Len(bnu::norm_2(FPar));
					if (Len > 1e-7)
						FrictionForce = -10.0*m_MovingFriction*bnu::norm_2(FPerp)*(FPar/Len);
				}	
			}
			else 
			{
				//f = -mu_k*vhat*|normal_force|				
				double Len(bnu::norm_2(VelPar));
				if (Len > 1e-7)
					FrictionForce = -10.0*m_MovingFriction*bnu::norm_2(FPerp)*(VelPar/bnu::norm_2(VelPar));
			}
		}
	}
}

