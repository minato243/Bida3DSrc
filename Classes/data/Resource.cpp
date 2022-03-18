#include "Resource.h"

using namespace std;

std::string ingameUI(string name) {
	return "res/ui/ingame/" + name;
};

std::string lobbyUI(string name) {
	return "res/ui/lobby/" + name;
};

std::string ballnum(string name) {
	return "res/ui/ballnum/" + name;
}

std::string payment(string name) {
	return "res/ui/payment/" + name;
}

std::string jsonGUI(string name) {
	return "res/GUI/" + name;
}

//font
const std::string Res::FONT_BITMAP_NUMBER_1 = "fonts/number_1.fnt";

const std::string Res::FONT_BITMAP_DICE_NUMBER = "fonts/diceNumber.fnt";
const std::string Res::font_facebook = "GUI/Font/facebook.TTF";
const std::string Res::font_viet = "fonts/font_viet.ttf";

//zcsd
//screen
const std::string Res::ZCSD_SCREEN_MENU = "zcsd/screen_menu.json";
const std::string Res::ZCSD_SCREEN_NETWORK = "zcsd/screen_network.json";
const std::string Res::ZCSD_SCREEN_LOCALIZATION = "zcsd/screen_localize.json";
const std::string Res::ZCSD_SCREEN_DRAGON_BONES = "zcsd/screen_dragon_bones.json";
const std::string Res::ZCSD_SCREEN_DECRYPTION = "zcsd/screen_decryption.json";
const std::string Res::ZCSD_SCREEN_ZALO = "zcsd/screen_zalo.json";
//popup
const std::string Res::ZCSD_POPUP_MINI_GAME = "zcsd/game/mini_game/PopupMiniGame.json";

//images
const std::string Res::Slot1_png = "zcsd/slot1.png";
const std::string Res::placeholder_bg = "res/ui/bg.jpg";

// 3D Models
const std::string Res::BALL_HI_POLY = "res/3d/ball/Ball_Hi.c3b";
const std::string Res::BALL_HI_POLY_2 = "res/3d/ball/Ball_Hi.c3t";
const std::string Res::BALL_3D = "res/3d/ball/Ball_Number.c3b";
const std::string Res::BALL_SHADOW = "res/3d/ball/shadow.png";
const std::string Res::TABLE_3D = "res/3d/table/Table_Bida3D.c3b";
const std::string Res::CUE_3D = "res/3d/cue/Cue.c3b";
const std::string Res::LOBBY_3D = "res/3d/lobby_new/Lobby_Room.c3b";
const std::string Res::AUDIENCE_BOARD = "res/3d/lobby_new/Audience.c3b";
const std::string Res::NPC_3D = "res/3d/npc/body.c3b";
const std::string Res::NPC_3D_TEXTURE_CLOTHES = "res/3d/npc/BidaNPC_Clothes copy.png";
const std::string Res::NPC_3D_TEXTURE_SKIN = "res/3d/npc/BidaNPC_Skin copy.png";
const std::string Res::NPC_3D_TEXTURE_HAIR = "res/3d/npc/BidaNPC_Hair copy.png";
const std::string Res::NPC_3D_OPTIMIZE = "res/3d/npc/rig_2.c3b";
const std::string Res::NPC_3D_CLOTHES = "res/3d/npc/clothes.c3b";
const std::string Res::NPC_IDLE = "res/3d/npc/main_idle.c3b";
const std::string Res::NPC_POSE_1 = "res/3d/npc/pose_1.c3b";
const std::string Res::NPC_POSE_2 = "res/3d/npc/pose_2.c3b";

const std::string Res::CUE_CHALLENGE_3D_YELLOW = "res/3d/cue_challenge/Icon_Yellow.c3b";
const std::string Res::CUE_CHALLENGE_3D_RED = "res/3d/cue_challenge/Icon_Red.c3b";
const std::string Res::CUE_CHALLENGE_3D_PINK = "res/3d/cue_challenge/Icon_Pink.c3b";
const std::string Res::CUE_CHALLENGE_3D_BLUE = "res/3d/cue_challenge/Icon_Blue.c3b";
const std::string Res::CUE_CHALLENGE_3D_TEX = "res/3d/cue_challenge/Icon.png";

