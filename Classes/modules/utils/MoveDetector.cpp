#include "MoveDetector.h"

USING_NS_CC;

MoveDetector::MoveDetector()
	:_moved(true)
{
	_thresholdSquare = THRESHOLD_DEFAULT * THRESHOLD_DEFAULT;
}

MoveDetector::MoveDetector(float threshold)
	:_moved(true)
{
	_thresholdSquare = threshold * threshold;
}

MoveDetector::~MoveDetector()
{
}

void MoveDetector::init(cocos2d::Vec2 startPoint)
{
	_moved = false;
	_startPoint = startPoint;
}

void MoveDetector::setPoint(cocos2d::Vec2 point)
{
	if (_moved) return;

	auto dx = point.x - _startPoint.x;
	auto dy = point.y - _startPoint.y;
	auto distanceSquare = dx * dx + dy * dy;

	if (distanceSquare > _thresholdSquare) {
		_moved = true;
	}
}

bool MoveDetector::isMoved()
{
	return _moved;
}
