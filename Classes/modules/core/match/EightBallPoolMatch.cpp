#include "EightBallPoolMatch.h"
#include <cocos2d.h>
#include <core/utils/LanguageMgr.h>
#include <data/GameConstant.h>
#include <modules/core/utils/TimeUtils.h>
#include <modules/utils/SoundMgr.h>
#include <modules/ui/GuiMgr.h>
#include <modules/core/match/EightBallPollCtrl.h>
#include <core/GameMgr.h>
#include <modules/ui/GuiMgr.h>
#include <modules/user/User.h>
#include "modules/core/ps/internal/PhysicsConstants.hpp"

USING_NS_CC;
using namespace std;

EightBallPoolMatch::EightBallPoolMatch()
{
	reset();
	_manager = new MatchWrapper();
	_ballSuggestion = new MatchBallSuggestion();
}

EightBallPoolMatch::~EightBallPoolMatch()
{
	CC_SAFE_DELETE(_manager);
	CC_SAFE_DELETE(_ballSuggestion);
}

void EightBallPoolMatch::reset()
{
	_lag = false;
	int shotCounterLenght = sizeof(_shotCounter) / sizeof(_shotCounter[0]);
	for (int i = 0; i < shotCounterLenght; i++) {
		_shotCounter[i] = 0;
	}

	_calledPocket[0] = -1; _calledPocket[1] = -1;
	Player p1 = Player();
	Player p2 = Player();
	_players.push_back(p1);
	_players.push_back(p2);

	_playerTurns[0] = 0; _playerTurns[1] = 0;
	//_onBallPocketDelegates = [];
	//_onWorldPauseDelegates = [];
	
	resetGameData();
}

void EightBallPoolMatch::resetGameData()
{
	_started = false;
	_turn = 0;
	_curPlayerRemainTime = 0;
	_curPlayerIdx = 0;
	_foul = false;
	_paused = false;
	_counting = false;
	_extActivated = false;
	_animatingNotify = false;

	_ballTouchCushionCurTurn.clear();
	_ballCollisionsCurTurn.clear();
	_generalCollision.clear();
	_curTurnPocketBall.clear();

	_pocketedBall[NOT_DETERMINED].clear();
	_pocketedBall[SOLID].clear();
	_pocketedBall[STROKE].clear();

	_foulList.clear();
	_selectedBallType = false;
	_isSetBallPosition = false;
	_waitingPlayerIdx = -1;
	_serverFinish = true;
	_calledPocket[0] = -1; _calledPocket[1] = -1;
	_cushionShotCount[0] = 0; _cushionShotCount[1] = 0;
	_combineShotCount[0] = 0; _combineShotCount[1] = 0;

	_countTurnSkip = 0;
	_listPockedBallByPlayer[0].clear();
	_listPockedBallByPlayer[1].clear();
}

void EightBallPoolMatch::setPlayerListOrder(const std::vector<Player>& players)
{
	for (int i = 0; i < players.size(); i++)
	{
		if (players[i].id == gameMgr->_userInfo->_uId)
		{
			_players[0].id = players[i].id;
			_players[0].team = players[i].team;
		}
		else
		{
			_players[1].id = players[i].id;
			_players[1].team = players[i].team;
		}
	}
}

bool EightBallPoolMatch::onBreakTurnFinished(std::vector<PocketBallData>& pocketBalls, bool isCueBallFell, bool isEightBallFell, WinData * win)
{
	auto valid = true;
	auto ballCollideCushionCount = getBallCollideCushionCount();
	auto isCollideCushion = isCueBallCollideCushion();
	auto countPocketedBalls = 0;
	for (auto i = 0; i < pocketBalls.size(); i++) {
		if (pocketBalls[i].number != 0)
			countPocketedBalls++;
	}
	CCLOG("is cue ball collide cushion: ", isCollideCushion);

	if (ballCollideCushionCount < 4 && countPocketedBalls < 1) {
		// notify(GuiMgr.ANNOUNCE_FOUL, "not enough 4 balls collide || no pocketed ball");
		_foulList.push_back(languageMgr->localizedText("foul_break_not_enough_ball"));
		valid = false;
	}

	if (isCollideCushion) {
		// notify(GuiMgr.ANNOUNCE_FOUL, "cue ball collide cushion first")
		_foulList.push_back(languageMgr->localizedText("foul_break_touch_cushion"));
		valid = false;
	}

	CCLOG("break turn (client) valid: " + valid);
	if (!valid) {
		if (isCueBallFell) {
			_foul = true;
		}

		resetRack();
		if (isCueBallFell) {
			// notify(GuiMgr.ANNOUNCE_FOUL, "not valid + cue fell");
			_foulList.push_back(languageMgr->localizedText("foul_break_not_valid_pocketed"));
			notifyFoul();
			// autoPlayerSetCueBallPosition(1-_curPlayerIdx);
			return false;
		}
		else {
			// notify(GuiMgr.ANNOUNCE_FOUL, "not valid");
			_foulList.push_back(languageMgr->localizedText("foul_break_not_valid"));
			// autoPlayerSetCueBallPosition(_curPlayerIdx);
			notifyFoul();
			return false;
		}
	}
	else {
		// Valid break
		CCLOG("VALID BREAK");
		_breakTurn = false;

		// Special cases
		if (isEightBallFell && isCueBallFell) {
			win = new WinData(1 - _curPlayerIdx, "Eight ball and cue ball pocketed");
			// onPlayerWin(1-_curPlayerIdx, "Eight ball and cue ball pocketed");
			// return;
		}
		else if (isEightBallFell) {
			//if (_rule & gv.core.MatchWrapper.Rule.NO_WIN_ON_BREAK_8_BALL != 0) {
			//	_manager->putBackBallsToTable([8]);
			//}
			//else {
				win = new WinData(_curPlayerIdx, "Eight ball pocketed");
			//}
		}
		else if (isCueBallFell) {
			// notify(GuiMgr.ANNOUNCE_FOUL, "Cue ball pocketed");
			_foulList.insert(_foulList.begin(), languageMgr->localizedText("foul_cue_ball_pocketed"));
			notifyFoul(getNextPlayerIdx(), nullptr, true);
			// autoPlayerSetCueBallPosition(getNextPlayerIdx());
			return false;
		}
		// Normal cases. Change player if there is no fallen ball. Keep player if there is at least one fallen ball
	}
	return valid;
}

