#include "BallBodyV2.h"
#include "PhysicsConstants.hpp"
#include "PhysicsWorldV2.h"

using namespace ps;

ps::BallBody::BallBody()
	:_id(-1), _disabled(false), _fallingInPocket(false), _hasFrictionAccel(false),
	_inHole(false), _pureRolling(true), _totallyInHole(false), _world(nullptr), _outOfTable(false), _positionBackup(vector::ZERO),
	 _isLockPos(false), _lockType(0), _subscribe(NULL)
{
}

void ps::BallBody::setId(const int & id)
{
	_id = id;
}



const int & BallBody::id() 
{
	return _id;
}

void BallBody::setWorld(PhysicsWorldV2 *world) 
{
	_world = world;
}

void BallBody::setMotionStatus(const vector & position, const quaternion & quat)
{
	setQuaternion(quat);
	_position = position;
}

void BallBody::setQuaternion(const quaternion & quat)
{
	_quaternion.w = quat.w;
	_quaternion.x = quat.x;
	_quaternion.y = quat.y;
	_quaternion.z = quat.z;
}

void BallBody::outOfTable()
{
	if (_outOfTable)
		return;
	_outOfTable = true;
	_world->onBallOutOfTable(this);
}

void BallBody::fallToPocket()
{
	if (_inHole)
		return;
	_inHole = true;
	_world->onBallFallToHole(this);
}

bool BallBody::isOutOfTable()
{
	return _outOfTable;
}
bool ps::BallBody::isInHolde()
{
	return _inHole;
}

void ps::BallBody::reset()
{
	_linearVelocity.assignZero();
	_angularVelocity.assignZero();
	_position.assignZero();

	// setQuaternion(...);

	_disabled = false;

	_inHole = false;
	_totallyInHole = false;
	_outOfTable = false;

	_fallingInPocket = false;
	_lockType = 0;

	resetShootingState();
}

void BallBody::resetShootingState()
{
	_hasFrictionAccel = false;
	_pureRolling = false;
}

void BallBody::resetToLockPos()
{
	resetWithoutLockInfo();
	_position = _lockPos;
}

void BallBody::resetWithoutLockInfo()
{
	auto lockType = _lockType;
	vector lockPos = _lockPos;

	reset();

	_lockPos = lockPos;
	_lockType = lockType;
}

void BallBody::backupPosition()
{
	_positionBackup = _position;
}

void BallBody::restorePosition()
{
	_position = _positionBackup;
}

bool BallBody::step(double dt)
{
	if (isMotionless()) 
		return false;

	vector &position = _position;
	vector &linearVelocity = _linearVelocity;

	if (_inHole && position.z < HOLE_LIMIT_Z) {
		_totallyInHole = true;
		_disabled = true;

		_linearVelocity.assignZero();
	}

	_position = vector::add(position, vector::multiply(dt, linearVelocity));

	if (ExtMath::isZeroNumber(position.z)) {
		_position.z = 0;
	}
	return true;
}

void BallBody::setPureRolling(bool pureRolling)
{
	_pureRolling = pureRolling;
	_hasFrictionAccel = false;
}

