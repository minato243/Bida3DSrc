#include "IngameInPacket.h"
#include "core/utils/LanguageMgr.h"

ShootMsg::ShootMsg(fr::InPacket * pk)
	:BaseInPacket(pk)
{
	uId = 0;
}

ShootMsg::~ShootMsg()
{
}

void ShootMsg::readData()
{
	int error = _pk->getError();
	if (error == 0) {
		uId = _pk->getInt();
		shot.direction.x = _pk->getDouble();
		shot.direction.y = _pk->getDouble();
		shot.direction.z = _pk->getDouble();

		shot.offsetX = _pk->getDouble();
		shot.offsetY = _pk->getDouble();
		shot.force = _pk->getDouble();
	}
}

EndGameMsg::EndGameMsg(fr::InPacket * pk)
	:BaseInPacket(pk)
{
	isWinner = false;
}

EndGameMsg::~EndGameMsg()
{
}

void EndGameMsg::readData()
{
	int error = _pk->getError();
	if (error == 0) {
		int winner = _pk->getByte();
		isWinner = (winner == 1);
	}
}

PlayerReadyMsg::PlayerReadyMsg(fr::InPacket * pk)
	:BaseInPacket(pk)
{
}

PlayerReadyMsg::~PlayerReadyMsg()
{
}

void PlayerReadyMsg::readData()
{
}

ShotResultMsg::ShotResultMsg(fr::InPacket * pk)
	:BaseInPacket(pk)
{
}

ShotResultMsg::~ShotResultMsg()
{
}

void ShotResultMsg::readData()
{
	int error = _pk->getError();
	userId = _pk->getInt();
	phrase = _pk->getByte();
	seed = _pk->getDouble();
	cueSeed = _pk->getDouble();
	nextTurnPlayerId = _pk->getInt();
	isPlacingCueBall = _pk->getByte() == 1;
	ballsPocketed.clear();
	int numBallsPocketed = _pk->getInt();
	for (int i = 0; i < numBallsPocketed; i++) {
		PocketBallData data;
		data.number = _pk->getByte();
		data.pocketId = _pk->getByte();
		ballsPocketed.push_back(data);
	}
	ballsEnable.clear();
	int numBallsEnable = _pk->getInt();
	for (int i = 0; i < numBallsEnable; i++) {
		EnableBallData ball;
		ball.ballId = _pk->getByte();
		ball.isDisable = _pk->getByte() == 1;
		ball.isPocketed = _pk->getByte() == 1;
		ball.pos.x = _pk->getDouble();
		ball.pos.y = _pk->getDouble();
		ball.pos.z = _pk->getDouble();
	}
}

std::string ShotResultMsg::getStatus()
{
	switch (error) {
	case 0: return "Success";
	case 1: return "Lag ball out of range";
	case 2: return "Lag not touch foot";
	case 3: return "Lag pocketed";
	case 4: return "Lag touch side cushion";
	case 5: return "Lag undefined";
	case 6: return "Lag Touch Foot more than once";
	case 7: return "Break not reach target";
	case 8: return "Break touch cushion first";
	case 9: return "Play invalid cue ball";
	case 10: return "Play invalid first ball collision";
	case 11: return "Play cue ball jump";
	case 12: return "Play non cue ball collision";
	case 13: return "Play non cushion and pocketed";
	case 14: return "Play Time out";
	}
}

std::string ShotResultMsg::getVNStatus()
{
	return getStatus();
}

SetBallMsg::SetBallMsg(fr::InPacket * pk)
	:BaseInPacket(pk)
{
}

SetBallMsg::~SetBallMsg()
{
}

void SetBallMsg::readData()
{
	int error = _pk->getError();
	if (error == 0) {
		userId = _pk->getInt();
		ballId = _pk->getInt();
		ballPos.x = _pk->getDouble();
		ballPos.y = _pk->getDouble();
		ballPos.z = _pk->getDouble();
		submit = _pk->getByte() == 1;
	}
	else {
		CCLOG("ERROR set ball");
	}
}

std::string SetBallMsg::getStatus()
{
	switch (this->error) {
	case 1: return "Game not found";
	case 2: return "Match not found";
	case 3: return "Invalid player turn";
	case 4: return "Invalid match phrase";
	case 5: return "Invalid ball";
	case 6: return "FAIL";
	}
	return "Unhandled error";
}

