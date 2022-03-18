#ifndef QuickCustomTableView_h_
#define QuickCustomTableView_h_

#include "CustomTableView.h"
#include <data/Resource.h>
#include <data/GameConstant.h>

class QuickCustomTableViewCell
	:public cocos2d::Node
{
public:
	virtual cocos2d::Size getOriginalSize() = 0;
};

template<typename T, class U>
class QuickCustomTableView
	: public cocos2d::Layer,
	public cocos2d::extension::TableViewDataSource,
	public cocos2d::extension::TableViewDelegate
{
public:
	typedef int Factor;
	#define cext cocos2d::extension 
	#define cui cocos2d::ui

	typedef std::function<void(U*)> OnInitItemCallback;
	typedef std::function<void(cext::TableViewCell *)> TableViewCellTouchCallback;

	QuickCustomTableView();
	~QuickCustomTableView();

	QuickCustomTableView(cocos2d::ui::Widget *panel, cocos2d::extension::ScrollView::Direction direction, float primaryDirCount, float alternativeDirCount);

	static QuickCustomTableView<T, U> *create(cocos2d::Size size, cocos2d::extension::ScrollView::Direction direction, float primaryDirCount, float alternativeDirCount);

	bool init(cocos2d::Size size, cocos2d::extension::ScrollView::Direction direction, float primaryDirCount, float alternativeDirCount);

	void setHorOffsetFactor(Factor factor);

	void setVerOffsetFactor(Factor factor);

	void setTriggerOnUIOffset(cocos2d::Vec2 offset);

	float getPrimaryDirCount() const;

	std::vector<T*> getItems();

	void scrollViewDidScroll();

	virtual cocos2d::extension::TableViewCell* tableCellAtIndex(cocos2d::extension::TableView *table, ssize_t idx) override;

	virtual ssize_t numberOfCellsInTableView(cocos2d::extension::TableView *table) override;

	virtual void tableCellTouched(cocos2d::extension::TableView* table, cocos2d::extension::TableViewCell* cell) override;

	virtual cocos2d::Size tableCellSizeForIndex(cext::TableView *table, ssize_t idx) override
	{
		float wDiv = 1.f, hDiv = 1.f;
		if (_direction == cext::ScrollView::Direction::HORIZONTAL)
			wDiv = _primaryDirCount;
		else
			hDiv = _primaryDirCount;
		return cocos2d::Size(_size.width / wDiv, _size.height / hDiv);
	}

	virtual cocos2d::Size tableCellSizeForIndex()
	{
		float wDiv = 1.f, hDiv = 1.f;
		if (_direction == cext::ScrollView::Direction::HORIZONTAL)
			wDiv = _primaryDirCount;
		else
			hDiv = _primaryDirCount;
		return cocos2d::Size(_size.width / wDiv, _size.height / hDiv);
	}

	void onInitItem(U *item);

	void setData(const std::vector<T*> & data);

	void reloadData(const std::vector<T*> & data);

	void touchBtn(cocos2d::Ref * sender, cocos2d::ui::Widget::TouchEventType type);

	void update(float dt);

	void setInitItemCallback(const OnInitItemCallback & callback);

	void setTableViewTouchCallback(const TableViewCellTouchCallback & callback);

	cocos2d::extension::TableViewCell* getCellAtIndex(ssize_t idx);

private:

	int _horOffFactor;
	int _verOffFactor;

	int _triggerOffset = -1;

	std::vector<T*> _items;
	std::vector<T*> _dataList;

	OnInitItemCallback _onInitItemCallback;
	TableViewCellTouchCallback _onTableViewTouchCallback;
public:
	cocos2d::ui::Button *_button;
	cocos2d::extension::TableView *_tbView;
	cext::ScrollView::Direction _direction;
	cocos2d::Size _size;
	float _primaryDirCount;
	float _alterDirCount;
};

template<typename T, class U>
QuickCustomTableView<T, U> * QuickCustomTableView<T, U>::create(cocos2d::Size size, cocos2d::extension::ScrollView::Direction direction, float primaryDirCount, float alternativeDirCount)
{
	auto tb = new QuickCustomTableView<T, U>();
	tb->init(size, direction, primaryDirCount, alternativeDirCount);
	tb->autorelease();
	return tb;
}

template<typename T, class U>
void QuickCustomTableView<T, U>::setInitItemCallback(const OnInitItemCallback & callback)
{
	_onInitItemCallback = callback;
}

