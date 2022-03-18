#include "PlayerInfoItem.h"
#include "../../../data/Resource.h"
#include "../../../core/utils/StringUtility.h"
#include "../../utils/MShaderCache.h"
#include "../../../base/UIFactory.h"
#include "../../../core/utils/LanguageMgr.h"
#include "RT3LobbyLayer.h"

USING_NS_CC;
using namespace cocos2d::ui;
using namespace std;

PlayerInfoItem::PlayerInfoItem()
{
	_isLeftSide = true;
	_avatar = NULL;
}

PlayerInfoItem::~PlayerInfoItem()
{

}

bool PlayerInfoItem::init()
{
	if (!UIBaseLayer::init())
	{
		return false;
	}

	initWithBinaryFile(Res::GUI::getCsbPathFromJson(Res::GUI::PlayerInfoItem));
	initAvatar();
	
	return true;
}

void PlayerInfoItem::initGUI()
{
	initTag();
	autoInitComponent(_layout);

	_bg			= _nodeMap["bg"];
	_lbName		= (Text*)_nodeMap["lbName"];
	_nodeGold	= _nodeMap["nodeGold"];
	_lbGold		= (Text *)getControl("lbGold", _nodeGold);
	_nodeGlove	= _nodeMap["nodeGlove"];
	_lbGlove	= (Text *)getControl("lbGold", _nodeGlove);

	_lbInvite	= (Text *)_nodeMap["lbInvite"];

	_imgCrown	= (ImageView *)_nodeMap["imgCrown"];
	_imgUnknown = (ImageView *)_nodeMap["imgUnknown"];
	_imgCurrent = (ImageView *)_nodeMap["imgCurrent"];

	_imgAvatarBorder	= (ImageView *)_nodeMap["imgAvatarBorder"];
	_avatarMask			= (Sprite *)_nodeMap["avatarMask"];
	_imgLevel			= (ImageView *)_nodeMap["imgLevel"];

	_btnAddFriend		= (Button*)_nodeMap["btnAddFriend"];
	_btnKick			= (Button*)_nodeMap["btnKick"];
	_btnNode			= (Button*)_nodeMap["btnNode"];

	_lbInvite->ignoreContentAdaptWithSize(true);
	_lbInvite->setString(languageMgr->localizedText("text_touch_to_invite"));
}

void PlayerInfoItem::initTag()
{
	_tagMap.insert(pair<string, int>("btnKick", btnKick));
	_tagMap.insert(pair<string, int>("btnAddFriend", btnAddFriend));
	_tagMap.insert(pair<string, int>("btnNode", btnNode));
}

void PlayerInfoItem::initAvatar()
{
	auto node = _imgAvatarBorder;
	auto center = node->getParent()->convertToNodeSpace(node->convertToWorldSpace(_avatarMask->getPosition()));
	_avatarMask->setVisible(false);
	auto realAvatar = Sprite::create(Res::INGAME_AVATAR_FAKE_2);
	realAvatar->setAnchorPoint(Vec2(0.5, 0.5));
	realAvatar->setPosition(center);
	node->getParent()->addChild(realAvatar);
	_imgLevel->setLocalZOrder(2);
	node->setLocalZOrder(1);

	_imgCrown->setLocalZOrder(3);

	_mask = node;

	// gv.scaleToSize(realAvatar, node.width, node.height);

	auto state = GLProgramState::create(mShaderCache->_spriteUtils);
	realAvatar->setGLProgramState(state);
	state->setUniformFloat("u_grayscale", 0);
	state->setUniformFloat("u_opacity", 1);
	state->setUniformFloat("u_circle", 1);
	state->setUniformFloat("u_radius",_avatarMask->getContentSize().width / 2 * _avatarMask->getScaleX());
	state->setUniformFloat("u_width", realAvatar->getContentSize().width);
	state->setUniformFloat("u_height", realAvatar->getContentSize().height);
	_avatar = realAvatar;
}

