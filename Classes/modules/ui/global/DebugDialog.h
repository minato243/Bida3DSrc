#ifndef DebugDialog_h_
#define DebugDialog_h_

#include <core/gui/BaseLayer.h>
#include "../base/UIBaseLayer.h"

class DebugDialogTab
	:public UIBaseLayer
{
public:
	DebugDialogTab();
	~DebugDialogTab();

	CREATE_FUNC(DebugDialogTab);
	bool init();

	void initGUI();

	cocos2d::Size getOriginalSize();

	void onButtonTouched(cocos2d::ui::Button* button, int id);

	void onButtonRelease(cocos2d::ui::Button* button, int id);

	void loadData(std::string *name);

	void setDelegate(cocos2d::Node *delegate);

private:
	cocos2d::ui::Button *_btnBg;
	cocos2d::ui::Text *_lbTitle;

	cocos2d::Node *_delegate;
	long long beganTouch;
	std::string *_uiName;
};

class DebugDialog
	: public BaseLayer
{
public:
	enum TAG
	{
		btnBack = 101
	};
	DebugDialog();
	~DebugDialog();

	CREATE_FUNC(DebugDialog);
	virtual bool init();

	void initGUI();

	void initTag();

	void onButtonRelease(cocos2d::ui::Button* button, int id);

	void initCheatList();

	void initScrollList();

	void onInitTab(DebugDialogTab *tab);

	void showUI(cocos2d::Node *ui);

	void showUI(std::string *uiName);
private:
	cocos2d::ui::Widget *_pnContent;
	cocos2d::ui::Widget *_pnListCate;
	std::vector<cocos2d::Node *> _uis;
	std::vector<std::string *> _names;

public:

};
#endif