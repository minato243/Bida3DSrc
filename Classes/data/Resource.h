#ifndef RESOURCE_H_
#define RESOURCE_H_

#include <cocos2d.h>

class Res {
public:
    //font
	static const std::string     FONT_BITMAP_NUMBER_1;
	static const std::string         FONT_BITMAP_DICE_NUMBER;
	static const std::string         font_facebook;
	static const std::string         font_viet;
        //zcsd
        //screen
	static const std::string     ZCSD_SCREEN_MENU;
	static const std::string         ZCSD_SCREEN_NETWORK;
	static const std::string         ZCSD_SCREEN_LOCALIZATION;
	static const std::string         ZCSD_SCREEN_DRAGON_BONES;
	static const std::string         ZCSD_SCREEN_DECRYPTION;
	static const std::string         ZCSD_SCREEN_ZALO;
        //popup
	static const std::string         ZCSD_POPUP_MINI_GAME;
        //images
	static const std::string         Slot1_png;
	static const std::string         placeholder_bg;
        // 3D Models
	static const std::string         BALL_HI_POLY;
	static const std::string         BALL_HI_POLY_2;
	static const std::string         BALL_3D;
	static const std::string         BALL_SHADOW;
	static const std::string         TABLE_3D;
	static const std::string         CUE_3D;
	static const std::string         LOBBY_3D;
	static const std::string         AUDIENCE_BOARD;
	static const std::string         NPC_3D;
	static const std::string         NPC_3D_TEXTURE_CLOTHES;
	static const std::string         NPC_3D_TEXTURE_SKIN;
	static const std::string         NPC_3D_TEXTURE_HAIR;
	static const std::string         NPC_3D_OPTIMIZE;
	static const std::string         NPC_3D_CLOTHES;
	static const std::string         NPC_IDLE;
	static const std::string         NPC_POSE_1;
	static const std::string         NPC_POSE_2;
	static const std::string         CUE_CHALLENGE_3D_YELLOW;
	static const std::string         CUE_CHALLENGE_3D_RED;
	static const std::string         CUE_CHALLENGE_3D_PINK;
	static const std::string         CUE_CHALLENGE_3D_BLUE;
	static const std::string         CUE_CHALLENGE_3D_TEX;
	static const std::string         CUE_VANG_1;
	static const std::string         CUE_VANG_2;
	static const std::string         CUE_DO_1;
	static const std::string         CUE_DO_2;
	static const std::string         CUE_XANH_1;
	static const std::string         CUE_XANH_2;
	static const std::string         CUE_TIM_1;
	static const std::string         CUE_TIM_2;
	static const std::string         CUE_TEXTURE_SET_1;
	static const std::string         CUE_TEXTURE_SET_1_MRAO;
	static const std::string         CUE_TEXTURE_SET_2;
	static const std::string         CUE_TEXTURE_SET_2_MRAO;
	static const std::string         OLD_NPC_3D;
	static const std::string         OLD_NPC_IDLE;
	static const std::string         OLD_NPC_POSE_1;
	static const std::string         OLD_NPC_POSE_2;

	static const int NUM_BALL_TEXTURE = 23;
	static const int NUM_NORMAL_BALL_TEXTURE = 22;
	static std::string Ball_Textures[NUM_BALL_TEXTURE];

	// fx Ingame
	static const std::string         YOUR_OPP_TURN;
	static const std::string         FOUL;
	static const std::string         FXMidGame;
	static const std::string         ROUND;
	static const std::string         STYLE_POINT;
	static const std::string         ROUND_END;