StartMatchMsg::StartMatchMsg(fr::InPacket * pk)
	:BaseInPacket(pk)
{
}

StartMatchMsg::~StartMatchMsg()
{
}

void StartMatchMsg::readData()
{
	int error = _pk->getError();
	if (error != 0)
		return;
	gameId = _pk->getInt();
	matchId = _pk->getInt();
	firstTurnPlayer = _pk->getInt();
	int nPlayers = _pk->getByte();
	players.clear();
	while (nPlayers > 0) {
		Player p;
		this->readPlayer(p);
		players.push_back(p);
		nPlayers--;
	}
	seed = _pk->getDouble();
	cueSeed = _pk->getDouble();
}

void StartMatchMsg::readPlayer(Player & player)
{
	player.id = _pk->getInt();
	player.name = _pk->getString();
	player.avatar = _pk->getString();
	player.gold = (int)(_pk->getLong());
	player.cueId = _pk->getInt();
}

CallPocketMsg::CallPocketMsg(fr::InPacket * pk)
	:BaseInPacket(pk)
{
	pocketId = -1;
}

CallPocketMsg::~CallPocketMsg()
{
}

void CallPocketMsg::readData()
{
	int error = _pk->getError();
	if (error != 0)
		return;
	pocketId = _pk->getByte();
}

BetGoldMsg::BetGoldMsg(fr::InPacket * pk)
	:BaseInPacket(pk)
{
}

BetGoldMsg::~BetGoldMsg()
{
}

void BetGoldMsg::readData()
{
	int error = _pk->getError();
	if (error != 0)
		return;
	userId = _pk->getInt();
	gold = (int)(_pk->getLong());
}

std::string BetGoldMsg::getStatus()
{
	switch (_pk->getError()) {
	case 0: return "Success";
	case 1: return "Game not found";
	case 2: return "Invalid mode";
	case 3: return "Player not found";
	case 4: return "Fail";
	default: return "Unhandled error";
	}
}

UpdateCueDirMsg::UpdateCueDirMsg(fr::InPacket * pk)
	:BaseInPacket(pk)
{
}

UpdateCueDirMsg::~UpdateCueDirMsg()
{
}

void UpdateCueDirMsg::readData()
{
	int error = _pk->getError();
	if (error != 0)
	{
		CCLOG("readData Error cmdId = %d, error = %d", _pk->getCmdId(), error);
		return;
	}
		
	direction.x = _pk->getDouble();
	direction.y = _pk->getDouble();
	direction.z = _pk->getDouble();
}

UpdateCueBallPosMsg::UpdateCueBallPosMsg(fr::InPacket * pk)
	:BaseInPacket(pk)
{
}

UpdateCueBallPosMsg::~UpdateCueBallPosMsg()
{
}

void UpdateCueBallPosMsg::readData()
{
	int error = _pk->getError();
	if (error != 0)
	{
		CCLOG("readData Error cmdId = %d, error = %d", _pk->getCmdId(), error);
		return;
	}

	int a =_pk->getInt();
	int b =_pk->getInt();
	// cc.log("a,b,c: " + a + " " + b + " " + c);
	pos.x = _pk->getDouble();
	pos.y = _pk->getDouble(); 
	pos.z = _pk->getDouble(); 
	// cc.log("andsf: " + JSON.stringify(pos));
	submit =_pk->getByte() == 1;
}

StartTurnMsg::StartTurnMsg(fr::InPacket * pk)
	:BaseInPacket(pk)
{
}

StartTurnMsg::~StartTurnMsg()
{
}

void StartTurnMsg::readData()
{
	int error = _pk->getError();
	if (error != 0)
	{
		CCLOG("readData Error cmdId = %d, error = %d", _pk->getCmdId(), error);
		return;
	}

	curTurnPlayerId = _pk->getInt();
	isPlacingCueBall = _pk->getByte();
	CCLOG("StartTurnMsg::readData %d %s", curTurnPlayerId, isPlacingCueBall?"true":"false");
}

NewCueChallengeMsg::NewCueChallengeMsg(fr::InPacket * pk)
	:BaseInPacket(pk)
{
}

NewCueChallengeMsg::~NewCueChallengeMsg()
{
}

void NewCueChallengeMsg::readData()
{
	int error = _pk->getError();
	if (error != 0)
	{
		CCLOG("readData Error cmdId = %d, error = %d", _pk->getCmdId(), error);
		return;
	}

	int numNew =_pk->getInt();
	challenges.clear();
	for (int i = 0; i < numNew; i++) {
		CueChallengeData challenge;
		challenge.pocketId =_pk->getInt();
		challenge.cueType =_pk->getInt();
		challenges.push_back(challenge);
	}
}

