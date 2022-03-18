#ifndef LoginMgr_h__
#define LoginMgr_h__
#include <string>

#define loginMgr LoginMgr::getInstance()

class LoginMgr {
public:	
	struct SessionInfo {
		int error;
		std::string sessionKey;
	};
	LoginMgr();
	~LoginMgr();

	static LoginMgr* getInstance();
public:
	void loadCache();
	bool checkCurrentSocial(int social);
	bool checkNetwork();
	bool checkDeviceSupport(std::string social = "");
	void setDefaultLogin(int dLogin);
	void clearDefaultLogin();
	void loginFacebook();
	void loginGoogle();
	void loginAppleID();
	void loginGuest();
	void loginDev(std::string uid);
	void startConnectServer(std::string server = "", int port = 0);
	void stopConnectServer();
	void onLoginSuccess();
	void onLogout();
	void backToLoginScene();
	void saveServerIP(std::string ip, int port);
	std::string getServerIP();
	int getServerPort();
	std::string getGuestID();
	void onResponseAccessToken(bool res, std::string token);
	void onResponseSessionKey(bool res, std::string data);
	std::string getSessionKey();
	bool checkCacheSessionKey();
	std::string getCacheSessionKey();
	int getCurrentSocial();
	std::string getCurrentSocialName();
	void fetchSessionKey();
	void fetchSessionKeyGuest();
	int getDefaultLogin();
	int getCurrentLogin();
	void saveCurrentLogin();
	void clearAutoLogin();
	SessionInfo parseSessionKey(bool res, std::string ss);
public:
	int defaultLogin;
	int currentSocial;

	std::string accessToken;
	std::string	sessionKey;
	std::string openID;

	std::string ipServer;
	int portServer;

	static LoginMgr* _instance;
};
#endif // LoginMgr_h__
