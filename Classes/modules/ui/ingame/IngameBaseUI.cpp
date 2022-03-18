#include "IngameBaseUI.h"
#include <data/Resource.h>
#include "../elements/PowerUI.h"
#include <modules/core/interaction/InteractionManager.h>
#include <core/GameMgr.h>
#include <modules/core/utils/TimeUtils.h>
#include <modules/core/ps/internal/PhysicsConstants.hpp>
#include <base/UIFactory.h>
#include "../../core/inventory/InventoryMgr.h"

USING_NS_CC;
using namespace cocos2d::ui;

IngameBaseUI::IngameBaseUI()
{
	_powerBar = nullptr;
	_isMyTurn = true;

	_toggleCamModeCallBack = nullptr;
	_changeCueAngleCallBack = nullptr;
	
	_touchBeganTimeBtnBall = 0;
	_lastYBtnAngle = 0;
	_lastXBtnRuler = 0;

	_isDisable = false;
	_cueType = 2;

	_curAngle = 0;
}

IngameBaseUI::~IngameBaseUI()
{
	CC_SAFE_DELETE(_cueTypeSocket);
	CC_SAFE_DELETE(_cueTypeSocketBig);
}

bool IngameBaseUI::init()
{
	if(!BaseLayer::init())
		return false;

	std::string filePath = Res::GUI::getCsbPathFromJson(Res::GUI::IngameBaseUI);
	initWithBinaryFile(filePath);

	_layout->getChildByName("fake")->setVisible(false);
	initUIEffects();
	initUIStatesPos();
	initPowerbar();
	initPlacingBall();
	initBallTypeNotify();
	initAngleNode();
	initRulerNode();
	initCueTypeSocket();

	return true;
}

void IngameBaseUI::initGUI()
{
	_bg = _layout->getChildByName("bg");

	_btnBall = (Button*)getControl("btnBall", _bg);
	_btnAngle = (Button*)getControl("btnAngle", _bg);
	_btnRuler = (Button*)getControl("btnRuler", _bg);
	_btnEye = (Button*)getControl("btnEye", _bg);

	_ballIndicator = _btnBall;
	_ballIndicatorTarget = (ImageView*)getControl("ballIndicator", _btnBall);

	_btnBall->setTag(TAG::TAG_BTN_BALL);
	_btnBall->addTouchEventListener(CC_CALLBACK_2(IngameBaseUI::onCustomTouchEventHandler, this));
	_btnAngle->setTag(TAG::TAG_BTN_ANGLE);
	_btnAngle->addTouchEventListener(CC_CALLBACK_2(IngameBaseUI::onCustomTouchEventHandler, this));
	_btnRuler->setTag(TAG::TAG_BTN_RULE);
	_btnRuler->addTouchEventListener(CC_CALLBACK_2(IngameBaseUI::onCustomTouchEventHandler, this));
	_btnEye->setTag(TAG::TAG_BTN_EYE);
	_btnEye->addTouchEventListener(CC_CALLBACK_2(IngameBaseUI::onCustomTouchEventHandler, this));

	_imgPowerBar = (ImageView *)getControl("imgPowerBar", _bg);
	_imgBgBtnCue = (ImageView *)getControl("imgBgBtnCue", _bg);
	_btnCue = (Button*)getControl("btnCue");
	_btnCue->setTag(TAG::TAG_BTN_CUE);
	_btnCue->addTouchEventListener(CC_CALLBACK_2(IngameBaseUI::onCustomTouchEventHandler, this));

	_imgCollideBall = (ImageView *)getControl("imgCollideBall", _bg);
	_nodePlaceBall = getControl("nodePlaceBall", _bg);
	_imgPlaceBall = (ImageView *)getControl("imgPlaceBall", _nodePlaceBall);
	
	_imgAngleCue = (ImageView*)getControl("imgCue", _btnAngle);
	_lbAngleVal = (Text*)getControl("lbAngleVal", _btnAngle);

	_imgBarsBtnRuller = (Sprite*)getControl("imgBars", _btnRuler);
	_imgBars1BtnRuller = (Sprite*)getControl("imgBars1", _btnRuler);

	getControl("imgCollideBall", _bg)->setVisible(false);

	_pnCueIcon = (cocos2d::ui::Widget *) getControl("pnCueIcon", _bg);
	_pnCueIcon->setVisible(false);

	_imgTimerBg = (ImageView *)getControl("imgTimerBg", _bg);
}

