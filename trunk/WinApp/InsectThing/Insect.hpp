#pragma once
#include "../../Physics/Particle.hpp"
#include "./InsectController.hpp"
#include "../../Render/DiffSystemNode.hpp"
#include "./InsectEngine.hpp"

class Insect :
	public Edge::DiffSystemNode
{
public:
	typedef boost::shared_ptr<Insect> InsectPtr;
	typedef std::vector<Edge::TranslatePtr> TDTranslateContainer;
	typedef boost::tuple<Edge::ParticlePtr, Edge::ParticlePtr> TDParticleTuple;
	typedef std::list< TDParticleTuple > TDConnectContainer;

	Insect(void);
	virtual ~Insect(void);
	void SetParticle(int Idx, Edge::ParticlePtr pParticle);	
	void SetForce(int Idx, const bnu::vector<double>& Force);
	void ApplyVisitor(Edge::NodeVisitor* pVisitor);	
	void UpdateTranslateNodesFromState();
	void Setup(InsectController::InsectControllerPtr InsectController);
	TDTranslateContainer::size_type GetNumTranslateNodes() {return m_TranslateNodes.size();}
	const TDConnectContainer& GetFDConnect() {return m_FDConnect;}
	const TDConnectContainer& GetSpringConnect() {return m_SpringConnect;}
	void Accelerate();
	void Decelerate();
	void ZeroAcceleration();
	void BuildRoachConnectPoints(InsectController::InsectControllerPtr pInsCont);

private:
	 TDTranslateContainer m_TranslateNodes;	
	 TDConnectContainer m_FDConnect;	
	 TDConnectContainer m_SpringConnect;
};

