#include <iostream>
#include <fstream>
#include <numeric>
#include <boost/test/unit_test.hpp>
#include <boost/test/floating_point_comparison.hpp>
#include <boost/numeric/ublas/matrix.hpp>
#include <boost/numeric/ublas/io.hpp>
#include "../../Math/Math.hpp"
#include "../../Physics/Physics.hpp"

//run unit tests on the classes of the physics library.

using namespace std;
using namespace boost::unit_test_framework;
using namespace Edge;
using Particle::ParticlePtr;
using Spring::SpringPtr;

void ParticleTest()
{
	Particle P;
	ublas::zero_vector<double> Zero(3);
	ublas::vector<double> V(3);
	P.SetPosition(Zero);
	P.SetMass(1);
	P.SetVelocity(Zero);
	//In general KE = 0.5*mass*velocity^2
	//a particle with zero velocity should have zero kinetic energy
	BOOST_CHECK_CLOSE(P.GetKineticEnergy(), 0.0, 1e-5);
	V[0] = 1;
	V[1] = 0;
	V[2] = 0;
	P.SetVelocity(V);
	//a particle with a velocity of (1,0,0) should have a KE of 0.5*mass
	BOOST_CHECK_CLOSE(P.GetKineticEnergy(), 0.5*P.GetMass(), 1e-5);
	V[1] = 1;
	P.SetVelocity(V);
	//a particle with a velocity of (1,1,0) should have a KE of 0.5*mass*2
	BOOST_CHECK_CLOSE(P.GetKineticEnergy(), 0.5*P.GetMass()*2, 1e-5);
}



void MassSpringTest1()
{
	Particle::ParticlePtr pP0(new Particle);
	Particle::ParticlePtr pP1(new Particle);
	Particle::ParticlePtr pP2(new Particle);
	Spring::SpringPtr pS0(new Spring);
	Spring::SpringPtr pS1(new Spring);
	pP0->SetPosition(0,0,0);
	pP1->SetPosition(1,0,0);
	pP2->SetPosition(2,0,0);
	pS0->SetRestLen(1);
	pS0->SetConstant(1);
	pS1->SetRestLen(1);
	pS1->SetConstant(1);
	MassSpringSystem::TDPSPTuple PSP0, PSP1;
	PSP0 = make_tuple(pP0, pS0, pP1);
	PSP1 = make_tuple(pP1, pS1, pP2);
	MassSpringSystem::TDPSPTupleContainer PSPContainer;
	PSPContainer.push_back(PSP0);
	PSPContainer.push_back(PSP1);
	MassSpringSystem MS0(PSPContainer);	
	BOOST_CHECK_EQUAL(MS0.CountParticles(), 3);	
	MS0.ZeroForcesOnParticles();
	MassSpringSystem::TDParticleToForce::iterator it(MS0.PToFBegin());
	while (it != MS0.PToFEnd())
	{
		BOOST_CHECK_CLOSE(it->second[0], 0.0, 1e-5);
		BOOST_CHECK_CLOSE(it->second[1], 0.0, 1e-5);
		BOOST_CHECK_CLOSE(it->second[2], 0.0, 1e-5);
		++it;
	}
	pP2->SetPosition(3,0,0);
	MS0.CalculateForce();
	BOOST_CHECK_CLOSE(MS0.GetForce(pP2)[0], -1.0, 1e-5);
	BOOST_CHECK_CLOSE(MS0.GetForce(pP2)[1], 0.0, 1e-5);
	BOOST_CHECK_CLOSE(MS0.GetForce(pP2)[2], 0.0, 1e-5);
	BOOST_CHECK_CLOSE(MS0.GetForce(pP1)[0], 1.0, 1e-5);
	BOOST_CHECK_CLOSE(MS0.GetForce(pP1)[1], 0.0, 1e-5);
	BOOST_CHECK_CLOSE(MS0.GetForce(pP1)[2], 0.0, 1e-5);
	pP0->SetPosition(-1,0,0);
	MS0.CalculateForce();
	BOOST_CHECK_CLOSE(MS0.GetForce(pP1)[0], 0.0, 1e-5);
	BOOST_CHECK_CLOSE(MS0.GetForce(pP1)[1], 0.0, 1e-5);
	BOOST_CHECK_CLOSE(MS0.GetForce(pP1)[2], 0.0, 1e-5);
}

