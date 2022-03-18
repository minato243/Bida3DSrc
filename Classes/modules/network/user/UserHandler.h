#ifndef USER_HANDLER_H_
#define USER_HANDLER_H_

#include "BaseFramework.h"
#include "UserInPacket.h"

class UserHandler
{
public:
	UserHandler();
	~UserHandler();

	void onFinishConnect(bool isSuccess);
	void onDisconnected();
	void onReceived(fr::InPacket* pk);

	void getUserInfo();

public:
	long long _listenerID;
	long long timeHandShake;

public:
	static void send(fr::OutPacket* msg);
	static void requestHandshake();
	static void requestLogin(const std::string sessionkey, const std::string deviceId, int plaform, bool isLoginGuest);

};

#endif // !USER_HANDLER_H_

