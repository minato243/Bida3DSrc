#include "StaticMotionController.h"
#include "StaticWorld.h"
#include "modules/core/ps/internal/PhysicsConstants.hpp"
#include "../internal/PhysicSimulateResultV2.h"

using namespace ps;

ps::StaticMotionResult::StaticMotionResult()
{
	endTime = 0;
}

ps::StaticMotionResult::~StaticMotionResult()
{

}

ps::StaticMotionController::StaticMotionController()
{

}

ps::StaticMotionController::~StaticMotionController()
{

}

void ps::StaticMotionController::setMotions(std::vector<StaticWorld::MotionGroup *> motionGroups, const quaternion & quat)
{
	_motionGroups = motionGroups;
	_index = 0;
	_t = 0;

	if (motionGroups.size() == 0) 
	{
		_endTime = 0;
		_curMotionEndTime = 0;
	}
	else
	{
		_endTime = getMotionEndTime(motionGroups.back());
		_curMotionEndTime = getMotionEndTime(motionGroups[0]);
		_quatCalc.resetAngle(motionGroups[0]->angularMotion.origin, quat);
	}
}

ps::ExtMath::vector ps::StaticMotionController::calcFinalPosition()
{
	if (_motionGroups.size() == 0)
		return vector::ZERO;
	 
	StaticWorld::MotionGroup *motionGroup = _motionGroups.back();
	StaticMotion motion = motionGroup->linearMotion;

	return motion.calcPosition(motion.endTime);
}

ps::StaticMotionResult* ps::StaticMotionController::calcFinalResult()
{
	StaticMotionResult *result = new StaticMotionResult();
	if (_motionGroups.size() == 0)
		return NULL;

	StaticWorld::MotionGroup *group = _motionGroups.back();
	result->endTime = group->endTime;
	MotionResult motionResult = group->linearMotion.calcAll(result->endTime);
	result->position = motionResult.position;
	result->linearVelocity = motionResult.velocity;
	result->angularVelocity = group->angularMotion.calcVelocity(result->endTime);

	return result;

}

double ps::StaticMotionController::getEndTime()
{
	return _endTime;
}

void ps::StaticMotionController::updateSimulateResult(PhysicSimulateResult & result)
{
	std::list<PhysicSimulateResult::BallCushionPair> pair = result.cushionCollisions;
	for (auto i = 0; i < _motionGroups.size(); i ++)
	{
		StaticWorld::MotionGroup* motionGroup = _motionGroups.at(i);
		int type = motionGroup->event->type;

		if (type == StaticWorld::TYPE_EVENT_COLLIDE_CUSHION) {
			pair.push_back(PhysicSimulateResult::BallCushionPair(
				motionGroup->linearMotion.endTime,
				PhysicsConstants::CUE_BALL_ID,
				((StaticWorld::CushionEvent *)motionGroup->event)->cushionId
			));
		}
		else if (type == StaticWorld::TYPE_EVENT_COLLIDE_CUSHION) {
			pair.push_back(PhysicSimulateResult::BallCushionPair(
				motionGroup->linearMotion.endTime,
				PhysicsConstants::CUE_BALL_ID,
				((StaticWorld::CushionPointEvent *)motionGroup->event)->cushionId
			));
		}
	}
}

ps::MasseCueBall *ps::StaticMotionController::calMasseCueBall()
{
	StaticWorld::MotionGroup *startMotion = NULL;
	StaticWorld::MotionGroup *endMotion = NULL;

	for (auto i = 0; i < _motionGroups.size(); i++)
	{

		auto motionGroup = _motionGroups[i];
		if (motionGroup->linearMotion.velocity.z == 0) {
			startMotion = motionGroup;
			break;
		}
	}

	for (auto motionGroup : _motionGroups) {
		//tim thoi diem dau tien vector motion bat dau chuyen dong
		if (motionGroup->event != NULL && motionGroup->event->type != StaticWorld::TYPE_EVENT_MOTION_EXPIRED) {
			endMotion = motionGroup;
			break;
		}
		if (ExtMath::vector::checkVectorsParallel(motionGroup->linearMotion.velocity, motionGroup->linearMotion.acceleration)) {
			endMotion = motionGroup;
		}
		else break;
	}

	if (endMotion == NULL)
		endMotion = _motionGroups.at(_motionGroups.size() - 1);

	return new MasseCueBall(startMotion, endMotion);
}

