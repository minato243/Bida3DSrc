#ifndef MasseCueBall_h_
#define MasseCueBall_h_

#include "../static/StaticWorld.h"

namespace ps
{
	class MasseCueBall
	{
	public:
		StaticWorld::MotionGroup *startMotionGroup;
		StaticWorld::MotionGroup *endMotionGroup;

		MasseCueBall(StaticWorld::MotionGroup *startMotionGroup, StaticWorld::MotionGroup *endMotionGroup);
	protected:
	private:
	};
}

#endif