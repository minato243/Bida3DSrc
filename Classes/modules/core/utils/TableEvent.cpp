#include "TableEvent.h"

TableEventData::~TableEventData()
{
}

BallCollisionData::BallCollisionData(int ballNumber1, int ballNumber2, double velocity)
{
	numbers[0] = ballNumber1;
	numbers[1] = ballNumber2;
	this->velocity = velocity;
}

BallCollisionData::BallCollisionData(const BallCollisionData & data)
{
	numbers[0] = data.numbers[0];
	numbers[1] = data.numbers[1];
	this->velocity = data.velocity;
}

BallOutOfTableData::BallOutOfTableData(int number)
{
	this->number = number;
}

BallFellHoleData::BallFellHoleData(int number, int pocketId)
{
	this->number = number;
	this->pocketId = pocketId;
}

CushionCollisionData::CushionCollisionData(int ballId, int cushionId)
{
	this->ballId = ballId;
	this->cushionId = cushionId;
}

