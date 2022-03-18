#include "TrajectoryBuilder.h"
#include <data/DataStruct.h>
#include <data/GameConstant.h>
#include <modules/core/ps/internal/PhysicsConstants.hpp>
#include <cmath>

using namespace ps;
using namespace ps::ExtMath;

const ExtMath::vector TrajectoryBuilder::R_VEC = ExtMath::vector(0, 0, -PhysicsConstants::BALL_RADIUS);
const double TrajectoryBuilder::STEP_UNIT = 5;
const double TrajectoryBuilder::MIN_L_VEL = TrajectoryBuilder::STEP_UNIT;
const double TrajectoryBuilder::ROLLING_ACCEL_LEN = PhysicsConstants::G * PhysicsConstants::ROLLING_FRICTION_COEF;
const double TrajectoryBuilder::MAX_X = PhysicsConstants::TABLE_WIDTH / 2 - PhysicsConstants::BALL_RADIUS;
const double TrajectoryBuilder::MAX_Y = PhysicsConstants::TABLE_HEIGHT / 2 - PhysicsConstants::BALL_RADIUS;
const double TrajectoryBuilder::MIN_X = -TrajectoryBuilder::MAX_X;
const double TrajectoryBuilder::MIN_Y = -TrajectoryBuilder::MAX_Y;
const double TrajectoryBuilder::CST_2pG = 2 / PhysicsConstants::G;
const double TrajectoryBuilder::SLIDING_ACCEL_LEN = PhysicsConstants::G * PhysicsConstants::SLIDING_FRICTION_COEF;

TrajectoryBuilder::TrajectoryBuilder()
{
}

ps::TrajectoryBuilder::~TrajectoryBuilder()
{
}


TrajectoryElement *TrajectoryBuilder::buildBasicTrajectory(PhysicsCue* cue, BallBody* cueBall, std::vector<BallBody*> balls)
{
	auto cueDir = cue->getDirection();
	auto dir = vector::unit(vector(cueDir.x, cueDir.y));

	auto trajectory = new BasicTrajElem(0, cueBall->position(), dir);
	trajectory->checkCollisions(cue, balls);

	return trajectory;
}

std::vector<ps::ExtMath::vector> ps::TrajectoryBuilder::buildBasicPoints(BasicTrajElem & basicTrajectory)
{
	auto start = vector(basicTrajectory.position);
	auto end = basicTrajectory.calc(basicTrajectory.endTime);
	start.z = end.z = PhysicsConstants::BALL_RADIUS;
	auto points = std::vector<vector>();
	points.push_back(start);
	points.push_back(end);
	return points;
}


std::vector<TrajectoryElement *> ps::TrajectoryBuilder::buildTrajectories(double force, PhysicsCue* cue, BallBody* cueBall, std::vector<BallBody*> balls)
{
	std::vector<TrajectoryElement*> trajectories;
	if (force == 0) {
		return trajectories;
	}

	auto velocities = cue->calcCueBallVelocities(force);

	double time = 0;
	MotionData motion = {cueBall->position(), velocities.linearVelocity};

	TrajectoryElement *trajectory = nullptr;
	double zVal;

	while (motion.velocity.z > 0) {
		trajectory = new FlyTrajElem(time, motion.position, motion.velocity);
		time = trajectory->endTime;
		motion.position = trajectory->calc(time);
		motion.position.z = 0;
		zVal = motion.velocity.z - PhysicsConstants::BOUNCE_FLOOR_REFLECT_COEF;
		if (zVal < 0) {
			motion.velocity.z = 0;
		}
		else {
			motion.velocity.z = sqrt(motion.velocity.z * zVal);
		}
		trajectories.push_back(trajectory);
		trajectory->checkCollisions(cueBall, balls);
		if (trajectory->isFinished) break;
	}

	if (!trajectory || !trajectory->isFinished) {
		auto vc = vector::add(motion.velocity, vector::cross(velocities.angularVelocity, R_VEC));
		vc.z = 0;
		auto vcLen = vector::length(vc);
		if (ExtMath::isZeroNumber(vcLen)) {
			// Pure-rolling
			trajectory = new PureTrajElem(time, motion.position, motion.velocity);
			time = trajectory->endTime;
			trajectories.push_back(trajectory);
			trajectory->checkCollisions(cueBall, balls);
		}
		else {
			trajectory = new NonPureTrajElem(time, motion.position, motion.velocity, vc, vcLen);
			time = trajectory->endTime;
			trajectories.push_back(trajectory);
			trajectory->checkCollisions(cueBall, balls);
			if (!trajectory->isFinished) {
				motion = trajectory->calcAll(time);
				trajectory = new PureTrajElem(time, motion.position, motion.velocity);
				time = trajectory->endTime;
				trajectories.push_back(trajectory);
				trajectory->checkCollisions(cueBall, balls);
			}
		}
	}

	return trajectories;

}

