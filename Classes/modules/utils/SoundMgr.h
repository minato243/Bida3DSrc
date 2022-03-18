#ifndef SOUNDMGR_H_
#define SOUNDMGR_H_

#include <cocos2d.h>
#include <string>

#define soundMgr SoundMgr::getInstance()

class SoundMgr
	:public cocos2d::Ref
{
public:
	SoundMgr();
	~SoundMgr();

	void soundShoot(double force);
	
	void soundBallCollide(double velocity);
	
	void soundBreak(double velocity);

	void soundBallPocketed(const int & ballNum);

	void soundBallPocketedNoNumber();

	void soundEightBallWin();

	void soundEightBallLose();

	void soundButton();

	void soundFoul();

	void soundWorkshop(int state);

	void soundStar();

	void playWin(bool isWin);

	void playBgLobby();

	void playBgIngame();

	void stopBgLobby();

	void stopBgIngame();

	void onShow();

	void onHide();

	void playBgSearchingOpponent();

	void playBgMusic(const std::string & path, bool fade = true);

	void updateFading(float dt);

	void playSoundMatch();

private:
	static SoundMgr * _instance;
	int _lobbyBg;
	int _ingameBg;
	std::map<std::string, int> _bgIdMap;
	std::string _curBgPath;

	float _fadeBgTimeRemain;

public:
	static SoundMgr *getInstance();

	static const char *LOBBY_BG_MUSIC;
	const static std::vector<std::string> INGAME_BG_MUSIC;

	static const char *INGAME_BG_MUSIC1;
	static const char *INGAME_BG_MUSIC2;
	static const char *SEARCHING_BG_PATHS;
	static const char *WIN;
	static const char *LOSE;
	static const char *SOUND_MATCHED;

	static const float FADE_TIME;

	static const std::string UPDATE_FADING_KEY;
};


#endif // !SOUNDMGR_H_


