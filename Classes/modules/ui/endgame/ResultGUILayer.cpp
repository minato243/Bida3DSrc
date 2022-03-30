#include "ResultGUILayer.h"
#include "../../../data/Resource.h"
#include "../../../core/GameMgr.h"
#include "../channel/ModeUIItem.h"
#include "../../../core/utils/LanguageMgr.h"
#include "../../scene/GameScene.h"
#include "../../../core/utils/Avatar.h"
#include "../utils/UpdatingGoldNumber.h"
#include "core/utils/StringUtility.h"
#include "../../core/channel/ChannelMgr.h"

USING_NS_CC;
using namespace cocos2d::ui;
using namespace std;

ResultGUILayer::ResultGUILayer()
	:_winner(0), _gameMode(NO_MODE),_channelId(0)
{
	_keep = true;
}

ResultGUILayer::~ResultGUILayer()
{

}

bool ResultGUILayer::init()
{
	if (!UIBaseLayer::init("ResultGUILayer"))
	{
		return false;
	}

	string filePath = Res::GUI::getCsbPathFromJson(Res::GUI::ResultGUILayer);
	this->initWithBinaryFile(filePath);

	createUserRt3();
	return true;
}

void ResultGUILayer::initGUI()
{
	initTag();
	autoInitComponent(_layout);
	_bg = _nodeMap["bg"];
	_btnLobby = (Button *)_nodeMap["btnLobby"];
	_btnRePlay = (Button *)_nodeMap["btnRePlay"];

	_panelInfoGame = (cocos2d::ui::ImageView *) _nodeMap["panelInGame"];
	_imageMode = (cocos2d::Sprite *) _nodeMap["img_Mode"];
	_img_Channel = (cocos2d::Sprite *)_nodeMap["img_Channel"];
	_imageWinLose = (cocos2d::Sprite *) _nodeMap["img_LoseWin"];
	_img_ModePlay = (cocos2d::Sprite *) _nodeMap["img_ModePlay"];

	_panelUser_0 = (cocos2d::ui::Widget *) _nodeMap["panelUser_0"];
	_panelUser_1 = (cocos2d::ui::Widget *) _nodeMap["panelUser_1"];
	_userNodeList[0] = _panelUser_0;
	_userNodeList[1] = _panelUser_1;

	_panelUserRT3_0 = (cocos2d::ui::Widget *) _nodeMap["panelUserRT3_0"];
	_panelUserRT3_1 = (cocos2d::ui::Widget *) _nodeMap["panelUserRT3_1"];

	_text_Game_1 = (cocos2d::ui::Text *) _nodeMap["text_Game_1"];
	_text_Game_2 = (cocos2d::ui::Text *) _nodeMap["text_Game_2"];
	
	_check_1 = (cocos2d::Sprite *)(getControl("check", getControl("box", _text_Game_1)));
	_check_2 = (cocos2d::Sprite *)(getControl("check", getControl("box", _text_Game_2)));

}

void ResultGUILayer::initTag()
{
	_tagMap["btnLobby"] = btnLobby;
	_tagMap["btnRePlay"] = btnRePlay;
}

void ResultGUILayer::createUserRt3()
{
	int idx = 0;
	for (int j = 0; j < 2; j++) {
		auto nodePlayer = PlayerInfoRT3::create();
		_panelUserRT3_0->getChildByName("Node_" + to_string(j))->addChild(nodePlayer);
		_userRT3NodeList[idx++] = nodePlayer;
	}

	for (int j = 0; j < 2; j++) {
		auto nodePlayer = PlayerInfoRT3::create();
		_panelUserRT3_1->getChildByName("Node_" + to_string(j))->addChild(nodePlayer);
		_userRT3NodeList[idx++] = nodePlayer;
	}
}

void ResultGUILayer::onButtonRelease(cocos2d::ui::Button* button, int id)
{
	switch (id)
	{
	case btnLobby:
	{
		hide();
		auto gameScene = (GameScene *)gameMgr->_scene;
		gameScene->_lobbyUI->setVisible(true);
		break;
	}
	case btnRePlay:
	{
		gameMgr->_ebpCtrl->gotoMode(_gameMode, 1);
		hide();
		break;
	}
	default:
		break;
	}
}