	//Lobby;
	static const std::string 	LOBBY_PLAY;
	static const std::string 	LOBBY_RANK;
	static const std::string 	LOBBY_AVATAR_FAKE;
	static const std::string 	LOBBY_CUE_BOX;
	static const std::string 	LOBBY_CUE_FAKE;
	static const std::string 	LOBBY_BUTTON_GROUP;
	static const std::string 	LOBBY_CLAN_BUTTON;
	static const std::string 	LOBBY_EVENT_BUTTON;
	static const std::string 	LOBBY_USER_INFO;
	static const std::string 	LOBBY_PLAY_DEFLECT_EFX;
	static const std::string 	LOBBY_RANKING_BUTTON;
	static const std::string 	LOBBY_SETTING_BUTTON;
	static const std::string 	LOBBY_SHOP_BUTTON;
	static const std::string 	LOBBY_TV;
	static const std::string 	LOBBY_ANNOUNCEMENT_BOX;
	static const std::string 	QUESTION_MARK;

	//INGAME;

	static const std::string 	INGAME_ARROW;
	static const std::string 	INGAME_ARROW_RED;
	static const std::string 	INGAME_AVATAR_1;
	static const std::string 	INGAME_AVATAR_2;
	static const std::string 	INGAME_AVATAR_3;
	static const std::string 	INGAME_AVATAR_FAKE_1;
	static const std::string 	INGAME_AVATAR_FAKE_2;
	static const std::string 	INGAME_AVATAR_FAKE_5;
	static const std::string 	INGAME_AVATAR_FAKE_6;
	static const std::string 	INGAME_AVATAR_FAKE_3;
	static const std::string 	INGAME_AVATAR_FAKE_4;
	static const std::string 	INGAME_BALL_BUTTON;
	static const std::string 	INGAME_BALL_TOUCH_INDICATOR;
	static const std::string 	INGAME_BLUE_BUTTON;
	static const std::string 	INGAME_PURPLE_BUTTON;
	static const std::string 	INGAME_CAMERA_2D_ICON;
	static const std::string 	INGAME_CAMERA_3D_ICON;
	static const std::string 	INGAME_CHAT;
	static const std::string 	INGAME_ANGLE_BOX;
	static const std::string 	INGAME_EXTENSION;
	static const std::string 	INGAME_EXTENSION_GRAY;
	static const std::string 	INGAME_LEVEL_BOX_01;
	static const std::string 	INGAME_LEVEL_BOX_02;
	static const std::string 	INGAME_MENU_ICON;
	static const std::string 	INGAME_MONEY;
	static const std::string 	INGAME_POWER_BAR;
	static const std::string 	INGAME_POWER_BAR_1;
	static const std::string 	INGAME_POWER_BAR_2;
	static const std::string 	INGAME_REWARD_BOX;
	static const std::string 	INGAME_RULER_BAR;
	static const std::string 	INGAME_RULER_BOX;
	static const std::string 	INGAME_RULER_BOX_STENCIL;
	static const std::string 	INGAME_SIDE_BAR;
	static const std::string 	INGAME_SIDE_BAR_SOLID;
	static const std::string 	INGAME_CUE;
	static const std::string 	INGAME_CUE_BOX_ICON;
	static const std::string 	INGAME_TIMER_1;
	static const std::string 	INGAME_TIMER_2;
	static const std::string 	INGAME_USER_INFO_BAR;
	static const std::string 	INGAME_GOLD_COIN;
	static const std::string 	INGAME_POCKET_INDI;
	static const std::string 	INGAME_ANGLE_ICON;
	static const std::string 	INGAME_ARROW_POCKET;

	static const std::string 	INGAME_EBP_RULER_BARS;
	static const std::string 	INGAME_EBP_RULER_BUTTON;
	static const std::string 	INGAME_EBP_AVATAR_MASK;
	static const std::string 	INGAME_EBP_POWER_BAR;
	static const std::string 	INGAME_FINGERPRINT;

	static const std::string 	BALLNUM_1;
	static const std::string 	BALLNUM_2;
	static const std::string 	BALLNUM_3;
	static const std::string 	BALLNUM_4;
	static const std::string 	BALLNUM_5;
	static const std::string 	BALLNUM_6;
	static const std::string 	BALLNUM_7;
	static const std::string 	BALLNUM_8;
	static const std::string 	BALLNUM_9;
	static const std::string 	BALLNUM_10;
	static const std::string 	BALLNUM_11;
	static const std::string 	BALLNUM_12;
	static const std::string 	BALLNUM_13;
	static const std::string 	BALLNUM_14;
	static const std::string 	BALLNUM_15;
	static const std::string	BALLNUMS[15];

