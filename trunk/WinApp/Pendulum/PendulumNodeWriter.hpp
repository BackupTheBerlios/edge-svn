#pragma once
#include <fstream>
#include "./PendulumNode.hpp"
#include "../../Render/Render.hpp"

class PendulumNodeWriter :
	public Edge::NodeVisitor
{
public:
	typedef boost::shared_ptr<std::fstream> FStreamPtr;

	PendulumNodeWriter(void);
	virtual ~PendulumNodeWriter(void);
	
	void Init(PendulumNode::PendulumNodePtr pNode);
	void WriteTranslate(Edge::Translate& Trans, int Idx);	
	void AddFileStream(FStreamPtr pFileStream);
	void SetTime(double t) { m_Time = t; }
	double GetTime() { return m_Time; }

private:
	std::vector<FStreamPtr> m_FileStreams;
	double m_Time;
};
