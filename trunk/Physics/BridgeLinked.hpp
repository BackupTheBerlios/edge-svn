#pragma once

#include "./PartConstRB.hpp"

namespace Edge
{
	/**
	Represents a bridge made from links, like the wooden bridge 
	in Indiana Jones and the Temple of Doom. The one where near the
	end of the movie the boy jumps up and down on the bridge saying 
	'strong wood, strong wood'. But the bridge breaks in two and they
	are left hanging onto the ropes and climbing the planks for safety.
	*/
	class BridgeLinked :
		public Edge::PartConstRB
	{
	public:
		typedef boost::shared_ptr<BridgeLinked> BridgeLinkedPtr;		

		BridgeLinked(void);
		virtual ~BridgeLinked(void);
		virtual void BuildSystem();	
		void BreakLink(int i);
		
	private:		
		void SetupConstraintSystem();
	};
}