	static const std::string	WHITE_DOT_TEXTURE;
	static const std::string	CYAN_DOT_TEXTURE;

	static const std::string 	network_operator_viettel_icon;
	static const std::string 	network_operator_mobi_icon;
	static const std::string 	network_operator_vina_icon;

	static const std::string font_console;
	static const std::string font_consolei;
	static const std::string font_consoleb;
	static const std::string font_consolez;

	static const std::string hdriNameTable;
	static const std::string hdriNameBall;
	static const std::string hdriNameBarcelona;

	static const std::string Cue_MRAO;
	static const std::string Table_MRAO[3];


	public:
		struct Base {
			const static std::string img_btn_disable;
			const static std::string img_btn_press;
			const static std::string img_btn_normal;
		};

		class GUI {
		public:
			const static std::string	EightBallModeSelect;
			const static std::string 	EightBallModeSelect_new;
			const static std::string 	ModeSelectItem;
			const static std::string 	FindMatch;
			const static std::string 	RT3LobbyLayer;
			const static std::string 	RT3EnterRoomID;
			const static std::string 	RT3EnterFriendID;
			const static std::string 	GambleGUI;
			const static std::string 	ResultGUI;
			const static std::string 	ResultGiftGUI;
			const static std::string 	ResultGUINoRematch;
			const static std::string 	ResultGUIRT3;
			const static std::string 	S2ResultGUI;
			const static std::string 	ResultGUIRewardNode;
			const static std::string 	Deprecated_SponsorshipLobbyLayer;
			const static std::string 	Deprecated_SponsorTile;
			const static std::string 	Deprecated_SponsorDetail;
			const static std::string 	Deprecated_SponsorSelect;
			const static std::string 	Deprecated_SponsorThumbnail;
			const static std::string 	Deprecated_SponsorUnlockPopup;
			const static std::string 	Lobby_UI;
			const static std::string 	Ingame_UI;
			const static std::string 	Ingame_UI_New;
			const static std::string 	Ingame_UI_V3;
			const static std::string 	Ingame_UI_V4;
			const static std::string	ImageStartMatchingUI;
			const static std::string	DisconnectPlayerPopup;

			const static std::string 	BallTypeNotify;
			const static std::string 	MoveBallTargetLayer;
			const static std::string 	PlayerInfoItem;

			const static std::string	StylePointCell;
			const static std::string	StylePointAchievement;
			const static std::string	StylePointDailyTask;
			const static std::string	StyleTotalPointUI;
			const static std::string	IngameStyleLayer;
			const static std::string	IngameMidFXUI;

			const static std::string IngameBaseUI;
			const static std::string IngameEBPUI;

			//EndGame
			const static std::string OpenCardLayer;
			const static std::string CardItem;
			const static std::string CardGift;
			const static std::string ResultGUILayer;
			const static std::string PlayerInfoRT3;
			const static std::string ScoreResult;

			// Sponsorship
			const static std::string 	SponsorshipUI;
			const static std::string 	SponsorshipItemFull;
			const static std::string 	SponsorshipItemSmall;
			const static std::string 	SponsorshipRewardClaimLayer;
			const static std::string 	SponsorshipRewardItem;

			// Global popup
			const static std::string 	PopupTwoChoice;
			const static std::string 	PopupConfirmOneChoice;
			const static std::string 	XLargePopup;
			const static std::string 	FullScreenPopup;

			// Inventory
			const static std::string 	InventoryElementUI;
			const static std::string 	InventoryItemUI;
			const static std::string 	InventoryList;
			const static std::string 	SmallCueUI;
			const static std::string 	CueRenameUI;
			const static std::string 	CueInspectUI;
			const static std::string 	CueInspectUIv2;
			const static std::string 	CueInspectUIv2_2;
			const static std::string 	CueUpgradeUI;
			const static std::string 	CueUpgradePopupv2;
			const static std::string 	CueSalvageSubUI;
			const static std::string 	CueUpgradeSubUI;
			const static std::string 	InventoryUIv2;
			const static std::string 	InventoryElementUIv2;
			const static std::string 	InventoryItemElementUIv2_2;

