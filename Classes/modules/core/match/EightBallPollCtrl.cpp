#include "EightBallPollCtrl.h"
#include <cocos2d.h>
#include <core/GameMgr.h>
#include <modules/core/interaction/InteractionManager.h>
#include <modules/core/ps/internal/PhysicsConstants.hpp>
#include <modules/network/match/MatchHandler.h>
#include <modules/scene/GameScene.h>
#include <modules/network/game/GameHandler.h>
#include "../../ui/ingame/IngameStartMatchingUI.h"
#include "base/CCDirector.h"
#include "../inventory/InventoryMgr.h"
#include "../../network/cheat/CheatHandler.h"
#include "../../ui/lobby/ResultLayer.h"
#include "../../utils/SoundMgr.h"
#include "../../ui/ingame/DisconnectPopup.h"
#include "../../ui/endgame/ResultGUILayer.h"

USING_NS_CC;
using namespace ps;

EightBallPollCtrl::EightBallPollCtrl()
{
	//_history = [];

	_placeCueBall = PlaceCueBallState::CANNOT_PLACE; // 0: can't place, 1: placing, 2: after placed
	_callingPocket = SelectPocketState::CANNOT_SELECT; // 0: can't select, 1: selecting, 2: after

	//cueBallPos = { x: 0, y : 0, z : 0 };
	//pocketId = 0;

	_curMatch = new EightBallPoolMatch();
	_waitingForStartMatch = false;
	_startedMatch = false;
	_pocketId = -1;
	_currentGameInfo = NULL;
	_currentMode = NO_MODE;
	_channelId = 0;
	_receiveShootResult = false;
}

EightBallPollCtrl::~EightBallPollCtrl()
{
	CC_SAFE_DELETE(_curMatch);
}

void EightBallPollCtrl::showEightPoolModeSelectGUI()
{
	auto selectGui = guiMgr->getGui(Popup::SELECT_MODE_POPUP);
	selectGui->show();
}

bool EightBallPollCtrl::hasNextGame()
{
	//TODO
	return false;
}

void EightBallPollCtrl::receiveTimeOutTurn()
{
	_curMatch->onTurnTimeOut();
	//_history.push({ type: "time out" });
}

void EightBallPollCtrl::receiveSetBall(int ballId, ps::ExtMath::vector pos)
{
	interactionMgr->receiveStopInterpolateCueBall(pos);
	gameMgr->_table->setBallEnable(ballId);
	gameMgr->_table->setBallPosition(ballId, pos);

	//TODO
	//// CCLOG("RECEIVE SET BALL");
	//if (!gameMgr.onBeforeShoot_startedShot) {
	//	_history.push({
	//		type: "setBall",
	//		ballId : ballId,
	//		pos : pos
	//		});
	//}

	gameMgr->_table->updateCuePosition();
}

void EightBallPollCtrl::placingCueBallPosition(ps::ExtMath::vector & position)
{
	if (_curMatch->isCapCueBallToKitchen()) {
		ExtMath::vector::clamp(position, MIN_PLACING_BALL, MAX_PLACING_BALL_KITCHEN);
	}
	else {
		ExtMath::vector::clamp(position, MIN_PLACING_BALL, MAX_PLACING_BALL);
	}
	gameMgr->_table->setBallPosition(0, position);
	gameMgr->_table->setBallEnable(0);
}

EightBallPoolMatch * EightBallPollCtrl::getCurMatch()
{
	return _curMatch;
}

void EightBallPollCtrl::setSelectPocketState(SelectPocketState state)
{
	_callingPocket = state;
}

int EightBallPollCtrl::getGameId()
{
	return _gameId;
}

int EightBallPollCtrl::getPlayerIdx(int userId)
{
	for (int i = 0; i < _players.size(); i++)
	{
		if (_players[i].id == userId)
			return i;
	}
	return -1;
}

void EightBallPollCtrl::receiveChannelInfo(int ccu)
{
	if (_currentMode != RT3_MODE)
	{
		auto findMatchUI = (FindMatchLayer *)guiMgr->getGui(Popup::FIND_MATCH_LAYER);
		findMatchUI->setInfo((GameMode)_curGameMode, ccu);
		findMatchUI->show();
	}
}

void EightBallPollCtrl::receiveStartMatchingUI()
{
	auto findMatchUI = guiMgr->getGui(Popup::FIND_MATCH_LAYER);
	if (findMatchUI) findMatchUI->hide();

	auto startMatchingUi = guiMgr->getGui(Popup::INGAME_START_MATCHING_UI);
	startMatchingUi->show();
}

