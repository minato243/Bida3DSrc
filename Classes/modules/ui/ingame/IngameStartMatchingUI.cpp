#include "IngameStartMatchingUI.h"
#include "../../../data/Resource.h"
#include "core/utils/StringUtility.h"
#include "core/utils/Avatar.h"
#include "../../network/game/GameHandler.h"
#include "core/utils/LanguageMgr.h"
#include "core/GameMgr.h"
#include "../../core/utils/TimeUtils.h"
#include "data/config/ConfigMgr.h"
#include "../channel/ModeUIItem.h"
#include "../../core/utils/SpriteCoinFX.h"
#include "../utils/UpdatingGoldNumber.h"

USING_NS_CC;
using namespace cocos2d::ui;
using namespace std;

IngameStartMatchingUI::IngameStartMatchingUI()
{
	_selectedCardIdx = 0;
	_data = NULL;
	_numMember = 1;
	_opponentSelected = -1;
	_isWin = false;
	_isInitPosSelectCard = false;
}

IngameStartMatchingUI::~IngameStartMatchingUI()
{

}

bool IngameStartMatchingUI::init()
{
	if (!UIBaseLayer::init())
		return false;

	std::string filePath = Res::GUI::getCsbPathFromJson(Res::GUI::ImageStartMatchingUI);
	this->initWithBinaryFile(filePath);

	_keep = true;

	return true;
}

void IngameStartMatchingUI::initGUI()
{
	initTag();
	autoInitComponent(_layout);
	_bg					= _nodeMap["bg"];
	_panel_Text			= (Widget *) _nodeMap["panel_Text"];
	_lb_Content			= (Text *)_nodeMap["lb_Content"];

	_panelShowMode		= (Widget *) _nodeMap["panelShowMode"];
	_nodeP1				= getControl("nodeP1", _bg);
	_nodeP2				= getControl("nodeP2", _bg);
	_iconRule			= (Sprite *)getControl("iconRule", _panelShowMode);
	_icon_Mode			= (Sprite *)getControl("icon_Mode", _panelShowMode);

	_lb_TimeRemain		= (Text*)_nodeMap["lb_TimeRemain"];

	_listCardSelect[0] = (Button*)_nodeMap["btnCard_0"];
	_listCardSelect[1] = (Button*)_nodeMap["btnCard_1"];
	_listCardSelect[2] = (Button*)_nodeMap["btnCard_2"];

	_btnCardOppnent = (Button *)_nodeMap["btnCardOppnent"];
	_lb_GoFirst = (Text *)_nodeMap["lb_GoFirst"];
}

void IngameStartMatchingUI::initTag()
{
	_tagMap.insert(pair<string, int>("btnCard_0", btnCard_0));
	_tagMap.insert(pair<string, int>("btnCard_1", btnCard_1));
	_tagMap.insert(pair<string, int>("btnCard_2", btnCard_2));
}

void IngameStartMatchingUI::show()
{
	UIBaseLayer::show();
	refresh();
	setUIStart();
}

void IngameStartMatchingUI::onSelectCard(PickFirstTurnStateMsg *data)
{
	if (_opponentSelected == -1)
	{
		if (data->firstTurnPlayer == gameMgr->_userInfo->_uId)
		{
			_isWin = true;
		}
		else
		{
			_isWin = false;
		}

		bool selected = _selectedCardIdx != -1;
		for (auto ittr = data->keoBuaDao.begin(); ittr != data->keoBuaDao.end(); ittr++)
		{
			if (ittr->first == gameMgr->_userInfo->_uId)
			{
				_selectedCardIdx = ittr->second;
			}
			else
			{
				_opponentSelected = ittr->second;
			}
		}

		if (!selected)
		{
			onSelectPassive(_selectedCardIdx);
			return;
		}
	}

	doSelectCard();
}

