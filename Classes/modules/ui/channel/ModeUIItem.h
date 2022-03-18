#ifndef ModeUIItem_h_
#define ModeUIItem_h_

#include "../base/UIBaseLayer.h"
#include "../elements/QuickCustomTableView.h"

struct SelectModeData
{
	int mode;
	int rule;
	int id;

	SelectModeData(int mode, int rule, int id)
	{
		this->mode = mode;
		this->rule = rule;
		this->id = id;
	}
};

class ModeUIItem
	:public UIBaseLayer
{
public:
	ModeUIItem();
	~ModeUIItem();

	CREATE_FUNC(ModeUIItem);

	bool init();

	virtual void initGUI() override;

	void loadData(SelectModeData* data);

	void updateGiftFee();

	void updateGiftReward();

	cocos2d::Size getOriginalSize();

	void showOut();

	void runShowOut(float delayTime);
	
	void runShowIn(float delayTime);

private:
	cocos2d::ui::ImageView *_bg_Channel;
	cocos2d::ui::ImageView *_bg_board;
	cocos2d::Sprite *_bg_Icon_Mode;
	cocos2d::Sprite *_icon_Mode;
	cocos2d::ui::Text *_lb_Gold_Reward;
	cocos2d::Node *_nodeFee;
	cocos2d::ui::Text *_lb_Fee;
	cocos2d::ui::Text *_lb_Fee_0;
	cocos2d::Sprite *_img_NameChannel;
	cocos2d::ui::Text *_lb_MutiElo;
	
	cocos2d::ui::ImageView *_imgLock;
	cocos2d::ui::Text *_lb_LockLevel;

	static const int NUM_CHANNEL = 5;
	static const std::string BG_CHANNEL[NUM_CHANNEL];
	static const std::string BG_FEE[NUM_CHANNEL];

public:
	int _curMode;
	int _ballRule;
	int _channelId;

	static const std::string ICON_MODE[5];
	static const std::string NAME_MODE[5];
	static const std::string RULE[2][5];
	static const int RULE_8 = 0;
	static const int RULE_9 = 1;
	static const std::string NAME_CHANNEL[NUM_CHANNEL];
};
#endif // !ModeUIItem_h_
