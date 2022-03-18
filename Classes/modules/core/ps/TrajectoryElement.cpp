#include "TrajectoryElement.h"
#include <modules/core/ps/internal/PhysicsConstants.hpp>
#include <modules/core/ps/TrajectoryBuilder.h>
#include <data/GameConstant.h>
#include <cmath>

TrajectoryElement::TrajectoryElement()
{
}

TrajectoryElement::~TrajectoryElement()
{
	CCLOG("~TrajectoryElement");
}

void TrajectoryElement::init(double startTime, vector position, vector lineVel, vector lineAccel)
{
	this->startTime = startTime;
	this->position = position;
	this->lineVel = lineVel;
	this->lineAccel = lineAccel;
	this->duration = this->calcDuration();
	this->endTime = this->startTime + this->duration;
	this->isFinished = false;
	this->collidedBall = nullptr;
	this->collidedCushion = vector();
}

TrajectoryElement::TrajectoryElement(double startTime, vector position, vector lineVel, vector lineAccel)
{
	this->init(startTime, position, lineVel, lineAccel);
}

vector TrajectoryElement::calc(double time)
{
	auto t = time - this->startTime;
	auto haflTSquare = 0.5 * t * t;
	return vector(
		this->position.x + this->lineVel.x * t + this->lineAccel.x * haflTSquare,
		this->position.y + this->lineVel.y * t + this->lineAccel.y * haflTSquare,
		this->position.z + this->lineVel.z * t + this->lineAccel.z * haflTSquare
	);
	return vector();
}

double TrajectoryElement::calcX(double time)
{
	auto t = time - this->startTime;
	auto haflTSquare = 0.5 * t * t;
	return this->position.x + this->lineVel.x * t + this->lineAccel.x * haflTSquare;
}

double TrajectoryElement::calcY(double time)
{
	auto t = time - this->startTime;
	auto haflTSquare = 0.5 * t * t;
	return this->position.y + this->lineVel.y * t + this->lineAccel.y * haflTSquare;
}

vector TrajectoryElement::calcVelocity(double time) {
	auto t = time - this->startTime;
	auto v = this->lineVel;
	auto a = this->lineAccel;
	return vector(
		v.x + a.x * t,
		v.y + a.y * t,
		v.z + a.z * t
	);
}

MotionData TrajectoryElement::calcAll(double time)
{
	auto t = time - this->startTime;
	auto a = this->lineAccel;
	auto v = this->lineVel;
	auto p = this->position;

	auto atx = a.x * t;
	auto aty = a.y * t;
	auto atz = a.z * t;

	auto velocity = vector(
		v.x + atx,
		v.y + aty,
		v.z + atz
	);
	auto position = vector(
		p.x + (v.x + atx * 0.5) * t,
		p.y + (v.y + aty * 0.5) * t,
		p.z + (v.z + atz * 0.5) * t
	);
	return MotionData{position, velocity};
}

double TrajectoryElement::calcDuration()
{
	return 0.0;
}

const double COLLISION_DISTANCE_SQUARE = 4 * PhysicsConstants::BALL_RADIUS_SQUARE;

void TrajectoryElement::checkCollisions(BallBody* cueBall, std::vector<BallBody*> balls)
{
	auto la = this->lineAccel;
	auto lv = this->lineVel;
	auto pos = this->position;

	auto a = 0.25 * vector::dot(la, la);
	auto b = vector::dot(la, lv);
	auto vv = vector::dot(lv, lv);
	auto len = balls.size();

	auto i = 0;
	BallBody* collidedBall = nullptr;
	BallBody* ball;
	vector dp;
	double t = Infinity;

	// auto c, d, e;
	while (i < len) {
		ball = balls[i++];
		if (1 <= ball->id() && ball->id() <= 15) {
			dp = vector::sub(pos, ball->position());
			// c = vv + vector::dot(dp, la)
			// d = 2 * vector::dot(dp, lv)
			// e = vector::dot(dp, dp) - COLLISION_DISTANCE_SQUARE
			auto res = ExtMath::resolveQuarticEquation(
				a, b,
				vv + vector::dot(dp, la), 2 * vector::dot(dp, lv),
				vector::dot(dp, dp) - COLLISION_DISTANCE_SQUARE
			);

			for (auto j = 0; j < res.size(); j++) {
				if (res[j] >= 0 && res[j] < t) {
					t = res[j];
					collidedBall = ball;
				}
			}
		}
	}

	if (t <= this->duration) {
		this->isFinished = true;
		this->duration = t;
		this->endTime = this->startTime + this->duration;
		this->collidedBall = collidedBall;
		this->collidedCushion.x = this->collidedCushion.y = 0;
	}
}



