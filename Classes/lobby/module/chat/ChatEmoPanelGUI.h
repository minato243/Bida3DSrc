#ifndef ChatEmoPanelGUI_h__
#define ChatEmoPanelGUI_h__
#include "core/gui/BaseLayer.h"
#include "renderer/DBCCArmatureNode.h"
struct ChatEmo{
	int id;
	int vip;
	int use;
};
class ChatEmoNewCell:
	public cocos2d::extension::TableViewCell{
public:
	CREATE_FUNC(ChatEmoNewCell);
	bool init();
	void setEmotion(ChatEmo emo);
public:
	cocos2d::Node* bgItem;
	dragonBones::DBCCArmatureNode* eff;
	cocos2d::ui::ImageView* vip;
};

class ChatEmoPanelGUI:
	public BaseLayer,
	public cocos2d::extension::TableViewDataSource,
	public cocos2d::extension::TableViewDelegate
{
public:
	CREATE_FUNC(ChatEmoPanelGUI)
	ChatEmoPanelGUI();
	~ChatEmoPanelGUI();
	bool init();
public:
	void initGUI();
	void setVisible(bool visible);
	void onEnter();
	void open();
	void onTouchBoard();
	void finishTouchBoard();

	virtual void scrollViewDidScroll(cocos2d::extension::ScrollView* view)override {};
	virtual void scrollViewDidZoom(cocos2d::extension::ScrollView* view)override {}
	virtual void tableCellTouched(cocos2d::extension::TableView* table, cocos2d::extension::TableViewCell* cell)override;
	virtual cocos2d::Size tableCellSizeForIndex(cocos2d::extension::TableView *table, ssize_t idx)override;
	virtual cocos2d::extension::TableViewCell* tableCellAtIndex(cocos2d::extension::TableView *table, ssize_t idx)override;
	virtual ssize_t numberOfCellsInTableView(cocos2d::extension::TableView *table)override;
public:
	std::vector<ChatEmo> emoList;
public:
	static std::string className;
	cocos2d::Vec2 defaultPos;
	int waitTouch;
	cocos2d::Node* bg;
	cocos2d::Node* panel;
	cocos2d::extension::TableView* listView;

};

#endif // ChatEmoPanelGUI_h__
