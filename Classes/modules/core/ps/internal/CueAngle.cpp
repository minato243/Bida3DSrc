#include "CueAngle.h"
#include "PhysicsConstants.hpp"
#include "../../../../data/GameConstant.h"

using namespace ps;

CueAngle::CueAngle()
{
	_cueHeadRadius = 0.f;
	_cueTailRadius = 0.f;
	_cueLength = 0.f;
	_headBallDistance = 0.f;
	_deltaRadiusPerLength = 0.f;
	_lenLimit = 0;
	_lenLimit2 = 0;
}

CueAngle::~CueAngle()
{

}

void CueAngle::setBalls(std::vector<ps::BallBody*> *balls)
{
	_balls = balls;

	for (auto i = 0; i < balls->size(); i++)
	{
		_positions[balls->at(i)->id()] =  &balls->at(i)->position();
	}
}

void CueAngle::setCueOffset(ps::ExtMath::vector offset)
{
	_offset = offset;
}

void CueAngle::setCueDirection(ps::ExtMath::vector direction)
{
	_direction = direction;
}

void CueAngle::setCueShape(float cueHeadRadius, float cueTailRadius, float cueLength, float headBallDistance)
{
	_cueHeadRadius = cueHeadRadius;
	_cueTailRadius = cueTailRadius;
	_cueLength = cueLength;
	_headBallDistance = headBallDistance;

	_deltaRadiusPerLength = (cueTailRadius - cueHeadRadius) / cueLength;

	_lenLimit = cueLength + headBallDistance + PhysicsConstants::BALL_RADIUS;
	_lenLimit2 = _lenLimit * _lenLimit;
}

CueAngleValues CueAngle::calcMinValuesWithTheta(float theta)
{
	auto dir = _direction;
	auto offset = _offset;

	auto dirLen = sqrt(dir.x *dir.x+ dir.y * dir.y);
	auto maxOffsetV = sqrt(MAX(0.f, MAX_OFFSET_2 - offset.x * offset.x));

	auto ballValue = calcMinValuesWithBalls(theta, dir, dirLen, maxOffsetV);
	auto cushionValue = calcMinValuesWithCushions(theta, dir, dirLen, maxOffsetV);

	if (ballValue.offsetV > cushionValue.offsetV
		|| (ballValue.offsetV == cushionValue.offsetV && ballValue.theta > cushionValue.theta))
		return ballValue;

	return cushionValue;
}

CueAngleValues CueAngle::calcMinValuesWithRPT(ps::Rpt rpt)
{
	auto dir = vector(-std::cos(rpt.p), -std::sin(rpt.p), 0);
	auto theta = rpt.t;
	auto offset = _offset;

	auto dirLen = sqrt(dir.x *dir.x + dir.y * dir.y);
	auto maxOffsetV = sqrt(MAX(0.f, MAX_OFFSET_2 - offset.x * offset.x));

	auto ballValue = calcMinValuesWithBalls(theta, dir, dirLen, maxOffsetV);
	auto cushionValue = calcMinValuesWithCushions(theta, dir, dirLen, maxOffsetV);

	if (ballValue.offsetV > cushionValue.offsetV
		|| (ballValue.offsetV == cushionValue.offsetV && ballValue.theta > cushionValue.theta))
		return ballValue;

	return cushionValue;
}


CueAngleValues CueAngle::calcMinValuesWithBalls(float theta, ps::ExtMath::vector dir, float dirLen, float maxOffsetV)
{
	const ps::ExtMath::vector cueBallPos = *_positions[0];
	auto sinTheta = sin(theta);

	CueAngleValues finalValues = CueAngleValues(MIN_OFFSET_V, 0);
	for (auto i = 1; i < _positions.size(); i++)
	{
		if(_balls->at(i)->isDisabled())
			continue;
		auto value = calcMinValueWithBall(cueBallPos, *_positions[i], sinTheta, dir, dirLen, maxOffsetV);

		if (value.offsetV > finalValues.offsetV)
		{
			finalValues = value;
		}
		else if (value.offsetV == finalValues.offsetV && value.sinTheta > finalValues.sinTheta)
		{
			finalValues = value;
		}
	}

	finalValues.theta = asin(finalValues.sinTheta);
	return finalValues;
}

CueAngleValues CueAngle::calcMinValueWithBall(const ps::ExtMath::vector & cueBallPos, const ps::ExtMath::vector & ballPos, float sinTheta, ps::ExtMath::vector dir, float dirLen, float maxOffsetV)
{
	auto dx = cueBallPos.x - ballPos.x;
	auto dy = cueBallPos.y - ballPos.y;

	auto dot = dx * dir.x + dy * dir.y;
	if (dot <= 0)
		return CueAngleValues(MIN_OFFSET_V, 0);

	auto d2 = dx * dx + dy * dy;

	if (d2 >= _lenLimit2) {
		return CueAngleValues(MIN_OFFSET_V, 0);
	}

	auto dCosPhi = dot / dirLen;
	auto d2SinPhi2 = d2 - dCosPhi * dCosPhi;

	auto R = calcCueRadius(sqrt(d2 + PhysicsConstants::BALL_RADIUS_SQUARE)) + PhysicsConstants::BALL_RADIUS;
	auto R2 = R * R;

	if (R2 < d2SinPhi2) {
		return CueAngleValues(MIN_OFFSET_V, 0);
	}

	auto cValue = sqrt(R2 - d2SinPhi2);
	auto offsetV = cValue - sinTheta * dCosPhi;

	if (offsetV < maxOffsetV)
	{
		return CueAngleValues(offsetV, sinTheta);
	}

	auto minSinThetaValue = cValue - maxOffsetV;
	if (minSinThetaValue < 0)
	{
		return CueAngleValues(maxOffsetV, 0);
	}
	else
	{
		return CueAngleValues(maxOffsetV, minSinThetaValue/dCosPhi);
	}
}

