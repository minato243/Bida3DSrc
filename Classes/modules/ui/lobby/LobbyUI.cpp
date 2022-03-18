#include "LobbyUI.h"
#include <data/Resource.h>
#include <core/GameMgr.h>
#include <data/GameConstant.h>
#include "core/gui/SceneMgr.h"
#include "../ingame/IngameStartMatchingUI.h"
#include "../../scene/GameScene.h"
#include "base/CCDirector.h"
#include "../../network/cheat/CheatHandler.h"
#include "ResultLayer.h"
#include "../global/DebugDialog.h"
#include "../endgame/ResultGUILayer.h"
#include "../ingame/IngameMidFX.h"
#include "../global/DebugNode.h"
#include "core/utils/StringUtility.h"
#include "../../utils/SoundMgr.h"

USING_NS_CC;
using namespace cocos2d::ui;

LobbyUI::LobbyUI()
{
}

LobbyUI::~LobbyUI()
{
}

bool LobbyUI::init()
{
	if (!BaseLayer::init("LobbyUI"))
	{
		return false;
	}

	std::string filePath = Res::GUI::getCsbPathFromJson(Res::GUI::Lobby_UI);
	this->initWithBinaryFile(filePath);

	auto debugNode = DebugNode::create();
	_bg->addChild(debugNode);

	return true;
}

void LobbyUI::initGUI()
{
	initTag();
	autoInitComponent(_layout);
	_bg = (ImageView *)getControl("bg", _layout);

	_playPanel = (Widget*)getControl("playPanel", _bg);
	_eventPanel = (Widget*)getControl("eventPanel", _bg);
	
	_btnInventory = (Button *)_nodeMap["btnInventory"];
	_btnShop = (Button *)_nodeMap["btnShop"];

	_eventPanel->setVisible(false);

	_btnCueCollection = customButton("btnCueCollection", TAG::CUE_COLLECTION, _bg);
	_btnSetting = customButton("btnSetting", TAG::SETTING, _bg);

	_btnPlay = customButton("btn8BallRule", TAG::PLAY_NOW, _playPanel);
	_btnSolve = customButton("btnPlay9Ball", TAG::btnPlay9Ball, _playPanel);

	//user info
	_lbName = (Text *)_nodeMap["lbName"];
	_lbClub = (Text *)_nodeMap["lbClub"];
	_lbGold = (Text *)_nodeMap["lbGold"];
	_lbCash = (Text *)_nodeMap["lbCash"];
	_lbLevel = (Text *)_nodeMap["lbLevel"];
	_ldBar = (LoadingBar *)_nodeMap["ldBar"];

}

void LobbyUI::initTag()
{
	_tagMap["btnPlay9Ball"] = TAG::btnPlay9Ball;
}

void LobbyUI::updateUserInfo(User *user)
{
	_lbName->setString(user->_uName);
	_lbClub->setString("");
	_lbGold->setString(StringUtility::standartNumber(user->getGold()));
	_lbCash->setString(StringUtility::standartNumber(user->getCash()));

	_lbLevel->setString(StringUtility::standartNumber(user->_level));
}

void LobbyUI::onButtonRelease(cocos2d::ui::Button* button, int id)
{
	switch (id) {
	case TAG::PLAY_NOW:
		playNow();
		break;

	case TAG::INVENTORY:
	{
		auto inventory = guiMgr->getGui(Popup::INVENTORY_UI);
		inventory->show();
		break;
	}
	case TAG::SHOP:
	{
		sceneMgr->loadSceneByTag(SHOP_SCENE);
		break;
	}
	case TAG::CUE_COLLECTION:
	{
		auto cueCollection = guiMgr->getGui(Popup::CUE_COLLECTION_UI);
		cueCollection->show();
		this->setVisible(false);
		break;
	}
	case TAG::SETTING:
	{
		/*auto rt3Gui = guiMgr->getGui(Popup::RT3_FIND_MATCH_LAYER);
		rt3Gui->show();*/
		//guiMgr->getGui(Popup::DEBUG_DIALOG)->show();
		auto resultGui = (ResultGUILayer *)guiMgr->getGui(Popup::RESULT_GIFT_LAYER);
		resultGui->loadDataRT3(1, 1,1,1);
		resultGui->fakeRT3UserData();
		resultGui->show();
		//auto midgameFx = (IngameMidFX *)guiMgr->getGui(Popup::INGAME_MID_GAME_FX);
		//midgameFx->show();
		//midgameFx->runFX(2, 1, 0);
		break;
	}
		
	case TAG::btnPlay9Ball:
	{
		setVisible(false);
		auto selectModeGui = guiMgr->getGui(Popup::SELECT_MODE_POPUP);
		selectModeGui->show();
		break;
	}
	}
}

void LobbyUI::playNow()
{
	//gameMgr->_ebpCtrl->gotoMode(GameMode::RT1, true);
	gameMgr->_ebpCtrl->playWithBot(GameMode::QUICK_MODE, 0.85);
}

void LobbyUI::show()
{
	BaseLayer::show();
	soundMgr->playBgLobby();
}
