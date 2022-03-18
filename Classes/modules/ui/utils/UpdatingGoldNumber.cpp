#include "UpdatingGoldNumber.h"
#include "../../../core/utils/StringUtility.h"

USING_NS_CC;

UpdatingGoldNumber::UpdatingGoldNumber(cocos2d::ui::Text *lb, long long startNum, long long endNum, const std::string & key)
{
	_lb = lb;
	_startNumber = startNum;
	_endNumber = endNum;
	_key = key;

	_curNumber = startNum;
	_d = (endNum - startNum) / 43;
}

//-------------------

UpdatingGoldNumber::~UpdatingGoldNumber()
{

}

void UpdatingGoldNumber::startUpdate()
{
	Director::getInstance()->getScheduler()->schedule(CC_CALLBACK_1(UpdatingGoldNumber::update, this), this, 0, false, _key);
}

void UpdatingGoldNumber::update(float dt)
{
	_curNumber += _d;
	if (_d > 0 && _curNumber > _endNumber)
	{
		_curNumber = _endNumber;
		Director::getInstance()->getScheduler()->unschedule(_key, this);
	}
	else if (_curNumber < _endNumber)
	{
		_curNumber = _endNumber;
		Director::getInstance()->getScheduler()->unschedule(_key, this);
	}

	_lb->setString(StringUtility::standartNumber(_curNumber));
}

UpdatingGoldNumber* UpdatingGoldNumber::create(cocos2d::ui::Text *lb, long long startNum, long long endNum, const std::string & key)
{
	UpdatingGoldNumber * inst = new UpdatingGoldNumber(lb, startNum, endNum, key);
	inst->startUpdate();
	return inst;
}