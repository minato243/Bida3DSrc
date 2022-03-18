#ifndef IngameStartMatchingUI_H_
#define IngameStartMatchingUI_H_

#include <cocos2d.h>
#include <cocos/ui/CocosGUI.h>
#include <modules/core/match/Player.h>
#include "../base/UIBaseLayer.h"
#include "../../network/game/IngameInPacket.h"
#include "../../network/match/MatchingInPacket.h"

class IngameStartMatchingUI 
	: public UIBaseLayer
{
public:

	enum TAG
	{
		btnCard_0 = 0,
		btnCard_1 = 1,
		btnCard_2 = 2,
	};

	IngameStartMatchingUI();
	~IngameStartMatchingUI();

	bool init();
	CREATE_FUNC(IngameStartMatchingUI);
	
	void initGUI();

	void initTag();

	void show();

	void refresh();

	void onSelectCard(PickFirstTurnStateMsg *data);

	void doSelectCard();

	void onSelectPassive(int cardIdx);

	void updateDataPlayer(ResponseJoinRoomMsg *data);

	void updateDataPlayer(std::vector<Player> & player, GameMode gameMode, int channelId);

	void updatePlayerInfo(int team, int idxPlayer, const Player & player);

	void setMode(GameMode mode);

	void setModeRT1();

	void setNodePositionModeRT1(cocos2d::Node *node);

	void setNodePositionModeRT2(cocos2d::Node *node);

	void onButtonRelease(cocos2d::ui::Button* button, int id);

	void setTextContent();

	void setUIStart();

	void runActionLightFx(cocos2d::Node *lightFx, cocos2d::Node *parent, bool isReverse = false);

	void fakeDataPlayer();

	void fakeDataPickCard(float dt);

	void runFXMinusGold();

	void onShowPickCard();

	void openCardSelectOponent();

	void fxRandomSelect();

	void showUserWin();

	void updateTime(float dt);

	void hide();

	void runFlyCoin(cocos2d::Node *startNode, cocos2d::Node *endNode, int numberCoin);

	void onCoinMoveDone(cocos2d::Node *node);

private:
	cocos2d::ui::Widget *_panel_Text;
	cocos2d::ui::Text *_lb_Content;

	cocos2d::ui::Widget *_panelShowMode;
	cocos2d::Node *_nodeShowP1;
	cocos2d::Node *_nodeShowP2;
	cocos2d::Sprite *_iconRule;
	cocos2d::Sprite *_icon_Mode;

	cocos2d::Node *_nodeP1;
	cocos2d::Node *_nodeP2;

	cocos2d::ui::Text *_lb_TimeRemain;

	cocos2d::ui::Button *_btnCardOppnent;
	cocos2d::ui::Text *_lb_GoFirst;

	cocos2d::Vec2 _posSelectCard;
	bool _isInitPosSelectCard;

	static const int NUM_CARD = 3;
	cocos2d::ui::Button *_listCardSelect[NUM_CARD];

	int _selectedCardIdx;
	std::string _contentStr;

	int _numMember;
	ResponseJoinRoomMsg *_data;

	float _timeRemain;

	int _opponentSelected;
	bool _isWin;

	GameMode _gameMode;
	int _channelId;
	std::vector<Player> *_players;

public:
	static const float INGAME_START_CHOSE_PICK;
};

#endif // !IngameStartMatchingUI_H_



