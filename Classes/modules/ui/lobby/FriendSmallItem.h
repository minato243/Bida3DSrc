#ifndef FriendSmallItem_h_
#define FriendSmallItem_h_

#include "../base/UIBaseLayer.h"
#include "RT3LobbyLayer.h"

class FriendSmallItem
	: public UIBaseLayer
{
public:
	FriendSmallItem();
	~FriendSmallItem();

	CREATE_FUNC(FriendSmallItem);

	bool init();

	void initGUI();

	void initTag();

	void onButtonRelease(cocos2d::ui::Button* button, int id);

	void loadData(FriendData *data);

	cocos2d::Size getOriginalSize();

protected:
private:
	cocos2d::ui::Widget *_bg;
};
#endif
