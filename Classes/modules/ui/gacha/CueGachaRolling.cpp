#include "CueGachaRolling.h"
#include "../../../data/Resource.h"
#include "../../camera-view/GameCamera.h"
#include "CueGachaElementSingleUI.h"
#include "CueGachaRollingList.h"

USING_NS_CC;
using namespace std;
using namespace cocos2d::ui;

CueGachaRolling::CueGachaRolling()
{

}

CueGachaRolling::~CueGachaRolling()
{

}

bool CueGachaRolling::init()
{
	if (!BaseLayer::init("CueGachaRolling"))
	{
		return false;
	}

	std::string filePath = Res::GUI::getCsbPathFromJson(Res::GUI::CueGachaRollingUI);
	this->initWithBinaryFile(filePath);

	this->_keep = true;
	setCameraMask((int)GameCamera::FLAG_UI);

	return true;
}

void CueGachaRolling::initGUI()
{
	autoInitComponent(_layout);
	_bg = _nodeMap["bg"];

	_pnList = (cocos2d::ui::Widget *)_nodeMap["pnList"];
	_pnBoxSide = (cocos2d::ui::Widget *)_nodeMap["pnBoxSide"];
	_imgCurrent = (cocos2d::ui::ImageView *)_nodeMap["imgCurrent"];

	auto size = _pnList->getContentSize();
	auto numberOfElement = 4;
	auto elementHeight = size.height / numberOfElement;
	_elementHeight = elementHeight;

	auto stencil = DrawNode::create();
	stencil->drawSolidRect(Vec2(0,0), Vec2(size.width, size.height), Color4F(Color3B(255, 255, 255)));

	auto clippingNode = ClippingNode::create();
	clippingNode->setStencil(stencil);
	_pnList->addChild(clippingNode);

	for (int i = 0; i < numberOfElement + 2; i++)
	{
		auto element = CueGachaElementSingleUI::create(Size(size.width, elementHeight));
		element->setButtonInspectVisible(false);
		clippingNode->addChild(element);
		element->setPosition(Vec2(size.width/2, size.height - (i + 0.5) * elementHeight));
		_elements.push_back(element);
	}

	_upperBound = size.height + 0.5 * elementHeight;
	_lowerBound = -0.5 * elementHeight;
	_topElement = 0;
	_pnBoxSide->setPositionX(_pnList->getPositionX() / 2 + _pnBoxSide->getContentSize().width / 2);
}

void CueGachaRolling::initTag()
{
	_tagMap.insert(pair<string, int>("btnSkip", btnSkip));
}

void CueGachaRolling::onButtonRelease(cocos2d::ui::Button* button, int id)
{
	switch (id)
	{
	case TAG::btnSkip:
	{
		skipAnimation();
		break;
	}
	default:
		break;
	}
}

void CueGachaRolling::setRandomData(const std::vector<CueRateInBox*> &data, int cueItemIdx)
{
	_data = data;
	if (_rolling)
	{
		unschedule(schedule_selector(CueGachaRolling::rollUpdate));
	}

	_rolling = false;
	for (int i = 0; i < _elements.size(); i++)
	{
		_elements[i]->loadData(getARandomResult());
	}

	_boxCode = cueItemIdx;
}

void CueGachaRolling::onOpenBoxSuccess(CueRateInBox* lastData)
{
	_endingAnimation = false;
	_endAnimationTime = 0;
	_elementTilLast = -1;
	schedule(schedule_selector(CueGachaRolling::rollUpdate));
	_rolling = true;

	_lastData = lastData;

	scheduleOnce(schedule_selector(CueGachaRolling::beginSlowEndRoll), 5);
}

CueRateInBox* CueGachaRolling::getARandomResult()
{
	int r = rand() % _data.size();
	return _data[r];
}

void CueGachaRolling::beginSlowEndRoll(float dt)
{
	slowEndRoll();
}

