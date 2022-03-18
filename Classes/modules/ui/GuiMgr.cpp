#include "GuiMgr.h"
#include <cocos2d.h>
#include <cocostudio/CocoStudio.h>
#include <core/utils/LanguageMgr.h>
#include <data/Resource.h>
#include <data/GameConstant.h>
#include <base/UIFactory.h>
#include <modules/camera-view/GameCamera.h>
#include "inventory/cue_collection/CueCollectionUIDetail.h"
#include "core/gui/SceneMgr.h"
#include "inventory/cue_collection/CueCollectionUI.h"
#include "inventory/InventoryUI.h"
#include "shop/ShopUI.h"
#include "shop/ShopPopupConfirm.h"
#include "shop/ShopPopupResult.h"
#include "gacha/CueGachaRollingList.h"
#include "global/CheatItemUI.h"
#include "global/DebugDialog.h"
#include "inventory/CueInspectUI.h"
#include "ingame/IngameStartMatchingUI.h"
#include "lobby/FindMatchLayer.h"
#include "lobby/ResultLayer.h"
#include "lobby/ResultGift.h"
#include "ingame/DisconnectPopup.h"
#include "lobby/RT3LobbyLayer.h"
#include "channel/SelectModeLayer.h"
#include "endgame/ResultGUILayer.h"
#include "ingame/IngameMidFX.h"

USING_NS_CC;
using namespace std;

GuiMgr::GuiMgr()
{
	initPopupsMap();
}

GuiMgr::~GuiMgr()
{
}

void GuiMgr::notifyAnnouncerIngame(AnnounceType announceType, std::string message, const AnnouncementCallBack & callback)
{
	// select notify sprite frame
	SpriteFrame* announceRes = NULL;
	SpriteFrame* bg = NULL;
	double readTime = 1;
	switch (announceType) {
		case AnnounceType::ANNOUNCE_FOUL:
			announceRes = SpriteFrameCache::getInstance()->getSpriteFrameByName("IngameEBP_FOUL_text.png");
			bg = SpriteFrameCache::getInstance()->getSpriteFrameByName("IngameEBP_FOUL_box.png");
			readTime = 1.7;
			break;
		case AnnounceType::ANNOUNCE_WIN:
			announceRes = SpriteFrameCache::getInstance()->getSpriteFrameByName("win.png");
			bg = SpriteFrameCache::getInstance()->getSpriteFrameByName("IngameEBP_FOUL_box.png");
			readTime = 1.7;
			break;
		case AnnounceType::ANNOUNCE_LOSE:
			announceRes = SpriteFrameCache::getInstance()->getSpriteFrameByName("lose.png");
			bg = SpriteFrameCache::getInstance()->getSpriteFrameByName("IngameEBP_FOUL_box.png");
			readTime = 1.7;
			break;
		case AnnounceType::ANNOUNCE_OPPONENT_TURN:
			announceRes = SpriteFrameCache::getInstance()->getSpriteFrameByName("opponent_turn.png");
			bg = SpriteFrameCache::getInstance()->getSpriteFrameByName("IngameEBP_OPPO_box.png");
			readTime = 1.7;
			break;
		case AnnounceType::ANNOUNCE_PLAYER_TURN:
			announceRes = SpriteFrameCache::getInstance()->getSpriteFrameByName("your_turn.png");
			bg = SpriteFrameCache::getInstance()->getSpriteFrameByName("IngameEBP_URTURN_box.png");
			readTime = 1.7;
			break;
		default:
			CCLOG("announce null!");
			if(callback != nullptr) callback();
			notifyDebugText(message.compare("") == 0 ? message : "announce null");
			return;
	}

	// create action
	auto winSize = Director::getInstance()->getVisibleSize();
	auto width = winSize.width, height = winSize.height;
	auto runningScene = Director::getInstance()->getRunningScene();
	auto efxLength = 4;
	for (auto i = 0; i <= efxLength; i++) {
		auto box = Sprite::createWithSpriteFrame(bg);
		box->setPosition(Vec2(width / 2, height / 2));
		box->setAnchorPoint(Vec2(0.5f, 0.5f));
		box->setScale(0.01);
		runningScene->addChild(box);

		auto s =  Sprite::createWithSpriteFrame(announceRes);
		s->setAnchorPoint(Vec2(0.5, 0.5));
		s->setPosition(width / 2, height / 2);
		s->setScale(0.01);
		runningScene->addChild(s);
		
		if (i > 0) {
			s->setOpacity(100 - 15 * i);
			box->setOpacity(100 - 15 * i);
		}

		auto move = EaseInOut::create(ScaleTo::create(0.3, 1), 2);
		auto delayToRead = DelayTime::create(readTime);
		auto moveOut = EaseInOut::create(ScaleTo::create(0.3, 0.01f), 2);

		auto action = Sequence::create(
			move,
			delayToRead,
			moveOut,
			CallFuncN::create(CC_CALLBACK_1(GuiMgr::removeAfterAction, this)),
			NULL
		);
		s->runAction(action);

		if (i == 0)
		{
			std::function<void(Node *)> callf = std::bind(&GuiMgr::removeAndCallBack, this, placeholders::_1, callback);
			box->runAction(Sequence::create(
				move->clone(),
				delayToRead->clone(),
				moveOut->clone(),
				CallFuncN::create(callf),
				NULL
			));
		}
		box->runAction(action->clone());
	}
}

