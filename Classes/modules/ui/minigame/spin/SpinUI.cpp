#include "SpinUI.h"
#include "../../../../data/Resource.h"
#include "../../../core/minigame/spin/SpinMgr.h"
#include "../../../../core/utils/StringUtility.h"
#include "../../GuiMgr.h"
#include "../../../scene/GameScene.h"
#include "../../../../core/GameMgr.h"
#include "../../../../core/gui/SceneMgr.h"
#include "base/CCRef.h"

USING_NS_CC;
using namespace cocos2d::ui;
using namespace std;

SpinUI::SpinUI()
{
	_angle = 0;
	_angleSpeed = 10;
	_dAngleSpeed = 0;

	_backLightOpacity = 255.f;
	_backLightScale = 1.f;
	_dBackLightOpacity = 0.5f;
	_dBackLightScale = -0.003f;

	_isRotating = false;
}

SpinUI::~SpinUI()
{

}

bool SpinUI::init()
{
	if (!UIBaseLayer::init("SpinUI"))
		return false;

	std::string filePath = Res::GUI::getCsbPathFromJson(Res::GUI::SpinUI);
	this->initWithBinaryFile(filePath);

	setFog(true, 200);
	return true;
}

void SpinUI::initGUI()
{
	initTag();
	autoInitComponent(_layout);

	_bg				= _nodeMap["bg"];
	_imgWheel		= (ImageView *)_nodeMap["imgWheel"];

	_wheelNode		= _nodeMap["wheelNode"];
	_imgHighlight	= (Sprite*)_nodeMap["imgHighlight"];
	_imgLight1		= (Sprite *)_nodeMap["imgLight1"];
	_lbSpinText		= (Text*)_nodeMap["lbSpinText"];

	_rewardNode		= _nodeMap["rewardNode"];
	_lbGoldReward	= (Text *)_nodeMap["lbGoldAmount"];
	_rewardNode->setOpacity(0);
	getControl("img", _rewardNode)->setLocalZOrder(2);
	getControl("lbGoldAmount", _rewardNode)->setLocalZOrder(2);
	getControl("imgStar0", _rewardNode)->setLocalZOrder(3);
	getControl("imgStar1", _rewardNode)->setLocalZOrder(3);
	getControl("imgStar2", _rewardNode)->setLocalZOrder(3);

	_lbNotice2		= (Text *)_nodeMap["lbNotice2"];
	_lbNotice3		= (Text *)_nodeMap["lbNotice3"];

	_imgBackLight	= (Sprite *)_nodeMap["imgBackLight"];
}

void SpinUI::initTag()
{
	_tagMap["btnSpin"] = btnSpin;
	_tagMap["btnClose"] = btnClose;
}

void SpinUI::onButtonTouched(cocos2d::ui::Button* button, int id)
{
	switch (id)
	{
	case btnClose:
	
		break;
	default:
		break;
	}
}

void SpinUI::onButtonRelease(cocos2d::ui::Button* button, int id)
{
	switch (id)
	{
	case SpinUI::btnSpin:
		CCLOG("SpinUI::onButtonRelease btnSpin");
		spinWheel();
		break;
	case SpinUI::btnClose:
		hide();
		break;
	default:
		break;
	}
}

void SpinUI::loadData()
{
	char text[20];
	auto radius = _imgWheel->getContentSize().width / 2;
	
	for (auto i = 0; i < SpinConstant::N_CELL; i++)
	{
		sprintf(text, "lbCell%d", i);
		auto lbCell = (Text *)getControl(text, _imgWheel);
		auto gold = spinMgr->getCellById(i).gold;
		lbCell->setString(StringUtility::formatNumberSymbol(gold));

		auto alpha = i * 360.f / SpinConstant::N_CELL / 180 * M_PI;
		Vec2 pos;
		pos.x = radius * 0.7 * std::sin(alpha) + radius;
		pos.y = radius * 0.7 * std::cos(alpha) + radius;
		lbCell->setPosition(pos);
		lbCell->setRotation(i * 360.f / SpinConstant::N_CELL - 90);
	}

	_lbNotice2->setString(to_string(spinMgr->getNumberOfSpin()));
}

