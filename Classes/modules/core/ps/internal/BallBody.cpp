#include "BallBody.hpp"
#include "PhysicsConstants.hpp"
#include "PhysicsWorldV2.h"
#include <algorithm>
#include <cmath>

using namespace ps;

BallBody::BallBody() :
    _id(-1), _disabled(false), _fallingInPocket(false), _hasFrictionAccel(false),
    _inHole(false), _pureRolling(true), _totallyInHole(false), _world(nullptr), _outOfTable(false)
{}

void BallBody::setId(int id) {
    _id = id;
}

int BallBody::id() {
    return _id;
}

void BallBody::setWorld(PhysicsWorldV2 * world) {
    _world = world;
}

void ps::BallBody::outOfTable()
{
	if (_outOfTable) 
		return;
	_outOfTable = true;
	_world->onBallOutOfTable(this);
}

void ps::BallBody::setQuaternion(const quaternion & quat)
{
	_quaternion.w = quat.w;
	_quaternion.x = quat.x;
	_quaternion.y = quat.y;
	_quaternion.z = quat.z;
}

void ps::BallBody::setMotionStatus(const vector & position, const quaternion & quat)
{
	setQuaternion(quat);
	setPosition(position);
}

void BallBody::fallToPocket() {
    if (_inHole) return;
    _inHole = true;
    _world->onBallFallToHole(this);
}

void BallBody::reset() {
    _linearVelocity.assignZero();
    _angularVelocity.assignZero();
    _position.assignZero();
    _disabled = false;
    _fallingInPocket = false;
    _inHole = false;
    _totallyInHole = false;
	_outOfTable = false;
    resetShootingState();
}

void BallBody::resetShootingState() {
    // _frictionAccel = nullptr;
    _hasFrictionAccel = false;
    _pureRolling = false;
}

void BallBody::setPosition(const vector & position) {
    _position = position;
}

void BallBody::backupPosition() {
    _positionBackup = _position;
}

void BallBody::restorePosition() {
    _position = _positionBackup;
}

bool BallBody::step(double dt) {
	if (isMotionless()) return false;

	if (_inHole && _position.z < HOLE_LIMIT_Z) {
		_totallyInHole = true;
		_disabled = true;

		_linearVelocity.assignZero();
	}

	_position = vector::add(_position, vector::multiply(dt, _linearVelocity));

	if (ExtMath::isZeroNumber(_position.z)) {
		_position.z = 0;
	}
	return true;
}

void BallBody::setPureRolling(bool pureRolling) {
    _pureRolling = pureRolling;
    // _frictionAccel = null;
    _hasFrictionAccel = false;
}

bool BallBody::checkDisabled() {
	
	if (_fallingInPocket && _position.z < POCKET_LIMIT_Z) {
		_totallyInHole = true;
	}

	if (_position.z < DISABLED_Z || _totallyInHole) {
		if (!_totallyInHole) {
			outOfTable();
		}

		_disabled = true;

		_linearVelocity.assignZero();
		_angularVelocity.assignZero();

		return true;
	}
	return false;
}

void BallBody::updateFlyingVelocity(double dt) {
    _linearVelocity = vector::add(
        _linearVelocity,
        vector::multiply(dt, PhysicsConstants::GRAVITY)
    );
}

void BallBody::updatePureRollingVelocity(double dt) {
    vector frictionAcceleration = vector::multiply(
        PhysicsConstants::ROLLING_FRICTION_COEF * PhysicsConstants::G,
        vector::reverse(vector::unit(_linearVelocity))
    );

    vector linearVelocity = vector::add(
        _linearVelocity,
        vector::multiply(dt, frictionAcceleration)
    );
    if (ExtMath::isLessThanZeroNumber(vector::dot(linearVelocity, _linearVelocity))) {
        _linearVelocity.assignZero();
        _angularVelocity.x = _angularVelocity.y = 0;
    } else {
        _linearVelocity = linearVelocity;
        syncPureRollingAngularVelocity();
    }
}

