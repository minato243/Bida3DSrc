#include "BallPocket3DEffect.h"
#include <data/Resource.h>
#include "../camera-view/GameCamera.h"
#include "SettingMgr.h"
#include "Ball.h"
#include "Table.h"
#include "../core/utils/Utils.h"
#include "../../core/GameMgr.h"
#include "../utils/CocosUtils.h"

USING_NS_CC;
using namespace ps::ExtMath;

BallPocket3DEffect::BallPocket3DEffect()
	:Sprite3D()
{
	_flyUpTime = Table::EFX_SHOW_TIME * 0.3f;
	_flyDownTime = Table::EFX_SHOW_TIME * 0.1f;
	_stayTime = 1.5f;
	_totalTime = _flyUpTime + _stayTime + _flyDownTime;
	
	_isPlaying = false;
	_time = 0;
}

BallPocket3DEffect::~BallPocket3DEffect()
{

}

BallPocket3DEffect * BallPocket3DEffect::create(int num)
{
	BallPocket3DEffect *ball = new BallPocket3DEffect();
	ball->_ballId = num;
	ball->init(num);
	ball->_contentSize = ball->getBoundingBox().size;
	ball->autorelease();

	return ball;
}

bool BallPocket3DEffect::init(int ballId)
{
	if (!Sprite3D::initWithFile(Res::BALL_HI_POLY_2))
	{
		return false;
	}

	int textureIdx = (ballId - 1) % Res::NUM_NORMAL_BALL_TEXTURE + 1;
	setTexture(Res::Ball_Textures[textureIdx]);
	setCameraMask((int)GameCamera::FLAG_3D);
	setScale(1);
	initShader();
	scheduleUpdate();
	return true;
}

bool BallPocket3DEffect::initShader()
{
	if (SettingMgr::_instance == NULL)
	{
		return false;
	}

	auto mgr = settingMgr;
	auto program = mgr->getBallShaderProgram();
	if (program)
	{
		_state = GLProgramState::create(program);
		setGLProgramState(_state);

		_state->setUniformVec3("u_light_color", Vec3(1, 1, 1));
		_state->setUniformFloat("u_ao", 1);
		_state->setUniformFloat("u_metallic", Ball::METAL);
		_state->setUniformFloat("u_roughness", Ball::ROUGHNESS_EFFECT);

		_state->setUniformTexture("u_env_map", settingMgr->_ballEnvTexture);
		_state->setUniformTexture("u_irradiance_map", settingMgr->_ballIrradianceTexture);
		_state->setUniformTexture("u_brdf_lut", settingMgr->_brdfTexture);

		_state->setUniformFloat("u_opacity", 1);

		_state->setUniformInt("u_light_count", Ball::NUM_DEFAULT_LIGHT);
		setLightMask((int)GameCamera::FLAG_3D);
		for (unsigned i = 0; i < Ball::NUM_DEFAULT_LIGHT; i++)
		{
			_state->setUniformVec3("u_light_pos_"+ std::to_string(i), Ball::DefaultLights[i]);
		}

	}
	return true;
}

void BallPocket3DEffect::play()
{
	_isPlaying = true;
	_startPos = getPosition3D();
	_time = -0.5f;
}

void BallPocket3DEffect::setOpacity(int val)
{
	_state->setUniformFloat("u_opacity", val *1.f / 255);
}

void BallPocket3DEffect::update(float dt)
{
	if (!_isPlaying)
	{
		return;
	}

	updateRotationByCamera();
	_state->setUniformVec3("u_view_pos", gameMgr->_camera->getCamera()->getPosition3D());
	_time += dt;
	if (_time > _totalTime)
	{
		setVisible(false);
		_isPlaying = false;
		removeFromParent();
		return;
	}

	setVisible(_time > 0);
	if (_time < 0)
		return;

	if (_time < _flyUpTime)
	{
		float ratio = Utils::easeInOutCubic(_time / _flyUpTime);
		Vec3 pos = Vec3(_startPos);
		pos.y = 15 * ratio;

		setOpacity(std::min(255.f, ratio * 2* 255));
		setScale(1 + 2 * ratio);
		setPosition3D(pos);
	}
	else if (_time < _flyUpTime + _stayTime)
	{

	}
	else
	{
		float ratio = Utils::easeInOutQuad((_time - _flyUpTime - _stayTime) / _flyDownTime);
		Vec3 pos = Vec3(_startPos);
		pos.y = _startPos.y + 15 - 25 * ratio;
		setScale(std::max(3 * (1 - ratio), 0.5f));
		setPosition3D(pos);
	}
}

void BallPocket3DEffect::updateRotationByCamera()
{
	Vec3 pos = getPosition3D();
	Vec3 cameraPos = gameMgr->_camera->getCamera()->getPosition3D();
	vector gamePos = CocosUtils::transCCVecToGameVec(pos);
	vector cameraGamePos = CocosUtils::transCCVecToGameVec(cameraPos);
	vector delta = vector::sub(cameraGamePos, gamePos);
	vector uDelta = vector::unit(delta);

	vector d = vector::unit(vector(uDelta.x, uDelta.y, 0));

	double alpha = ExtMath::acos(d.y);
	vector q = vector(0, 0, d.x < 0 ? alpha: - alpha);
	quaternion quatHoz = quaternion(q);

	vector r = vector(ExtMath::asin(uDelta.z), 0, 0);
	quaternion quatVer = quaternion(r);

	quaternion quat = quaternion::multiply(quatHoz, quatVer);

	cocos2d::Quaternion cocosQuat = Quaternion(quat.x, quat.z, -quat.y, quat.w);
	setRotationQuat(cocosQuat);
}

