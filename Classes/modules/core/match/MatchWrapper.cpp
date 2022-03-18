#include "MatchWrapper.h"
#include <core/GameMgr.h>
#include <modules/core/interaction/InteractionManager.h>
#include <modules/core/match/EightBallPollCtrl.h>
#include <modules/utils/SoundMgr.h>
#include <modules/ui/ingame/IngameEightBallPoolUI.h>
#include "../../ui/ingame/IngameMidFX.h"

MatchWrapper::MatchWrapper()
{
	_animatingNotify = false;
	_gameCount = 1;
	_winner = -1;
	_curGameIdx = 0;
}

MatchWrapper::~MatchWrapper()
{
}

void MatchWrapper::init(GameMode mode)
{
	_gameMode = mode;
	switch (mode)
	{
	case NO_MODE:
		break;
	case QUICK_MODE:
	case STREAK_MODE:
	case FRIEND_MODE:
		_gameCount = 1;
		break;
	case RT2_MODE:
		_gameCount = 3;
		break;
	case RT3_MODE:
		_gameCount = 5;
		break;
	default:
		_gameCount = 1;
		break;
	}
}

void MatchWrapper::notifyBallType(bool isSolid)
{
	gameMgr->_ingameUI->notifyBallType(isSolid);
}

void MatchWrapper::setOnPocketedBall(int pId, std::vector<int> balls)
{
	((IngameEightBallPoolUI*)gameMgr->_ingameUI)->updateRemainingBalls(pId, balls);
}

void MatchWrapper::sendFinishAnim()
{
	gameHandler->sendEndTurn();
}

void MatchWrapper::resetRack()
{
	CCLOG("MatchWrapper::resetRack");
	//MainLayer.instance.resetBallPosition();
	//this._table.world()._flags.break = false;
	gameMgr->_table->reset();
}

void MatchWrapper::onGameFinished(int pId)
{
	CCLOG("MatchWrapper::onGameFinished %d", pId);
	auto curMode = gameMgr->_ebpCtrl->_currentMode;
	if (curMode != RT2_MODE && curMode != RT3_MODE)
	{
		onMatchEnd(pId);
	}
	else
	{
		_gameResults.push_back(pId);
		//count num game win of user and competitor
		int winScore = _gameCount / 2 +1;
		int numWin = 0;
		int numEnemyWin = 0;

		for (auto i = 0; i < _gameResults.size(); i++)
		{
			if (_gameResults[i] == 0) numWin ++;
			else numEnemyWin++;
		}
		if (numWin >= winScore || numEnemyWin >= winScore)
		{
			onMatchEnd(pId);
		}
		else
		{
			//wait for start new game
			nextGameClient(numWin, numEnemyWin, pId);
		}
		updateScore(numWin, numEnemyWin);
	}
}

void MatchWrapper::startPlacingCueBall()
{
	CCLOG("MatchWrapper::startPlacingCueBall");
	if (EightBallPollCtrl::DEBUG_PLAY_ALONE) {
		//gameMgr->_ebpCtrl->onSwitchPlayer(gv.user.id, true);
	}
	gameMgr->_ebpCtrl->onSwitchPlayer(gameMgr->_userInfo->_uId, true);
}

void MatchWrapper::highlightBalls(std::vector<int>* balls)
{
	gameMgr->_table->highlightBalls(balls);
}

void MatchWrapper::undoLetPlayerCallPocket()
{
	interactionMgr->setSelectingPocket(false);
	gameMgr->_ebpCtrl->setSelectPocketState(SelectPocketState::CANNOT_SELECT);
}

void MatchWrapper::letPlayerCallPocket(int pId)
{
	CCLOG("MatchWrapper::letPlayerCallPocket");
	notify(ANNOUNCE_DEFAULT, "Moi ban chon lo");
	if (pId == 0) {
		gameMgr->_ebpCtrl->setSelectPocketState(SelectPocketState::SELECTING);
	}
}

void MatchWrapper::onMatchEnd(int pId)
{
	bool isWin = pId == 0;
	soundMgr->playWin(isWin);
	notify(
		isWin ? AnnounceType::ANNOUNCE_WIN : AnnounceType::ANNOUNCE_LOSE, 
		"", 
		CC_CALLBACK_0(MatchWrapper::onNotifyEndGameComplete, this));

}

void MatchWrapper::onNotifyEndGameComplete()
{
	sendFinishAnim();
	gameMgr->_ebpCtrl->onMatchEnd(_isWinner);
}

