#ifndef CollisionResolver_h_
#define CollisionResolver_h_

#include "../static/StaticWorld.h"

namespace ps
{
	class CollisionResolver
	{
	private:
		static const double ANGULAR_COLLISION_CONST;
		static const double TS_CONST;
	public:
		CollisionResolver();
		~CollisionResolver();

		static ps::StaticWorld::Velocities resolveCollisionWithStaticPoint(
			vector & centerPoint, vector & collisionPoint,
			vector & linearVelocity, vector & angularVelocity,
			double coeffFriction, double coeffElastic
		);

		static void resolveCollisionBallWithBall(std::vector<vector> & result, vector & p_1, vector & p_2, vector & lv_1, vector & lv_2, double noiseAngle);
	};
}

#endif