const std::string Res::CUE_VANG_1 = "res/3d/cue/Cue_New/Gay_vang_1.c3b";
const std::string Res::CUE_VANG_2 = "res/3d/cue/Cue_New/Gay_vang_2.c3b";
const std::string Res::CUE_DO_1 = "res/3d/cue/Cue_New/Gay_do_1.c3b";
const std::string Res::CUE_DO_2 = "res/3d/cue/Cue_New/Gay_do_2.c3b";
const std::string Res::CUE_XANH_1 = "res/3d/cue/Cue_New/Gay_xanh_1.c3b";
const std::string Res::CUE_XANH_2 = "res/3d/cue/Cue_New/Gay_xanh_2.c3b";
const std::string Res::CUE_TIM_1 = "res/3d/cue/Cue_New/Gay_tim_1.c3b";
const std::string Res::CUE_TIM_2 = "res/3d/cue/Cue_New/Gay_tim_2.c3b";
const std::string Res::CUE_TEXTURE_SET_1 = "res/3d/cue/Cue_New/Gay_Set1.png";
const std::string Res::CUE_TEXTURE_SET_1_MRAO = "res/3d/cue/Cue_New/Gay_Set1_MRAO.png";
const std::string Res::CUE_TEXTURE_SET_2 = "res/3d/cue/Cue_New/Gay_Set2.png";
const std::string Res::CUE_TEXTURE_SET_2_MRAO = "res/3d/cue/Cue_New/Gay_Set2_MRAO.png";

const std::string Res::OLD_NPC_3D = "res/3d/npc_old/body.c3b";
const std::string Res::OLD_NPC_IDLE = "res/3d/npc_old/main_idle.c3b";
const std::string Res::OLD_NPC_POSE_1 = "res/3d/npc_old/pose_1.c3b";
const std::string Res::OLD_NPC_POSE_2 = "res/3d/npc_old/pose_2.c3b";

string Res::Ball_Textures[NUM_BALL_TEXTURE] = {
		"res/3d/ball/Ball_No0.png",
		"res/3d/ball/Ball_No1.png",
		"res/3d/ball/Ball_No2.png",
		"res/3d/ball/Ball_No3.png",
		"res/3d/ball/Ball_No4.png",
		"res/3d/ball/Ball_No5.png",
		"res/3d/ball/Ball_No6.png",
		"res/3d/ball/Ball_No7.png",
		"res/3d/ball/Ball_No8.png",
		"res/3d/ball/Ball_No9.png",
		"res/3d/ball/Ball_No10.png",
		"res/3d/ball/Ball_No11.png",
		"res/3d/ball/Ball_No12.png",
		"res/3d/ball/Ball_No13.png",
		"res/3d/ball/Ball_No14.png",
		"res/3d/ball/Ball_No15.png",
		"res/3d/ball/Ball_No16.png",
		"res/3d/ball/Ball_No17.png",
		"res/3d/ball/Ball_No18.png",
		"res/3d/ball/Ball_No19.png",
		"res/3d/ball/Ball_No20.png",
		"res/3d/ball/Ball_No21.png",
		"res/3d/ball/Ball_No22.png"
};
// fx Ingame
const std::string Res::YOUR_OPP_TURN	= "res/effect/ingame/Turn";
const std::string Res::FOUL				= "res/effect/ingame/Foul";
const std::string Res::FXMidGame		= "res/effect/ingame/Chia2manhinh";
const std::string Res::ROUND			= "res/effect/ingame/Round";
const std::string Res::STYLE_POINT		= "res/effect/ingame/Text_diemphongcach";
const std::string Res::ROUND_END		= "res/effect/ingame/Tranquyetdinh";

//Lobby
const std::string Res::LOBBY_PLAY = "res/ui/lobby/PlayNow.png";
const std::string Res::LOBBY_RANK = "res/ui/lobby/PlayRank.png";
const std::string Res::LOBBY_AVATAR_FAKE = "res/ui/lobby/AvatarFake.png";
const std::string Res::LOBBY_CUE_BOX = "res/ui/lobby/CueBox.png";
const std::string Res::LOBBY_CUE_FAKE = "res/ui/lobby/CueFake.png";
const std::string Res::LOBBY_BUTTON_GROUP = "res/ui/lobby/ButtonGroup.png";
const std::string Res::LOBBY_CLAN_BUTTON = "res/ui/lobby/ClanButton.png";
const std::string Res::LOBBY_EVENT_BUTTON = "res/ui/lobby/EventButton.png";
const std::string Res::LOBBY_USER_INFO = "res/ui/lobby/InfoBorder.png";
const std::string Res::LOBBY_PLAY_DEFLECT_EFX = "res/ui/lobby/PlayDeflect.png";
const std::string Res::LOBBY_RANKING_BUTTON = "res/ui/lobby/RankingButton.png";
const std::string Res::LOBBY_SETTING_BUTTON = "res/ui/lobby/SettingButton.png";
const std::string Res::LOBBY_SHOP_BUTTON = "res/ui/lobby/ShopButton.png";
const std::string Res::LOBBY_TV = "res/ui/lobby/TV.png";
const std::string Res::LOBBY_ANNOUNCEMENT_BOX = "res/ui/lobby/AnnouncementBox.png";
const std::string Res::QUESTION_MARK = "res/ui/lobby/no_resource.png";