void MassSpringTest2()
{
	Particle::ParticlePtr pP0(new Particle);
	Particle::ParticlePtr pP1(new Particle);
	Particle::ParticlePtr pP2(new Particle);
	Spring::SpringPtr pS0(new Spring);
	Spring::SpringPtr pS1(new Spring);
	//setup particles in a triangle where the base is d = 2 and the sides sqrt(2)
	pP0->SetPosition(0,0,0);
	pP1->SetPosition(1,1,0);
	pP2->SetPosition(2,0,0);
	pS0->SetRestLen(sqrt(2.0));
	pS0->SetConstant(1);
	pS1->SetRestLen(sqrt(2.0));
	pS1->SetConstant(1);
	MassSpringSystem::TDPSPTuple PSP0, PSP1;
	PSP0 = make_tuple(pP0, pS0, pP1);
	PSP1 = make_tuple(pP1, pS1, pP2);
	MassSpringSystem::TDPSPTupleContainer PSPContainer;
	PSPContainer.push_back(PSP0);
	PSPContainer.push_back(PSP1);
	MassSpringSystem MS0(PSPContainer);	
	BOOST_CHECK_EQUAL(MS0.CountParticles(), 3);	
	MS0.CalculateForce();	
	MassSpringSystem::TDParticleToForce::iterator it(MS0.PToFBegin());
	while (it != MS0.PToFEnd())
	{
		BOOST_CHECK_CLOSE(it->second[0], 0.0, 1e-5);
		BOOST_CHECK_CLOSE(it->second[1], 0.0, 1e-5);
		BOOST_CHECK_CLOSE(it->second[2], 0.0, 1e-5);
		++it;
	}
	pP1->SetPosition(1, 0.9, 0);
	MS0.CalculateForce();	
	const bnu::vector<double>& Force(MS0.GetForce(pP1));
	//using mathematica to work what the force should be....
	BOOST_CHECK_CLOSE(Force[0], 0.0, 1e-5);
	BOOST_CHECK_CLOSE(Force[1], 0.092118, 1e-5);
	BOOST_CHECK_CLOSE(Force[2], 0.0, 1e-5);	
}

