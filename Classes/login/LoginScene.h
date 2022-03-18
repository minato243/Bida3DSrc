#ifndef LoginScene_h__
#define LoginScene_h__
#include "core/gui/BaseLayer.h"

class LoginScene:
public BaseLayer{
public:
	CREATE_FUNC(LoginScene);
	LoginScene();
	~LoginScene();
	bool init();
public:
	void initGUI();
	void onEnterFinish();
	void updateUI();
	void showEffectGUI();
	void onButtonRelease(cocos2d::ui::Button* button, int id);
	void processLogin();
	void autoLogin(int nSocial);
	void switchLogin(int id);
	void loginDev();
	int convertSocialToID(int social);

public:
	cocos2d::Node* pAccount;
	cocos2d::ui::Button* btnGuest;
	cocos2d::ui::Button* btnAppleID;
	cocos2d::ui::Button* btnFacebook;
	cocos2d::Node* pDev;
	cocos2d::ui::TextField* tfDev;
	cocos2d::Node* bar;

	cocos2d::ui::Text* lbLoading;

	cocos2d::Vec2 btnAppleID_savePos;
	bool fLoginDelay;

	// name
	static const std::string className;
};
const int LOGINSCENE_BTN_LOGIN_FACEBOOK = 1001;

const int LOGINSCENE_BTN_LOGIN_GOOGLE = 1002;

const int LOGINSCENE_BTN_LOGIN_GUEST = 1003;

const int LOGINSCENE_BTN_LOGIN_DEV = 1004;

const int LOGINSCENE_BTN_LOGIN_APPLEID = 1005;

#endif // LoginScene_h__