void CueGachaRolling::slowEndRoll()
{
	_endAnimationTime = true;

	auto time = CueGachaRollingList::END_ANIMATION_TIME;
	auto speed = CueGachaRollingList::ANIMATION_SPEED;
	auto moveLength = 1 / 2 * speed * time;

	int i = _topElement, count = 0;
	auto center = _imgCurrent->getPositionY();

	while (_elements[i]->getPositionY() > center + _elementHeight /2)
	{
		count ++ ;
		i = (i + 1) % _elements.size();
	}
	auto offset = (_elements[i]->getPositionY() - center) / _elementHeight +1/2;
	_elementTilLast = (int)(moveLength / _elementHeight + offset) - (_elements.size() - count - 1);
}

void CueGachaRolling::calcDistanceFromChosenElementToCenter()
{
	auto pos = 0;
	for (int i = 0; i < _elements.size(); i++) {
		if (_elements[i]->getData() == _lastData) {
			pos = _elements[i]->getPositionY();
		}
	}
	_distanceToMiddle = pos - _imgCurrent->getPositionY();
}

void CueGachaRolling::onAutoRollStopped()
{
	calcDistanceFromChosenElementToCenter();
	schedule(schedule_selector(CueGachaRolling::postRollUpdate));
}

void CueGachaRolling::postRollUpdate(float dt)
{
	auto speed = MIN(CueGachaRollingList::ANIMATION_SPEED_AFTER, abs(_distanceToMiddle/dt));

	if (_distanceToMiddle > 0)
	{
		speed = -speed;
	}
	auto dy = speed * dt;
	moveAllElementDy(dy);
	_distanceToMiddle += dy;
	if (abs(_distanceToMiddle) < 0.001)
	{
		unschedule(schedule_selector(CueGachaRolling::postRollUpdate));
		scheduleOnce(schedule_selector(CueGachaRolling::postRollUpdateDone),2);
	}
}

void CueGachaRolling::postRollUpdateDone(float dt)
{
	CCLOG("CueGachaRolling::postRollUpdateDone TODO");
	//gv.gameMgr.loadSceneByTag(Scenes.INSPECT_SCENE).loadData(this.lastData.cueInfo, true, true);
}

void CueGachaRolling::rollUpdate(float dt)
{
	if (!_rolling)
		return;

	auto stopRolling = false;
	if (_endAnimationTime >= CueGachaRollingList::END_ANIMATION_TIME)
	{
		stopRolling = true;
		_endAnimationTime = CueGachaRollingList::END_ANIMATION_TIME;
		onAutoRollStopped();
	}

	auto speed = CueGachaRollingList::ANIMATION_SPEED *(1 - _endAnimationTime / CueGachaRollingList::END_ANIMATION_TIME);
	moveAllElementDy(speed * dt);
	highlightCenterElement();

	if (_endingAnimation)
	{
		_endingAnimation += dt;
		_rolling = !stopRolling;
	}
}

void CueGachaRolling::skipAnimation()
{
	if (_rolling)
	{
		unschedule(schedule_selector(CueGachaRolling::rollUpdate));
	}

	if (abs(_distanceToMiddle) > 0.001)
	{
		unschedule(schedule_selector(CueGachaRolling::postRollUpdate));
	}

	_rolling = false;
	_endAnimationTime = 0;
	_endingAnimation = false;
	_distanceToMiddle = 0;

	//gameMgr.loadSceneByTag(Scenes.INSPECT_SCENE).loadData(this.lastData.cueInfo, true, true);
}

void CueGachaRolling::moveAllElementDy(float dy)
{
	auto center = _imgCurrent->getPositionY();
	for (int i = 0; i < _elements.size(); i++)
	{
		auto e = _elements[i];
		e->setPositionY(e->getPositionY() + dy);

		if (e->getPositionY() > _upperBound)
		{
			auto y = e->getPositionY() - _elements.size() * _elementHeight;
			e->setPositionY(y);
			e->setVisible(false);
			_topElement = (i + 1) % _elements.size();
			_elementTilLast--;
			if (_elementTilLast == 0)
			{
				e->loadData(_lastData);
			}
		}
	}
}

void CueGachaRolling::highlightCenterElement()
{
	auto i = _topElement;
	while (_elements[i]->getPositionY() >= _imgCurrent->getPositionY() + _elementHeight / 2
		&& (!_lastData || _elements[i]->getData() != _lastData))
	{
		_elements[i]->highlight(false);
		i = (i + 1) % _elements.size();
	}
	_elements[i]->highlight(true);
}