bool BallBody::checkDisabled()
{
	vector position = _position;

	if (_fallingInPocket && position.z < POCKET_LIMIT_Z) {
		_totallyInHole = true;
	}

	if (position.z < DISABLED_Z || _totallyInHole) {
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

void BallBody::updateFlyingVelocity(double dt)
{
	_linearVelocity.z -= dt * PhysicsConstants::G;
}

void BallBody::updatePureRollingVelocity(double dt)
{
	vector velocity = vector::add(
		_linearVelocity,
		vector::multiply(
			NEG_ROLLING_FRICTION_LENGTH * dt,
			vector::unit(_linearVelocity)
		)
	);

	if (!ExtMath::isGreatThanZeroNumber(vector::dot(velocity, _linearVelocity))) {
		_linearVelocity.assignZero();
		_angularVelocity.x = 0;
		_angularVelocity.y = 0;
	}
	else {
		_linearVelocity = velocity;
		syncPureRollingAngularVelocity();
	}
}

void BallBody::updateNonPureRollingVelocity(double dt)
{
	vector linearAngularVelocity = vector::crossVectorWithZVec(
		_angularVelocity, -PhysicsConstants::BALL_RADIUS
	);
	vector velocityAtBottomPoint = vector::add(linearAngularVelocity, _linearVelocity);

	vector crtFrcAccel = vector::multiply(
		NEG_SLIDING_FRICTION_LENGTH,
		vector::unit(velocityAtBottomPoint)
	);

	bool checkPureRolling = vector::equalsZero(crtFrcAccel) ||
		(_hasFrictionAccel && (vector::dot(_frictionAccel, crtFrcAccel) <= 0));

	if (checkPureRolling) {
		_pureRolling = true;
		return;
	}

	vector dv = vector::multiply(dt, crtFrcAccel);
	vector dw = vector::multiply(
		DW_CONSTANTS * dt,
		vector(crtFrcAccel.y, -crtFrcAccel.x, 0)
	);

	_linearVelocity = vector::add(_linearVelocity, dv);
	_angularVelocity = vector::add(_angularVelocity, dw);

	_frictionAccel = crtFrcAccel;
	_hasFrictionAccel = true;
}

void BallBody::updateSpinAngular(double dt)
{
	vector &angularVelocity = _angularVelocity;
	// Update spin acceleration
	double angularZ = angularVelocity.z;

	if (angularZ > 0) {
		angularVelocity.z -= (dt * SPINNING_FRICTION_ACCELERATION);

		if (angularVelocity.z * angularZ < 0) {
			angularVelocity.z = 0;
		}

	}
	else if (angularZ < 0) {
		angularVelocity.z += (dt * SPINNING_FRICTION_ACCELERATION);

		if (angularVelocity.z * angularZ < 0) {
			angularVelocity.z = 0;
		}
	}
}

void BallBody::updateVelocityWithGravity(double dt)
{
	FloorSurface *flrSur = _world->floorSurface();

	vector normAccel = flrSur->normalAcceleration(_position);
	vector totalAccel = vector::add(normAccel, PhysicsConstants::GRAVITY);

	_linearVelocity = vector::add(_linearVelocity, vector::multiply(dt, totalAccel));

	if (_fallingInPocket) {
		_linearVelocity = vector::multiply(vector::length(_linearVelocity), vector::unit(totalAccel));
	}
	else {
		if (totalAccel.x != 0 || totalAccel.y != 0) { // Not verticals
			// Update linear direction;
			_linearVelocity = vector::multiply(
				vector::length(_linearVelocity),
				vector::unit(totalAccel)
			);

			_fallingInPocket = true;
			fallToPocket();
		}
	}
}

void BallBody::updateVelocity(double dt)
{
	if (checkDisabled()) return;
	if (ExtMath::isGreatThanZeroNumber(_position.z)) {
		updateFlyingVelocity(dt);
		return;
	}

	if (_pureRolling) {
		updatePureRollingVelocity(dt);
	}
	else {
		updateNonPureRollingVelocity(dt);
	}

	updateSpinAngular(dt);
	updateVelocityWithGravity(dt);
}

void BallBody::updateQuaternion(double dt)
{
	vector angular = vector::multiply(dt, _angularVelocity);
	_quaternion = quaternion::multiply(
		quaternion(angular),
		_quaternion
	);
}

bool BallBody::isMotionless()
{
	if (_disabled) return true;
	return vector::equalsZero(_linearVelocity)
		&& vector::equalsZero(_angularVelocity);
}

void ps::BallBody::bounceFloor(vector & normalAcceleration)
{
	vector rflVel = vector::reflect(
		vector::reverse(_linearVelocity),
		normalAcceleration
	);
	double value = rflVel.z * (rflVel.z - BOUNCE_FLOOR_REFLECT_COEF);

	if (value > 0) {
		rflVel.z = sqrt(value);
	}
	else {
		rflVel.z = 0;
	}

	_linearVelocity = rflVel;
}

void ps::BallBody::bounceEdge(vector & normalAcceleration)
{
	_linearVelocity = vector::reflect(
		vector::reverse(_linearVelocity),
		normalAcceleration
	);
}

void ps::BallBody::bounceSegment(segment & seg)
{
	vector segmentDirection = vector::fromSegment(seg);
	vector collisionPoint = segment::projectPointOnLine(_position, seg);
	bounceSegmentDirection(segmentDirection, collisionPoint);
}

void ps::BallBody::bounceIntersectSegmentPoint(vector & p)
{
	vector r = vector::sub(_position, p);
	vector segmentDirection = vector::unit(vector(r.y, -r.x, 0));
	bounceSegmentDirection(segmentDirection, p);
}

void ps::BallBody::bounceSegmentDirection(vector & segmentDirection, vector & collisionPoint)
{
	vector &position = _position;
	vector &linearVelocity = _linearVelocity;
	vector &angularVelocity = _angularVelocity;

	vector rVec = vector::sub(collisionPoint, position);
	vector v_spin = vector::cross(angularVelocity, rVec);

	vector dv = vector::multiply(
		PhysicsConstants::SPIN_CUSHION_FRICTION,
		vector::add(v_spin, linearVelocity)
	);
	vector dw = vector::multiply(
		BOUNCE_DW_CONSTANTS,
		vector::cross(rVec, dv)
	);

	bool zIsZero = ExtMath::isZeroNumber(position.z);

	if (zIsZero) { dv.z = 0; }

	_angularVelocity = vector::sub(angularVelocity, dw);

	vector velocity = vector::sub(linearVelocity, dv);

	if (zIsZero) {
		bounceSegmentDirectionOnFloor(velocity, segmentDirection);
	}
	else {
		bounceSegmentDirectionOnSky(velocity, rVec);
	}

	if (!_pureRolling) {
		setPureRolling(false);
	}
}

double ps::BallBody::calcPenetrationSegmentTimeBack(segment & seg)
{
	vector u = vector::sub(seg.s, seg.e);
	vector PS = vector::sub(seg.s, _position);
	vector alpha = vector::cross(u, _linearVelocity);
	vector beta = vector::cross(u, PS);
	double gamma = BALL_RADIUS_SQUARE * vector::dot(u, u);

	double ab = vector::dot(alpha, beta);
	double aa = vector::dot(alpha, alpha);
	double bb = vector::dot(beta, beta);
	double delta = ab * ab - aa * (bb - gamma);

	return (ab - sqrt(delta)) / aa;
}

double ps::BallBody::calcPenetrationPointTimeBack(vector & point)
{
	vector u = vector::sub(_position, point);
	double uv = vector::dot(u, _linearVelocity);
	double vv = vector::dot(_linearVelocity, _linearVelocity);
	double uu = vector::dot(u, u);
	double delta = uv * uv - vv * (uu - BALL_RADIUS_SQUARE);

	return (-uv - sqrt(delta)) / vv;
}

void ps::BallBody::bounceSegmentDirectionOnFloor(vector & linearVelocity, vector & segDir)
{
	vector v1 = vector::project(linearVelocity, segDir);
	vector v2 = vector::sub(linearVelocity, v1);

	_linearVelocity = vector::sub(v1, vector::multiply(BOUNCE_CUSHION_COEF, v2));
}

void ps::BallBody::finalizeStep()
{
	_linearVelocity.fixZeroVectorPrecision();
	_angularVelocity.fixZeroVectorPrecision();
}

void ps::BallBody::enable()
{
	_disabled = false;
	_fallingInPocket = false;
	_inHole = false;
	_totallyInHole = false;
}

void ps::BallBody::disable()
{
	_disabled = true;
	_inHole = true;
	_totallyInHole = true;
	_fallingInPocket = true;
}

ps::ExtMath::vector ps::BallBody::lockPos()
{
	if (_isLockPos) return _lockPos;
	
	return vector::ZERO;
}

int ps::BallBody::isLockPos()
{
	return _lockType;
}

void ps::BallBody::setLockPos(const vector & pos)
{
	_lockPos = pos;
}

void ps::BallBody::makeCapture(BallBodyCapture & capture)
{
	capture.id = _id;
	capture.disabled = _disabled;
	capture.fallingInPocket = _fallingInPocket;
	capture.position = _position;
}

void ps::BallBody::bounceSegmentDirectionOnSky(vector & linearVelocity, vector & rVec)
{
	vector v1 = vector::project(linearVelocity, rVec);
	vector v2 = vector::sub(linearVelocity, v1);

	_linearVelocity = vector::sub(
		v2, 
		vector::multiply(BOUNCE_CUSHION_COEF, v1));
}

void BallBody::syncPureRollingAngularVelocity()
{
	double angularZ = _angularVelocity.z;

	vector angularVelocity = vector::crossZVecWithVector(
		INVERSE_BALL_RADIUS,
		_linearVelocity
	);

	angularVelocity.z = angularZ;

	_angularVelocity = angularVelocity;
}

vector & BallBody::position()
{
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

void BallBody::setPosition(const vector & pos)
{
	_position = pos;
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

bool BallBody::isInHole() {
	return _inHole;
}

void ps::BallBody::dispatch()
{
	if (!_subscribe) return;
	_subscribe();
}

void ps::BallBody::setSubcribe(std::function<void()> subcribe)
{
	_subscribe = subcribe;
}

bool ps::BallBody::isTotallyInHole()
{
	return _totallyInHole;
}

bool ps::BallBody::isDisabled()
{
	return _disabled;
}

void ps::BallBody::cheatTotalInHole(bool totalInHole)
{
	_totallyInHole = totalInHole;
}


const double ps::BallBody::HOLE_LIMIT_Z = -3 * PhysicsConstants::BALL_RADIUS;
const double ps::BallBody::POCKET_LIMIT_Z = - PhysicsConstants::BALL_RADIUS * 1.7;
const double ps::BallBody::DISABLED_Z = -75;

const double ps::BallBody::NEG_ROLLING_FRICTION_LENGTH = -PhysicsConstants::ROLLING_FRICTION_COEF * PhysicsConstants::G;;

const double ps::BallBody::BALL_RADIUS_SQUARE = PhysicsConstants::BALL_RADIUS_SQUARE;

const double ps::BallBody::NEG_SLIDING_FRICTION_LENGTH = -PhysicsConstants::SLIDING_FRICTION_COEF * PhysicsConstants::G;

const double ps::BallBody::DW_CONSTANTS = 2.5 / PhysicsConstants::BALL_RADIUS;;

const double ps::BallBody::SPINNING_FRICTION_ACCELERATION = PhysicsConstants::SPINING_FRICTION_COEF * PhysicsConstants::G;

const double ps::BallBody::BOUNCE_FLOOR_REFLECT_COEF = PhysicsConstants::BOUNCE_FLOOR_REFLECT_COEF;

const double ps::BallBody::BOUNCE_DW_CONSTANTS = 1. / PhysicsConstants::BALL_RADIUS_SQUARE;

const double ps::BallBody::BOUNCE_CUSHION_COEF = PhysicsConstants::BOUNCE_CUSHION_COEF;

const double ps::BallBody::INVERSE_BALL_RADIUS = 1. / PhysicsConstants::BALL_RADIUS;
