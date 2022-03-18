#include "ChooseLoginTypeLayer.h"
#include <data/Resource.h>
#include "LoadingSceneNew.h"

USING_NS_CC;
using namespace cocos2d::ui;

ChooseLoginTypeLayer::ChooseLoginTypeLayer()
{
	_parentLayer = nullptr;
}

ChooseLoginTypeLayer::~ChooseLoginTypeLayer()
{
}

bool ChooseLoginTypeLayer::init()
{
	if (!BaseLayer::init())
	{
		return false;
	}

	renderLoginTypeButtons();

	return true;
}

void ChooseLoginTypeLayer::initGUI()
{
}

void ChooseLoginTypeLayer::renderLoginTypeButtons()
{
	Size winSize = Director::getInstance()->getVisibleSize();
	auto margin = 10;
	auto center = Vec2(winSize.width / 2, winSize.height / 2);
	
	// Render DevID button
	auto devBtn = createCustomButton("Login With DevID", CC_CALLBACK_0(ChooseLoginTypeLayer::onButtonClick, this));
	devBtn->setPosition(Vec2(
		center.x + (devBtn->getContentSize().width / 2 + margin), 
		center.y - (devBtn->getContentSize().height / 2 + margin)
	));
	addChild(devBtn);
}

cocos2d::ui::Button * ChooseLoginTypeLayer::createCustomButton(const char * text, const CUSTOM_CALLBACK & callback)
{
	auto btn = Button::create(Res::INGAME_BLUE_BUTTON);
	btn->setScale9Enabled(true);
	btn->setCapInsets(Rect(10, 10, 10, 10));
	btn->setContentSize(Size(150, 50));
	auto lbl = LabelBMFont::create();
	lbl->setString(text);
	btn->addChild(lbl);
	lbl->setPosition(btn->getContentSize().width / 2, btn->getContentSize().height / 2);
	btn->addTouchEventListener([this](Ref *sender, Widget::TouchEventType type) {
		onButtonClick();
	});
	return btn;
}

void ChooseLoginTypeLayer::onButtonClick()
{
	auto loadingScene = dynamic_cast<LoadingSceneNew *>(_parentLayer);
	if (loadingScene)
	{
		loadingScene->onBtnDevClick();
	}
}

void ChooseLoginTypeLayer::setParentLayer(BaseLayer * parent)
{
	_parentLayer = parent;
}