FlyTrajElem::FlyTrajElem(double startTime, vector position, vector lineVel)
{
	this->init(startTime, position, lineVel, PhysicsConstants::GRAVITY);
}


double FlyTrajElem::calcDuration()
{
	auto maxDuration = TrajectoryBuilder::CST_2pG * this->lineVel.z;
	auto endPos = this->calc(this->startTime + maxDuration);
	if (TrajectoryBuilder::isOutSize(endPos)) {
		this->isFinished = true;

		double tx = Infinity;
		if (this->lineVel.x > 0) {
			tx = (TrajectoryBuilder::MAX_X - this->position.x) / this->lineVel.x;
		}
		else if (this->lineVel.x < 0) {
			tx = (TrajectoryBuilder::MIN_X - this->position.x) / this->lineVel.x;
		}

		double ty = Infinity;
		if (this->lineVel.y > 0) {
			ty = (TrajectoryBuilder::MAX_Y - this->position.y) / this->lineVel.y;
		}
		else if (this->lineVel.y < 0) {
			ty = (TrajectoryBuilder::MIN_Y - this->position.y) / this->lineVel.y;
		}

		return std::min(std::min(tx, ty), maxDuration);
	}
	else {
		return maxDuration;
	}
}

PureTrajElem::PureTrajElem(double startTime, vector position, vector velocity)
{
	this->lineVelLen = vector::length(velocity);
	auto rollingAccel = vector::multiply(-TrajectoryBuilder::ROLLING_ACCEL_LEN / lineVelLen, velocity);
	this->lineVel = velocity;
	this->init(startTime, position, velocity, rollingAccel);
}

double PureTrajElem::calcDuration()
{
	auto maxDuration = this->lineVelLen / TrajectoryBuilder::ROLLING_ACCEL_LEN;
	auto t1 = TrajectoryBuilder::calcAxisDuration(this->position.x, this->lineVel.x, this->lineAccel.x, TrajectoryBuilder::MAX_X);
	auto t2 = TrajectoryBuilder::calcAxisDuration(this->position.y, this->lineVel.y, this->lineAccel.y, TrajectoryBuilder::MAX_Y);
	auto t = std::min(t1, t2);

	if (t <= maxDuration) {
		this->isFinished = true;
		// Calc collided cushion
		if (t1 < t2) { // X cushion
			auto x = this->calcX(t1);
			if (x > 0) {
				this->collidedCushion.x = TrajectoryBuilder::MAX_X;
			}
			else {
				this->collidedCushion.x = -TrajectoryBuilder::MAX_X;
			}
		}
		else { // Y cushion
			auto y = this->calcY(t2);
			if (y > 0) {
				this->collidedCushion.y = TrajectoryBuilder::MAX_Y;
			}
			else {
				this->collidedCushion.y = -TrajectoryBuilder::MAX_Y;
			}
		}
		return t;
	}
	else {
		return maxDuration;
	}
}


NonPureTrajElem::NonPureTrajElem(double startTime, vector position, vector velocity, vector vc, double vcLength)
{
	this->vcLen = vcLength;
	this->vc = vc;
	auto slidingAccel = vector::multiply(-TrajectoryBuilder::SLIDING_ACCEL_LEN / vcLen, vc);
	this->init(startTime, position, velocity, slidingAccel);
}

