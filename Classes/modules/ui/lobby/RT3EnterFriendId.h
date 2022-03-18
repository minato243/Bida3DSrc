#ifndef RT3EnterFriendId_h_
#define RT3EnterFriendId_h_

#include "../base/UIBaseLayer.h"

class RT3EnterFriendId
	:public UIBaseLayer
{
public:
	enum TAG
	{
		btnSubmit,
		btnBack
	};

	RT3EnterFriendId();
	~RT3EnterFriendId();

	CREATE_FUNC(RT3EnterFriendId);
	bool init();

	void initGUI();

	void initTag();

	void onButtonRelease(cocos2d::ui::Button* button, int id);

	void inviteFriend();

protected:
private:
	cocos2d::ui::Button *_btnSubmit;
	cocos2d::ui::Button *_btnBack;
	cocos2d::ui::TextField *_UserId;
};
#endif
