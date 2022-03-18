#include "LerpUtils.h"
#include <data/GameConstant.h>
#include <modules/core/ps/internal/ExtensionMath.hpp>

USING_NS_CC;
using namespace ps;
using namespace ps::ExtMath;

const double NUMBER_LERP_STOP_RPT = 1e-4;
const double VECTOR_LERP_STOP_RPT = 1e-4;
const double SPHERICAL_LERP_STOP_RPT = 1e-4;

SphericalLerp:: SphericalLerp(double lerp, const SphericalLerpCallBack & listener)
{
	_lerp = lerp * 60;
	_current = Rpt(0, 0 , 0);
	_target = Rpt(0, 0, 0);
	this->_enabled = false;
	this->_listener = listener;
}

SphericalLerp::~SphericalLerp()
{
}

void SphericalLerp::setCurrent(Rpt current)
{
	//CCLOG("SphericalLerp.setCurrent %lf %lf %lf", current.r, current.p, current.t);
	this->_current = current;
}

void SphericalLerp::setTarget(Rpt target)
{
	//CCLOG("SphericalLerp.setTarget %lf %lf %lf", target.r, target.p, target.t);
	this->_target = target;
}

void SphericalLerp::fastForward()
{
	setCurrent(this->_target);
}

void SphericalLerp::dispatch()
{
	_listener(_current);
}

void SphericalLerp::update(float deltaTime)
{
	if (!_enabled) return;

	if (_target.r == _current.r
		&& _target.p == _current.p
		&& _target.t == _current.t) 
		return;

	auto lerp = _lerp * deltaTime;
	auto dr = _target.r - _current.r;
	auto dp = _target.p - _current.p;
	auto dt = _target.t - _current.t;

	if ((abs(dr) + abs(dp) + abs(dt)) < SPHERICAL_LERP_STOP_RPT) {
		_current.r = _target.r;
		_current.p = _target.p;
		_current.t = _target.t;
	}
	else {
		_current.r = _current.r + dr * lerp;
		_current.p = _current.p + dp * lerp;
		_current.t = _current.t + dt * lerp;
	}
	_listener(_current);
}

void SphericalLerp::simplifyRadian()
{
	this->_current.p = ExtMath::simplifyRadian(this->_current.p);
	this->_target.p = ExtMath::simplifyRadian(this->_target.p);
}

Rpt SphericalLerp::getCurrent()
{
	return this->_current;
}

void SphericalLerp::onceCompleted(std::function<void()> callback)
{
	/*this->_completedListener = {
			context: context,
			handler : callback
	};*/
}

SphericalLerp* SphericalLerp::makeLerp(double lerp, const SphericalLerpCallBack &listener)
{
	return new SphericalLerp(lerp, listener);
}

VectorLerp::VectorLerp(double lerp, VectorLerpCallBack listener) 
{
	this->_lerp = lerp * 60;
	this->_current = ExtMath::vector();
	this->_target = ExtMath::vector();
	this->_enabled = false;
	this->_listener = listener;
}

VectorLerp::~VectorLerp() {

}

void VectorLerp::setCurrent(ExtMath::vector current) {
	//CCLOG("  VectorLerp::setCurrent %lf %lf %lf", current.x, current.y, current.z);
	this->_current.x = current.x;
	this->_current.y = current.y;
	this->_current.z = current.z;
}

void VectorLerp::setTarget(ExtMath::vector target) {
	//CCLOG(" VectorLerp::setTarget %lf %lf %lf", target.x, target.y, target.z);
	this->_target.x = target.x;
	this->_target.y = target.y;
	this->_target.z = target.z;
}

void VectorLerp::fastForward()
{
	this->setCurrent(this->_target);
}

void VectorLerp::dispatch() {
	this->_listener(this->_current);
}

void VectorLerp::update(float dt) {
	if (!_enabled) return;

	if (_target.x == _current.x
		&& _target.y == _current.y
		&& _target.z == _current.z
		) return;

	auto lerp = _lerp * dt;
	auto dx = _target.x - _current.x;
	auto dy = _target.y - _current.y;
	auto dz = _target.z - _current.z;

	if ((abs(dx) + abs(dy) + abs(dz)) < VECTOR_LERP_STOP_RPT) {
		_current.x = _target.x;
		_current.y = _target.y;
		_current.z = _target.z;
	}
	else {
		_current.x = _current.x + dx * lerp;
		_current.y = _current.y + dy * lerp;
		_current.z = _current.z + dz * lerp;
	}
	_listener(_current);
}

const ps::ExtMath::vector & VectorLerp::getCurrent()
{
	return this->_current;
}

