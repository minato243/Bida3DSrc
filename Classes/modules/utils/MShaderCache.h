#ifndef MShaderCache_H_
#define MShaderCache_H_

#include <cocos2d.h>

#define mShaderCache MShaderCache::getInstance()

class MShaderCache
{
public:
	MShaderCache();
	~MShaderCache();

	cocos2d::GLProgram * _spriteUtils;
	cocos2d::GLProgram * _grayscale;
	cocos2d::GLProgram * _spritePercentStencil;
	cocos2d::GLProgram * _glowSchedule;
	cocos2d::GLProgram * _blur;
	cocos2d::GLProgram * _contrast;
private:
	static MShaderCache *_instance;

public:
	static MShaderCache *getInstance();
};

#endif // !SHADERCACHE_H_



