#ifndef UIFACTORY_H_
#define UIFACTORY_H_

#include <cocos2d.h>
#include <cocos/ui/CocosGUI.h>
#include <map>

#define uiFactory UIFactory::getInstance()
class UIFactory
{
public:
	static void scaleToSize(cocos2d::Node *view, float width, float height);
	static cocos2d::ui::Text *createLabelOutline(std::string text, std::string fontName, int fontSize, 
		cocos2d::Color3B color = cocos2d::Color3B::WHITE, cocos2d::Color3B outlineColor = cocos2d::Color3B::BLACK,
		int outlineSize = 2);

	static void normalNode(cocos2d::Node *node, std::vector<std::string> *exception = nullptr);

	static void grayNode(cocos2d::Node *node);

	static void normalSprite(cocos2d::Sprite *sprite);

	static void graySprite(cocos2d::Sprite *sprite);

	static void loadButtonTextures(cocos2d::ui::Button *btn, const std::string & texturePath, 
		cocos2d::ui::Widget::TextureResType type = cocos2d::ui::Widget::TextureResType::PLIST);

	static float getTextWidth(cocos2d::ui::Text *lb);

	static cocos2d::Size getScaleFactor();

private:
	std::map<cocos2d::Node *, cocos2d::Color4B> _originalColorMap;

	static UIFactory *_instance;

public:
	static UIFactory *getInstance();
};

#endif



