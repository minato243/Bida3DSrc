#include "LoadingGui.h"
#include "SceneMgr.h"

USING_NS_CC;
using namespace cocos2d::ui;
LoadingGui::LoadingGui()
{

}

LoadingGui::~LoadingGui()
{

}

LoadingGui* LoadingGui::create(std::string text, bool fog)
{
	LoadingGui* loading = new LoadingGui();
	loading->init(text, fog);
	loading->autorelease();
	return loading;
}

bool LoadingGui::init(std::string text, bool fog)
{
	Layer::init();

	_layerColor = nullptr;
	_message = text;
	_fog = fog;

	return true;
}

void LoadingGui::timeout(float time)
{
	runAction(Sequence::create(DelayTime::create(time), RemoveSelf::create(), nullptr));
}

void LoadingGui::onEnter()
{
	Layer::onEnter();

	if (_fog) {
		_layerColor = LayerColor::create(Color4B::BLACK);
		_layerColor->runAction(FadeTo::create(0.25f, 150));
		addChild(_layerColor);
	}

	Size size = Director::getInstance()->getWinSize();

	commonbg = Sprite::create("common/common_loading_icon.png");
	addChild(commonbg);
	commonbg->setPosition(Vec2(size.width / 2, size.height / 2 + 15));
	commonbg->setScale(0.75f);

	commoncircel = Sprite::create("common/common_loading_circle.png");
	addChild(commoncircel);
	commoncircel->setPosition(Vec2(size.width / 2, size.height / 2 + 15));
	commoncircel->setScale(0.75f);
	commoncircel->runAction(RepeatForever::create(RotateBy::create(2.5f, 360)));


	float scale = size.width / 800;
	scale = (scale > 1) ? 1 : scale;

	_label = Text::create();
	_label->setAnchorPoint(Vec2(0.5f, 0.5f));
	_label->setFontName("fonts/tahoma.ttf");
	_label->setFontSize(SceneMgr::FONT_SIZE_DEFAULT);
	_label->setTextHorizontalAlignment(TextHAlignment::CENTER);
	_label->setColor(sceneMgr->ccWhite);
	_label->setString(_message);
	_label->setScale(scale);
	_label->setPosition(Vec2(size.width / 2, size.height / 2 - 50));
	addChild(_label);

	if (_fog) {


		EventListenerTouchOneByOne* listener = EventListenerTouchOneByOne::create();
		listener->setSwallowTouches(true);

		listener->onTouchBegan = [this](cocos2d::Touch* touch, cocos2d::Event* event) {
			return true;
		};

		Director::getInstance()->getEventDispatcher()->addEventListenerWithSceneGraphPriority(listener, this);
	}

}

void LoadingGui::remove()
{
	if (_layerColor) {
		_layerColor->runAction(FadeTo::create(0.2, 0));
	}
	runAction(Sequence::create(DelayTime::create(0.2f), RemoveSelf::create(), nullptr));
}

