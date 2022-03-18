#ifndef CueCollectionUI_h_
#define CueCollectionUI_h_

#include <core/gui/BaseLayer.h>
#include "../../elements/QuickCustomTableView.h"
#include "CueCollectionCueItem.h"
#include <cocos-ext.h>
#include "data/DataStruct.h"

class CueCollectionUI
	: public BaseLayer, cocos2d::extension::TableViewDataSource, cocos2d::extension::TableViewDelegate
{
public:
	enum TAG
	{
		btnTabMasse,
		btnTabBreak,
		btnTabJump,
		btnBack
	};
	CueCollectionUI();
	~CueCollectionUI();

	CREATE_FUNC(CueCollectionUI);
	bool init();

	void initGUI();

	void show();

	void hide();



	void refresh();

	void updateTab();

	void initCamera();

	void initTabs();

	void switchTab(int tabIdx);

	int getTypeByTabIndex(int idx);

	void reloadTableViewData(bool keep);

	void initList();

	void onButtonRelease(cocos2d::ui::Button* button, int id);

	virtual cocos2d::extension::TableViewCell* tableCellAtIndex(cocos2d::extension::TableView *table, ssize_t idx) override;

	virtual ssize_t numberOfCellsInTableView(cocos2d::extension::TableView *table) override;

	virtual void tableCellTouched(cocos2d::extension::TableView* table, cocos2d::extension::TableViewCell* cell) override;

	virtual cocos2d::Size tableCellSizeForIndex(cocos2d::extension::TableView *table, ssize_t idx) override;

	virtual void setVisible(bool visible) override;


	virtual void onEnter() override;

protected:
private:
	int _curTab;

	cocos2d::ui::ImageView *_imgBg;
	cocos2d::ui::Widget *_pnDockCount;
	cocos2d::ui::Text *_lbCount;
	cocos2d::ui::ImageView *_imgType;
	cocos2d::ui::ImageView *_imgTabBar;

	cocos2d::ui::Widget *_pnContent;

	cocos2d::ui::Button *_btnTabMasse;
	cocos2d::ui::Button *_btnTabBreak;
	cocos2d::ui::Button *_btnTabJump;

	cocos2d::Camera *_cam;

	std::vector<CueInfoData *> _dataList;
	cocos2d::extension::TableView *_tableView;

	float _tabActivePosX;
	float _tabInactivePosX;

	std::string _iconNameBtnTabs[4];
	std::vector<cocos2d::ui::Button *> _btnTab;

public:
	static const std::string className;
};

#endif