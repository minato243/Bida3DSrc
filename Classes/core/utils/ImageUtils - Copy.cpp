#include "ImageUtils.h"

cocos2d::Vec2 ImageUtils::imageMiddle(cocos2d::Node* image)
{
	return cocos2d::Vec2(image->getContentSize().width / 2, image->getContentSize().height / 2);
}