void EightBallPollCtrl::onSelectPickFirstTurn(PickFirstTurnStateMsg *msg)
{
	auto startMatchingUI = (IngameStartMatchingUI*)guiMgr->getGui(Popup::INGAME_START_MATCHING_UI);
	startMatchingUI->onSelectCard(msg);
}

void EightBallPollCtrl::sendFakeClientResult(bool isFoul, bool isSwitchPlayer, const std::vector<PocketBallData> & pocketedBall, const std::vector<BallBody *> *activeBalls)
{
	CCLOG("EightBallPollCtrl::sendFakeClientResult");
	gameHandler->sendFakeResultClient(isFoul, isSwitchPlayer, pocketedBall, activeBalls);
}

void EightBallPollCtrl::resetMatch()
{
	_startedMatch = false;
	_waitingForStartMatch = false;
	gameMgr->_startedShot = false;
	gameMgr->_table->reset();
	_curMatch->reset();
}

void EightBallPollCtrl::onReceiveDisconnect(PlayerDisconnectMsg *msg)
{
	auto disconnectPopup = (DisconnectPopup*)guiMgr->getGui(Popup::DISCONNECT_POPUP);

	for (auto i = 0; i < msg->players.size(); i++) 
	{
		auto player = _curMatch->getPlayerFromId(msg->players[i].id);
		if (player != NULL)
			msg->players[i].name = player->name;
		else msg->players[i].name = "";
	}
	disconnectPopup->setDisconnectState(msg->players);
	disconnectPopup->show();
}

void EightBallPollCtrl::onReceiveReconnect(PlayerReconnectMsg *msg)
{
	auto disconnectPopup = (DisconnectPopup *)guiMgr->getGui(Popup::DISCONNECT_POPUP);
	disconnectPopup->onPlayerReconnect(msg->uId);
	if (msg->canStart)
	{
		disconnectPopup->hide();
		receiveStartTurn(msg->startTurn, true);
	}
}

bool EightBallPollCtrl::checkShootResult()
{
	if (!_receiveShootResult)
		return true;
	if (gameMgr->_table->getWorld()->isRunning())
		return true;

	auto balls = gameMgr->_table->getAllBall();
	for (auto enableBall : _shootResult.ballsEnable)
	{
		auto ball = gameMgr->_table->getBallWithId(enableBall.ballId);
		auto pos = ball->body()->position();
		if (!ExtMath::isZeroNumber(pos.x - enableBall.pos.x)
			|| !ExtMath::isZeroNumber(pos.y - enableBall.pos.y)
			|| !ExtMath::isZeroNumber(pos.z - enableBall.pos.z))
		{
			CCLOG("Different result server client");
			return false;
		}
	}

	for (auto pocketedBall : _shootResult.ballsPocketed)
	{
		auto ball = gameMgr->_table->getBallWithId(pocketedBall.number);
		auto pos = ball->body()->position();
		if (!ball->body()->isInHole())
		{
			CCLOG("Different result server client");
			return false;
		}
	}
	return true;
}

void EightBallPollCtrl::receiveShoot(ShootData shot)
{
	CCLOG("Rcv Shoot force = %f, direction (%f %f %f)", shot.force, shot.direction.x, shot.direction.y, shot.direction.z );
	Cue *cue = gameMgr->_table->getCue();
	cue->updateDirection(shot.direction);
	cue->setOffset(ps::ExtMath::vector(shot.offsetX, shot.offsetY));
	//TODO gameMgr->mainLayer.updateCueDirection();
	gameMgr->_startedShot = true;

	auto isLag = _curMatch->isLag();
	cue->setVisible(true);
	
	gameMgr->shootByForce(shot.force, true);
}

void EightBallPollCtrl::opponentReady()
{
	CCLOG("EightBallPollCtrl::opponentReady TODO");
	//getGambleLayer().setVisibleReady(1, true);
}