void BallBody::updateNonPureRollingVelocity(double dt) {
	vector &linearVelocity = _linearVelocity;
	vector &angularVelocity = _angularVelocity;
	vector &frictionAccel = _frictionAccel;

	vector linearAngularVelocity = vector::crossVectorWithZVec(
		angularVelocity, -PhysicsConstants::BALL_RADIUS
	);
	vector velocityAtBottomPoint = vector::add(linearAngularVelocity, linearVelocity);

	vector crtFrcAccel = vector::multiply(
		NEG_SLIDING_FRICTION_LENGTH,
		vector::unit(velocityAtBottomPoint)
	);

	boolean checkPureRolling = vector::equalsZero(crtFrcAccel) ||
		(_hasFrictionAccel && (vector::dot(frictionAccel, crtFrcAccel) <= 0));

	if (checkPureRolling) {
		_pureRolling = true;
		return;
	}

	vector dv = vector::multiply(dt, crtFrcAccel);
	vector dw = vector::multiply(
		DW_CONSTANTS * dt,
		vector(crtFrcAccel.y, -crtFrcAccel.x, 0)
	);

	setLinearVelocity(vector::add(linearVelocity, dv));
	setAngularVelocity(vector::add(angularVelocity, dw));

	_frictionAccel = crtFrcAccel;
	_hasFrictionAccel = true;
}

void BallBody::updateSpinAngular(double dt) {
    // Update spin acceleration
    double angularZ = _angularVelocity.z;
    if (angularZ != 0) {
        double angularZAcceleration = angularZ > 0
            ? -PhysicsConstants::SPINNING_FRICTION_ACCELERATION
            : PhysicsConstants::SPINNING_FRICTION_ACCELERATION;

        _angularVelocity.z += (dt * angularZAcceleration);
        if (_angularVelocity.z * angularZ < 0) {
            _angularVelocity.z = 0;
        }
    }
}

void BallBody::updateVelocityWithGravity(double dt) {
    vector normalAcceleration = _world->floorSurface()->normalAcceleration(_position);

    vector totalAcceleration = vector::add(normalAcceleration, PhysicsConstants::GRAVITY);
    _linearVelocity = vector::add(_linearVelocity, vector::multiply(dt, totalAcceleration));

    if (!_fallingInPocket) {
        if (totalAcceleration.x != 0 || totalAcceleration.y != 0) { // Not verticals
            // Update linear direction;
            double velocityLength = vector::length(_linearVelocity);
            _linearVelocity = vector::multiply(velocityLength, vector::unit(totalAcceleration));
            _fallingInPocket = true;
            fallToPocket();
        }
    } else {
        double velocityLength = vector::length(_linearVelocity);
        _linearVelocity = vector::multiply(velocityLength, vector::unit(totalAcceleration));
    }
}

void BallBody::updateQuaternion(double dt) {
    vector angular = vector::multiply(dt, _angularVelocity);
    _quaternion = quaternion::multiply(
        quaternion(angular),
        _quaternion
    );
}

void BallBody::updateVelocity(double dt) {
    if (checkDisabled()) return;
    if (ExtMath::isGreatThanZeroNumber(_position.z)) {
        updateFlyingVelocity(dt);
        return;
    }

    if (_pureRolling) {
        updatePureRollingVelocity(dt);
    } else {
        updateNonPureRollingVelocity(dt);
    }

    updateSpinAngular(dt);
    updateVelocityWithGravity(dt);
}

bool BallBody::isMotionless() {
    if (_disabled) return true;
    return vector::equalsZero(_linearVelocity)
        && vector::equalsZero(_angularVelocity);
}

void BallBody::bounceFloor(const vector & normalAcceleration) {
    vector reflectVelocity = vector::reflect(
        vector::reverse(_linearVelocity),
        normalAcceleration
    );
    double value = reflectVelocity.z * (reflectVelocity.z - PhysicsConstants::BOUNCE_FLOOR_REFLECT_COEF);
    reflectVelocity.z = std::sqrt(std::max(0.0, value));

    _linearVelocity = reflectVelocity;
}

