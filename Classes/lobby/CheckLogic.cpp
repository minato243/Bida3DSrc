#include "CheckLogic.h"
#include "core/gui/Dialog.h"
#include "core/gui/SceneMgr.h"
#include "core/GameData.h"
#include "ChanelConfig.h"

bool CheckLogic::checkQuickPlay()
{
	long long minGold = ChanelConfig::instance()->getMinBet() * ChanelConfig::instance()->betTime;
	return (gamedata->userData.bean >= minGold);
}
/*
void CheckLogic::onUpdateMoney(update)
{
	cocos2d::Node* curScene = sceneMgr->getRunningScene()->getMainLayer();
	if (BoardScene* board = dynamic_cast<BoardScene*>(curScene)) {
		//board.updateMoney(update);
		//if (gamedata->gameLogic) {
		//	gamedata->gameLogic->updateMoney(update);
		//	board.updateMoney(update);
		//}

	}
}
*/

void CheckLogic::showNotifyNetworkSlow()
{
	if (CheckLogic::checkInBoard()) {
		//js no implement
		/*cocos2d::Node* gui = sceneMgr->getMainLayer();
		if (gui)
			gui->networkSlow(isSlow);*/
	}
}

void CheckLogic::updatePing(float ping)
{
	if (CheckLogic::checkInBoard()) {
		//js no implement
		/*var gui = sceneMgr.getRunningScene().getMainLayer();
		if (gui) gui.updatePing(ping);*/
	}
}

std::string CheckLogic::getDialogClassName()
{
	return Dialog::className;
}

std::string CheckLogic::getUserInfoClassName()
{
	return "";
}

cocos2d::Vec2 CheckLogic::getPosFromPlayer(int id)
{

	return cocos2d::Vec2(0, 0);
}


std::string CheckLogic::getCardResource(int id)
{
	if (id < 0 || id > 52) {
		return "";
	}
	std::string _card =  cocos2d::StringUtils::format("cards/%d.png", id);
	return _card;
}


bool CheckLogic::checkInBoard()
{
	return false;
}