void IngameBaseUI::initUIStatesPos()
{
	cocos2d::Node* uis[] = {
		_btnBall,
		_btnAngle,
		_btnRuler,
		_btnEye,
		_imgPowerBar,
		_imgBgBtnCue,
		// _imgMenuChat
	};

	int n = 6;
	_uis.clear();
	_uiShowPoss.clear();
	_uiHidePoss.clear();
	auto winSize = Director::getInstance()->getVisibleSize();
	for (int i = 0; i < n; i++)
	{
		auto ui = uis[i];
		Vec2 showPos = ui->getPosition();
		Vec2 hidePos = showPos;
		if (showPos.x < winSize.width / 2)
		{
			auto uiPosToOutside = -(1 - ui->getAnchorPoint().x) * ui->getContentSize().width - 10; // x offset to put ui totally outside of left side + 10 more px
			hidePos.x = uiPosToOutside;
		}
		else
		{
			auto uiPosToOutside = winSize.width + ui->getAnchorPoint().x * ui->getContentSize().width + 10; // x offset to put ui totally outside of right side + 10 more px
			hidePos.x = uiPosToOutside;
		}
		_uis.push_back(ui);
		_uiShowPoss.push_back(showPos);
		_uiHidePoss.push_back(hidePos);
	}
}

void IngameBaseUI::initUIEffects()
{
	auto btnCue = _imgBgBtnCue->getChildByName("btnCue");
	((ImageView*)btnCue->getChildByName("imgBox2")->getChildByName("imgCueType"))->ignoreContentAdaptWithSize(true);
	((ImageView*)btnCue->getChildByName("imgBox3")->getChildByName("imgCueType"))->ignoreContentAdaptWithSize(true);
	((ImageView*)btnCue->getChildByName("imgBox4")->getChildByName("imgCueType"))->ignoreContentAdaptWithSize(true);

	_ballIndicator = _btnBall;
	auto btnBall = _btnBall;
	auto btnEye = _btnEye;
	auto btnAngle = _btnAngle;
	auto btnRuler = _btnRuler;

	_buttonList.clear();
	_buttonList.push_back(btnEye);
	_buttonList.push_back(btnRuler);
	_buttonList.push_back(btnAngle);
	_buttonList.push_back(btnBall);

	for (int i = 0; i < _buttonList.size()-1; i++)
	{
		auto btn = _buttonList[i];
		auto aboveBtn = _buttonList[i+1];
		float dist = aboveBtn->getPositionY() - btn->getPositionY() - btn->getContentSize().height;
		_btnDistList.push_back(dist);
	}
}

void IngameBaseUI::initPowerbar()
{
	auto power = _imgPowerBar->getChildByName("imgPower");
	auto imgCue = power->getChildByName("imgCue");
	imgCue->removeFromParent();

	auto cn = ClippingNode::create();
	power->addChild(cn, imgCue->getLocalZOrder(), "clipping");
	auto stencil = Sprite::createWithSpriteFrameName("IngameEBP_Power_Full.png");
	stencil->setAnchorPoint(Vec2(0, 0));
	stencil->setPosition(Vec2(0, 2));
	cn->setStencil(stencil);
	cn->addChild(imgCue, cn->getLocalZOrder(), "imgCue");

	auto powerBar = PowerUI::create(_imgPowerBar);
	addChild(powerBar);
	PowerUIListener *listener = new PowerUIListener();
	listener->onChange = CC_CALLBACK_1(IngameBaseUI::onPowerBarChange, this);
	listener->onRelease = CC_CALLBACK_1(IngameBaseUI::onPowerBarRelease, this);
	powerBar->setListener(listener);
	_powerBar = powerBar;

}

