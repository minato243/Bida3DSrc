#include "SelectModeLayer.h"
#include "../../../data/Resource.h"
#include "../elements/QuickCustomTableView.h"
#include "ModeUIItem.h"
#include "../../../core/utils/LanguageMgr.h"
#include "../../../data/DataStruct.h"
#include "../GuiMgr.h"
#include "../../core/channel/ChannelMgr.h"
#include "../../../core/GameMgr.h"
#include "core/utils/StringUtility.h"
#include "../../scene/GameScene.h"
#include "core/gui/SceneMgr.h"

USING_NS_CC;
using namespace cocos2d::ui;
using namespace std;

SelectModeLayer::SelectModeLayer()
{
	_lastMode = NO_MODE;
	_curMode = NO_MODE;
	_ballRule = BALL_8_RULE;
}

SelectModeLayer::~SelectModeLayer()
{

}

bool SelectModeLayer::init()
{
	if (!UIBaseLayer::init())
		return false;

	initWithBinaryFile(Res::GUI::getCsbPathFromJson(Res::GUI::ModeSelect));
	textIgnore(_layout);

	initChannel();

	_listBtnText[0] = _text_Quick;
	_listBtnText[1] = _text_Streak;
	_listBtnText[2] = _text_RT2;
	_listBtnText[3] = _text_RT3;
	_listBtnText[4] = _text_FriendMode;
	_listBtnText[5] = _text_Practise;

	int numBtn = 6;
	for (auto i = 0; i < numBtn; i++)
	{
		_listBtnText[i]->setTouchEnabled(true);
		_listBtnText[i]->addClickEventListener(CC_CALLBACK_1(SelectModeLayer::onBtnTextClick, this));
	}

	auto defaultMode = GameMode::QUICK_MODE;

	_curMode = defaultMode;
	showSelectText(defaultMode);
	showModeData(defaultMode);
	setBgFoot(defaultMode);

	return true;
}

void SelectModeLayer::initGUI()
{
	initTag();
	autoInitComponent(_layout);
	_bg = _nodeMap["bg"];

	_lb_Mode_Detail = (Text *)_nodeMap["lb_Mode_Detail"];
	_text_Quick = (Text*)_nodeMap["text_Quick"];
	_text_Streak = (Text*)_nodeMap["text_Streak"];
	_text_RT2 = (Text*)_nodeMap["text_RT2"];
	_text_RT3 = (Text*)_nodeMap["text_RT3"];
	_text_FriendMode = (Text*)_nodeMap["text_FriendMode"];
	_text_Practise = (Text*)_nodeMap["text_Practise"];

	_panelFoot = (Widget *)_nodeMap["panelFoot"];
	_bgChoose = (Sprite *)_nodeMap["bgChoose"];

	_topPanel = (Widget *)_nodeMap["topPanel"];
	_btnBack = (Button *)_nodeMap["btnBack"];
	_lbGold = (Text *)_nodeMap["lbGold"];
	_lbCash = (Text *)_nodeMap["lbCash"];
	char text[10];
	for (auto i = 0; i < NUM_BTN_TEXT - 1; i++)
	{
		sprintf(text, "line_%d", i);
		_listLine[i] = (cocos2d::Sprite *) getControl(text, _panelFoot);
	}

	_text_Quick->setTag(GameMode::QUICK_MODE);
	_text_Streak->setTag(GameMode::STREAK_MODE);
	_text_RT2->setTag(GameMode::RT2_MODE);
	_text_RT3->setTag(GameMode::RT3_MODE);
	_text_FriendMode->setTag(GameMode::FRIEND_MODE);
	_text_Practise->setTag(GameMode::PRACTISE_MODE);

	
}

void SelectModeLayer::initTag()
{
	_tagMap["btnBack"] = TAG::btnBack;
	_tagMap["btn_Quick"] = GameMode::QUICK_MODE;
	_tagMap["btn_Streak"] = GameMode::STREAK_MODE;
	_tagMap["btn_RT2"] = GameMode::RT2_MODE;
	_tagMap["btn_RT3"] = GameMode::RT3_MODE;
	_tagMap["btn_FriendMode"] = GameMode::FRIEND_MODE;
	_tagMap["btn_Practise"] = GameMode::PRACTISE_MODE;
}

