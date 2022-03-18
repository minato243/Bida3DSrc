#include "BroadcastMgr.h"
#include "core/gui/SceneMgr.h"
#include "core/Constant.h"
#include "login/LoginScene.h"

USING_NS_CC;
using namespace cocos2d::ui;

const float Broadcast::UPDATE_DELTA_TIME = 1.0f/60;

const float Broadcast::WIDTH_RATIO_DEFAULT = 0.3f;

const cocos2d::Vec2 Broadcast::ANCHOR_DEFAULT = Vec2(0, 0.5f);

const int Broadcast::MESSAGE_NORMAL = 1;

const int Broadcast::MESSAGE_RICH = 2;

const std::string Broadcast::TYPE_SYSTEM = "system_broadcast";

const std::string Broadcast::TYPE_EVENT = "event_broadcast";

const int Broadcast::TIMEOUT_MESSAGE = 30;

BroadcastNode::BroadcastNode()
{

}

BroadcastNode::~BroadcastNode()
{

}

BroadcastNode* BroadcastNode::create(std::string message, int type, cocos2d::Vec2 pos, int rp)
{
	BroadcastNode* broadcastNode = new BroadcastNode();
	broadcastNode->init(message, type, pos, rp);
	broadcastNode->autorelease();
	return broadcastNode;
}

bool BroadcastNode::init(std::string message, int type, cocos2d::Vec2 pos, int rp)
{
	Node::init();

	lbMessage = nullptr;

	doMessage = false;
	moveX = 0;

	numRepeat = rp;

	isDead = false;

	//ID = new Date().getTime();
	sprite = Sprite::create("Common/bgBroadcast.png");
	sprite->setOpacity(150);
	addChild(sprite);

	float length = Director::getInstance()->getVisibleSize().width * Broadcast::WIDTH_RATIO_DEFAULT;
	sprite->setPosition(Vec2(pos.x + length * 0.5, pos.y + 10));
	node = Node::create();
	addChild(node);
	node->setPosition(pos);
	switch (type) {
	case Broadcast::MESSAGE_NORMAL: {
		createMessage(message, pos);
		break;
	}
	case Broadcast::MESSAGE_RICH: {
		
		break;
	}
	}

	setVisible(false);
	retain();

	sceneMgr->layerSystem->addChild(this);
	return true;
}

void BroadcastNode::createMessage(std::string message, cocos2d::Vec2 pos)
{
	float length = Director::getInstance()->getVisibleSize().width *Broadcast::WIDTH_RATIO_DEFAULT;
	lbMessage = Text::create();
	lbMessage->setAnchorPoint(Broadcast::ANCHOR_DEFAULT);
	lbMessage->setFontName(SceneMgr::FONT_NORMAL);
	lbMessage->setFontSize(SceneMgr::FONT_SIZE_DEFAULT);
	lbMessage->setTextHorizontalAlignment(TextHAlignment::RIGHT);
	lbMessage->setColor(sceneMgr->ccWhite);
	lbMessage->setString(message);
	lbMessage->setPositionX(length);
	lbMessage_saveX = length;
	lbMessage->setPositionY(10);
	lbMessage_overX = -lbMessage->getContentSize().width;

	clipNode(pos);
}

void BroadcastNode::clipNode(cocos2d::Vec2 pos)
{
	Size winSize = Director::getInstance()->getVisibleSize();
	float length = winSize.width *Broadcast::WIDTH_RATIO_DEFAULT;

	DrawNode* shape = DrawNode::create();
	Color4F green = Color4F(0, 1, 0, 1);
	shape->drawRect(Vec2(0, 0), Vec2(length, 70), green);

	ClippingNode* clipper = ClippingNode::create();
	clipper->setTag(100);
	clipper->setAnchorPoint(Vec2(1, 0.5f));
	clipper->setStencil(shape);
	clipper->addChild(lbMessage);
	float scale = winSize.width / CONSTANT_WIDTH;
	scale = (scale > 1) ? 1 : scale;
	clipper->setScale(scale);

	float toX = 0;
	toX = -lbMessage->getContentSize().width - length;
	nTimeoutMessage = abs(toX / 30);
	moveX = -1;

	node->addChild(clipper);
}

void BroadcastNode::doPlay()
{
	numRepeat--;
	timeoutMessage = nTimeoutMessage;
	lbMessage->setPositionX(lbMessage_saveX);
	doMessage = true;
	setVisible(true);

	//cc.log("++newPlay " + this.ID + " | " + this.timeoutMessage + " | " + this.numRepeat);

	if (!getParent()) {
		retain();
		sceneMgr->layerSystem->addChild(this);
	}
}

void BroadcastNode::doReload()
{
	if (doMessage) {
		sceneMgr->layerSystem->addChild(this);
	}
}

void BroadcastNode::doFinish()
{
	if (numRepeat <= 0) {
		doMessage = false;
		isDead = true;
	}
	else {
		doPlay();
	}
}

