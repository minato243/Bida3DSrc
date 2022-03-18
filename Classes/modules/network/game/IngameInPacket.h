#ifndef IN_GAME_IN_PACKET_H_
#define IN_GAME_IN_PACKET_H_

#include "../base/BasePacket.h"
#include <data/DataStruct.h>
#include <modules/core/match/Player.h>

class ShootMsg
	:public BaseInPacket
{
public:
	ShootMsg(fr::InPacket *pk);
	~ShootMsg();

	void readData();

public:
	int uId;
	ShootData shot;
};

class EndGameMsg
: public BaseInPacket
{
public:
	EndGameMsg(fr::InPacket *pk);
	~EndGameMsg();

	void readData();

public:
	bool isWinner;
};

class PlayerReadyMsg
	: public BaseInPacket
{
public:
	PlayerReadyMsg(fr::InPacket *pk);
	~PlayerReadyMsg();

	void readData();

};

class SetBallMsg
	: public BaseInPacket
{
public:
	SetBallMsg(fr::InPacket *pk);
	~SetBallMsg();

	void readData();

	std::string getStatus();

public:
	int userId;
	int ballId;
	ps::ExtMath::vector ballPos;
	bool submit;
};

class ShotResultMsg
	: public BaseInPacket
{
public:
	ShotResultMsg(fr::InPacket *pk);
	~ShotResultMsg();

	void readData();

	std::string getStatus();

	std::string getVNStatus();

public:
	int userId;
	int phrase;
	int nextTurnPlayerId;
	bool isPlacingCueBall;
	double seed;
	double cueSeed;
	std::vector<PocketBallData> ballsPocketed;
	std::vector<EnableBallData> ballsEnable;
};

class StartMatchMsg
	: public BaseInPacket
{
public:
	StartMatchMsg(fr::InPacket *pk);
	~StartMatchMsg();

	void readData();

	void readPlayer(Player & player);
public:
	int gameId;
	int matchId;
	int firstTurnPlayer;

	std::vector<Player> players;
	double seed;
	double cueSeed;
};


class CallPocketMsg
	: public BaseInPacket
{
public:
	CallPocketMsg(fr::InPacket *pk);
	~CallPocketMsg();

	void readData();

public:
	int pocketId;
};

class BetGoldMsg
	: public BaseInPacket
{
public:
	BetGoldMsg(fr::InPacket *pk);
	~BetGoldMsg();

	void readData();

	std::string getStatus();

public:
	int userId;
	int gold;
};

class UpdateCueDirMsg
	: public BaseInPacket
{
public:
	UpdateCueDirMsg(fr::InPacket *pk);
	~UpdateCueDirMsg();

	void readData();

public:
	ps::ExtMath::vector direction;
};

class UpdateCueBallPosMsg
	: public BaseInPacket
{
public:
	UpdateCueBallPosMsg(fr::InPacket *pk);
	~UpdateCueBallPosMsg();

	void readData();

public:
	ps::ExtMath::vector pos;
	bool submit;
};

class StartTurnMsg
	: public BaseInPacket
{
public:
	StartTurnMsg(fr::InPacket *pk);
	~StartTurnMsg();

	void readData();

public:
	int curTurnPlayerId;
	bool isPlacingCueBall;
};

class NewCueChallengeMsg
	: public BaseInPacket
{
public:
	NewCueChallengeMsg(fr::InPacket *pk);
	~NewCueChallengeMsg();

	void readData();

public:
	std::vector<CueChallengeData> challenges;
};

class SwitchCueMsg
	: public BaseInPacket
{
public:
	SwitchCueMsg(fr::InPacket *pk);
	~SwitchCueMsg();

	void readData();

public:
	int userId;
	int cueId;
};

class CallExtensionMsg
	: public BaseInPacket
{
public:
	CallExtensionMsg(fr::InPacket *pk);
	~CallExtensionMsg();

	void readData();

public:
	int userId;
};

class StartCheatMsg
	: public BaseInPacket
{
public:
	StartCheatMsg(fr::InPacket *pk);
	~StartCheatMsg();

	void readData();

};

class JoinWaitingRoomMsg
	: public BaseInPacket
{
public:
	JoinWaitingRoomMsg(fr::InPacket *pk);
	~JoinWaitingRoomMsg();

	void readData();

	std::string getStatus();

public:
	int channelId;
	int roomId;
	int hostId;
	std::vector<Player> players;
};

class KickPlayerMsg
	: public BaseInPacket
{
public:
	KickPlayerMsg(fr::InPacket *pk);
	~KickPlayerMsg();

	void readData();

	std::string getStatus();

public:
	int userId;
};

class LeaveRoomMsg
	: public BaseInPacket
{
public:
	LeaveRoomMsg(fr::InPacket *pk);
	~LeaveRoomMsg();

	void readData();

	std::string getStatus();

public:
	int userId;
};


class MatchingRoomRT3Msg
	: public BaseInPacket
{
public:
	MatchingRoomRT3Msg(fr::InPacket *pk);
	~MatchingRoomRT3Msg();

	void readData();

	std::string getStatus();

};


class OpenCardMsg
	: public BaseInPacket
{
public:
	OpenCardMsg(fr::InPacket *pk);
	~OpenCardMsg();

	void readData();

public:
	int userId;
	int cardIdx;
	int type;
	int quantity;
	int openTime;
};

class RevealCardMsg
	: public BaseInPacket
{
public:
	RevealCardMsg(fr::InPacket *pk);
	~RevealCardMsg();

	void readData();

public:
	std::vector<CardData> cards;
};

class StartCardOpeningMsg
	: public BaseInPacket
{
public:
	StartCardOpeningMsg(fr::InPacket *pk);
	~StartCardOpeningMsg();

	void readData();

};

class PickFirstTurnStateMsg
	:public BaseInPacket
{
public:
	PickFirstTurnStateMsg(fr::InPacket *pk);
	~PickFirstTurnStateMsg();

	void readData();

public:
	int firstTurnPlayer;
	std::map<int, int> keoBuaDao;

};

struct PlayerDisconnectData
{
	int id;
	float timeRemain;
	std::string name;

	PlayerDisconnectData(int id, int timeRemain)
	{
		this->id = id;
		this->timeRemain = timeRemain;
		this->name = "";
	}
};
class PlayerDisconnectMsg
	:public BaseInPacket
{
public:
	PlayerDisconnectMsg(fr::InPacket *pk);
	~PlayerDisconnectMsg();

	void readData();

public:
	std::vector<PlayerDisconnectData> players;
};

class PlayerReconnectMsg
	:public BaseInPacket
{
public:
	PlayerReconnectMsg(fr::InPacket *pk);
	~PlayerReconnectMsg();

	void readData();

public:
	int uId;
	bool canStart;
	int startTurn;
};

#endif // !IN_GAME_IN_PACKET_H_


