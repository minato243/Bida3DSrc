#ifndef RT3EnterRoomId_h_
#define RT3EnterRoomId_h_

#include "../base/UIBaseLayer.h"

class RT3EnterRoomId
	:public UIBaseLayer
{
public:
	enum TAG
	{
		btnZero		= 0,
		btnOne		= 1, 
		btnTwo		= 2, 
		btnThree	= 3, 
		btnFour		= 4, 
		btnFive		= 5, 
		btnSix		= 6,
		btnSeven	= 7, 
		btnEight	= 8, 
		btnNine		= 9,
		btnBackSpace = 10, 
		btnOk		= 11,
		btnBack		= 12
	};
	RT3EnterRoomId();
	~RT3EnterRoomId();

	CREATE_FUNC(RT3EnterRoomId);

	bool init();

	void initGUI();

	void initTag();

	void onButtonRelease(cocos2d::ui::Button* button, int id);

	void updateRoomId(int newCharacter);

	void backSpace();

	void updateRoomLabel();

	void joinRoom(int roomId);

protected:
private:

	cocos2d::ui::Text *_lbTitle;
	cocos2d::ui::Text *_lbRoomId;

	cocos2d::ui::Button *_btnZero;
	cocos2d::ui::Button *_btnOk;
	cocos2d::ui::Button *_btnBackSpace;
	cocos2d::ui::Button *_btnOne;
	cocos2d::ui::Button *_btnTwo;
	cocos2d::ui::Button *_btnThree;
	cocos2d::ui::Button *_btnFour;
	cocos2d::ui::Button *_btnFive;
	cocos2d::ui::Button *_btnSix;
	cocos2d::ui::Button *_btnbtnSeven;
	cocos2d::ui::Button *_btnbtnEight;
	cocos2d::ui::Button *_btnNine;

	int _roomId;

public:
	static const int MAX_ROOM_ID = 100000;
};
#endif