void EightBallPoolMatch::rollbackShotResult(const ShootResultData & data)
{
	//TODO
	/*_table->getWorld()->restoreStateFromData(data);
	_justShoot = false;*/
}

void EightBallPoolMatch::forceChangePlayerTurn()
{
	_curPlayerIdx = getNextPlayerIdx();
}

int EightBallPoolMatch::getNextPlayerIdx(int idx)
{
	if(idx == -1) 
		idx = _curPlayerIdx;
	return 1 - idx;
}

void EightBallPoolMatch::notifyFoul(int player, WinData * win, bool placeBall)
{
	if (_foulList.size() == 0) return;
	if (player == -1) player = getNextPlayerIdx();

	_ignoreNotifyTurn = true;
	CUSTOM_CALLBACK callBack = std::bind(&EightBallPoolMatch::notifyFoulDone, this, player, win, placeBall);
	soundMgr->soundFoul();
	notify(AnnounceType::ANNOUNCE_FOUL, _foulList[0], callBack);
}

void EightBallPoolMatch::notifyFoulDone(int player, WinData * win, bool placeBall)
{
	CCLOG("EightBallPoolMatch::notifyFoulDone ");
	_foulList.clear();
	if (win != nullptr ) {
		onPlayerWin(win->pId, win->message);
		return;
	}

	if (_curTurnPocketBall.size() > 0 && _selectedBallType)
	{
		updatePocketedBalls();
	}

	_ignoreNotifyTurn = false;
	changePlayer();

	if (placeBall) {
		letPlayerSetCueBallPosition(player);
	}

	_manager->sendFinishAnim();
}

void EightBallPoolMatch::updatePocketedBalls()
{
	// Remain ball on the table
	std::vector<int> solids = std::vector<int>{1,2,3,4,5,6,7};
	for (int i = 0; i < _pocketedBall[BallType::SOLID].size(); i ++)
	{
		int number = _pocketedBall[BallType::SOLID].at(i).number;
		for (auto it = solids.begin(); it != solids.end();) 
		{
			if (*it == number) 
			{
				it = solids.erase(it);
			}
			else
			{
				++it;
			}
		}
	}
		
	std::vector<int> stripes = std::vector<int>{ 9, 10, 11, 12, 13, 14, 15 };
	for (int i = 0; i < _pocketedBall[BallType::STROKE].size(); i++)
	{
		int number = _pocketedBall[BallType::STROKE].at(i).number;
		for (auto it = stripes.begin(); it != stripes.end();) 
		{
			if (*it == number) 
			{
				it = stripes.erase(it);
			}
			else 
			{
				++it;
			}
		}
	}

	if (solids.size() == 0) solids.push_back(8);
	if (stripes.size() == 0) stripes.push_back(8);

	_manager->setOnPocketedBall(0, _players[0].getBallType() == SOLID ? solids : stripes);
	_manager->setOnPocketedBall(1, _players[1].getBallType() == SOLID ? solids : stripes);

}

bool EightBallPoolMatch::isLag()
{
	return _lag;
}

int EightBallPoolMatch::getCurPlayerIdx()
{
	return _curPlayerIdx;
}

void EightBallPoolMatch::resetTimer()
{
	_curPlayerRemainTime = TURN_TIME;
}

void EightBallPoolMatch::update(float dt)
{
	if (!_started || _paused)
		return;

	if (_table->getWorld()->isRunning())
		return;

	if (_animatingNotify)
		return;

	_curPlayerRemainTime -= dt;
	if (_curPlayerRemainTime < 0)
	{
		if (FAKE_CLIENT_RESULT)
		{
			onFoul(TableEventType::TIME_OUT);
		}
	}
}

bool EightBallPoolMatch::isCurPlayer(int idx)
{
	if (idx == -1) 
		idx = _curPlayerIdx;
	return idx == 0;
}

float EightBallPoolMatch::getTimeRemain()
{
	return _curPlayerRemainTime;
}

bool EightBallPoolMatch::curPlayerNextTurn()
{
	CCLOG("EightBallPoolMatch::curPlayerNextTurn");
	_oldPlayerIdx = _curPlayerIdx;
	return false;
}