void MassSpringTest()
{
	cout << "MassSpringTest" << endl;
	Particle::ParticlePtr pP0(new Particle);
	Particle::ParticlePtr pP1(new Particle);
	Spring::SpringPtr pS0(new Spring);
	pP0->SetPosition(0,0,0);
	pP1->SetPosition(1,0,0);
	pS0->SetRestLen(1);
	pS0->SetConstant(1);
	MassSpringSystem::TDPSPTuple PSP;
	PSP = make_tuple(pP0, pS0, pP1);
	MassSpringSystem::TDPSPTupleContainer PSPContainer;
	PSPContainer.push_back(PSP);
	MassSpringSystem MS0(PSPContainer);	
	BOOST_CHECK_EQUAL(MS0.CountParticles(), 2);	

	//test that the forces on the particles are 0
	MS0.ZeroForcesOnParticles();
	MassSpringSystem::TDParticleToForce::iterator it(MS0.PToFBegin());
	while (it != MS0.PToFEnd())
	{
		BOOST_CHECK_CLOSE(it->second[0], 0.0, 1e-5);
		BOOST_CHECK_CLOSE(it->second[1], 0.0, 1e-5);
		BOOST_CHECK_CLOSE(it->second[2], 0.0, 1e-5);
		++it;
	}

	MS0.CalculateForce();
	//given the configuration the forces on the particles should be 0
	it = MS0.PToFBegin();
	while (it != MS0.PToFEnd())
	{
		BOOST_CHECK_CLOSE(it->second[0], 0.0, 1e-5);
		BOOST_CHECK_CLOSE(it->second[1], 0.0, 1e-5);
		BOOST_CHECK_CLOSE(it->second[2], 0.0, 1e-5);
		++it;
	}
	//change the rest length of the spring and recalculate the forces on the particles
	MS0.SetDamping(0.0);
	pS0->SetRestLen(0.5);
	MS0.CalculateForce();
	//the force should be [0.5, 0.0, 0.0] and p1 [-0.5,0,0]
	it = MS0.PToFBegin();
	BOOST_CHECK_CLOSE(it->second[0], 0.5, 1e-5);
	BOOST_CHECK_CLOSE(it->second[1], 0.0, 1e-5);
	BOOST_CHECK_CLOSE(it->second[2], 0.0, 1e-5);
	it++;
	BOOST_CHECK_CLOSE(it->second[0], -0.5, 1e-5);
	BOOST_CHECK_CLOSE(it->second[1], 0.0, 1e-5);
	BOOST_CHECK_CLOSE(it->second[2], 0.0, 1e-5);
	
	//change the positions of the particles and set the rest length to 1
	pS0->SetRestLen(1.0);
	pP1->SetPosition(2*cos(Pi/4.0), 2*sin(Pi/4.0), 0);
	MS0.CalculateForce();
	//force on particles should be [0.707, 0.707, 0.0] and -[0.707, 0.707, 0.0]
	it = MS0.PToFBegin();
	BOOST_CHECK_CLOSE(it->second[0], 0.707107, 1e-5);
	BOOST_CHECK_CLOSE(it->second[1], 0.707107, 1e-5);
	BOOST_CHECK_CLOSE(it->second[2], 0.0, 1e-5);
	++it;
	BOOST_CHECK_CLOSE(it->second[0], -0.707107, 1e-5);
	BOOST_CHECK_CLOSE(it->second[1], -0.707107, 1e-5);
	BOOST_CHECK_CLOSE(it->second[2], 0.0, 1e-5);

	MassSpringTest1();
	MassSpringTest2();
}

void FixedDistTest()
{
	cout << "FixedDistTest" << endl;
	Particle::ParticlePtr pP0(new Particle);
	Particle::ParticlePtr pP1(new Particle);	
	pP0->SetPosition(0,0,0);
	pP1->SetPosition(1,0,0);
	FixedDist FD(pP0, pP1, 0, 1, 2);
	//check that the distance to maintain is correct
	BOOST_CHECK_CLOSE(FD.GetDistance(), 1.0, 1e-5);
	
	pP0->SetPosition(0,0,0);
	pP1->SetPosition(cos(Pi/4.0), sin(Pi/4.0), 0);
	//check that the distance to maintain is correct
	FixedDist FD1(pP0, pP1, 0, 1, 2);
	BOOST_CHECK_CLOSE(FD1.GetDistance(), 1.0, 1e-5);
	//since the constraint is initially satisfied check that the drift correction force is 0
	FD1.CalculateDriftForce();	
	BOOST_CHECK_CLOSE(FD1.GetDriftForce(pP0)[0], 0.0, 1e-5);
	BOOST_CHECK_CLOSE(FD1.GetDriftForce(pP0)[1], 0.0, 1e-5);
	BOOST_CHECK_CLOSE(FD1.GetDriftForce(pP0)[2], 0.0, 1e-5);
	BOOST_CHECK_CLOSE(FD1.GetDriftForce(pP1)[0], 0.0, 1e-5);
	BOOST_CHECK_CLOSE(FD1.GetDriftForce(pP1)[1], 0.0, 1e-5);
	BOOST_CHECK_CLOSE(FD1.GetDriftForce(pP1)[2], 0.0, 1e-5);

	//break the constraint, so that we get a non-zero drift correction force
	pP1->SetPosition(2*cos(Pi/4.0), 2*sin(Pi/4.0), 0);
	FD1.CalculateDriftForce();	
	BOOST_CHECK_CLOSE(FD1.GetDriftForce(pP0)[0], cos(Pi/4.0), 1e-5);
	BOOST_CHECK_CLOSE(FD1.GetDriftForce(pP0)[1], sin(Pi/4.0), 1e-5);
	BOOST_CHECK_CLOSE(FD1.GetDriftForce(pP0)[2], 0.0, 1e-5);
	BOOST_CHECK_CLOSE(FD1.GetDriftForce(pP1)[0], -cos(Pi/4.0), 1e-5);
	BOOST_CHECK_CLOSE(FD1.GetDriftForce(pP1)[1], -sin(Pi/4.0), 1e-5);
	BOOST_CHECK_CLOSE(FD1.GetDriftForce(pP1)[2], 0.0, 1e-5);
}