VectorLerp * VectorLerp::makeLerp(double lerp, const VectorLerpCallBack &listener)
{
	return new VectorLerp(lerp, listener);
}

NumberLerp::NumberLerp(double lerp, const NumberLerpCallBack &listener)
{
	this->_lerp = lerp * 60;
	this->_current = 0;
	this->_target = 0;
	this->_enabled = false;
	this->_listener = listener;
}

NumberLerp::~NumberLerp()
{
}

void NumberLerp::setCurrent(double current) 
{
	this->_current = current;
}

void NumberLerp::setTarget(double target) 
{
	this->_target = target;
}

void NumberLerp::fastForward()
{
	this->setCurrent(this->_target);
}

void NumberLerp::dispatch()
{
	this->_listener(this->_current);
}

void NumberLerp::update(float dt) {
	if (!this->_enabled) return;
	if (this->_target == this->_current) return;

	auto lerp = this->_lerp * dt;
	auto d = this->_target - this->_current;

	if (abs(d) < NUMBER_LERP_STOP_RPT) {
		this->_current = this->_target;
	}
	else {
		this->_current = this->_current + d * lerp;
	}
	this->_listener(this->_current);
}

NumberLerp * NumberLerp::makeLerp(double lerp, const NumberLerpCallBack &listener)
{
	return new NumberLerp(lerp, listener);
}

RPTOLerp::RPTOLerp(double lerp, const RPTOLerpCallBack &listener)
{
	_resultRPT = Rpt(0, 0, 0);
	_resultOff = ps::ExtMath::vector(0,0);

	_rptLerp = new SphericalLerp(lerp, CC_CALLBACK_1(RPTOLerp::handleRPT, this));
	_offLerp = new VectorLerp(lerp, CC_CALLBACK_1(RPTOLerp::handleOff, this));

	_listener = listener;
	_enabled = false;
}

RPTOLerp::~RPTOLerp()
{
	CC_SAFE_DELETE(_rptLerp);
	CC_SAFE_DELETE(_offLerp);
}

void RPTOLerp::setCurrent(Rpt currentRPT, ps::ExtMath::vector currentOff)
{

	_rptLerp->setCurrent(currentRPT);
	_resultRPT.r = currentRPT.r;
	_resultRPT.p = currentRPT.p;
	_resultRPT.t = currentRPT.t;

	_offLerp->setCurrent(currentOff);
	_resultOff.x = currentOff.x;
	_resultOff.y = currentOff.y;
	_resultOff.z = currentOff.z;
}

void RPTOLerp::setTarget(const ps::ExtMath::Rpt & targetRPT, const ps::ExtMath::vector & targetOff)
{
	//CCLOG("RPTOLerp::setTarget targetOff %f %f %f", targetOff.x, targetOff.y, targetOff.z);
	_rptLerp->setTarget(targetRPT);
	_offLerp->setTarget(targetOff);
}

void RPTOLerp::setTarget(Rpt targetRPT)
{
	_rptLerp->setTarget(targetRPT);
}

void RPTOLerp::setEnabled(bool enabled)
{
	_enabled = enabled;
	_rptLerp->setEnabled(enabled);
	_offLerp->setEnabled(enabled);
}

void RPTOLerp::fastForward()
{
	CCLOG("RPTOLerp::fastForward");
	_rptLerp->fastForward();
	_offLerp->fastForward();
}

void RPTOLerp::dispatch()
{
	_rptLerp->dispatch();
	_offLerp->dispatch();
	_listener(_resultRPT, _resultOff);
}

void RPTOLerp::update(float dt)
{
	_updated = false;
	_rptLerp->update(dt);
	_offLerp->update(dt);
	if (_updated) {
		//CCLOG("RPTOLerp::updateED");
		_listener(_rptLerp->getCurrent(), _offLerp->getCurrent());
	}
}

void RPTOLerp::handleRPT(Rpt value)
{
	_resultRPT = value;
	_updated = true;
}

void RPTOLerp::handleOff(ps::ExtMath::vector value)
{
	_resultOff = value;
	_updated = true;
}

void RPTOLerp::simplifyRadian()
{
	_rptLerp->simplifyRadian();
}

void RPTOLerp::onceCompleted(CUSTOM_CALLBACK callback)
{
	_completedListener = callback;
}

void RPTOLerp::clearOnceCompletedListener()
{
	_completedListener = nullptr;
}

RPTOLerp * RPTOLerp::makeLerp(double lerp, const RPTOLerpCallBack &listener)
{
	return new RPTOLerp(lerp, listener);
}
