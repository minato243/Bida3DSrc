#ifndef ResultGUILayer_h_
#define ResultGUILayer_h_

#include "../base/UIBaseLayer.h"
#include "../../../data/GameConstant.h"
#include "../../user/User.h"
#include "../../core/inventory/ItemInfo.h"
#include "../../core/match/Player.h"

class ScorePanel;

class ResultGUILayer
	: public UIBaseLayer
{
public:
	enum TAG
	{
		btnLobby,
		btnRePlay,
	};
	ResultGUILayer();
	~ResultGUILayer();

	CREATE_FUNC(ResultGUILayer);

	bool init();

	void initGUI();

	void initTag();

	void createUserRt3();

	void onButtonRelease(cocos2d::ui::Button* button, int id);

	void loadData(GameMode _gameMode, int channelId, int winner);

	void loadDataStreak(GameMode _gameMode, int channelId, int winner, const std::vector<bool> & listStreak);

	void loadDataStreak(const std::vector<bool> & listStreak);

	void loadDataRT1(int channelId, int winnerIdx);

	void loadDataRT2(int channelId, int winner, int scoreP1, int scoreP2);

	void loadDataRT3(int channelId, int winner, int scoreP1, int scoreP2);

	void loadUserInfo(const std::vector<Player> & listPlayer);

	void loadRT3UserInfo(const std::vector<Player> & listPlayer);

	void show();

	void moveToDefaultPos(cocos2d::Node *node, float delayTime, float moveTime = 0.5f);

	void runFXExp();

	void runFXMinus();

	void fakeUserData();

	void fakeRT3UserData();

private:
	cocos2d::ui::Button *_btnLobby;
	cocos2d::ui::Button *_btnRePlay;

	cocos2d::ui::ImageView *_panelInfoGame;

	cocos2d::Sprite *_imageWinLose;
	cocos2d::Sprite *_imageMode;
	cocos2d::Sprite *_img_Channel;
	cocos2d::Sprite *_img_ModePlay;

	cocos2d::ui::Widget *_panelUser_0;
	cocos2d::ui::Widget *_panelUser_1;

	cocos2d::ui::Widget *_panelUserRT3_0;
	cocos2d::ui::Widget *_panelUserRT3_1;


	cocos2d::Sprite *_check_1;
	cocos2d::Sprite *_check_2;

	cocos2d::ui::Text *_text_Game_1;
	cocos2d::ui::Text *_text_Game_2;
	
	cocos2d::Node *_userNodeList[2];
	cocos2d::Node *_userRT3NodeList[4];

	GameMode _gameMode;
	int _winner;
	int _channelId;
	std::vector<ItemInfoData> _listGift;
	std::vector<Player> _listPlayer;

public:
	static const int NUM_MODE = 4;
	static const std::string BG_INFO[NUM_MODE];
	static const std::string GRADIENT_INFO[NUM_MODE];
};

class PlayerInfoRT3 
	:public cocos2d::Node
{
public:
	PlayerInfoRT3();
	~PlayerInfoRT3();

	bool init();

	CREATE_FUNC(PlayerInfoRT3);

	void initGUI();

	void updateInfoPlayer(const Player & player, bool isWinner);

private:
	cocos2d::Node *_gui;
	cocos2d::ui::Text *_lbGold;
	cocos2d::ui::LoadingBar *_ldBar;
	cocos2d::ui::Text *_lbLevel;
	cocos2d::ui::ImageView *_imgAvatarBorder;
	cocos2d::Sprite *_crown;
	cocos2d::Node *_nodeRank;
	cocos2d::ui::Text *_lb_NameRank;
	cocos2d::ui::Text *_textNoRank;
};


class ScorePanel
	:public cocos2d::Node
{
public:
	ScorePanel();
	~ScorePanel();

	CREATE_FUNC(ScorePanel);
	bool init();

	void initGUI();

public:
	cocos2d::ui::Text *_lb_Name;
	cocos2d::ui::Text *_lb_Level;
	cocos2d::ui::Text *_lb_Number;
	cocos2d::Sprite *_icon;
};
#endif
