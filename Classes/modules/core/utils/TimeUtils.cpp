#include "TimeUtils.h"
#include <ctime>
#include <time.h>


TimeUtils::TimeUtils()
{
	_current = std::time(0) *1000;
}

void TimeUtils::update(float dt)
{
	_current +=(int) (dt * 1000);
}

long long TimeUtils::current()
{
	return _current/1000;
}

long long TimeUtils::milisecondTime()
{
	return _current;
}

int TimeUtils::getRemainTimeEndDay()
{
	time_t curtime = current();
	tm* time = localtime(&curtime);
	time->tm_hour = 23; time->tm_min = 59; time->tm_sec = 59;
	long endDay = mktime(time);
	long timeRemain = endDay - curtime;
	return timeRemain;
}

TimeUtils *TimeUtils::_instance = nullptr;
TimeUtils * TimeUtils::getInstance()
{
	if (_instance == nullptr) {
		_instance = new TimeUtils();
	}
	return _instance;
}
