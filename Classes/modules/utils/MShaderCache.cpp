#include "MShaderCache.h"

USING_NS_CC;

MShaderCache::MShaderCache()
{
	_spriteUtils = GLProgram::createWithFilenames("res/shaders/sprite_util.vert", "res/shaders/sprite_util.frag");
	_spriteUtils->link();
	_spriteUtils->updateUniforms();
	_spriteUtils->retain();

	_grayscale = GLProgram::createWithFilenames("res/shaders/grayscale.vert", "res/shaders/grayscale.frag");
	_grayscale->link();
	_grayscale->updateUniforms();
	_grayscale->retain();

	_spritePercentStencil = GLProgram::createWithFilenames("res/shaders/stencil_percent.vert", "res/shaders/stencil_percent.frag");
	_spritePercentStencil->link();
	_spritePercentStencil->updateUniforms();
	_spritePercentStencil->retain();

	_glowSchedule = GLProgram::createWithFilenames("res/shaders/glow_threshold_schedule.vert", "res/shaders/glow_threshold_schedule.frag");
	_glowSchedule->link();
	_glowSchedule->updateUniforms();
	_glowSchedule->retain();

	_blur = GLProgram::createWithFilenames("res/shaders/blur.vert", "res/shaders/blur.frag");
	_blur->link();
	_blur->updateUniforms();
	_blur->retain();

	_contrast = GLProgram::createWithFilenames("res/shaders/basic_2d.vert", "res/shaders/contrast_2d.frag");
	_contrast->link();
	_contrast->updateUniforms();
	_contrast->retain();
}

MShaderCache::~MShaderCache()
{
}

MShaderCache * MShaderCache::_instance = nullptr;
MShaderCache * MShaderCache::getInstance()
{
	if (_instance == nullptr)
	{
		_instance = new MShaderCache();
	}
	return _instance;
}
