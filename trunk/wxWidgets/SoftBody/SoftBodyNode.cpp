#include "./SoftBodyNode.hpp"

using namespace Edge;

SoftBodyNode::SoftBodyNode(void)
{
}

SoftBodyNode::~SoftBodyNode(void)
{
}

void SoftBodyNode::Setup()
{
	Mesh::MeshPtr pMesh(new Mesh);
	Translate::TranslatePtr pTrans(new Translate);
	pMesh->LoadObj("./sphere.obj");
	pTrans->SetTranslate(0,0,0);		
	pTrans->AddChild(pMesh);
	AddChild(pTrans);
}