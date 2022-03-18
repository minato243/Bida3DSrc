#ifndef LOADING_SCENE_NEW_H
#define LOADING_SCENE_NEW_H

#include <core/gui/BaseLayer.h>
#include "ChooseLoginTypeLayer.h"
#include "DevLoginLayer.h"

class LoadingSceneNew
	:public BaseLayer
{
public:
	LoadingSceneNew();
	~LoadingSceneNew();

	bool init();
	CREATE_FUNC(LoadingSceneNew);

	void initGUI();

	void renderBackground();

	void onCompletePreload();

	void onEnter();

	void update(float dt);

	void onBtnDevClick();

	void showLoginWithDevID();

	void doLoginDev(int uId, const std::string & fakeDeviceId);

	void processLogin();

	void autoLogin(int nSocial);

	void switchLogin(int id);

private:
	cocos2d::ui::Text *_lb;
	cocos2d::Layer *_loadingLayer;
	ChooseLoginTypeLayer *_loginLayer;
	DevLoginLayer *_devLoginLayer;

	float _percent;
	bool _isLoading;

public:
	static const std::string className;
};

#endif // !LOADING_SCENE_NEW


