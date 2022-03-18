#include "CheatItemUI.h"
#include "../../../data/Resource.h"
#include "../../core/inventory/ItemConstant.h"
#include "../../network/cheat/CheatHandler.h"
#include "../../../core/GameMgr.h"
#include "../../network/cue/CueHandler.h"
#include "../../core/inventory/InventoryMgr.h"

USING_NS_CC;
using namespace cocos2d::ui;
using namespace std;

CheatItemUI::CheatItemUI()
{

}

CheatItemUI::~CheatItemUI()
{

}

bool CheatItemUI::init()
{
	if (!BaseLayer::init("CheatItemUI"))
	{
		return false;
	}

	std::string filePath = Res::GUI::getCsbPathFromJson(Res::GUI::CheatInventoryItem);
	this->initWithBinaryFile(filePath);

	this->_keep = true;

	return true;
}

void CheatItemUI::initGUI()
{
	initTag();
	autoInitComponent(_layout);

	_tfType = (TextField*)_nodeMap["tfType"];
	_tfNum = (TextField*)_nodeMap["tfNum"];
}

void CheatItemUI::initTag()
{
	_tagMap.insert(pair<string, int>("btnSubmit", btnSubmit));
}

void CheatItemUI::onButtonRelease(cocos2d::ui::Button* button, int id)
{
	switch (id)
	{
	case TAG::btnSubmit:
	{
		if (getType() == ItemConstant::CASH)
		{
			auto num = getNum();
			cheatHandler->requestCheatCash(num);
			gameMgr->_userInfo->setCash(num);
			guiMgr->refreshAllGui();
		}

		else if (getType() == ItemConstant::GOLD)
		{
			auto num = getNum();
			cheatHandler->requestCheatGold(num);
			gameMgr->_userInfo->setGold(num);
			gameMgr->_lobbyUI->updateUserInfo(gameMgr->_userInfo);
		}
		else
		{
			int type = getType();
			int num = getNum();
			cueHandler->requestCheatInventoryItem(type, num);
			inventoryMgr->getInventory()->setItemQuantity(type, num);
		}

		guiMgr->refreshGui(Popup::LOBBY_UI);
		break;
	}
	default:
		break;
	}
}

int CheatItemUI::getType()
{
	string typeStr = _tfType->getString();
	int value = atoi(typeStr.c_str());
	return value;
}

int CheatItemUI::getNum()
{
	string numStr = _tfNum->getString();
	int value = atoi(numStr.c_str());
	return value;
}

