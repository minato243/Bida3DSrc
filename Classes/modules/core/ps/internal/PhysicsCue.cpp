#include "PhysicsCue.hpp"
#include "PhysicsConstants.hpp"
#include <algorithm>
#include <cmath>
#include <cocos2d.h>
#include "modules/core/inventory/CueInfo.h"
#include "core/utils/Utility.h"
#include "../../utils/Utils.h"

using namespace ps;

PhysicsCue::PhysicsCue() : _direction(1, 0, 0), _offset(0, 0) {
	initDefaultPhysicsCue();
}

void PhysicsCue::initDefaultPhysicsCue() {
	_attrs.tip_e = PhysicsConstants::TIP_E;
	_attrs.tip_coef = PhysicsConstants::TIP_COEF;
	_attrs.mass_ratio = PhysicsConstants::BALL_CUE_MASS_RATIO;
}

void PhysicsCue::setCueAttributes(const PhysicsCueAttributes & attrs) {
	_attrs.tip_e = attrs.tip_e;
	_attrs.tip_coef = attrs.tip_coef;
	_attrs.mass_ratio = attrs.mass_ratio;
}

void PhysicsCue::setDirection(const vector & direction) {
	_direction = direction;
}

void ps::PhysicsCue::updateDirection(const vector& direction)
{
   // CCLOG("direction %lf %lf %lf", direction.x, direction.y, direction.z);
    _direction = direction;
    setCueDirection(direction);
}

vector ps::PhysicsCue::getDirection()
{
    return _direction;
}

void PhysicsCue::setOffset(const vector & offset) {
    CCLOG("setOffset %f, %f", offset.x, offset.y);
	_offset = offset;
}

vector ps::PhysicsCue::getOffset()
{
    return this->_offset;
}

void ps::PhysicsCue::setCueInfo(CueInfo *data)
{
	_cueInfo = data;
	_attrs.tip_coef = data->_tip_coef;
	_attrs.tip_e = data->_tip_e;
}

CueInfo * ps::PhysicsCue::getCueInfo()
{
	return _cueInfo;
}

PhysicsCue::ResultVelocities PhysicsCue::calcCueBallVelocities(double force) {
    vector localPoint = calcCollisionLocalPoint(_offset, _direction);
	CUSTOMLOG("calcCueBallVelocities Force %.13lf, offset = (%.15lf %.15lf %.15lf), direction = (%.15lf %.15lf %.15lf), tipCoef = %.13lf",
		force, _offset.x, _offset.y, _offset.z, _direction.x, _direction.y, _direction.z, _attrs.tip_coef);
	CUSTOMLOG("offset %s %s %s", getBinaryDouble(_offset.x).c_str(), getBinaryDouble(_offset.y).c_str(), getBinaryDouble(_offset.z).c_str());
	CUSTOMLOG("direction %s %s %s", getBinaryDouble(_direction.x).c_str(), getBinaryDouble(_direction.y).c_str(), getBinaryDouble(_direction.z).c_str());
	CUSTOMLOG("force %s", getBinaryDouble(force).c_str());

    vector vR = vector::reverse(localPoint);

    vector velocity = vector::multiply(force, _direction);
    vector vy = vector::project(velocity, vR);
    vector vx = vector::sub(velocity, vy);
    vector Vy = vy;
    vector Vx = vx;

    vector W_dir = vector::unit(vector::cross(Vx, Vy));
    vector W = vector::multiply(2.5 * vector::length(Vx) * _attrs.tip_coef / PhysicsConstants::BALL_RADIUS, W_dir);
	CUSTOMLOG("W %s %s %s", getBinaryDouble(W.x).c_str(), getBinaryDouble(W.y).c_str(), getBinaryDouble(W.z).c_str());
    vector linearVelocity = vector::add(Vx, Vy);
    if (_direction.z < 0) {
        // Calculate bouncing
        linearVelocity = vector::reflect(
            vector::reverse(linearVelocity),
            vector(0, 0, 1)
        );
        double zVal = linearVelocity.z * (linearVelocity.z - PhysicsConstants::BOUNCE_FLOOR_REFLECT_COEF);
        linearVelocity.z = sqrt(std::max(0.0, zVal));
        // const pitch = mathlib.asin(Math.abs(direction.z));
        // if (pitch > Math.PI / 4) {

        // }
        vector velProj = vector::project(linearVelocity, _direction);
        if (vector::dot(velProj, _direction) < 0) {
            linearVelocity = vector::sub(linearVelocity, velProj);
        }
    }
	CUSTOMLOG("V %s %s %s", getBinaryDouble(linearVelocity.x).c_str(), getBinaryDouble(linearVelocity.y).c_str(), getBinaryDouble(linearVelocity.z).c_str());

    ResultVelocities result;
    result.linearVelocity = linearVelocity;
    result.angularVelocity = W;
	CUSTOMLOG("Result V = (%lf, %lf, %lf), (%lf, %lf, %lf)", linearVelocity.x, linearVelocity.y, linearVelocity.z, W.x, W.y, W.z);
    return result;
}

vector PhysicsCue::calcCollisionLocalPoint(const vector & offset, const vector & direction) {
    if (ExtMath::isZeroNumber(offset.x) && ExtMath::isZeroNumber(offset.y)) {
        vector point = vector::multiply(-PhysicsConstants::BALL_RADIUS, direction);
        return point;
    }

    vector upWorld = vector(0, 0, 1);
    vector right = vector::unit(vector::cross(direction, upWorld));

    if (vector::equalsZero(right)) {
        right = vector(0, -1, 0);
    }
    vector up = vector::unit(vector::cross(right, direction));
    vector offsetPoint = vector::add(
        vector::multiply(offset.x, right),
        vector::multiply(offset.y, up)
    );

    double uv = vector::dot(offsetPoint, direction);
    double uu = vector::dot(offsetPoint, offsetPoint);
    double vv = vector::dot(direction, direction);

    double delta = uv * uv - vv * (uu - PhysicsConstants::BALL_RADIUS_SQUARE);
    double t = (-uv - std::sqrt(delta)) / vv;

    vector point = vector::add(offsetPoint, vector::multiply(t, direction));
    return point;
}