template<typename T, class U>
void QuickCustomTableView<T, U>::setTableViewTouchCallback(const TableViewCellTouchCallback & callback)
{
	_onTableViewTouchCallback = callback;
}

template<typename T, class U>
void QuickCustomTableView<T, U>::onInitItem(U* item)
{
	if (_onInitItemCallback != nullptr)
	{
		_onInitItemCallback(item);
	}
}

template<typename T, class U>
void QuickCustomTableView<T, U>::update(float dt)
{
}

template<typename T, class U>
void QuickCustomTableView<T, U>::touchBtn(cocos2d::Ref * sender, cocos2d::ui::Widget::TouchEventType type)
{

}

template<typename T, class U>
QuickCustomTableView<T, U>::~QuickCustomTableView()
{

}

template<typename T, class U>
QuickCustomTableView<T, U>::QuickCustomTableView()
{
	_onInitItemCallback = nullptr;
	_onTableViewTouchCallback = NULL;
	_dataList.clear();
}

/*
primaryDirCount number row, default = -1, belong size of table view and cell
alternativeDirCount number item in a row
*/
template<typename T, class U>
bool QuickCustomTableView<T, U>::init(cocos2d::Size size, cocos2d::extension::ScrollView::Direction direction, float primaryDirCount, float alternativeDirCount)
{
	_size = size;
	_primaryDirCount = primaryDirCount;
	_alterDirCount = alternativeDirCount;
	_direction = direction;

	auto templateItem = new U();
	templateItem->init();
	auto originalSize = templateItem->getOriginalSize();
	delete templateItem;

	if (primaryDirCount < 0) {
		if (direction == cocos2d::extension::ScrollView::Direction::VERTICAL) {
			auto scaleX = _size.width / (_alterDirCount * originalSize.width);
			_primaryDirCount = _size.height / (originalSize.height * scaleX);
		}
		else {
			auto scaleY = _size.height / (_alterDirCount * originalSize.height);
			_primaryDirCount = _size.width / (originalSize.width * scaleY);
		}
	}

	cocos2d::Layer::init();
	_tbView = cocos2d::extension::TableView::create(this, size);
	_tbView->setDelegate(this);
	_tbView->setDirection(direction);
	_tbView->setVerticalFillOrder(cocos2d::extension::TableView::VerticalFillOrder::TOP_DOWN);

	_button = cocos2d::ui::Button::create(Res::Base::img_btn_normal);
	_button->setSwallowTouches(false);
	_button->setPosition(cocos2d::Vec2(0, 0));
	_button->setAnchorPoint(cocos2d::Vec2(0, 0));
	_button->setZoomScale(0);
	_button->setScale(size.width / 46, size.height / 36);
	_button->addTouchEventListener(CC_CALLBACK_2(QuickCustomTableView::touchBtn, this));
	_button->setOpacity(0);

	addChild(_tbView);
	addChild(_button);

	scheduleUpdate();

	return true;
}


template<typename T, class U>
void QuickCustomTableView<T, U>::tableCellTouched(cocos2d::extension::TableView* table, cocos2d::extension::TableViewCell* cell)
{
	if (_onTableViewTouchCallback)
	{
		_onTableViewTouchCallback(cell);
	}
}

template<typename T, class U>
ssize_t QuickCustomTableView<T, U>::numberOfCellsInTableView(cocos2d::extension::TableView *table)
{
	return ceil(_dataList.size() / this->_alterDirCount);
}

