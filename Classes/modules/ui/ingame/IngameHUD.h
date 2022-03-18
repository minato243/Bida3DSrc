#ifndef INGAMEHUD_H_
#define INGAMEHUD_H_

#include <cocos2d.h>
#include <cocos/ui/CocosGUI.h>
#include <modules/core/match/Player.h>

class IngameHUD: public cocos2d::Node
{
public:
	IngameHUD();
	~IngameHUD();

	bool init();
	CREATE_FUNC(IngameHUD);

	void initComponent(cocos2d::ui::Widget *hud, cocos2d::ui::ImageView *avatarBox, cocos2d::Node* playerNodes[]);

	void initAnimationsPos();

	void initHUDBalls();

	void initGameModeNodes();

	void scheduleAutoHide();

	void onGameStart(int playerCount);

	void setModeFull(bool full);

	void setGameMode(int mode);

	void update(float dt);

	void setPlayerData();

	void resetActivePlayer();

	void setActivePlayer(int idx, Player info);

	void updateRemainingBalls(int pid, std::vector<int> balls);

	void setIngameUI(cocos2d::Node *ingameUI);

	void setModeFullComplete(cocos2d::Node * avatar1);

private:
	int _curActivePlayer;
	bool _modeFull;
	cocos2d::Node *_ingameUI;
	cocos2d::ui::Widget *_hud;
	cocos2d::ui::Widget *_minimizedHud;
	cocos2d::Node* _avatars[2];
	cocos2d::ui::ImageView *_imgPanelP1;
	cocos2d::ui::ImageView *_imgPanelP2;

	cocos2d::Vec2 _showPosition1;
	cocos2d::Vec2 _showPosition2;
	cocos2d::Vec2 _hidePosition1;
	cocos2d::Vec2 _hidePosition2;

	std::vector<cocos2d::ui::ImageView*> _gameModeNodes;
	std::vector<cocos2d::Vec2> _gameModeShowPositions;
	std::vector<cocos2d::Vec2> _gameModeHidePositions;
	
	std::vector<long> _interactions;
	long _lastInteraction;

	int _gameMode;

public:
	const static int AUTO_HIDE_THRESHOLD = 3000;
	const static int AUTO_HIDE_THRESHOLD_AFTER_TOUCH = 0;
};


#endif // !INGAMEHUD_H_



