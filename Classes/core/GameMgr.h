#ifndef GameMgr_h__
#define GameMgr_h__

#include "gui/BaseLayer.h"
#include <modules/core-implement/Table.h>
#include <modules/ui/ingame/IngameUI.h>
#include <modules/core/interaction/TouchManager.h>
#include <modules/camera-view/GameCamera.h>
#include <modules/core/match/EightBallPollCtrl.h>
#include <modules/user/User.h>
#include <modules/ui/ingame/IngameBaseUI.h>
#include "modules/core/ps/TrajectoryBuilder.h"
#include "modules/core/ps/internal/CueAngle.h"
#include "modules/ui/lobby/LobbyUI.h"

#define gameMgr GameMgr::getInstance()

class GameMgr
{
public:
	GameMgr();
	~GameMgr();
	static GameMgr* getInstance();
public:
	void start();
	void initCheat();
	void initPlugin();
	void initModule();
	void addSearchPath();
	void preloadGUI();
	void loadCache();
	void cleanUserData();
	void updatePayments(bool enablePayment, std::vector<bool> payments);
	void requestCheckAppVersion();
	void requestCountryID();
	void onEnterScene();
	void onUpdateScene(float dt);	
	

	void onCheckOpenLogin();
	// open scene
	BaseLayer* openLoading();

	BaseLayer* openLogin();

	BaseLayer* openLobby();

	bool openShop();

	bool checkSupportBean();

	bool checkEnablePayment();

	std::string getInstallDate();

	int getPlatform();

	std::string getDeviceID();

	std::string getCountryID();

	bool isGlobal();

	bool isCurPlayer(int pId);

	void startGameScene();

	void startTestScene();

	void initInteraction();

	void initTouchManager();
	
	//ingame
	float onChangePower(float percent);

	void shoot(float percent);

	void shootByForce(double force, bool opponentShoot = false);

	double calcForce(float percent);

	void updateTrajectory(double force = 0);

	void updateBasicGuildLine();

	void setEnableGuildLine(bool enable);

	void update(float dt);

	Ball *getSuggestedBall();

	CueInfo *getCurrentCueInfo();

	void addUserInfo(int userId, User *user);

	void setBallMode2D(bool val);

	void onCallPocketEnded();

	void updateUserInfo(UserInfoInPacket *data);
public:
	static GameMgr* _instance;
	int nServicesAPI;
	Table *_table;
	IngameBaseUI *_ingameUI;
	LobbyUI *_lobbyUI;
	cocos2d::Scene *_scene;
	TouchManager *_touchManager;
	GameCamera *_camera;
	CueAngle *_cueAngle;

	float _force;
	TrajectoryBuilder *_trajectoryBuilder;

	EightBallPollCtrl *_ebpCtrl;
	User *_userInfo;
	bool _startedShot;
private:
	bool _enableGuildLine;
};


#endif // GameMgr_h__