void PlayerInfoItem::loadData(const Player * player, bool isKey)
{
	if (player == NULL)
	{
		setUnknown();
		return;
	}

	_userId = player->id;
	_userName = player->name;

	_lbInvite->setVisible(false);
	_lbName->setVisible(true);
	_lbName->setString(player->name);
	_nodeGold->setVisible(true);
	_lbGold->setString(StringUtility::standartNumber(player->gold));
	_nodeGlove->setVisible(true);
	_lbGlove->setString(StringUtility::standartNumber(player->glove));

	_imgCrown->setVisible(isKey);
	setAvatar(player->avatar);
	setActive(true);

}

void PlayerInfoItem::setUnknown()
{
	_avatar->setVisible(false);
	_imgUnknown->setVisible(true);
	setActive(false);
	_lbName->setVisible(false);
	_nodeGold->setVisible(false);
	_nodeGlove->setVisible(false);
	_imgCurrent->setVisible(false);
	_imgCrown->setVisible(false);
	_imgLevel->setVisible(false);

	_lbInvite->setVisible(_isLeftSide);
	_btnKick->setVisible(false);
}

void PlayerInfoItem::setAvatar(const std::string & avatar)
{
	_imgUnknown->setVisible(false);
	_avatar->setTexture(avatar);
	UIFactory::scaleToSize(_avatar, _mask->getContentSize().width, _mask->getContentSize().height);

	auto state = _avatar->getGLProgramState();
	state->setUniformFloat("u_width", _avatar->getContentSize().width * _avatar->getScaleX());
	state->setUniformFloat("u_height", _avatar->getContentSize().height * _avatar->getScaleY());
}

void PlayerInfoItem::setActive(bool isActive)
{
	_active = isActive;
}

void PlayerInfoItem::setSide(bool isLeftSide)
{
	_isLeftSide = isLeftSide;

	_btnAddFriend->setVisible(false);
	if (_isLeftSide)
	{
		auto bg = (ImageView *)_bg;
		bg->loadTexture("rt3_Avatar_Board.png", TextureResType::PLIST);
		_imgAvatarBorder->loadTexture("rt3_Avatar_Border.png", TextureResType::PLIST);
		_imgLevel->loadTexture("rt3_Level.png", TextureResType::PLIST);

		_nodeGold->setVisible(true);
		_nodeGlove->setVisible(true);
	}
	else
	{
		_lbInvite->setVisible(false);
		auto bg = (ImageView *)_bg;
		bg->loadTexture("rt3_Avatar_Board_Red.png", TextureResType::PLIST);
		_imgAvatarBorder->loadTexture("rt3_Avatar_Border_Red.png", TextureResType::PLIST);
		_imgLevel->loadTexture("rt3_Level_02.png", TextureResType::PLIST);

		_nodeGold->setVisible(true);
		_nodeGlove->setVisible(true);
	}
}

void PlayerInfoItem::onButtonRelease(cocos2d::ui::Button* button, int id)
{
	if (!_isLeftSide)
		return;
	auto rt3Lobby = dynamic_cast<RT3LobbyLayer*>(_delegate);
	if (!rt3Lobby)
		return;

	switch (id)
	{
	case TAG::btnKick:
	{
		CCLOG("PlayerInfoItem::onButtonRelease btnKick");
		rt3Lobby->onKickPlayer(_userId, _userName);
		break;
	}

	case TAG::btnNode:
	{
		CCLOG("PlayerInfoItem::onButtonRelease btnNode");
		rt3Lobby->showInviteFriendPopup();
		break;
	}

	case TAG::btnAddFriend:
	{
		CCLOG("PlayerInfoItem::onButtonRelease btnAddFriend");
		break;
	}
		
	}
}

void PlayerInfoItem::setDelegate(cocos2d::Node *delegate)
{
	_delegate = delegate;
}