void EightBallPollCtrl::startMatch(StartMatchMsg * pk, bool isFirst)
{
	resetMatch();
	_callingPocket = SelectPocketState::CANNOT_SELECT;
	interactionMgr->startNewMatch();

	_curMatch->_manager->init((GameMode)_curGameMode);
	_curMatch->setPlayerListOrder(pk->players);
	_curMatch->subscribeToTable(gameMgr->_table);
	_curMatch->start(isFirst?0:1);
	_curMatch->applySeed(pk->seed);
	_startedMatch = true;

	auto cueBag = inventoryMgr->getInventory()->getCueBag();
	cueBag->resetOnNewMatch();
	int usingCueId = cueBag->getUsingCue()->_id;
	_playerCueData = &(_players[0].getCueInfo(usingCueId));
	
	onSwitchCue(_playersCueData[getPlayerIdx(pk->firstTurnPlayer)]);

	auto gameScene = dynamic_cast<GameScene *> (Director::getInstance()->getRunningScene());
	if (gameScene)
	{
		auto matchingUI = guiMgr->getGui(Popup::INGAME_START_MATCHING_UI);
		matchingUI->hide();
		auto ingameUI = ((IngameEightBallPoolUI*)gameScene->_ingameUI);

		ingameUI->resetActivePlayer();
		ingameUI->setGameMode((GameMode)_curGameMode);
		ingameUI->setVisible(true);
		ingameUI->startMatch(_curMatch);
	}
}

void EightBallPollCtrl::receiveStartTurn(int pId, bool isPlacingCueBall)
{
	_startedMatch = true;
	_curMatch->startNextTurnByUid(pId);
	onSwitchPlayer(pId, isPlacingCueBall);

}

void EightBallPollCtrl::onStartPlayerTurn(int pId)
{
	auto ui = gameMgr->_ingameUI;
	if (pId == gameMgr->_userInfo->_uId)
	{
		ui->setMyTurnLayout();
	} 
	else
	{
		ui->setEnemyTurnLayout();
	}
}

void EightBallPollCtrl::onSwitchPlayer(int pId, bool isPlacingCueBall)
{
	CCLOG(" EightBallPollCtrl::onSwitchPlayer %d %d %d", pId, gameMgr->_userInfo->_uId , isPlacingCueBall);
	_placeCueBall = PlaceCueBallState::CANNOT_PLACE;
	_callingPocket = _callingPocket == SelectPocketState::AFTER ? 
		SelectPocketState::CANNOT_SELECT : _callingPocket;
	bool isCurPlayer = gameMgr->isCurPlayer(pId);

	_curMatch->resetTimer();

	gameMgr->setEnableGuildLine(isCurPlayer);
	gameMgr->_ingameUI->disablePlacingBall();
	gameMgr->_ingameUI->getPowerUI()->setForceValue(0);
	gameMgr->_table->hideAllHighlightPockets();

	auto isCapToKitchen = _curMatch->isCapCueBallToKitchen(); // Check break turn

	if (isPlacingCueBall) {
		gameMgr->_table->getWhiteBall()->body()->reset();
		auto pos = gameMgr->_table->getCueBallDefaultPosForPlacing();
		gameMgr->_table->setBallPosition(0, pos);
		if (gameMgr->isCurPlayer(pId)) {
		}
		_placeCueBall = isCapToKitchen ? PlaceCueBallState::AFTER_PLACE : PlaceCueBallState::PLACING;
	}

	if (isCurPlayer) {
		onSwitchCue(_playerCueData, false, true);
		interactionMgr->switchTurn(InteractionManager::Turns::MY_TURN, isPlacingCueBall, _callingPocket == SelectPocketState::SELECTING);
		if (_callingPocket == SelectPocketState::SELECTING) {
			gameMgr->_ingameUI->notifySelectPocket();
		}
	}
	else {
		int idx = getPlayerIdx(pId);
		onSwitchCue(_playersCueData[idx], false);
		interactionMgr->switchTurn(InteractionManager::Turns::COMPETITOR_TURN, false, false);
	}
	gameMgr->_table->updateCuePosition();
	gameMgr->_table->setGuildLineVisible(isCurPlayer);
}

void EightBallPollCtrl::onBeforeShoot()
{
	auto cueBallBody = gameMgr->_table->getWhiteBall()->body();
	if (_placeCueBall != PlaceCueBallState::CANNOT_PLACE) {
		gameHandler->sendSetBall(0, cueBallBody->position(), true);
		gameMgr->_startedShot = true;
		//_history.splice(_history.length - 1, 0, {
		//	type: "setBall",
		//	ballId : 0,
		//	pos : cueBallBody.position
		//	});
	}
	//if (callingPocket == 2) {
	//	gv.connector.sendCallPocket(pocketId);
	//	gameMgr->interactionMgr._pocketId = -1;
	//}
	_callingPocket = SelectPocketState::CANNOT_SELECT;
	_placeCueBall = PlaceCueBallState::CANNOT_PLACE;
	_receiveShootResult = false;
	gameMgr->_ebpCtrl->_curMatch->afterPlayerSetCueBallPosition(-1, cueBallBody->position());
	gameMgr->_ingameUI->disablePlacingBall();
	gameMgr->_ingameUI->setEnemyTurnLayout();
}

