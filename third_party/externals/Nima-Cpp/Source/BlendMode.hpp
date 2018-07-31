#ifndef _NIMA_BLENDMODE_HPP_
#define _NIMA_BLENDMODE_HPP_

namespace nima
{
	enum class BlendMode : signed int
	{
		Off = -1,
		Normal = 0,
		Multiply = 1,
		Screen = 2,
		Additive = 3
	};
}

#endif