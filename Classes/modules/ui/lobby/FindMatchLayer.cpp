#include "FindMatchLayer.h"
#include <data/Resource.h>
#include <base/UIFactory.h>
#include <core/utils/LanguageMgr.h>
#include "../../network/match/MatchHandler.h"
#include "core/gui/SceneMgr.h"
#include "../../scene/GameScene.h"
#include "../../utils/SoundMgr.h"

USING_NS_CC;
using namespace cocos2d::ui;

FindMatchLayer::FindMatchLayer()
{
	_timeElapsed = -1;
	_keep = true;
	_numDot = 0;
}

FindMatchLayer::~FindMatchLayer()
{
}

bool FindMatchLayer::init()
{
	if (!BaseLayer::init())
	{
		return false;
	}

	initWithBinaryFile(Res::GUI::getCsbPathFromJson(Res::GUI::FindMatch));

	return true;
}

void FindMatchLayer::initGUI()
{
	autoInitComponent(_layout);
	_bg = (ImageView *)getControl("bg", _layout);

	_nodeP1 = getControl("nodeP1", _bg);
	_nodeP2 = getControl("nodeP2", _bg);

	_btnCancel = customButton("btnCancel", TAG::BTN_CANCEL, _bg);
	
	_lbMatching = (Text *)getControl("lbMatching", _bg);
	_lbTime = (Text *)getControl("lbTime", _bg);
	_lbEstimate = (Text *)getControl("lbEstimate", _bg);
	_lbCCU = (Text *)getControl("lbCCU", _bg);
	_lbSubtitle = (Text *)getControl("lbSubtitle", _bg);
	_lbBtnCancel = (Text *)getControl("lb", _btnCancel);

	_lbBtnCancel->setString(languageMgr->localizedText("text_cancel_matching"));
	_lbMatching->setString(languageMgr->localizedText("text_matching"));

	_iconCircle_0 = (cocos2d::Sprite *) _nodeMap["iconCircle_0"];
	_iconCircle_1 = (cocos2d::Sprite *) _nodeMap["iconCircle_1"];
}

void FindMatchLayer::onEnter()
{
	BaseLayer::onEnter();
}

void FindMatchLayer::startTimer()
{
	_timeElapsed = 0;
	setPlayer2InfoVisible(false);
	schedule(schedule_selector(FindMatchLayer::updateTimer), 0.5f);
}

void FindMatchLayer::stopTimer()
{
	_timeElapsed = -1;
	unschedule(schedule_selector(FindMatchLayer::updateTimer));
}

void FindMatchLayer::updateTimer(float dt)
{
	_timeElapsed += dt;
	auto h = (int)(_timeElapsed / 3600);
	auto m = (int)((_timeElapsed - 3600 * h) / 60);
	auto s = (int)(_timeElapsed - 3600 * h - 60 * m);

	char hText[5], mText[5], sText[5];
	if (h == 0) sprintf(hText, "");
	else if (h < 10) sprintf(hText, "0%d:", h);
	else sprintf(hText, "%d:", h);

	if (m < 10) sprintf(mText, "0%d:", m);
	else sprintf(mText, "%d:", m);
	
	if (s < 10) sprintf(sText, "0%d", s);
	else sprintf(sText, "%d", s);

	char timeText[20];
	sprintf(timeText, "%s%s%s", hText, mText, sText);
	//TODO: string formatter
	_lbTime->setString(timeText);

	_numDot ++;
	if (_numDot > 3) _numDot = 0;

	std::string text = "";
	for (int i = 0; i < _numDot; i++)
	{
		text += ".";
	}
	_lbMatching->setString(languageMgr->localizedText("text_matching") + text);
}

void FindMatchLayer::setPlayer2InfoVisible(bool isVisible)
{
	//auto node = _nodeP2;
	//node->getChildByName("lbName")->setVisible(isVisible);
	//node->getChildByName("nodeAvatar")->setVisible(false);
	//if (node->getChildByName("avatar"))
	//	node->getChildByName("avatar")->setVisible(isVisible);
	//node->getChildByName("trophyBox")->setVisible(isVisible);
	//// node._lbMoney.setVisible(val);

	//_lbTime->setVisible(!isVisible);
	//_lbMatching->setVisible(!isVisible);
}

