#include "FindMatchLayer.h"
#include <data/Resource.h>
#include <base/UIFactory.h>
#include <core/utils/LanguageMgr.h>
#include "../../network/match/MatchHandler.h"
#include "core/gui/SceneMgr.h"
#include "../../scene/GameScene.h"
#include "../../utils/SoundMgr.h"
#include "../../network/cheat/CheatHandler.h"

USING_NS_CC;
using namespace cocos2d::ui;

FindMatchLayer::FindMatchLayer()
{
	_timeElapsed = -1;
	_keep = true;
	_numDot = 0;
}

FindMatchLayer::~FindMatchLayer()
{
}

bool FindMatchLayer::init()
{
	if (!BaseLayer::init())
	{
		return false;
	}

	initWithBinaryFile(Res::GUI::getCsbPathFromJson(Res::GUI::FindMatch));

	return true;
}

void FindMatchLayer::initGUI()
{
	autoInitComponent(_layout);
	_bg = (ImageView *)getControl("bg", _layout);

	_nodeP1 = getControl("nodeP1", _bg);
	_nodeP2 = getControl("nodeP2", _bg);

	_btnCancel = customButton("btnCancel", TAG::BTN_CANCEL, _bg);
	
	_lbMatching = (Text *)getControl("lbMatching", _bg);
	_lbTime = (Text *)getControl("lbTime", _bg);
	_lbEstimate = (Text *)getControl("lbEstimate", _bg);
	_lbCCU = (Text *)getControl("lbCCU", _bg);
	_lbSubtitle = (Text *)getControl("lbSubtitle", _bg);
	_lbBtnCancel = (Text *)getControl("lb", _btnCancel);

	_lbBtnCancel->setString(languageMgr->localizedText("text_cancel_matching"));
	_lbMatching->setString(languageMgr->localizedText("text_matching"));

	_iconCircle_0 = (cocos2d::Sprite *) _nodeMap["iconCircle_0"];
	_iconCircle_1 = (cocos2d::Sprite *) _nodeMap["iconCircle_1"];
}

void FindMatchLayer::onEnter()
{
	BaseLayer::onEnter();
}

void FindMatchLayer::startTimer()
{
	_timeElapsed = 0;
	schedule(schedule_selector(FindMatchLayer::updateTimer), 0.5f);
}

void FindMatchLayer::stopTimer()
{
	_timeElapsed = -1;
	unschedule(schedule_selector(FindMatchLayer::updateTimer));
}

void FindMatchLayer::updateTimer(float dt)
{
	_timeElapsed += dt;
	auto h = (int)(_timeElapsed / 3600);
	auto m = (int)((_timeElapsed - 3600 * h) / 60);
	auto s = (int)(_timeElapsed - 3600 * h - 60 * m);

	char hText[5], mText[5], sText[5];
	if (h == 0) sprintf(hText, "");
	else if (h < 10) sprintf(hText, "0%d:", h);
	else sprintf(hText, "%d:", h);

	if (m < 10) sprintf(mText, "0%d:", m);
	else sprintf(mText, "%d:", m);
	
	if (s < 10) sprintf(sText, "0%d", s);
	else sprintf(sText, "%d", s);

	char timeText[20];
	sprintf(timeText, "%s%s%s", hText, mText, sText);
	//TODO: string formatter
	_lbTime->setString(timeText);

	_numDot ++;
	if (_numDot > 3) _numDot = 0;

	std::string text = "";
	for (int i = 0; i < _numDot; i++)
	{
		text += ".";
	}
	_lbMatching->setString(languageMgr->localizedText("text_matching") + text);

	if (_timeElapsed >= AUTO_FIND_BOT_TIME && _timeElapsed - dt < AUTO_FIND_BOT_TIME) {
		matchHandler->requestCancelMatching();
		scheduleOnce(schedule_selector(FindMatchLayer::playWithBot), 0.1f);
	}
}

void FindMatchLayer::onFoundOpponent(const Player & p2Data)
{
	_foundOpponent = true;
	stopTimer();
}

void FindMatchLayer::setInfo(GameMode mode, int ccu)
{
	setMode(mode);
	setCCU(ccu);
}

void FindMatchLayer::setCCU(int ccu)
{
	if (ccu == -1)
		_lbCCU->setString("CCU: ?");
	else
	{
		char text[10];
		sprintf(text, "CCU: %d", ccu);
		_lbCCU->setString(text);
	}
}

void FindMatchLayer::onButtonRelease(cocos2d::ui::Button * button, int id)
{
	CCLOG("FindMatchLayer::onButtonRelease");
	switch (id)
	{
	case TAG::BTN_CANCEL:
	{
		matchHandler->requestCancelMatching();
		stopTimer();
		hide();
		auto gameScene = (GameScene *)sceneMgr->getRunningScene();
		gameScene->_lobbyUI->show();
		break;
	}
	default:
		break;
	}
}

void FindMatchLayer::reset()
{
	startTimer();
	_foundOpponent = false;
	std::vector<std::string> imgList = std::vector<std::string>{ 
			Res::INGAME_AVATAR_FAKE_1, 
			Res::INGAME_AVATAR_FAKE_2, 
			Res::INGAME_AVATAR_FAKE_1, 
			Res::INGAME_AVATAR_FAKE_2 
		};
}

void FindMatchLayer::show()
{
	BaseLayer::show();

	_bg->setOpacity(0);
	_bg->runAction(FadeIn::create(0.5));

	_iconCircle_0->stopAllActions();
	_iconCircle_1->stopAllActions();

	_iconCircle_0->runAction(RepeatForever::create(RotateBy::create(2,30)));
	_iconCircle_1->runAction(RepeatForever::create(RotateBy::create(2,-30)));

	soundMgr->playBgSearchingOpponent();
}

void FindMatchLayer::setMode(GameMode mode)
{
	_gameMode = mode;
	std::string modeName = "";
	char text[20];
	sprintf(text, "text_Mode_%d", mode);
	modeName = languageMgr->localizedText(text);
	_lbSubtitle->setString(modeName);
}

void FindMatchLayer::playWithBot(float dt)
{
	cheatHandler->requestCheatPlayWithBot(_gameMode, 0.85f);
}

const float FindMatchLayer::AUTO_FIND_BOT_TIME = 10.f;

