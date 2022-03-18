#include "LoginScene.h"
#include "core/GameMgr.h"
#include "core/Config.h"
#include "core/Constant.h"
#include "LoginMgr.h"
#include "core/utils/JNI.h"

USING_NS_CC;
using namespace cocos2d::ui;

LoginScene::LoginScene()
{
	fLoginDelay = false;
}
LoginScene::~LoginScene()
{

}

bool LoginScene::init()
{
	BaseLayer::init();
	initWithBinaryFile("LoginScene.csb");
	return true;
}

void LoginScene::initGUI()
{
	Size size = Director::getInstance()->getVisibleSize();

	Node* logo = getControl("logo");
	logo->setScale(_scale);
	logo->setPosition(Vec2(size.width / 2, size.height / 2 + logo->getContentSize().width / 2));

	// account
	pAccount = getControl("pAccount");
	pAccount->setPositionX(size.width / 2);

	Node* bg = getControl("bg", pAccount);
	bg->setScaleX(2);

	// other
	btnGuest = customButton("guest", LOGINSCENE_BTN_LOGIN_GUEST, pAccount, true);
	btnAppleID = customButton("appleid", LOGINSCENE_BTN_LOGIN_APPLEID, pAccount, true);
	btnFacebook = customButton("facebook", LOGINSCENE_BTN_LOGIN_FACEBOOK, pAccount, true);
	btnAppleID_savePos = btnAppleID->getPosition();

	// dev
	pDev = getControl("pDev");
	customButton("dev", LOGINSCENE_BTN_LOGIN_DEV, pDev, true);
	tfDev = (TextField*)getControl("tfDev", pDev);

	// version
	bar = getControl("bar");
	bar->setScaleX(size.width / bar->getContentSize().width);
	bar->setScaleY(_scale);
	bar->setPosition(Vec2(size.width / 2, 0));

	lbLoading = (Text*)getControl("lbUpdate");

	lbLoading->setPosition(Vec2(size.width / 2, bar->getContentSize().height * _scale));
}

void LoginScene::onEnterFinish()
{
	updateUI();

	// delay 500ms
	processLogin();

	//TODO
	// stop music
	///audioEngine.stopMusic();
}

void LoginScene::updateUI()
{
	pDev->setVisible(Config::ENABLE_CHEAT && Config::DEV_LOCAL);

	pAccount->setVisible(true);

	bar->setVisible(false);
	lbLoading->setVisible(false);

	showEffectGUI();
}
void LoginScene::showEffectGUI()
{
	if (pDev->isVisible()) {
		pDev->runAction(Sequence::create(
			FadeOut::create(0),
			Show::create(),
			FadeIn::create(0.3f),
			nullptr
		));
	}

	if (pAccount->isVisible()) {
		// other
		btnFacebook->setOpacity(0);
		btnFacebook->runAction(Sequence::create (
			DelayTime::create(1),
			Show::create(),
			FadeIn::create(0.5f),
			nullptr
		));

		// other
		btnGuest->setOpacity(0);
		btnGuest->runAction(Sequence::create(
			DelayTime::create(1),
			Show::create(),
			FadeIn::create(0.5f),
			nullptr
		));

		// facebook
		btnAppleID->setPositionX(Director::getInstance()->getVisibleSize().width * 1.5f);
		btnAppleID->runAction(Sequence::create(
			DelayTime::create(0.3f),
			EaseBackOut::create(MoveTo::create(0.3f, btnAppleID_savePos)),
			nullptr
		));
	}
}
void LoginScene::onButtonRelease(cocos2d::ui::Button* button, int id)
{
	switch (id) {
	case LOGINSCENE_BTN_LOGIN_FACEBOOK:
	case LOGINSCENE_BTN_LOGIN_GOOGLE:
	case LOGINSCENE_BTN_LOGIN_GUEST:
	case LOGINSCENE_BTN_LOGIN_APPLEID:
	case LOGINSCENE_BTN_LOGIN_DEV: {

		fLoginDelay = true;
		Director::getInstance()->getScheduler()->schedule([this, id](float dt) {
			switchLogin(id);
		}, this, 0, 0, 0.1f, false, "login_delay");

		break;
	}
	}
}

void LoginScene::processLogin() {
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
void LoginScene::autoLogin(int nSocial)
{
	Director::getInstance()->getScheduler()->schedule([this, nSocial](float dt) {
		switchLogin(convertSocialToID(nSocial));
	}, this, 0, 0, 1.0f, false, "login_delay");
}

void LoginScene::switchLogin(int id)
{
	if (fLoginDelay) {
		fLoginDelay = false;
	}

	switch (id) {
	case LOGINSCENE_BTN_LOGIN_FACEBOOK: {
		loginMgr->loginFacebook();
		break;
	}
	case LOGINSCENE_BTN_LOGIN_APPLEID: {
		loginMgr->loginAppleID();
		break;
	}
	case LOGINSCENE_BTN_LOGIN_GUEST: {
		loginMgr->loginGuest();
		break;
	}
	case LOGINSCENE_BTN_LOGIN_DEV: {
		this->loginDev();
		break;
	}
	}
}

void LoginScene::loginDev()
{
	std::string devID = tfDev->getString();
	if (devID.empty()) devID = "1";
	loginMgr->loginDev(devID);
}

int LoginScene::convertSocialToID(int social)
{
	switch (social) {
	case SOCIAL_FACEBOOK:
		return LOGINSCENE_BTN_LOGIN_FACEBOOK;
	case SOCIAL_GOOGLE:
		return LOGINSCENE_BTN_LOGIN_GOOGLE;
	case SOCIAL_GUEST:
		return LOGINSCENE_BTN_LOGIN_GUEST;
	case SOCIAL_APPLEID:
		return LOGINSCENE_BTN_LOGIN_APPLEID;
	}

	return 0;
}

const std::string LoginScene::className = "LoginScene";

