#ifndef GUIMGR_H_
#define GUIMGR_H_

#include <string.h>
#include <functional>
#include <cocos2d.h>
#include <data/GameConstant.h>
#include <data/Resource.h>
#include <cocos/ui/CocosGUI.h>
#include "core/gui/BaseLayer.h"
#include "popup/MessagePopup.h"


enum AnnounceType {
	ANNOUNCE_FOUL,
	ANNOUNCE_PLAYER_TURN,
	ANNOUNCE_OPPONENT_TURN,
	ANNOUNCE_WIN,
	ANNOUNCE_LOSE,
	
	STYLE_POINT_1,
	STYLE_POINT_2,
	STYLE_POINT_3,
	STYLE_POINT_4,
	STYLE_POINT_5,
	STYLE_POINT_6,
	STYLE_POINT_7,
	STYLE_POINT_8,

	NOTICE_ROUND_2,
	NOTICE_ROUND_3,
	NOTICE_ROUND_4,
	NOTICE_ROUND_END,

	ANNOUNCE_DEFAULT
};

enum Popup
{
	EIGHT_BALL_MODE_SELECT = 1,
	FIND_MATCH_LAYER = 2,
	GAMBLE_LAYER = 3,
	RESULT_LAYER = 4,
	S2_RESULT_LAYER = 5,
	UNLOCK_SPONSORSHIP = 6,
	LOBBY_UI = 7,
	// UNLOCK_SPONSOR_GUI= 8,
	SPONSOR_DETAIL = 8,
	SPONSOR_SELECT = 9,
	POPUP_CONFIRM_2_OPTION = 10,
	POPUP_CONFIRM_1_OPTION = 11,
	SPONSORSHIP_UI = 12,
	SPONSOR_REWARD_CLAIM = 13,
	RT3_FIND_MATCH_LAYER = 14,
	RESULT_LAYER_RT3 = 15,
	CHANNEL_SELECT = 16,
	OPEN_CARD_LAYER = 17,
	INGAME_BASE_UI = 18,
	INGAME_EBP_UI = 19,
	INGAME_ARCADE_UI = 20,
	INGAME_START_MATCHING_UI = 21,
	RESULT_GIFT_LAYER = 22,
	DISCONNECT_POPUP = 23,
	SELECT_MODE_POPUP = 24,

	POP_GIFT_GOLD = 25,
	LINK_SOCIAL_POPUP = 26,
	POPUP_NOTIFY = 27,
	INGAME_MID_GAME_FX = 28,

	// Inventory
	COMPETITION_BAG_UI = 100,
	INVENTORY_UI = 101,
	CUE_RENAME = 102,
	CUE_INSPECT_UI = 103,
	CUE_UPGRADE_UI = 104,
	CUE_UPGRADE_POPUP = 105,

	// Gacha
	GACHA_UI = 200,
	BUY_ITEM_POPUP = 201,
	GACHA_ROLLING = 202,
	CUE_UNBOX_UI = 203,


	// Setting
	SETTING_UI = 300,

	//OFFER
	OFFER_BASE_UI = 350,

	// SHOP
	SHOP_UI = 400,
	PAYMENT_RESULT = 401,
	BANK_SELECT_UI = 402,
	SMS_SELECT_UI = 403,
	ZING_CARD_ENTER_UI = 404,

	POPUP_CONFIRM = 405,
	POPUP_RESULT = 406,
	POPUP_SEARCH = 407,

	// ACHIEVEMENT
	ACHIEVEMENTS_UI = 500,
	REPUTATION_LAYER = 501,

	// WORKSHOP
	WORKSHOP_UI = 600,

	// CUE_COLLECTION
	CUE_COLLECTION_UI = 700,
	Cue_COLLECTION_DETAIL = 701,

	// HALL OF FAME
	SUPER_CUP_UI = 800,
	PLAYER_HALL_OF_FAME = 801,
	RECEIVE_TROPHY_UI = 802,

	// Arcade
	ARCADE_RESULT_UI = 60,
	ARCADE_MAP = 61,
	ARCADE_LEVEL_INFO = 62,

	// Debug guis
	CUE_DEBUG = 9000,
	// CONFIRM_POPUP= 10,

	DEBUG_DIALOG = 9001,

	PROFILE_GUI = 100,
	PROFILE_USER_GUI = 101,
	POPUP_RECEIVED_GIFT = 102,
	FRIEND_GUI = 105,
	POPUP_INVITE_RT3 = 106,

	// Tutorial
	TUTORIAL_LESSON_UI = 900,

	// offer UI
	OFFER_STARTER_UI = 1000,
	OFFER_POPUP_OPEN_CHEST = 1001,

	// spin UI
	SPIN_UI = 1100,

	MESSAGE_POUP = 11000,
};

#define guiMgr GuiMgr::getInstance()

class GuiMgr
{
public:

	typedef std::function<void()> AnnouncementCallBack;

	GuiMgr();
	~GuiMgr();

	void notifyAnnouncerIngame(AnnounceType announceType, std::string message, const AnnouncementCallBack & callback);

	void notifyDebugText(std::string message, cocos2d::Vec2 position = cocos2d::Vec2::ZERO);

	void notifyText(std::string text, cocos2d::Vec2 position = cocos2d::Vec2::ZERO, std::string fontName = Res::font_facebook, int fontSize = 30, cocos2d::Color3B color = gv::ANNOUCEMENT_TEXT_COLOR);

	void notifyTextDone(cocos2d::Node *label);

	void removeAfterAction(cocos2d::Node *node);

	void removeAndCallBack(cocos2d::Node *node, const AnnouncementCallBack & callback);

	BaseLayer *getGui(Popup tag);

	BaseLayer *createPopup(Popup tag);

	void show(BaseLayer *popup);

	void hide(BaseLayer *popup, bool keepPopup, int tag);

	cocos2d::Scene *getCurScene();

	void initMapsForScenes(int tag);

	void initPopupsMap();

	void refreshGui(Popup tag);

	void refreshAllGui();

	void showMessagePopup(const std::string &title, const std::string &message, const MessagePoup::PopupListener & callback, std::string okText = "Ok");

	void showMessagePopup(const std::string &message, const MessagePoup::PopupListener & callback, std::string okText = "Ok");

	void showOkCancelPopup(const std::string &message, const MessagePoup::PopupListener & callback, std::string okText="Ok", std::string cancelText = "Cancel");

	void showOkCancelPopup(const std::string &title, const std::string &message, const MessagePoup::PopupListener & callback, std::string okText = "Ok", std::string cancelText = "Cancel");

private:
	std::vector<cocos2d::ui::Text *> _notifies;
	std::vector<cocos2d::Action *> _actions;

	std::map<int, std::map<Popup, BaseLayer*>> _popups;

	std::map<int, std::vector<BaseLayer*>> _popupStack;
	std::map<int, int> _popupZorder;

	static GuiMgr* _instance;
	
public:
	int _curSceneTag;
public:
	static GuiMgr * getInstance();
};


#endif // !GUIMGR_H_


