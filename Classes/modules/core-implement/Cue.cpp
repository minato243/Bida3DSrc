#include "Cue.h"
#include <data/Resource.h>
#include <modules/core/ps/internal/PhysicsConstants.hpp>
#include "../camera-view/GameCamera.h"
#include "../core/utils/Utils.h"
#include "../ui/inventory/ModelOffset.h"
#include "SettingMgr.h"
#include "Ball.h"

USING_NS_CC;
using namespace ps;

const double Cue::OFFSET_DISTANCE = -5.5;
const double Cue::MOVE_IN_DISTANCE = -100;
const double Cue::MOVE_OUT_DISTANCE = -100;

Cue::Cue():_cue(nullptr)
{
	_state = NULL;
	_cue = NULL;
	_MRAOTexturePath = "";
	_swingCallback = NULL;
	_shootAnimCallback = NULL;

	_moveInCallbackAction = NULL;
	_moveOutCallbackAction = NULL;

	PhysicsCue();
	init();
	_cueInfo = NULL;
}

Cue::~Cue()
{
	CC_SAFE_DELETE(_moveInLerp);
	CC_SAFE_DELETE(_moveOutLerp);
	CC_SAFE_DELETE(_rotateLerp);
}


bool Cue::init()
{
	if (!Node::init()) {
		return false;
	}
	initCue();
	std::function<void(double)> moveInFunc = std::bind(&Cue::updateCueMoveIn, this , std::placeholders::_1);
	std::function<void(double)> moveOutFunc = std::bind(&Cue::updateCueMoveOut, this, std::placeholders::_1);
	std::function<void(ps::ExtMath::vector)> cueRotateFunc = std::bind(&Cue::updateCueRotate, this, std::placeholders::_1);
	_moveInLerp = new NumberLerp(0.08, moveInFunc);
	_moveOutLerp = new NumberLerp(0.125, moveOutFunc);
	_rotateLerp = new VectorLerp(0.125, cueRotateFunc);
	return true;
}

void Cue::initCue()
{
	ModelPathData modelData;
	modelData.model = Res::CUE_3D;
	loadModel(modelData);
}

void Cue::loadModel(ModelPathData paths)
{
	if (_cue != nullptr) {
		_cue->removeFromParent();
	}
	_cue = Sprite3D::create(paths.model);
	_paths.insert(std::pair<Sprite3D*, std::string>(_cue, paths.model));
	_cue->setCameraMask((int)GameCamera::FLAG_3D);
	addChild(_cue);

	bool isDefault = paths.model == Res::CUE_3D;
	if (!isDefault) {
		_cue->setTexture(paths.tex);
		_cue->setRotation3D(Vec3(0, 90, 0));
		//_cue->_MRAOTexturePath = paths.mrao;
		setupShader(_cue, shaderPrograms[qualityLevel], qualityLevel);
	}
	_cue->setScale(isDefault ? 1 : 0.45);
	if (!isDefault) {
		_cue->setPosition3D(Vec3(Cue::OFFSET_DISTANCE, 0, 0));
	}
}

void Cue::cueMoveByForce(double forcePercent) {
	auto pos = _cue->getPosition3D();
	pos.x = Cue::OFFSET_DISTANCE - forcePercent / 100 * 50;
	setAllCueModelPosition3D(pos);
}

void Cue::updateCueMoveIn(double x) {
	auto pos = _cue->getPosition3D();
	pos.x = x;
	setAllCueModelPosition3D(pos);
}

void Cue::updateCueMoveOut(double x) {
	auto pos = _cue->getPosition3D();
	pos.x = x;
	setAllCueModelPosition3D(pos);
	if (abs(x - Cue::MOVE_OUT_DISTANCE) < 1) {
		CCLOG("Disable");
		setVisible(false);
		pos.x = Cue::OFFSET_DISTANCE;
		setAllCueModelPosition3D(pos);
		_moveOutLerp->setEnabled(false);
	}
}

void Cue::setAllCueModelPosition3D(Vec3 pos) 
{
	_cue->setPosition3D(pos);
}

void Cue::initShader(GLProgram *shader, int level) {
	qualityLevel = level;
	shaderPrograms[level] = shader;

	setupShader(_cue, shader, level);
}