void IngameBaseUI::initPlacingBall()
{
	_placingBallControl = PlacingBallControlUI::create();
	_placingBallControl->initNode(_imgPlaceBall);
	addChild(_placingBallControl);
}

void IngameBaseUI::initBallTypeNotify()
{
	Size winSize = Director::getInstance()->getVisibleSize();
	this->_ballTypeNotify = BallTypeNotify::create();
	this->addChild(this->_ballTypeNotify);
	this->_ballTypeNotify->setPosition(winSize.width / 2, winSize.height / 2);
	this->_ballTypeNotify->setVisible(false);
}

void IngameBaseUI::initAngleNode()
{
	auto btnAngle = _btnAngle;
	auto btnBall = _btnBall;
	auto btnBallSize = btnBall->getContentSize();
	auto distance = btnAngle->getPositionY() -
		(btnBall->getPositionY() + btnBallSize.height * btnBall->getScaleY());
	btnAngle->setPositionY(btnBall->getPositionY() + btnBallSize.height * btnBall->getScaleY() + distance);

	auto spriteMask = Sprite::createWithSpriteFrameName("IngameEBP_Avatar_Mask.png");
	spriteMask->setAnchorPoint(Vec2(0, 0));
	spriteMask->setPosition(0, 0);
	auto btnAngleSize = btnAngle->getContentSize();
	UIFactory::scaleToSize(spriteMask, btnAngleSize.width, btnAngleSize.height);

	auto cn = ClippingNode::create();
	btnAngle->addChild(cn);
	cn->setStencil(spriteMask);
	_imgAngleCue->removeFromParent();
	cn->addChild(_imgAngleCue);

	Sprite *angleRange = (Sprite*)getControl("angleRange", _btnAngle);
	auto angleCn = ClippingNode::create();
	btnAngle->addChild(angleCn);
	angleRange->removeFromParent();
	angleCn->addChild(angleRange);

	auto angleStencil = DrawNode::create();
	auto nodeMask = getControl("nodeMask", _btnAngle);
	angleStencil->setPosition(nodeMask->getPosition());
	Vec2 points[] = { Vec2(0, 0), Vec2(100, 0), Vec2(100, 25) };
	angleStencil->drawSolidPoly(points, 3, Color4F(Color3B(255, 255, 255)));
	angleCn->setStencil(angleStencil);
	_angleStencil = angleStencil;

	setUIAngle(0);
}

void IngameBaseUI::initRulerNode()
{
	auto btnRuler = _btnRuler;
	_midPointBtnRuler = _imgBarsBtnRuller->getPositionX();
	auto leftPoint = _midPointBtnRuler - 80;
	auto rightPoint = _midPointBtnRuler + 80;

	_imgBarsBtnRuller->removeFromParent();
	_imgBars1BtnRuller->removeFromParent();

	auto stencil = Sprite::createWithSpriteFrameName("IngameEBP_Ruler_Button.png");
	auto imgBtnRuler = btnRuler->getChildByName("img");
	stencil->setScale(imgBtnRuler->getScale()*imgBtnRuler->getContentSize().width / stencil->getContentSize().width);
	stencil->setPosition(imgBtnRuler->getPosition());

	auto cn = ClippingNode::create();
	cn->setAlphaThreshold(0.5);
	cn->setStencil(stencil);
	cn->addChild(_imgBarsBtnRuller);
	cn->addChild(_imgBars1BtnRuller);

	btnRuler->addChild(cn);
}