void EightBallPoolMatch::changePlayer()
{
	int nextPlayer = getNextPlayerIdx(_curPlayerIdx);
	CCLOG("EightBallPoolMatch::changePlayer nextPlayer %d", nextPlayer);
	_oldPlayerIdx = _curPlayerIdx;
	gameMgr->_ebpCtrl->onSwitchPlayer(_players[_curPlayerIdx].id, false);
}

void EightBallPoolMatch::startLag(int idx)
{
	_lag = true;
	_started = false;
	_lagPlayerIdx = idx;
}

void EightBallPoolMatch::startNextTurnByUid(int uId)
{
	CCLOG("START NEXT TURN BY ID %d %d %d", uId, _players[0].id, uId == _players[0].id);
	if (uId == _players[0].id) {
		startNextTurn(true);
	}
	else {
		startNextTurn(false);
	}
}

void EightBallPoolMatch::startNextTurn(bool isNextTurn)
{
	_oldPlayerIdx = _curPlayerIdx;
	_curPlayerIdx = isNextTurn ? 0 : 1;
	_curPlayerRemainTime = TURN_TIME;
	startTurnPlayerClientOrServer(_curPlayerIdx, _ignoreNotifyTurn, true);
}

void EightBallPoolMatch::startTurnPlayerClientOrServer(int idx, bool ignoreNotifyTurn, int server)
{
	if (server) {
		/*if (_ignoreServerStartTurn) {
			return;
		}*/
		_serverFinish = true;
	}
	else {
		_ignoreNotifyTurn = ignoreNotifyTurn;
	}
	CCLOG("start turn player client or server. idx %d, ignoreNotify %d, server %d", idx, ignoreNotifyTurn, server);
	startTurnPlayer(idx, ignoreNotifyTurn);
}

std::vector<int> *EightBallPoolMatch::getValidBall(int idx)
{
	if (_lag) return new std::vector<int>();

	if(idx == -1) idx = _curPlayerIdx;

	auto validType = _players[idx].getBallType();
	auto ans = new std::vector<int>();
	for (auto num = 1; num <= 15; num++) {
		if (num == 8) continue;
		if (!_selectedBallType 
			|| (!isBallInPocket(num) && getBallType(num) == validType)) 
		{
			ans->push_back(num);
		}
	}
	if (!_selectedBallType || isPlayerFinishAllBallOfType(idx)) ans->push_back(8);

	return ans;
}

Ball * EightBallPoolMatch::getSuggestedBall()
{
	std::vector<int> *ballIds = getValidBall();
	std::vector<Ball *> balls = _table->getAllBall();
	Ball *cueBall = _table->getWhiteBall();
	Cue *cue = _table->getCue();
	double trajectLength = cue->_cueInfo->getWorldTrajectoryLengths().trajectA;

	Ball *ball =_ballSuggestion->suggestBall(*ballIds, balls, cueBall, trajectLength);

	return ball;
}

bool EightBallPoolMatch::isBallInPocket(int number)
{
	int ballType = getBallType(number);
	for (auto i = 0; i < _pocketedBall[ballType].size(); i++) {
		if (_pocketedBall[ballType].at(i).number == number)
			return true;
	}
	return false;
}

BallType EightBallPoolMatch::getBallType(int ballNumber)
{
	if (ballNumber == 0 || ballNumber ==8)
		return BallType::NOT_DETERMINED;
	if (ballNumber < 8)
		return BallType::SOLID;
	return BallType::STROKE;
}

void EightBallPoolMatch::highlightValidBall(int idx)
{
	auto ans = getValidBall(idx);
	_manager->highlightBalls(ans);
}

void EightBallPoolMatch::onShoot(int cueType, bool opponentShoot)
{
	CCLOG("ON SHOOT", cueType, opponentShoot);
	_table->hightLightAllBalls();
	_serverFinish = false;
	if (!(_lag && opponentShoot)) {
		_justShoot = true;
	}

	if (!opponentShoot) {
		_shotCounter[cueType] += 1;
	}
	// shotCounter.trigger()
}

void EightBallPoolMatch::onTurnTimeOut()
{
	CCLOG("time out");
	if (EightBallPollCtrl::DEBUG_PLAY_ALONE) {
		_paused = true;
		_curPlayerRemainTime = TURN_TIME;
	}
	// Announce time out then start the other player's turn

	// Just for test
	// this.startTurnPlayer(1-this._curPlayerIdx);
	onFoul(TableEventType::TIME_OUT);

		// if(this._isSetBallPosition){
		_playerThatSetBallPos = getNextPlayerIdx(_playerThatSetBallPos);
	// this._manager._onTurnTimeOut(true, this._playerThatSetBallPos === 0);
// }
// else{
//     this._manager._onTurnTimeOut(false);
// }
}

void EightBallPoolMatch::handleBallCollision(BallCollisionData *ballCollisionData)
{
	BallCollisionData event = BallCollisionData(*ballCollisionData);
	_ballCollisionsCurTurn.push_back(event);
	_generalCollision.push_back(event);
	checkFoul(event);

	auto firstHitInBreak = false;
	auto current = timeUtils->current();
	if (_breakTurn && _lastCollide < 0) {
		firstHitInBreak = true;
	}

	if (_breakTurn && firstHitInBreak) {
		_lastCollide = timeUtils->current();
		soundMgr->soundBreak(event.velocity);
	}
	else if ((_breakTurn && current - _lastCollide < 500) || current - _lastCollide < 200) {
		// Nothing
	}
	else {
		_lastCollide = timeUtils->current();
		soundMgr->soundBallCollide(event.velocity);
	}
}

