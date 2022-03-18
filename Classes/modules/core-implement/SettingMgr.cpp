#include "SettingMgr.h"
#include <cocos2d.h>
#include <core/GameMgr.h>
#include <data/Resource.h>
#include <cocos/renderer/CCTexture2D.h>

USING_NS_CC;
using namespace std;

SettingMgr::SettingMgr()
	:_matSpecialShader(true), _camera(nullptr)
{
	_ballEnvTexture = NULL;
	init();
	initShaders();
}

SettingMgr::~SettingMgr()
{
}

void SettingMgr::init()
{
	_quality = Quality::MEDIUM;
	_bgVolume = 100;
	_sfxVolume = 100;

	_objBallGuildLength = 20;
	_cueBallGuildLength = 10;
}

void SettingMgr::initShaders()
{
	auto defaultShader = GLProgram::createWithFilenames("res/shaders/3d_shader.vert", "res/shaders/3d_shader.frag");
	defaultShader->addAttribute("a_position", 0);
	defaultShader->addAttribute("a_normal", 1);
	defaultShader->addAttribute("a_texCoord", 2);
	defaultShader->link();
	defaultShader->updateUniforms();
	defaultShader->retain();
	auto ios = CC_TARGET_PLATFORM == CC_PLATFORM_IOS;
	ShaderData shaders;
	shaders.ball.push_back({"res/shaders/3d_shader_ball_low.vert", "res/shaders/IOS/3d_shader_ball_low.frag"});
	shaders.ball.push_back({ "res/shaders/3d_shader.vert", "res/shaders/IOS/3d_shader_ball_5.frag" });
	shaders.ball.push_back({ "res/shaders/3d_shader_high.vert", "res/shaders/IOS/3d_shader_ball_high.frag" });

	shaders.table.push_back({ "res/shaders/3d_shader.vert", "res/shaders/IOS/3d_shader_table_5.frag" });
	shaders.table.push_back({ "res/shaders/3d_shader.vert",ios ? "res/shaders/IOS/3d_shader_table_5_IOS.frag" : "res/shaders/IOS/3d_shader_table_5.frag" });
	shaders.table.push_back({ "res/shaders/3d_shader_high.vert", ios ? "res/shaders/IOS/3d_shader_table_high_IOS.frag" : "res/shaders/IOS/3d_shader_table_high.frag" });

	shaders.cue.push_back({ "res/shaders/3d_shader.vert", "res/shaders/IOS/3d_shader_cue_5.frag" });
	shaders.cue.push_back({ "res/shaders/3d_shader.vert",ios ? "res/shaders/IOS/3d_shader_cue_5_IOS.frag" : "res/shaders/IOS/3d_shader_cue_5.frag" });
	shaders.cue.push_back({ "res/shaders/3d_shader_high.vert", ios ? "res/shaders/IOS/3d_shader_cue_high_IOS.frag" : "res/shaders/IOS/3d_shader_cue_high.frag" });
	shaders.tableMat = {"res/shaders/3d_shader_high.vert", "res/shaders/IOS/3d_shader_mat_2.frag"};

	// for(auto i=gv.SettingMgr.Quality.BEST-2;i<=gv.SettingMgr.Quality.BEST;i++){
	for (auto i = 0; i < 3; i++) {
		// auto vert = frags.vert[i];
		// auto vertSkinned = 'res/shaders/3d_shader_skinned.vert';
		// auto fragBall = frags.ball[i];

		auto ballShader =  GLProgram::createWithFilenames(shaders.ball[i][0], shaders.ball[i][1]);
		ballShader->addAttribute("a_position", 0);
		ballShader->addAttribute("a_normal", 1);
		ballShader->addAttribute("a_texCoord", 2);
		ballShader->link();
		ballShader->updateUniforms();
		ShaderCache::getInstance()->addProgram(ballShader, "BallShader_" + std::to_string(i));

		auto tableShader =  GLProgram::createWithFilenames(shaders.table[i][0], shaders.table[i][1]);
		tableShader->addAttribute("a_position", 0);
		tableShader->addAttribute("a_normal", 1);
		tableShader->addAttribute("a_texCoord", 2);
		tableShader->link();
		tableShader->updateUniforms();
		ShaderCache::getInstance()->addProgram(tableShader, "TableShader_" + std::to_string(i));

		auto matShader =  GLProgram::createWithFilenames(shaders.tableMat[0], shaders.tableMat[1]);
		matShader->addAttribute("a_position", 0);
		matShader->addAttribute("a_normal", 1);
		matShader->addAttribute("a_texCoord", 2);
		matShader->link();
		matShader->updateUniforms();
		// matShader.retain();
		// this._shaders.tableMat[i] = matShader;
		// this._shaders.cue[i] = tableShader;
		char matStr[20];
		sprintf(matStr, "TableMatShader_%d", i);
		ShaderCache::getInstance()->addProgram(matShader, matStr);

		auto cueShader =  GLProgram::createWithFilenames(shaders.cue[i][0], shaders.cue[i][1]);
		cueShader->addAttribute("a_position", 0);
		cueShader->addAttribute("a_normal", 1);
		cueShader->addAttribute("a_texCoord", 2);
		cueShader->link();
		cueShader->updateUniforms();
		ShaderCache::getInstance()->addProgram(cueShader, "CueShader_" + std::to_string(i));
	}
}

