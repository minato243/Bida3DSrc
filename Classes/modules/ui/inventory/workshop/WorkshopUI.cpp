#include "WorkshopUI.h"
#include "../../../../data/Resource.h"
#include "WorkshopUpgradeUI.h"
#include "WorkshopFusionUI.h"
#include "WorkshopSwapUI.h"
#include "../../../../base/UIFactory.h"

USING_NS_CC;
using namespace cocos2d::ui;

WorkshopUI::WorkshopUI()
{
	_curTab = Tab::UNKNOWN;
}

WorkshopUI::~WorkshopUI()
{

}

bool WorkshopUI::init()
{
	if (!BaseLayer::init())
		return false;

	std::string filePath = Res::GUI::getCsbPathFromJson(Res::GUI::WorkshopUI);
	this->initWithBinaryFile(filePath);

	initGUI();
	initTabs();

	return true;
}

void WorkshopUI::initGUI()
{
	getControl("fake", _layout)->setVisible(false);
	_bg = (Widget *)getControl("bg", _layout);

	_imgTabs = (ImageView*)getControl("imgTabs", _bg);
	_btnTabFusion = customButton("btnTabFusion", TAG::BTN_TAB_FUSHION, _imgTabs);
	_btnTabUpgrade = customButton("btnTabUpgrade", TAG::BTN_TAB_UPGRADE, _imgTabs);
	_btnTabSwap = customButton("btnTabSwap", TAG::BTN_TAB_SWAP, _imgTabs);
	_btnTabs.push_back(_btnTabUpgrade); 
	_btnTabs.push_back(_btnTabFusion);
	_btnTabs.push_back(_btnTabSwap);
	_texNameActives.insert(_texNameActives.begin(), 
		{"workshop_Upgrade_Tab_01.png", "workshop_Fusion_Tab_01.png", "workshop_Swap_Tab_01.png"});
	_texNameInactives.insert(_texNameInactives.begin(),
		{ "workshop_Upgrade_Tab_02.png", "workshop_Fusion_Tab_02.png", "workshop_Swap_Tab_02.png" });

	auto bgStuff1 = getControl("bgStuff1", _bg);
	customButton("btnBack", TAG::BTN_BACK, bgStuff1);

	auto dockInventory = getControl("dockInventory", _bg);
	customButton("btnInventory", TAG::BTN_INVENTORY, dockInventory);
}

void WorkshopUI::initTabs()
{
	auto upgradeTab = WorkshopUpgradeUI::create();
	addChild(upgradeTab);

	auto fusionTab = WorkshopFusionUI::create();
	addChild(fusionTab);

	auto swapTab = WorkshopSwapUI::create();
	addChild(swapTab);

	_tabs.push_back(upgradeTab);
	_tabs.push_back(fusionTab);
	_tabs.push_back(swapTab);

	switchTab(Tab::UPGRADE);
}

void WorkshopUI::refresh()
{

}

void WorkshopUI::setTrackingCue(int cue)
{

}

void WorkshopUI::switchTab(Tab tabIdx)
{
	CCLOG(" WorkshopUI::switchTab %d", tabIdx);
	if (_curTab == tabIdx) return;
	_curTab = tabIdx;

	for (int i = 0; i < _tabs.size(); i++)
	{
		auto tab = _tabs[i];
		bool active = i == tabIdx;
		tab->setVisible(active);
		//if (active) {
		//	tab->refresh();
		//	tab->onSwitched() ;
		//}
		_imgTabs->getChildByName("imgTabActive" + std::to_string(i + 1))->setVisible(active);
		UIFactory::loadButtonTextures(_btnTabs[i], active ? _texNameActives[i] : _texNameInactives[i]);
	}
}

void WorkshopUI::setParentScene(cocos2d::Scene *scene)
{

}

void WorkshopUI::onButtonRelease(cocos2d::ui::Button* button, int id)
{
	switch (id) {
	case TAG::BTN_TAB_FUSHION:
		this->switchTab(Tab::FUSION);
		break;
	case TAG::BTN_TAB_UPGRADE:
		this->switchTab(Tab::UPGRADE);
		break;
	case TAG::BTN_TAB_SWAP:
		this->switchTab(Tab::SWAP);
		break;
	case TAG::BTN_BACK:
		CCLOG("WorkshopUI::onButtonRelease TAG::BTN_TAB_BACK");
		break;
	case TAG::BTN_INVENTORY:
		CCLOG("WorkshopUI::onButtonRelease TAG::BTN_TAB_INVENTORY");
		break;
	}
}