SwitchCueMsg::SwitchCueMsg(fr::InPacket * pk)
	:BaseInPacket(pk)
{
}

SwitchCueMsg::~SwitchCueMsg()
{
}

void SwitchCueMsg::readData()
{
	int error = _pk->getError();
	if (error != 0)
	{
		CCLOG("readData Error cmdId = %d, error = %d", _pk->getCmdId(), error);
		return;
	}

	userId = _pk->getInt();
	cueId = _pk->getInt();
}

CallExtensionMsg::CallExtensionMsg(fr::InPacket * pk)
	:BaseInPacket(pk)
{
}

CallExtensionMsg::~CallExtensionMsg()
{
}

void CallExtensionMsg::readData()
{
	int error = _pk->getError();
	if (error != 0)
	{
		CCLOG("readData Error cmdId = %d, error = %d", _pk->getCmdId(), error);
		return;
	}

	userId = _pk->getInt();
}

StartCheatMsg::StartCheatMsg(fr::InPacket * pk)
	:BaseInPacket(pk)
{
}

StartCheatMsg::~StartCheatMsg()
{
}

void StartCheatMsg::readData()
{
	int error = _pk->getError();
	if (error != 0)
	{
		CCLOG("readData Error cmdId = %d, error = %d", _pk->getCmdId(), error);
		return;
	}
}

JoinWaitingRoomMsg::JoinWaitingRoomMsg(fr::InPacket * pk)
	:BaseInPacket(pk)
{
}

JoinWaitingRoomMsg::~JoinWaitingRoomMsg()
{
}

void JoinWaitingRoomMsg::readData()
{
	int error = _pk->getError();
	if (error != 0)
	{
		CCLOG("readData Error cmdId = %d, error = %d", _pk->getCmdId(), error);
		return;
	}

	channelId = _pk->getInt();
	roomId = _pk->getInt();
	hostId = _pk->getInt();
	players.clear();
	int playerCount = _pk->getInt();
	for (int i = 0; i < playerCount; i++) {
		Player player;
		player.id = _pk->getInt();
		player.name = _pk->getString();
		player.avatar = _pk->getString();
		player.gold = _pk->getLong();
		player.glove = _pk->getInt();
		players.push_back(player);
	}
}

std::string JoinWaitingRoomMsg::getStatus()
{
	switch (_pk->getError()) {
		case 0: 
			return languageMgr->localizedText("text_join_room_rt3_success");
		case 1: 
			return languageMgr->localizedText("text_join_room_rt3_playing_user");
		case 2: 
			return languageMgr->localizedText("text_join_room_rt3_invalid_room");
		case 3: 
			return languageMgr->localizedText("text_join_room_rt3_playing_room");
		case 4: 
			return languageMgr->localizedText("text_join_room_rt3_friend_slot_holding");
		case 5: 
			return languageMgr->localizedText("text_join_room_rt3_kicked");
		case 6: 
			return languageMgr->localizedText("text_join_room_rt3_matching_room");
		case 7: 
			return languageMgr->localizedText("text_join_room_rt3_unhandled");
	}
	return "";
}

KickPlayerMsg::KickPlayerMsg(fr::InPacket * pk)
	:BaseInPacket(pk)
{
}

KickPlayerMsg::~KickPlayerMsg()
{
}

void KickPlayerMsg::readData()
{
	int error = _pk->getError();
	if (error != 0)
	{
		CCLOG("readData Error cmdId = %d, error = %d", _pk->getCmdId(), error);
		return;
	}

	userId = _pk->getInt();
}

std::string KickPlayerMsg::getStatus()
{
	switch (_pk->getError()) {
	case 0: return "success";
	case 1: return "Not permission";
	case 2: return "Invalid player";
	case 3: return "Invalid room";
	case 4: return "Fail";
	}
}

LeaveRoomMsg::LeaveRoomMsg(fr::InPacket * pk)
	:BaseInPacket(pk)
{
}

LeaveRoomMsg::~LeaveRoomMsg()
{
}

void LeaveRoomMsg::readData()
{
	int error = _pk->getError();
	if (error != 0)
	{
		CCLOG("readData Error cmdId = %d, error = %d", _pk->getCmdId(), error);
		return;
	}

	userId = _pk->getInt();
}

