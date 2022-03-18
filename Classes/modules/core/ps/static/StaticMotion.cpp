#include "StaticMotion.h"
#include <cocos2d.h>

using namespace ps;
ps::StaticMotion::StaticMotion()
{
	acceleration = vector::ZERO;
	velocity = vector::ZERO;
	origin = vector::ZERO;
	startTime = -1;
	endTime = 0;
}

ps::StaticMotion::StaticMotion(const StaticMotion & src)
{
	acceleration = src.acceleration;
	velocity = src.velocity;
	origin = src.origin;
	startTime = src.startTime;
	endTime = src.endTime;
}

ps::StaticMotion::~StaticMotion()
{

}

ps::MotionResult ps::StaticMotion::calcAll(double time)
{
	vector o = origin;
	if (startTime < 0)
	{
		return MotionResult(vector(o), vector::ZERO);
	}

	vector v = velocity;
	vector a = acceleration;

	double t = MIN(time, endTime) - startTime;

	vector at = vector::multiply(t, a);
	double half_t = 0.5 * t;
	vector vel = vector::add(v, at);
	vector pos = vector(
		o.x + v.x * t + at.x * half_t,
		o.y + v.y * t + at.y * half_t,
		o.z + v.z * t + at.z * half_t
	);
	return MotionResult(pos, vel);
}

ps::ExtMath::vector ps::StaticMotion::calcVelocity(double time)
{
	if (startTime < 0)
	{
		return vector::ZERO;
	}
	double t = MIN(time, endTime) - startTime;
	vector vel = vector::add(velocity, vector::multiply(t, acceleration));
	return vel;
}

ps::ExtMath::vector ps::StaticMotion::calcPosition(double time)
{
	vector o = origin;
	if (startTime < 0)
	{
		return vector(o);
	}

	vector v = velocity;
	vector a = acceleration;

	double t = MIN(time, endTime) - startTime;

	double half_t2 = 0.5 * t * t;
	vector pos = vector(
		o.x + v.x * t + a.x * half_t2,
		o.y + v.y * t + a.y * half_t2,
		o.z + v.z * t + a.z * half_t2
	);
	return pos;
}

ps::EquationResults ps::StaticMotion::resolveMotionEquation(double d, vector p, vector v, vector a)
{
	double ea, eb, ec, ed, ee;
	EquationResults results;

	if (vector::equalsZero(a)) {
		if (vector::equalsZero(v)) return EquationResults::empty();

		ec = vector::dot(v, v);
		ed = 2 * vector::dot(p, v);
		ee = vector::dot(p, p) - d * d;

		results = Equation::resolveQuadraticEquation(ec, ed, ee);
		return results;
	}

	ea = 0.25 * vector::dot(a, a);
	eb = vector::dot(a, v);
	ec = vector::dot(p, a) + vector::dot(v, v);
	ed = 2 * vector::dot(p, v);
	ee = vector::dot(p, p) - d * d;

	results = Equation::resolveQuarticEquation(ea, eb, ec, ed, ee);
	return Equation::applyNRMethodQuarticAll(results, ea, eb, ec, ed, ee);
}

//MotionResult
ps::MotionResult::MotionResult(const vector & position, const vector & velocity)
{
	this->position = position;
	this->velocity = velocity;
}

ps::MotionResult::~MotionResult()
{

}
