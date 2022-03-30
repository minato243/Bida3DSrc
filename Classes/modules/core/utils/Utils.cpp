#include "Utils.h"
#include "../../../data/Resource.h"
#include "TimerSprite.h"
#include "../../camera-view/GameCamera.h"
#include "data/GameConstant.h"
#include "data/DataStruct.h"

USING_NS_CC;
using namespace std;

GLProgramState* Utils::setUpCue3DShader(cocos2d::Sprite3D *cue, std::string albedoPath, std::string mraoPath)
{
	auto properties = Properties::createNonRefCounted("res/material/3d_effects.material#cue_inspect");

	cue->setTexture(albedoPath);
	auto mat = Material::createWithProperties(properties);
	cue->setMaterial(mat);
	cue->setLightMask(0);
	// cue->getMaterial(0).getTechniqueByName("outline").getPassByIndex(0).getGLProgramState().setUniformVec3("u_view_pos", cc.math)
	auto state = cue->getMaterial(0)->getTechniqueByName("outline")->getPassByIndex(0)->getGLProgramState();
	state->setUniformVec3("u_light_color", Vec3(1, 1, 1));
	state->setUniformVec3("u_view_pos", Vec3(0, 0, -100));
	state->setUniformFloat("u_ao", 1.0);
	state->setUniformFloat("u_opacity", 1);

	auto albedo = TextureCache::getInstance()->addImage(albedoPath);
	auto mraoMap = TextureCache::getInstance()->addImage(mraoPath);
	state->setUniformTexture("u_albedo", albedo);
	state->setUniformTexture("u_mrao_map", mraoMap);

	state->setUniformFloat("u_lod_level", 1);

	auto path = "Texture/" + Res::hdriNameTable + "_env/";
	auto textureCube = TextureCube::create(path + "px.png", path + "nx.png", path + "py.png", path + "ny.png", path + "pz.png", path + "nz.png");
	// var textureCube = jsb.TextureCube.create("Sprite3DTest/skybox/left.jpg","Sprite3DTest/skybox/right.jpg", "Sprite3DTest/skybox/top.jpg", "Sprite3DTest/skybox/bottom.jpg", "Sprite3DTest/skybox/front.jpg", "Sprite3DTest/skybox/back.jpg");

	//set the texture parameters
	Texture2D::TexParams cubeParams;
	cubeParams.minFilter = GL_LINEAR;
	cubeParams.magFilter = GL_LINEAR;
	cubeParams.wrapS = GL_MIRRORED_REPEAT;
	cubeParams.wrapT = GL_MIRRORED_REPEAT;
	textureCube->setTexParameters(cubeParams);
	state->setUniformTexture("u_env_map", textureCube);

	auto skybox = Skybox::create();
	skybox->setTexture(textureCube);
	Director::getInstance()->getRunningScene()->addChild(skybox);
	skybox->setVisible(false);

	path = "Texture/" + Res::hdriNameTable + "_hdr/";
	auto cubeIrradiance = TextureCube::create(path + "px.png", path + "nx.png", path + "py.png", path + "ny.png", path + "pz.png", path + "nz.png");
	Texture2D::TexParams cubeIrradianceParams;
	cubeParams.minFilter = GL_LINEAR;
	cubeParams.magFilter = GL_LINEAR;
	cubeParams.wrapS = GL_MIRRORED_REPEAT;
	cubeParams.wrapT = GL_MIRRORED_REPEAT;
	cubeIrradiance->setTexParameters(cubeIrradianceParams);
	state->setUniformTexture("u_irradiance_map", cubeIrradiance);

	skybox = Skybox::create();
	skybox->setTexture(cubeIrradiance);
	Director::getInstance()->getRunningScene()->addChild(skybox);
	skybox->setVisible(false);

	auto texture = TextureCache::getInstance()->addImage("res/shaders/ibl_brdf_lut.png");
	state->setUniformTexture("u_brdf_lut", texture);
	return state;
}

float Utils::easeInOutCubic(float x)
{
	return x < 0.5 ? 4 * x * x * x : 1 - pow(-2 * x + 2, 3) / 2;
}

float Utils::easeInOutSine(float x)
{
	return -(cos(M_PI * x) - 1) / 2;
}

float Utils::easeInSine(float x)
{
	return 1 - cos((x * M_PI) / 2);
}

