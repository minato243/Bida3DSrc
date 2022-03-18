#ifndef SelectModeLayer_h_
#define SelectModeLayer_h_

#include "../base/UIBaseLayer.h"
#include "ModeUIItem.h"

class SelectModeLayer
	: public UIBaseLayer
{
public:
	enum TAG
	{
		btnBack = 1001
	};
	SelectModeLayer();
	~SelectModeLayer();

	CREATE_FUNC(SelectModeLayer);

	bool init();

	void initGUI();

	void initTag();

	void onButtonRelease(cocos2d::ui::Button* button, int id);

	void initChannel();

	void onBtnTextClick(cocos2d::Ref *btn);

	void showSelectText(GameMode mode);

	void setBgFoot(GameMode mode);

	void showModeData(GameMode mode);

	void runTableAppear();

	void resetData();

	void tableCellTouched(cocos2d::extension::TableViewCell *cell);

	void joinChannel(GameMode mode, int channelId);

	void show();

	void hide();

	void updateUserInfo();

private:
	cocos2d::ui::Text *_lb_Mode_Detail;
	cocos2d::ui::Text *_text_Quick;
	cocos2d::ui::Text *_text_Streak;
	cocos2d::ui::Text *_text_RT2;
	cocos2d::ui::Text *_text_RT3;
	cocos2d::ui::Text *_text_FriendMode;
	cocos2d::ui::Text *_text_Practise;

	cocos2d::ui::Widget *_panelFoot;
	cocos2d::ui::Widget *_topPanel;

	cocos2d::ui::Button *_btnBack;
	cocos2d::ui::Text *_lbGold;
	cocos2d::ui::Text *_lbCash;

	static const int NUM_BTN_TEXT = 6;
	cocos2d::ui::Text *_listBtnText[NUM_BTN_TEXT];
	cocos2d::Sprite *_listLine[NUM_BTN_TEXT -1];
	cocos2d::Sprite *_bgChoose;

	cocos2d::Layer *_tableView;
	
	GameMode _lastMode;
	GameMode _curMode;
	Rule _ballRule;

	std::vector<SelectModeData *> _selectModeList;
	std::map<cocos2d::Node *, float> _dx;

public:
	static const int NUMBER_CHANNEL = 5;

};
#endif // !SelectModeLayer_h_