void FixedPositionTest()
{
	cout << "FixedPositionTest" << endl;
	Particle::ParticlePtr pP0(new Particle);
	pP0->SetPosition(0,1,0);
	FixedPosition FP(pP0, 0, 1, 1);
	BOOST_CHECK_CLOSE(FP.GetPosition()[0], 0.0, 1e-5);
	BOOST_CHECK_CLOSE(FP.GetPosition()[1], 1.0, 1e-5);
	BOOST_CHECK_CLOSE(FP.GetPosition()[2], 0.0, 1e-5);
	pP0->SetPosition(0, 2, 0);
	FP.CalculateDriftForce();
	BOOST_CHECK_CLOSE(FP.GetDriftForce(pP0)[0], 0.0, 1e-5);
	BOOST_CHECK_CLOSE(FP.GetDriftForce(pP0)[1], -1.0, 1e-5);
	BOOST_CHECK_CLOSE(FP.GetDriftForce(pP0)[2], 0.0, 1e-5);
}

void FixedPlaneTest()
{
	cout << "FixedPlaneTest" << endl;
	bnu::vector<double> Normal(3);
	Particle::ParticlePtr pP(new Particle);
	pP->SetPosition(0, 0, 0);
	Normal[0] = 0; Normal[1] = 1; Normal[0] = 0;
	FixedPlane FPlane(Normal, 0, pP, 0, 1);
	FPlane.CalculateDriftForce();
	//constraint initially satisfied so drift force should be 0
	BOOST_CHECK_CLOSE(FPlane.GetDriftForce(pP)[0], 0.0, 1e-5);
	BOOST_CHECK_CLOSE(FPlane.GetDriftForce(pP)[1], 0.0, 1e-5);
	BOOST_CHECK_CLOSE(FPlane.GetDriftForce(pP)[2], 0.0, 1e-5);

	//break the constraint
	pP->SetPosition(0, 1, 0);
	FPlane.CalculateDriftForce();
	//force should be [0,-1,0]
	BOOST_CHECK_CLOSE(FPlane.GetDriftForce(pP)[0], 0.0, 1e-5);
	BOOST_CHECK_CLOSE(FPlane.GetDriftForce(pP)[1], -1.0, 1e-5);
	BOOST_CHECK_CLOSE(FPlane.GetDriftForce(pP)[2], 0.0, 1e-5);
}