void SelectModeLayer::onButtonRelease(cocos2d::ui::Button* button, int id)
{
	CCLOG("SelectModeLayer::onButtonRelease %x", button);
	int tag = button->getTag();
	if (tag == TAG::btnBack) {
		hide();
		auto gameScene = (GameScene *)sceneMgr->getRunningScene();
		gameScene->_lobbyUI->setVisible(true);
		return;
	}
	auto selectIdx = (GameMode)tag;

	if (selectIdx == FRIEND_MODE || selectIdx == PRACTISE_MODE)
	{
		guiMgr->showMessagePopup("Coming soon", NULL);
		return;
	}

	if (selectIdx != _curMode)
	{
		_curMode = selectIdx;
		showSelectText(selectIdx);
		showModeData(selectIdx);
		setBgFoot(selectIdx);
	}
}

void SelectModeLayer::initChannel()
{
	auto tableView = new QuickCustomTableView<SelectModeData, ModeUIItem>();
	auto size = Size(_bg->getContentSize().width, 410);
	tableView->init(size, extension::ScrollView::Direction::HORIZONTAL, -1, 1);
	tableView->setTableViewTouchCallback(CC_CALLBACK_1(SelectModeLayer::tableCellTouched, this));
	_bg->addChild(tableView);

	resetData();
	tableView->reloadData(_selectModeList);

	_tableView = tableView;
}

void SelectModeLayer::onBtnTextClick(cocos2d::Ref *btn)
{
	CCLOG("SelectModeLayer::onBtnTextClick %x", btn);
	auto text = (Text*)btn;
	int tag = text->getTag();
	auto selectIdx = (GameMode)tag;
	
	if (selectIdx == FRIEND_MODE || selectIdx == PRACTISE_MODE)
	{
		guiMgr->showMessagePopup("Coming soon", NULL);
		return;
	}

	if (selectIdx != _curMode)
	{
		_curMode = selectIdx;
		showSelectText(selectIdx);
		showModeData(selectIdx);
		setBgFoot(selectIdx);
	}

}

void SelectModeLayer::showSelectText(GameMode mode)
{
	auto width = _panelFoot->getContentSize().width;
	auto dWidth = width / 6.5;
	float totalWidth = 0;

	for (auto i = 0; i < NUM_BTN_TEXT; i++) {
		float d = 1.f;
		if (i == mode - 1) {
			d = 1.5;
		}
		auto c = _listBtnText[i];

		auto cWidth = d * dWidth;

		c->stopAllActions();
		_dx[c] = totalWidth + cWidth / 2;
		auto pos = Vec2(_dx[c], c->getPositionY());
		c->runAction(MoveTo::create(0.2, pos));
		totalWidth += cWidth;
	}

	for (auto i = 1; i < NUM_BTN_TEXT; i++) {
		auto c0 = _listBtnText[i];
		auto c1 = _listBtnText[i - 1];

		if (i == mode || i + 1 == mode)
			_listLine[i -1]->setVisible(false);
		else _listLine[i - 1]->setVisible(true);
		_listLine[i - 1]->setPositionX(_dx[c0] / 2 + _dx[c1] / 2);
	}
	_bgChoose->stopAllActions();

	auto bgPos = Vec2(_dx[_listBtnText[mode -1]], _bgChoose->getPositionY());
	_bgChoose->runAction(MoveTo::create(0.2, bgPos));
}

void SelectModeLayer::setBgFoot(GameMode mode)
{
	for (auto i = 0; i < NUM_BTN_TEXT; i++)
	{
		if (i == mode - 1)
		{
			_listBtnText[i]->runAction(EaseBackInOut::create(ScaleTo::create(0.1, 1)));
			_listBtnText[i]->setTextColor(Color4B(0x54, 0xf8, 0xfa, 255));
		}
		else 
		{
			_listBtnText[i]->runAction(EaseBackInOut::create(ScaleTo::create(0.1, 0.8)));
			_listBtnText[i]->setTextColor(Color4B(0x29, 0x68, 0xa1, 255));
		}
	}
}

