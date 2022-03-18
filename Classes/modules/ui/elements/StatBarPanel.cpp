#include "StatBarPanel.h"
#include "../../core/ps/internal/ExtensionMath.hpp"
#include "../../core/utils/TimeUtils.h"

USING_NS_CC;
using namespace cocos2d::ui;

StatBarPanel::StatBarPanel(cocos2d::ui::Widget * panel)
{
	_panel = panel;
	_lbValue = (Text* )panel->getChildByName("lbValue");
	_bar = (ImageView *)panel->getChildByName("bar");
	if (_bar == nullptr)
	{
		_bar = (ImageView *)panel->getChildByName("clippingNode")->getChildByName("bar");
	}

	if (!panel)
		_valid = false;

	_animationStartTime = 0;
	_animationTime = 0;
	_panel = panel;
	_postfix = "";
	_lbValue->ignoreContentAdaptWithSize(true);
	initPanel();
	setDefaultValue();
	retain();
}

StatBarPanel::~StatBarPanel()
{

}

void StatBarPanel::setPostfix(const std::string & str)
{
	_postfix = str;
}

void StatBarPanel::setDefaultValue()
{
	_lbValue->setText("?");
	setPercent(0);
	_animationTime = 0;
	//todo
	//_panel->unschedule(schedule_selector(&StatBarPanel::updateInfo));
}

void StatBarPanel::initPanel()
{
	auto panel = _panel;
	auto barSize = _bar->getContentSize();
	barSize.width *= _bar->getScale();
	barSize.height *= _bar->getScale();
	auto stencil = DrawNode::create();
	stencil->setPosition(_bar->getPosition());
	_stencilOriginalX = stencil->getPositionX() - barSize.width;
	auto rotationWidthDif = barSize.height*tan(10 * ps::ExtMath::PI / 180);
	stencil->setRotationX(10);
	stencil->drawSolidRect(Vec2(-barSize.width / 2 - rotationWidthDif / 2, -barSize.height / 2), 
		Vec2(barSize.width / 2 + rotationWidthDif / 2, barSize.height / 2), 
		Color4F(Color3B(255, 255, 255)));
	auto clippingNode = ClippingNode::create(stencil);
	_stencil = stencil;
	_bar->removeFromParent();
	_panel->addChild(clippingNode, _panel->getLocalZOrder(), "clippingNode");
	clippingNode->addChild(_bar, _bar->getLocalZOrder(), "bar");
}

void StatBarPanel::setInfo(int val, float percent)
{
	auto curTime = timeUtils->current();
	if (curTime - _animationStartTime > _animationTime) {
		Director::getInstance()->getScheduler()->schedule(schedule_selector(StatBarPanel::updateInfo), this, 0, false);
	}
	_endVal = val;
	_endPercent = percent;
	_animationStartTime = curTime;
	_animationTime = ANIMATION_TIME * _endPercent;
}

void StatBarPanel::updateInfo(float dt)
{
	auto curTime = timeUtils->current();
	auto passedTime = curTime - _animationStartTime;
	if (passedTime >= _animationTime) {
		_setInfo(_endVal, _endPercent);
		Director::getInstance()->getScheduler()->unschedule(schedule_selector(StatBarPanel::updateInfo), this);
		return;
	}

	auto val = _endVal * passedTime / _animationTime;
	auto percent = _endPercent * passedTime / _animationTime;
	_setInfo(ceil(val), percent);
}

void StatBarPanel::_setInfo(int val, float percent)
{
	_lbValue->setString(std::to_string(val) + _postfix);
	setPercent(percent);
}

void StatBarPanel::setPercent(float percentOnOne)
{
	auto x = _stencilOriginalX + _bar->getContentSize().width * _bar->getScaleX() * percentOnOne;
	_stencil->setPositionX(x);
}


StatBarPanelAngle::StatBarPanelAngle(cocos2d::ui::Widget * panel)
	:StatBarPanel(panel)
{

}

StatBarPanelAngle::~StatBarPanelAngle()
{

}

//StatBarPanelAngle-----------------------------------------------

void StatBarPanelAngle::setInfo(float val1, float val2, float percent)
{
	auto curTime = timeUtils->current();
	if (curTime - _animationStartTime > _animationTime) {
		Director::getInstance()->getScheduler()->schedule(schedule_selector(StatBarPanelAngle::updateInfo), this, 0, false);
	}
	_endVal1 = val1;
	_endVal2 = val2;
	_endPercent = percent;
	_animationStartTime = curTime;
	_animationTime = _endPercent * ANIMATION_TIME;
}

void StatBarPanelAngle::setDefaultValue()
{
	StatBarPanel::setDefaultValue();
	_lbValue->setText("?-?");
}

void StatBarPanelAngle::updateInfo(float dt)
{
	auto curTime = timeUtils->current();
	auto passedTime = curTime - _animationStartTime;
	if (passedTime >= _animationTime) {
		_setInfo(_endVal1, _endVal2, _endPercent);
		Director::getInstance()->getScheduler()->unschedule(schedule_selector(StatBarPanelAngle::updateInfo), this);
		return;
	}


	auto val1 = _endVal1 * passedTime / _animationTime;
	auto val2 = _endVal2 * passedTime / _animationTime;
	auto percent = _endPercent * passedTime / _animationTime;
	_setInfo(ceil(val1), ceil(val2), percent);
}

void StatBarPanelAngle::_setInfo(int val1, int val2, float percent)
{
	_lbValue->setText(std::to_string(val1) + '-' + std::to_string(val2));
	setPercent(percent);
}
