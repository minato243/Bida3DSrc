#ifndef RT3LobbyLayer_h_
#define RT3LobbyLayer_h_

#include "../base/UIBaseLayer.h"
#include "../../network/match/MatchingInPacket.h"
#include "PlayerInfoItem.h"
#include "RT3EnterRoomId.h"
#include "../../network/game/IngameInPacket.h"
#include "RT3EnterFriendId.h"

struct FriendData
{
	int userId;
	std::string name;

	FriendData(int userId, std::string name)
	{
		this->userId = userId;
		this->name = name;
	}
};

class RT3LobbyLayer
	:public UIBaseLayer
{
public:
	enum TAG
	{
		btnJoinRoom,
		btnFind,
		btnBack,
		btnListFriend
	};
	RT3LobbyLayer();
	~RT3LobbyLayer();


	CREATE_FUNC(RT3LobbyLayer);

	bool init();

	void initGUI();

	void initTag();

	void initEnterRoomId();

	void updateClock(float dt);

	void startClock();

	void stopClock();

	void onEnter();

	void setRoomInfo(JoinWaitingRoomMsg *msg);

	void loadData(const std::vector<Player> & players);

	void initBgListFriend();

	void onButtonRelease(cocos2d::ui::Button* button, int id);

	void leave();

	void showInviteFriendPopup();

	void onKickPlayer(int userId, const std::string & userName);

	void onConfirm(int btnId, int userId);

	void doKickPlayer(int userId);

	void moveListFriend();

public:
	static int const NUM_PLAYER = 4;

private:
	cocos2d::Node *_nodeP1;
	cocos2d::Node *_nodeP2;
	cocos2d::Node *_nodeP3;
	cocos2d::Node *_nodeP4;

	cocos2d::Node *_playerNodes[NUM_PLAYER];

	cocos2d::ui::Text *_lbTime;
	cocos2d::ui::Text *_lbRoomId;

	cocos2d::ui::Button *_btnFind;
	cocos2d::ui::Button *_btnJoinRoom;
	cocos2d::ui::Button *_btnBack;

	cocos2d::ui::ImageView *_bgListFriend;
	cocos2d::ui::Button *_btnFriendList;

	std::vector<PlayerInfoItem *> _playerSlots;
	RT3EnterRoomId* _enterRoomIdPopup;
	RT3EnterFriendId *_inviteFriendPopup;

	long _startClockTime;

	int _roomId;
	int _hostId;
	bool _isHost;

	bool _isShowListFriend;

};
#endif // !RT3LobbyLayer_h_
