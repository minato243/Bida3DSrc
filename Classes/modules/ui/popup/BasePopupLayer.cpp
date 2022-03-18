#include "BasePopupLayer.h"
#include "../GuiMgr.h"

USING_NS_CC;
using namespace cocos2d::ui;
using namespace std;

BasePopupLayer::BasePopupLayer()
	:BaseLayer()
{
	_swallowTouchBackground = true;
}

BasePopupLayer::~BasePopupLayer()
{

}

bool BasePopupLayer::init(std::string id)
{
	BaseLayer::init(id);
	return true;
}

void BasePopupLayer::initGUI()
{

}

void BasePopupLayer::initBlockListener()
{
	_listener = EventListenerTouchOneByOne::create();
	_listener->setSwallowTouches(true);

	_listener->onTouchBegan = [this](cocos2d::Touch* touch, cocos2d::Event* event) {
		return true;
	};

	_listener->onTouchMoved = [this](cocos2d::Touch* touch, cocos2d::Event* event) {
	};

	_listener->onTouchEnded = [this](cocos2d::Touch* touch, cocos2d::Event* event) {
		onClose();
	};

	Director::getInstance()->getEventDispatcher()->addEventListenerWithSceneGraphPriority(_listener, this);
}

void BasePopupLayer::onEnter()
{
	BaseLayer::onEnter();
	initBlockListener();
}

void BasePopupLayer::onCloseDone()
{
	_isShow = false;
	bool keep = _keep;
	guiMgr->hide(this, _keep, this->_tag);

	if (keep)
	{
		retain();
		removeFromParent();
		_parent = NULL;
	}
}

