#pragma once

#include "../Math/Math.hpp"
#include "./Particle.hpp"
#include "./Spring.hpp"

namespace Edge
{
	/**
	Computes the force on the given particles using the Clausius-Clapeyron equation. 
	P = V^-1RT where R = 8.31441417 = Avogadro's number * Boltzmanns constant, T = temperature (Kelvin).
	Volume is approximated using a ellipsoid bounding volume.
	*/
	class IdealGasSystem
	{
	public:		
		typedef bnu::vector<double> TDForce;
		typedef boost::shared_ptr<IdealGasSystem> IdealGasSystemPtr;
		typedef std::vector<Edge::ParticlePtr> TDParticleVector;
		typedef std::map<Edge::ParticlePtr, TDForce> TDParticleToForce;
		typedef std::vector<Edge::TDNormalPtr> TDNormalContainer;

		IdealGasSystem(void);
		virtual ~IdealGasSystem(void);
		
		void AddParticle(Edge::ParticlePtr pP);
		void CalculateForce();
		const TDForce& GetForce(Edge::ParticlePtr pParticle) const;
		void ResizeParticleForceMap();
		void SetParticleContainer(const TDParticleVector& Particles);
		double CalcVolume();
		void SetNormalContainer(const TDNormalContainer& Normals);

	private:		
		TDParticleVector m_Particles;
		TDNormalContainer m_Normals;
		TDParticleToForce m_PToF;
		const double m_R;
		double m_T;
	};
}