void FindMatchLayer::onFoundOpponent(const Player & p2Data)
{
	_foundOpponent = true;
	// for(let i=0;i<_avatarsScrolling.length;i++){
	//     _avatarsScrolling[i].removeFromParent();
	// }
	// _avatarsScrolling.length = 0;
	stopTimer();
	setPlayer2InfoVisible(true);

	setPlayerInfo(1, p2Data);

}

void FindMatchLayer::setInfo(GameMode mode, int ccu)
{
	std::string modeName = "";
	switch (mode)
	{
	case RT1:
		modeName = languageMgr->localizedText("game_mode_RT1");
		break;
	case STREAK_MODE:
		modeName = languageMgr->localizedText("game_mode_RT1");
		break;
	case RT2_MODE:
		modeName = languageMgr->localizedText("game_mode_RT1");
		break;
	case RT3_MODE:
		modeName = languageMgr->localizedText("game_mode_RT1");
		break;
	default:
		break;
	}

	_lbSubtitle->setString(modeName);

	if(ccu == -1)
		_lbCCU->setString("CCU: ?");
	else
	{
		char text[10];
		sprintf(text, "CCU: %d", ccu);
		_lbCCU->setString(text);
	}
}

void FindMatchLayer::setPlayerInfo(int idx, const Player & data)
{
	if (idx != 0 && idx != 1)
		return;

	//playerData[idx] = data;
	//Node *node;
	//if (idx == 0) node = _nodeP1;
	//else node = _nodeP2;
	//((Text*)node->getChildByName("lbName"))->setString(data.name);
	//// node._lbMoney.setString(data.gold);
	//std::string avatar = data.avatar;
	//if(data.avatar.compare("") == 0 ) avatar = Res::INGAME_AVATAR_FAKE_1;

	//setPlayerAvatar(idx, data.avatar);

	//node->getChildByName("nodeAvatar")->setVisible(false);
}

void FindMatchLayer::onButtonRelease(cocos2d::ui::Button * button, int id)
{
	CCLOG("FindMatchLayer::onButtonRelease");
	switch (id)
	{
	case TAG::BTN_CANCEL:
	{
		matchHandler->requestCancelMatching();
		stopTimer();
		hide();
		auto gameScene = (GameScene *)sceneMgr->getRunningScene();
		gameScene->_lobbyUI->show();
		
		break;
	}
	default:
		break;
	}
}

void FindMatchLayer::setPlayerAvatar(int idx, std::string avatarTexture)
{
	/*Node *node;
	if (idx == 0) node = _nodeP1;
	else node = _nodeP2;

	if (node->getChildByName("clippingNode") == nullptr) 
	{
		initAvatarClippingNode(node);
	}
	auto avatarNode = node->getChildByName("clippingNode")->getChildByName("avatar");
	((Sprite *)avatarNode)->setTexture(avatarTexture);
	Size avatarSize = node->getChildByName("imgAvatarBorder")->getContentSize();
	UIFactory::scaleToSize(avatarNode, avatarSize.width, avatarSize.height);*/
}

void FindMatchLayer::initAvatarClippingNode(cocos2d::Node * node)
{
	auto avatar = Sprite::create();
	auto nodeAvatar = ClippingNode::create();
	nodeAvatar->setPosition(node->getChildByName("nodeAvatar")->getPosition());
	auto stencil = node->getChildByName("nodeAvatar")->getChildByName("imgAvatarMask");
	//stencil->removeFromParent();
	// stencil.setPosition(nodeAvatar.getPosition());
	auto imgAvatarBorder = node->getChildByName("imgAvatarBorder");
	Size size = imgAvatarBorder->getContentSize();
	UIFactory::scaleToSize(stencil, size.width*0.87*imgAvatarBorder->getScaleX(), size.height*0.87*imgAvatarBorder->getScaleY());
	nodeAvatar->setStencil(stencil);
	nodeAvatar->addChild(avatar);
	nodeAvatar->setAlphaThreshold(0.1);
	node->addChild(nodeAvatar);
	
	nodeAvatar->setName("clippingNode");
	avatar->setName("avatar");
}