void EightBallPoolMatch::handleBallOutOfTable(BallOutOfTableData * ballOutTable)
{
	_curTurnOutOfTableBall.push_back(ballOutTable->number);
}

void EightBallPoolMatch::handleBallFellInHole(BallFellHoleData *ballFellInHoleData)
{
	CCLOG("handleBallFellInHole %d TODO", ballFellInHoleData->number);
	_curTurnPocketBall.push_back(PocketBallData(ballFellInHoleData->number, ballFellInHoleData->pocketId));
	//check foul
	checkFoul(*ballFellInHoleData);
	soundMgr->soundBallPocketed(ballFellInHoleData->number);
	_table->onPocketedBall(ballFellInHoleData->pocketId, ballFellInHoleData->number);
}

void EightBallPoolMatch::handleCushionCollision(int ballNumber, int cushionId)
{
	CushionCollisionData cushionCollisionData = CushionCollisionData(ballNumber, cushionId);
	_ballTouchCushionCurTurn.push_back(cushionCollisionData);
	CCLOG("handleCushionCollision %d %d TODO", ballNumber, cushionId);
}

int EightBallPoolMatch::getPlayerIdFromIdx(int idx)
{
	int id = _players[idx].id;
	return id;
}

void EightBallPoolMatch::applySeed(long long seed)
{
	CCLOG("EightBallPoolMatch::applySeed %ld", seed);
	gameMgr->_table->getWorld()->setRandomSeed(seed);
}

void EightBallPoolMatch::loadFakeDataMatch()
{
	map<int, ps::ExtMath::vector> _ballPos;
	_ballPos[0] = PhysicsConstants::CUE_BALL_POSITION;
	for (int i = 0; i < NUM_BALL; i++)
	{
		_ballPos[i+1] = ps::ExtMath::vector(0, -63.85, -4);
	}
	_ballPos[1] = ps::ExtMath::vector(100, 50);
	_ballPos[8] = ps::ExtMath::vector(100, -50);

	_pocketedBall[SOLID].clear();
	for (int i = 0; i < 6; i++)
	{
		_pocketedBall[SOLID].push_back(PocketBallData(i+2, 1));
	}

	_pocketedBall[STROKE].clear();
	for (int i = 0; i < 7; i++)
	{
		_pocketedBall[STROKE].push_back(PocketBallData(i + 9, 1));
	}
	
	setPlayerBallType(0, SOLID);
	setPlayerBallType(1, STROKE);

	_started = true;
	_turn = 0;
	_breakTurn = false;
	gameMgr->_table->loadFakeData(_ballPos);
}

void EightBallPoolMatch::notifyBallType(bool isSolid)
{
	_manager->notifyBallType(isSolid);
}

Player * EightBallPoolMatch::getPlayerFromId(int uId)
{
	for (auto i = 0; i < _players.size(); i++)
	{
		if (_players[i].id == uId)
		{
			return &_players[i];
		}
	}

	return NULL;
}

void EightBallPoolMatch::onEndGame(int winner)
{
	_started = false;
	_paused = true;

	_manager->onGameFinished(winner);
}

void EightBallPoolMatch::checkFoul(BallCollisionData & event)
{
	//First cue ball collision is the wrong type
	if (event.numbers[0] != 0 && event.numbers[1] != 0) return;
	int number = event.numbers[0] + event.numbers[1];
	if (_selectedBallType && _ballCollisionsCurTurn.size() == 1) {
		auto type = getBallType(number);
		CCLOG("EightBallPoolMatch::checkFoul BallCollisionData type = %d, _curPlayerIdx = %d, _players[_curPlayerIdx].getBallType()", 
			type, _curPlayerIdx,  _players[_curPlayerIdx].getBallType());
		// Collide with 8-ball when play selected the ball type
		if (type == NOT_DETERMINED) {
			if (_selectedBallType && !isPlayerFinishAllBallOfType()) {
				_foulList.push_back(languageMgr->localizedText("foul_collide_8_ball"));
				onFoul();
				return;
			}
		}

		else if (type != _players[_curPlayerIdx].getBallType()) {
			_foulList.push_back(languageMgr->localizedText("foul_collide_wrong_type"));
			onFoul();
			return;
		}
	}
}

void EightBallPoolMatch::checkFoul(BallFellHoleData & event)
{
	if (event.number == 0)
	{
		_foulList.push_back(languageMgr->localizedText("foul_cue_ball_pocketed"));
		onFoul();
	}
}

void EightBallPoolMatch::checkFoul(BallOutOfTableData & event)
{
	if (event.number == 0)
	{
		_foulList.push_back(languageMgr->localizedText("foul_cue_out_table"));
		onFoul();
	}
}

bool EightBallPoolMatch::isPlayerFinishAllBallOfType(int idx)
{
	if (idx == -1)
		idx = _curPlayerIdx;
	auto playerType = _players[idx].getBallType();
	return _pocketedBall[playerType].size() == 7;
}

void EightBallPoolMatch::setPlayerBallType(int idx, BallType type)
{
	std::string msg = "player " + std::to_string(idx + 1) + " get ball type " + (type == BallType::SOLID ? "SOLID" : "STROKE");
	notify(AnnounceType::ANNOUNCE_DEFAULT, msg, nullptr);
	_players[idx].setBallType(type);
	_players[1 - idx].setBallType(getAnotherType(type));
	_selectedBallType = true;
}