//INGAME

const std::string Res::INGAME_ARROW = "res/ui/ingame/Arrow.png";
const std::string Res::INGAME_ARROW_RED = "#Arrow_red.v3.png";
const std::string Res::INGAME_AVATAR_1 = "res/ui/ingame/Avatar_01.png";
const std::string Res::INGAME_AVATAR_2 = "res/ui/ingame/Avatar_02.png";
const std::string Res::INGAME_AVATAR_3 = "res/ui/ingame/Avatar_03.png";
const std::string Res::INGAME_AVATAR_FAKE_1 = "res/ui/ingame/AvatarFake5.jpg";
const std::string Res::INGAME_AVATAR_FAKE_2 = "res/ui/ingame/AvatarFake6.jpeg";
const std::string Res::INGAME_AVATAR_FAKE_5 = "res/ui/ingame/AvatarFake1.png";
const std::string Res::INGAME_AVATAR_FAKE_6 = "res/ui/ingame/AvatarFake2.png";
const std::string Res::INGAME_AVATAR_FAKE_3 = "res/ui/ingame/AvatarFake3.jpeg";
const std::string Res::INGAME_AVATAR_FAKE_4 = "res/ui/ingame/AvatarFake4.png";
const std::string Res::INGAME_BALL_BUTTON = "res/ui/ingame/Ball_Button.png";
const std::string Res::INGAME_BALL_TOUCH_INDICATOR = "res/ui/ingame/Ball_Touch_Indicator.png";
const std::string Res::INGAME_BLUE_BUTTON = "res/ui/ingame/Blue_Button_Normal.png";
const std::string Res::INGAME_PURPLE_BUTTON = "res/ui/ingame/Purple_Button_Normal.png";
const std::string Res::INGAME_CAMERA_2D_ICON = "res/ui/ingame/Camera_2D_Icon.png";
const std::string Res::INGAME_CAMERA_3D_ICON = "res/ui/ingame/Camera_3D_Icon.png";
const std::string Res::INGAME_CHAT = "res/ui/ingame/Chat_Icon.png";
const std::string Res::INGAME_ANGLE_BOX = "res/ui/ingame/Degree_Box.png";
const std::string Res::INGAME_EXTENSION = "res/ui/ingame/Extension_Button.png";
const std::string Res::INGAME_EXTENSION_GRAY = "res/ui/ingame/Extension_Button_Gray.png";
const std::string Res::INGAME_LEVEL_BOX_01 = "res/ui/ingame/Level_Box_01.png";
const std::string Res::INGAME_LEVEL_BOX_02 = "res/ui/ingame/Level_Box_02.png";
const std::string Res::INGAME_MENU_ICON = "res/ui/ingame/Menu_Icon.png";
const std::string Res::INGAME_MONEY = "res/ui/ingame/Money_Icon.png";
const std::string Res::INGAME_POWER_BAR = "res/ui/ingame/Power_Bar.png";
const std::string Res::INGAME_POWER_BAR_1 = "res/ui/ingame/Power_Bar_01.png";
const std::string Res::INGAME_POWER_BAR_2 = "res/ui/ingame/Power_Bar_02.png";
const std::string Res::INGAME_REWARD_BOX = "res/ui/ingame/Reward_Box.png";
const std::string Res::INGAME_RULER_BAR = "#Ruler_Box_01.v3.png";
const std::string Res::INGAME_RULER_BOX = "#Ruler_Box_02.v3.png";
const std::string Res::INGAME_RULER_BOX_STENCIL = "res/ui/ingame/Ruler_Box_Stencil.png";
const std::string Res::INGAME_SIDE_BAR = "#Ruler_Box_03.v3.png";
const std::string Res::INGAME_SIDE_BAR_SOLID = "#Ruler_Box_04.v3.png";
const std::string Res::INGAME_CUE = "res/ui/ingame/Stick.png";
const std::string Res::INGAME_CUE_BOX_ICON = "res/ui/ingame/Stick_Icon.png";
const std::string Res::INGAME_TIMER_1 = "res/ui/ingame/Timer_Bar_01.png";
const std::string Res::INGAME_TIMER_2 = "res/ui/ingame/Timer_Bar_02.png";
const std::string Res::INGAME_USER_INFO_BAR = "res/ui/ingame/Userinfo_Bar.png";
const std::string Res::INGAME_GOLD_COIN = "#Gold.png";
const std::string Res::INGAME_POCKET_INDI = "res/ui/ingame/pocket_indi.png";
const std::string Res::INGAME_ANGLE_ICON = "#Angle.v3.png";


