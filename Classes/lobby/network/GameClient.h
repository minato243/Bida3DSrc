#ifndef GameClient_h__
#define GameClient_h__

#include "BaseFramework.h"
#include "LobbyHandler.h"
#include "GameClient.h"
#include "modules/network/user/UserHandler.h"
#include "modules/network/match/MatchHandler.h"
#include "modules/network/game/GameHandler.h"
#include "modules/network/base/BaseHandler.h"

enum ConnectState {
	DISCONNECTED = 0,
	CONNECTING = 1,
	CONNECTED = 2,
	NEED_QUIT = 3
};


class GameClient
{
public:
	GameClient();
	~GameClient();
	static GameClient* getInstance();
	static void destroyInstance();
public:
	fr::GsnClient* getNetwork();
	void connect(std::string server = "", int port = 0);
	void sendPacket(fr::OutPacket* msg);
	void disconnect();
	void retryConnectInGame();
	void retryManualConnect();
	void startPingPong();
	void receivePingPong();
	void destroy();
	void addHandler(BaseHandler *handler);

	static void disconnectHandle();
	static void connectFailedHandle();
	static void processRetryConnect();
	static void showNetworkFail();

public:
	fr::GsnClient* _tcpClient;
	LobbyHandler* lobbyHandler;
	UserHandler *userHandler;

	std::vector<BaseHandler*> _handlerList;

	long long _clientID;
	ConnectState connectState;
	bool connectServer;
	int timeSendLatency;

	static GameClient* _instance;
	static int connectCount;
	static int retryCount;
	static bool reconnect;



	static bool holding;
	static int holdingRoom;
	static std::string holdingPass;
	static bool connectLai;
public:
	static const int MAX_RETRY_CONNECT;
	static const int MAX_NETWORK_UNSTABLE;
	static const int TIME_OUT_RETRY;

	static const int PINGPONG_TIMEOUT;
	static const int PINGPONG_INTERVAL;
	static const int NETWORK_SLOW_NOTIFY;
	static const std::vector<float> NETWORK_STATE_TIME_NOTIFY;
};

#define pingPongHandler PingPongHandler::getInstance()
class PingPongHandler {
public:
	PingPongHandler();
	~PingPongHandler();
	static PingPongHandler* getInstance();
public:
	void startPing();
	void receivePing();
	void sendPing();
	void checkNeedPing();
	void autoDisconnect();
	void updatePing(float dt);
public:
	static PingPongHandler* _instance;

	float timePingPong;
	float timeDelayPingPong;
	float timeRetryPingPong;
	std::vector<float> arStateTime;

	bool isRetryPingpong;
	bool isDelayPingpong;
	bool isPingpong;

	bool isNetworkSlow;

	bool isStopHandler;
};

#define timeoutConnectHandler TimeoutConnectHandler::getInstance()
class TimeoutConnectHandler {
public:
	TimeoutConnectHandler();
	~TimeoutConnectHandler();

	static TimeoutConnectHandler* getInstance();
public:
	void startCountDown();
	void clearCountDown();
	void doTimeoutConnect();
	void updateCountDown(float dt);
public:
	float timeCountDown;
	bool isWaitConnect;
	static TimeoutConnectHandler* _inst;
};
#endif // GameClient_h__
