#include "../base/UIBaseLayer.h"
#ifndef PopupOpenChest_h_
#define PopupOpenChest_h_

class PopupOpenChest
	:public UIBaseLayer
{
public:
	PopupOpenChest();
	~PopupOpenChest();

	CREATE_FUNC(PopupOpenChest);
	bool init();

	void initGUI();

	void runAnimation();

	void runActionRingChest();

	void runActionOpenChest();

	void runReceiveGift();

	void updateInventory();

	void flyToPos(cocos2d::Node *node, cocos2d::Vec2 pos, bool isStar);

	void show();

private:
	cocos2d::ui::ImageView *_imgChest;
	cocos2d::Sprite *_imgChestOpen;

	std::vector<cocos2d::Node *> _giftNodes;
	cocos2d::Node *_nodeStar;
	cocos2d::Node *_nodeCard;
	cocos2d::Node *_nodeGold;
};
#endif