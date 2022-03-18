#include "StaticBall.h"
#include "modules/core/ps/internal/PhysicsConstants.hpp"
#include "../internal/CollisionResolver.h"
#include <algorithm>

using namespace ps;

ps::StaticBall::StaticBall()
{

}

ps::StaticBall::~StaticBall()
{

}

ps::MotionResult ps::StaticBall::calcAll(double time)
{
	return _linearMotion.calcAll(time);
}

ps::ExtMath::vector ps::StaticBall::calcPosition(double time)
{
	return _linearMotion.calcPosition(time);
}

ps::ExtMath::vector ps::StaticBall::calcLinearVelocity(double time)
{
	return _linearMotion.calcVelocity(time);
}

ps::ExtMath::vector ps::StaticBall::calcAngularVelocity(double time)
{
	return _angularMotion.calcVelocity(time);
}

void ps::StaticBall::updateVelocities(double time, vector & linearVelocity, vector & angularVelocity)
{
	StaticMotion &linearMotion = _linearMotion;
	StaticMotion &angularMotion = _angularMotion;

	vector position = linearMotion.origin = linearMotion.calcPosition(time);
	angularMotion.origin = angularMotion.calcPosition(time);

	if (ExtMath::isZeroNumber(position.z)) {
		// on the table
		updateVelocitiesOnTheTable(time, position, linearVelocity, angularVelocity);
	}
	else {
		// on the sky
		updateVelocitiesOnTheSky(time, position, linearVelocity, angularVelocity);
	}
}

void ps::StaticBall::updateVelocities(double time, vector & linearVelocity)
{
	vector angularVelocity = _angularMotion.calcVelocity(time);
	updateVelocities(time, linearVelocity, angularVelocity);
}

void ps::StaticBall::updateVelocities(double time)
{
	vector linearVelocity = _linearMotion.calcVelocity(time);
	vector angularVelocity = _angularMotion.calcVelocity(time);
	updateVelocities(time, linearVelocity, angularVelocity);
}

void ps::StaticBall::updateVelocitiesOnTheTable(double time, vector & position, vector & linearVelocity, vector & angularVelocity)
{
	StaticMotion & linearMotion = _linearMotion;
	StaticMotion & angularMotion = _angularMotion;

	bool lvEqZero = vector::equalsZero(linearVelocity);
	bool motionless = (lvEqZero && vector::equalsZero(angularVelocity));

	if (motionless) {
		linearMotion.startTime = angularMotion.startTime = -1;
		return;
	}

	if (ExtMath::isLessThanZeroNumber(linearVelocity.z)) {
		vector collisionPoint = vector(position);
		collisionPoint.z = -PhysicsConstants::BALL_RADIUS;

		StaticWorld::Velocities velocities = CollisionResolver::resolveCollisionWithStaticPoint(
			position, collisionPoint,
			linearVelocity, angularVelocity,
			PhysicsConstants::FLOOR_U, PhysicsConstants::FLOOR_E
		);

		if (velocities.linearVelocity.z <= BOUNCE_FLOOR_REFLECT_COEF) {
			velocities.linearVelocity.z = 0;
		}

		linearVelocity = velocities.linearVelocity;
		angularVelocity = velocities.angularVelocity;
		// updateLinearVelocityFloorBouncing(linearVelocity);
	}

	if (ExtMath::isZeroNumber(linearVelocity.z)) {
		linearVelocity.z = 0.0;
		updateVelocitiesNonJump(time, linearVelocity, angularVelocity, lvEqZero);
	}
	else {
		updateVelocitiesJumpOnTheTable(time, linearVelocity, angularVelocity);
	}
}

void ps::StaticBall::updateVelocitiesNonJump(double time, vector & linearVelocity, vector & angularVelocity, bool lvEqZero)
{
	vector vc = vector::add(vector::cross(angularVelocity, R_DOWN_VEC), linearVelocity);

	if (vector::equalsZero(vc)) {
		if (lvEqZero) {
			updateVelocitiesSpiningOnly(time, angularVelocity);
		}
		else {
			updateVelocitiesPureRolling(time, linearVelocity, angularVelocity);
		}
	}
	else {
		updateVelocitiesNonPureRolling(time, linearVelocity, angularVelocity, vc);
	}
}

void ps::StaticBall::updateVelocitiesSpiningOnly(double time, vector & angularVelocity)
{
	StaticMotion &linearMotion = _linearMotion;
	StaticMotion &angularMotion = _angularMotion;

	// Spining only
	linearMotion.startTime = -1;
	angularMotion.startTime = time;

	vectorSetZero(angularMotion.velocity);
	vectorSetZero(angularMotion.acceleration);

	angularMotion.endTime = time + updateSpiningMotion(angularVelocity, angularMotion);
}

void ps::StaticBall::updateVelocitiesPureRolling(double time, vector & linearVelocity, vector & angularVelocity)
{
	StaticMotion &linearMotion = _linearMotion;
	StaticMotion &angularMotion = _angularMotion;

	double dt = vector::length(linearVelocity) / ROLLING_FRICTION_LENGTH;

	linearMotion.startTime = angularMotion.startTime = time;
	linearMotion.endTime = angularMotion.endTime = time + dt;

	linearMotion.velocity = vector(linearVelocity);
	linearMotion.acceleration = vector::multiply(-1.0 / dt, linearVelocity);

	// Synchronize Angular Motion With Linear Motion
	angularMotion.velocity = vector::multiply(
		INVERSE_BALL_RADIUS_SQUARE,
		vector::cross(linearVelocity, R_DOWN_VEC)
	);

	angularMotion.acceleration = vector::multiply(
		INVERSE_BALL_RADIUS_SQUARE,
		vector::cross(linearMotion.acceleration, R_DOWN_VEC)
	);

	// And Check Spining
	dt = updateSpiningMotion(angularVelocity, angularMotion);
	if (dt > 0.0) {
		angularMotion.endTime = std::min(angularMotion.endTime, time + dt);
	}
}

