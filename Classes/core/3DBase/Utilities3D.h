#ifndef Utilities3D_h__
#define Utilities3D_h__

#include "cocos2d.h"

class Utilities3D {
public:
	static cocos2d::Animate3D* makeAnimation(std::string anim, int startFrame = -1, int endFrame = -1, float scale = 1);
};
#endif // Utilities3D_h__
