#ifndef __FLOOR_SURFACE__
#define __FLOOR_SURFACE__
#include "ExtensionMath.hpp"
#include "PhysicsEntities.hpp"
#include "PhysicsConstants.hpp"

namespace ps {
	using namespace ExtMath;

	class FloorSurface {
	public:
		AABB safeZone;
		AABB * limits = nullptr;
		Pocket * pockets = nullptr;
		AABB tableEdges;

		void setLimits(AABB safeZone, AABB *limits, Pocket *pockets, AABB tableEdges);

		bool isOnTableEdges(const vector & p);

		bool isInSafeZone(const vector & p);

		bool isOutSideTable(const vector& p);

		Pocket * findExtractPocket(const vector & p);

		Pocket * findRegionPocket(const vector & p);

		vector normalAcceleration(const vector & p);

		double minimumZOfPosition(const vector & p);

	private:
		static const int TOP_RIGHT_POCKET_ID;
		static const int TOP_SIDE_POCKET_ID;
		static const int TOP_LEFT_POCKET_ID;
		static const int BOTTOM_LEFT_POCKET_ID;
		static const int BOTTOM_SIDE_POCKET_ID;
		static const int BOTTOM_RIGHT_POCKET_ID;
		
	};
}

#endif