const std::string Res::INGAME_ARROW_POCKET = "res/ui/ingame/Arrow_Pocket.png";

const std::string Res::INGAME_FINGERPRINT = "res/ui/ingame/FingerPrint.png";

const std::string Res::INGAME_EBP_RULER_BARS = "#IngameEBP_Ruler_Bars.png";

const std::string Res::INGAME_EBP_RULER_BUTTON = "#IngameEBP_Ruler_Button.png";

const std::string Res::INGAME_EBP_AVATAR_MASK = "#IngameEBP_Avatar_Mask.png";

const std::string Res::INGAME_EBP_POWER_BAR = "#IngameEBP_Power_Bar.png";

const std::string Res::BALLNUM_1 = "res/ui/ballnum/Ball_01.png";
const std::string Res::BALLNUM_2 = "res/ui/ballnum/Ball_02.png";
const std::string Res::BALLNUM_3 = "res/ui/ballnum/Ball_03.png";
const std::string Res::BALLNUM_4 = "res/ui/ballnum/Ball_04.png";
const std::string Res::BALLNUM_5 = "res/ui/ballnum/Ball_05.png";
const std::string Res::BALLNUM_6 = "res/ui/ballnum/Ball_06.png";
const std::string Res::BALLNUM_7 = "res/ui/ballnum/Ball_07.png";
const std::string Res::BALLNUM_8 = "res/ui/ballnum/Ball_08.png";
const std::string Res::BALLNUM_9 = "res/ui/ballnum/Ball_09.png";
const std::string Res::BALLNUM_10 = "res/ui/ballnum/Ball_10.png";
const std::string Res::BALLNUM_11 = "res/ui/ballnum/Ball_11.png";
const std::string Res::BALLNUM_12 = "res/ui/ballnum/Ball_12.png";
const std::string Res::BALLNUM_13 = "res/ui/ballnum/Ball_13.png";
const std::string Res::BALLNUM_14 = "res/ui/ballnum/Ball_14.png";
const std::string Res::BALLNUM_15 = "res/ui/ballnum/Ball_15.png";
const std::string Res::BALLNUMS[15] = {BALLNUM_1, BALLNUM_2, BALLNUM_3, BALLNUM_4, BALLNUM_5, 
BALLNUM_6, BALLNUM_7, BALLNUM_8, BALLNUM_9, BALLNUM_10,
BALLNUM_11, BALLNUM_12, BALLNUM_13, BALLNUM_14, BALLNUM_15};

const std::string Res::WHITE_DOT_TEXTURE = "res/Texture/white_dot.png";
const std::string Res::CYAN_DOT_TEXTURE = "res/Texture/cyan_dot.png";

const std::string Res::network_operator_viettel_icon = "res/ui/payment/store_popup_tab_viettel_i5.png";
const std::string Res::network_operator_mobi_icon = "res/ui/payment/store_popup_tab_mobi_i5.png";
const std::string Res::network_operator_vina_icon = "res/ui/payment/store_popup_tab_vina_i5.png";

const std::string Res::font_console = "res/GUI/Font/consola.ttf";
const std::string Res::font_consolei = "res/GUI/Font/consolai.ttf";
const std::string Res::font_consoleb = "res/GUI/Font/consolab.ttf";
const std::string Res::font_consolez = "res/GUI/Font/consolaz.ttf";

const std::string Res::hdriNameTable = "room";
const std::string Res::hdriNameBall = "room2";
const std::string Res::hdriNameBarcelona = "circus";

const std::string Res::Cue_MRAO = "res/3d/cue/Cue_MRAO_Map.png";

