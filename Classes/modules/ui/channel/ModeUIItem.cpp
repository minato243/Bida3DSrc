#include "ModeUIItem.h"
#include "../../../core/utils/LanguageMgr.h"
#include "../../core/channel/ChannelMgr.h"
#include "../../../core/utils/StringUtility.h"
#include "core/GameMgr.h"

USING_NS_CC;
using namespace cocos2d::ui;
using namespace std;

ModeUIItem::ModeUIItem()
{
	CCLOG("ModeUIItem::ModeUIItem %x", this);
}

ModeUIItem::~ModeUIItem()
{

}

bool ModeUIItem::init()
{
	if (!UIBaseLayer::init())
		return false;

	initWithBinaryFile(Res::GUI::getCsbPathFromJson(Res::GUI::ModeItem));

	return true;
}

void ModeUIItem::initGUI()
{
	autoInitComponent(_layout);
	
	_bg_Channel		= (ImageView *)_nodeMap["bg_Channel"];
	_bg_board		= (ImageView *) _nodeMap["bg_board"];
	_bg_Icon_Mode	= (Sprite *) _nodeMap["bg_Icon_Mode"];
	_icon_Mode		= (Sprite *) _nodeMap["icon_Mode"];
	_lb_Gold_Reward = (Text *) _nodeMap["lb_Gold_Reward"];

	_nodeFee	= _nodeMap["nodeFee"];
	_lb_Fee		= (Text *) _nodeMap["lb_Fee"];
	_lb_Fee_0	= (Text *)_nodeMap["lb_Fee_0"];
	_img_NameChannel	= (Sprite *)_nodeMap["img_NameChannel"];
	_lb_MutiElo			= (Text *)_nodeMap["lb_MutiElo"];

	_imgLock	= (ImageView *)_nodeMap["imgLock"];
	_lb_LockLevel = (Text *)_nodeMap["lb_LockLevel"];
	_bg_Channel->setPosition(Vec2(0, 0));
	_bg_Channel->setColor(Color3B::WHITE);
}

void ModeUIItem::loadData(SelectModeData *data)
{
	_curMode = data->mode;
	_ballRule = data->rule;
	_channelId = data->id;
	CCLOG("ModeUIItem::loadData, %d, %d, %d", _curMode, _ballRule, _channelId);
	_bg_Channel->setScale(1);

	if(_curMode != NO_MODE)
		_icon_Mode->setSpriteFrame(ICON_MODE[_curMode -1]);
	_bg_Channel->loadTexture(BG_CHANNEL[_channelId], TextureResType::PLIST);
	_bg_board->loadTexture(BG_FEE[_channelId], TextureResType::PLIST);
	_img_NameChannel->setSpriteFrame(NAME_CHANNEL[_channelId]);
	_lb_MutiElo->setVisible(_curMode == GameMode::RT2_MODE || _curMode == GameMode::RT3_MODE);
	_lb_MutiElo->setString("x" + to_string(_channelId + 1) + " " + languageMgr->localizedText("text_ELO"));

	_imgLock->setVisible(false);
	_lb_LockLevel->setVisible(false);

	updateGiftFee();
	updateGiftReward();
}

void ModeUIItem::updateGiftFee()
{
	if (_curMode == GameMode::NO_MODE)
		return;
	auto listFee = channelMgr->getListFee(_curMode, _channelId+1);

	_lb_Fee_0->ignoreContentAdaptWithSize(true);
	Sprite *icon = (Sprite *)_nodeFee->getChildByName("icon");
	float width = _lb_Fee_0->getContentSize().width;

	if (_curMode == GameMode::STREAK_MODE && gameMgr->_userInfo->getLastS2MatchResult(_channelId)) {
		icon->setVisible(false);
		_lb_Fee->setVisible(false);
		_lb_Fee_0->setString(languageMgr->localizedText("text_Already_win"));
	}
	else {
		icon->setVisible(true);
		_lb_Fee->setVisible(true);
		_lb_Fee_0->setString(languageMgr->localizedText("text_Entry_fee"));
		_lb_Fee->ignoreContentAdaptWithSize(true);
		_lb_Fee->setString(StringUtility::formatNumberSymbol(listFee->num));
		width = _lb_Fee->getContentSize().width + 5 + _lb_Fee_0->getContentSize().width + 5 + icon->getContentSize().width * icon->getScale();
	}
	
	_lb_Fee_0->setPositionX(-width / 2);
	icon->setPositionX(_lb_Fee_0->getPositionX() + _lb_Fee_0->getContentSize().width + 5);
	_lb_Fee->setPositionX(icon->getPositionX() + icon->getContentSize().width * icon->getScale() + 5);
	
	CC_SAFE_DELETE(listFee);
}