std::string LeaveRoomMsg::getStatus()
{
	return std::string();
}

MatchingRoomRT3Msg::MatchingRoomRT3Msg(fr::InPacket * pk)
	:BaseInPacket(pk)
{
}

MatchingRoomRT3Msg::~MatchingRoomRT3Msg()
{
}

void MatchingRoomRT3Msg::readData()
{
	int error = _pk->getError();
	if (error != 0)
	{
		CCLOG("readData Error cmdId = %d, error = %d", _pk->getCmdId(), error);
		return;
	}
}

std::string MatchingRoomRT3Msg::getStatus()
{
	switch (_pk->getError()) {
	case 0: return "Success";
	case 1: return "Invalid room";
	case 2: return "Not permission";
	case 3: return "Already matching";
	case 4: return "failed";
	}
	return "failed";
}

OpenCardMsg::OpenCardMsg(fr::InPacket * pk)
	:BaseInPacket(pk)
{
}

OpenCardMsg::~OpenCardMsg()
{
}

void OpenCardMsg::readData()
{
	int error = _pk->getError();
	if (error != 0)
	{
		CCLOG("readData Error cmdId = %d, error = %d", _pk->getCmdId(), error);
		return;
	}

	userId = _pk->getInt();
	cardIdx = _pk->getShort();
	type = _pk->getInt();
	quantity = _pk->getInt();
	openTime = (int)(_pk->getLong());
}

RevealCardMsg::RevealCardMsg(fr::InPacket * pk)
	:BaseInPacket(pk)
{
}

RevealCardMsg::~RevealCardMsg()
{
}

void RevealCardMsg::readData()
{
	int error = _pk->getError();
	if (error != 0)
	{
		CCLOG("readData Error cmdId = %d, error = %d", _pk->getCmdId(), error);
		return;
	}

	int cardCount = _pk->getByte();
	cards.clear();
	for (int i = 0; i < cardCount; i++) {
		CardData card;
		card.reward.type = _pk->getInt();
		card.reward.quantity = _pk->getInt();
		card.isOpened = _pk->getByte() == 1;
		card.userId = _pk->getInt();
		card.openTime = _pk->getLong();
		cards.push_back(card);
	}
}

StartCardOpeningMsg::StartCardOpeningMsg(fr::InPacket * pk)
	:BaseInPacket(pk)
{
}

StartCardOpeningMsg::~StartCardOpeningMsg()
{
}

void StartCardOpeningMsg::readData()
{
	int error = _pk->getError();
	if (error != 0)
	{
		CCLOG("readData Error cmdId = %d, error = %d", _pk->getCmdId(), error);
		return;
	}
}

PickFirstTurnStateMsg::PickFirstTurnStateMsg(fr::InPacket *pk)
	:BaseInPacket(pk)
{

}

PickFirstTurnStateMsg::~PickFirstTurnStateMsg()
{

}

void PickFirstTurnStateMsg::readData()
{
	if (error == 0)
	{
		firstTurnPlayer = _pk->getInt();
		keoBuaDao.clear();
		int n = _pk->getInt();
		for (int i = 0; i < n; i++)
		{
			int k = _pk->getInt();
			int v = _pk->getInt();
			keoBuaDao.insert(std::pair<int, int>(k, v));
		}
	}
}

PlayerDisconnectMsg::PlayerDisconnectMsg(fr::InPacket *pk)
	:BaseInPacket(pk)
{

}

PlayerDisconnectMsg::~PlayerDisconnectMsg()
{

}

void PlayerDisconnectMsg::readData()
{
	if (!error)
	{
		int n = _pk->getInt();
		for (auto i = 0; i < n; i++)
		{
			int id = _pk->getInt();
			int timeRemain = _pk->getInt() / 1000;

			players.push_back(PlayerDisconnectData(id, timeRemain));
		}
	}
}

PlayerReconnectMsg::PlayerReconnectMsg(fr::InPacket *pk)
	:BaseInPacket(pk)
{
	uId = 0;
	canStart = false;
	startTurn = 0;
}

PlayerReconnectMsg::~PlayerReconnectMsg()
{

}

void PlayerReconnectMsg::readData()
{
	if (error == 0)
	{
		uId = _pk->getInt();
		canStart = _pk->getByte() == 1;
		startTurn = _pk->getInt();
	}
}