const std::string Res::Table_MRAO[3] = {
	"res/3d/table/Table_Metal_MRAO.png",
		"res/3d/table/Table_ETC_MRAO.png",
		"res/3d/table/Table_Mat_MRAO.png"};

const std::string Res::GUI::EightBallModeSelect = jsonGUI("ModeSelect/8BallModeSelect.json");
const std::string Res::GUI::EightBallModeSelect_new = jsonGUI("ModeSelect/8BallModeSelect_New.json");
const std::string Res::GUI::ModeSelectItem = jsonGUI("ModeSelect/ModeSelectItem.json");
const std::string Res::GUI::FindMatch = jsonGUI("FindMatch/FindMatch.json");
const std::string Res::GUI::RT3LobbyLayer = jsonGUI("FindMatch/RT3FindMatch.json");
const std::string Res::GUI::RT3EnterRoomID = jsonGUI("FindMatch/RT3EnterRoomID.json");
const std::string Res::GUI::RT3EnterFriendID = jsonGUI("FindMatch/RT3InviteFriend.json");
const std::string Res::GUI::GambleGUI = jsonGUI("Game/GambleGUI.json");
const std::string Res::GUI::ResultGUI = jsonGUI("Game/ResultGUI.json");
const std::string Res::GUI::ResultGiftGUI = jsonGUI("Game/ResultGift.json");
const std::string Res::GUI::ResultGUINoRematch = jsonGUI("Game/ResultGUINoRematch.json");
const std::string Res::GUI::ResultGUIRT3 = jsonGUI("Game/ResultGUIRT3.json");
const std::string Res::GUI::S2ResultGUI = jsonGUI("Game/ResultGUI.json");
const std::string Res::GUI::ResultGUIRewardNode = jsonGUI("Game/ResultGUIRewardNode.json");
const std::string Res::GUI::Deprecated_SponsorshipLobbyLayer = jsonGUI("MainLobby/Sponsor/SponsorLayer.json");
const std::string Res::GUI::Deprecated_SponsorTile = jsonGUI("MainLobby/Sponsor_Deprecated/SponsorTile.json");
const std::string Res::GUI::Deprecated_SponsorDetail = jsonGUI("MainLobby/Sponsor_Deprecated/SponsorDetail.json");
const std::string Res::GUI::Deprecated_SponsorSelect = jsonGUI("MainLobby/Sponsor_Deprecated/SponsorSelect.json");
const std::string Res::GUI::Deprecated_SponsorThumbnail = jsonGUI("MainLobby/Sponsor/SponsorThumbnail.json");
const std::string Res::GUI::Deprecated_SponsorUnlockPopup = jsonGUI("MainLobby/Sponsor/SponsorUnlockPopup.json");
const std::string Res::GUI::Lobby_UI = jsonGUI("MainLobby/LobbyUILayer.json");
const std::string Res::GUI::Ingame_UI = jsonGUI("Game/IngameLayer.json");
const std::string Res::GUI::Ingame_UI_New = jsonGUI("Game/IngameLayerNew.json");
const std::string Res::GUI::Ingame_UI_V3 = jsonGUI("Game/IngameLayer.v3.json");
const std::string Res::GUI::Ingame_UI_V4 = "res/GUI/Game/IngameLayer.v4.json";
const std::string Res::GUI::ImageStartMatchingUI = jsonGUI("Game/IngameStartMatching.json");
const std::string Res::GUI::DisconnectPlayerPopup = jsonGUI("Game/DisconnectPlayerPopup.json");

const std::string Res::GUI::BallTypeNotify = jsonGUI("Game/BallTypeNotify.json");
const std::string Res::GUI::MoveBallTargetLayer = jsonGUI("Game/MoveBallTargetLayer.json");
const std::string Res::GUI::PlayerInfoItem = jsonGUI("FindMatch/PlayerInfoItem.json");
const std::string Res::GUI::IngameBaseUI = jsonGUI("Game/IngameLayerBase.json");
const std::string Res::GUI::IngameEBPUI = jsonGUI("Game/IngameLayerEbp.json");

const std::string Res::GUI::StylePointCell = jsonGUI("Game/StylePointCell.json");
const std::string Res::GUI::StylePointAchievement = jsonGUI("Game/StylePointAchievement.json");
const std::string Res::GUI::StylePointDailyTask = jsonGUI("FindMatch/StylePointDailyTask.json");
const std::string Res::GUI::StyleTotalPointUI = jsonGUI("Game/StyleTotalPoint.json");
const std::string Res::GUI::IngameStyleLayer = jsonGUI("Game/IngameStyleLayer.json");
const std::string Res::GUI::IngameMidFXUI = jsonGUI("Game/IngameMidFX.json");

