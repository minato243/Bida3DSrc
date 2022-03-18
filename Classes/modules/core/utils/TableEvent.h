#ifndef TABLE_EVENT_H
#define TABLE_EVENT_H

#include <cocos2d.h>
#include <functional>

enum TableEventType {
	// For event that take numbers as param, add one more param useId to convert id -> number
	// Data: ball number {number: n, number: holeId}
	BALL_FELL_HOLE,

	// Data: Ball number {number: n}
	BALL_OUT_OF_TABLE,

	// Data: 2 Ball: numbers {numbers: []}, if cue ball collide then numbers[0] = 0;
	BALL_COLLIDE_BALL,

	// Data: Ball number, cushion idx {number: n, number: idx}
	BALL_COLLIDE_CUSHION,

	//              3
	//   ---------------------
	//   |                   |
	// 1 |          4        | 2
	//   ---------------------

	// Data: none {}
	BALL_OUT_OF_RANGE,

	// Data: none {}
	TIME_OUT,

	BALL_FIRST_COLLISION,

	WORLD_ON_PAUSED,

	ALL_BALLS_FELL

};

class TableEvent
{
	typedef std::function<void()> TableEventCallBack;
public:
	TableEvent();

	void subscribe(TableEventType type, TableEventCallBack callback);

	void unsubcribe(TableEventCallBack callback);

	void dispatch();
private:

public:
	
};

class TableEventData {
public:
	TableEventData() {};
	virtual ~TableEventData();
};

class BallFellHoleData : public TableEventData {
public:
	BallFellHoleData() {};
	BallFellHoleData(int number, int pocketId);
	
	int number;
	int pocketId;
};

class BallOutOfTableData : public TableEventData {
public:
	BallOutOfTableData() {};
	BallOutOfTableData(int number);

public:
	int number;
};

class BallCollisionData : public TableEventData {
public: 
	BallCollisionData() {};
	BallCollisionData(int ballNumber1, int ballNumber2, double velocity);
	BallCollisionData(const BallCollisionData & data);
public:
	int numbers[2];
	double velocity;
};

class CushionCollisionData : public TableEventData 
{
public:
	CushionCollisionData() {};
	~CushionCollisionData() {};

	CushionCollisionData(int ballId, int cushionId);

public:
	int ballId;
	int cushionId;
};

#endif // !TABLE_EVENT_H



