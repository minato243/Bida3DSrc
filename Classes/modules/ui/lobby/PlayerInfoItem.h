#ifndef PlayerInfoItem_h_
#define PlayerInfoItem_h_

#include "../base/UIBaseLayer.h"
#include "../../core/match/Player.h"

class PlayerInfoItem
	:public UIBaseLayer
{
public:
	enum TAG
	{
		btnKick,
		btnNode,
		btnAddFriend
	};
	PlayerInfoItem();
	~PlayerInfoItem();

	CREATE_FUNC(PlayerInfoItem);

	bool init();

	void initGUI();

	void initTag();

	void initAvatar();

	void loadData(const Player * player, bool isKey);

	void setUnknown();

	void setAvatar(const std::string & avatar);

	void setActive(bool isActive);

	void setSide(bool isLeftSide);

	void onButtonRelease(cocos2d::ui::Button* button, int id);

	void setDelegate(cocos2d::Node *delegate);

private:
	cocos2d::ui::Text *_lbName;
	cocos2d::Node *_nodeGold;
	cocos2d::ui::Text *_lbGold;
	cocos2d::Node *_nodeGlove;
	cocos2d::ui::Text *_lbGlove;
	cocos2d::ui::Text *_lbLevel;

	cocos2d::ui::Text *_lbInvite;

	cocos2d::ui::ImageView *_imgCrown;
	cocos2d::ui::ImageView *_imgUnknown;
	cocos2d::ui::ImageView *_imgCurrent;
	
	cocos2d::ui::Button *_btnAddFriend;
	cocos2d::ui::Button *_btnKick;
	cocos2d::ui::Button *_btnNode;

	cocos2d::Sprite *_avatar;

	cocos2d::ui::ImageView *_imgAvatarBorder;
	cocos2d::Sprite *_avatarMask;
	cocos2d::ui::ImageView *_imgLevel;

	cocos2d::Node *_mask;

	cocos2d::Node *_delegate;

	bool _isLeftSide;
	bool _active;

	int _userId;
	std::string _userName;
};

#endif