void ResultGUILayer::loadData(GameMode gameMode, int channelId, int winner)
{
	CCLOG("ResultGUILayer::loadData %d %d %d", gameMode, channelId, winner);
	//set list reward
	_gameMode = gameMode;
	_winner = winner;
	_channelId = channelId;

	if (_gameMode != GameMode::STREAK_MODE) {
		auto itemList = channelMgr->getListItemWin(gameMode, channelId);
		_listGift.insert(_listGift.begin(), itemList.begin(), itemList.end());
	}

	_text_Game_1->setVisible(gameMode == GameMode::STREAK_MODE);
	_text_Game_2->setVisible(gameMode == GameMode::STREAK_MODE);

	_panelUser_0->setVisible(gameMode != GameMode::RT3_MODE);
	_panelUser_1->setVisible(gameMode != GameMode::RT3_MODE);

	Sprite *img_Number0 = (Sprite *)getControl("img_Number", _panelUser_0);
	Sprite *img_Number1 = (Sprite *)getControl("img_Number", _panelUser_1);
	img_Number0->setVisible(gameMode != GameMode::STREAK_MODE);
	img_Number1->setVisible(gameMode != GameMode::STREAK_MODE);

	_panelUserRT3_0->setVisible(gameMode == GameMode::RT3_MODE);
	_panelUserRT3_1->setVisible(gameMode == GameMode::RT3_MODE);

	if (winner == 0){
		_imageWinLose->setSpriteFrame("EndGame_YOUWIN.png");
	}
	else{
		_imageWinLose->setSpriteFrame("EndGame_YOULOSE.png");
	}

	_img_Channel->setSpriteFrame(ModeUIItem::NAME_CHANNEL[channelId]);
	_imageMode->setSpriteFrame(ModeUIItem::NAME_MODE[gameMode - 1]);
	getControl("left", _imageMode)->setPositionX(0);
	getControl("right", _imageMode)->setPositionX(_imageMode->getContentSize().width);
	_img_ModePlay->setSpriteFrame(ModeUIItem::ICON_MODE[gameMode - 1]);
}

void ResultGUILayer::loadDataStreak(const std::vector<bool> & listStreak)
{
	if (listStreak.size() > 1)
	{
		_check_1->setVisible(true);
		_check_1->setSpriteFrame(listStreak[1]? "EndGame_Check.png" : "EndGame_EndGame_X.png");
	}
	else
	{
		_check_1->setVisible(false);
	}
}

void ResultGUILayer::loadDataStreak(GameMode _gameMode, int channelId, int winner, const std::vector<bool> & listStreak)
{
	loadData(_gameMode, channelId, winner);
	loadDataStreak(listStreak);
}

void ResultGUILayer::loadDataRT1(int channelId, int winnerIdx)
{
	loadData(GameMode::QUICK_MODE, channelId, winnerIdx);
	Sprite *img_Number0 = (Sprite *)getControl("img_Number", _panelUser_0);
	Sprite *img_Number1 = (Sprite *)getControl("img_Number", _panelUser_1);

	img_Number0->setSpriteFrame(winnerIdx == 0 ? "1_a.png" : "0_a.png");
	img_Number1->setSpriteFrame(winnerIdx == 0 ? "0_b.png" : "1_b.png");
}

void ResultGUILayer::loadDataRT2(int channelId, int winner, int scoreP1, int scoreP2)
{
	loadData(GameMode::RT2_MODE, channelId, winner);
	Sprite *img_Number0 = (Sprite *)getControl("img_Number", _panelUser_0);
	Sprite *img_Number1 = (Sprite *)getControl("img_Number", _panelUser_1);

	img_Number0->setSpriteFrame(to_string(scoreP1) + "_a.png");
	img_Number1->setSpriteFrame(to_string(scoreP2) + "_b.png");
}

void ResultGUILayer::loadDataRT3(int channelId, int winner, int scoreP1, int scoreP2)
{
	loadData(GameMode::RT3_MODE, channelId, winner);
	Sprite *img_Number0 = (Sprite *)getControl("img_Number", _panelUserRT3_0);
	Sprite *img_Number1 = (Sprite *)getControl("img_Number", _panelUserRT3_1);

	img_Number0->setSpriteFrame(to_string(scoreP1)+"_a.png");
	img_Number1->setSpriteFrame(to_string(scoreP2) + "_b.png");
}

void ResultGUILayer::loadUserInfo(const std::vector<Player> & listPlayer)
{
	_listPlayer = listPlayer;

	Node *node = NULL;
	bool is2P = listPlayer.size() == 2;

	for (auto i = 0; i < listPlayer.size(); i++)
	{
		auto player = listPlayer[i];
		Node *node = _userNodeList[i];

		ImageView *imgAvatarBorder = (ImageView *)getControl("imgAvatarBorder", node);
		auto avatar = Avatar::create(Res::INGAME_AVATAR_FAKE_1, player.avatar);
		avatar->setScale(imgAvatarBorder->getContentSize().width/avatar->defaultAvatar->getContentSize().width *0.95);
		imgAvatarBorder->addChild(avatar);
		avatar->setLocalZOrder(-1);
		avatar->setPosition(Vec2(imgAvatarBorder->getContentSize().width/2, imgAvatarBorder->getContentSize().height/2));
		Sprite *crown = (Sprite *)getControl("crown", imgAvatarBorder);
		crown->setVisible(_winner == i);
		
		Sprite *goldBar = (Sprite *)getControl("goldBar", node);
		Text *lbGold = (Text *)getControl("lbGold", goldBar);
		lbGold->setString(StringUtility::standartNumber(player.gold));
		
		Text *lbName = (Text *)getControl("lbName", node);
		lbName->setString(player.name);
		
		//TODO update clup ranking, carer
		auto nodeRank = getControl("nodeRank", node);
		auto bg_Rank = getControl("bg_Rank", nodeRank);;
		Text *lb_NameRank = (Text *)getControl("lb_NameRank", bg_Rank);
		lb_NameRank->setVisible(false);

		auto imgLevel = getControl("imgLevel", node);
		Text *lbLevel = (Text *)getControl("lbLevel", imgLevel);
	}
}