void Cue::setupShader(cocos2d::Sprite3D * cue, cocos2d::GLProgram *shader, int level)
{
	auto state = GLProgramState::create(shader);
	if (level >= 0)
	{
		state->setUniformVec3("u_light_color", Vec3(1, 1, 1));
		state->setUniformVec3("u_view_pos", Vec3(0, 160, 0));
		std::string texturePath = _MRAOTexturePath;
		if (_MRAOTexturePath.compare("") == 0) texturePath = Res::CUE_TEXTURE_SET_1_MRAO;
			
		auto mraoMap = TextureCache::getInstance()->addImage(texturePath);
		state->setUniformTexture("u_mrao_map", mraoMap);
		state->setUniformTexture("u_env_map", settingMgr->_tableEnvTexture);
		state->setUniformTexture("u_irradiance_map", settingMgr->_tableIrradianceTexture);
		state->setUniformTexture("u_brdf_lut", settingMgr->_brdfTexture);
		state->setUniformFloat("u_metallic", -1);
		state->setUniformFloat("u_roughness", -1);
		state->setUniformFloat("u_ao", -1);
		state->setUniformFloat("u_opacity", 1);
	}

	cue->setGLProgramState(state);
	cue->setLightMask(0);

	_state = state;
	setLights(Ball::DefaultLights, Ball::NUM_DEFAULT_LIGHT, Ball::DefaultLightColor);
}

void Cue::setCuePosition(vector position) {
	setPosition3D(Vec3(
		position.x,
		position.z + ps::PhysicsConstants::BALL_RADIUS,
		-position.y
	));
}

void Cue::updateCueRotate(vector direction) {
	setCueDirection(direction);
}

void Cue::setCueDirection(const ps::ExtMath::vector& direction) {
	CCLOG(" Cue::setCueDirection");
	auto l1 = direction.x * direction.x + direction.y * direction.y;
	auto l2 = l1 + direction.z * direction.z;

	auto pitch = std::acos(std::min(1., sqrt(l1 / l2)));

	auto yaw = std::acos(std::min(1., direction.x / sqrt(l1)));
	if (direction.y < 0) {
		yaw = 2 * ExtMath::PI - yaw;
	}

	// // Add 5 degree to make the cue visually higher => not touch the table
	auto quaternion_1 = ExtMath::quaternion(
		ExtMath::vector(0, -pitch, 0)
	);
	auto quaternion_2 = ExtMath::quaternion(
		ExtMath::vector(0, 0, yaw)
	);
	auto quaternion = ExtMath::quaternion::multiply(quaternion_1, quaternion_2);
	// auto quaternion = ExtMath::quaternion::quatFromVec(direction);
	auto curQua = getRotationQuat();
	setRotationQuat(cocos2d::Quaternion(
		quaternion.x,
		quaternion.z,
		quaternion.y,
		quaternion.w
	));
}

void Cue::doShootAnimation(double force, const CueAnimationCallBack &callback) {
	auto p = getPosition3D();
	auto dir = getDirection();
	force *= 100.f / 600;
	_shootAnimCallback = callback;

	auto animCallback = std::bind(&Cue::doShootAnimationComplete, this, force);

	chainMoveTo3D(std::vector<float>{0.3, 0.1}, std::vector<Vec3>{
		Vec3(p.x - dir.x * force, p.y - dir.z * force, p.z + dir.y * force),
		Vec3(p.x + dir.x * force * 0.05, p.y + dir.z * force * 0.05, p.z - dir.y * force * 0.05)
	}, animCallback);
}

void Cue::doShootAnimationComplete(double f)
{
	setOffset(vector(0, 0));
	if(_shootAnimCallback)
		_shootAnimCallback();

	if (f < 200)
	{
		swingCueOutOfView(false, true);
	}
	else
	{
		swingCueOutOfView(false, false);
	}
}

void Cue::chainMoveTo3D(std::vector<float> intervals, std::vector<cocos2d::Vec3> positions, std::function<void()> callback)
{
	if(positions.size() != intervals.size()) {
		CCLOG("need equal positions length + intervals length");
		return;
	}
	if (positions.size() < 1) {
		callback();
		return;
	}
	auto interval = intervals[0];
	auto position = positions[0];
	std::vector<float> newIntervals = std::vector<float>(intervals.begin()+1, intervals.end());
	std::vector<Vec3> newPositions = std::vector<Vec3>(positions.begin() + 1, positions.end());

	std::function<void()> newCallback = std::bind(&Cue::chainMoveTo3D, this, newIntervals, newPositions, callback);
	moveTo3D(interval, position, newCallback);
}

