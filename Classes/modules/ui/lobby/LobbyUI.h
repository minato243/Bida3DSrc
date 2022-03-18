#ifndef LOBBY_UI_H_
#define LOBBY_UI_H_

#include <core/gui/BaseLayer.h>
#include "../../user/User.h"

class LobbyUI
	:public BaseLayer
{
public:

	enum TAG
	{
		PLAY_NOW,
		PLAY,
		SETTING,
		INVENTORY,
		SHOP,
		SPONSORSHIP,
		GACHA,
		LEADERBOARD,
		ACHIEVEMENT,
		CUE_COLLECTION, 
		btnPlay9Ball
	};

	LobbyUI();
	~LobbyUI();

	CREATE_FUNC(LobbyUI);
	bool init();

	void initGUI();

	void initTag();

	void initComponent();

	void initAvatarNode();

	void initButtonEffects();

	void updateUserInfo(User *user);

	void onButtonRelease(cocos2d::ui::Button* button, int id);

	void playNow();

	void show();

public:
	cocos2d::ui::ImageView *_bg;
	cocos2d::ui::Widget *_playPanel;
	cocos2d::ui::Widget *_eventPanel;
	cocos2d::ui::Widget *_supportPanel;
	cocos2d::ui::Widget *_topPanel, *_leftPanel;

	cocos2d::ui::Button *_btnInventory;
	cocos2d::ui::Button *_btnClan;
	cocos2d::ui::Button *_btnShop;

	cocos2d::ui::Button *_btnCueCollection;
	cocos2d::ui::Button *_btnSetting;

	cocos2d::ui::Button *_btnPlay;
	cocos2d::ui::Button *_btnRank;
	cocos2d::ui::Button *_btnSolve;

	cocos2d::ui::Button *_btnPlay9Ball;

	//user info
	cocos2d::ui::Text *_lbName;
	cocos2d::ui::Text *_lbClub;
	cocos2d::ui::Text *_lbGold;
	cocos2d::ui::Text *_lbCash;
	cocos2d::ui::LoadingBar *_ldBar;
	cocos2d::ui::Text *_lbLevel;

};


#endif // !LOBBY_UI_H_