void SpinUI::spinWheel()
{
	if (_isRotating)
	{
		guiMgr->notifyText("The wheel is rotating.");
		return;
	}

	if (spinMgr->getNumberOfSpin() <= 0)
	{
		guiMgr->notifyText("Out of spin");
		return;
	}

	_isRotating = true;
	spinMgr->sendSpinWheel();
}

void SpinUI::spinWheelToCellId(int targetId)
{
	_targetCellId = targetId;
	auto targetAngle = -spinMgr->getBisectorAngleByCellId(targetId) + SpinConstant::POINTER_ANGLE + SpinConstant::CELL_ANGLE/2;
	auto totalRotatingDegree = SpinConstant::N_TURNING_ROUND * 360 + targetAngle;
	_isRotating = true;
	_angle = 0;
	_angleSpeed = SpinConstant::BASE_ANGLE_SPEED;
	_dAngleSpeed = _angleSpeed * _angleSpeed / (2 * totalRotatingDegree);

	_lbSpinText->runAction(FadeOut::create(0.1));
	_lbNotice2->runAction(
		Sequence::create(
			ScaleTo::create(0.1, 1.4),
			ScaleTo::create(0.1, 1.0),
			NULL
		)
	);
	_lbNotice3->runAction(
		Sequence::create(
			ScaleTo::create(0.1, 1.4),
			ScaleTo::create(0.1, 1.0),
			NULL
		)
	);
}

void SpinUI::updateAll(float dt)
{
	auto dAngle = _angleSpeed * dt - _dAngleSpeed * dt *dt / 2;
	_angle += max(dAngle, 0.f);

	if (_angleSpeed > _dAngleSpeed * dt + 1e-2)
	{
		_angleSpeed -= _dAngleSpeed * dt;

		if (_angleSpeed < _dAngleSpeed *dt + 1e-2)
		{
			onRotatingFinished();
		}
	}
	updateComponent();
}

void SpinUI::show()
{
	UIBaseLayer::show();
	loadData();

	_bg->runAction(EaseBackOut::create(ScaleTo::create(0.2, 1)));
	_imgHighlight->setOpacity(0);
	_lbSpinText->stopAllActions();
	_lbSpinText->runAction(
		RepeatForever::create(
			Sequence::create(
				ScaleTo::create(0.5, 1.2),
				ScaleTo::create(0.5, 1),
				NULL
			)
		)
	);
}

void SpinUI::updateComponent()
{
	_backLightOpacity += _dBackLightOpacity;
	_backLightScale += _dBackLightScale;
	if (_backLightOpacity >= 255 || _backLightOpacity <= 200)
	{
		_dBackLightOpacity = -_dBackLightOpacity;
	}

	if (_backLightScale >= 1 || _backLightScale <= 0.9)
	{
		_dBackLightScale = -_dBackLightScale;
	}
	
	_imgWheel->setRotation(_angle);
	_imgBackLight->setOpacity((int)_backLightOpacity);
	_imgBackLight->setScale(_backLightScale);

	updateNumberOfSpin();
}

void SpinUI::onRotatingFinished()
{
	_angleSpeed = 0;
	auto cell = spinMgr->getCellById(_targetCellId);

	auto targetAngle = -spinMgr->getBisectorAngleByCellId(_targetCellId) + SpinConstant::POINTER_ANGLE + SpinConstant::CELL_ANGLE / 2;
	_angle = targetAngle;
	_targetCellId = -1;

	_imgLight1->runAction(RotateBy::create(8, 720));
	_lbGoldReward->setString(StringUtility::standartNumber(cell.gold));

	_imgHighlight->runAction(
		Sequence::create(
			Repeat::create(
				Sequence::create(
					FadeIn::create(0.1),
					FadeOut::create(0.1),
					NULL
				),
				3
			),
			FadeIn::create(0.1),
			NULL
		)
	);

	auto action = Sequence::create(
		DelayTime::create(0.4),
		CallFunc::create(CC_CALLBACK_0(SpinUI::runAnimReward, this)),
		CallFunc::create(CC_CALLBACK_0(SpinUI::runAnimShowWheel, this)),
		NULL
	);
	runAction(action);
	gameMgr->_userInfo->addGold(cell.gold);
}

