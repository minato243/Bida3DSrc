#ifndef CueCollectionUIDetail_h_
#define CueCollectionUIDetail_h_

#include <cocos-ext.h>
#include <core/gui/BaseLayer.h>
#include "../../elements/StatBarPanel.h"
#include "data/DataStruct.h"

class CueCollectionTableView;

class CueCollectionUIDetail
	: public BaseLayer
{
public:
	enum TAG 
	{
		BTN_BACK
	};
	CueCollectionUIDetail();
	~CueCollectionUIDetail();

	CREATE_FUNC(CueCollectionUIDetail);
	bool init();

	void initGUI();

	void initBar();

	void initList();

	void onButtonRelease(cocos2d::ui::Button* button, int id);

	void show();

	void hide();

	void loadData(int type, int idx);

	void loadCueInfo(CueInfoData *cue);

	void setName(int type);

	void setStar(int star);

	void updateTab();

	void scrollToData(CueInfoData *data);

	void setActiveItem(int idx);

protected:
private:
	std::vector<CueInfoData*> _dataList;
	int _nullPadding;
	CueCollectionTableView *_tableView;

public:
	cocos2d::ui::Widget *_pnStats;
	cocos2d::ui::ImageView *_imgLevel;
	cocos2d::ui::Widget *_pnForce;
	cocos2d::ui::Widget *_pnSpin;
	cocos2d::ui::Widget *_pnAngle;
	cocos2d::ui::Widget *_pnAim;
	cocos2d::ui::Widget *_pnAccuracy;

	StatBarPanel *_pnForceWrapper;
	StatBarPanel *_pnSpinWrapper;
	StatBarPanelAngle *_pnAngleWrapper;
	StatBarPanel *_pnAimWrapper;
	StatBarPanel *_pnAccuracyWrapper;

	cocos2d::ui::Widget *_pnContent;

	int _type;
	int _curIdx;
};

class CueCollectionTableView
	: public cocos2d::Layer,
	public cocos2d::extension::TableViewDataSource, 
	public cocos2d::extension::TableViewDelegate
{
public:
	CueCollectionTableView();
	~CueCollectionTableView();

	bool init(cocos2d::Size size, float primaryDirCount, float alternativeDirCount);

	void touchBtn(cocos2d::Ref * sender, cocos2d::ui::Widget::TouchEventType type);

	virtual cocos2d::Size tableCellSizeForIndex(cocos2d::extension::TableView *table, ssize_t idx) override;


	virtual cocos2d::extension::TableViewCell* tableCellAtIndex(cocos2d::extension::TableView *table, ssize_t idx) override;


	virtual ssize_t numberOfCellsInTableView(cocos2d::extension::TableView *table) override;


	virtual void tableCellTouched(cocos2d::extension::TableView* table, cocos2d::extension::TableViewCell* cell) override;

	void setData(const std::vector<CueInfoData*> & data);

	void reloadData(const std::vector<CueInfoData*> & data);

	void scrollToIndex(int idx);

	virtual void scrollViewDidScroll(cocos2d::extension::ScrollView* view) override;

	void scrollToNearest();

	void onStopScrolling();

	void update(float dt);

public:
	cocos2d::extension::TableView *_tbView;
	cocos2d::ui::Button *_button;
	std::vector<CueInfoData*> _dataList;

	cocos2d::Size _size;
	float _primaryDirCount;

	bool _autoSnap;
	bool _isTouching;
	bool _isOffset;
	bool _isScrollToNearestCell;
	bool _scrolling;
	bool _scrollLastFrame;
	bool _scrollCurFrame;

	cocos2d::Vec2 _offset;

	BaseLayer *_mainUI;
};

#endif