void EightBallPollCtrl::onMatchEnd(bool isWinner)
{
	switch (_curGameMode)
	{
	case GameMode::QUICK_MODE:
	{
		onMatchRT1End(isWinner);
		break;
	}
	case GameMode::STREAK_MODE:
	{
		onMatchS2End(isWinner);
		break;
	}

	case GameMode::RT2_MODE:
	{
		onMatchRT2End(isWinner);
		break;
	}

	case GameMode::RT3_MODE:
	{
		onMatchRT3End(isWinner);
		break;
	}
	}
}

void EightBallPollCtrl::onMatchRT1End(bool isWinner)
{
	auto resultGui = (ResultGUILayer*) guiMgr->getGui(Popup::RESULT_GIFT_LAYER);
	resultGui->loadUserInfo(_players);
	resultGui->show();
	resultGui->loadData(QUICK_MODE, _channelId, isWinner ? 0 : 1);
}

void EightBallPollCtrl::onMatchS2End(bool isWinner)
{
	bool isSecondGame = gameMgr->_userInfo->getLastS2MatchResult(_channelId);
	std::vector<bool> listStreak;
	if (isSecondGame) listStreak.push_back(true);
	listStreak.push_back(isWinner);

	auto resultGui = (ResultGUILayer*)guiMgr->getGui(Popup::RESULT_GIFT_LAYER);
	resultGui->loadDataStreak(STREAK_MODE, _channelId, isWinner ? 0 : 1, listStreak);
	resultGui->show();

	resultGui->loadUserInfo(_players);
}

void EightBallPollCtrl::onMatchRT2End(bool isWinner)
{
	auto resultGui = (ResultGUILayer*)guiMgr->getGui(Popup::RESULT_GIFT_LAYER);

	int *scores = _curMatch->_manager->getScore();
	resultGui->loadDataRT2(_channelId, isWinner ? 0 : 1, scores[0], scores[1]);
	resultGui->show();
	CC_SAFE_DELETE(scores);

	resultGui->loadUserInfo(_players);
}

void EightBallPollCtrl::onMatchRT3End(bool isWinner)
{
	auto resultGui = (ResultGUILayer*)guiMgr->getGui(Popup::RESULT_GIFT_LAYER);

	int *scores = _curMatch->_manager->getScore();
	resultGui->loadDataRT2(_channelId, isWinner ? 0 : 1, scores[0], scores[1]);
	resultGui->show();
	CC_SAFE_DELETE(scores);

	resultGui->loadRT3UserInfo(_players);
}

void EightBallPollCtrl::setShootResult(ShotResultMsg * data)
{
	_receiveShootResult = true;
	_shootResult.setData(data->userId, data->phrase, 
		data->nextTurnPlayerId, data->isPlacingCueBall, 
		data->ballsPocketed, data->ballsEnable);

	checkShootResult();
}

void EightBallPollCtrl::onEndGame(bool isWinner)
{
	_curMatch->onEndGame(isWinner ? 0 : 1);
	gameMgr->_ingameUI->setVisible(false);
	_curMatch->reset();
	gameMgr->_table->reset();
}

void EightBallPollCtrl::onCallPocketFinish(int pocketId)
{
	_pocketId = pocketId;
	auto canSetBall = _placeCueBall != PlaceCueBallState::CANNOT_PLACE;
	_callingPocket = SelectPocketState::AFTER;
	gameMgr->onCallPocketEnded();

	gameHandler->sendCallPocket(pocketId);
	_curMatch->callPocket(0, pocketId);
}

void EightBallPollCtrl::gotoModeRT3(int sendQuickMatch, int channelId)
{
	matchHandler->requestQuickMatch(RT3_MODE, 1, channelId);
	showRT3Lobby();
}

void EightBallPollCtrl::showRT3Lobby()
{
	auto rt3Gui = guiMgr->getGui(Popup::RT3_FIND_MATCH_LAYER);
	rt3Gui->show();
}

