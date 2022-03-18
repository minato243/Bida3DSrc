#ifndef TIMEUTILS_H_
#define TIMEUTILS_H_

#define timeUtils TimeUtils::getInstance()

class TimeUtils
{
public:
	TimeUtils();
	~TimeUtils();

	void update(float dt);

	long long current();

	long long milisecondTime();

	int getRemainTimeEndDay();

private:
	long long _current;
	static TimeUtils *_instance;
public:
	static TimeUtils *getInstance();
};

#endif // !TIMEUTILS_H_