void DriftForceTest1()
{
	ConstraintSystem CS;
	Particle::ParticlePtr pP0(new Particle);
	Particle::ParticlePtr pP1(new Particle);	
	Particle::ParticlePtr pP2(new Particle);	
	//setup particles in a triangle where the base is d = 2 and the sides sqrt(2)
	pP0->SetPosition(0,0,0);
	pP1->SetPosition(1,1,0);
	pP2->SetPosition(2,0,0);
	FixedDist::FixedDistPtr pFD0(new FixedDist(pP0, pP1, 0, 1, 2));	
	FixedDist::FixedDistPtr pFD1(new FixedDist(pP1, pP2, 0, 1, 2));	
	CS.ResizeParticles(3);
	CS.SetParticle(0, pP0);
	CS.SetParticle(1, pP1);
	CS.SetParticle(2, pP2);
	CS.AddConstraint(pFD0);
	CS.AddConstraint(pFD1);
	CS.ResizeJacobian();
	CS.ResizeDJacobian();
	CS.CalculateDriftForce();
	//all should be 0 because constraints are satisfied
	BOOST_CHECK_CLOSE(CS.GetDriftForce()[0], 0.0, 1e-5);
	BOOST_CHECK_CLOSE(CS.GetDriftForce()[1], 0.0, 1e-5);
	BOOST_CHECK_CLOSE(CS.GetDriftForce()[2], 0.0, 1e-5);
	BOOST_CHECK_CLOSE(CS.GetDriftForce()[3], 0.0, 1e-5);
	BOOST_CHECK_CLOSE(CS.GetDriftForce()[4], 0.0, 1e-5);
	BOOST_CHECK_CLOSE(CS.GetDriftForce()[5], 0.0, 1e-5);
	BOOST_CHECK_CLOSE(CS.GetDriftForce()[6], 0.0, 1e-5);
	BOOST_CHECK_CLOSE(CS.GetDriftForce()[7], 0.0, 1e-5);
	BOOST_CHECK_CLOSE(CS.GetDriftForce()[8], 0.0, 1e-5);
	pP1->SetPosition(1, 0.9, 0);
	CS.CalculateDriftForce();
	//stretch = sqrt(2) - sqrt(1*1 + 0.9*0.9)
	/*BOOST_CHECK_CLOSE(CS.GetDriftForce()[0], 1 - sqrt(2.0), 1e-5);
	BOOST_CHECK_CLOSE(CS.GetDriftForce()[1], 0.0, 1e-5);
	BOOST_CHECK_CLOSE(CS.GetDriftForce()[2], 0.0, 1e-5);
	BOOST_CHECK_CLOSE(CS.GetDriftForce()[3], 0.0, 1e-5);
	BOOST_CHECK_CLOSE(CS.GetDriftForce()[4], 0.0, 1e-5);
	BOOST_CHECK_CLOSE(CS.GetDriftForce()[5], 0.0, 1e-5);
	BOOST_CHECK_CLOSE(CS.GetDriftForce()[6], 0.0, 1e-5);
	BOOST_CHECK_CLOSE(CS.GetDriftForce()[7], 0.0, 1e-5);
	BOOST_CHECK_CLOSE(CS.GetDriftForce()[8], 0.0, 1e-5);*/
}

void DriftForceTest0()
{
	ConstraintSystem CS;
	Particle::ParticlePtr pP0(new Particle);
	Particle::ParticlePtr pP1(new Particle);	
	pP0->SetPosition(0,0,0);
	pP1->SetPosition(1,0,0);
	FixedDist::FixedDistPtr pFD(new FixedDist(pP0, pP1, 0, 1, 2));	
	CS.ResizeParticles(2);
	CS.SetParticle(0, pP0);
	CS.SetParticle(1, pP1);
	CS.AddConstraint(pFD);
	CS.ResizeJacobian();
	CS.ResizeDJacobian();
	CS.CalculateDriftForce();
	BOOST_CHECK_CLOSE(CS.GetDriftForce()[0], 0.0, 1e-5);
	BOOST_CHECK_CLOSE(CS.GetDriftForce()[1], 0.0, 1e-5);
	BOOST_CHECK_CLOSE(CS.GetDriftForce()[2], 0.0, 1e-5);
	BOOST_CHECK_CLOSE(CS.GetDriftForce()[3], 0.0, 1e-5);
	BOOST_CHECK_CLOSE(CS.GetDriftForce()[4], 0.0, 1e-5);
	BOOST_CHECK_CLOSE(CS.GetDriftForce()[5], 0.0, 1e-5);

	pP1->SetPosition(2,0,0);
	CS.CalculateDriftForce();
	//force on p0 shoule be [1,0,0]
	BOOST_CHECK_CLOSE(CS.GetDriftForce()[0], 1.0, 1e-5);
	BOOST_CHECK_CLOSE(CS.GetDriftForce()[1], 0.0, 1e-5);
	BOOST_CHECK_CLOSE(CS.GetDriftForce()[2], 0.0, 1e-5);
	//force on p1 should be [-1,0,0]
	BOOST_CHECK_CLOSE(CS.GetDriftForce()[3], -1.0, 1e-5);
	BOOST_CHECK_CLOSE(CS.GetDriftForce()[4], 0.0, 1e-5);
	BOOST_CHECK_CLOSE(CS.GetDriftForce()[5], 0.0, 1e-5);
}