BallType EightBallPoolMatch::getAnotherType(BallType type)
{
	if (type == BallType::NOT_DETERMINED)
		return BallType::NOT_DETERMINED;

	if (type == BallType::SOLID)
		return BallType::STROKE;
	return BallType::SOLID;
}

bool EightBallPoolMatch::isCurTeam(int pId)
{
	return pId == 0;
}

void EightBallPoolMatch::letPlayerSetCueBallPosition(int pId)
{
	CCLOG("Let Player Set Cue Ball Position: %d", pId);

	guiMgr->notifyDebugText("Player " + std::to_string(pId + 1) + " set ball position");
	_turn += 1;
	_isSetBallPosition = true;
	_playerThatSetBallPos = pId;

	highlightValidBall(pId);
	if(gameMgr->_userInfo->_uId == getPlayerIdFromIdx(pId))
		_manager->startPlacingCueBall();
}

void EightBallPoolMatch::afterPlayerSetCueBallPosition(int pId, ps::ExtMath::vector pos)
{
	if (pId == -1)
		pId = _playerThatSetBallPos;
	// Start turn player
	_isSetBallPosition = false;
}

bool EightBallPoolMatch::isCapCueBallToKitchen()
{
	return _breakTurn;
}

void EightBallPoolMatch::setManager(MatchWrapper * manager)
{
	_manager = manager;
}

void EightBallPoolMatch::onPlayerWin(int pId, std::string msg)
{
	_paused = true;
	_manager->onGameFinished(pId);
	_table->hideAllHighlightPockets();
	gameHandler->sendFakeEndGame();
}

void EightBallPoolMatch::notify(AnnounceType type, std::string str, const CUSTOM_CALLBACK & callback)
{
	CCLOG("EightBallPoolMatch::notify type = %d", type);
	CUSTOM_CALLBACK notifyComplete = std::bind(&EightBallPoolMatch::notifyComplete , this, callback);
	auto announce = std::bind(& GuiMgr::notifyAnnouncerIngame, guiMgr, type, str, notifyComplete);
	if (_animatingNotify) {
		_notifyQueues.push_back(announce);
	}
	else {
		_animatingNotify = true;
		announce();
	}
}

void EightBallPoolMatch::notifyComplete(const CUSTOM_CALLBACK & callback)
{
	if (_notifyQueues.size() > 0) {
		auto item = _notifyQueues.at(0);
		_notifyQueues.erase(_notifyQueues.begin());
		item();
	}
	else {
		_animatingNotify = false;
	}
	if(callback != nullptr) callback();
}

void EightBallPoolMatch::resetRack()
{
	_manager->resetRack();
}

void EightBallPoolMatch::selectBallType(int pId, std::vector<PocketBallData> & pocketBalls)
{
	for (int i = 0; i < pocketBalls.size(); i++) {
		// CCLOG("SELECT BALL TYPE WITH bALL: " + JSON.stringify(pocketBalls[i]));
		if (getBallType(pocketBalls[i].number) != NOT_DETERMINED) {
			BallType type = getBallType(pocketBalls[i].number);
			setPlayerBallType(pId, type);

			updatePocketedBalls();
			_manager->notifyBallType(getCurGroupIdx(pId) == 1 ^ type == BallType::SOLID);

			break;
		}
	}
}

void EightBallPoolMatch::onFoul(TableEventType eventType)
{
	CCLOG("Foul type: %d", eventType);
	_foul = true;
	if (eventType == TableEventType::TIME_OUT) 
	{
		_countTurnSkip++;

		_ignoreServerStartTurn = true;
		auto player = getNextPlayerIdx();

		auto callback = std::bind(&EightBallPoolMatch::onNotifyTimeOutDone, this, player);
		notify(AnnounceType::ANNOUNCE_FOUL, languageMgr->localizedText("foul_time_out"), callback);

		if (FAKE_CLIENT_RESULT)
		{
			std::vector<PocketBallData> pocketedData;
			auto ballBodys = gameMgr->_table->getWorld()->allBalls();
			gameMgr->_ebpCtrl->sendFakeClientResult(true, true, pocketedData, ballBodys);
			//changePlayer();
		}
	}
} 

void EightBallPoolMatch::onFoul()
{
	_foul = true;
}

void EightBallPoolMatch::onNotifyTimeOutDone(int playerIdx)
{
	_ignoreServerStartTurn = false;
	_foul = false;
	letPlayerSetCueBallPosition(playerIdx);
	//notify(isCurTeam(playerIdx) ? ANNOUNCE_PLAYER_TURN : ANNOUNCE_OPPONENT_TURN);
	changePlayer();
}

int EightBallPoolMatch::getBallCollideCushionCount()
{
	auto ans = 0;
	std::vector<int> collideNumber;
		for (auto i = 0; i < _ballTouchCushionCurTurn.size(); i++) {
			auto ballNum = _ballTouchCushionCurTurn[i].ballId;
			if (ballNum != 0 && find(collideNumber.begin(), collideNumber.end(), ballNum) == collideNumber.end()) {
				collideNumber.push_back(ballNum);
				ans++;
			}
		}
	// CCLOG("total ball collide cushion: " + ans);
	return ans;
}