//Endgame 

const std::string Res::GUI::OpenCardLayer = jsonGUI("EndGame/OpenCardLayer.json");
const std::string Res::GUI::CardItem = jsonGUI("EndGame/CardItemNode.json");
const std::string Res::GUI::CardGift = jsonGUI("EndGame/CardResultGift.json");
const std::string Res::GUI::ResultGUILayer = jsonGUI("EndGame/ResultGUILayer.json");
const std::string Res::GUI::PlayerInfoRT3 = jsonGUI("EndGame/PlayerInfoRT3.json");
const std::string Res::GUI::ScoreResult = jsonGUI("EndGame/ScoreResult.json");

// Sponsorship
const std::string Res::GUI::SponsorshipUI = jsonGUI("MainLobby/Sponsorship/SponsorshipUI.json");
const std::string Res::GUI::SponsorshipItemFull = jsonGUI("MainLobby/Sponsorship/SponsorshipItemFull.json");
const std::string Res::GUI::SponsorshipItemSmall = jsonGUI("MainLobby/Sponsorship/SponsorshipItemSmall.json");
const std::string Res::GUI::SponsorshipRewardClaimLayer = jsonGUI("MainLobby/Sponsorship/SponsorClaimRewardLayer.json");
const std::string Res::GUI::SponsorshipRewardItem = jsonGUI("MainLobby/Sponsorship/RewardNode.json");

// Global popup
const std::string Res::GUI::PopupTwoChoice = jsonGUI("GlobalPopup/PopupTwoChoice2.json");
const std::string Res::GUI::PopupConfirmOneChoice = jsonGUI("GlobalPopup/PopupConfirm.json");
const std::string Res::GUI::XLargePopup = jsonGUI("GlobalPopup/XLargePopup.json");
const std::string Res::GUI::FullScreenPopup = jsonGUI("GlobalPopup/FullScreenPopup.json");

// Inventory
const std::string Res::GUI::InventoryElementUI = jsonGUI("Inventory/InventoryElementUI.json");
const std::string Res::GUI::InventoryItemUI = jsonGUI("Inventory/InventoryElementItemUI.json");
const std::string Res::GUI::InventoryList = jsonGUI("Inventory/InventoryListLayer.json");
const std::string Res::GUI::SmallCueUI = jsonGUI("Inventory/SmallCueUI.json");
const std::string Res::GUI::CueRenameUI = jsonGUI("Inventory/RenameCueUI.json");
const std::string Res::GUI::CueInspectUI = jsonGUI("Inventory/CueInspectLayer.json");
const std::string Res::GUI::CueInspectUIv2 = jsonGUI("Inventory/CueInspectLayerv2.1.json");
const std::string Res::GUI::CueInspectUIv2_2 = jsonGUI("Inventory/CueInspectLayerv2.2.json");
const std::string Res::GUI::CueUpgradeUI = jsonGUI("Inventory/CueUpgradeUI.json");
const std::string Res::GUI::CueUpgradePopupv2 = jsonGUI("Inventory/CueUpgradeV2Popup.json");
const std::string Res::GUI::CueSalvageSubUI = jsonGUI("Inventory/CueSalvageSubUI.json");
const std::string Res::GUI::CueUpgradeSubUI = jsonGUI("Inventory/CueUpgradeSubUI.json");
const std::string Res::GUI::InventoryUIv2 = jsonGUI("Inventory/InventoryUIv2.1.json");
const std::string Res::GUI::InventoryElementUIv2 = jsonGUI("Inventory/InventoryElementUIv2.1.json");
const std::string Res::GUI::InventoryItemElementUIv2_2 = jsonGUI("Inventory/InventoryItemElementUIv2.2.json");

// Gacha
const std::string Res::GUI::CueGachaUI = jsonGUI("Gacha/GachaUI.json");
const std::string Res::GUI::CueGachaRollingListUI = jsonGUI("Gacha/CueGachaRollingList.json");
const std::string Res::GUI::CueGachaRollingUI = jsonGUI("Gacha/CueGachaRolling.json");
const std::string Res::GUI::BuyItemPopup = jsonGUI("Gacha/BuyItemPopup.json");
const std::string Res::GUI::CueGachaElementUI = jsonGUI("Gacha/GachaCueElementUI_new.json");
const std::string Res::GUI::LoadingPopup = jsonGUI("GlobalPopup/LoadingPopup.json");
const std::string Res::GUI::StreakBox = jsonGUI("Game/StreakNode.json");

