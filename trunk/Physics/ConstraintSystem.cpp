#include "./ConstraintSystem.hpp"
#include "./FixedDist.hpp"
#include "./FixedPosition.hpp"
#include "../Math/MatrixSupport.hpp"
#include <boost/numeric/ublas/vector_proxy.hpp>
#include <boost/numeric/ublas/matrix_proxy.hpp>

using namespace std;
using namespace Edge;
using Edge::ConstraintSystem;

ConstraintSystem::ConstraintSystem(void) :
	m_MassInvCalculated(false),
	m_MassCalculated(false),
	m_ks(20),
	m_kd(20)
{	
}

ConstraintSystem::~ConstraintSystem(void)
{
}

void ConstraintSystem::AddConstraint(ConstraintPtr pCont)
{
	m_Constraints.push_back(pCont);
}

void ConstraintSystem::AddParticle(ParticlePtr pP)
{
	m_Particles.push_back(pP);	
}

void ConstraintSystem::SetConstraint(int Idx, ConstraintPtr pCons)
{
	assert(Idx < (int)m_Constraints.size());
	assert(Idx > -1);
	m_Constraints[Idx] = pCons;
}

void ConstraintSystem::SetParticle(int Idx, Edge::ParticlePtr pPart)
{
	assert(Idx < (int)m_Particles.size());
	assert(Idx > -1);
	m_Particles[Idx] = pPart;
}

void ConstraintSystem::ResizeParticles(TDParticleVector::size_type Size)
{
	m_Particles.resize(Size);
	//and resize the data structures that depend on particle size
	ResizeConstraintForce();
	ResizeVelocities();
	ResizeMass();
	ResizeMassInv();
	ResizeDJacobian();
	ResizeJacobian();
	ResizeForce();	
}

void ConstraintSystem::ResizeConstraints(TDConstraintVector::size_type Size)
{
	m_Constraints.resize(Size);	
}

void ConstraintSystem::ClearConstraints()
{
	m_Constraints.clear();
}

void ConstraintSystem::ClearParticles()
{
	m_Particles.clear();
}

void ConstraintSystem::CalculateMassMatrix() 
{
	if (m_MassCalculated)
	{
		TDParticleVector::size_type n = m_Particles.size();		
		//m_Mass = bnu::zero_matrix<double>(m_Mass.size1(), m_Mass.size2());
		m_Mass.clear();
		for (TDParticleVector::size_type i = 0; i < n; ++i)
		{
			m_Mass(i*3,i*3) = m_Particles[i]->GetMass();
			m_Mass(i*3+1,i*3+1) = m_Particles[i]->GetMass();
			m_Mass(i*3+2,i*3+2) = m_Particles[i]->GetMass();
		}
		m_MassCalculated = true;
	}
}

void ConstraintSystem::CalculateLambda()
{	
	//solve the folowing linear system for lambda
	//J*MInv*J^t*lambda = -DJ*Velocities - J*MInv*Q

	bnu::matrix<double> A;
	bnu::matrix<double> Temp;
	bnu::vector<double> u;	
	bnu::vector<double> OldLambda(m_Lambda);	
//	m_Lambda = bnu::zero_vector<double>(m_Lambda.size());
	m_Lambda.clear();
	/*fstream FileJacobian("Jacobian.txt", ios_base::out | ios_base::trunc);
	fstream FileDJacobian("DJacobian.txt", ios_base::out | ios_base::trunc);
	fstream FileInvMass("InvMass.txt", ios_base::out | ios_base::trunc);
	fstream FileVelocities("Velocities.txt", ios_base::out | ios_base::trunc);
	fstream FileLambda("Lambda.txt", ios_base::out | ios_base::trunc);	
	fstream FileForces("Forces.txt", ios_base::out | ios_base::trunc);	
	fstream FileA("A.txt", ios_base::out | ios_base::trunc);	
	fstream Fileu("u.txt", ios_base::out | ios_base::trunc);	*/
			
	//before solving check that all our inputs are the right size.
	TDParticleVector::size_type n = m_Particles.size();
	CalculateJacobian();
	CalculateDJacobian();
	CalculateMassInvMatrix();
	CalculateVelocities();
	CalculateConVel();
	CalculateConPos();
	/*matrixWrite(FileJacobian, m_Jacobian);
	matrixWrite(FileDJacobian, m_DJacobian);
	matrixWrite(FileInvMass, m_MassInv);
	VectorWrite(FileVelocities, m_Velocities);
	VectorWrite(FileForces, m_Forces);*/
	assert(m_Velocities.size() == n * 3);
	assert(m_Jacobian.size2() == n * 3);
	assert(m_DJacobian.size2() == n * 3);
	assert(m_Jacobian.size2() == m_MassInv.size1());
	assert(m_Forces.size() == m_MassInv.size2());
	Temp = bnu::prod(m_Jacobian, m_MassInv);
	A = bnu::prod(Temp, trans(m_Jacobian));	
	u = bnu::prod(-m_DJacobian, m_Velocities) - 
		bnu::prod(bnu::prod(m_Jacobian, m_MassInv), m_Forces)
		-m_ks*m_ConPos - m_kd*m_ConVel;
	/*matrixWrite(FileA, A);
	VectorWrite(Fileu, u);	*/
	bool Breakdown = false;
	BiCGSolve(A, m_Lambda, u, Breakdown);
	if (Breakdown)
	{
		//failed to solve for lambda - basically means that A is singular
		//m_Lambda = OldLambda;
		//m_Lambda = bnu::zero_vector<double>(m_Lambda.size());
	}
	/*VectorWrite(FileLambda, m_Lambda);*/
}


