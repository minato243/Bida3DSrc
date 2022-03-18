#include "Ball.h"
#include <data/Resource.h>
#include <modules/core/ps/internal/ExtensionMath.hpp>
#include <modules/core/ps/internal/PhysicsConstants.hpp>
#include <cmath>
#include <data/GameConstant.h>
#include "../core/ps/internal/PhysicsWorldV2.h"
#include <modules/core-implement/SettingMgr.h>
USING_NS_CC;
using namespace ps;

const Vec3 Ball::DefaultLights[NUM_DEFAULT_LIGHT] = { Vec3(-200, 150, 100), Vec3(-200, 150, -100), Vec3(200, 150, -100), Vec3(200, 150, 100) };
const Vec3 Ball::DefaultLightColor = Vec3(1, 1, 1);
const Vec3 Ball::DefaultTableLights[2] = { Vec3(-200, 150, 100), Vec3(200, 150, -100) };
const Vec3 Ball::TableETCLights[2] = { Vec3(0, 30, -175), Vec3(0, 30, 175) };

const double Ball::ROUGHNESS_2D = 0.5;
const double Ball::METAL = 0.02;
const double Ball::ROUGHNESS = 0.23;
const double Ball::ROUGHNESS_EFFECT = 0.35;

Ball* Ball::createBall()
{
	return Ball::create();
}

Ball* Ball::createBall(int id)
{
	Ball *ball = Ball::create();
	ball->body()->setId(id);
	return ball;
}


bool Ball::init()
{
	if (!Node::init()) {
		return false;
	}
	initBall();
	initPhysicsBody();
	return true;
}

void Ball::initBall()
{
	//TODO
	auto ball = Sprite3D::create(Res::BALL_HI_POLY_2);
	ball->setScale(1);
	_ball = ball;
	_specular = 4;
	metal = 0.02;
	rough = 0.23;
	

	auto properties = Properties::createNonRefCounted("res/material/3d_ball.material#ball");
	_mat = Material::createWithProperties(properties);
	ball->setMaterial(_mat);
	

	addChild(ball);
	initShadow();
	initHighlight();

	// DEBUG
	initDebugIndicator();
	// setMovingArrowEnabled(false);

}

void Ball::initShader(int level)
{
	CCLOG("initShader");
	qualityLevel = level;
	// if (!Ball.USING_BALL_SHADER) return;
	// _shadow.circle.setVisible(level < gv.SettingMgr.Quality.BEST);
	auto ball = _ball;
	Technique *technique;

	switch (level) {
	case 0:
		ball->getMaterial(0)->setTechnique("low");
		technique = ball->getMaterial(0)->getTechniqueByName("low");
		break;
	case 1:
		ball->getMaterial(0)->setTechnique("medium");
		technique = ball->getMaterial(0)->getTechniqueByName("medium");
		break;
	case 2:
		ball->getMaterial(0)->setTechnique("high");
		technique = ball->getMaterial(0)->getTechniqueByName("high");
		break;
	}
	GLProgramState* state = technique->getPassByIndex(0)->getGLProgramState();
	// state = cc.GLProgramState.create(shader);

	if (level >= 0) {
		state->setUniformVec3("u_light_color", Vec3(1, 1, 1));
		state->setUniformVec3("u_view_pos", Vec3(0, 160, 0));
	}

	if (level >= 0) {
		state->setUniformFloat("u_ao", 1);
		state->setUniformFloat("u_metallic", metal);
		state->setUniformFloat("u_roughness", rough);

		state->setUniformTexture("u_env_map", settingMgr->_ballEnvTexture);
		state->setUniformTexture("u_irradiance_map", settingMgr->_ballIrradianceTexture);
		state->setUniformTexture("u_brdf_lut", settingMgr->_brdfTexture);
	}
	if (level <= 1) {
		state->setUniformFloat("u_opacity", 1);
	}

	// ball->setGLProgramState(state);
	ball->setLightMask(0);
	_state = state;
	setNumber(_number);
	setLights(std::vector<Vec3>(Ball::DefaultLights, Ball::DefaultLights+4), Ball::DefaultLightColor);
}

void Ball::initPhysicsBody()
{
	auto body = new ps::BallBody();
	_body = body;
	std::function<void()> func = std::bind(&Ball::onBodyChange, this);
	_body->setSubcribe(func);
}

