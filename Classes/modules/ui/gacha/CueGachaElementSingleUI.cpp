#include "CueGachaElementSingleUI.h"
#include "data/Resource.h"
#include "../../utils/MShaderCache.h"

USING_NS_CC;
using namespace cocos2d::ui;
using namespace std;

CueGachaElementSingleUI::CueGachaElementSingleUI()
	:CueGachaElementUI()
{
	_sprite = nullptr;
}

CueGachaElementSingleUI::~CueGachaElementSingleUI()
{

}

bool CueGachaElementSingleUI::init()
{
	if (!BaseLayer::init("CueGachaElementSingleUI"))
	{
		return false;
	}

	std::string filePath = Res::GUI::getCsbPathFromJson(Res::GUI::CueGachaElementUI);
	this->initWithBinaryFile(filePath);

	_size = getOriginalSize();

	initSprite();
	refresh();
	return true;
}

void CueGachaElementSingleUI::initSprite()
{
	_rt = RenderTexture::create(_size.width, _size.height, Texture2D::PixelFormat::RGBA8888);
	_rt->retain();
	updateRenderTexture();
}

CueGachaElementSingleUI* CueGachaElementSingleUI::create(cocos2d::Size size)
{
	auto element = new CueGachaElementSingleUI();
	element->_size = size;
	element->init();
	element->autorelease();
	return element;
}

void CueGachaElementSingleUI::setButtonInspectVisible(bool isVisible)
{
	_btnInspect->setVisible(isVisible);
	updateRenderTexture();
}

void CueGachaElementSingleUI::loadData(CueRateInBox *data)
{
	CueGachaElementUI::loadData(data);
	updateRenderTexture();
}

void CueGachaElementSingleUI::highlight(bool isHighlight)
{
	_sprite->getGLProgramState()->setUniformFloat("u_highlight", isHighlight ? 1 : 0);
}

CueRateInBox * CueGachaElementSingleUI::getData()
{
	return _data;
}

void CueGachaElementSingleUI::updateRenderTexture()
{
	_rt->beginWithClear(0, 0, 0, 0);
	for (int i = 0; i < _layout->getChildren().size(); i++)
	{
		_layout->getChildren().at(i)->visit();
	}
	_rt->end();

	if (!_sprite)
	{
		_sprite = Sprite::createWithTexture(_rt->getSprite()->getTexture());
		addChild(_sprite);

		_sprite->setAnchorPoint(Vec2(0.5, 1));
		_sprite->setPosition(0, _size.height / 2);
		_sprite->setFlippedY(true);

		auto state = GLProgramState::create(mShaderCache->_spriteUtils);
		_sprite->setGLProgramState(state);
		state->setUniformFloat("u_opacity", 1.0);

	}
}

void CueGachaElementSingleUI::setOpacity(int val)
{
	_sprite->getGLProgramState()->setUniformFloat("u_opacity", val * 1.f / 255);
	_cue3D->setOpacity(val);
}

cocos2d::Sprite * CueGachaElementSingleUI::getSprite()
{
	return _sprite;
}

void CueGachaElementSingleUI::scaleSpriteAndModel(float scale)
{
	_sprite->setScale(scale);
	_cue3D->setScale(_cueOriginalScale[_cue3D] * scale);

	auto ratio = _cueRatio[_cue3D];
	auto p = _cue3D->getPosition();

	p.x = ratio.x * _size.width * scale - _size.width * scale / 2;
	p.y = -((0.5 - ratio.y) * _size.height * scale);
	_cue3D->setPosition(p);
}

