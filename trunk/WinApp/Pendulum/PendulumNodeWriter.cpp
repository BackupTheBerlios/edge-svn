#include ".\PendulumNodeWriter.hpp"
#include <sstream>

using namespace std;

PendulumNodeWriter::PendulumNodeWriter(void)
{
}

PendulumNodeWriter::~PendulumNodeWriter(void)
{
}

void PendulumNodeWriter::WriteTranslate(Edge::Translate& Trans, int Idx)
{
	int n = m_FileStreams.size();
	assert(Idx < n);
	assert(Idx > -1);	
	double x, y, z;
	Trans.GetTranslate(x, y, z);
	*(m_FileStreams[Idx]) << m_Time << " " << x << " " << y << " " << z << "\n";	
}

void PendulumNodeWriter::AddFileStream(FStreamPtr pFileStream)
{
	m_FileStreams.push_back(pFileStream);
}

void PendulumNodeWriter::Init(PendulumNode::PendulumNodePtr pNode)
{
	m_FileStreams.resize(pNode->GetNumTranslateNodes()); 
	int n = m_FileStreams.size();
	for (int i = 0; i < n; i++)
	{
		stringstream str;
		str << "Translate" << i << ".txt";
		m_FileStreams[i] = FStreamPtr(new fstream(str.str().c_str(), ios_base::out));		
	}
}