bool EightBallPoolMatch::isCueBallCollideCushion()
{
	if (_generalCollision.size() == 0) return false;
	// CCLOG("general ball collision: ", _generalCollision);
	TableEventData *event = &_generalCollision[0];
	CushionCollisionData* cushionCollision = dynamic_cast<CushionCollisionData*>(event);

	return cushionCollision && cushionCollision->ballId == 0;
}

void EightBallPoolMatch::start(int firstPlayer)
{
	_started = true;
	_breakTurn = true;
	_isSetBallPosition = true;
	_lastCollide = -1;
	_curPlayerIdx = firstPlayer;
	_curPlayerRemainTime = TURN_TIME;
}

void EightBallPoolMatch::subscribeToTable(Table *table)
{
	CCLOG("subcribe to table");
	_table = table;
	_pauseDelegateIdx = table->subscribeOnPaused(CC_CALLBACK_0(EightBallPoolMatch::onWorldPaused, this));
	_eventDelegateIdx = table->subscribeTableEvents(CC_CALLBACK_2(EightBallPoolMatch::onTableEvent, this));
}

void EightBallPoolMatch::onTableEvent(TableEventType eventType, TableEventData * event)
{
	switch (eventType)
	{
	case TableEventType::BALL_COLLIDE_BALL:
	{
		BallCollisionData *eventData = dynamic_cast<BallCollisionData *>(event);
		if (eventData != nullptr) {
			handleBallCollision(eventData);
		}
		break;
	}

	case TableEventType::BALL_FELL_HOLE:
	{
		BallFellHoleData *eventData = dynamic_cast<BallFellHoleData *>(event);
		if (eventData != nullptr) {
			handleBallFellInHole(eventData);
		}
		break;
	}

	case TableEventType::BALL_COLLIDE_CUSHION:
	{
		CushionCollisionData *eventData = dynamic_cast<CushionCollisionData *>(event);
		if (eventData != nullptr) {
			handleCushionCollision(eventData->ballId, eventData->cushionId);
		}
		break;
	}

	case TableEventType::BALL_OUT_OF_TABLE:
	{
		break;
	}
	default:
		break;
	}
}

void EightBallPoolMatch::logAndCompareBallPosition()
{
	CCLOG("COMPARE BALL POS");
	//if (EightBallPollCtrl::DEBUG_PLAY_ALONE) {
	//	// for(auto i=0;i<_table.world().balls.length;i++){
	//	//     CCLOG(i, ".", _table.world().balls[i].position);
	//	// }
	//	// _table.world()._random.getSeed();
	//	return;
	//}
	//if (!gameMgr->_ebpCtrl._shootResult) return;
	//auto results = gameMgr->_ebpCtrl._shootResult.ballsEnable;
	//if (!results) return;
	//std::vector<> outOfSyncedBall;
	//for (auto i = 0; i < results.length; i++) {
	//	auto ball = gameMgr->_table->world()->balls[i];

	//	auto matchStatus = ball._disabled == results[i].isDisable || ball._inHole == results[i].isPocketed;
	//	if (ball._inHole && matchStatus) continue;

	//	auto clPos = ball.position;
	//	auto svPos = results[i].pos;
	//	auto matchPos = svPos.x == = clPos.x && svPos.y == = clPos.y && svPos.z == = clPos.z;
	//	if (svPos.z < -75 && clPos.z < -75) matchPos = true;
	//	// CCLOG("Ball", results[i].ballId, "is", match ? "match":"not match");
	//	if (!matchPos) {
	//		outOfSyncedBall.push(results[i].ballId);
	//		if (!matchStatus) CCLOG("NO match status. disabled:", ball._disabled, results[i].isDisable, "pocket", ball._inHole, results[i].isPocketed);
	//		else {
	//			CCLOG(results[i].ballId, clPos, svPos);
	//		}
	//		// CCLOG("Ball", results[i].ballId, "Client pos", clPos, "disable", ball._disabled, "Server pos", svPos, "disable", results[i].isDisable);
	//		// _table.world().balls[i].position = results[i].pos;
	//	}
	//}
	//if (outOfSyncedBall.size() == 0) {
	//	CCLOG("All balls are matched!");
	//}
	//else {
	//	notify(AnnounceType::ANNOUNCE_DEFAULT, "OUT OF SYNC!! " + outOfSyncedBall);
	//	_manager->resetBallsPositionAndNotify(results);
	//}
}

