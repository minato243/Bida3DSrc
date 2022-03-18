#include "ResultLayer.h"
#include "data/Resource.h"
#include "../../core/channel/ChannelMgr.h"
#include "ResultGift.h"
#include "../GuiMgr.h"

USING_NS_CC;
using namespace cocos2d::ui;
using namespace std;

ResultLayer::ResultLayer()
{
	_keep = false;
}

ResultLayer::~ResultLayer()
{
}

bool ResultLayer::init()
{
	if (!BasePopupLayer::init("ResultLayer"))
	{
		return false;
	}

	string filePath = Res::GUI::getCsbPathFromJson(Res::GUI::ResultGUI);
	this->initWithBinaryFile(filePath);

	return true;
}

void ResultLayer::initGUI()
{
	autoInitComponent(_layout);
	auto bg = (Widget *)_nodeMap["bg"];
	auto panelShowWinLose = (Widget *)_nodeMap["panelShowWinLose"];
	bg->setTouchEnabled(false);
	panelShowWinLose->setTouchEnabled(false);

	_nodeP0 = _nodeMap["nodeP0"];
	_nodeP1 = _nodeMap["nodeP1"];
	_imgWinLose = (Sprite*)_nodeMap["image_WinLose"];
}

void ResultLayer::setPlayerWinInfo(int winnerIdx)
{
	std::string spriteFrameName = "";
	if (winnerIdx == 0)
		spriteFrameName = "EndGame_YOUWIN.png";
	else
		spriteFrameName = "EndGame_YOULOSE.png";
	_imgWinLose->setSpriteFrame(spriteFrameName);
}

void ResultLayer::onEnter()
{
	BasePopupLayer::onEnter();
	scheduleOnce(schedule_selector(ResultLayer::showResultGift), 1.5);
}

void ResultLayer::setMode(GameMode mode, int matchMode)
{
	switch (mode)
	{
	case RT1:
		if (_result.winnerIdx == 0) 
		{
			rewards = channelMgr->getListItemWin(mode, matchMode);
		}
		else 
		{
			rewards.clear();
		}
		break;
	case STREAK_MODE:
		break;
	case RT2_MODE:
		break;
	case RT3_MODE:
		break;
	default:
		break;
	}
}

void ResultLayer::setDataResult(const ResultData & data)
{
	_result.gameMode = data.gameMode;
	_result.matchMode = data.matchMode;
	_result.winnerIdx = data.winnerIdx;

	setPlayerWinInfo(data.winnerIdx);
	setMode(data.gameMode, data.matchMode);
}

void ResultLayer::showResultGift(float dt)
{
	ResultGift * giftGui = (ResultGift *)guiMgr->getGui(Popup::RESULT_GIFT_LAYER);
	giftGui->show();
	hide();
}