void Cue::moveTo3D(float interval, cocos2d::Vec3 position, std::function<void()> callback)
{
	_moveToStartPos = getPosition3D();
	_moveTo = position;
	_moveToCallback = callback;
	_moveToCurrentInterval = 0;
	_moveToInterval = interval;
}

void Cue::onEnter()
{
	Node::onEnter();
	scheduleUpdate();
}

void Cue::update(float dt)
{
	Node::update(dt);
	_moveInLerp->update(dt); 
	_moveOutLerp->update(dt);

	moveTo3DUpdate(dt);
	cueSwingUpdate(dt);
}

void Cue::moveTo3DUpdate(float dt)
{
	if (_moveToCurrentInterval < _moveToInterval) {
		auto  lastMoveToUpdate = false;
		_moveToCurrentInterval += dt;
		if (_moveToCurrentInterval >= _moveToInterval) {
			_moveToCurrentInterval = _moveToInterval;
			lastMoveToUpdate = true;
		}
		auto s = _moveToStartPos; // start
		auto d = _moveTo;			// destination
		auto cur = _moveToCurrentInterval;
		auto t = _moveToInterval;
		setPosition3D(
			Vec3(
				s.x * (t - cur) / t + d.x * cur / t,
				s.y * (t - cur) / t + d.y * cur / t,
				s.z * (t - cur) / t + d.z * cur / t
			));
		if (lastMoveToUpdate) {
			_moveToCallback();
		}
	}
	// if(_extrapolateDirection){
	// 	auto  delta = gv.ALMOST_REALTIME_UPDATE_DELTA;
	// 	auto  delay = _updateDelay;
	// 	if(delay > delta*3.5){
	// 		_extrapolatePosition = false;
	// 		_updateDelay = 0;
	// 		setCueDirection(gv.ExtensionMath.vector.unit(_newDir));
	// 		// body().position = _newPos;
	// 		if(_onDirectionChangeCallback){
	// 			_onDirectionChangeCallback(_newDir);
	// 		}
	// 		return;
	// 	}
	//
	// 	if(_updateDelay > delta*1.5){
	// 		_oldDir = _curDir;
	// 		delay = dt;
	// 	}
	// 	// _updateDelay += dt;
	// 	auto  newDir = gv.ExtensionMath.vector.mix(_oldDir, _newDir, delay/delta);
	// 	_curDir = newDir;
	// 	if(_onDirectionChangeCallback){
	// 		_onDirectionChangeCallback(newDir);
	// 	}
	// 	setCueDirection(gv.ExtensionMath.vector.unit(newDir));
	// 	// cc.log("extrapolating " + JSON.stringify(newPos));
	// 	_updateDelay += dt;
	// }
}

void Cue::cueSwingUpdate(float dt)
{
	bool isInAnimation = isInCueSwingAnimation();
	int direction = _swingParams.direction;

	if (isInAnimation)
	{
		_swingParams.swingCurrentInterval += dt * direction;
		float cur = _swingParams.swingCurrentInterval;
		if (_swingParams.swingCurrentInterval < _swingParams.swingPullbackInterval)
		{
			float t;
			if (_swingParams.direction > 0) 
			{
				t = Utils::easeOutSine(cur / _swingParams.swingPullbackInterval);
			}
			else
			{
				t = Utils::easeInOutQuad(cur / _swingParams.swingPullbackInterval);
				if (cur <= 0)
				{
					if (_swingParams.callback)
					{
						_swingParams.callback();
						_swingParams.callback = NULL;
					}
					return;
				}
			}
			float x = -15 * t;
			_cue->setPosition3D(Vec3(Cue::OFFSET_DISTANCE + x, getOffset().y, getOffset().x));
		}
		else if (cur < _swingParams.swingPullbackInterval + _swingParams.delay)
		{
			_cue->setPosition3D(Vec3(Cue::OFFSET_DISTANCE - 15, getOffset().y, getOffset().x));
		} 
		else
		{
			cur = _swingParams.swingCurrentInterval - _swingParams.swingPullbackInterval - _swingParams.delay;
			float total = _swingParams.swingInterval;
			if (direction > 0 && cur >= total)
			{
				if (_swingParams.callback != NULL)
				{
					_swingParams.callback();
					_swingParams.callback = NULL;
				}
				return;
			}

			float t = Utils::easeInOutSine(cur/total);
			
			float cueLength = modelOffset->getModelWidthByPath(_paths[_cue]) * 7 / 9 * _cue->getScale();

			float x = Cue::OFFSET_DISTANCE + cueLength * ExtMath::cos(t * M_PI / 2) - cueLength - 40 * t - 15;
			float y = cueLength * ExtMath::sin(t * M_PI / 2) + getOffset().y;
			float z = 15 * ExtMath::sin(t * M_PI) + getOffset().x;
			_cue->setPosition3D(Vec3(x, y, z));
			_cue->setRotation3D(Vec3(-t * 90, 90 - asin(z / cueLength) * 180 / M_PI, 0));

			if (t > Cue::SWING_OUT_TRANSPARENT_THRESHOLD) 
			{
				int opacity = 255 - (int)(255 * (t - Cue::SWING_OUT_TRANSPARENT_THRESHOLD) / Cue::SWING_OUT_TRANSPARENT_DURATION);
				setOpacity(opacity);
			}
		}
	}
}