void IngameBaseUI::initCueTypeSocket()
{
	auto btnCue =_imgBgBtnCue->getChildByName("btnCue");
	_sockets.clear();
	_socketPos.clear();

	_sockets.push_back((ImageView *)btnCue->getChildByName("imgBox2"));
	_sockets.push_back((ImageView *)btnCue->getChildByName("imgBox3"));
	_sockets.push_back((ImageView *)btnCue->getChildByName("imgBox4"));

	for (int i = 0; i < _sockets.size(); i++) 
	{
		_socketTypes.insert(std::pair<ImageView*, int>(_sockets[i], i+2));

		_socketPos.push_back(_sockets[i]->getPosition());
		_sockets[i]->ignoreContentAdaptWithSize(true);
		((ImageView*)_sockets[i]->getChildByName("imgCueType"))->ignoreContentAdaptWithSize(true);
	}

	_cueTypeSocket = new CueTypeSocket(_btnCue);
	_cueTypeSocketBig = new CueTypeSocket(_pnCueIcon);
}

void IngameBaseUI::onEnter()
{
	BaseLayer::onEnter();
	scheduleUpdate();
}

void IngameBaseUI::update(float dt)
{
	BaseLayer::update(dt);
	
	if (!_isDisable && _match)
	{
		_match->update(dt);
	}
}

void IngameBaseUI::onPowerBarRelease(float percent)
{
	interactionMgr->handleShoot(percent);
}

void IngameBaseUI::onPowerBarChange(float percent)
{
	if (!_isMyTurn) return;
	//if (!_match.canShoot()) return;*/
	auto force = gameMgr->onChangePower(percent);
	_powerBar->setForceValue(force);
}

void IngameBaseUI::setMyTurnLayout()
{
	if (_isMyTurn) return;
	_isMyTurn = true;
	updateTurnLayout(_isMyTurn);
}

void IngameBaseUI::setEnemyTurnLayout()
{
	if (!_isMyTurn) return;
	_isMyTurn = false;
	updateTurnLayout(_isMyTurn);
}

void IngameBaseUI::updateTurnLayout(bool isMyTurn, bool onlyHUD)
{
	if (onlyHUD)
		return;
	bool myTurn = isMyTurn;
	for (int i = 0; i < _uis.size(); i++) {
		auto object = _uis[i];
		auto targetY = object->getPositionY();
		auto targetX = myTurn ? _uiShowPoss[i].x : _uiHidePoss[i].x;
		object->stopAllActions();
		if (myTurn) {
			object->runAction(
				EaseOut::create(
					MoveTo::create(0.5, Vec2(targetX, targetY)),
					2)
			);
		}
		else {
			object->runAction(EaseIn::create(
				MoveTo::create(0.5, Vec2(targetX, targetY)),
				2)
			);
		}
	}

	if (myTurn) {
		_nodePlaceBall->setVisible(true);
		_imgCollideBall->setOpacity(255);
	}
	else {
		//_nodePlaceBall->setVisible(false);
		_imgCollideBall->setOpacity(0);
	}
}

void IngameBaseUI::setCamMode3DLayout()
{
	updateTurnLayout(true);
}

void IngameBaseUI::setCamMode2DLayout()
{
	updateTurnLayout(false);
}

void IngameBaseUI::setCamModeFreeLayout()
{
	updateTurnLayout(false);
}

void IngameBaseUI::notifyBallType(bool isSolid)
{
	_ballTypeNotify->showWithType(isSolid);
}

void IngameBaseUI::notifySelectPocket()
{
	_ballTypeNotify->showSelectEightBallPocket();
}

void IngameBaseUI::subscribeChangeCueAngle(const ChangeCueAngleCallBack & callback)
{
	_changeCueAngleCallBack = callback;
}

void IngameBaseUI::subscribeToggleCamMode(const ToggleCamModeCallBack & callback)
{
	_toggleCamModeCallBack = callback;
}