void SettingMgr::initTextures()
{
	auto scene = Director::getInstance()->getRunningScene();
	auto path = "Texture/" + Res::hdriNameBall + "_env/";
	auto textureCube = TextureCube::create(path + "px.png", path + "nx.png", path + "py.png", path + "ny.png", path + "pz.png", path + "nz.png");

	auto skybox = Skybox::create();
	skybox->setTexture(textureCube);
	scene->addChild(skybox);
	skybox->setVisible(false);
	skybox->retain();
	this->_ballEnvTexture = textureCube;

	path = "Texture/" + Res::hdriNameBall + "_hdr/";
	auto cubeIrradiance = TextureCube::create(path + "px.png", path + "nx.png", path + "py.png", path + "ny.png", path + "pz.png", path + "nz.png");

	auto skybox2 = Skybox::create();
	skybox2->setTexture(cubeIrradiance);
	scene->addChild(skybox2);
	skybox2->setVisible(false);
	skybox2->retain();
	this->_ballIrradianceTexture = cubeIrradiance;

	auto texture = TextureCache::getInstance()->addImage("res/shaders/ibl_brdf_lut.png");
	this->_brdfTexture = texture;

	path = "Texture/" + Res::hdriNameTable + "_env/";
	auto tableEnv = TextureCube::create(path + "px.png", path + "nx.png", path + "py.png", path + "ny.png", path + "pz.png", path + "nz.png");
	Texture2D::TexParams params;
	params.minFilter = GL_LINEAR; params.magFilter = GL_LINEAR; params.wrapS = GL_CLAMP_TO_EDGE; params.wrapT = GL_CLAMP_TO_EDGE;

	tableEnv->setTexParameters(params);

	auto skybox3 = Skybox::create();
	skybox3->setTexture(tableEnv);
	scene->addChild(skybox3);
	skybox3->setVisible(false);
	skybox3->retain();
	this->_tableEnvTexture = tableEnv;


	path = "Texture/" + Res::hdriNameTable + "_hdr/";
	auto tableHdri = TextureCube::create(path + "px.png", path + "nx.png", path + "py.png", path + "ny.png", path + "pz.png", path + "nz.png");
	tableHdri->setTexParameters(params);

	auto skybox4 = Skybox::create();
	skybox4->setTexture(tableHdri);
	scene->addChild(skybox4);
	skybox4->setVisible(false);
	skybox4->retain();
	this->_tableIrradianceTexture = tableHdri;
}

void SettingMgr::autoSelectProfile()
{
	Quality level = Quality::MEDIUM;
	setShaderQuality(level);
}

void SettingMgr::setShaderQuality(Quality level)
{
	CCLOG("SettingMgr::setShaderQuality %d", level);
	auto balls = gameMgr->_table->getAllBall();
	for (int i = 0; i < balls.size(); i++) {
		balls[i]->initShader(level);
	}

	auto table = gameMgr->_table;

	table->initShader(GLProgramCache::getInstance()->getProgram("TableShader_" + std::to_string(level)), ShaderCache::getInstance()->getProgram("TableMatShader_0"), level);

	table->getCue()->initShader(ShaderCache::getInstance()->getProgram("CueShader_" + std::to_string(level)), level);

	if (_camera != NULL) this->onCameraChangePosition(_camera->getCamera()->getPosition3D());

}

void SettingMgr::onCameraChangePosition(cocos2d::Vec3 position)
{
	auto table = gameMgr->_table;
	table->onChangeCameraPosition(position);
	
	auto balls = table->getAllBall();
	std::for_each(balls.begin(), balls.end(), [&](Ball *ball) {ball->onChangeCameraPosition(position); });

	auto cue = table->getCue();
	cue->onChangeCameraPosition(position);

	//todo update npc
	this->_lastCameraPosition = position;

}

void SettingMgr::setCamera(GameCamera * camera)
{
	_camera = camera;
	camera->subscribe(CC_CALLBACK_1(SettingMgr::onCameraChangePosition, this));
}

cocos2d::GLProgram * SettingMgr::getBallShaderProgram()
{
	return ShaderCache::getInstance()->getProgram("BallShader_" + to_string((int)_quality));
}

SettingMgr *SettingMgr::_instance = nullptr;

SettingMgr * SettingMgr::getInstance()
{
	if (_instance == nullptr) {
		_instance = new SettingMgr();
	}
	return _instance;
}