void Cue::animateVisible(bool delay /*= false*/, CUSTOM_CALLBACK callback /*= NULL*/)
{
	setVisible(true);
	stopAllAnimateMovingActions();

	swingCueOutOfView(true, delay, NULL);
	_moveInCallback = callback;

	if (callback)
	{
		_moveInCallbackAction = Sequence::create(
			DelayTime::create(Cue::SWING_OUT_PULL_BACK_TIME + Cue::SWING_OUT_TIME),
			CallFunc::create(CC_CALLBACK_0(Cue::animationVisibleComplete, this)),
			NULL
		);
		runAction(_moveInCallbackAction);
	}
}

void Cue::animationVisibleComplete()
{
	if (_moveInCallback)
	{
		_moveInCallback();
		_moveInCallback = NULL;
	}
	_moveInCallbackAction = NULL;
}

void Cue::animateInvisible(CUSTOM_CALLBACK callback /*= NULL*/)
{
	_moveInLerp->setEnabled(false);
	stopAllAnimateMovingActions();
	_moveOutCallback = callback;

	if (callback)
	{
		_moveOutCallbackAction = Sequence::create(
			DelayTime::create(0.5),
			CallFunc::create(CC_CALLBACK_0(Cue::animationInvisibleComplete, this)),
			NULL
		);
		runAction(_moveOutCallbackAction);
	}
}

void Cue::animationInvisibleComplete()
{
	if (_moveOutCallback)
	{
		_moveOutCallback();
		_moveOutCallback = NULL;
	}
	_moveOutCallbackAction = NULL;
}

void Cue::setVisible(bool visible) {
	Node::setVisible(visible);
	if (!visible) {
		_moveInLerp->setEnabled(false);
	}
}

void Cue::onChangeCameraPosition(cocos2d::Vec3 position)
{
	if (_state) {
		_state->setUniformVec3("u_view_pos", position);
	}
}

double Cue::getForceMax()
{
	
	if (_cueInfo)
	{
		return _cueInfo->_force;
	}

	return DEFAULT_FORCE;
}

double Cue::getMinAngle()
{
	double ret = getAngle()[0];
	return ret;
}

double Cue::getMaxAngle()
{
	double ret = getAngle()[1];
	return ret;
}

double Cue::getMinRadian()
{
	double ret = getAngle()[0] * M_PI / 180;
	return ret;
}

double Cue::getMaxRadian()
{
	double ret = getAngle()[1] * M_PI / 180;
	return ret;
}

const double * Cue::getAngle()
{
	if (_cueInfo)
	{
		return _cueInfo->getAngle();
	}
	return DEFAULT_ANGLE;
}

void Cue::setCueInfo(CueInfo *cueInfo)
{
	PhysicsCue::setCueInfo(cueInfo);
	loadModel(_cueInfo->getModelPath());
	fastForwardSwing();
}

void Cue::swingCueOutOfView(bool swingIn, bool delay /*= false*/, CUSTOM_CALLBACK callback /*= NULL*/)
{
	setVisible(true);
	resetSwingParam(swingIn);
	_swingParams.delay = delay;
	_swingCallback = callback;
	_swingParams.callback = std::bind(&Cue::onSwingCueOfViewComplete, this, swingIn);
	cueSwingUpdate(0);
}