void EightBallPoolMatch::onWorldPaused()
{
	auto pocketBalls = _curTurnPocketBall;
	auto ballPocketResult = getBallPocketResult(pocketBalls);

	auto hasBallTypePocketed = ballPocketResult.hasBallTypePocketed;
	auto isCueBallFell = ballPocketResult.isCueBallFell;
	auto isEightBallFell = ballPocketResult.isEightBallFell;

	WinData *win = nullptr;

	// this is the first turn, has different rule
	if (_breakTurn) {
		bool rightBreak = onBreakTurnFinished(pocketBalls, isCueBallFell, isEightBallFell, win);
		logAndCompareBallPosition();

		if (!rightBreak) return;
	}
	else {
		// Select ball type
		logAndCompareBallPosition();

		// If 8-ball fell before all ball of type fell
		if (!isPlayerFinishAllBallOfType() && isEightBallFell) {
			win = new WinData(getNextPlayerIdx(), "8 ball fell before all ball finished");
			_foulList.insert(_foulList.begin(),languageMgr->localizedText("foul_8_ball_pocketed"));
			_foul = true;
			onPlayerWin(1 - _curPlayerIdx, "8 ball fell before all ball finished");
			return;
		}

		if (!_selectedBallType && !_foul) {
			selectBallType(_curPlayerIdx, pocketBalls);
		}

		if (isPlayerFinishAllBallOfType() && isEightBallFell) {
			win = new WinData(_curPlayerIdx, "win by rule");
			// check called pocket (if called pocket == -1 means double at last)

			auto calledPocket = _calledPocket[getCurGroupIdx()];
			if ((getPocketOfBall(8) != calledPocket) && calledPocket != -1) {
				CCLOG("first eq:", getPocketOfBall(8), " ", calledPocket, "2nd eq:", calledPocket != -1);
				win = new WinData(getNextPlayerIdx(), "8-ball pocketed on wrong pocket");
			}
			else if (_foul) {
				win = new WinData(getNextPlayerIdx(), "cue ball fell with 8-ball in last turn");
			}
			CCLOG("check 8 ball on finish all ball: %d", win->pId);
		}
	}

	auto ballBodys = gameMgr->_table->getWorld()->allBalls();
	if (_foul) {
		CCLOG("Foul!!");
		//if (_players[_curPlayerIdx].id == gameMgr->_userInfo->_uId || _players[_curPlayerIdx].id < 0)
		//	gameMgr->_ebpCtrl->sendFakeClientResult(_foul, !hasBallTypePocketed, _curTurnPocketBall, ballBodys);
		notifyFoul(-1, win);
		return;
	}
	else {
		// If there is no foul => count for combination shot & cushion shot
		_combineShotCount[_curPlayerIdx] += getCurTurnCombin();
		_cushionShotCount[_curPlayerIdx] += getCurTurnCushion();
	}

	if (win != nullptr) {
		onPlayerWin(win->pId, win->message);
		return;
	}

	int myId = gameMgr->_userInfo->_uId;
	CCLOG("EightBallPoolMatch::onWorldPaused myId= %d, _players[_curPlayerIdx].id = %d", myId, _players[_curPlayerIdx].id);
	//if (_players[_curPlayerIdx].id == myId || _players[_curPlayerIdx].id < 0)
	//	gameMgr->_ebpCtrl->sendFakeClientResult(_foul, !hasBallTypePocketed, _curTurnPocketBall, ballBodys);

	if (_selectedBallType && pocketBalls.size() > 0)
		updatePocketedBalls();

	if (hasBallTypePocketed) {
		curPlayerNextTurn();
	}
	else {
		changePlayer();
	}

	gameMgr->_ebpCtrl->checkShootResult();

	_manager->sendFinishAnim();
	
	CC_SAFE_DELETE(win);
}

int EightBallPoolMatch::getPocketOfBall(int ballNum)
{
	CCLOG("get pocket of ball %d %d %d",  ballNum, _pocketedBall);
	for (auto i = 0; i < _pocketedBall[getBallType(ballNum)].size(); i++) {
		if (_pocketedBall[getBallType(ballNum)][i].number == ballNum) {
			return _pocketedBall[getBallType(ballNum)][i].pocketId;
		}
	}
	return -1;
}

const double EightBallPoolMatch::TURN_TIME = GameConstant::SHOOT_TIME;

//can
int EightBallPoolMatch::getCurTurnCushion()
{
	std::vector<int> cushions;
	for (auto i = 0; i < _curTurnPocketBall.size(); i++) {
		auto number = _curTurnPocketBall[i].number;
		if (number == 0) continue;

		// _ballTouchCushionCurTurn.findIndex(event => event.number === number) !== -1
		for (auto j = 0; j < _ballTouchCushionCurTurn.size(); j++) {
			if (_ballTouchCushionCurTurn[j].ballId == number) {
				cushions.push_back(_ballTouchCushionCurTurn[j].cushionId);
				break;
			}
		}

		if (cushions.size() > 0) {
			CCLOG("ball num %d touch cushion before pocketed", number, &cushions);
		}
	}
	return cushions.size();
}

//khung
int EightBallPoolMatch::getCurTurnCombin()
{
	auto count = 0;
	for (auto i = 0; i < _curTurnPocketBall.size(); i++) {
		auto number = _curTurnPocketBall[i].number;
		if (number == 0) continue;
		// Tim nhung cap va cham voi bi cai (loai bo va cham dau tien)
		//for (int j = 0; j < _ballCollisionsCurTurn.size(); j++) 
		//{
		//	if (_ballCollisionsCurTurn[j].numbers[0] == number 
		//		|| _ballCollisionsCurTurn[j].numbers[1] == number)
		//	{
		//		count++;
		//	}
		//}

		// Tim nhung cap va cham co chua bi dang xet (number) va bi va cham voi bi aang xet la bi khac bi 0
		for (int j = 0; j < _ballCollisionsCurTurn.size(); j++)
		{
			if ((_ballCollisionsCurTurn[j].numbers[0] == number && _ballCollisionsCurTurn[j].numbers[1] != 0)
				|| (_ballCollisionsCurTurn[j].numbers[1] == number && _ballCollisionsCurTurn[j].numbers[0] != 0))
			{
				count++;
			}
		}
	}
	return count;
}