void FindMatchLayer::reset()
{
	startTimer();
	_foundOpponent = false;
	std::vector<std::string> imgList = std::vector<std::string>{ 
			Res::INGAME_AVATAR_FAKE_1, 
			Res::INGAME_AVATAR_FAKE_2, 
			Res::INGAME_AVATAR_FAKE_1, 
			Res::INGAME_AVATAR_FAKE_2 
		};
	startScrollingAvatarEffect(imgList);
}

void FindMatchLayer::startScrollingAvatarEffect(std::vector<std::string>& imgList)
{
	/*_avatarsScrolling = [];
	return;

	const nodeP2 = _bg._nodeP2;
	if (!nodeP2._clippingNode) {
		initAvatarClippingNode(nodeP2);
	}

	const height = nodeP2._nodeAvatar._imgAvatarMask.height;
	const ref = this;
	const trail = 3;
	for (let i = 0; i < imgList.length; i++) {
		let clippingNode = new cc.ClippingNode();
		clippingNode.setPosition(0, -height / 2 + i * height * 1.3);
		let stencil = new cc.Sprite(nodeP2._nodeAvatar._imgAvatarMask.getTexture());
		stencil.setAnchorPoint(0.5, 0);
		stencil.setScale(nodeP2._clippingNode.getStencil().getScaleX(), nodeP2._clippingNode.getStencil().getScaleY());
		clippingNode.setAlphaThreshold(0.1);
		clippingNode.setStencil(stencil);

		const sprite = new cc.Sprite(imgList[i]);
		let s = sprite;
		_avatarsScrolling.push(clippingNode);
		nodeP2._clippingNode.addChild(clippingNode, -i * 2);

		const update = function(dt) {
			if (ref._foundOpponent) return;
			y -= 10 * height * 1.3 * dt;
			if (y < -height * 1.5) {
				y = height * 1.3 *(imgList.length - 1);
			}
		};

		sprite.setAnchorPoint(0.5, 0);
		gv.scaleToSize(sprite, nodeP2._imgAvatarBorder.width, nodeP2._imgAvatarBorder.height);
		clippingNode.addChild(sprite);
		clippingNode.scheduleUpdate();
		clippingNode.update = update;
		for (let j = 0; j < trail; j++) {
			s.trail = new cc.Sprite(imgList[i]);
			s.trail.setOpacity(0.6*(trail - j) / trail * 255);
			s.trail.setAnchorPoint(0.5, 0);
			gv.scaleToSize(s.trail, nodeP2._imgAvatarBorder.width, nodeP2._imgAvatarBorder.height);

			let clippingNode = new cc.ClippingNode();
			clippingNode.setPosition(0, -height / 2 + i * height * 1.3 + j * height * 1.3 / trail);
			let stencil = new cc.Sprite(nodeP2._nodeAvatar._imgAvatarMask.getTexture());
			stencil.setAnchorPoint(0.5, 0);
			stencil.setScale(nodeP2._clippingNode.getStencil().getScaleX(), nodeP2._clippingNode.getStencil().getScaleY());
			clippingNode.setStencil(stencil);
			clippingNode.setAlphaThreshold(0.1);

			clippingNode.addChild(s.trail);

			clippingNode.scheduleUpdate();
			clippingNode.update = update;
			_avatarsScrolling.push(clippingNode);

			s = s.trail;
			nodeP2._clippingNode.addChild(clippingNode, -i * 2 - 1);
		}
	}*/
}

void FindMatchLayer::show()
{
	BaseLayer::show();

	_bg->setOpacity(0);
	_bg->runAction(FadeIn::create(0.5));

	_iconCircle_0->stopAllActions();
	_iconCircle_1->stopAllActions();

	_iconCircle_0->runAction(RepeatForever::create(RotateBy::create(2,30)));
	_iconCircle_1->runAction(RepeatForever::create(RotateBy::create(2,-30)));

	soundMgr->playBgSearchingOpponent();
}

