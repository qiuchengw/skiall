#ifndef _NIMA_SOLVER_HPP_
#define _NIMA_SOLVER_HPP_

namespace nima
{
	class Solver
	{
		public:
			virtual int order() = 0;
			virtual bool needsSolve() = 0;
			virtual bool suppressMarkDirty() = 0;
			virtual void suppressMarkDirty(bool suppressIt) = 0;
			virtual void solveStart() = 0;
			virtual void solve() = 0;
	};
}
#endif