void Cue::onSwingCueOfViewComplete(bool swingIn)
{
	setVisible(swingIn);
	if (_swingCallback)
	{
		_swingCallback();
	}
}

bool Cue::isInCueSwingAnimation()
{
	int direction = _swingParams.direction;
	float delayTime = _swingParams.delay ? Cue::SWING_OUT_DELAY : 0;
	bool ret = (direction > 0 && _swingParams.swingCurrentInterval <= _swingParams.swingInterval + delayTime + _swingParams.swingPullbackInterval)
		|| (direction < 0 && _swingParams.swingCurrentInterval >= 0);
	
	return ret;
}

void Cue::resetSwingParam(bool swingIn, bool delay /*= false*/)
{
	_swingParams.delay = delay;
	float delayTime = delay ? Cue::SWING_OUT_DELAY : 0;
	_swingParams.swingCurrentInterval = swingIn ? Cue::SWING_OUT_PULL_BACK_TIME + Cue::SWING_OUT_TIME + delayTime : 0;
	_swingParams.swingPullbackInterval = Cue::SWING_OUT_PULL_BACK_TIME;
	_swingParams.swingInterval = Cue::SWING_OUT_TIME;
	_swingParams.swingStartPos = getPosition3D();
	_swingParams.callback = NULL;
	_swingParams.direction = swingIn ? -1 : 1;

	_cue->setRotation3D(Vec3(0, 90, 0));
	_cue->setPosition3D(Vec3(0, 0, 0));
	setOpacity(255);
}

void Cue::fastForwardSwing()
{
	int direction = _swingParams.direction;

	if (direction > 0) {
		_cue->setPosition3D(Vec3(-132, 78, 0));
		_cue->setRotation3D(Vec3(-90, 90, 0));
		_swingParams.swingCurrentInterval = _swingParams.swingInterval + _swingParams.swingPullbackInterval + (_swingParams.delay ? Cue::SWING_OUT_DELAY : 0);
	}
	else {
		_cue->setPosition3D(Vec3(Cue::OFFSET_DISTANCE, 0, 0));
		_cue->setRotation3D(Vec3(0, 90, 0));
		_swingParams.swingCurrentInterval = 0;
	}

	if (_swingParams.callback) {
		_swingParams.callback();
		_swingParams.callback = NULL;
	}
}

void Cue::setOpacity(int value)
{
	value = std::min(255, std::max(value, 0));
	if (value == _opacity) return;
	_opacity = value;
	for (int i = 0; i < _cues.size(); i ++) 
	{
		auto cue = _cues[i];
		auto state = _states[cue];
		if (state) {
			state->setUniformFloat("u_opacity", value / 255);
		}
	}
}

void Cue::setLights(const cocos2d::Vec3 *lightPos, int num, cocos2d::Vec3 lightColor)
{
	if (_state)
	{
		_state->setUniformVec3("u_light_color", lightColor);
		_state->setUniformInt("u_light_count", num);
		for (int i = 0; i < num; i++)
		{
			_state->setUniformVec3("u_light_pos_" + std::to_string(i), lightPos[i]);
		}
	}
}

void Cue::stopAllAnimateMovingActions()
{
	if (_moveOutCallbackAction) {
		stopAction(_moveOutCallbackAction);
		_moveOutCallbackAction = NULL;
	}
	if (_moveInCallbackAction) {
		stopAction(_moveInCallbackAction);
		_moveInCallbackAction = NULL;
	}
}

void Cue::setCueOffset(const ps::ExtMath::vector & offset)
{
	if (isInCueSwingAnimation())
	{
		return;
	}
	Vec3 pos = Vec3(Cue::OFFSET_DISTANCE, offset.y, offset.x);
	_cue->setPosition3D(pos);
}

void Cue::setOffset(const ps::ExtMath::vector & offset)
{
	PhysicsCue::setOffset(offset);
	setCueOffset(offset);
}

const float Cue::SWING_OUT_TIME = 1.5f;

const float Cue::SWING_OUT_PULL_BACK_TIME = 0.8f;

const float Cue::SWING_OUT_TRANSPARENT_THRESHOLD = 0.2f;

const float Cue::SWING_OUT_TRANSPARENT_DURATION = 0.4f;

const float Cue::SWING_OUT_DELAY = 1;

const double Cue::DEFAULT_ANGLE[2] = {3, 89};

const double Cue::DEFAULT_FORCE = 500;