void IngameBaseUI::onButtonTouched(cocos2d::ui::Button * btn, int id)
{
	if (id == TAG::TAG_BTN_BALL || id == TAG::TAG_BTN_ANGLE
		|| id == TAG::TAG_BTN_RULE)
	{
		btn->runAction(ScaleTo::create(0.2, 2));
		schedule(schedule_selector(IngameBaseUI::onBelowButtonTouchingUpdate));
	}

	switch (id) {
	case TAG::TAG_BTN_EYE:
		if (_toggleCamModeCallBack) _toggleCamModeCallBack(true);
		break;
	case IngameBaseUI::TAG::TAG_BTN_BALL:
		if (!_isMyTurn)
			break;
		_touchBeganTimeBtnBall = timeUtils->current();
		_beforePosBtnBall = btn->convertToNodeSpace(btn->getTouchBeganPosition());
		interactionMgr->startChangingCueOffset();
		break;
	case IngameBaseUI::TAG::TAG_BTN_ANGLE:
		if (!_isMyTurn)
			break;
		_lastYBtnAngle = btn->getTouchBeganPosition().y;
		break;

	case TAG::TAG_BTN_RULE:
		_lastXBtnRuler = btn->convertToNodeSpace(btn->getTouchBeganPosition()).x;
		break;

	}
}

void IngameBaseUI::onButtonTouchedMove(cocos2d::ui::Button * button, int id)
{
	switch (id)
	{
	case IngameBaseUI::TAG::TAG_BTN_BALL:
	{
		if (!_isMyTurn)
			break;
		Vec2 pos = button->getTouchMovePosition();
		Vec2 pos2 = button->convertToNodeSpace(pos);
		submitBallTarget(pos2, true);
		break;
	}
	case IngameBaseUI::TAG::TAG_BTN_ANGLE:
	{
		if (_changeCueAngleCallBack != nullptr)
		{
			auto y = button->getTouchMovePosition().y;
			float delta = y - _lastYBtnAngle;
			_curAngle += delta * IngameBaseUI::CHANGE_ANGLE_SPEED;
			_curAngle = std::max(0.f, std::min(_curAngle, IngameBaseUI::MAX_ANGLE));
			_changeCueAngleCallBack(_curAngle);
			_lastYBtnAngle = y;
		}
		break;
	}
	case TAG::TAG_BTN_RULE:
	{
		submitMoveMili(button->convertToNodeSpace(button->getTouchMovePosition()).x);
		break;
	}

	default:
		break;
	}
}

void IngameBaseUI::onButtonRelease(cocos2d::ui::Button * button, int id)
{
	if (_isDisable)
		return;
	if (id == TAG::TAG_BTN_BALL
		|| id == TAG::TAG_BTN_ANGLE
		|| id == TAG::TAG_BTN_RULE)
	{
		button->runAction(
		Sequence::create(
			ScaleTo::create(0.2, 1),
			CCCallFunc::create([this] {
				unschedule(schedule_selector(IngameBaseUI::onBelowButtonTouchingUpdate));
				onBelowButtonTouchingUpdate(0);
				}),
			nullptr
		));
	}
	switch (id) {
	case IngameBaseUI::TAG::TAG_BTN_BALL:
		if (timeUtils->current() - _touchBeganTimeBtnBall < 500)
		{
			submitBallTarget(button->convertToNodeSpace(button->getTouchEndPosition()));
		}
		interactionMgr->stopChangingCueOffset();
		break;
	case IngameBaseUI::TAG::TAG_BTN_ANGLE:
	{
		break;
	}
	case TAG::TAG_BTN_RULE:
	{
		break;
	}
	case TAG::TAG_BTN_EYE:
	{
		if (_toggleCamModeCallBack != nullptr)
		{
			_toggleCamModeCallBack(false);
		}
		break;
	}

	case TAG::TAG_BTN_CUE:
	{
		inventoryMgr->useNextCue();
		break;
	}
	}
}

void IngameBaseUI::onButtonCanceled(cocos2d::ui::Button * button, int id)
{
	if (_isDisable)
		return;
	if (id == TAG::TAG_BTN_BALL
		|| id == TAG::TAG_BTN_ANGLE
		|| id == TAG::TAG_BTN_RULE)
	{
		button->runAction(
			Sequence::create(
				ScaleTo::create(0.2, 1),
				CCCallFunc::create([this] {
					unschedule(schedule_selector(IngameBaseUI::onBelowButtonTouchingUpdate));
					onBelowButtonTouchingUpdate(0);
					}),
				nullptr
			));
	}
}