void IngameStartMatchingUI::doSelectCard()
{
	CCLOG("IngameStartMatchingUI::doSelectCard %ld", timeUtils->current());
	unschedule(schedule_selector(IngameStartMatchingUI::updateTime));
	_lb_TimeRemain->setVisible(false);

	auto nodeP1 = _nodeP1;
	auto nodeP2 = _nodeP2;

	nodeP1->runAction(MoveTo::create(0.2, _defaultPos[nodeP1]));
	nodeP2->runAction(MoveTo::create(0.2, _defaultPos[nodeP2]));

	_lb_Content->runAction(ScaleTo::create(0.1, 0, 0));

	auto selectCard = _listCardSelect[_selectedCardIdx];
	selectCard->stopAllActions();
	selectCard->runAction(MoveTo::create(0.2, Vec2(0, selectCard->getPositionY())));
	selectCard->setTouchEnabled(false);

	auto card = _btnCardOppnent;
	card->setScale(0);
	card->setVisible(true);
	Sprite *oponentChoose = (Sprite *)getControl("type", card);
	oponentChoose->setSpriteFrame("StartGame_Loading.png");
	card->runAction(Sequence::create(
		DelayTime::create(0.1),
		ScaleTo::create(0.5, 1, 1),
		NULL
	));
		
	if (_opponentSelected != -1) {
		runAction(Sequence::create(
			DelayTime::create(1),
			CallFunc::create([this]() 
			{
				openCardSelectOponent();
			}),
			NULL
		));
	}
}

void IngameStartMatchingUI::onSelectPassive(int cardIdx)
{
	CCLOG("IngameStartMatchingUI::onSelectPassive %ld", timeUtils->current());
	for (int i = 0; i < NUM_CARD; i++)
	{
		auto btn = _listCardSelect[i];
		btn->setScale(1);
		btn->setTouchEnabled(false);
		btn->stopAllActions();

		if (i == cardIdx)
		{
			btn->runAction(Sequence::create(
				ScaleTo::create(0.1, 0, 1),
				ScaleTo::create(0.1, 1, 1),
				DelayTime::create(1),
				CallFunc::create(this, CC_CALLFUNC_SELECTOR(IngameStartMatchingUI::doSelectCard)),
				NULL
			));
		}
		else
		{
			CCLOG("onSelectPassive hide");
			btn->runAction(EaseSineIn::create(ScaleTo::create(0.1, 0, 0)));
		}

	}
}

void IngameStartMatchingUI::updateDataPlayer(ResponseJoinRoomMsg *data)
{
	_data = data;
	_numMember = 1;
	if (data->players.size() == 4)
		_numMember = 2;
	setMode(data->gameMode);
	
	for (int i = 0; i < data->players.size(); i++)
	{
		int team = i / _numMember;
		int idxPlayer = i % _numMember;
		updatePlayerInfo(team, idxPlayer, data->players[i]);
	}
}

void IngameStartMatchingUI::updateDataPlayer(std::vector<Player> & players, GameMode gameMode, int channelId)
{
	_players = &players;
	_gameMode = gameMode;
	_channelId = channelId;

	_numMember = 1;
	if (players.size() == 4)
		_numMember = 2;
	setMode(gameMode);

	for (int i = 0; i < players.size(); i++)
	{
		int team = i / _numMember;
		int idxPlayer = i % _numMember;
		updatePlayerInfo(team, idxPlayer, players[i]);
	}
}

void IngameStartMatchingUI::updatePlayerInfo(int team, int idxPlayer, const Player & player)
{
	auto node = getControl("nodeP" + to_string(team + 1), _bg);
	auto imgAvatarBoder = (ImageView*)getControl("imgAvatarBorder_"+ to_string(idxPlayer), node);

	auto lbName = (Text*)getControl("lb_Name", imgAvatarBoder);
	auto lbGold = (Text*)getControl("lb_Gold", imgAvatarBoder);
	auto lbGoldMinus = (Text*)getControl("lb_Gold_Minus", imgAvatarBoder);

	auto avatar = Avatar::create(Res::INGAME_AVATAR_FAKE_1, player.avatar);
	imgAvatarBoder->addChild(avatar);
	auto pos = Vec2(imgAvatarBoder->getContentSize().width/2, imgAvatarBoder->getContentSize().height/2);
	avatar->setPosition(pos);
	auto scale = imgAvatarBoder->getContentSize().width / avatar->defaultAvatar->getContentSize().width;
	avatar->setScale(scale);

	lbName->setString(player.name);
	lbGold->setString(StringUtility::standartNumber(player.gold));
	
	lbGoldMinus->setPositionX(lbGold->getPositionX() + (lbGold->getContentSize().width + 10) * team == 0?1:-1);
	lbGoldMinus->setPositionY(lbGold->getPositionY());

	imgAvatarBoder->runAction(MoveTo::create(0.3f, _defaultPos[imgAvatarBoder]));
}

