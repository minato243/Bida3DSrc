#ifndef CheckLogic_h__
#define CheckLogic_h__

#include "cocos2d.h"
#include "network/LobbyPackets.h"

class CheckLogic {

public:
	static bool checkQuickPlay();
	//static void onUpdateMoney(update);//ko dung
	static bool checkInBoard();
	static void showNotifyNetworkSlow();
	static void updatePing(float ping);
	static std::string getDialogClassName();
	static std::string getUserInfoClassName();
	static cocos2d::Vec2 getPosFromPlayer(int id);
	static std::string getCardResource(int id);
	static bool checkPlayNewEmoticon() { return true; };
};
#endif // CheckLogic_h__