void GuiMgr::notifyDebugText(std::string message, cocos2d::Vec2 position)
{
	if (position.isZero())
	{
		position.x = Director::getInstance()->getVisibleSize().width / 2;
		position.y = 20;
	}

	if (gv::ENABLE_SHOW_DEBUG_NOTIFY) 
	{
		notifyText(message, position);
	}
}

void GuiMgr::notifyText(std::string text, cocos2d::Vec2 position, std::string fontName, int fontSize, cocos2d::Color3B color)
{
	auto label = UIFactory::createLabelOutline(
		text,
		fontName,
		fontSize,
		color);
	if (position.isZero()) {
		position.x = Director::getInstance()->getVisibleSize().width / 2;
		position.y = 20;
	}
	label->setCameraMask((int)GameCamera::FLAG_UI_ALWAYS_TOP);
	label->setTextHorizontalAlignment(TextHAlignment::CENTER);
	label->setPosition(position);
	Director::getInstance()->getRunningScene()->addChild(label);
	label->setLocalZOrder(100000);
	label->setVisible(false);
	//effect
	auto move = MoveTo::create(1.5, Vec2(position.x, position.y + 10));
	auto fadeOut = FadeTo::create(2.5, 150);
	auto spawnAct = Spawn::create(Show::create(), move, fadeOut, nullptr);
	_notifies.push_back(label);
	auto action = Sequence::create(spawnAct, CallFuncN::create(CC_CALLBACK_1(GuiMgr::notifyTextDone, this)), nullptr);
	action->retain();
	_actions.push_back(action);

	if (_notifies.size()== 1) {
		label->runAction(action);
		_actions.at(0)->release();
	}
}

void GuiMgr::notifyTextDone(cocos2d::Node *label)
{
	label->removeFromParent();
	if (_notifies.size() > 0) {
		_notifies.erase(_notifies.begin());
		_actions.erase(_actions.begin());
		if (_notifies.size() > 0) {
			_notifies.at(0)->runAction(_actions.at(0));
			_actions.at(0)->release();
		}
			
	}
}

void GuiMgr::removeAfterAction(cocos2d::Node * node)
{
	node->removeFromParent();
}

void GuiMgr::removeAndCallBack(cocos2d::Node * node, const AnnouncementCallBack & callback)
{
	node->removeFromParent();
	callback();
}

BaseLayer * GuiMgr::getGui(Popup tag)
{
	_curSceneTag = sceneMgr->_curSceneTag;
	if (_popups[_curSceneTag].find(tag) != _popups[_curSceneTag].end() 
		&& _popups[_curSceneTag][tag] != NULL)
		return _popups[_curSceneTag][tag];

	return createPopup(tag);
}

BaseLayer * GuiMgr::createPopup(Popup tag)
{
	BaseLayer *popup = nullptr;
	switch (tag)
	{
		case Popup::INVENTORY_UI:
			popup = InventoryUI::create();
		break;

		case Popup::CUE_COLLECTION_UI:
			popup = CueCollectionUI::create();
			break;
		case Popup::Cue_COLLECTION_DETAIL:
			popup = CueCollectionUIDetail::create();
			break;
		case Popup::SHOP_UI:
			popup = ShopUI::create();
			break;
		case Popup::POPUP_CONFIRM:
			popup = ShopPopupConfirm::create();
			break;
		case Popup::POPUP_RESULT:
			popup = ShopPopupResult::create();
			break;

		case Popup::GACHA_ROLLING:
			popup = CueGachaRollingList::create();
			break;
		case Popup::DEBUG_DIALOG:
			popup = DebugDialog::create();
			break;
		case Popup::CUE_INSPECT_UI:
			popup = CueInspectUI::create();
			break;

		case Popup::FIND_MATCH_LAYER:
			popup = FindMatchLayer::create();
			break;

		case Popup::RT3_FIND_MATCH_LAYER:
			popup = RT3LobbyLayer::create();
			break;

		case Popup::INGAME_START_MATCHING_UI:
			popup = IngameStartMatchingUI::create();
			break;

		case Popup::MESSAGE_POUP:
			popup = MessagePoup::create();
			break;

		case Popup::RESULT_LAYER:
			popup = ResultLayer::create();
			break;

		case Popup::RESULT_GIFT_LAYER:
			popup = ResultGUILayer::create();
			break;

		case Popup::DISCONNECT_POPUP:
			popup = DisconnectPopup::create();
			break;

		case Popup::SELECT_MODE_POPUP:
			popup = SelectModeLayer::create();
			break;

		case Popup::INGAME_MID_GAME_FX:
			popup = IngameMidFX::create();
			break;
	default:
		break;
	}

	if (popup) 
	{
		popup->setTag(tag);
		_popups[_curSceneTag].insert(std::pair<Popup, BaseLayer*>(tag, popup));
	}
	return popup;
}

