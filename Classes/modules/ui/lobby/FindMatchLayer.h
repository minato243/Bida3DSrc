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

	void setPlayer2InfoVisible(bool isVisible);

	void onFoundOpponent(const Player & p2Data);

	void setInfo(GameMode mode, int ccu = -1);

	void setPlayerInfo(int idx, const Player & player);

	void onButtonRelease(cocos2d::ui::Button* button, int id);

	void setLayout(GameMode mode);

	void setLayoutRT1();

	void setLayoutS2();

	void setLayoutRT2();

	void setPlayerAvatar(int idx, std::string avatarStr);

	void initAvatarClippingNode(cocos2d::Node *node);

	void reset();

	void startScrollingAvatarEffect(std::vector<std::string> & imgList);

	void show();

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
};

#endif // !FIND_MATCH_LAYER_H