void BroadcastNode::doUpdate(float dt)
{
	if (doMessage && getParent()) {
		lbMessage->setPositionX(lbMessage->getPositionX() + moveX);
		timeoutMessage -= dt;
		if (timeoutMessage <= 0 || lbMessage->getPositionX() <= lbMessage_overX)
		{
			doFinish();
		}
	}
}

BroadcastHandler::BroadcastHandler(std::string id, int type, cocos2d::Vec2 pos)
{
	currentBroadcast = nullptr;
	timeoutWait = 0;
	handlerID = id;
	broadcastType = type;
	broadcastPos = pos;
}

BroadcastHandler::~BroadcastHandler()
{

}

void BroadcastHandler::addMessage(std::string message, int repeat /*= 0*/, bool top /*= false*/)
{
	BroadcastNode* br = BroadcastNode::create(message, broadcastType, broadcastPos, repeat);
	top = top || false;
	if (top)
		arMessage.insert(arMessage.begin(), br);
	else
		arMessage.push_back(br);

	checkAndPlay();
}

void BroadcastHandler::checkAndPlay(bool timeout)
{
	for (int i = 0; i < broadcastMgr->arHandler.size(); i++) {
		if (handlerID != broadcastMgr->arHandler[i]->handlerID) {
			if (broadcastMgr->arHandler[i]->currentBroadcast) {
				return;
			}
		}
	}
	if (timeout) {
		if (currentBroadcast) {
			currentBroadcast->removeFromParent();
			currentBroadcast = nullptr;
		}
	}

	if (currentBroadcast) 
		return;
	if (arMessage.size() <= 0) {
		for (int i = 0; i < broadcastMgr->arHandler.size(); i++) {
			if (handlerID != broadcastMgr->arHandler[i]->handlerID) {
				if (broadcastMgr->arHandler[i]->arMessage.size() > 0) {
					broadcastMgr->arHandler[i]->checkAndPlay();
				}
			}
		}
		return;
	}

	timeoutWait = Broadcast::TIMEOUT_MESSAGE;

	currentBroadcast = arMessage[0];
	currentBroadcast->doPlay();

	arMessage.erase(arMessage.begin());
}

void BroadcastHandler::reloadBroadcast()
{
	if (currentBroadcast) {
		currentBroadcast->doReload();
	}
}

void BroadcastHandler::updateHandler(float dt)
{
	if (timeoutWait > 0 && (currentBroadcast == nullptr)) {
		timeoutWait -= dt;
		if (timeoutWait <= 0) {
			checkAndPlay(true);
		}
	}

	if (currentBroadcast) {
		currentBroadcast->doUpdate(dt);
		if (currentBroadcast->isDead) {
			currentBroadcast->removeFromParent();
			currentBroadcast = nullptr;
			checkAndPlay();
		}
	}
}

BroadcastMgr::BroadcastMgr()
{
	doUpdate = false;
}

BroadcastMgr::~BroadcastMgr()
{

}

BroadcastMgr* BroadcastMgr::getInstance()
{
	if (!_inst) {
		_inst = new BroadcastMgr();
	}
	return _inst;
}

void BroadcastMgr::onStart()
{
	Size winSize = Director::getInstance()->getVisibleSize();

	float x = winSize.width * 0.5 - winSize.width*Broadcast::WIDTH_RATIO_DEFAULT * 0.5;
	float y = winSize.height - 30;
	addHandler(Broadcast::TYPE_SYSTEM, Broadcast::MESSAGE_NORMAL, Vec2(x, y));

}

void BroadcastMgr::reloadBroadcast()
{
	doUpdate = true;

	// remove when login
	if (sceneMgr->checkMainLayer<LoginScene>()) {
		arHandler.clear();
		doUpdate = false;
		sceneMgr->layerSystem->removeAllChildren();
	}

	// reload all broadcast running
	for (int i = 0, size = arHandler.size(); i < size; i++) {
		arHandler[i]->reloadBroadcast();
	}
}

void BroadcastMgr::addHandler(std::string id, int type, cocos2d::Vec2 pos)
{
	if (!getHandler(id)) {
		arHandler.push_back(new BroadcastHandler(id, type, pos));
	}
}

BroadcastHandler* BroadcastMgr::getHandler(std::string id)
{
	for (int i = 0, size = arHandler.size(); i < size; i++) {
		if (arHandler[i]->handlerID.compare(id) == 0) {
			return arHandler[i];
		}
	}
	return nullptr;
}

void BroadcastMgr::addMessage(std::string handlerId, std::string message, int repeat /*= 0*/, bool top /*= false*/)
{
	BroadcastHandler* handler = getHandler(handlerId);
	if (handler) {
		handler->addMessage(message, repeat, top);
	}
}

void BroadcastMgr::updateBroadcast(float dt)
{
	if (!doUpdate) return;

	for (int i = 0, size = arHandler.size(); i < size; i++) {
		arHandler[i]->updateHandler(dt);
	}
}

BroadcastMgr* BroadcastMgr::_inst = nullptr;