void EightBallPollCtrl::cancelRT3Matching()
{
	showEightPoolModeSelectGUI();
	_currentMode = GameMode::NO_MODE;
}

void EightBallPollCtrl::gotoMode(int mode, int quick, int channelId)
{
	_currentMode = (GameMode)mode;
	_channelId = channelId;

	if (mode == GameMode::RT3_MODE)
	{
		gotoModeRT3(quick, channelId);
		return;
	}

	matchHandler->fetchChannelInfo(mode, quick);
	matchHandler->requestQuickMatch(mode, quick, channelId);
	((GameScene*)gameMgr->_scene)->showFindMatchLayer();
}

void EightBallPollCtrl::playWithBot(int mode, double accuracy)
{
	matchHandler->fetchChannelInfo(GameMode::QUICK_MODE, true);
	cheatHandler->requestCheatPlayWithBot(mode, accuracy);

	((GameScene*)gameMgr->_scene)->showFindMatchLayer();
}

void EightBallPollCtrl::onResponeQuickMatch(ResponseJoinRoomMsg * data)
{
	setGameInfo(data);
	afterJoinedRoom();

}

void EightBallPollCtrl::onReceiveStartingMatchPacket(StartMatchMsg * pk)
{
	if (_waitingForStartMatch) {
		CCLOG("after wait start match");
		gameMgr->_startedShot = false;
		_waitingForStartMatch = false;
	}
	if (gameMgr->_startedShot) {
		//TODO
	/*	auto handler = receiveStartingMatch.bind(this, data);
		gameMgr.table.world().subscribe(gv.TableEvent.WORLD_ON_PAUSED, handler);*/
		_waitingForStartMatch = true;
		CCLOG("wait start match");
		return;
	}
	auto seed = (long long int)pk->seed;
	CCLOG("Seed = %lf ", seed);
	auto isFirst = pk->firstTurnPlayer == userInst->_uId;
	//TODO
	//auto cueBag = inventoryMgr.getInventory().getCueBag();
	//cueBag.resetOnNewMatch();
	_playersCueData.clear();
	for (int i = 0; i < _players.size(); i++) {
		_playersCueData.push_back(&_players[i].cues[0]);
	}

	for (int i = 0; i < _players.size(); i++)
	{
		onPlayerSwitchCue(_players[i].cueId, i);
	}

	if (_gameId == pk->gameId) {
		if (_curGameMode == GameMode::QUICK_MODE) {
		}

		if (hasNextGame())
		{
			onReceiveStartNextGame(pk);
		}
		else
		{
			startMatch(pk, isFirst);
		}
		
	}

	onSwitchPlayer(pk->firstTurnPlayer, isFirst);
}

void EightBallPollCtrl::onReceiveStartNextGame(StartMatchMsg *pk)
{
	_curMatch->resetGameData();

	_callingPocket = SelectPocketState::CANNOT_SELECT;
	interactionMgr->startNewMatch();

	_curMatch->setPlayerListOrder(pk->players);
	_curMatch->subscribeToTable(gameMgr->_table);
	_curMatch->applySeed(pk->seed);
	_startedMatch = true;

	auto cueBag = inventoryMgr->getInventory()->getCueBag();
	cueBag->resetOnNewMatch();
	int usingCueId = cueBag->getUsingCue()->_id;
	_playerCueData = &(_players[0].getCueInfo(usingCueId));

	auto gameScene = dynamic_cast<GameScene *> (Director::getInstance()->getRunningScene());
	if (gameScene)
	{
		auto ingameUI = ((IngameEightBallPoolUI*)gameScene->_ingameUI);

		ingameUI->resetActivePlayer();
		ingameUI->setGameMode((GameMode)_curGameMode);
		ingameUI->setVisible(true);
		ingameUI->startMatch(_curMatch);
	}

}

void EightBallPollCtrl::afterJoinedRoom()
{
	_receivedJoinRoom = true;
}