void IngameBaseUI::onCustomTouchEventHandler(cocos2d::Ref * pSender, cocos2d::ui::Widget::TouchEventType type)
{
	Button* sender = (Button*)pSender;
	switch (type)
	{
	case ui::Widget::TouchEventType::BEGAN:
		onButtonTouched(sender, sender->getTag());
	case ui::Widget::TouchEventType::MOVED:
		onButtonTouchedMove(sender, sender->getTag());
		break;
	case ui::Widget::TouchEventType::ENDED:
		onButtonRelease(sender, sender->getTag());
		break;
	case ui::Widget::TouchEventType::CANCELED:
		onButtonCanceled(sender, sender->getTag());
		break;
	}
}

void IngameBaseUI::onBelowButtonTouchingUpdate(float dt)
{
	for (int i = 1; i < _buttonList.size(); i++)
	{
		auto btn = _buttonList[i];
		auto belowButton = _buttonList[i - 1];
		btn->setPositionY(belowButton->getPositionY() 
			+ belowButton->getContentSize().height * belowButton->getScaleY()
			+ _btnDistList[i-1]);
	}
}

void IngameBaseUI::submitBallTarget(cocos2d::Vec2 pos, bool moving)
{
	auto ballIndicatorSize = this->_ballIndicator->getContentSize();
	auto center = Vec2(ballIndicatorSize.width / 2, ballIndicatorSize.height / 2);
	if (moving) {
		auto d = Vec2(pos.x - _ballIndicatorData.beforePos.x, pos.y - _ballIndicatorData.beforePos.y);
		_ballIndicatorData.offset.x += d.x;
		_ballIndicatorData.offset.y += d.y;
		_ballIndicatorData.beforePos = pos;
	}
	else {
		_ballIndicatorData.offset = Vec2(pos.x - center.x, pos.y - center.y);
	}
	// Clamp
	// const p = _ballIndicator._ballIndicator.getPosition();
	auto r = _ballIndicatorData.offset;
	auto length = sqrt(r.x * r.x + r.y * r.y);
	auto maxWidth = ballIndicatorSize.width * 0.75 * 0.5;
	if (length > maxWidth) {
		auto angle = acos(r.x / length);
		if (r.y < 0) angle = -angle;
		r.x = maxWidth * cos(angle);
		r.y = maxWidth * sin(angle);
	}

	auto h = r.x / maxWidth, v = r.y / maxWidth;
	//setBallIndicatorOffset(r, false);
	InteractionManager::getInstance()->setCueOffset(
		ps::ExtMath::vector(h * PhysicsConstants::BALL_RADIUS * 0.8, v * PhysicsConstants::BALL_RADIUS * 0.8)
	);
}

void IngameBaseUI::submitMoveMili(float posX)
{
	auto d = posX - _lastXBtnRuler;
	moveDXButtonRuler(d);

	interactionMgr->doMovingMili(d);

	_lastXBtnRuler = posX;
}

void IngameBaseUI::moveDXButtonRuler(float dx)
{
	auto btnRuler = _btnRuler;
	auto leftPoint = _midPointBtnRuler - 80;
	auto rightPoint = _midPointBtnRuler + 80;

	_imgBarsBtnRuller->setPositionX(_imgBarsBtnRuller->getPositionX() + dx);
	_imgBars1BtnRuller->setPositionX(_imgBars1BtnRuller->getPositionX() + dx);
	if (_imgBarsBtnRuller->getPositionX() < leftPoint) {
		_imgBarsBtnRuller->setPositionX(rightPoint - (leftPoint - _imgBarsBtnRuller->getPositionX()));
	}

	if (_imgBars1BtnRuller->getPositionX() < leftPoint) {
		_imgBars1BtnRuller->setPositionX(rightPoint - (leftPoint - _imgBars1BtnRuller->getPositionX()));
	}
	if (_imgBarsBtnRuller->getPositionX() > rightPoint) {
		_imgBarsBtnRuller->setPositionX(leftPoint + (_imgBarsBtnRuller->getPositionX() - rightPoint));
	}
	if (_imgBars1BtnRuller->getPositionX() > rightPoint) {
		_imgBars1BtnRuller->setPositionX(leftPoint + (_imgBars1BtnRuller->getPositionX() - rightPoint));
	}
}