void SelectModeLayer::showModeData(GameMode mode)
{
	char text[20];
	sprintf(text, "text_Mode_Detail_%d", mode);
	_lb_Mode_Detail->setString(languageMgr->localizedText(text));

	_lb_Mode_Detail->setScale(0);
	_lb_Mode_Detail->runAction(EaseBackOut::create(ScaleTo::create(0.2, 1)));
	float timeDelay = 0;

	auto tableView = dynamic_cast<QuickCustomTableView<SelectModeData, ModeUIItem> *>(_tableView);
	if (_lastMode == NO_MODE)
	{
		for (auto i = 0; i < NUMBER_CHANNEL; i++)
		{
			auto cell = tableView->getCellAtIndex(i);
			if (cell)
			{
				auto item = cell->getChildByTag(TAG_COMMON);
				auto modeUI = dynamic_cast<ModeUIItem*>(item);
				modeUI->showOut();
			}
		}
	}
	else
	{
		for (auto i = 0; i < NUMBER_CHANNEL; i++)
		{
			auto cell = tableView->getCellAtIndex(i);
			if (cell)
			{
				auto item = cell->getChildByTag(TAG_COMMON);
				auto modeUI = dynamic_cast<ModeUIItem*>(item);
				modeUI->runShowOut(timeDelay);
				timeDelay += 0.1;
			}
		}
	}
	
	runAction(
		Sequence::create(
			DelayTime::create(0.2 + timeDelay),
			CallFunc::create(CC_CALLBACK_0(SelectModeLayer::runTableAppear, this)),
			NULL
		)
	);
	_lastMode = _curMode;
}

void SelectModeLayer::runTableAppear()
{
	float timeDelay;
	auto tableView = dynamic_cast<QuickCustomTableView<SelectModeData, ModeUIItem> *>(_tableView);
	if (_curMode != PRACTISE_MODE) 
	{
		// Normal mode
		this->resetData();
		tableView->reloadData(_selectModeList);
		timeDelay = 0;
		for (auto i = 0; i < SelectModeLayer::NUMBER_CHANNEL; i++) {
			auto cell = tableView->getCellAtIndex(i);
			if (cell) {
				auto item = cell->getChildByTag(TAG_COMMON);
				auto modeUI = dynamic_cast<ModeUIItem*>(item);
				modeUI->runShowIn(timeDelay);
				timeDelay += 0.1;
			}
		}
	}
	else {
		//TODO
		
	}
}

void SelectModeLayer::resetData()
{
	deleteVector(_selectModeList);
	_selectModeList.clear();
	for (int i = 0; i < NUMBER_CHANNEL; i++)
	{
		SelectModeData *item = new SelectModeData(_curMode, _ballRule, i);
		_selectModeList.push_back(item);
	}
}

void SelectModeLayer::tableCellTouched(cocos2d::extension::TableViewCell *cell)
{
	auto item = cell->getChildByTag(TAG_COMMON);
	auto modeUI = dynamic_cast<ModeUIItem*>(item);
	if (modeUI)
	{
		joinChannel((GameMode)modeUI->_curMode, modeUI->_channelId);
	}
}

void SelectModeLayer::joinChannel(GameMode mode, int channelId)
{
	if (channelMgr->joinChannel(mode, channelId + 1))
	{
		gameMgr->_ebpCtrl->gotoMode(mode, true, channelId + 1);
		hide();
	}
	else
	{
		guiMgr->showMessagePopup(languageMgr->localizedText("NOT ENOUGH FEE"), NULL, languageMgr->localizedText("text_agree"));
	}
}

void SelectModeLayer::show()
{
	UIBaseLayer::show();

	_panelFoot->setPositionY(-_panelFoot->getContentSize().height);
	_panelFoot->runAction(MoveTo::create(0.2, _defaultPos[_panelFoot]));

	_lb_Mode_Detail->setString(languageMgr->localizedText("text_Mode_Detail_"+ to_string(_curMode)));
	updateUserInfo();
}

void SelectModeLayer::hide()
{
	Vec2 pos = Vec2(_defaultPos[_panelFoot].x, -_panelFoot->getSize().height);
	_panelFoot->runAction(MoveTo::create(0.2, pos));
	_lb_Mode_Detail->runAction(ScaleTo::create(0.2, 0));
	UIBaseLayer::hide();
}

void SelectModeLayer::updateUserInfo()
{
	User *user = gameMgr->_userInfo;

	_lbGold->setString(StringUtility::standartNumber(user->getGold()));
	_lbCash->setString(StringUtility::standartNumber(user->getCash()));
}