void BallBody::bounceEdge(const vector & normalAcceleration) {
    vector reflectVelocity = vector::reflect(
            vector::reverse(_linearVelocity),
            normalAcceleration
    );

    _linearVelocity = reflectVelocity;
}

bool BallBody::isPureRolling() {
    return _pureRolling;
}

void BallBody::finalizeStep(double dt) {
    _linearVelocity.fixZeroVectorPrecision();
    _angularVelocity.fixZeroVectorPrecision();
}

void BallBody::bounceSegment(const segment & seg) {
    vector segmentDirection = vector::fromSegment(seg);
    vector collisionPoint = segment::projectPointOnLine(_position, seg);
    bounceSegmentDirection(segmentDirection, collisionPoint);
}

void BallBody::bounceIntersectSegmentPoint(const vector & p) {
    vector r = vector::sub(_position, p);
    vector up(0, 0, 1); // Move this to static vector
    vector segmentDirection = vector::unit(vector::cross(r, up));
    bounceSegmentDirection(segmentDirection, p);
}

void BallBody::bounceSegmentDirection(const vector & segmentDirection, const vector & collisionPoint) {
    vector rVec = vector::sub(collisionPoint, _position);
    vector v_spin = vector::cross(_angularVelocity, rVec);
    vector dv = vector::multiply(
        PhysicsConstants::SPIN_CUSHION_FRICTION,
        vector::add(v_spin, _linearVelocity)
    );

    vector dw = vector::multiply(
        1 / PhysicsConstants::BALL_RADIUS_SQUARE,
        vector::cross(rVec, dv)
    );

    bool zIsZero = ExtMath::isZeroNumber(_position.z);

    if (zIsZero) {
        dv.z = 0;
    }

    _angularVelocity = vector::sub(_angularVelocity, dw);

    vector linearVelocity = vector::sub(_linearVelocity, dv);

    if (zIsZero) {
        bounceSegmentDirectionOnFloor(linearVelocity, segmentDirection);
    } else {
        bounceSegmentDirectionOnSky(linearVelocity, rVec);
    }

    if (!isPureRolling()) {
        setPureRolling(false);
    }
}

void BallBody::bounceSegmentDirectionOnFloor(const vector & linearVelocity, const vector & segDir) {
    vector v1 = vector::project(linearVelocity, segDir);
    vector v2 = vector::sub(linearVelocity, v1);

    // _linearVelocity = vector::multiply(
    //         PhysicsConstants::BOUNCE_CUSHION_COEF,
    //         vector::add(v1, vector::reverse(v2))
    // );
    _linearVelocity = vector::add(
        v1,
        vector::multiply(-PhysicsConstants::BOUNCE_CUSHION_COEF, v2)
    );
}

void BallBody::bounceSegmentDirectionOnSky(const vector & linearVelocity, const vector & rVec) {
    vector v1 = vector::project(linearVelocity, rVec);
    vector v2 = vector::sub(linearVelocity, v1);

    // _linearVelocity = vector::multiply(
    //         PhysicsConstants::BOUNCE_CUSHION_COEF,
    //         vector::add(v2, vector::reverse(v1))
    // );
    _linearVelocity = vector::add(
        v2,
        vector::multiply(-PhysicsConstants::BOUNCE_CUSHION_COEF, v1)
    );
}

double BallBody::calcPenetrationSegmentTimeBack(const segment & seg) {
    vector u = vector::sub(seg.s, seg.e);
    vector PS = vector::sub(seg.s, _position);
    vector alpha = vector::cross(u, _linearVelocity);
    vector beta = vector::cross(u, PS);
    double gamma = PhysicsConstants::BALL_RADIUS_SQUARE * vector::dot(u, u);

    double ab = vector::dot(alpha, beta);
    double aa = vector::dot(alpha, alpha);
    double bb = vector::dot(beta, beta);
    double delta = ab * ab - aa * (bb - gamma);
    double t = (ab - std::sqrt(delta)) / aa;

    return t;
}

