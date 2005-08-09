#pragma once

#include "../Math/Math.hpp"
#include "./Particle.hpp"
#include "./Spring.hpp"

namespace Edge
{
	class MassSpringSystem 
	{
	public:
		typedef boost::tuple<Edge::ParticlePtr, 
			Edge::Spring::SpringPtr, Edge::ParticlePtr> TDPSPTuple;
		typedef boost::shared_ptr<MassSpringSystem> MassSpringSystemPtr;
		typedef std::vector< TDPSPTuple > TDPSPTupleContainer;
		typedef std::map<Edge::ParticlePtr, bnu::vector<double> > TDParticleToForce;
		
		MassSpringSystem();
		MassSpringSystem(const TDPSPTupleContainer&);
		virtual ~MassSpringSystem(void);
				
		int CountParticles(void) const;
		void CalculateForce();
		const bnu::vector<double>& GetForce(Edge::ParticlePtr pParticle) const;
		void SetDamping(double Damping) {m_Damping = Damping;}
		double GetDamping() {return m_Damping;}
		void ResizeParticleForceMap();
		void ZeroForcesOnParticles();
		void SetPSPContainer(const TDPSPTupleContainer& TD);		
		bool IsParticleInSystem(const Edge::ParticlePtr pParticle);		
				
		TDParticleToForce::iterator PToFBegin();
		TDParticleToForce::iterator PToFEnd();
		TDParticleToForce::const_iterator PToFBegin() const;
		TDParticleToForce::const_iterator PToFEnd() const;

		TDPSPTupleContainer::iterator PSPBegin();
		TDPSPTupleContainer::iterator PSPEnd();
		TDPSPTupleContainer::const_iterator PSPBegin() const;
		TDPSPTupleContainer::const_iterator PSPEnd() const;

	private:
		double m_Damping;
		TDPSPTupleContainer m_PSPContainer;
		TDParticleToForce m_PToF;
	};
}