void Ball::setNumber(int number)
{
	_number = number;
	
	_ball->setTexture(Res::Ball_Textures[(number - 1) % 15 + 1]);
}

void Ball::initShadow()
{
	CCLOG("initShadow");
	auto shadow = Sprite3D::create();
	auto circle = Sprite::create(Res::BALL_SHADOW);
	auto node = Node::create();
	circle->setScale(0.018);
	circle->setRotation3D(Vec3(-90, 0, 0));
	circle->setOpacity(120);

	auto circle2 = Sprite::create(Res::BALL_SHADOW);
	circle2->setScale(0.018);
	circle2->setPosition(-1.5, 0);
	circle2->setOpacity(120);
	circle2->setRotation3D(Vec3(-90, 0, 0));
	node->addChild(circle2);

	auto circle3 = Sprite::create(Res::BALL_SHADOW);
	circle3->setScale(0.018);
	circle3->setPosition(1.5, 0);
	circle3->setOpacity(120);
	circle3->setRotation3D(Vec3(-90, 0, 0));
	node->addChild(circle3);

	shadow->addChild(node);
	node->addChild(circle);
	shadow->setPosition3D(Vec3(0, 1, 0));
	addChild(shadow);
	_shadow = shadow;
}

void Ball::initHighlight()
{
	_highlight = Sprite::create(Res::INGAME_POCKET_INDI);
	_highlight->setCameraMask((int)GameCamera::FLAG_3D);
	_highlight->setScale(0.07);
	_highlight->setRotation3D(Vec3(90, 0, 0));
	_highlight->setPosition3D(Vec3(0, PhysicsConstants::BALL_RADIUS, 0));
	addChild(_highlight);
	_highlight->setVisible(false);
}

void Ball::setLights(std::vector<cocos2d::Vec3> lightPositions, Vec3 lightColor)
{
	_state->setUniformVec3("u_light_color", lightColor);
	_state->setUniformInt("u_light_count", lightPositions.size());
	for (auto i = 0; i < lightPositions.size(); i++) {
		_state->setUniformVec3("u_light_pos_" + std::to_string(i), lightPositions[i]);
	}
}


void Ball::initDebugIndicator()
{
	auto bb = BillBoard::create();
	_ball->addChild(bb);

	auto r = PhysicsConstants::BALL_RADIUS;
	auto dot = DrawNode::create();
	dot->drawCircle(Vec2(0, 0), r * 1.1, 0, 10, false, 3,3, Color4F(Color4B(0, 255, 0, 128)));
	bb->addChild(dot);
	_debugIndicator = dot;
	dot->setVisible(false);

	auto lockPos = DrawNode::create();
	lockPos->drawCircle(Vec2(0, 0), r * 1.1, 0, 10, false, 1.5, 1.5, Color4F(Color4B(255, 255, 0, 128)));
	bb->addChild(lockPos);
	lockPos->setLocalZOrder(1);
	lockPos->setVisible(false);

	auto lockPos2 = DrawNode::create();
	lockPos2->drawCircle(Vec2(0, 0), r * 1.1, 0, 10, false, 1.5, 1.5, Color4F(Color4B(255, 0, 0, 128)));
	bb->addChild(lockPos2);
	lockPos2->setLocalZOrder(1);
	lockPos2->setVisible(false);

}

BallBody* Ball::body()
{
	return _body;
}

void Ball::setMode2D()
{
	_state->setUniformFloat("u_roughness", Ball::ROUGHNESS_2D);
}

void Ball::resetRoughness()
{
	_state->setUniformFloat("u_roughness", rough);
}

void Ball::setBallHighlightVisible(bool isHightLight)
{
	_highlight->setVisible(isHightLight);
}