void ConstraintSystemTest()
{
	cout << "ConstraintSystemTest" << endl;
	DriftForceTest0();
	DriftForceTest1();	
}

void FixedVectorTest()
{
	cout << "FixedVectorTest" << endl;
	Particle::ParticlePtr pP0(new Particle);
	Particle::ParticlePtr pP1(new Particle);	
	pP0->SetPosition(0,0,0);
	pP1->SetPosition(1,0,0);
	bnu::vector<double> u(pP0->GetPosition() - pP1->GetPosition())
	FixedVector FV(pP0, pP1, 0, 1, 2);
	//check that the distance to maintain is correct
	BOOST_CHECK_CLOSE(FV.GetVector()[0], u[0], 1e-5);
	BOOST_CHECK_CLOSE(FV.GetVector()[1], u[1], 1e-5);
	BOOST_CHECK_CLOSE(FV.GetVector()[2], u[2], 1e-5);
	
	pP0->SetPosition(0,0,0);
	pP1->SetPosition(0,1,2);
	bnu::vector<double> u(pP0->GetPosition() - pP1->GetPosition())
	//check that the distance to maintain is correct
	FixedDist FV1(pP0, pP1, 0, 1, 2);
	BOOST_CHECK_CLOSE(FV1.GetVector()[0], u[0],1e-5);
	BOOST_CHECK_CLOSE(FV1.GetVector()[1], u[1],1e-5);
	BOOST_CHECK_CLOSE(FV1.GetVector()[2], u[2],1e-5);
	//since the constraint is initially satisfied check that the drift correction force is 0
	FV1.CalculateDriftForce();	
	BOOST_CHECK_CLOSE(FV1.GetDriftForce(pP0)[0], 0.0, 1e-5);
	BOOST_CHECK_CLOSE(FV1.GetDriftForce(pP0)[1], 0.0, 1e-5);
	BOOST_CHECK_CLOSE(FV1.GetDriftForce(pP0)[2], 0.0, 1e-5);
	BOOST_CHECK_CLOSE(FV1.GetDriftForce(pP1)[0], 0.0, 1e-5);
	BOOST_CHECK_CLOSE(FV1.GetDriftForce(pP1)[1], 0.0, 1e-5);
	BOOST_CHECK_CLOSE(FV1.GetDriftForce(pP1)[2], 0.0, 1e-5);

	//break the constraint, so that we get a non-zero drift correction force
	pP1->SetPosition(2*cos(Pi/4.0), 2*sin(Pi/4.0), 0);
	FV1.CalculateDriftForce();	
	BOOST_CHECK_CLOSE(FV1.GetDriftForce(pP0)[0], cos(Pi/4.0), 1e-5);
	BOOST_CHECK_CLOSE(FV1.GetDriftForce(pP0)[1], sin(Pi/4.0), 1e-5);
	BOOST_CHECK_CLOSE(FV1.GetDriftForce(pP0)[2], 0.0, 1e-5);
	BOOST_CHECK_CLOSE(FV1.GetDriftForce(pP1)[0], -cos(Pi/4.0), 1e-5);
	BOOST_CHECK_CLOSE(FV1.GetDriftForce(pP1)[1], -sin(Pi/4.0), 1e-5);
	BOOST_CHECK_CLOSE(FV1.GetDriftForce(pP1)[2], 0.0, 1e-5);
}


test_suite* init_unit_test_suite(int argc, char* argv[])
{
    test_suite* test = BOOST_TEST_SUITE("Physics test");
	//TODO: Complete tests on the mass spring system that simulate damping.
	//At the moment all particles have 0 velocity so damping forces don't kick
	//in and are not tested for.
	test->add(BOOST_TEST_CASE(&MassSpringTest));
	test->add(BOOST_TEST_CASE(&FixedDistTest));
	test->add(BOOST_TEST_CASE(&FixedPositionTest));
	test->add(BOOST_TEST_CASE(&FixedPlaneTest));
	test->add(BOOST_TEST_CASE(&FixedVectorTest));
	test->add(BOOST_TEST_CASE(&ConstraintSystemTest));
    return test;
}
