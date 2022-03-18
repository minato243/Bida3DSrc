#ifndef BroadcastMgr_h__
#define BroadcastMgr_h__

#include "cocos2d.h"
#include "cocos/ui/CocosGUI.h"
#include "extensions/cocos-ext.h"
#include "cocostudio/CocoStudio.h"

class Broadcast {
public:
	static const float UPDATE_DELTA_TIME;
	static const float WIDTH_RATIO_DEFAULT;
	static const cocos2d::Vec2 ANCHOR_DEFAULT;

	static const int MESSAGE_NORMAL;
	static const int MESSAGE_RICH;

	static const std::string TYPE_SYSTEM;
	static const std::string TYPE_EVENT;

	static const int TIMEOUT_MESSAGE;
};

class BroadcastNode :
	public cocos2d::Node {
public:
	BroadcastNode();
	~BroadcastNode();
	static BroadcastNode* create(std::string message, int type, cocos2d::Vec2 pos, int rp = 0);
	bool init(std::string message, int type, cocos2d::Vec2 pos, int rp = 0);
public:
	void createMessage(std::string message, cocos2d::Vec2 pos);
	void clipNode(cocos2d::Vec2 pos);
	void doPlay();
	void doReload();
	void doFinish();
	void doUpdate(float dt);
public:
	cocos2d::ui::Text* lbMessage;
	bool doMessage;
	float moveX;
	int numRepeat;
	bool isDead;
	cocos2d::Sprite* sprite;
	cocos2d::Node* node;
	float nTimeoutMessage;
	float timeoutMessage;

	float lbMessage_saveX;
	float lbMessage_overX;
};

class BroadcastHandler {
public:
	BroadcastHandler(std::string id, int type, cocos2d::Vec2 pos);
	~BroadcastHandler();
public:
	void addMessage(std::string message, int repeat = 0, bool top = false);
	void checkAndPlay(bool timeout = false);
	void reloadBroadcast();
	void updateHandler(float dt);
public:
	std::vector<BroadcastNode*> arMessage;
	BroadcastNode* currentBroadcast;
	std::string handlerID;
	float timeoutWait;
	int broadcastType;
	cocos2d::Vec2 broadcastPos;
};
#define broadcastMgr BroadcastMgr::getInstance()

class BroadcastMgr {
public:
	BroadcastMgr();
	~BroadcastMgr();
	static BroadcastMgr* getInstance();
public:
	void onStart();
	void reloadBroadcast();
	void addHandler(std::string id, int type, cocos2d::Vec2 pos);
	BroadcastHandler* getHandler(std::string id);
	void addMessage(std::string handlerId, std::string message, int repeat = 0, bool top =  false);
	void updateBroadcast(float dt);
public:
	static BroadcastMgr* _inst;

	bool doUpdate;
	std::vector<BroadcastHandler*> arHandler;
};

#endif // BroadcastMgr_h__