template<typename T, class U>
cocos2d::extension::TableViewCell* QuickCustomTableView<T, U>::tableCellAtIndex(cocos2d::extension::TableView *table, ssize_t idx)
{
	cext::TableViewCell* cell = table->dequeueCell();
	U *item;

	if (!cell)
	{
		cell = cext::TableViewCell::create();
		auto cellSize = tableCellSizeForIndex(table, idx);
		cocos2d::Size itemSize;
		bool isHor = false;
		if (_direction == cext::ScrollView::Direction::HORIZONTAL)
		{
			isHor = true;
			itemSize.width = cellSize.width;
			itemSize.height = cellSize.height / _alterDirCount;
		}
		else
		{
			itemSize.width = cellSize.width / _alterDirCount;
			itemSize.height = cellSize.height;
		}

		for (int i = 0; i < _alterDirCount; i++)
		{
			int dataIdx = idx * _alterDirCount + i;
		
			item = new U();
			item->init();
			cell->addChild(item);
			cocos2d::Vec2 pos;
			if (isHor)
			{
				pos.x = itemSize.width / 2;
				pos.y = itemSize.height * (2 * i + 1) / 2;
			} 
			else 
			{
				pos.x = itemSize.width * (2 * i + 1) / 2;
				pos.y = itemSize.height / 2;
			}
			item->setPosition(pos);
			item->setTag(TAG_COMMON+ i);
			onInitItem(item);
			if (dataIdx < _dataList.size())
			{
				item->setVisible(true);
				item->loadData(_dataList[dataIdx]);
			}
			else
			{
				item->setVisible(false);
			}
		}
		
	}
	else
	{
		for (int i = 0; i < _alterDirCount; i++)
		{
			int dataIdx = idx * _alterDirCount + i;
			item = (U*)cell->getChildByTag(TAG_COMMON+i);
			if (dataIdx < _dataList.size())
			{
				item->loadData(_dataList[dataIdx]);
				item->setVisible(true);
			}
			else
			{
				item->setVisible(false);
			}
		}
		
	}

	return cell;
}

template<typename T, class U>
void QuickCustomTableView<T, U>::scrollViewDidScroll()
{
	//if (this->triggerOffset < 0 || this->numberOfCellsInTableView() == 0) return;
	//auto currentOffset = tbView.getContentOffset();
	//auto minOffset = tbView.minContainerOffset();
	//auto itemCountOffset = 0;
	//auto offset = 0;
	//auto size = this->numberOfCellsInTableView(tbView);
	//auto curItem;

	//for (auto i = 0; i < size; i++) {
	//	auto lastOffset = offset;
	//	auto cellsize;
	//	curItem = this->getCellAtIdx(i).getItem();
	//	if (this->_direction == cc.SCROLLVIEW_DIRECTION_VERTICAL) {
	//		cellsize = this->tableCellSizeForIndex(tbView, i).height;
	//		offset += cellsize;
	//		if (offset + minOffset.y > currentOffset.y) {
	//			itemCountOffset += (currentOffset.y - minOffset.y - lastOffset) / cellsize;
	//			break;
	//		}
	//	}
	//	else {
	//		cellsize = this->tableCellSizeForIndex(tbView, i).width;
	//		offset += cellsize;
	//		if (offset + minOffset.x > currentOffset.x) {
	//			itemCountOffset += (currentOffset.x - minOffset.x - lastOffset) / cellsize;
	//			break;
	//		}
	//	}
	//	itemCountOffset += 1;
	//}

	//if (this->lastItemCountOffset < this->triggerOffset && this->triggerOffset < itemCountOffset) {
	//	curItem.onIncreaseTriggerOffset();
	//}
	//else if (this->lastItemCountOffset > this->triggerOffset && this->triggerOffset > itemCountOffset) {
	//	curItem.onDecreaseTriggerOffset();
	//}
	//this->lastItemCountOffset = itemCountOffset;
}

template<typename T, class U>
std::vector<T*> QuickCustomTableView<T, U>::getItems()
{

}

template<typename T, class U>
float QuickCustomTableView<T, U>::getPrimaryDirCount() const
{
	return this->_primaryDirCount;
}

template<typename T, class U>
void QuickCustomTableView<T, U>::setTriggerOnUIOffset(cocos2d::Vec2 offset)
{
	this->triggerOffset = offset;
}

template<typename T, class U>
void QuickCustomTableView<T, U>::setVerOffsetFactor(Factor factor)
{
	this->_verOffFactor = factor;
}

template<typename T, class U>
void QuickCustomTableView<T, U>::setHorOffsetFactor(Factor factor)
{
	this->_horOffFactor = factor;
}

template<typename T, class U>
void QuickCustomTableView<T, U>::setData(const std::vector<T*> & data)
{
	_dataList.clear();
	_dataList.insert(_dataList.begin(), data.begin(), data.end());
}

template<typename T, class U>
void QuickCustomTableView<T, U>::reloadData(const std::vector<T*> & data)
{
	setData(data);
	_tbView->reloadData();
}

template<typename T, class U>
cocos2d::extension::TableViewCell* QuickCustomTableView<T, U>::getCellAtIndex(ssize_t idx)
{
	return _tbView->cellAtIndex(idx);
}

#endif // !QuickCustomTableView_h_

