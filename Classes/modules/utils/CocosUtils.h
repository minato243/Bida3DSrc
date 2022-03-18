#ifndef COCOSUTILS_H_
#define COCOSUTILS_H_

#include <cocos2d.h>
#include <modules/core/ps/internal/ExtensionMath.hpp>

class CocosUtils
{
public:
	static cocos2d::Vec3 transVecToCCVec(ps::ExtMath::vector vec);
	static ps::ExtMath::vector transCCVecToGameVec(cocos2d::Vec3 vec);
};

#endif // !COCOSUTILS_H_