void EightBallPollCtrl::setGameInfo(ResponseJoinRoomMsg * data)
{
	CC_SAFE_DELETE(_currentGameInfo);
	_currentGameInfo = data;
	_curGameMode = data->gameMode;
	CCLOG("on Found opponent:", data);
	_gameId = data->gameId;
	

	if(data->players[1].id == userInst->_uId)
	{
		std::iter_swap(data->players.begin(), data->players.begin()+1);
	}

	// Player team always 0
	auto player = data->players[0];
	if (player.team == 1) {
		player.team = 0;
		data->players[1].team = 1;
	}

	_players = data->players;

	for (auto i = 0; i < _players.size(); i++) {
		auto player = _players[i];
		if (player.id == userInst->_uId) {
			//todo
			//player.cueBag = gv.inventoryMgr.getInventory().getCueBag().getCuesArray();
			continue;
		}

		if (player.id != userInst->_uId) {
			auto user = new User(player.id, player.name, player.avatar);
			user->setGold(player.gold);
			gameMgr->addUserInfo(user->_uId, user);
		}

		/*for (auto j = 0; j < _players[i].cues.size(); j++) {
			_players[i].cues[j] = new gv.CueInfo(_players[i].cueBag[j].cueId, _players[i].cueBag[j]);
		}*/
	}

	auto findMatchUI = (FindMatchLayer *)guiMgr->getGui(Popup::FIND_MATCH_LAYER);
	findMatchUI->show();
	findMatchUI->hide();

	auto ingameStartMatchingUi = (IngameStartMatchingUI *)guiMgr->getGui(Popup::INGAME_START_MATCHING_UI);
	ingameStartMatchingUi->show();
	ingameStartMatchingUi->updateDataPlayer(_players, data->gameMode, data->channelId);

	if (data->gameMode == GameMode::QUICK_MODE) {
		_goldBet = data->players[0].goldBet;
	}
	soundMgr->playSoundMatch();
	soundMgr->playBgIngame();
}

void EightBallPollCtrl::receiveSwitchCue(SwitchCueMsg * data)
{
	CCLOG("EightBallPollCtrl::receiveSwitchCue TODO");
	if (gameMgr->isCurPlayer(data->userId)) {
		CCLOG("use cue %d", data->cueId);
		inventoryMgr->useCueInMatch(data->cueId);
	}
	else {
		auto idx = getPlayerIdx(data->userId);
		onPlayerSwitchCue(data->cueId, idx);
	}

}

void EightBallPollCtrl::onPlayerSwitchCue(int cueId)
{
	auto playerIdx = getPlayerIdx(gameMgr->_userInfo->_uId);
	onPlayerSwitchCue(cueId, playerIdx);
	guiMgr->refreshGui(Popup::COMPETITION_BAG_UI);
}

void EightBallPollCtrl::onPlayerSwitchCue(int cueId, int playerIdx)
{
	if (playerIdx < 0) return;
	CueInfo *cueInfo = nullptr;
	auto isCurPlayer = _players[playerIdx].id == gameMgr->_userInfo->_uId;
	for (int i = 0; i < _players[playerIdx].cues.size(); i++) {
		if (_players[playerIdx].cues[i]._id == cueId) {
			cueInfo = &_players[playerIdx].cues[i];
			break;
		}
	}
	if (cueInfo == nullptr) {
		return;
	}
	if (isCurPlayer) {
		_playerCueData = cueInfo;
	}
	_playersCueData[playerIdx] = cueInfo;
	onSwitchCue(cueInfo, true, playerIdx == 0);
	// enemyCueData._type = cueInfo.type;
}

void EightBallPollCtrl::onSwitchCue(CueInfo *cueInfo, bool updateAngle /*= true*/, bool isCurPlayer /*= false*/)
{
	gameMgr->_table->getCue()->setCueInfo(cueInfo);
	interactionMgr->onChangeAngle(0);

	if (isCurPlayer)
	{
		gameMgr->_ingameUI->setCueIcon(cueInfo->_type, cueInfo->_angle);
	}
}

const ps::ExtMath::vector EightBallPollCtrl::MIN_PLACING_BALL = vector(
	-PhysicsConstants::TABLE_WIDTH / 2 + PhysicsConstants::BALL_RADIUS,
	-PhysicsConstants::TABLE_HEIGHT / 2 + PhysicsConstants::BALL_RADIUS
);

const ps::ExtMath::vector EightBallPollCtrl::MAX_PLACING_BALL = vector(
	PhysicsConstants::TABLE_WIDTH / 2 - PhysicsConstants::BALL_RADIUS,
	PhysicsConstants::TABLE_HEIGHT / 2 - PhysicsConstants::BALL_RADIUS
);

const ps::ExtMath::vector EightBallPollCtrl::MAX_PLACING_BALL_KITCHEN = vector(
	-60 + PhysicsConstants::BALL_RADIUS,
	EightBallPollCtrl::MAX_PLACING_BALL.y
);
