#ifndef PLAYER_H_
#define PLAYER_H_

#include <data/DataStruct.h>
#include "../inventory/CueInfo.h"

class Player
{
public:
	Player();
	~Player();

	bool isBallTypeValid(BallType type);

	BallType getBallType();
	
	void setBallType(BallType type);

	void bet(int gold);

	CueInfo & getCueInfo(int cueId);

private:
	BallType _ballType;

public:
	int id;
	int team;
	std::string name;
	std::string avatar;
	int gold;
	int goldBet;
	int cueId;
	int glove;
	int rankId;
	int exp;

	std::vector<CueInfo> cues;
};
#endif // !PLAYER_H_



