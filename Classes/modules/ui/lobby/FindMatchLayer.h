#ifndef FIND_MATCH_LAYER_H
#define FIND_MATCH_LAYER_H

#include <core/gui/BaseLayer.h>
#include <modules/core/match/Player.h>
#include <data/GameConstant.h>

class FindMatchLayer
	: public BaseLayer
{
public:
	enum TAG 
	{
		BTN_CANCEL
	};
	FindMatchLayer();
	~FindMatchLayer();

	CREATE_FUNC(FindMatchLayer);
	bool init();

	void initGUI();

	void onEnter();

	void startTimer();

	void stopTimer();

	void updateTimer(float dt);


	void onFoundOpponent(const Player & p2Data);

	void setInfo(GameMode mode, int ccu = -1);

	void setCCU(int ccu);

	void onButtonRelease(cocos2d::ui::Button* button, int id);

	void reset();

	void show();

	void setMode(GameMode mode);

	void playWithBot(float dt);

private:
	cocos2d::Node *_nodeP1;
	cocos2d::Node *_nodeP2;

	cocos2d::ui::Button *_btnCancel;
	cocos2d::ui::Text *_lbMatching;

	cocos2d::ui::Text *_lbSubtitle;
	cocos2d::ui::Text *_lbCCU;
	cocos2d::ui::Text *_lbEstimate;
	cocos2d::ui::Text *_lbTime;

	cocos2d::ui::Text *_lbBtnCancel;
	cocos2d::Sprite *_iconCircle_0;
	cocos2d::Sprite *_iconCircle_1;

	float _timeElapsed;
	bool _foundOpponent;
	int _numDot;

	GameMode _gameMode;

	static const float AUTO_FIND_BOT_TIME;
};

#endif // !FIND_MATCH_LAYER_H



