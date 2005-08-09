#pragma once

#include <boost/numeric/ublas/vector.hpp>
#include <boost/shared_ptr.hpp>
#include <iostream>
#include <list>

namespace bnu=boost::numeric::ublas;

namespace Edge
{	
	class Particle
	{
	public:					
		Particle(void);
		Particle(const bnu::vector<double>& Location, 
			const bnu::vector<double>& Velocity, double Mass);
		virtual ~Particle(void);

		const bnu::vector<double>& GetPosition() const {return m_Position;}
		void SetPosition(const bnu::vector<double>& Position) {m_Position = Position;}
		void SetPosition(double x, double y, double z);

		const bnu::vector<double>& GetVelocity() const {return m_Velocity;}
		void SetVelocity(const bnu::vector<double>& Velocity) {m_Velocity = Velocity;}
		void SetVelocity(double x, double y, double z);

		double GetMass() const {return m_Mass;}
		void SetMass(double Mass) {m_Mass = Mass;}
		
		double GetKineticEnergy() const;

		friend std::ostream& operator << (std::ostream& Out, const Particle& P);

	private:
		bnu::vector<double> m_Position;
		bnu::vector<double> m_Velocity;
		double m_Mass;
	};	

	typedef boost::shared_ptr<Particle> ParticlePtr;
	typedef std::vector<ParticlePtr> TDParticleVector;
	typedef std::list<ParticlePtr> TDParticleList;
}