void ConstraintSystem::CalculateConstraintForce()
{		
	//fstream FileCForce("ConstraintForce.txt", ios_base::out | ios_base::trunc);
	CalculateLambda();	
	m_CForce = bnu::prod(bnu::trans(m_Jacobian), m_Lambda);
	//VectorWrite(FileCForce, m_CForce);
}

void ConstraintSystem::CalculateMassInvMatrix()
{
	if (!m_MassInvCalculated)
	{
		TDParticleVector::size_type n = m_Particles.size();		
		//m_MassInv = bnu::zero_matrix<double>(m_MassInv.size1(), m_MassInv.size2());
		m_MassInv.clear();
		for (TDParticleVector::size_type i = 0; i < n; ++i)
		{
			m_MassInv(i*3,i*3) = 1.0/m_Particles[i]->GetMass();
			m_MassInv(i*3+1,i*3+1) = 1.0/m_Particles[i]->GetMass();
			m_MassInv(i*3+2,i*3+2) = 1.0/m_Particles[i]->GetMass();
		}
		m_MassInvCalculated = true;
	}
}

void ConstraintSystem::CalculateVelocities()
{
	TDParticleVector::size_type n = m_Particles.size();	
	for (TDParticleVector::size_type i = 0; i < n; ++i)
	{
		m_Velocities[i*3] = m_Particles[i]->GetVelocity()[0];
		m_Velocities[i*3+1] = m_Particles[i]->GetVelocity()[1];
		m_Velocities[i*3+2] = m_Particles[i]->GetVelocity()[2];
	}
}

void ConstraintSystem::SetForce(const bnu::vector<double>& Forces)
{
	m_Forces = Forces;
}

void ConstraintSystem::SetForce(int Idx, const bnu::vector<double>& Forces)
{
	m_Forces[Idx*3] = Forces[0];
	m_Forces[Idx*3+1] = Forces[1];
	m_Forces[Idx*3+2] = Forces[2];
}

void ConstraintSystem::SetForceAllParticles(const bnu::vector<double>& Force)
{	
	TDParticleVector::size_type NumParticles = m_Particles.size();	
	for (TDParticleVector::size_type i = 0; i < NumParticles; ++i)
	{
		m_Forces[i*3] = Force[0];
		m_Forces[i*3+1] = Force[1];
		m_Forces[i*3+2] = Force[2];
	}
}

void ConstraintSystem::SetParticles(const TDParticleVector& Particles)
{
	m_Particles = Particles;
	ResizeConstraintForce();
	ResizeVelocities();
	ResizeMass();
	ResizeMassInv();
	ResizeDJacobian();
	ResizeJacobian();
	ResizeForce();
}

void ConstraintSystem::RemoveConstraint(TDConstraintVector::size_type Idx)
{
	m_Constraints.erase(m_Constraints.begin() + Idx);	
}

