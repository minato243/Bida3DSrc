#include "TriggerUtil.h"
#include <data/DataStruct.h>

using namespace tu;

TimeTrigger::TimeTrigger(const TimeTriggerCallBack &callback, double period)
{
	_callback = callback;
	_period = period;
	_time = 0;
	_enabled = false;
	//_data = {};
}

TimeTrigger::~TimeTrigger()
{
}

void tu::TimeTrigger::setEnabled(bool enabled)
{
	_enabled = enabled;
	_time = 0;
}

void tu::TimeTrigger::update(float dt)
{
	if (_enabled) {
		if (_period == 0) { // Call Every Frame
			_callback(dt);
		}
		else {
			auto time = _time + dt;
			if (time < _period) {
				_time = time;
			}
			else {
				_time = time - _period;
				_callback(_period);
			}
		}
	}
}

TimeTrigger * tu::TimeTrigger::makeTrigger(const TimeTriggerCallBack & callback, double period)
{
	return new TimeTrigger(callback, period);
}

//----------------LimitedTimeTrigger-----------------
tu::LimitedTimeTrigger::LimitedTimeTrigger(LimitedTimeTriggerrCallBack callback, double period)
{
	_callback = callback;
	_period = period;
	_time = 0;
	_enabled = false;
	_remainTime = 0;
}

tu::LimitedTimeTrigger::~LimitedTimeTrigger()
{
}

void tu::LimitedTimeTrigger::enable(double time)
{
	_remainTime = time;
	_time = 0;
	_enabled = true;
}

void tu::LimitedTimeTrigger::disable()
{
	_remainTime = 0;
	_enabled = false;
}

void tu::LimitedTimeTrigger::update(float dt)
{
	if (_remainTime > 0) {
		if (dt > _remainTime) {
			dt = _remainTime;
		}
		_remainTime -= dt;

		if (_period == 0) { // Call Every Frame
			if(_callback) _callback(dt, &_data);
		}
		else {
			auto time = _time + dt;
			if (time < _period) {
				_time = time;
			}
			else {
				_time = time - _period;
				if (_callback) _callback(_period, &_data);
			}
		}

		if (_remainTime <= 0) {
			for (auto i = 0; i < _completeHandlers.size(); i++) {
				auto handler = _completeHandlers[i];
				handler();
			}
			_completeHandlers.clear();
			_enabled = false;
		}
	}
}

void tu::LimitedTimeTrigger::onceCompleted(const LimitedTimeTriggerrCompleteCallBack & callback)
{
	if (_remainTime <= 0) {
		callback();
	}
	else {
		_completeHandlers.push_back(callback);
	}
}

TransitionData *tu::LimitedTimeTrigger::data()
{
	return &_data;
}

bool tu::LimitedTimeTrigger::isEnable()
{
	return _enabled;
}

LimitedTimeTrigger * tu::LimitedTimeTrigger::makeTrigger(LimitedTimeTriggerrCallBack callback, double period)
{
	return new LimitedTimeTrigger(callback, period);
}



//---------------------TimeoutTrigger------------------------
tu::TimeoutTrigger::TimeoutTrigger(TimeoutTriggerCallBack callback)
{
	_callback = callback;
	_remainTime = 0;
}

tu::TimeoutTrigger::~TimeoutTrigger()
{
}

void tu::TimeoutTrigger::enable(double timeout)
{
	_remainTime = timeout;
}

void tu::TimeoutTrigger::disable()
{
	_remainTime = 0;
}

void tu::TimeoutTrigger::update(float dt)
{
	if (_remainTime ==0) return;
	_remainTime -= dt;
	if (_remainTime > 0) return;
	_remainTime = 0;
	_callback();
}

TimeoutTrigger * tu::TimeoutTrigger::makeTrigger(const TimeoutTriggerCallBack & callback)
{
	return new TimeoutTrigger(callback);
}


tu::Trigger::Trigger(const TriggerCallBack & callback)
{
	this->callback = callback;
	this->run = false;
}

//---------------TriggerUtil------------------
tu::TriggerUtil::TriggerUtil()
{
}

tu::TriggerUtil::~TriggerUtil()
{
}

Trigger tu::TriggerUtil::registerOnce(const TriggerCallBack & callback)
{
	return Trigger(callback);
}

void tu::TriggerUtil::trigger(Trigger trig)
{
	if (trig.run) return;
	trig.run = true;
	trig.callback();
}


template<class T>
void tu::Dispatcher<T>::subscribe(const DispatcherCallBack & callback)
{
	_subscribers.push_back(callback);
}

template<class T>
void tu::Dispatcher<T>::unsubscribe(const DispatcherCallBack & callback)
{
	for (int i = 0; i < this->_subscribers.size(); i++) {
		if (getAddress<void>(this->_subscribers.at(i)) == getAddress<void>(callback)) {
			this->_subscribers.erase(this->_subscribers.begin() + i);
			return;
		}
	}
}

template<class T>
void tu::Dispatcher<T>::dispatch(T data)
{
	for (auto i = 0; i < _subscribers.size(); i++) {
		auto sub = this->_subscribers[i];
		sub(data);
	}
}

void tu::PositionDispacher::subscribe(const PosDispatcherCallBack & callback)
{
	_subscribers.push_back(callback);
}

void tu::PositionDispacher::unsubscribe(const PosDispatcherCallBack & callback)
{
	for (int i = 0; i < this->_subscribers.size(); i++) {
		if (getAddress2(this->_subscribers.at(i)) == getAddress2(callback)) {
			this->_subscribers.erase(this->_subscribers.begin() + i);
			return;
		}
	}
}

void tu::PositionDispacher::dispatch(cocos2d::Vec3 data)
{
	for (auto i = 0; i < _subscribers.size(); i++) {
		auto sub = this->_subscribers[i];
		sub(data);
	}
}
