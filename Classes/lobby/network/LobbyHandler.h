#ifndef LobbyHandler_h__
#define LobbyHandler_h__
#include "BaseFramework.h"

class LobbyHandler
{
public:
	LobbyHandler();
	~LobbyHandler();

public:
	void onFinishConnect(bool isSuccess);
	void onDisconnected();
	void onReceived(fr::InPacket* pk);
public:

	static void send(fr::OutPacket* msg);
	static void requestHandshake();
	static void requestLogin(const std::string sessionkey, const std::string deviceId, int plaform, bool isLoginGuest);
	static void requestMobile(std::string deviceName, std::string osVersion, int platformId, std::string deviceID, std::string installDate);
	static void requestGetConfig();
	static void requestUserInfo();
	static void requestSelectChannel(int channelID);
	static void requestUpdateInfo();
	static void requestJoinRoom(int channelID, int id);
	static void requestFinishTutorial(int step);
	static void requestQuickPlay(int index);
	static void requestGetSupportBean();
	static void requestGiftInfo();
	static void requestPingpong();
	static void requestTutorialSuccess();
	static void requestChatTotal(char typeMsg, int toId, std::string msg, int emoticonIndex);
	static void requestPurchaseIAPApple(std::string receiptData);
	static void requestConfigShop(char type, int version = 0);

public:
	long long _listenerID;
	long long timeHandShake;
};

#endif // LobbyHandler_h__