void IngameStartMatchingUI::setMode(GameMode mode)
{
	_icon_Mode->setSpriteFrame(SpriteFrameCache::getInstance()->getSpriteFrameByName(ModeUIItem::ICON_MODE[(int)mode - 1]));
	_iconRule->setSpriteFrame(SpriteFrameCache::getInstance()->getSpriteFrameByName(ModeUIItem::RULE[ModeUIItem::RULE_8][(int)mode-1]));
	Sprite *image = (Sprite *)getControl("image", _panelShowMode);
	image->setSpriteFrame(SpriteFrameCache::getInstance()->getSpriteFrameByName(ModeUIItem::NAME_MODE[(int)mode - 1]));
	_iconRule->setScale(0);
	switch (mode)
	{
	case QUICK_MODE:
	case STREAK_MODE:
	case RT2_MODE:
		setModeRT1();
		break;
	case RT3_MODE:
		setNodePositionModeRT2(_nodeP1);
		setNodePositionModeRT2(_nodeP2);
		break;
	default:
		break;
	}
}

void IngameStartMatchingUI::setModeRT1()
{
	setNodePositionModeRT1(_nodeP1);
	setNodePositionModeRT1(_nodeP2);
}

void IngameStartMatchingUI::setNodePositionModeRT1(cocos2d::Node *node)
{
	auto imgAvatarBorder_0 = getControl("imgAvatarBorder_0", node);
	imgAvatarBorder_0->setVisible(true);
	imgAvatarBorder_0->setPositionY(100);

	auto imgAvatarBorder_1 = getControl("imgAvatarBorder_1", node);
	imgAvatarBorder_1->setVisible(false);
}

void IngameStartMatchingUI::setNodePositionModeRT2(cocos2d::Node *node)
{
	auto imgAvatarBorder_0 = getControl("imgAvatarBorder_0", node);
	imgAvatarBorder_0->setPositionY(75);

	auto imgAvatarBorder_1 = getControl("imgAvatarBorder_1", node);
	imgAvatarBorder_1->setVisible(true);
}

void IngameStartMatchingUI::onButtonRelease(cocos2d::ui::Button* button, int id)
{
	gameHandler->sendPickKeoBuabao(id);
	_selectedCardIdx = id;
	_contentStr = languageMgr->localizedText("text_you_chose") + " " + languageMgr->localizedText("text_type_card_" + to_string(id));
	_lb_Content->stopAllActions();
	_lb_Content->runAction(
		Sequence::create(ScaleTo::create(0.1, 0, 0),
			CallFunc::create(CC_CALLBACK_0(IngameStartMatchingUI::setTextContent, this)),
			DelayTime::create(0.1),
			ScaleTo::create(0.1, 1, 1),
			NULL
		));

	for (int i = 0; i < NUM_CARD; i++)
	{
		_listCardSelect[i]->setTouchEnabled(false);
		if (id == i)
		{
			if (i == 1)
			{
				button->runAction(Sequence::create(
					ScaleTo::create(0.1, 0, 1), 
					ScaleTo::create(0.1, 1, 1),
					EaseSineOut::create(MoveTo::create(0.5, Vec2(0, _defaultPos[button].y))),
					DelayTime::create(0.1),
					NULL
				));
			}
			else
			{
				button->runAction(Sequence::create(
					ScaleTo::create(0.1, 0, 1),
					ScaleTo::create(0.1, 1, 1),
					MoveTo::create(0.5, Vec2(0, _defaultPos[button].y)),
					DelayTime::create(0.1),
					NULL
				));
			}
		}
		else
		{
			_listCardSelect[i]->runAction(EaseSineIn::create(ScaleTo::create(0.2, 0, 0)));
		}
	}
}

void IngameStartMatchingUI::setTextContent()
{
	_lb_Content->setString(_contentStr);

	auto line = getControl("line_0", _panel_Text);
	line->stopAllActions();
	auto pos = Vec2(_lb_Content->getPositionX() - _lb_Content->getContentSize().width / 2 - 10, line->getPositionY());
	line->runAction(MoveTo::create(0.3, pos));

	auto line2 = getControl("line_1", _panel_Text);
	line2->stopAllActions();
	auto pos2 = Vec2(_lb_Content->getPositionX() + _lb_Content->getContentSize().width / 2 + 10, line->getPositionY());
	line2->runAction(MoveTo::create(0.3, pos2));
}

