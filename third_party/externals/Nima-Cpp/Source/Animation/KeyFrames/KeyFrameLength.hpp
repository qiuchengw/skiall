#ifndef _NIMA_KEYFRAMELENGTH_HPP_
#define _NIMA_KEYFRAMELENGTH_HPP_

#include "KeyFrameNumeric.hpp"

namespace nima
{
	class ActorNode;

	class KeyFrameLength : public KeyFrameNumeric
	{
		protected:
			void setValue(ActorComponent* component, float value, float mix) override;
	};
}

#endif