double NonPureTrajElem::calcDuration()
{
	auto maxDuration = this->vcLen / (3.5 * TrajectoryBuilder::SLIDING_ACCEL_LEN);
	auto t1 = TrajectoryBuilder::calcAxisDuration(this->position.x, this->lineVel.x, this->lineAccel.x, TrajectoryBuilder::MAX_X);
	auto t2 = TrajectoryBuilder::calcAxisDuration(this->position.y, this->lineVel.y, this->lineAccel.y, TrajectoryBuilder::MAX_Y);
	auto t = std::min(t1, t2);

	if (t <= maxDuration) {
		this->isFinished = true;
		if (t1 < t2) { // X cushion
			auto x = this->calcX(t1);
			if (x > 0) {
				this->collidedCushion.x = TrajectoryBuilder::MAX_X;
			}
			else {
				this->collidedCushion.x = -TrajectoryBuilder::MAX_X;
			}
		}
		else { // Y cushion
			auto y = this->calcY(t2);
			if (y > 0) {
				this->collidedCushion.y = TrajectoryBuilder::MAX_Y;
			}
			else {
				this->collidedCushion.y = -TrajectoryBuilder::MAX_Y;
			}
		}
		return t;
	}
	else {
		return maxDuration;
	}
}

BasicTrajElem::BasicTrajElem(double startTime, ps::ExtMath::vector position, ps::ExtMath::vector direction)
{
	this->init(startTime, position, direction, vector(0,0,0));
}

BasicTrajElem::~BasicTrajElem()
{
}

double BasicTrajElem::calcDuration()
{
	auto t1 = TrajectoryBuilder::calcAxisDuration(position.x, lineVel.x, 0, TrajectoryBuilder::MAX_X);
	auto t2 = TrajectoryBuilder::calcAxisDuration(position.y, lineVel.y, 0, TrajectoryBuilder::MAX_Y);
	if (t1 < t2) { // X cushion
		auto x = calcX(t1);
		if (x > 0) {
			collidedCushion.x = TrajectoryBuilder::MAX_X;
		}
		else {
			collidedCushion.x = -TrajectoryBuilder::MAX_X;
		}
	}
	else { // Y cushion
		auto y = calcY(t2);
		if (y > 0) {
			collidedCushion.y = TrajectoryBuilder::MAX_Y;
		}
		else {
			collidedCushion.y = -TrajectoryBuilder::MAX_Y;
		}
	}

	return MIN(t1, t2);
}

void BasicTrajElem::checkCollisions(ps::PhysicsCue * cue, std::vector<ps::BallBody *> balls)
{
	auto ballPos = position;
	auto dir = lineVel;

	auto len = balls.size();
	auto i = 0;
	BallBody* ball;
	vector d;
	double dv, delta, tt;
	double t = INFINITY;
	BallBody* collidedBall = nullptr;
	while (i < len) {
		ball = balls[i];
		if (1 <= ball->id() && ball->id() <= 15) {
			d = vector::sub(ballPos, ball->position());
			dv = vector::dot(dir, d);
			delta = dv * dv - (vector::dot(d, d) - COLLISION_DISTANCE_SQUARE);
			if (delta >= 0) {
				tt = -dv + sqrt(delta);
				if (tt >= 0) {
					if (t > tt) {
						t = tt;
						collidedBall = ball;
					}
					// t = m_min(tt, t);
					tt = -dv - sqrt(delta);
					if (tt >= 0) {
						if (t > tt) {
							t = tt;
							collidedBall = ball;
						}
						// t = m_min(tt, t);
					}
				}
			}
		}
		i = i + 1;
	}

	if (t <= duration) {
		isFinished = true;
		duration = t;
		endTime = startTime + duration;
		collidedBall = collidedBall;
		collidedCushion.x = collidedCushion.y = 0;
	}
}