std::list<ps::PhysicSimulateResult::FlyingCueBall> ps::StaticMotionController::calFlyingCueBall()
{
	std::list<PhysicSimulateResult::FlyingCueBall> _flyingCueBalls;
	double cueBallTimeCheckPoint = -1;
	double cueBallHighCheckPoint = 0;

	for (StaticWorld::MotionGroup *motionGroup : _motionGroups) {
		if (cueBallTimeCheckPoint == -1 && motionGroup->linearMotion.velocity.z > 0) {
			cueBallTimeCheckPoint = motionGroup->endTime;
		}
		else if (cueBallTimeCheckPoint != -1 && motionGroup->linearMotion.velocity.z >= 0) {
			cueBallHighCheckPoint = MAX(cueBallHighCheckPoint, motionGroup->linearMotion.velocity.z);
		}
		else if (cueBallTimeCheckPoint != -1 && motionGroup->linearMotion.velocity.z == 0) {
			_flyingCueBalls.push_back(PhysicSimulateResult::FlyingCueBall(cueBallTimeCheckPoint, motionGroup->endTime, cueBallHighCheckPoint));
			cueBallTimeCheckPoint = -1;
		}
	}

	return _flyingCueBalls;
}

ps::ExtMath::vector ps::StaticMotionController::position()
{
	return _motionGroups[_index]->linearMotion.calcPosition(_t);
}

void ps::StaticMotionController::update(double dt)
{
	if (isFinished()) return;
	_t += dt;
	if (_t > _endTime) {
		_t = _endTime;
		_index = _motionGroups.size() - 1;
		_curMotionEndTime = _endTime;
	}
	else {
		updateMotionIndex();
	}

	updateQuaternion();
}

bool ps::StaticMotionController::isFinished()
{
	return _t >= _endTime;
}

void ps::StaticMotionController::updateMotionIndex()
{
	while (_t > _curMotionEndTime)
	{
		onMotionFinishedRunning(_motionGroups[_index]);
		_curMotionEndTime = getMotionEndTime(_motionGroups[++_index]);
	}
}

void ps::StaticMotionController::updateQuaternion()
{
	StaticMotion & angularMotion = _motionGroups[_index]->angularMotion;
	auto angle = angularMotion.calcPosition(_t);
	_quatCalc.updateAngle(angle);
}

void ps::StaticMotionController::onMotionFinishedRunning(StaticWorld::MotionGroup *motionGroup)
{
	auto type = motionGroup->event->type;
	switch (type) {
	case 1:
		processCollideWithCushion(motionGroup);
		break;
	case 3:
		processCollideWithCushionPoint(motionGroup);
	}
}

void ps::StaticMotionController::processCollideWithCushion(StaticWorld::MotionGroup *motionGroup)
{
	//TODO
	//auto cushionId = 1;
	//auto cueBall = _physicsWorld.balls[0];

	//const data = {
	//	useId: true,
	//	ballType : cueBall.ballType,
	//	id : cueBall.id,
	//	cushion : motionGroup.event.segmentId
	//};
	//_physicsWorld->onEvent(TableEvent.BALL_COLLIDE_CUSHION, data);
}

void ps::StaticMotionController::processCollideWithCushionPoint(StaticWorld::MotionGroup *motionGroup)
{
	//TODO

	/*auto cueBall = this._physicsWorld.balls[0];
	const data = {
		useId: true,
		ballType : cueBall.ballType,
		id : cueBall.id,
		cushion : -1
	};
	this._physicsWorld.onEvent(TableEvent.BALL_COLLIDE_CUSHION, data);*/
}

double ps::StaticMotionController::getMotionEndTime(StaticWorld::MotionGroup *motionGroup)
{
	StaticMotion & linearMotion = motionGroup->linearMotion;
	StaticMotion & angularMotion = motionGroup->angularMotion;

	if (linearMotion.startTime != -1)
		return linearMotion.endTime;
	if (angularMotion.startTime != -1)
		return angularMotion.endTime;

	return Infinity;
}

ps::ExtMath::quaternion ps::StaticMotionController::quat()
{
	return _quatCalc._quaternion;
}

//QuaternionCalculator
ps::QuaternionCalculator::QuaternionCalculator()
{
	_eulerAngle = vector::ZERO;
	_quaternion = quaternion();
}

void ps::QuaternionCalculator::resetAngle(const vector & eulerAngle, const quaternion & quat)
{
	_eulerAngle = eulerAngle;
	_quaternion = quat;
}

void ps::QuaternionCalculator::updateAngle(const vector & eulerAngle)
{
	auto delta = vector::sub(eulerAngle, _eulerAngle);
	_eulerAngle = eulerAngle;

	_quaternion = quaternion::multiply(
		quaternion::quatFromVec(delta),
		_quaternion
	);
}

