#ifndef MOVEDETECTOR_H_
#define MOVEDETECTOR_H_

#include <cocos2d.h>

class MoveDetector {
public:
	MoveDetector();
	MoveDetector(float threshold);
	~MoveDetector();

	void init(cocos2d::Vec2 startPoint);

	void setPoint(cocos2d::Vec2 point);
	
	bool isMoved();

private:
	cocos2d::Vec2 _startPoint;
	bool _moved;
	float _thresholdSquare;

	const float THRESHOLD_DEFAULT = 15;
};

#endif // !MOVEDETECTOR_H_
