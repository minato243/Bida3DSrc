#include "IngameMidFX.h"
#include "../../../data/Resource.h"

USING_NS_CC;
using namespace std;
using namespace cocos2d::ui;

IngameMidFX::IngameMidFX()
{
	_callback = NULL;
}

IngameMidFX::~IngameMidFX()
{

}

bool IngameMidFX::init()
{
	if (!UIBaseLayer::init())
		return false;

	std::string filePath = Res::GUI::getCsbPathFromJson(Res::GUI::IngameMidFXUI);
	this->initWithBinaryFile(filePath);

	initFX();
	return true;
}

void IngameMidFX::initGUI()
{
	autoInitComponent(_layout);

	_text_Image_0 = (Sprite*)_nodeMap["text_Image_0"];
	_text_Image_1 = (Sprite*)_nodeMap["text_Image_1"];

	_panelFX = _nodeMap["panelFX"];
}

void IngameMidFX::initFX()
{
	_fxMidGame = spine::SkeletonAnimation::createWithJsonFile(Res::FXMidGame + ".json", Res::FXMidGame+".atlas", 1);
	_panelFX->addChild(_fxMidGame);
	auto panelSize = _panelFX->getContentSize();
	_fxMidGame->setPosition(Vec2(panelSize.width/2, panelSize.height/2));
}

void IngameMidFX::loadData(int userScore, int enemyScore)
{
	char frameName[100];
	sprintf(frameName, "NumberIngame/text_score_blue_%d.png", userScore);

	char frameName1[100];
	sprintf(frameName1, "NumberIngame/text_score_red_%d.png", enemyScore);

	_text_Image_0->setSpriteFrame(frameName);
	_text_Image_1->setSpriteFrame(frameName1);
}

void IngameMidFX::runFX(int userScore, int enemyScore, int winnerIdx)
{
	_userScore = userScore;
	_enemyScore = enemyScore;
	Node *text = NULL;
	if (winnerIdx == 0)
	{
		loadData(userScore - 1, enemyScore);
		text = _text_Image_0;
	}
	else
	{
		loadData(userScore, enemyScore -1);
		text = _text_Image_1;
	}
	
	_fxMidGame->setAnimation(0, "animation", false);

	_text_Image_0->runAction(FadeIn::create(0.3));
	_text_Image_1->runAction(FadeIn::create(0.3));

	auto action = Sequence::create(
		DelayTime::create(1),
		EaseBackIn::create(ScaleTo::create(0.5, 1.4)),
		CCCallFunc::create(CC_CALLBACK_0(IngameMidFX::updateNewScore, this)),
		EaseSineInOut::create(ScaleTo::create(0.5, 1)),
		NULL
	);
	text->runAction(action);

	runAction(Sequence::create(
		DelayTime::create(2.5),
		CCCallFunc::create(CC_CALLBACK_0(IngameMidFX::onRunFXComplete, this)),
		NULL
	));
}

void IngameMidFX::updateNewScore()
{
	loadData(_userScore, _enemyScore);
}

void IngameMidFX::onRunFXComplete()
{
	hide();
	if (_callback != NULL)
	{
		_callback();
	}
}

void IngameMidFX::setCallback(CUSTOM_CALLBACK callback)
{
	_callback = callback;
}