float Utils::easeOutSine(float x)
{
	return sin((x * M_PI) / 2);
}

float Utils::easeInOutQuad(float x)
{
	return x < 0.5 ? 2 * x * x : 1 -(-2 * x + 2) * (-2 * x + 2) / 2;
}

cocos2d::Color3B Utils::mix(const cocos2d::Color3B & a, const cocos2d::Color3B & b, float r)
{
	if (r <= 0) return a;
	if (r >= 1) return b;

	Color3B ret;
	ret.r = mix(a.r, b.r, r);
	ret.g = mix(a.g, b.g, r);
	ret.b = mix(a.b, b.b, r);

	return ret;
}

float Utils::mix(const float & a, const float & b, const float & r)
{
	if (r <= 0) return a;
	if (r >= 1) return b;
	
	return a * (1 - r) + b * r;
}

Node* Utils::getEfkCircle(bool shadow /*= false*/)
{
	CCLOG("Utils::getEfkCircle");
	if (_instance == NULL) getInstance();
	if (!shadow && _instance->_efkCirclePool.size() > 0) {
		CCLOG("Utils::getEfkCircle instance->_efkCirclePool.size() > 0");
		std::list<Node*>::iterator ittr = _instance->_efkCirclePool.begin();
		Node *emitter = *ittr;
		_instance->_efkCirclePool.pop_front();
		emitter->setVisible(true);
		auto node = emitter->getChildByTag(TAG_COMMON);
		node->setVisible(true);
		node->setOpacity(255);
		return emitter;
	}
	if (shadow && _instance->_efkShadowPool.size() > 0) {
		CCLOG("Utils::getEfkCircle instance->_efkShadowPool.size() > 0");
		std::list<Node*>::iterator ittr = _instance->_efkShadowPool.begin();
		Node *emitter = *ittr;
		_instance->_efkShadowPool.pop_front();
		emitter->setVisible(true);
		auto node = emitter->getChildByTag(TAG_COMMON);
		node->setVisible(true);
		node->setOpacity(255);
		return emitter;
	}
	auto path = shadow ? "cricle_shadow.png" : "cricle_alpha.png";
	auto sprite = TimerSprite::create(path);
	sprite->retain();
	if (!shadow)
		sprite->setBlendFunc(BlendFunc{ GL_ONE, GL_ONE });
	sprite->setScale(0.010);
	Node *bb;
	if (shadow) bb = Sprite3D::create();
	else bb = BillBoard::create();
	bb->retain();
	CCLOG("%x", bb);

	Node *node = Node::create();
	node->addChild(sprite);
	sprite->setTag(TAG_COMMON);
	node->setTag(TAG_COMMON);
	bb->addChild(node);

	
	bb->setCameraMask((int)GameCamera::FLAG_3D);
	if (shadow) {
		bb->setRotation3D(Vec3(-90, 0, 0));
		_instance->_efkShadowPool.push_back(bb);
	}
	else _instance->_efkCirclePool.push_back(bb);

	_instance->_isShadowMaps[bb] = shadow;
	return bb;
}

void Utils::releaseEfkCircle(cocos2d::Node * efk)
{
	CCLOG("Utils::releaseEfkCircle");
	if (_instance->_isShadowMaps[efk]) {
		_instance->_efkShadowPool.push_back(efk);
	}
	else {
		_instance->_efkCirclePool.push_back(efk);
	}
	efk->setOpacity(255);
	efk->getChildByTag(TAG_COMMON)->setVisible(false);
}

std::string Utils::getBinary64(double value)
{
	mDouble mValue;
	mValue.f = value;

	bool start = false;
	std::string ret = "";
	for (int i = 63; i >= 0; i--)
	{
		char text[2];
		if (start) {
			sprintf(text, "%d", (mValue.iValue >> i) & 1);
			ret += text;
		}
		else if ((mValue.iValue >> i) & 1) {
			start = true;
			sprintf(text, "%d", (mValue.iValue >> i) & 1);
			ret += text;
		}
	}
	if (!start) ret = "0";
	return ret;
}

Utils * Utils::getInstance()
{
	if (_instance == NULL) {
		_instance = new Utils();
	}
	return _instance;
}

Utils * Utils::_instance = NULL;