void IngameStartMatchingUI::setUIStart()
{
	CCLOG("IngameStartMatchingUI::setUIStart");
	for (int i = 0; i < NUM_CARD; i++)
	{
		_listCardSelect[i]->setPosition(_defaultPos[_listCardSelect[i]]);
		_listCardSelect[i]->setScale(0);
		_listCardSelect[i]->setTouchEnabled(true);
		_listCardSelect[i]->setOpacity(255);
	}

	_btnCardOppnent->setVisible(false);
	_btnCardOppnent->setOpacity(255);
	_lb_GoFirst->setVisible(false);

	auto line1 = _nodeMap["StartGame_line_1"];
	auto lightFx1 = getControl("lightFx", line1);
	lightFx1->setOpacity(0);
	lightFx1->setPositionX(0);
	runActionLightFx(lightFx1, line1);

	auto line2 = _nodeMap["StartGame_line_0"];
	auto lightFx2 = getControl("lightFx", line2);
	lightFx2->setPositionX(line2->getContentSize().width);
	runActionLightFx(lightFx2, line2, true);

	_panelShowMode->setVisible(true);
	_panelShowMode->setOpacity(255);
	Sprite *image = (Sprite *)getControl("image", _panelShowMode);
	image->runAction(Sequence::create(
		EaseSineInOut::create(MoveTo::create(1.f, _defaultPos[image])),
		DelayTime::create(0.5),
		NULL
	));
		
	auto bg_Icon = getControl("bg_Icon", _panelShowMode);
	_contentStr = languageMgr->localizedText("text_8_ball_rule");
	setTextContent();
	_lb_Content->setScale(0);
	_lb_Content->runAction(EaseSineInOut::create(ScaleTo::create(0.5, 1, 1)));
	bg_Icon->runAction(EaseSineInOut::create(ScaleTo::create(0.5, 1, 1)));

	_iconRule->setOpacity(255);

	_icon_Mode->setScale(0);
	_icon_Mode->setOpacity(255);
	_icon_Mode->runAction(EaseSineInOut::create(ScaleTo::create(0.2, 1)));
	
	_lb_TimeRemain->setScale(0);

	_panelShowMode->stopAllActions();
	_panelShowMode->runAction(
		Sequence::create(
			DelayTime::create(2), 
			CallFunc::create(CC_CALLBACK_0(IngameStartMatchingUI::runFXMinusGold, this)),
			DelayTime::create(3.5), 
			CallFunc::create(CC_CALLBACK_0(IngameStartMatchingUI::onShowPickCard, this)),
			NULL
		));

	//scheduleOnce(schedule_selector(IngameStartMatchingUI::fakeDataPickCard), 7);
}

void IngameStartMatchingUI::runActionLightFx(cocos2d::Node *lightFx, cocos2d::Node *parent, bool isReverse)
{
	auto imgSize = parent->getContentSize();
	auto action = Sequence::create(
		EaseSineInOut::create(MoveTo::create(1, Vec2(imgSize.width/2, lightFx->getPositionY()))),
		EaseSineInOut::create(MoveTo::create(1, Vec2(isReverse?0:imgSize.width, lightFx->getPositionY()))),
		NULL
	);
	lightFx->runAction(action);

	float time = lightFx->getContentSize().width / imgSize.width;
	auto action2 = Sequence::create(
		FadeIn::create(time),
		DelayTime::create(2.5 - 2 * time),
		FadeOut::create(time),
		NULL
	);
	lightFx->runAction(action2);
}

void IngameStartMatchingUI::fakeDataPlayer()
{
	ResponseJoinRoomMsg *msg = new ResponseJoinRoomMsg(new fr::InPacket());
	msg->gameMode = QUICK_MODE;
	for (int i = 0; i < 2; i++)
	{
		Player p = Player();
		p.id = gameMgr->_userInfo->_uId+i;
		p.name = "Fake " + 1;
		p.gold = 10000;
		msg->players.push_back(p);
	}

	this->updateDataPlayer(msg);

}

void IngameStartMatchingUI::fakeDataPickCard(float dt)
{
	CCLOG("IngameStartMatchingUI::fakeDataPickCard %ld", timeUtils->current());
	PickFirstTurnStateMsg *pickFirstTurn = new PickFirstTurnStateMsg(new fr::InPacket());
	pickFirstTurn->firstTurnPlayer = gameMgr->_userInfo->_uId+1;
	pickFirstTurn->keoBuaDao[_data->players[0].id] = 0;
	pickFirstTurn->keoBuaDao[_data->players[1].id] = 0;
	
	onSelectCard(pickFirstTurn);
}

