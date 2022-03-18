#include "ResultGift.h"
#include "../../../data/Resource.h"
#include "../../../core/GameMgr.h"
#include "../../scene/GameScene.h"

ResultGift::ResultGift()
{
	_keep = false;
}

ResultGift::~ResultGift()
{

}

bool ResultGift::init()
{
	if (!BasePopupLayer::init("ResultGift"))
	{
		return false;
	}

	std::string filePath = Res::GUI::getCsbPathFromJson(Res::GUI::ResultGiftGUI);
	this->initWithBinaryFile(filePath);

	return true;
}

void ResultGift::initGUI()
{
	initTag();
	autoInitComponent(_layout);
}

void ResultGift::initTag()
{
	_tagMap["btnLobby"] = btnLobby;
	_tagMap["btnRePlay"] = btnRePlay;
}

void ResultGift::onButtonRelease(cocos2d::ui::Button * button, int id)
{
	switch (id)
	{
	case btnLobby:
		((GameScene *)(gameMgr->_scene))->toLobby();
		hide();
		break;
	case btnRePlay:
	{
		hide();
		gameMgr->_ebpCtrl->resetMatch();
		gameMgr->_ebpCtrl->playWithBot(GameMode::QUICK_MODE, 0.85);
		break;
	}
		
	default:
		break;
	}
}