void IngameBaseUI::enablePlacingBall(cocos2d::Vec2 startPosition, const PlacingBallControlUI::PlaceBallCallBack & listener)
{
	_placingBallControl->enablePlacingBall(startPosition, listener);
}

void IngameBaseUI::setPlacingPosition(cocos2d::Vec2 position)
{
	_placingBallControl->setPlacingPosition(position);
}

void IngameBaseUI::disablePlacingBall()
{
	_placingBallControl->disablePlacingBall();
}

void IngameBaseUI::setBallIndicatorOffset(cocos2d::Vec2 offset, bool percent)
{
	auto ballIndicatorSize = _ballIndicator->getContentSize();
	auto center = Vec2(ballIndicatorSize.width / 2, ballIndicatorSize.height / 2);
	auto maxWidth = ballIndicatorSize.width * 0.75 * 0.5;
	if (percent) {
		offset.x *= maxWidth * 1.0 / 0.8;
		offset.y *= maxWidth * 1.0 / 0.8;
	}
	_ballIndicatorData.offset = offset;
	this->_ballIndicatorTarget->setPosition(Vec2(center.x + offset.x, center.y + offset.y));
}

void IngameBaseUI::setUIAngle(float angle)
{
	float degree = angle * 180 / M_PI;
	_imgAngleCue->setRotation(-degree);
	_lbAngleVal->setString(std::to_string((int)(degree)));
}

void IngameBaseUI::setCueIcon(const int & cueType, double cueRange[])
{
	_cueType = cueType;
	_angleStencil->clear();
	auto p1 = Vec2(100, 100 * std::tan(cueRange[0] / 180 * M_PI));
	auto p2 = Vec2(100, 100 * std::tan(cueRange[1] / 180 * M_PI));
	
	Vec2 points[] = {Vec2(0, 0), p1, p2};
	_angleStencil->drawSolidPoly(points, 3, Color4F(Color3B(255,255,255)));

	_pnCueIcon->setVisible(true);
	forceHideBigIcon();
	_pnCueIcon->runAction(Sequence::create(
		FadeTo::create(0.2, 255),
		CCCallFunc::create(CC_CALLBACK_0(IngameBaseUI::changePnCueIconBig, this)),
		NULL
	));

	_cueTypeSocket->changeToType(cueType, NULL, true);
}

void IngameBaseUI::forceHideBigIcon()
{
	_pnCueIcon->stopAllActions();
	_pnCueIcon->setOpacity(0);
}

void IngameBaseUI::changePnCueIconBig()
{
	_cueTypeSocketBig->changeToType(_cueType, [this]() {
		_pnCueIcon->stopAllActions();
		_pnCueIcon->runAction(Sequence::create(
			DelayTime::create(0.2),
			FadeTo::create(0.5, 0),
			NULL
		));
	});
}

PowerUI * IngameBaseUI::getPowerUI()
{
	return _powerBar;
}

void IngameBaseUI::setCallPocketLayout(bool placing)
{
	setEnemyTurnLayout();
	if (placing)
	{
		_nodePlaceBall->setVisible(true);
	}
}

void IngameBaseUI::enable()
{
	_isDisable = false;
	_powerBar->enable();
}

void IngameBaseUI::disable()
{
	_isDisable = true;
	_powerBar->disable();
}

void IngameBaseUI::startMatch(EightBallPoolMatch *match)
{
	_match = match;
}

const float IngameBaseUI::BALL_INDICATOR_REGION_SCALE = 0.65f;

const float IngameBaseUI::CHANGE_ANGLE_SPEED = 0.002f;

const float IngameBaseUI::MAX_ANGLE = M_PI / 2 - 0.001;

const float IngameBaseUI::RADIAN_PER_DEGREE = M_PI / 180;

