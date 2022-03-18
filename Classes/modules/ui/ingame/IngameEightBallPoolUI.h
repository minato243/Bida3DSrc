#ifndef IngameEightBallPoolUI_h_
#define IngameEightBallPoolUI_h_

#include <cocos2d.h>
#include <functional>
#include <core/gui/BaseLayer.h>
#include <cocos/ui/CocosGUI.h>
#include "IngameBaseUI.h"
#include <modules/core/match/EightBallPoolMatch.h>

class IngameEightBallPoolUI: public IngameBaseUI
{
public:
	IngameEightBallPoolUI();
	~IngameEightBallPoolUI();

	bool init();
	CREATE_FUNC(IngameEightBallPoolUI);

	void initHUD();

	void initRemainBalls();

	void initAvatars();

	void initTimers();

	void initUIStatesPos2();

	void initEightBallEffect();

	void notifyBallType();
	
	void update(float dt);

	void setCurMatchPlayerCount();

	void activePlayer(int idx);

	void setTimeRemain(int idx, float time);

	void startMatch(EightBallPoolMatch *match);

	void resetEffectNoti();

	void updateRemainingBalls(int pid, std::vector<int> balls);

	void resetRemainingBalls();

	void setGameMode(GameMode mode);

	void resetActivePlayer();

	void showAllPlayers();

	void hideAllPlayers();

	void setDefaultUI();

	void setScore(const int & userScore, const int & enemyScore);

private:
	cocos2d::Node *_hud;
	cocos2d::ui::Widget *_imgP1;
	cocos2d::ui::Widget *_imgP2;
	cocos2d::Node *_nodeEightBall1;
	cocos2d::Node *_nodeEightBall2;
	cocos2d::Node *_nodeRemainBall1;
	cocos2d::Node *_nodeRemainBall2;
	std::vector<cocos2d::Node*> _eightBallEfx;

	cocos2d::Node* _playerNode[2];
	std::vector<std::vector<cocos2d::Sprite *>> _balls;
	std::map<cocos2d::Sprite*, float> _originalXBalls;
	std::vector<cocos2d::Node *> _avatars;
	std::vector<cocos2d::ProgressTimer *> _timer;
	std::vector<cocos2d::ui::Text *> _timeAlert;
	cocos2d::ui::ImageView *_timeBar;

	float _timeInLastUpdate;

	float _avatarsWidth;

	bool _showedEfxEnemyBalls;
	bool _effectShowBallType;
	int _curPlayerIdx;
};


#endif // !IngameEightBallPoolUI_h_