void ResultGUILayer::loadRT3UserInfo(const std::vector<Player> & listPlayer)
{
	for (auto i = 0; i < listPlayer.size(); i++) {
		Node *node = _userRT3NodeList[i];
		PlayerInfoRT3 *playerInfo = dynamic_cast<PlayerInfoRT3 *>(node);
		playerInfo->updateInfoPlayer(listPlayer[i], _winner == (i/2));
	}
}

void ResultGUILayer::show()
{
	UIBaseLayer::show();
	_bg->setOpacity(0);

	_panelInfoGame->setPositionY(_defaultPos[_panelInfoGame].y + _panelInfoGame->getContentSize().width);
	
	_panelUserRT3_0->setPositionX(-_panelUserRT3_0->getContentSize().width);
	_panelUserRT3_1->setPositionX(_defaultPos[_panelUserRT3_1].x + _panelUserRT3_1->getContentSize().width);

	_panelUser_0->setPositionX(-_panelUser_0->getContentSize().width);
	_panelUser_1->setPositionX(_defaultPos[_panelUser_1].x + _panelUser_1->getContentSize().width);
	_text_Game_1->setScale(0);
	_text_Game_2->setScale(0);

	_btnRePlay->setScale(0);
	_btnLobby->setScale(0);

	_bg->runAction(FadeIn::create(0.2f));
	float timeDelay = 0.5f;
	_panelInfoGame->runAction(Sequence::create(
		DelayTime::create(timeDelay),
		MoveTo::create(0.5, _defaultPos[_panelInfoGame]),
		NULL
	));
	timeDelay += 0.5f;

	if (_panelUser_0->isVisible()) {
		_panelUser_0->runAction(Sequence::create(
			DelayTime::create(timeDelay),
			MoveTo::create(0.5, _defaultPos[_panelUser_0]),
			NULL
		));
		_panelUser_1->runAction(Sequence::create(
			DelayTime::create(timeDelay),
			MoveTo::create(0.5, _defaultPos[_panelUser_1]),
			NULL
		));
		timeDelay += 0.5f;
	}

	if (_panelUserRT3_0->isVisible()) {
		moveToDefaultPos(_panelUserRT3_0, timeDelay);
		moveToDefaultPos(_panelUserRT3_1, timeDelay);
		timeDelay += 0.5f;
	}

	if (_text_Game_1->isVisible()) {
		_text_Game_1->runAction(Sequence::create(
			DelayTime::create(timeDelay),
			ScaleTo::create(0.5f, 1),
			NULL
		));
		_text_Game_2->runAction(Sequence::create(
			DelayTime::create(timeDelay),
			ScaleTo::create(0.5f, 1),
			NULL
		));
		timeDelay += 0.5f;
	}

	runAction(Sequence::create(
		DelayTime::create(timeDelay),
		CallFunc::create(CC_CALLBACK_0(ResultGUILayer::runFXExp, this)),
		NULL
	));

	if (_listGift.size() == 0 || _gameMode == GameMode::RT3_MODE) {

	}
	else {
		runAction(Sequence::create(
			DelayTime::create(timeDelay),
			CallFunc::create(CC_CALLBACK_0(ResultGUILayer::runFXMinus, this)),
			NULL
		));
		timeDelay += 1;
	}
	_btnRePlay->runAction(Sequence::create(
		DelayTime::create(timeDelay),
		ScaleTo::create(0.2f,1),
		NULL
	));

	_btnLobby->runAction(Sequence::create(
		DelayTime::create(timeDelay),
		ScaleTo::create(0.2f, 1),
		NULL
	));
}

void ResultGUILayer::moveToDefaultPos(cocos2d::Node *node, float delayTime, float moveTime)
{
	node->runAction(Sequence::create(
		DelayTime::create(delayTime),
		MoveTo::create(0.5, _defaultPos[node]),
		NULL
	));
}

