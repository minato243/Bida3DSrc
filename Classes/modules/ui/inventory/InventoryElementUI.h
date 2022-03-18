#ifndef InventoryElementUI_h_
#define InventoryElementUI_h_

#include <cocos2d.h>
#include <core/gui/BaseLayer.h>
#include "../../../data/DataStruct.h"
#include "../../core/inventory/CueInfo.h"

struct ElementTypeFrameData 
{
	std::string icon;
	std::string background;
	cocos2d::Color3B color;

	ElementTypeFrameData(const std::string & icon, const std::string & background, const cocos2d::Color3B & color)
	{
		this->icon = icon;
		this->background = background;
		this->color = color;
	}
};

class Battery;

class InventoryElementUI
	: public BaseLayer
{
public:
	typedef CueInfo ItemData;

	enum TAG
	{
		BTN_ITEM,
		BTN_SWITCH,
		BTN_AUTO_CHARGE
	};

	InventoryElementUI();
	~InventoryElementUI();

	CREATE_FUNC(InventoryElementUI);
	bool init();

	static InventoryElementUI *create(const cocos2d::Size & size);

	cocos2d::Size getOriginalSize();

	void scaleToSize(const cocos2d::Size & size);

	void initGUI();

	void initTagMap();

	void initBattery();

	void loadData(ItemData *data);

	void setAutoRecharge(bool isAuto);

	void setContainerRect(cocos2d::Rect rect);

	void addCue3D(std::string model, std::string texture);

	void setBattery(int value, int max);

	void setBatteryPercent(float percent);

	void setStars(int star);

	void onButtonRelease(cocos2d::ui::Button* button, int id);

private:
	cocos2d::ui::Widget *_bg;
	cocos2d::ui::Button *_btnItem;
	cocos2d::ui::Text *_lbConfigId;
	cocos2d::ui::Text *_lbName;
	cocos2d::ui::Text *_lbLevel;
	cocos2d::ui::Text *_lbGeneralPoint;

	cocos2d::ui::Button *_btnSwitch;
	cocos2d::ui::Text *_lbBtnSwitch;

	cocos2d::Node *_nodeStars;

	cocos2d::ui::Text *_lbLowBattery;
	cocos2d::ui::Text *_lbChargeFee;
	cocos2d::ui::ImageView *_imgGoldLbChargeFee;

	cocos2d::ui::Text *_lbBattery;
	cocos2d::ui::ImageView *_imgBattery;
	Battery *_battery;

	cocos2d::ui::Button *_btnAutoRecharge;

	cocos2d::Size _size;
	cocos2d::Rect _rect;

	ItemData* _data;
	std::map<std::string, cocos2d::Sprite3D*> _cues;
	cocos2d::Sprite3D *_cue3d;
public:
	static ElementTypeFrameData TYPE_FRAMES[4];
};


class Battery
{
public:
	Battery();
	~Battery();

	static Battery *create(cocos2d::Node *batteryBox, cocos2d::ui::ImageView *battery, cocos2d::ui::Text *text);

	bool init(cocos2d::Node *batteryBox, cocos2d::ui::ImageView *battery, cocos2d::ui::Text *text);

	void setPercent(float percent);

	void setValue(int val, int max);

private:
	cocos2d::ui::ImageView *_batteryBox;
	cocos2d::ui::ImageView *_battery;
	cocos2d::ui::Text *_text;

	cocos2d::DrawNode *_stencil;
	float _rectWidthStencil;
};
#endif