BallPocketResult EightBallPoolMatch::getBallPocketResult(std::vector<PocketBallData> & pocketBallData)
{
	bool hasBallTypePocketed = false;
	bool isCueBallFell = false;
	bool isEightBallFell = false;
	for (int it = 0; it < pocketBallData.size(); it ++) {
		pocketBallData[it].player = _curPlayerIdx;
		pocketBallData[it].foul = _foul;
		auto ballNumber = pocketBallData[it].number;
		if (ballNumber == 0)
			isCueBallFell = true;
		if (ballNumber == 8) {
			isEightBallFell = true;
			_pocketedBall[0].push_back(pocketBallData[it]);
		}
		else {
			_pocketedBall[getBallType(ballNumber)].push_back(pocketBallData[it]);
			if (_players[_curPlayerIdx].isBallTypeValid(getBallType(ballNumber))) {
				hasBallTypePocketed = true;
			}
			_listPockedBallByPlayer[_curPlayerIdx].push_back(ballNumber);
		}

		//if (!gv.achievementMgr.isAchievementUnlocked(ACHIEVEMENT_ID.POCKET_10_BALL_IN_A_GAME)) {
		//	if (listPockedBallByPlayer[0].length >= 10)
		//		gv.achievementHandler.sendUnlockAchievement(ACHIEVEMENT_ID.POCKET_10_BALL_IN_A_GAME);
		//}
	}

	return BallPocketResult(hasBallTypePocketed, isCueBallFell, isEightBallFell);
}

int EightBallPoolMatch::getCurGroupIdx(int idx)
{
	if (idx == -1) {
		return _curPlayerIdx;
	}
	return idx;
}

void EightBallPoolMatch::callPocket(int pId, int pocketId)
{
	_calledPocket[pId] = pocketId;
}

void EightBallPoolMatch::startTurnPlayer(int idx, bool ignoreNotifyTurn)
{
	CCLOG("EightBallPoolMatch::startTurnPlayer %d", idx);
	if ((EightBallPollCtrl::DEBUG_PLAY_ALONE || isCurPlayer(idx)) && isPlayerFinishAllBallOfType(idx)) {
		CCLOG("Let player select pocket");
		letPlayerSelectPocket(idx);
	}

	//if (!_serverFinish && _turn != 0) return;

	if (_waitingPlayerIdx != -1 && _waitingPlayerIdx != idx && _turn != 0) {
		CCLOG("server has different result - use result of server. SERVER: %d. CLIENT: ", idx, idx);
		if (!(EightBallPollCtrl::DEBUG_PLAY_ALONE || isCurPlayer(idx)) || !isPlayerFinishAllBallOfType(idx)) {
			_manager->undoLetPlayerCallPocket();
		}
	}
	// CCLOG("start turn player " + idx);

	auto nextTurnSetBall = _foul;

	// CCLOG("Start turn player", idx, ignoreNotifyTurn, _turn);
	_foul = false;
	_turn += ignoreNotifyTurn ? 0 : 1;
	_playerTurns[idx]++;
	bool switchPlayer = _curPlayerIdx == idx;
	_curPlayerIdx = idx;

	if (EightBallPollCtrl::DEBUG_PLAY_ALONE) {
		_curPlayerIdx = idx;
		_curPlayerRemainTime = ignoreNotifyTurn ? _curPlayerRemainTime : TURN_TIME;
	}

	_isSetBallPosition = false;
	// _clientFinish = true;
	_extActivated = false;
	_ballTouchCushionCurTurn.clear();
	_ballCollisionsCurTurn.clear();
	_generalCollision.clear();
	_curTurnPocketBall.clear();
	_curTurnOutOfTableBall.clear();
	_foulList.clear();
	_paused = false;
	_serverFinish = true;
	_counting = true;
	gameMgr->_table->resetEventCount();
	_ignoreNotifyTurn = false;
	_waitingPlayerIdx = -1;

	// Only called when it's break turn
	if (_breakTurn) {
		_lastCollide = -1;
	}
	if (_breakTurn && !ignoreNotifyTurn) {
		CCLOG("Break turn, let player", idx, "set ball pos");
		letPlayerSetCueBallPosition(idx);
	}

	// CCLOG("isPlayerFinishAllBallOfType(idx)", isPlayerFinishAllBallOfType(idx));
	// if(!nextTurnSetBall && (DEBUG_PLAY_ALONE || idx === 0) && isPlayerFinishAllBallOfType(idx)){
	//     CCLOG("Let player select pocket");
	//     letPlayerSelectPocket(idx);
	// }

	highlightValidBall(_curPlayerIdx);
	int curPlayerId = getPlayerIdFromIdx(_curPlayerIdx);
	gameMgr->_ebpCtrl->onStartPlayerTurn(curPlayerId);

	if(idx != _oldPlayerIdx)
		notify(isCurTeam(idx) ? AnnounceType::ANNOUNCE_PLAYER_TURN : AnnounceType::ANNOUNCE_OPPONENT_TURN);
	

	if (switchPlayer)
	{
		gameMgr->_ebpCtrl->onSwitchPlayer(_players[idx].id, _foul);
	}

	if (EightBallPollCtrl::DEBUG_PLAY_ALONE) {
		gameMgr->_ebpCtrl->onSwitchPlayer(userInst->_uId, _breakTurn && !ignoreNotifyTurn && _isSetBallPosition);
	}
}

void EightBallPoolMatch::letPlayerSelectPocket(int idx)
{
	_manager->letPlayerCallPocket(idx);
}