void GuiMgr::show(BaseLayer *popup)
{
	_curSceneTag = sceneMgr->_curSceneTag;
	if (popup != nullptr)
	{
		if (!popup->getParent())
		{
			getCurScene()->addChild(popup);
			popup->setLocalZOrder(_popupZorder[_curSceneTag]);
			popup->setVisible(true);

			if (_popupZorder.find(_curSceneTag) != _popupZorder.end())
			{
				_popupZorder[_curSceneTag]++;
			}
		}
	}
}

void GuiMgr::hide(BaseLayer *popup, bool keepPopup, int tag)
{
	_curSceneTag = sceneMgr->_curSceneTag;
	Popup tagPopup = (Popup)tag;
	if (popup) {
		popup->setVisible(false);
		if (!keepPopup) {
			popup->removeFromParent();
			_popups[_curSceneTag][tagPopup] = nullptr;
		}
		
	}
	//cc.log('popups[tag]', popups[tag]);
}

cocos2d::Scene * GuiMgr::getCurScene()
{
	return sceneMgr->getRunningScene();
}

void GuiMgr::initMapsForScenes(int tag)
{
	map<Popup, BaseLayer *> p;
	vector<BaseLayer*> v;
	_popups.insert(pair<int, map<Popup, BaseLayer*>>(tag, p));
	_popupZorder.insert( pair<int, int>(tag, 101));
	_popupStack.insert(pair<int, vector<BaseLayer*>>(tag, v));
}

void GuiMgr::initPopupsMap()
{
	initMapsForScenes(SceneTag::MAIN_SCENE);
	initMapsForScenes(SceneTag::INSPECT_SCENE);
	initMapsForScenes(SceneTag::SHOP_SCENE);
	initMapsForScenes(SceneTag::SPONSORSHIP_CLAIM);
	initMapsForScenes(SceneTag::WORKSHOP_SCENE);
	initMapsForScenes(SceneTag::ACHIEVEMENT_SCENE);
	initMapsForScenes(SceneTag::LOADING_SCENE);
}

void GuiMgr::refreshGui(Popup tag)
{
	for (auto ittr = _popups.begin(); ittr != _popups.end(); ittr++)
	{
		auto curMap = ittr->second;
		if (curMap.find(tag) != curMap.end())
		{
			curMap[tag]->refresh();
		}
	}
}

void GuiMgr::refreshAllGui()
{
	refreshGui(Popup::LOBBY_UI);
	refreshGui(Popup::SHOP_UI);
	refreshGui(Popup::INVENTORY_UI);
	refreshGui(Popup::GACHA_ROLLING);
	refreshGui(Popup::PROFILE_GUI);
}

void GuiMgr::showMessagePopup(const std::string &message, const MessagePoup::PopupListener & callback, std::string okText /*= "Ok"*/)
{
	auto popup = (MessagePoup*)getGui(Popup::MESSAGE_POUP);
	popup->setOkDialog(message, callback);
	popup->setOkBtnText(okText);
	popup->show();
}

void GuiMgr::showMessagePopup(const std::string &title, const std::string &message, const MessagePoup::PopupListener & callback, std::string okText /*= "Ok"*/)
{
	auto popup = (MessagePoup*)getGui(Popup::MESSAGE_POUP);
	popup->setOkDialog(title, message, callback);
	popup->setOkBtnText(okText);
	popup->show();
}

void GuiMgr::showOkCancelPopup(const std::string &message, const MessagePoup::PopupListener & callback, std::string okText/*="Ok"*/, std::string cancelText /*= "Cancel"*/)
{
	auto popup = (MessagePoup*)getGui(Popup::MESSAGE_POUP);
	popup->setOkCancelDialog(message, callback);
	popup->setBtnText(okText, cancelText);
	popup->show();
}

void GuiMgr::showOkCancelPopup(const std::string &title, const std::string &message, const MessagePoup::PopupListener & callback, std::string okText /*= "Ok"*/, std::string cancelText /*= "Cancel"*/)
{
	auto popup = (MessagePoup*)getGui(Popup::MESSAGE_POUP);
	popup->setOkCancelDialog(title, message, callback);
	popup->setBtnText(okText, cancelText);
	popup->show();
}

GuiMgr *GuiMgr::_instance = nullptr;
GuiMgr * GuiMgr::getInstance()
{
	if (_instance == nullptr) {
		_instance = new GuiMgr();
	}
	return _instance;
}