// Debug
const std::string Res::GUI::IngameDebugUI = jsonGUI("GlobalPopup/DebugIngameDialog.json");
const std::string Res::GUI::DebugDialog = jsonGUI("GlobalPopup/DebugScreen.json");
const std::string Res::GUI::DebugDialogTab = jsonGUI("GlobalPopup/DebugDialogTab.json");
const std::string Res::GUI::CheatS2Result = jsonGUI("GlobalPopup/CheatS2Result.json");
const std::string Res::GUI::CheatGold = jsonGUI("GlobalPopup/CheatGold.json");
const std::string Res::GUI::CheatInventoryItem = jsonGUI("GlobalPopup/CheatInventoryItem.json");
const std::string Res::GUI::CheatCue = jsonGUI("GlobalPopup/CheatCue.json");
const std::string Res::GUI::CheatAddCue = jsonGUI("GlobalPopup/CheatAddCue.json");
const std::string Res::GUI::CheatSponsorshipFinish = jsonGUI("GlobalPopup/QuickFinishSponsorship.json");
const std::string Res::GUI::CheatSponsorshipNewChallenge = jsonGUI("GlobalPopup/CheatSponsorshipChallenges.json");
const std::string Res::GUI::CheatGuildLineLength = jsonGUI("GlobalPopup/CheatIngameGuildline.json");
const std::string Res::GUI::DebugNode = jsonGUI("GlobalPopup/DebugFloatingNode.json");
const std::string Res::GUI::DebugIngame = jsonGUI("GlobalPopup/CheatIngame.json");
const std::string Res::GUI::CheatOffer = jsonGUI("GlobalPopup/CheatDisplayOffer.json");

// Achievement
const std::string Res::GUI::AchievementUI = jsonGUI("Achievement/AchievementLayer.json");
const std::string Res::GUI::AchievementItem = jsonGUI("Achievement/AchievementItem.json");
const std::string Res::GUI::ReputationIcon = jsonGUI("Achievement/ReputationIcon.json");
const std::string Res::GUI::ReputationLayer = jsonGUI("Achievement/ReputationLayer.json");

//Friend GUI
const std::string Res::GUI::FriendUI = jsonGUI("Friend/FriendUI.json");
const std::string Res::GUI::FriendItem = jsonGUI("Friend/FriendItem.json");
const std::string Res::GUI::PopupReceivedGift = jsonGUI("Friend/PopupReceivedGift.json");
const std::string Res::GUI::FriendSmallItem = jsonGUI("Friend/FriendSmallItem.json");
const std::string Res::GUI::PopupInviteToRT3 = jsonGUI("FindMatch/InviteModeRT3.json");
const std::string Res::GUI::FriendUIV2 = jsonGUI("Friend/FriendUIV2.json");
const std::string Res::GUI::FriendInviteTab = jsonGUI("Friend/FriendTabInvite.json");
const std::string Res::GUI::FriendNodeInfo = jsonGUI("Friend/NodeFriend.json");
const std::string Res::GUI::FriendTabListFriend = jsonGUI("Friend/FriendTabListFriend.json");

const std::string Res::GUI::ShopGUIv2 = jsonGUI("Shop/ShopUIv2.1.json");
const std::string Res::GUI::ShopItemCuev2 = jsonGUI("Shop/ShopCueItemv2.1.json");
const std::string Res::GUI::ShopGUI = jsonGUI("Shop/ShopLayer.json");
const std::string Res::GUI::ShopGUINew = jsonGUI("Shop/ShopLayer_new.json");
const std::string Res::GUI::ShopItem = jsonGUI("Shop/ShopItem.json");
const std::string Res::GUI::PaymentResultUI = jsonGUI("Payment/PaymentResultUI.json");
const std::string Res::GUI::BankSelectUI = jsonGUI("Payment/BankSelectUI.json");
const std::string Res::GUI::BankItem = jsonGUI("Payment/BankItem.json");
const std::string Res::GUI::SMSSelectUI = jsonGUI("Payment/SMSSelectUI.json");
const std::string Res::GUI::ZingCardEnterUI = jsonGUI("Payment/ZingCardEnterUI.json");