void Ball::update(float dt)
{
	Node::update(dt);

	//if (_updateRotate) {
	//	auto quat = _ball->getRotationQuat();
	//	auto ourQuat = ExtMath::quaternion(quat.w, quat.x, -quat.z, quat.y);

	//	const offset = gv.gameMgr.cue._offset;
	//	auto direction = ExtMath::vector( gv.gameMgr.cue._direction.x,gv.gameMgr.cue._direction.y );
	//	double dirLength = std::sqrt(direction.x * direction.x + direction.y * direction.y);
	//	direction.x /= dirLength; direction.y /= dirLength;

	//	const rotateXVelo = offset.v * dt * ExtMath::PI * (-direction.y);
	//	const rotateYVelo = offset.h * dt * ExtMath::PI;
	//	const rotateZVelo = offset.v * dt * ExtMath::PI * direction.x;

	//	const rotate = gv.ExtMath.quaternion.quatFromVec({ x: rotateXVelo, y : rotateZVelo, z : rotateYVelo });
	//	ourQuat = gv.ExtMath.quaternion.multiply(rotate, ourQuat);
	//	quat = new cc.math.quaternion(ourQuat.x, ourQuat.z, -ourQuat.y, ourQuat.w);
	//	_ball->setRotationQuat(quat);
	//	body().quaternion = ourQuat;
	//}

	if (_extrapolatePosition) {
		auto delta = gv::ALMOST_REALTIME_UPDATE_DELTA;
		if (_updateDelay > delta * 3 / 2) {
			_extrapolatePosition = false;
			// body().position = _newPos;
			return;
		}
		_updateDelay += dt;
		auto newPos = ps::ExtMath::vector::mix(_oldPos, _newPos, _updateDelay / delta);
		body()->position() = newPos;
		// cc.log("extrapolating " + JSON.stringify(newPos));
	}
}

void Ball::onBodyChange()
{
	setBallPosition(_body->position());
	setBallQuaternion(_body->quaternionRotation());
}

void Ball::setBallPosition(ps::ExtMath::vector position)
{
	auto onTableEdge = PhysicsWorldV2::floorSurface()->isOnTableEdges(position);
	auto outsideTable = PhysicsWorldV2::floorSurface()->isOutSideTable(position);

	auto minZ = onTableEdge ? std::min(position.z + 0.1, 0.5 + ps::PhysicsConstants::CUSHION_HEIGHT) : std::min(position.z + 0.1, 0.1);
	if (!onTableEdge && outsideTable) minZ = -50;
	_shadow->setPosition3D(Vec3(
		position.x,
		minZ,
		-position.y
	));
	_shadow->setScale(1 + position.z / 75);
	// auto opacity =Math.max(30, 120 - (position.z - minZ) * 4.5);
	// _shadow.circle.setSubShadowOpacityAndBlur(opacity);
	// _shadow.circle.setSubShadowsDistance(1.5 + (position.z - minZ)*0.015);
	_ball->setPosition3D(Vec3(
		position.x,
		ps::PhysicsConstants::BALL_RADIUS + position.z,
		-position.y
	));
	_body->setPosition(position);

	setVisible(!_body->isTotallyInHole() && !_body->isDisabled());
		
	if (_draw != nullptr) {
		_draw->setPosition3D(_ball->getPosition3D());
	}
	//if (_positionCallback) {
	//	auto previousPosition = _previousPosition || position;
	//	auto delta = gv.ExtensionMath.vector.sub(position, previousPosition);
	//	_positionCallback.call(null, position, delta);
	//	_previousPosition = position;
	//}

	//if (gv::gameMgr->table != nullptr) {
	//	gv::gameMgr->table->onBallChangePosition(this);
	//}

}

void Ball::setBallQuaternion(ps::ExtMath::quaternion quaternion)
{
	auto quat = cocos2d::Quaternion(
		quaternion.x,
		quaternion.z,
		-quaternion.y,
		quaternion.w
	);
	_ball->setRotationQuat(quat);
	// this.updateQuaternionShader(quat);
}

void Ball::setSpecular(double val) {
_specular = std::max(val, 0.01);
_state->setUniformFloat("u_specular", _specular);
}


void Ball::setMetallic(double val) {
	if (!Ball::USING_BALL_SHADER) return;
	_state->setUniformFloat("u_metallic", val);
	if (_number == 0) {
		metal = val;
	}
}

void Ball::setRoughness(double val) {
	if (!Ball::USING_BALL_SHADER) return;
	_state->setUniformFloat("u_roughness", val);
	if (_number == 0) {
		rough = val;
	}
}

void Ball::highlight(bool visibility) {
	visibility = visibility && !body()->isTotallyInHole();
	visibility = visibility || _number == 0;
	_state->setUniformFloat("u_grayscale", visibility ? 0 : 1);
}

void Ball::onChangeCameraPosition(cocos2d::Vec3 position)
{
	if (_state) {
		// if(fr.platformWrapper.isIOs()){
		// 	this._state.setUniformVec3("u_cameraPosition", position);
		// }
		// else{
		_state->setUniformVec3("u_view_pos", position);
		// }
	}
}