void IngameStartMatchingUI::runFXMinusGold()
{
	auto gold = configMgr->channelConfig->getChannelConfig(_gameMode, _channelId).entryFee;
	if (_gameMode == GameMode::STREAK_MODE && gameMgr->_userInfo->getLastS2MatchResult(_channelId)) {
		gold = 0;
		return;
	}
	auto players = *_players;
	auto ebpc = gameMgr->_ebpCtrl;

	for (int i = 0; i < players.size(); i++)
	{
		int team = i / _numMember;
		int playerIdx = i % _numMember;

		players[i].bet(gold);

		auto nodeP = getControl("nodeP" + to_string(team + 1), _bg);
		auto imageBorder = getControl("imgAvatarBorder_"+to_string(playerIdx), nodeP);
		auto lbGoldMinus = (Text*)getControl("lb_Gold_Minus", imageBorder);
		auto lbGold = (Text*)getControl("lb_Gold", imageBorder);
		UpdatingGoldNumber::create(lbGold, players[i].gold + gold, players[i].gold, "player"+to_string(i));
		lbGoldMinus->setString("-" + StringUtility::standartNumber(gold));
		lbGoldMinus->setOpacity(255);
		lbGoldMinus->runAction(Sequence::create(
			EaseBackOut::create(ScaleTo::create(0.5, 1, 1)), 
			Spawn::create(
				MoveBy::create(0.75, Vec2(0, 30)), 
				Spawn::create(
					FadeOut::create(0.75), 
					MoveBy::create(0.75, Vec2(0, 20)),
                              NULL
				)
			),
			NULL
		));

		runFlyCoin(imageBorder, _icon_Mode, 10);
		
	}
	_iconRule->runAction(Sequence::create(
		DelayTime::create(0.7f),
		ScaleTo::create(1, 1),
		NULL
	));
}

void IngameStartMatchingUI::onShowPickCard()
{
	CCLOG("IngameStartMatchingUI::onShowPickCard");
	_lb_Content->runAction(Sequence::create(
		ScaleTo::create(0.1, 0, 0),
		CallFunc::create([this]() {
			_contentStr = languageMgr->localizedText("text_pick_card");
			setTextContent();
		}
		),
		ScaleTo::create(0.1, 1),
		NULL
	));
	_icon_Mode->runAction(FadeOut::create(0.1));
	_iconRule->runAction(FadeOut::create(0.1));

	_lb_TimeRemain->stopAllActions();
	_lb_TimeRemain->setScale(0);
	_lb_TimeRemain->runAction(EaseSineInOut::create(ScaleTo::create(0.1, 1)));

	for (int i = 0; i < NUM_CARD; i++)
	{
		auto btn = _listCardSelect[i];
		btn->setScale(0);
		btn->runAction(
			Sequence::create(
				DelayTime::create(i * 0.1), 
				EaseSineInOut::create(ScaleTo::create(0.1, 1, 1)),
				NULL
			));
	}
	
	_timeRemain = INGAME_START_CHOSE_PICK;
	_lb_TimeRemain->setString(to_string((int)_timeRemain));
	schedule(schedule_selector(IngameStartMatchingUI::updateTime), 1);
}


void IngameStartMatchingUI::openCardSelectOponent()
{
	CCLOG("openCardSelectOponent %d", _opponentSelected);
	auto card = _btnCardOppnent;
	card->runAction(Sequence::create(
		ScaleTo::create(0.1, 0, 1),
		CallFunc::create([this]() {
			auto node = _btnCardOppnent;
			auto type = (Sprite *)getControl("type", node);
			type->setSpriteFrame("StartGame_" + to_string(_opponentSelected) + ".png");
			}),
		ScaleTo::create(0.1, 1, 1),
		NULL
	));

	runAction(Sequence::create(
		DelayTime::create(1),
		CallFunc::create([this]() 
		{
			if (_selectedCardIdx == _opponentSelected) {
				fxRandomSelect();
			}
			else {
				showUserWin();
			}
		}),
		NULL
	));
}

void IngameStartMatchingUI::fxRandomSelect()
{
	auto card1 = _listCardSelect[_selectedCardIdx];
	auto card2 = _btnCardOppnent;

	card1->runAction(Sequence::create(
		FadeTo::create(0.3, 255), 
		FadeTo::create(0.3, 0), 
		FadeTo::create(0.6, 255), 
		FadeTo::create(0.6, 0), 
		FadeTo::create(0.9, 255), 
		CallFunc::create([this]() {
			showUserWin();
		}),
		NULL
	));

	card2->runAction(Sequence::create(
		FadeTo::create(0.3, 0), 
		FadeTo::create(0.3, 255), 
		FadeTo::create(0.6, 0), 
		FadeTo::create(0.6, 255), 
		FadeTo::create(0.9, 0), 
		FadeTo::create(0.1, 255),
		NULL
	));
}

