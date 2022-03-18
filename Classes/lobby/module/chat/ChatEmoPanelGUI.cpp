#include "ChatEmoPanelGUI.h"
#include "ChatConfig.h"
#include "renderer/DBCCArmatureNode.h"
#include "renderer/DBCCFactory.h"
#include "core/utils/StringUtility.h"
#include "lobby/network/LobbyHandler.h"

USING_NS_CC;
using namespace extension;
using namespace dragonBones;
using namespace cocos2d::ui;

bool ChatEmoNewCell::init()
{
	TableViewCell::init();

	bgItem = Node::create ();
	bgItem->setPosition(59, getContentSize().height/ 2 - 5);
	addChild(bgItem);

	eff = DBCCFactory::getInstance()->buildArmatureNode("Emoticon");

	if (eff) {
		eff->setScale(1);
		bgItem->addChild(eff);
	}

	vip = ImageView::create();
	vip->setPosition(Vec2(35, -20));
	vip->setVisible(false);
	addChild(vip);
	return true;
}

void ChatEmoNewCell::setEmotion(ChatEmo emo)
{
	std::string vipIcon = "Interact/icon_vip@level.png";
	vip->loadTexture(StringUtility::replaceAll(vipIcon, "@level", chatConfig->getLevelCanUseAnItem(emo.id, true)));
	vip->setContentSize(vip->getVirtualRendererSize());
	if (eff) {
		if (emo.id > 18)
			eff->getAnimation()->gotoAndPlay(std::to_string(emo.id - 100 + 18), -1, -1, 0);
		else
			eff->getAnimation()->gotoAndPlay(std::to_string(emo.id), -1, -1, 0);
	}

	vip->setVisible(!chatConfig->canUseItem(emo.id, true));
	eff->setOpacity(vip->isVisible() ? 85 : 255);
	if (vip->isVisible()) {
		eff->getAnimation()->gotoAndStop(std::to_string(emo.id), -1, -1, 0);
	}
}


ChatEmoPanelGUI::ChatEmoPanelGUI()
{
}

ChatEmoPanelGUI::~ChatEmoPanelGUI()
{
}

bool ChatEmoPanelGUI::init()
{

	std::vector<int> arVip = { 1, 3, 4, 5, 6, 7, 8, 2, 9, 10, 11, 12, 13, 18, 14, 15, 16, 17 };
	for (int i = 0; i < arVip.size(); i++) {
		ChatEmo obj;
		obj.id = arVip[i];
		obj.vip = 0;
		obj.use = 0;
		emoList.push_back(obj);
	}

	defaultPos = Vec2(0, 0);

	BaseLayer::init(ChatEmoPanelGUI::className);
	initWithBinaryFile("ChatBoardEmoPanel.csb");

	waitTouch = 0;

	return true;
}

void ChatEmoPanelGUI::initGUI()
{
	bg = getControl("bg");
	panel = getControl("panel", bg);

	listView = TableView::create(this, Size(panel->getContentSize().width, panel->getContentSize().height * 1.8f));
	listView->setDirection(cocos2d::extension::ScrollView::Direction::HORIZONTAL);
	listView->setVerticalFillOrder(TableView::VerticalFillOrder::TOP_DOWN);
	listView->setDelegate(this);
	panel->addChild(listView);
	listView->reloadData();

	EventListenerTouchOneByOne* bgMoreListenter = EventListenerTouchOneByOne::create();
	bgMoreListenter->setSwallowTouches(false);

	bgMoreListenter->onTouchBegan = [this](cocos2d::Touch* touch, cocos2d::Event* event) {
		return true;
	};
	bgMoreListenter->onTouchEnded = [this](cocos2d::Touch* touch, cocos2d::Event* event) {
		((ChatEmoPanelGUI*)event->getCurrentTarget())
			->onTouchBoard();
	};

	Director::getInstance()->getEventDispatcher()->addEventListenerWithSceneGraphPriority(bgMoreListenter, this);

	scheduleUpdate();
}

void ChatEmoPanelGUI::setVisible(bool visible)
{
	BaseLayer::setVisible(visible);
	listView->setTouchEnabled(visible);
}

void ChatEmoPanelGUI::onEnter()
{
	BaseLayer::onEnter();
}

void ChatEmoPanelGUI::open()
{
	waitTouch = false;

	bool isShow = isVisible();

	setPosition(defaultPos);
	if (isShow) {
		runAction(Sequence::create(MoveTo::create(0.15f, Vec2(defaultPos.x, -400)), Hide::create(), nullptr));
	}
	else {
		setVisible(true);
		setPosition(defaultPos.x, -400);
		runAction(MoveTo::create(0.15f, defaultPos));
	}
}

void ChatEmoPanelGUI::onTouchBoard()
{
	waitTouch = true;
	runAction(Sequence::create(
		DelayTime::create(0.1f),
		CallFunc::create(CC_CALLBACK_0(ChatEmoPanelGUI::finishTouchBoard, this)),
		nullptr
	));
}

void ChatEmoPanelGUI::finishTouchBoard()
{
	if (!waitTouch) return;
	if (!isVisible()) return;

	runAction(Sequence::create(
		MoveTo::create(0.15f, Vec2(defaultPos.x, defaultPos.y - 250)), 
		Hide::create(), 
		nullptr));
}

void ChatEmoPanelGUI::tableCellTouched(cocos2d::extension::TableView* table, cocos2d::extension::TableViewCell* cell)
{
	int idx = cell->getIdx();
	ChatEmo emoIdx = emoList[idx];

	if (chatConfig->canUseItem(emoIdx.id, true)) {
		LobbyHandler::requestChatTotal(2, -1, "", emoIdx.id);
	}

	runAction(Sequence::create(
		MoveTo::create (0.15f, Vec2(defaultPos.x, defaultPos.y - 250)), 
		Hide::create(), 
		nullptr));
}

cocos2d::Size ChatEmoPanelGUI::tableCellSizeForIndex(cocos2d::extension::TableView *table, ssize_t idx)
{
	return Size(113, 107);
}

cocos2d::extension::TableViewCell* ChatEmoPanelGUI::tableCellAtIndex(cocos2d::extension::TableView *table, ssize_t idx)
{
	TableViewCell* cell = table->dequeueCell();
	if (!cell) {
		cell = ChatEmoNewCell::create();
	}
	dynamic_cast<ChatEmoNewCell*>(cell)->setEmotion(emoList[idx]);
	return cell;
}

ssize_t ChatEmoPanelGUI::numberOfCellsInTableView(cocos2d::extension::TableView *table)
{
	return emoList.size();
}

std::string ChatEmoPanelGUI::className = "ChatEmoPanelGUI";