//Shop new
const std::string Res::GUI::ShopGUIv3 = jsonGUI("Shop/ShopUI_new.json");
const std::string Res::GUI::ShopTabItem = jsonGUI("Shop/ShopTab/ShopTabItem.json");
const std::string Res::GUI::ShopTabLimited = jsonGUI("Shop/ShopTab/ShopTabLimited.json");
const std::string Res::GUI::ShopItemV3 = jsonGUI("Shop/ShopTab/ShopItem.json");
const std::string Res::GUI::ShopItemLimitedV3 = jsonGUI("Shop/ShopTab/ShopItemLimited.json");
const std::string Res::GUI::ShopPopupConfirm = jsonGUI("Shop/ShopTab/ShopPopupConfirm.json");
const std::string Res::GUI::ShopPopupResult = jsonGUI("Shop/ShopTab/ShopPopupError.json");
const std::string Res::GUI::ShopPopupSearch = jsonGUI("Shop/ShopTab/ShopSearchGUI.json");
const std::string Res::GUI::ShopItemCueBox = jsonGUI("Shop/ShopTab/ShopItemCueBox.json");

const std::string Res::GUI::SettingUI = jsonGUI("Setting/SettingUI.json");

// Workshop
const std::string Res::GUI::WorkshopUI = jsonGUI("Workshop/WorkshopUILayer.json");
const std::string Res::GUI::WorkshopUpgradeUI = jsonGUI("Workshop/WorkshopUpgrade.json");
const std::string Res::GUI::WorkshopUpgradeCueSelect = jsonGUI("Workshop/WorkshopUpgradeCueSelect.json");
const std::string Res::GUI::WorkshopUpgradeCueItemUI = jsonGUI("Workshop/WorkshopUpgradeCueItemUI.json");
const std::string Res::GUI::WorkshopFusionUI = jsonGUI("Workshop/WorkshopFusion.json");
const std::string Res::GUI::WorkshopSwapUI = jsonGUI("Workshop/WorkshopSwap.json");
const std::string Res::GUI::WorkshopSwapGemItem = jsonGUI("Workshop/WorkshopSwapGemItem.json");

const std::string Res::GUI::CueCollectionUI = jsonGUI("CueCollection/CueCollectionUI.json");
const std::string Res::GUI::CueCollectionUIDetail = jsonGUI("CueCollection/CueCollectionUIDetail.json");

// Offer
const std::string Res::GUI::OfferBaseUI = jsonGUI("Offer/OfferBaseUI.json");
const std::string Res::GUI::OfferNodeItem = jsonGUI("Offer/OfferNodeItem.json");
const std::string Res::GUI::OfferStarterUI = jsonGUI("Offer/OfferStarterUIV2.json");
const std::string Res::GUI::OfferOnlyOneUI = jsonGUI("Offer/OfferOnlyUI.json");
const std::string Res::GUI::OfferItemUI = jsonGUI("Offer/OfferStarterItem.json");
const std::string Res::GUI::OfferCumulativeUI = jsonGUI("Offer/OfferCumulative.json");
const std::string Res::GUI::PopupOpenChest = jsonGUI("Offer/PopupOpenChest.json");
const std::string Res::GUI::OfferOnlyGoldUI = jsonGUI("Offer/OfferOnlyGold.json");
const std::string Res::GUI::OfferOnlyCashUI = jsonGUI("Offer/OfferOnlyCash.json");

// spin
const std::string Res::GUI::SpinUI = jsonGUI("Spin/SpinPopup.json");

// Refactor
const std::string Res::GUI::UnlockSponsorGUI = jsonGUI("Sponsorship/UnlockSponsorSlot.json");

// Debug GUI
const std::string Res::GUI::CueDebug = jsonGUI("PlaceholderGUI/CueGUI.json");

//Matching
const std::string Res::GUI::ModeSelect = jsonGUI("Matching/ModeSelect.json");
const std::string Res::GUI::ModeItem = jsonGUI("Matching/ModeItem.json");

std::string Res::GUI::getCsbPathFromJson(const std::string & jsonPath)
{
	std::string filePath = std::string(jsonPath);
	filePath.replace(filePath.end() - 5, filePath.end(), ".csb");
	return filePath;
}

const string Res::Base::img_btn_disable = "res/Default/Button_Disable.png";
const string Res::Base::img_btn_press = "res/Default/Button_Disable.png";
const string Res::Base::img_btn_normal = "res/Default/Button_Normal.png";