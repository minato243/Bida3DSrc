#include "LoadingSceneNew.h"
#include <data/Resource.h>
#include <login/LoginMgr.h>
#include "core/gui/SceneMgr.h"
#include "core/Constant.h"

USING_NS_CC;
using namespace cocos2d::ui;

LoadingSceneNew::LoadingSceneNew()
{
	_percent = 0;
	_isLoading = true;
	_loginLayer = nullptr;
	_devLoginLayer = nullptr;
	_tag = SceneTag::LOADING_SCENE;

}

LoadingSceneNew::~LoadingSceneNew()
{
}

bool LoadingSceneNew::init()
{
	if (!BaseLayer::init())
	{
		return false;
	}

	renderBackground();

	return true;
}

void LoadingSceneNew::initGUI()
{
}

void LoadingSceneNew::renderBackground()
{
	Size winSize = Director::getInstance()->getVisibleSize();
	auto layer = Layer::create();
	auto bg = Sprite::create(Res::placeholder_bg);
	bg->setPosition(winSize.width / 2, winSize.height / 2);
	bg->setScale(std::max(winSize.width / bg->getContentSize().width, winSize.height / bg->getContentSize().height));
	layer->addChild(bg);

	auto text = Text::create();
	text->setPosition(Vec2(winSize.width / 2, winSize.height * 3 / 4));
	text->setFontSize(48);
	layer->addChild(text);
	_lb = text;

	_loadingLayer = layer;
	addChild(layer);
}

void LoadingSceneNew::onCompletePreload()
{
	if (_loginLayer == nullptr)
	{
		_loginLayer = ChooseLoginTypeLayer::create();
		addChild(_loginLayer);
		_loginLayer->setParentLayer(this);
	}
}

void LoadingSceneNew::onEnter()
{
	BaseLayer::onEnter();
	scheduleUpdate();
}

void LoadingSceneNew::update(float dt)
{
	_percent += 1;
	if (_percent > 100)
	{
		_percent = 100;
	}

	char text[50];
	sprintf(text, "%d%%", (int)_percent);
	_lb->setText(text);

	if (_percent == 100 && _isLoading) 
	{
		_isLoading = false;
		onCompletePreload();
	}
}

void LoadingSceneNew::onBtnDevClick()
{
	_loginLayer->removeFromParent();
	_loginLayer = nullptr;
	showLoginWithDevID();
}

void LoadingSceneNew::showLoginWithDevID()
{
	if (_devLoginLayer == nullptr)
	{
		_devLoginLayer = DevLoginLayer::create();
		addChild(_devLoginLayer);
		_devLoginLayer->setParentLayer(this);
	}
}

void LoadingSceneNew::doLoginDev(int uId, const std::string & fakeDeviceId)
{
	CCLOG("LoadingSceneNew::doLoginDev");
	loginMgr->loginDev(std::to_string(uId));
}

void LoadingSceneNew::processLogin()
{
	int dLogin = loginMgr->getDefaultLogin();
	if (dLogin > -1) {
		autoLogin(dLogin);
	}
	else {
		int nSocial = loginMgr->getCurrentLogin();
		if (nSocial) {
			autoLogin(nSocial);
		}
	}
}

void LoadingSceneNew::autoLogin(int nSocial)
{
	Director::getInstance()->getScheduler()->schedule([this, nSocial](float dt) {
		switchLogin(nSocial);
	}, this, 0, 0, 1.0f, false, "login_delay");
}

void LoadingSceneNew::switchLogin(int id)
{
	switch (id) {
	case SOCIAL_FACEBOOK: {
		loginMgr->loginFacebook();
		break;
	}
	case SOCIAL_APPLEID: {
		loginMgr->loginAppleID();
		break;
	}
	case SOCIAL_GUEST: {
		loginMgr->loginGuest();
		break;
	}
	case SOCIAL_GOOGLE: {
		loginMgr->loginGoogle();
		break;
	}
	default:
		_devLoginLayer->doLogin();
	}
}

const std::string LoadingSceneNew::className = "LoadingSceneNew";
