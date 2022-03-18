#ifndef Utils_h_
#define Utils_h_

#include <cocos2d.h>

class Utils
{
public:
	static cocos2d::GLProgramState* setUpCue3DShader(cocos2d::Sprite3D *cue, std::string albedoPath, std::string mraoPath);

	static float easeInOutCubic(float x);

	static float easeInOutSine(float x);

	static float easeInSine(float x);

	static float easeOutSine(float x);

	static float easeInOutQuad(float x);

	static cocos2d::Color3B mix(const cocos2d::Color3B & a, const cocos2d::Color3B & b, float r);

	static float mix(const float & a, const float & b, const float & r);

	static cocos2d::Node* getEfkCircle(bool shadow = false);

	static void releaseEfkCircle(cocos2d::Node * efk);

	std::list<cocos2d::Node *> _efkShadowPool;
	std::list<cocos2d::Node *> _efkCirclePool;
	std::map<cocos2d::Node *, bool> _isShadowMaps;
 
	static Utils *getInstance();
private:
	static Utils *_instance;
};

#endif