void MatchWrapper::notify(AnnounceType type, const std::string & message, const CUSTOM_CALLBACK & callback)
{
	CUSTOM_CALLBACK notifyComplete = std::bind(&MatchWrapper::notifyComplete, this, callback);
	auto announce = std::bind(&GuiMgr::notifyAnnouncerIngame, guiMgr, type, message, notifyComplete);
	if (_animatingNotify) {
		_notifyQueues.push_back(announce);
	}
	else {
		_animatingNotify = true;
		announce();
	}
}

void MatchWrapper::notifyComplete(const CUSTOM_CALLBACK & callback)
{
	if (_notifyQueues.size() > 0) {
		auto item = _notifyQueues.at(0);
		_notifyQueues.erase(_notifyQueues.begin());
		item();
	}
	else {
		_animatingNotify = false;
	}
	if (callback != nullptr) callback();
}

int* MatchWrapper::getScore()
{
	int numWin = 0;
	int numEnemyWin = 0;

	for (auto i = 0; i < _gameResults.size(); i++)
	{
		if (_gameResults[i] == 0) numWin++;
		else numEnemyWin++;
	}

	int *ret = new int[2];
	ret[0] = numWin;
	ret[1] = numEnemyWin;

	return ret;
}

void MatchWrapper::onReceiveStartNextGame(StartMatchMsg *pk)
{
	if (hasNextGame())
	{
		_receivedNextGame = true;
		nextGame();
	}
}

void MatchWrapper::nextGameClient(const int & userScore, const int &  enemyScore, const int & winnerIdx)
{
	auto endRoundGui = (IngameMidFX *)guiMgr->getGui(Popup::INGAME_MID_GAME_FX);
	endRoundGui->show();
	endRoundGui->runFX(userScore, enemyScore, winnerIdx);
	endRoundGui->setCallback(CC_CALLBACK_0(MatchWrapper::doNextGameClient, this));
}

void MatchWrapper::doNextGameClient()
{
	_clientDonePreviousGame = true;
	nextGame();
}

void MatchWrapper::updateScore(int userWin, int enemyWin)
{
	auto ingameUI = (IngameEightBallPoolUI *)gameMgr->_ingameUI;
	ingameUI->setScore(userWin, enemyWin);
}

bool MatchWrapper::hasNextGame()
{
	if (_gameMode == RT2_MODE || _gameMode == RT3_MODE)
	{
		bool isFinished = getWinner() == -1;
		return isFinished;
	}
	return false;
}

void MatchWrapper::nextGame()
{
	//TODO show eff new round
	if (!_clientDonePreviousGame || !_receivedNextGame)
	{
		CCLOG("server or client didn't finish game. something is wrong. client done: %s , server send response: %s", 
			_clientDonePreviousGame?"true":"false",
			_receivedNextGame?"true":"false");
		return;
	}

	//notify
	auto type = AnnounceType::NOTICE_ROUND_2;
	if (_curGameIdx == _gameCount - 1)
	{
		type = AnnounceType::NOTICE_ROUND_END;
	}
	else if (_curGameIdx == 0)
	{
		type = AnnounceType::NOTICE_ROUND_2;
	}
	else if (_curGameIdx == 1)
	{
		type = AnnounceType::NOTICE_ROUND_3;
	}
	else if (_curGameIdx == 2)
	{
		type = AnnounceType::NOTICE_ROUND_4;
	}

	auto callback = CC_CALLBACK_0(MatchWrapper::onNotifyNewRoundComplete, this);
	notify(type, "", callback);
}

void MatchWrapper::onNotifyNewRoundComplete()
{
	_receivedNextGame = false;
	_clientDonePreviousGame = false;
	
	applySeed();
	onNextGame();
	_curGameIdx++;
}

int MatchWrapper::getWinner()
{
	int userWin = 0;
	int enemyWin = 0;
	for (auto i = 0; i < _gameResults.size(); i++)
	{
		if (_gameResults[i] == 0) userWin++;
		else enemyWin++;
	}

	if (userWin > _gameCount / 2) return 0;
	if (enemyWin > _gameCount / 2) return 1;
	return -1;
}

void MatchWrapper::onNextGame()
{
	resetRack();
	auto ingameUI = dynamic_cast<IngameEightBallPoolUI *>(gameMgr->_ingameUI);
	ingameUI->resetRemainingBalls();
}

void MatchWrapper::applySeed()
{
	CCLOG("MatchWrapper::applySeed TODO");
}