bnu::matrix<double>::size_type ConstraintSystem::CalculateRowSize()
{
	bnu::matrix<double>::size_type Rows=0;
	TDConstraintVector::const_iterator it;
	for (it = m_Constraints.begin(); it != m_Constraints.end(); ++it)
	{
		Rows += (*it)->GetRowSize();
	}	
	return Rows;
}

void ConstraintSystem::CalculateConPos()
{
	TDConstraintVector::const_iterator it;
	TDConstraintVector::size_type RowOffset=0;
	int RowSize = 0;

	for (it = m_Constraints.begin(); 
		it != m_Constraints.end(); 
		++it)
	{
		RowSize = (*it)->GetRowSize();		
		bnu::vector_range< bnu::vector<double> > vr(m_ConPos, bnu::range(RowOffset, RowOffset + RowSize));		
		(*it)->CalculateConPos();
		vr = (*it)->GetConPos();
		RowOffset += RowSize;
	}
}

void ConstraintSystem::CalculateConVel()
{
	TDConstraintVector::const_iterator it;
	TDConstraintVector::size_type RowOffset=0;
	int RowSize = 0;

	for (it = m_Constraints.begin(); 
		it != m_Constraints.end(); 
		++it)
	{
		RowSize = (*it)->GetRowSize();		
		bnu::vector_range< bnu::vector<double> > vr(m_ConVel, bnu::range(RowOffset, RowOffset + RowSize));		
		(*it)->CalculateConVel();
		bnu::vector<double> temp((*it)->GetConVel());
		vr = (*it)->GetConVel();		
		RowOffset += RowSize;
	}
}

void ConstraintSystem::CalculateJacobian()
{
	TDConstraintVector::const_iterator it;
	TDConstraintVector::size_type RowOffset=0;
	int RowSize = 0;

	for (it = m_Constraints.begin(); 
		it != m_Constraints.end(); 
		++it)
	{
		RowSize = (*it)->GetRowSize();	
		int size = m_Particles.size();
		bnu::matrix_range < bnu::matrix<double> > mr(m_Jacobian, 
			bnu::range(RowOffset, RowOffset+ RowSize), 
			bnu::range(0,m_Particles.size()*3));
		(*it)->CalculateJacobian();
		mr = (*it)->GetJacobian();		
		RowOffset += RowSize;
	}	
}

void ConstraintSystem::CalculateDJacobian()
{
	TDConstraintVector::const_iterator it;
	TDConstraintVector::size_type RowOffset=0;
	int RowSize = 0;

	for (it = m_Constraints.begin(); 
		it != m_Constraints.end(); 
		++it)
	{
		RowSize = (*it)->GetRowSize();
		bnu::matrix_range < bnu::matrix<double> > mr(m_DJacobian, 
			bnu::range(RowOffset, RowOffset+RowSize), 
			bnu::range(0,m_Particles.size()*3));
		(*it)->CalculateDJacobian();
		mr = (*it)->GetDJacobian();
		RowOffset += RowSize;
	}	
}

void ConstraintSystem::ResizeVelocities()
{
	m_Velocities.resize(m_Particles.size()*3);
}

void ConstraintSystem::ResizeLambda()
{
	m_Lambda.resize(CalculateRowSize());
}

void ConstraintSystem::ResizeMass()
{
	m_Mass.resize(m_Particles.size()*3, m_Particles.size()*3);
}

void ConstraintSystem::ResizeMassInv()
{
	m_MassInv.resize(m_Particles.size()*3, m_Particles.size()*3);
}

void ConstraintSystem::ResizeJacobian()
{
	m_Jacobian.resize(CalculateRowSize(), m_Particles.size()*3);
}

void ConstraintSystem::ResizeDJacobian()
{
	m_DJacobian.resize(CalculateRowSize(), m_Particles.size()*3);
}

void ConstraintSystem::ResizeForce()
{
	m_Forces.resize(m_Particles.size()*3);
}

void ConstraintSystem::ResizeConstraintForce()
{
	m_CForce.resize(m_Particles.size()*3);
}

void ConstraintSystem::ResizeConVel()
{
	m_ConVel.resize(CalculateRowSize());
}

void ConstraintSystem::ResizeConPos()
{
	m_ConPos.resize(CalculateRowSize());
}