			// Gacha
			const static std::string 	CueGachaUI;
			const static std::string 	CueGachaRollingListUI;
			const static std::string 	CueGachaRollingUI;
			const static std::string 	BuyItemPopup;
			const static std::string 	CueGachaElementUI;
			const static std::string 	LoadingPopup;
			const static std::string 	StreakBox;

			// Debug
			const static std::string 	IngameDebugUI;
			const static std::string 	DebugDialog;
			const static std::string 	DebugDialogTab;
			const static std::string 	CheatS2Result;
			const static std::string 	CheatGold;
			const static std::string 	CheatInventoryItem;
			const static std::string 	CheatCue;
			const static std::string 	CheatAddCue;
			const static std::string 	CheatSponsorshipFinish;
			const static std::string 	CheatSponsorshipNewChallenge;
			const static std::string 	CheatGuildLineLength;
			const static std::string 	DebugNode;
			const static std::string 	DebugIngame;
			const static std::string 	CheatOffer;

			// Achievement
			const static std::string 	AchievementUI;
			const static std::string 	AchievementItem;
			const static std::string 	ReputationIcon;
			const static std::string 	ReputationLayer;

			//Friend GUI
			const static std::string 	FriendUI;
			const static std::string 	FriendItem;
			const static std::string 	PopupReceivedGift;
			const static std::string 	FriendSmallItem;
			const static std::string 	PopupInviteToRT3;
			const static std::string 	FriendUIV2;
			const static std::string 	FriendInviteTab;
			const static std::string 	FriendNodeInfo;
			const static std::string 	FriendTabListFriend;
			
			const static std::string 	ShopGUIv2;
			const static std::string 	ShopItemCuev2;
			const static std::string 	ShopGUI;
			const static std::string 	ShopGUINew;
			const static std::string 	ShopItem;
			const static std::string 	PaymentResultUI;
			const static std::string 	BankSelectUI;
			const static std::string 	BankItem;
			const static std::string 	SMSSelectUI;
			const static std::string 	ZingCardEnterUI;

			//Shop new
			const static std::string 	ShopGUIv3;
			const static std::string 	ShopTabItem;
			const static std::string 	ShopTabLimited;
			const static std::string 	ShopItemV3;
			const static std::string 	ShopItemLimitedV3;
			const static std::string 	ShopPopupConfirm;
			const static std::string 	ShopPopupResult;
			const static std::string 	ShopPopupSearch;
			const static std::string 	ShopItemCueBox;

			const static std::string 	SettingUI;

			// Workshop
			const static std::string 	WorkshopUI;
			const static std::string 	WorkshopUpgradeUI;
			const static std::string 	WorkshopUpgradeCueSelect;
			const static std::string 	WorkshopUpgradeCueItemUI;
			const static std::string 	WorkshopFusionUI;
			const static std::string 	WorkshopSwapUI;
			const static std::string 	WorkshopSwapGemItem;

			// Offer
			const static std::string OfferBaseUI;
			const static std::string OfferNodeItem;
			const static std::string OfferStarterUI;
			const static std::string OfferOnlyOneUI;
			const static std::string OfferItemUI;
			const static std::string OfferCumulativeUI;
			const static std::string PopupOpenChest;
			const static std::string OfferOnlyGoldUI;
			const static std::string OfferOnlyCashUI;

			// spin
			const static std::string SpinUI;

			// CueCollection
			const static std::string CueCollectionUI;
			const static std::string CueCollectionUIDetail;

			// Refactor
			const static std::string 	UnlockSponsorGUI;

			// Debug GUI
			const static std::string 	CueDebug;

			//Matching
			const static std::string 	ModeSelect;
			const static std::string 	ModeItem;

			static std::string getCsbPathFromJson(const std::string & jsonPath);

		};
};

#endif // !RESOURCE_H_