void ModeUIItem::updateGiftReward()
{
	if (_curMode == GameMode::NO_MODE)
		return;
	auto listReward = channelMgr->getListItemWin(_curMode, _channelId+1);
	_lb_Gold_Reward->ignoreContentAdaptWithSize(true);
	_lb_Gold_Reward->setString("Prize: " + StringUtility::formatNumberSymbol(listReward[0].num));
	auto icon = _lb_Gold_Reward->getChildByName("icon");
	icon->setPositionX(_lb_Gold_Reward->getSize().width + 5);
	_lb_Gold_Reward->setPositionX(_lb_Gold_Reward->getParent()->getContentSize().width/2 -2.5 - 0.8 *icon->getContentSize().width/2);
}

cocos2d::Size ModeUIItem::getOriginalSize()
{
	return _bg_Channel->getContentSize();
}

void ModeUIItem::showOut()
{
	CCLOG("ModeUIItem::showOut %x", this);
	_bg_Channel->setScale(0);
}

void ModeUIItem::runShowOut(float delayTime)
{
	CCLOG("ModeUIItem::runShowOut %d", this);
	auto node = _bg_Channel;
	node->stopAllActions();
	node->setScale(1);
	auto action = Sequence::create(
		DelayTime::create(delayTime),
		EaseBackOut::create(CCScaleTo::create(0.2, 0)),
		NULL
	);
	node->runAction(action);

}

void ModeUIItem::runShowIn(float delayTime)
{
	CCLOG("ModeUIItem::runShowIn %d", this);
	auto node = _bg_Channel;
	node->stopAllActions();
	node->setScale(0);
	auto action = Sequence::create(
		DelayTime::create(delayTime),
		EaseBackIn::create(CCScaleTo::create(0.2, 1)),
		NULL
	);
	node->runAction(action);
}

const std::string ModeUIItem::ICON_MODE[5] = 
{
	"Channel/Raceto1_icon.png",
	"Channel/Streak2_icon.png",
	"Channel/Raceto2_icon.png",
	"Channel/Raceto3_icon.png",
	"Channel/Friendmode_icon.png"
};

const std::string ModeUIItem::NAME_MODE[5] =
{
	"Channel/Raceto1.png",
	"Channel/Streak2.png",
	"Channel/Raceto2.png",
	"Channel/Raceto3.png",
	"Channel/Friendmode.png"
};

const string ModeUIItem::BG_CHANNEL[NUM_CHANNEL] = 
{ 
	"Channel/Board_01.png",
	"Channel/Board_02.png",
	"Channel/Board_03.png",
	"Channel/Board_04.png",
	"Channel/Board_05.png"
};

const std::string ModeUIItem::BG_FEE[NUM_CHANNEL] = 
{
	"Channel/Reward_01.png",
	"Channel/Reward_02.png",
	"Channel/Reward_03.png",
	"Channel/Reward_04.png",
	"Channel/Reward_05.png"
};

const std::string ModeUIItem::NAME_CHANNEL[NUM_CHANNEL] = {
	"Channel/Tapsu.png",
	"Channel/SoCap.png",
	"Channel/TrungCap.png",
	"Channel/CaoCap.png",
	"Channel/ThuongCap.png",
};

const std::string ModeUIItem::RULE[2][5] = {
	{
		"Channel/Effect_01.png",
		"Channel/Effect_02.png",
		"Channel/Effect_03.png",
		"Channel/Effect_04.png",
	},
	{
		"Channel/Effect_01.png",
		"Channel/Effect_02.png",
		"Channel/Effect_03.png",
		"Channel/Effect_04.png",
	}
};

