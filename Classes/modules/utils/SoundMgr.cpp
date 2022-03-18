#include "SoundMgr.h"
#include <cocos2d.h>
#include <core/GameSound.h>
#include "SimpleAudioEngine.h"

using namespace CocosDenshion;
USING_NS_CC;
using namespace std;

SoundMgr::SoundMgr()
{
	_lobbyBg = -1;
	_ingameBg = -1;
	_fadeBgTimeRemain = 0;
}

SoundMgr::~SoundMgr()
{
}

void SoundMgr::soundShoot(double force)
{
	if (force > 650) {
		playEffect("res/sound/old_bida/staffhit_strongest.mp3");
	}
	else if (force > 250) {
		playEffect("res/sound/old_bida/staffhit_medium.mp3");
	}
	else {
		playEffect("res/sound/old_bida/staff_hit_weak.mp3");
	}
}

void SoundMgr::soundBallCollide(double velocity)
{
	if (velocity > 175) {
		playEffect("res/sound/old_bida/ball_first_hit_0" + std::to_string((rand() % 3)) + ".mp3");
	}
	else if (velocity > 115) {
		playEffect("res/sound/old_bida/ball_second_hit.mp3");
	}
	else if (velocity > 60) {
		playEffect("res/sound/old_bida/ball_third_hit.mp3");
	}
	else {
		playEffect("res/sound/old_bida/ball_fourd_hit.mp3");
	}
}

void SoundMgr::soundBreak(double velocity)
{
	if(velocity > 175) {
		playEffect("res/sound/old_bida/break_00.mp3");
	}
	else if (velocity > 100) {
		playEffect("res/sound/old_bida/break_01.mp3");
	}
	else {
		playEffect("res/sound/old_bida/break_02.mp3");
	}
}

void SoundMgr::soundBallPocketed(const int & ballNum)
{
	if (ballNum == 8) return;
	soundBallPocketedNoNumber();
	char text[10];
	if(ballNum <10)sprintf(text, "0%d", ballNum);
	else sprintf(text, "%d", ballNum);

	playEffect("res/sound/music/number_ball_" + string(text) + ".mp3");
}

void SoundMgr::soundBallPocketedNoNumber()
{
	playEffect("res/sound/old_bida/ball_drop_0" + std::to_string(rand() % 3) + ".mp3");
}

void SoundMgr::soundEightBallWin()
{
	soundBallPocketedNoNumber();
	playEffect("res/sound/music/ball_8_end.mp3");
}

void SoundMgr::soundEightBallLose()
{
	soundBallPocketedNoNumber();
	playEffect("res/sound/music/wrong_ball.mp3");
}

void SoundMgr::soundButton()
{
	playEffect("res/sound/music/ui_button.mp3");
}

void SoundMgr::soundFoul()
{
	playEffect("res/sound/music/wrong_ball.mp3");
}

void SoundMgr::soundWorkshop(int state)
{
	switch (state) {
	case 0:
		playEffect("res/sound/music/upgrade_success.mp3");
		break;
	case 1:
		playEffect("res/sound/music/upgrade_processing.mp3");
		break;
	case 2:
		playEffect("res/sound/music/upgrade_fail.mp3");
		break;
	}
}

void SoundMgr::soundStar()
{
	playEffect("res/sound/old_bida/star_hole.mp3");
}

void SoundMgr::playWin(bool isWin)
{
	if (isWin) soundEightBallWin();
	else soundEightBallLose();
}

void SoundMgr::playBgLobby()
{
	playBgMusic(LOBBY_BG_MUSIC);
}

void SoundMgr::playBgIngame()
{
	int r = rand() % INGAME_BG_MUSIC.size();
	playBgMusic(INGAME_BG_MUSIC[r]);
}

void SoundMgr::stopBgLobby()
{
	SimpleAudioEngine::getInstance()->pauseEffect(_lobbyBg);
}

void SoundMgr::stopBgIngame()
{
	SimpleAudioEngine::getInstance()->pauseEffect(_ingameBg);
}

void SoundMgr::onShow()
{
	SimpleAudioEngine::getInstance()->resumeAllEffects();
}

void SoundMgr::onHide()
{
	SimpleAudioEngine::getInstance()->pauseAllEffects();
}


void SoundMgr::playBgSearchingOpponent()
{
	playBgMusic(SoundMgr::SEARCHING_BG_PATHS);
}

void SoundMgr::playBgMusic(const std::string &path, bool fade /*= true*/)
{
	SimpleAudioEngine::getInstance()->pauseAllEffects();

	if (_bgIdMap.find(path) != _bgIdMap.end())
	{
		SimpleAudioEngine::getInstance()->resumeEffect(_bgIdMap[path]);
	}
	else
	{
		int id = playEffect(path, true);
		_bgIdMap.insert(pair<string, int>(path, id));
	}
	_curBgPath = path;

	Director::getInstance()->getScheduler()->schedule(CC_CALLBACK_1(SoundMgr::updateFading, this), this, 0, 0, 0, 1, UPDATE_FADING_KEY);
}

void SoundMgr::updateFading(float dt)
{
	_fadeBgTimeRemain -= dt;
	if (_fadeBgTimeRemain < 0)
	{
		Director::getInstance()->getScheduler()->unschedule(UPDATE_FADING_KEY, this);
	}

	float volume = 1 - _fadeBgTimeRemain / SoundMgr::FADE_TIME;

	SimpleAudioEngine::getInstance()->setEffectsVolume(volume);
}

void SoundMgr::playSoundMatch()
{
	playEffect(SOUND_MATCHED);
}

SoundMgr *SoundMgr::_instance = nullptr;
SoundMgr * SoundMgr::getInstance()
{
	if (_instance == nullptr) {
		_instance = new SoundMgr();
	}
	return _instance;
}

const char *SoundMgr::LOBBY_BG_MUSIC = "res/sound/music/bida3dlobby.mp3";
const vector<string> SoundMgr::INGAME_BG_MUSIC = {
	"res/sound/music/bida3Dingame.mp3",
	"res/sound/music/ingame_v3.mp3"
};

const char *SoundMgr::SEARCHING_BG_PATHS = "res/sound/music/finding_opponent.mp3";
const char *SoundMgr::WIN = "res/sound/old_bida/win.mp3";
const char *SoundMgr::LOSE = "res/sound/old_bida/lose.mp3";

const char * SoundMgr::SOUND_MATCHED = "res/sound/music/opponent_matched.mp3";

const float SoundMgr::FADE_TIME = 0.5f;

const std::string SoundMgr::UPDATE_FADING_KEY = "UpdateFading";