void IngameStartMatchingUI::showUserWin()
{
	CCLOG("IngameStartMatchingUI::showUserWin %ld", timeUtils->current());
	_lb_GoFirst->setVisible(true);
	_lb_GoFirst->setScale(0);
	_lb_GoFirst->runAction(
		EaseSineIn::create(ScaleTo::create(0.1, 1))
	);

	if (_isWin)
	{
		_lb_GoFirst->setString(languageMgr->localizedText("text_GoFirst"));
		_nodeP2->runAction(FadeTo::create(0.2f, 100));
		_btnCardOppnent->runAction(FadeTo::create(0.2, 100));
	}
	else
	{
		_lb_GoFirst->setString(languageMgr->localizedText("text_GoSecond"));
		_nodeP2->runAction(FadeTo::create(0.2f, 100));
		_listCardSelect[_selectedCardIdx]->runAction(FadeTo::create(0.2, 100));
	}

	runAction(Sequence::create(
		DelayTime::create(0.5f),
		CallFunc::create([this]() {hide(); }),
		NULL
	));
}

void IngameStartMatchingUI::updateTime(float dt)
{
	_timeRemain -= 1;
	_lb_TimeRemain->setString(to_string((int)_timeRemain));

	if (_timeRemain <= 0)
	{
		unschedule(schedule_selector(IngameStartMatchingUI::updateTime));
	}
}

void IngameStartMatchingUI::hide()
{
	BaseLayer::hide();
}

void IngameStartMatchingUI::runFlyCoin(cocos2d::Node *startNode, cocos2d::Node *endNode, int numberCoin)
{
	Size size = startNode->getContentSize();
	Vec2 startPos = startNode->convertToWorldSpace(Vec2(size.width/2,size.height/2));
	startPos = convertToNodeSpace(startPos);

	size = endNode->getContentSize();
	Vec2 endPos = endNode->convertToWorldSpace(Vec2(size.width/2, size.height/2));
	endPos = convertToNodeSpace(endPos);

	JumpBy *actionJump = JumpBy::create(0.4, Vec2(0, 15), 15, 3);

	for (auto i = 0; i < numberCoin; i++) {
		Vec2 pos = Vec2(startPos.x + rand() %150 - 75, startPos.y + rand()%100 -75);
		Vec2 posMid = Vec2(endPos.x * 2/3 + pos.x /3, endPos.y /2 + pos.y/2);
		if (endPos.y > startPos.y) posMid.y = startPos.y - rand() % 100 - 50;
		else posMid.y = startPos.y + rand() % 100 - 50;

		Sprite *sprite = SpriteGoldCoin::create();
		sprite->runAction(Sequence::create(
			DelayTime::create(rand_0_1() *0.2f),
			ScaleTo::create(0.05, 0.8, 1),
			EaseBackInOut::create(ScaleTo::create(0.2, 0.6, 0.6)),
			NULL
		));
		sprite->setGlobalZOrder(400);
		addChild(sprite);
		sprite->setPosition(pos);
		sprite->setVisible(true);

		ccBezierConfig bezierConf;
		bezierConf.controlPoint_1 = pos;
		bezierConf.controlPoint_2 = posMid;
		bezierConf.endPosition = endPos;
		EaseIn *actionMove = EaseIn::create(BezierTo::create(0.75, bezierConf),1);
		ScaleTo *actionScale = ScaleTo::create(0.75, 0.25f);

		sprite->runAction(Sequence::create(
			actionJump->clone(),
			DelayTime::create(i *0.075),
			Spawn::createWithTwoActions(
				actionMove,
				actionScale
			),
			CallFunc::create([this, endNode]() {
			endNode->stopAllActions();
			endNode->runAction(Sequence::create(
				ScaleTo::create(0.02f, 1.2f),
				ScaleTo::create(0.02f, 1),
				NULL
			));
		}),
			CallFuncN::create(CC_CALLBACK_1(IngameStartMatchingUI::onCoinMoveDone, this)),
			NULL
		));
	}
}

void IngameStartMatchingUI::onCoinMoveDone(cocos2d::Node *node)
{
	node->removeFromParent();
}

void IngameStartMatchingUI::refresh()
{
	_selectedCardIdx = -1;
	_data = NULL;
	_numMember = 1;
	_opponentSelected = -1;
}

const float IngameStartMatchingUI::INGAME_START_CHOSE_PICK = 3.f;


