#ifndef SETTINGMGR_H_
#define SETTINGMGR_H_

#include <vector>
#include <string.h>
#include <cocos2d.h>
#include <modules/camera-view/SphericalCamera.h>
#include <modules/camera-view/GameCamera.h>

#define settingMgr SettingMgr::getInstance()

enum Quality
{
	LOW,
	MEDIUM,
	HIGH
};

struct ShaderData {
	std::vector<std::vector<std::string>> ball;
	std::vector <std::vector<std::string>> cue;
	std::vector <std::vector<std::string>> table;
	std::vector<std::string> tableMat;
};

class SettingMgr
{
public:
	SettingMgr();
	~SettingMgr();

	void init();
	void initShaders();
	void initTextures();
	void autoSelectProfile();
	void setShaderQuality(Quality level);
	void onCameraChangePosition(cocos2d::Vec3 position);

	void setCamera(GameCamera *camera);

	cocos2d::GLProgram *getBallShaderProgram();
private:
	Quality _quality;
	int _bgVolume;
	int _sfxVolume;
	int _objBallGuildLength;
	int _cueBallGuildLength;
	bool _matSpecialShader;
	cocos2d::Vec3 _lastCameraPosition;

public:
	static SettingMgr *_instance;
	static SettingMgr *getInstance();

	cocos2d::TextureCube* _ballEnvTexture;
	cocos2d::TextureCube* _ballIrradianceTexture;
	cocos2d::TextureCube* _tableEnvTexture;
	cocos2d::TextureCube* _tableIrradianceTexture;
	cocos2d::Texture2D* _brdfTexture;
	GameCamera *_camera;
};


#endif // !SETTINGMGR_H_



