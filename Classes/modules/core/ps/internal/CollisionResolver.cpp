#include "CollisionResolver.h"
#include "PhysicsConstants.hpp"

using namespace ps;

const double ps::CollisionResolver::ANGULAR_COLLISION_CONST = 5 / (7 * PhysicsConstants::BALL_RADIUS * PhysicsConstants::BALL_RADIUS);
const double ps::CollisionResolver::TS_CONST = 2.0 / 7;

ps::CollisionResolver::CollisionResolver()
{

}

ps::CollisionResolver::~CollisionResolver()
{

}

ps::StaticWorld::Velocities ps::CollisionResolver::resolveCollisionWithStaticPoint(vector & centerPoint, vector & collisionPoint, vector & linearVelocity, vector & angularVelocity, double coeffFriction, double coeffElastic)
{
	vector R = vector::sub(collisionPoint, centerPoint);
	vector vn = vector::project(linearVelocity, R);
	vector va = vector::sub(linearVelocity, vn);

	vector vc = vector::add(va, vector::cross(angularVelocity, R));
	double vcLength = vector::length(vc);
	double dvcLength = 3.5 * coeffFriction * (1 + coeffElastic) * vector::length(vn);
	vector dvc;

	if (dvcLength > vcLength) {
		dvc = vector::reverse(vc);
	}
	else {
		dvc = vector::multiply(-dvcLength / vcLength, vc);
	}
	vector va2 = vector::add(va, vector::multiply(TS_CONST, dvc));
	vector w2 = vector::add(angularVelocity, vector::multiply(
		ANGULAR_COLLISION_CONST,
		vector::cross(R, dvc)
	));

	vector vn2 = vector::multiply(-coeffElastic, vn);

	return StaticWorld::Velocities(
		vector::add(va2, vn2),
		w2
	);
}

void ps::CollisionResolver::resolveCollisionBallWithBall(std::vector<vector> & result, vector & p_1, vector & p_2, vector & lv_1, vector & lv_2, double noiseAngle)
{
	vector R = vector::sub(p_1, p_2);
	if (noiseAngle != 0) {
		R = vector::rotate2D(R, noiseAngle);
	}

	vector v12 = vector::project(lv_1, R);
	vector v11 = vector::sub(lv_1, v12);

	vector v21 = vector::project(lv_2, R);
	vector v22 = vector::sub(lv_2, v21);

	result.push_back(vector::add(v11, v21));
	result.push_back(vector::add(v12, v22));
}
