#include "UIFactory.h"
#include <modules/utils/MShaderCache.h>


USING_NS_CC;
using namespace cocos2d::ui;

void UIFactory::scaleToSize(cocos2d::Node *view, float w, float h)
{
	auto size = view->getContentSize();
	auto scale = w / size.width < h / size.height ? w / size.width : h / size.height;
	view->setScale(scale);
}

Text * UIFactory::createLabelOutline(std::string text, std::string fontName, int fontSize, cocos2d::Color3B color, cocos2d::Color3B outlineColor, int outlineSize)
{
	auto label = Text::create(text, fontName, fontSize);

	label->setColor(color);	
	Color4B outlineColor4B = Color4B(outlineColor);
	label->enableOutline(outlineColor4B, outlineSize);
	
	return label;
}

void UIFactory::normalNode(cocos2d::Node * node, std::vector<std::string>* exception)
{
	auto children = node->getChildren();
	//if (exception.indexOf(node) >= 0) {
	//	return;
	//}

	if (dynamic_cast<Sprite *>(node)) 
	{
		normalSprite((Sprite*)node);
	} 
	else if (dynamic_cast<ImageView *>(node)) 
	{
		ImageView *img = (ImageView*)node;
		img->setBright(true);
	}
	else if (dynamic_cast<Button *>(node))
	{
		Button *btn = (Button *)node;
		btn->setBright(true);
	}
	else if (dynamic_cast<Text *>(node))
	{
		Text* text = (Text *)node;
		Color4B originalColor = uiFactory->_originalColorMap.at(node);
		CCLOG("UIFactory::grayNode node = %d", node);
		text->setTextColor(originalColor);
	}
	
	for (int i = 0; i < children.size(); i++) {
		normalNode(children.at(i), exception);
	}
}

void UIFactory::grayNode(cocos2d::Node * node)
{
	auto children = node->getChildren();

	if (dynamic_cast<Sprite *>(node))
	{
		grayNode((Sprite*)node);
	}
	else if (dynamic_cast<ImageView *>(node))
	{
		ImageView *img = (ImageView*)node;
		img->setBright(false);
	}
	else if (dynamic_cast<Button *>(node))
	{
		Button *btn = (Button *)node;
		btn->setBright(false);
	}
	else if (dynamic_cast<Text *>(node))
	{
		Text* text = (Text *)node;
		Color4B orignalColor = text->getTextColor();
		CCLOG("UIFactory::grayNode node = %d", node);
		getInstance()->_originalColorMap.insert(std::pair<Node *, Color4B>(node, orignalColor));
		float gray = 0.299 * orignalColor.r + 0.587 * orignalColor.g + 0.114 * orignalColor.b;
		text->setTextColor(Color4B(gray, gray, gray, 255));
	}

	// change color children
	for (int i = 0; i < children.size(); i++) {
		grayNode(children.at(i));
	}
}

void UIFactory::normalSprite(cocos2d::Sprite * sprite)
{
	auto state = GLProgramState::create(mShaderCache->_grayscale);
	sprite->setGLProgramState(state);
	state->setUniformFloat("u_grayscale", 0);
}

void UIFactory::graySprite(cocos2d::Sprite * sprite)
{
	auto state = GLProgramState::create(mShaderCache->_grayscale);
	sprite->setGLProgramState(state);
	state->setUniformFloat("u_grayscale", 1);
}

void UIFactory::loadButtonTextures(cocos2d::ui::Button *btn, const std::string & texturePath, cocos2d::ui::Widget::TextureResType type /*= cocos2d::ui::Widget::TextureResType::PLIST*/)
{
	btn->loadTextures(texturePath, texturePath, texturePath, type);
}

float UIFactory::getTextWidth(cocos2d::ui::Text *lb)
{
	auto tmpLb = cocos2d::ui::Text::create(lb->getString(), lb->getFontName(), lb->getFontSize());
	return tmpLb->getContentSize().width;
}

cocos2d::Size UIFactory::getScaleFactor()
{
	auto director = Director::getInstance();
	auto frameSize = director->getOpenGLView()->getFrameSize();
	auto winSize = director->getWinSize();
	auto scaleFactor = Size(frameSize.width / winSize.width, frameSize.height / winSize.height);
	return scaleFactor;
}

UIFactory *UIFactory::_instance = nullptr;
UIFactory *UIFactory::getInstance()
{
	if (_instance == nullptr) {
		_instance = new UIFactory();
	}
	return _instance;
}