CueAngleValues CueAngle::calcMinValuesWithCushions(float theta, ps::ExtMath::vector dir, float dirLen, float maxOffsetV)
{
	auto cueBallPos = *_positions[0];

	CueAngleValues finalValues = CueAngleValues(MIN_OFFSET_V, 0);

	for (auto i = 0; i < CUSHION_LENGTH; ++i) {
		CueAngleValues values = calcMinValuesWithCushion(theta, cueBallPos, CUSHIONS[i], dir, dirLen, maxOffsetV);
		if (values.offsetV > finalValues.offsetV) {
			finalValues = values;
		}
		else if (values.offsetV == finalValues.offsetV) {
			if (values.theta > finalValues.theta) {
				finalValues = values;
			}
		}
	}

	return finalValues;
}

CueAngleValues CueAngle::calcMinValuesWithCushion(float theta, ps::ExtMath::vector position, CueAngleCushion cushion, ps::ExtMath::vector dir, float dirLen, float maxOffsetV)
{
	auto normal = cushion.normal;
	auto dotVal = dir.x * normal.x + dir.y * normal.y;
	if (dotVal <= 0) return CueAngleValues(MIN_OFFSET_V, 0);

	float lp;
	if (normal.x == 0) {
		lp = abs(position.y - cushion.origin.y);
	}
	else {
		lp = abs(position.x - cushion.origin.x);
	}

	auto d = dirLen * lp / dotVal;

	if (d >= _lenLimit) return CueAngleValues(MIN_OFFSET_V, 0, 0);

	auto r = calcCueRadius(d);

	auto offsetV = r - d * sin(theta) + CUSHION_DELTA_HEIGHT * cos(theta);

	if (offsetV < maxOffsetV) {
		return CueAngleValues(offsetV, theta, 0);
	}

	auto minTheta = solveTrigonometricEquation(d, -CUSHION_DELTA_HEIGHT, r - maxOffsetV);

	if (minTheta < 0)
		return CueAngleValues(maxOffsetV, 0, 0);
	else
		return CueAngleValues(maxOffsetV, minTheta, 0);
}

float CueAngle::solveTrigonometricEquation(float a, float b, float c)
{
	auto norm = sqrt(a * a + b * b);
	auto beta = calcAngle(b, a / norm);
	auto gamma = asin(c / norm);

	float result = 0;

	auto delta = gamma - beta;
	auto k = findK(delta);
	if (k != MIN_FLOAT) {
		result = delta + k * TWO_PI;
	}

	delta = PI - gamma - beta;
	k = findK(delta);
	if (k != MIN_FLOAT) {
		if (result == MIN_FLOAT) {
			result = delta + k * TWO_PI;
		}
		else {
			result = MIN(result, delta + k * TWO_PI);
		}
	}

	return result;
}

float CueAngle::findK(float alpha)
{
	auto under = -alpha / TWO_PI;
	auto up = under + 0.25;
	auto k = ceilf(under);
	if (k <= up)
		return k;
	return MIN_FLOAT;
}

float CueAngle::calcAngle(float sinSign, float cosValue)
{
	auto angle = acos(cosValue);
	if (sinSign < 0) {
		return TWO_PI - angle;
	}
	return angle;
}

float CueAngle::calcCueRadius(float pointToCueBallDistance)
{
	return _cueHeadRadius + _deltaRadiusPerLength *
		(pointToCueBallDistance - _headBallDistance);
}

const float CueAngle::MAX_OFFSET = PhysicsConstants::OFFSET_MAX_R;
const float CueAngle::MAX_OFFSET_2 = CueAngle::MAX_OFFSET * CueAngle::MAX_OFFSET;

const float CueAngle::MIN_OFFSET_V = std::numeric_limits<float>::min();

CueAngleCushion CueAngle::CUSHIONS[] = {
	CueAngleCushion(vector(1,0,0), vector(- PhysicsConstants::TABLE_WIDTH/2, 0, 0)),
	CueAngleCushion(vector(0,-1,0), vector(0, PhysicsConstants::TABLE_HEIGHT / 2, 0)),
	CueAngleCushion(vector(-1,0,0), vector(PhysicsConstants::TABLE_WIDTH / 2, 0, 0)),
	CueAngleCushion(vector(0,1,0), vector(0, -PhysicsConstants::TABLE_HEIGHT / 2, 0)),
};

const float CueAngle::CUSHION_DELTA_HEIGHT = PhysicsConstants::CUSHION_HEIGHT - PhysicsConstants::BALL_RADIUS;

const float CueAngle::MIN_FLOAT = std::numeric_limits<float>::min();

