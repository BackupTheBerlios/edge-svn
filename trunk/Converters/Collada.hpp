#pragma once

#include <string>
#include "../Render/Node.hpp"

namespace Edge
{

class Collada
{
	public:
		Edge::NodePtr Import(std::string Filename);
};

}