void SpinUI::runAnimReward()
{
	for (auto i = 0; i < 3; i++) {
		char text[20];
		sprintf(text, "imgStar%d", i);
		auto node = getControl(text, _rewardNode);
		node->runAction(RotateBy::create(8, randomAngle(300, 700)));
		node->runAction(
			Repeat::create(
				Sequence::create(
					FadeIn::create(randomAngle(0.3, 0.7)),
					FadeOut::create(randomAngle(0.3, 0.7)),
					NULL
				),
				8
			)
		);
	}
	
	_wheelNode->runAction(TintTo::create(0.1, Color3B(100, 100, 100)));
	createMovingCoins();

	_rewardNode->setScale(0);
	_rewardNode->setOpacity(0);
	_rewardNode->runAction(FadeIn::create(0.2));
	_rewardNode->runAction(
		Sequence::create(
			EaseBackOut::create(ScaleTo::create(0.45, 1)),
			DelayTime::create(2.1),
			FadeOut::create(0.2),
			CallFunc::create(CC_CALLBACK_0(SpinUI::onAnimRewardComplete, this)),
			NULL
		)
	);
}

void SpinUI::runAnimShowWheel()
{
	_wheelNode->runAction(TintTo::create(0.2, Color3B(255, 255, 255)));
	_lbSpinText->runAction(
		Sequence::create(
			DelayTime::create(0.5),
			FadeIn::create(0.3),
			NULL
		)
	);
}

void SpinUI::createMovingCoins()
{
	auto nCoin = SpinConstant::N_REWARD_COIN;
	auto rewardPos = getControl("img", _rewardNode)->convertToWorldSpace(Vec2(0,0));
	auto startPos = Vec2(0, 0);
	auto endPos = goldPanel()->convertToWorldSpace(Vec2(0,0));
	endPos.x += -rewardPos.x - goldPanel()->getContentSize().width * 3 / 2;
	endPos.y += -rewardPos.y - goldPanel()->getContentSize().height * 3 / 2;

	for (auto i = 1; i <= nCoin; i++)
	{
		for (auto j = 1; j <= 5; j++)
		{
			auto coin = Sprite::createWithSpriteFrameName("Icon_Gold.png");
			coin->setLocalZOrder(1);
			_rewardNode->addChild(coin);
			auto randPoint = Vec2(0, 0);
			randPoint.x = randomAngle(startPos.x - 200, endPos.x + 200);
			randPoint.y = randomAngle(startPos.y - 200, endPos.y + 200);
			ccBezierConfig bezier;
			bezier.controlPoint_1 = Vec2(startPos.x + rand() % 40 - 20, startPos.y + rand() % 40 - 20);
			bezier.controlPoint_2 = randPoint;
			bezier.endPosition = Vec2(endPos.x + rand() % 40 - 20, endPos.y + rand() % 40 - 20);
			coin->setPosition(startPos);
			coin->runAction(Sequence::create(
				DelayTime::create(i * 6.f / 60),
				ScaleTo::create(1.45, 0.8),
				NULL
			));
			coin->runAction(Sequence::create(
				DelayTime::create(i * 6.f / 60),
				FadeIn::create(0.1),
				EaseOut::create(BezierTo::create(0.9, bezier), 1),
				FadeOut::create(0.15),
				CallFuncN::create([this](Node *node) {
					node->removeFromParent();
				}),
				NULL
			));
		}
	}
}

void SpinUI::onAnimRewardComplete()
{
	_imgHighlight->runAction(FadeOut::create(0.1));
	_isRotating = false;
}

void SpinUI::updateNumberOfSpin()
{
	auto nSpin = spinMgr->getNumberOfSpin();
	_lbNotice2->setString(to_string(nSpin));
}

void SpinUI::onEnter()
{
	UIBaseLayer::onEnter();
	schedule(schedule_selector(SpinUI::updateAll), 1/60.f);
}

void SpinUI::onExit()
{
	unschedule(schedule_selector(SpinUI::updateAll));
	UIBaseLayer::onExit();
}

float SpinUI::randomAngle(float s, float e)
{
	float r = static_cast<float>(rand()) / static_cast<float>(RAND_MAX);
	return s + r *(e-s);
}

cocos2d::Node * SpinUI::goldPanel()
{
	auto gameScene = dynamic_cast<GameScene *> (sceneMgr->getRunningScene());
	if (gameScene)
	{
		auto lobbyUI = gameScene->_lobbyUI;
		auto goldIcon = lobbyUI->_topPanel;
		return goldIcon;
	}
	return NULL;
}

