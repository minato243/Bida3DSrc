#ifndef CustomTableView_h_
#define CustomTableView_h_

#include <core/gui/BaseLayer.h>
#include <cocos-ext.h>

class CustomTableView
	:public BaseLayer, cocos2d::extension::TableViewDataSource, cocos2d::extension::TableViewDelegate
{
public:
	typedef int TableData;

	CustomTableView();
	~CustomTableView();

	CustomTableView(cocos2d::Size size, cocos2d::extension::ScrollView::Direction direction);

	bool init(const cocos2d::Size & size, cocos2d::extension::ScrollView::Direction direction);

	void initGUI();

	void loadData(std::vector<TableData> data);

	void getCellAtIdx(int idx);

	void reloadAtIdx(int idx);

	void removeCellAtIdx(int idx);

	void scrollToIndex(int idx, bool scrollAnim = true);

	void scrollToOffsetIn(cocos2d::Vec2 offset, float time);

	void scrollUpdate(float dt);

	void scrollBy(cocos2d::Vec2 p);

	void scroolTo(cocos2d::Vec2 p, bool scrollAnim = true);

	void touchBtn(cocos2d::Ref* sender, cocos2d::ui::Widget::TouchEventType type);

	void lockScroll();

public:
	cocos2d::extension::TableView *_tbView;
private:
	bool _touching;
	
	cocos2d::ui::Button *_button;

	cocos2d::Vec2 _offset;
	bool _isCaceOffset;
	cocos2d::Size _size;
	
	std::vector<TableData> _data;
public:
	virtual cocos2d::extension::TableViewCell* tableCellAtIndex(cocos2d::extension::TableView *table, ssize_t idx) override;


	virtual ssize_t numberOfCellsInTableView(cocos2d::extension::TableView *table) override;


	virtual void tableCellTouched(cocos2d::extension::TableView* table, cocos2d::extension::TableViewCell* cell) override;

};

#endif