double BallBody::calcPenetrationPointTimeBack(const vector & point) {
    vector u = vector::sub(_position, point);
    double uv = vector::dot(u, _linearVelocity);
    double vv = vector::dot(_linearVelocity, _linearVelocity);
    double uu = vector::dot(u, u);
    double delta = uv * uv - vv * (uu - PhysicsConstants::BALL_RADIUS_SQUARE);

    double t = (-uv - std::sqrt(delta)) / vv;

    return t;
}

void BallBody::syncPureRollingAngularVelocity() {
    vector r_bottom = vector(0, 0, -PhysicsConstants::BALL_RADIUS);
    double angularZ = _angularVelocity.z;
    _angularVelocity = vector::multiply(
        1 / vector::dot(r_bottom, r_bottom),
        vector::cross(r_bottom, vector::reverse(_linearVelocity))
    );
    _angularVelocity.z = angularZ;	
}

bool BallBody::isDisabled() {
    return _disabled;
}

vector & BallBody::position() {
    return _position;
}

vector & BallBody::linearVelocity() {
    return _linearVelocity;
}

vector & BallBody::angularVelocity() {
    return _angularVelocity;
}

quaternion & BallBody::quaternionRotation() {
    return _quaternion;
}

void BallBody::setLinearVelocity(const vector & value) {
    _linearVelocity = value;
}

void BallBody::setAngularVelocity(const vector & value) {
    _angularVelocity = value;
}

bool BallBody::isFallingInPocket() {
    return _fallingInPocket;
}

void BallBody::enable() {
    _disabled = false;
    _fallingInPocket = false;
    // LocNB
    _inHole = false;
    _totallyInHole = false;
}

void BallBody::disable() {
    _disabled = true;
}

bool BallBody::isInHole() {
	return _inHole;
}

void ps::BallBody::dispatch()
{
    if (!this->_subscribe) return;

    //auto qua = this->_quaternion;
    //auto pos = this->_position;

    //auto lQua = this->_dispatchedQuaternion;
    //auto lPos = this->_dispatchedPosition;

    //if (pos.x == = lPos.x && pos.y == = lPos.y && pos.z == = lPos.z &&
    //    qua.x == = lQua.x && qua.y == = lQua.y && qua.z == = lQua.z && qua.w == = lQua.w
    //    ) return;

    //lPos.x = pos.x;
    //lPos.y = pos.y;
    //lPos.z = pos.z;

    //lQua.x = qua.x;
    //lQua.y = qua.y;
    //lQua.z = qua.z;
    //lQua.w = qua.w;

    this->_subscribe();
}

void ps::BallBody::setSubcribe(std::function<void()> subcribe)
{
    this->_subscribe = subcribe;
}

bool ps::BallBody::isTotallyInHole()
{
    return _totallyInHole;
}

bool ps::BallBody::isDisable()
{
    return _disabled;
}

void ps::BallBody::cheatTotalInHole(bool totalInHole)
{
	_totallyInHole = totalInHole;
}

void ps::BallBody::makeCapture(BallBodyCapture & capture)
{
	capture.id = _id;
	capture.disabled = _disabled;
	capture.fallingInPocket = _fallingInPocket;
	capture.position = _position;
}

const double ps::BallBody::POCKET_LIMIT_Z = -PhysicsConstants::BALL_RADIUS * 1.7;

const double ps::BallBody::DISABLED_Z = -75;

const double ps::BallBody::NEG_SLIDING_FRICTION_LENGTH = -PhysicsConstants::SLIDING_FRICTION_COEF * PhysicsConstants::G;;

const double ps::BallBody::DW_CONSTANTS = 2.5 / PhysicsConstants::BALL_RADIUS;

const double ps::BallBody::HOLE_LIMIT_Z = -3 * PhysicsConstants::BALL_RADIUS;