void ps::StaticBall::updateVelocitiesNonPureRolling(double time, vector & linearVelocity, vector & angularVelocity, vector & vc)
{
	StaticMotion &linearMotion = _linearMotion;
	StaticMotion &angularMotion = _angularMotion;

	linearMotion.startTime = angularMotion.startTime = time;

	double dt = vector::length(vc) / SLIDING_FRICTION_LENGTH;

	linearMotion.endTime = angularMotion.endTime = time + dt / 3.5;
	linearMotion.velocity = vector(linearVelocity);
	angularMotion.velocity = vector(angularVelocity);

	linearMotion.acceleration = vector::multiply(-1.0 / dt, vc);
	angularMotion.acceleration = vector::multiply(C1 / dt, vector::cross(vc, R_DOWN_VEC));

	// And Check Spining
	dt = updateSpiningMotion(angularVelocity, angularMotion);

	if (dt > 0.0) {
		angularMotion.endTime = std::min(angularMotion.endTime, time + dt);
	}
}

void ps::StaticBall::updateVelocitiesJumpOnTheTable(double time, vector & linearVelocity, vector & angularVelocity)
{
	StaticMotion & linearMotion = _linearMotion;
	StaticMotion & angularMotion = _angularMotion;

	angularMotion.startTime = linearMotion.startTime = time;
	angularMotion.endTime = linearMotion.endTime = time + C2 * linearVelocity.z;

	linearMotion.velocity = vector(linearVelocity);
	angularMotion.velocity = vector(angularVelocity);
	vectorSetZero(angularMotion.acceleration);
	linearMotion.acceleration = vector(GRAVITY_VEC);
}

void ps::StaticBall::updateVelocitiesOnTheSky(double time, vector & position, vector & linearVelocity, vector & angularVelocity)
{
	StaticMotion &linearMotion = _linearMotion;
	StaticMotion &angularMotion = _angularMotion;

	double vz = linearVelocity.z;
	double motionTime = (vz + sqrt(vz * vz + C3 * position.z)) / G;

	angularMotion.startTime = linearMotion.startTime = time;
	angularMotion.endTime = linearMotion.endTime = time + motionTime;

	linearMotion.velocity = vector(linearVelocity);
	angularMotion.velocity = vector(angularVelocity);
	vectorSetZero(angularMotion.acceleration);
	linearMotion.acceleration = vector(GRAVITY_VEC);
}

double ps::StaticBall::updateSpiningMotion(vector & angularVelocity, StaticMotion & angularMotion)
{
	if (ExtMath::isZeroNumber(angularVelocity.z)) {
		angularVelocity.z = 0.0;
		return 0.0;
	}
	else {
		angularMotion.velocity.z = angularVelocity.z;

		if (angularVelocity.z < 0) {
			angularMotion.acceleration.z = SPINING_FRICTION_LENGTH;
		}
		else {
			angularMotion.acceleration.z = -SPINING_FRICTION_LENGTH;
		}

		return (-angularVelocity.z / angularMotion.acceleration.z);
	}
}

void ps::StaticBall::updateLinearVelocityFloorBouncing(vector & linearVelocity)
{
	double z = linearVelocity.z;
	double val = z * (z + BOUNCE_FLOOR_REFLECT_COEF);
	if (val <= 0) {
		linearVelocity.z = 0;
	}
	else {
		linearVelocity.z = sqrt(val);
	}
}

void ps::StaticBall::vectorSetZero(vector & v)
{
	v.x = v.y = v.z = 0;
}

const double StaticBall::BOUNCE_FLOOR_REFLECT_COEF = PhysicsConstants::BOUNCE_FLOOR_REFLECT_COEF;
const double StaticBall::ROLLING_FRICTION_LENGTH = PhysicsConstants::ROLLING_FRICTION_COEF * PhysicsConstants::G;
const double StaticBall::SLIDING_FRICTION_LENGTH = PhysicsConstants::SLIDING_FRICTION_COEF * PhysicsConstants::G;
const double StaticBall::SPINING_FRICTION_LENGTH = PhysicsConstants::SPINING_FRICTION_COEF * PhysicsConstants::G;
const double StaticBall::INVERSE_BALL_RADIUS_SQUARE = 1 / PhysicsConstants::BALL_RADIUS_SQUARE;
const double StaticBall::BALL_RADIUS_SQUARE = PhysicsConstants::BALL_RADIUS_SQUARE;
const double StaticBall::G = PhysicsConstants::G;
const vector StaticBall::R_DOWN_VEC = vector(0.0, 0.0, -PhysicsConstants::BALL_RADIUS);
const vector StaticBall::GRAVITY_VEC = vector(0.0, 0.0, -PhysicsConstants::G);

const double StaticBall::C1 = 2.5 / StaticBall::BALL_RADIUS_SQUARE;
const double StaticBall::C2 = 2.0 / StaticBall::G;
const double StaticBall::C3 = 2.0 * G;