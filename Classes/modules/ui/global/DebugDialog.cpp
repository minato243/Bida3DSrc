#include "DebugDialog.h"
#include "../../../data/Resource.h"
#include "CheatItemUI.h"
#include "../../camera-view/GameCamera.h"
#include "CheatOfferUI.h"
#include "../../core/utils/TimeUtils.h"
#include "../elements/QuickCustomTableView.h"

USING_NS_CC;
using namespace cocos2d::ui;
using namespace std;

DebugDialog::DebugDialog()
{

}

DebugDialog::~DebugDialog()
{

}

bool DebugDialog::init()
{
	if (!BaseLayer::init("DebugDialog"))
	{
		return false;
	}

	std::string filePath = Res::GUI::getCsbPathFromJson(Res::GUI::DebugDialog);
	this->initWithBinaryFile(filePath);

	this->_keep = true;

	//setCameraMask((int)GameCamera::FLAG_UI_ALWAYS_TOP);

	return true;
}

void DebugDialog::initGUI()
{
	initTag();
	autoInitComponent(_layout);

	_pnContent = (Widget*)_nodeMap["pnContent"];
	_pnListCate = (Widget *)_nodeMap["pnListCate"];

	initCheatList();
	initScrollList();
}

void DebugDialog::initTag()
{
	_tagMap.insert(pair<string, int>("btnBack",TAG::btnBack));
}

void DebugDialog::onButtonRelease(cocos2d::ui::Button* button, int id)
{
	switch (id)
	{
	case TAG::btnBack:
	{
		hide();
		break;
	}
	default:
		break;
	}
}

void DebugDialog::initCheatList()
{
	auto pos = Vec2(_pnContent->getContentSize().width/2, _pnContent->getContentSize().height / 2);

	_uis.push_back(CheatItemUI::create());
	_uis.push_back(CheatOfferUI::create());

	for (auto ui : _uis)
	{
		_pnContent->addChild(ui);
		ui->setPosition(pos);
	}

	_names = {
		new string("Cheat Item"),
		new string("Cheat Offer"),
	};

	showUI(_uis[1]);
}

void DebugDialog::initScrollList()
{
	auto size = _pnListCate->getContentSize();
	auto scrollView = QuickCustomTableView<string, DebugDialogTab>::create(size, extension::ScrollView::Direction::VERTICAL, 6, 1);
	scrollView->setInitItemCallback(CC_CALLBACK_1(DebugDialog::onInitTab, this));
	scrollView->reloadData(_names);
	
	_pnListCate->addChild(scrollView);
}

void DebugDialog::onInitTab(DebugDialogTab *tab)
{
	tab->setDelegate(this);
}

void DebugDialog::showUI(cocos2d::Node *ui)
{
	if (!ui) return;
	for (auto i = 0; i < _uis.size(); i++) {
		if (_uis[i]) _uis[i]->setVisible(false);
	}
	ui->setVisible(true);
}

void DebugDialog::showUI(std::string *uiName)
{
	for (auto i = 0; i < _names.size(); i++) {
		if (_names[i]->compare(*uiName) == 0)
		{
			showUI(_uis[i]);
			return;
		}
	}
}

// DebugDialogTab
DebugDialogTab::DebugDialogTab()
{

}

DebugDialogTab::~DebugDialogTab()
{

}

bool DebugDialogTab::init()
{
	if (!UIBaseLayer::init())
	{
		return false;
	}

	std::string filePath = Res::GUI::getCsbPathFromJson(Res::GUI::DebugDialogTab);
	this->initWithBinaryFile(filePath);
	return true;
}

void DebugDialogTab::initGUI()
{
	_tagMap["btnBg"] = 0;
	autoInitComponent(_layout);
	_btnBg = (Button *)_nodeMap["btnBg"];
	_lbTitle = (Text *)_nodeMap["lbTitle"];
}

cocos2d::Size DebugDialogTab::getOriginalSize()
{
	return _btnBg->getContentSize();
}

void DebugDialogTab::onButtonTouched(cocos2d::ui::Button* button, int id)
{
	beganTouch = timeUtils->milisecondTime();
}

void DebugDialogTab::onButtonRelease(cocos2d::ui::Button* button, int id)
{
	if (timeUtils->milisecondTime() - beganTouch > 150)
		return;

	CCLOG("onButtonRelease");
	if (_delegate != NULL)
	{
		auto debugDialog = dynamic_cast<DebugDialog *>(_delegate);
		if (debugDialog)
		{
			debugDialog->showUI(_uiName);
		}
	}
}

void DebugDialogTab::loadData(std::string *name)
{
	_uiName = name;
	_lbTitle->setString(*name);
}

void DebugDialogTab::setDelegate(cocos2d::Node *delegate)
{
	_delegate = delegate;
}