void ResultGUILayer::runFXExp()
{
	
}

void ResultGUILayer::runFXMinus()
{
	if (_listGift.size() == 0)
		return;
	long long gold = _listGift[0].num;
	if (_gameMode == GameMode::RT3_MODE)
		return;

	auto player = _listPlayer[0];
	Text *lb;
	if (_winner == 0) {
		lb = (Text *)getControl("lbGold", getControl("goldBar", _panelUser_0));
	}
	else {
		lb = (Text *)getControl("lbGold", getControl("goldBar", _panelUser_1));
		player = _listPlayer[1];
	}

	UpdatingGoldNumber::create(lb, player.gold, player.gold + gold, "ResultGUILayer");
}

void ResultGUILayer::fakeUserData()
{
	std::vector<Player> players;
	for (auto i = 0; i < 2; i++) {
		Player p = Player();
		p.id = gameMgr->_userInfo->_uId + i;
		p.name = "Fake " + 1;
		p.gold = 10000;

		players.push_back(p);
	}
	loadUserInfo(players);
}

void ResultGUILayer::fakeRT3UserData()
{
	std::vector<Player> players;
	for (auto i = 0; i < 4; i++) {
		Player p = Player();
		p.id = gameMgr->_userInfo->_uId + i;
		p.name = "Fake " + 1;
		p.gold = 10000;

		players.push_back(p);
	}
	loadRT3UserInfo(players);
}

const std::string ResultGUILayer::GRADIENT_INFO[NUM_MODE] = 
{
	"EndGame_1v1_Gradient.png",
	"EndGame_Streak2_Gradient.png",
	"EndGame_RT2_Gradient.png",
	"EndGame_RT3_Gradient.png"
};

const std::string ResultGUILayer::BG_INFO[NUM_MODE] = 
{
	"EndGame_1v1_Board.png",
	"EndGame_Streak2_Board.png",
	"EndGame_RT2_Board.png",
	"EndGame_RT3_Board.png"
};


// score panel
ScorePanel::ScorePanel()
{

}

ScorePanel::~ScorePanel()
{

}

bool ScorePanel::init()
{
	if (!Node::init())
		return false;

	string filePath = Res::GUI::getCsbPathFromJson(Res::GUI::ResultGUILayer);
	auto gui = CSLoader::createNode(filePath);
	addChild(gui);

	return true;
}

void ScorePanel::initGUI()
{

}

//PlayerInfoRT3-----------------
PlayerInfoRT3::PlayerInfoRT3()
	:Node()
{

}

PlayerInfoRT3::~PlayerInfoRT3()
{

}

bool PlayerInfoRT3::init()
{
	if (!Node::init())
		return false;
	std::string filePath = Res::GUI::getCsbPathFromJson(Res::GUI::PlayerInfoRT3);
	_gui = CSLoader::createNode(filePath);
	addChild(_gui);
	_gui->setScale(0.8f);

	initGUI();
	return true;
}

void PlayerInfoRT3::initGUI()
{
	_imgAvatarBorder = (ImageView *)_gui->getChildByName("imgAvatarBorder");
	_crown = (Sprite *)_imgAvatarBorder->getChildByName("crown");

	_lbGold = (Text *)_gui->getChildByName("Image_57")->getChildByName("lbGold");
	_ldBar = (LoadingBar *)_gui->getChildByName("panelBar")->getChildByName("ldBar");
	_lbLevel = (Text *)_gui->getChildByName("imgLevel")->getChildByName("lbLevel");
	_nodeRank = _gui->getChildByName("nodeRank");
	_lb_NameRank = (Text *)_nodeRank->getChildByName("bg_Rank")->getChildByName("lb_NameRank");
	_textNoRank = (Text *)_nodeRank->getChildByName("textNoRank");
}

void PlayerInfoRT3::updateInfoPlayer(const Player& player, bool isWinner)
{
	auto avatarNode = _imgAvatarBorder->getChildByTag(TAG_COMMON);
	auto avatar = dynamic_cast<Avatar *>(avatarNode);
	if (!avatar) {
		auto avatar = Avatar::create(Res::INGAME_AVATAR_FAKE_1, player.avatar);
		avatar->setTag(TAG_COMMON);
		avatar->setScale(_imgAvatarBorder->getContentSize().width / avatar->defaultAvatar->getContentSize().width *0.95);
		_imgAvatarBorder->addChild(avatar);
		avatar->setLocalZOrder(-1);
		avatar->setPosition(Vec2(_imgAvatarBorder->getContentSize().width / 2, _imgAvatarBorder->getContentSize().height / 2));
	}
	else {
		avatar->updateAvatar(player.avatar);
	}

	_crown->setVisible(isWinner);
	//TODO update ranking cup
	_lb_NameRank->setVisible(false);
}