std::vector<ps::ExtMath::vector> ps::TrajectoryBuilder::buildPoints(std::vector<TrajectoryElement*> trajectories)
{
	if (trajectories.size() == 0) 
		return std::vector<ps::ExtMath::vector>();
	auto len = trajectories.size();
	auto time = trajectories[len - 1]->endTime;

	std::vector<vector> points;

	auto index = 0;
	auto trajectory = trajectories[index];
	auto current = trajectory->startTime;

	vector point;
	double lVel;

	// Fly Trajectory
	if (dynamic_cast<FlyTrajElem*>(trajectory) || dynamic_cast<NonPureTrajElem*>(trajectory)) {
		while (current <= time) {
			MotionData motion = trajectory->calcAll(current);
			motion.position.z += PhysicsConstants::BALL_RADIUS;
			points.push_back(motion.position);
			//CCLOG("TrajectoryBuilder::MIN_L_VEL %lf %lf", TrajectoryBuilder::MIN_L_VEL, vector::length(motion.velocity));
			lVel = std::max(TrajectoryBuilder::MIN_L_VEL, vector::length(motion.velocity));
			current += TrajectoryBuilder::STEP_UNIT / lVel;
			if (current > trajectory->endTime) {
				while (index < trajectories.size() -1 && current > trajectory->endTime){
					trajectory = trajectories[++index];
				}
				if (index < trajectories.size() - 1) {
					current = trajectory->startTime;
					if (dynamic_cast<PureTrajElem*>(trajectory)) {
						break;
					}
				}
			}
		}
	}

	if (trajectory) {
		// Pure-rolling
		if (current <= time) {
			point = trajectory->calc(current);
			point.z += PhysicsConstants::BALL_RADIUS;
			points.push_back(point);
			if (time != current) {
				point = trajectory->calc(time);
				point.z += PhysicsConstants::BALL_RADIUS;
				points.push_back(point);
			}
		}
	}
	else {
		// Fill end point of last trajectory
		if (trajectories.size() > 0) {
			trajectory = trajectories[trajectories.size() - 1];
			if (time != current) {
				point = trajectory->calc(time);
				point.z += PhysicsConstants::BALL_RADIUS;
				points.push_back(point);
			}
		}
	}

	return points;
}

TrajectoryReflectData ps::TrajectoryBuilder::calcBasicReflectDir(TrajectoryElement * trajectory)
{
	return calcReflectDir(trajectory, 30);
}

bool ps::TrajectoryBuilder::isOutSize(vector position)
{
	return (
		position.x > MAX_X || position.x < TrajectoryBuilder::MIN_X ||
		position.y > MAX_Y || position.y < TrajectoryBuilder::MIN_Y
		);
	return false;
}

TrajectoryReflectData ps::TrajectoryBuilder::calcReflectDir(TrajectoryElement * trajectory, double lengthVelocity)
{
	auto endPos = trajectory->calc(trajectory->endTime);
	if (dynamic_cast<FlyTrajElem*>(trajectory)) {
		auto ball = trajectory->collidedBall;
		auto trajectoryReflect = TrajectoryReflectData();
		trajectoryReflect.seg1.s = endPos;
		trajectoryReflect.seg1.e = endPos;
		if(ball == nullptr) trajectoryReflect.ballId = -1;
		else trajectoryReflect.ballId = ball->id();
		return trajectoryReflect;
	} // Not supported yet
	auto velocity = trajectory->calcVelocity(trajectory->endTime);
	auto rawVelocity = vector::length(velocity);

	if (lengthVelocity != -1) {
		velocity = vector::multiply(lengthVelocity, vector::unit(velocity));
		rawVelocity = lengthVelocity;
	}
	else if (rawVelocity > 30) {
		velocity = vector::multiply(30 / rawVelocity, velocity);
		rawVelocity = 30;
	}

	if (trajectory->collidedBall) {
		auto ballPos = vector(trajectory->collidedBall->position());

		auto v2 = vector::unit(vector::sub(ballPos, endPos));
		auto cosAlpha = vector::dot(v2, vector::unit(velocity));
		v2 = vector::multiply(rawVelocity * cosAlpha, v2);
		// White ball velo direction after collsion
		auto v1 = vector::sub(velocity, v2);

		endPos.z += PhysicsConstants::BALL_RADIUS;
		ballPos.z += PhysicsConstants::BALL_RADIUS;
		auto trajectoryReflect = TrajectoryReflectData(endPos, vector::add(endPos, v1),
			ballPos, vector::add(ballPos, v2), trajectory->collidedBall->id());
		return trajectoryReflect;
	}
	else if (trajectory->collidedCushion.x || trajectory->collidedCushion.y) {
		auto normal = vector(trajectory->collidedCushion.y, trajectory->collidedCushion.x);
		auto v = vector::reflect(velocity, normal);

		endPos.z += PhysicsConstants::BALL_RADIUS;
		auto trajectoryReflect = TrajectoryReflectData(endPos, vector::add(endPos, v));
		return trajectoryReflect;
	}

	return TrajectoryReflectData();
}

double ps::TrajectoryBuilder::calcAxisDuration(double x, double v, double a, double limit)
{
	if (a == 0) {
		if (v > 0) {
			return (limit - x) / v;
		}
		else if (v < 0) {
			return -(x + limit) / v;
		}
		return 0;
	}
	else {
		if (x == limit && v > 0) {
			return 0;
		}
		if (x == -limit && v < 0) {
			return 0;
		}

		auto double_a = 2 * a;
		auto d = v * v - double_a * x;
		auto double_a_limit = double_a * limit;
		auto d1 = d + double_a_limit; // high
		auto d2 = d - double_a_limit; // low

		auto duration = Infinity;

		if (x != limit && d1 >= 0) {
			auto sd1 = sqrt(d1);
			auto t11 = (-sd1 - v) / a;
			auto t12 = (sd1 - v) / a;

			if (t11 >= 0) {
				duration = fmin(duration, t11);
			}
			if (t12 >= 0) {
				duration = fmin(duration, t12);
			}
		}

		if (x != -limit && d2 >= 0) {
			auto sd2 = sqrt(d2);
			auto t21 = (-sd2 - v) / a;
			auto t22 = (sd2 - v) / a;
			if (t21 >= 0) {
				duration = fmin(t21, duration);
			}
			if (t22 >= 0) {
				duration = fmin(t22, duration);
			}
		}

		return duration;
	}
}


