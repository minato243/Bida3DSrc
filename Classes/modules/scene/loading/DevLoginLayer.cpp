#include "DevLoginLayer.h"
#include <data/Resource.h>
#include <core/utils/StorageUtil.h>
#include "LoadingSceneNew.h"

USING_NS_CC;
using namespace cocos2d::ui;

DevLoginLayer::DevLoginLayer()
{
}

DevLoginLayer::~DevLoginLayer()
{
}

bool DevLoginLayer::init()
{
	BaseLayer::init();
	renderInputBoxes();
	return true;
}

void DevLoginLayer::initGUI()
{
}

void DevLoginLayer::renderInputBoxes()
{
	Size winSize = Director::getInstance()->getVisibleSize();

	auto inputBg = Scale9Sprite::create(Res::INGAME_REWARD_BOX);
	inputBg->setScaleX(1.1);
	inputBg->setCapInsets(Rect(10, 10, 10, 10));
	inputBg->setContentSize(Size(400, 70));

	auto inputBox = EditBox::create(inputBg->getContentSize(), inputBg);
	inputBox->setInputFlag(EditBox::InputFlag::SENSITIVE);
	inputBox->setFontSize(40);
	inputBox->setPlaceHolder("ID");
	inputBox->setPosition(Vec2(winSize.width / 2, winSize.height / 2));
	addChild(inputBox);

	auto savedID = StorageUtil::getInt("user_id", 10);
	inputBox->setText(std::to_string(savedID).c_str());

	_inputBox = inputBox;

	// Render button
	auto btn = Button::create(Res::INGAME_BLUE_BUTTON);
	btn->setScale9Enabled(true);
	btn->setCapInsets(Rect(10, 10, 10, 10));
	btn->setContentSize(Size(150, 80));
	
	auto lbl = LabelBMFont::create();
	lbl->setString("Login");
	btn->addChild(lbl);
	lbl->setPosition(Vec2(btn->getContentSize().width / 2, btn->getContentSize().height / 2));
	
	btn->addTouchEventListener(CC_CALLBACK_2(DevLoginLayer::onButtonTouch,this));
	btn->setPosition(Vec2(
		inputBox->getPositionX(), 
		inputBox->getPositionY() - inputBox->getContentSize().height / 2 - btn->getContentSize().height / 2 - 30));
	addChild(btn);
	
	renderSocialIDBox();
}

void DevLoginLayer::renderSocialIDBox()
{
	Size winSize = Director::getInstance()->getVisibleSize();

	auto savedDevice = StorageUtil::getString("device_id", "");
	auto inputBg = Scale9Sprite::create(Res::INGAME_REWARD_BOX);
	inputBg->setScaleX(1.1);
	inputBg->setCapInsets(Rect(10, 10, 10, 10));
	inputBg->setContentSize(Size(400, 70));

	auto saveUid = StorageUtil::getInt("user_id", 102);
	auto inputBox = EditBox::create(inputBg->getContentSize(), inputBg);
	inputBox->setInputFlag(EditBox::InputFlag::SENSITIVE);
	inputBox->setFontSize(40);
	inputBox->setPlaceHolder("Fake Device ID");
	inputBox->setPosition(Vec2(winSize.width / 2, winSize.height / 2 + 100));
	inputBox->setText(savedDevice.c_str());
	addChild(inputBox);

	_socialIDBox = inputBox;
}

void DevLoginLayer::onButtonTouch(Ref * sender, Widget::TouchEventType type)
{
	if (type == Widget::TouchEventType::ENDED) {
		doLogin();
	}
}

void DevLoginLayer::doLogin()
{
	auto str = _inputBox->getText();
	auto id = atoi(str);
	StorageUtil::setInt("user_id", (int)id);
	StorageUtil::setString("device_id", _socialIDBox->getText());

	auto loadingScene = dynamic_cast<LoadingSceneNew *>(_parentLayer);
	if (loadingScene)
	{
		loadingScene->doLoginDev(id, _socialIDBox->getText());
	}
}

void DevLoginLayer::setParentLayer(BaseLayer * parent)
{
	_parentLayer = parent;
}
