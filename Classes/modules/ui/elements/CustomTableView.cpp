#include "CustomTableView.h"
#include "../../../data/Resource.h"

USING_NS_CC_EXT;
USING_NS_CC;
using namespace cocos2d::ui;

CustomTableView::CustomTableView()
{
	_isCaceOffset = false;
}

CustomTableView::CustomTableView(cocos2d::Size size, cocos2d::extension::ScrollView::Direction direction)
{
	_isCaceOffset = false;
	init(size, direction);
}

CustomTableView::~CustomTableView()
{

}

bool CustomTableView::init(const cocos2d::Size & size, cocos2d::extension::ScrollView::Direction direction)
{
	//TODO this->data = [];
	this->_touching = false;

	this->_tbView = TableView::create(this, size);
	this->_tbView->setDirection(direction);
	this->_tbView->setPosition(0, 0);
	this->_tbView->setDelegate(this);
	this->_tbView->setVerticalFillOrder(TableView::VerticalFillOrder::TOP_DOWN);

	this->_button = Button::create(Res::Base::img_btn_normal);
	this->_button->setSwallowTouches(false);
	this->_button->setPosition(Vec2(0,0));
	this->_button->setAnchorPoint(Vec2(0,0));
	this->_button->setZoomScale(0);
	this->_button->setScale(size.width / 46, size.height / 36);
	this->_button->addTouchEventListener(CC_CALLBACK_2(CustomTableView::touchBtn, this));
	this->_button->setOpacity(0);

	this->addChild(this->_button);
	this->addChild(this->_tbView);

	return true;
}

void CustomTableView::initGUI()
{

}

void CustomTableView::loadData(std::vector<TableData> data)
{
	this->_data = data;
	this->unschedule(schedule_selector(CustomTableView::scrollUpdate));
	this->_tbView->reloadData();
}

void CustomTableView::getCellAtIdx(int idx)
{

}

void CustomTableView::reloadAtIdx(int idx)
{

}

void CustomTableView::removeCellAtIdx(int idx)
{

}

void CustomTableView::scrollToIndex(int idx, bool scrollAnim /*= true*/)
{

}

void CustomTableView::scrollToOffsetIn(cocos2d::Vec2 offset, float time)
{

}

void CustomTableView::scrollUpdate(float dt)
{

}

void CustomTableView::scrollBy(cocos2d::Vec2 p)
{

}

void CustomTableView::scroolTo(cocos2d::Vec2 p, bool scrollAnim /*= true*/)
{

}

void CustomTableView::touchBtn(cocos2d::Ref* sender, cocos2d::ui::Widget::TouchEventType type)
{
	this->lockScroll();
	switch (type) {
	case Widget::TouchEventType::BEGAN:
		this->_touching = true;
		break;
	case Widget::TouchEventType::MOVED:
	{
		auto p = ((Button*)sender)->getTouchMovePosition();
		auto buttonP = this->_button->convertToWorldSpace(p);
		auto direction = this->_tbView->getDirection();
		if (direction == cocos2d::extension::ScrollView::Direction::HORIZONTAL) {
			if ((p.x > buttonP.x + _size.width) || p.x < buttonP.x) {
				if (!this->_isCaceOffset) {
					this->_offset = this->_tbView->getContentOffset();
				}
				this->_tbView->setContentOffset(this->_offset, false);
			}
			else {
				this->_isCaceOffset = false;
			}
		}
		else if (direction == cocos2d::extension::ScrollView::Direction::VERTICAL) {
			if ((p.y > buttonP.y + _size.height) || p.y < buttonP.y) {
				if (!this->_isCaceOffset) {
					this->_offset = this->_tbView->getContentOffset();
				}
				this->_tbView->setContentOffset(this->_offset, false);
			}
			else {
				this->_isCaceOffset = false;
			}
		}
		break;
	}
		
	case Widget::TouchEventType::ENDED:
	case Widget::TouchEventType::CANCELED:
		this->_touching = false;
		break;
	}
}

void CustomTableView::lockScroll()
{

}

cocos2d::extension::TableViewCell* CustomTableView::tableCellAtIndex(TableView *table, ssize_t idx)
{
	return nullptr;
}

ssize_t CustomTableView::numberOfCellsInTableView(TableView *table)
{
	return 0;
}

void CustomTableView::tableCellTouched(TableView* table, TableViewCell* cell)
{
}

