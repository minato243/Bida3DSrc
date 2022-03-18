#ifndef GameSound_h__
#define GameSound_h__

#include <vector>
#include <string>

const int VOICES_NUMBER = 3;

unsigned int playEffect(const std::string &path, bool isLoop = false, float gain = 1.0f);

class GameSound
{
public:
	GameSound();
	~GameSound();
	static void stopMusic();
	static void playSoundSpinWheel();
	static void playSoundGiftNormal();
	static void playSoundGiftBig();
	static std::string getRandomSound(std::string path, int number, std::string duoi);
	static bool getRandom(int percent);
	static void playDkThoat();
	static void playJackpot();
	static void playHuyThoat();
	static void playClick();
	static void playAnBai();
	static void playCardFinish();
	static void playLose();
	static void playWin();
	static void playStart();
	static void shuffleCard();
	static void playChiaBai();
	static void playBocBai();
	static void playCupFly();
	static void playDanhBai();
	static void playNewJoin();
	static void playNextTurn();
	static void playSwiftCard();
	static void playXepBai();
	static void playPopUp();
	static void playCountDown();
	static void playPotOut();
	static void playRegQuit(bool success);
	static void playVoiceDiscard();
	static void playVoiceEatPot();
	static void playVoiceThinkSlow();
	static void playVoiceWin();
	static void playVoiceWinTrophy();
	static void playVoiceEatCard();
	static void playVoiceDraw();
	static void playVoicePass();
	static void playVoiceWelcome();
	static void playBackgroundMusic();
	static void playInGameBackgroundMusic();
	static void stopBackgroundMusic();

	static void stopAllEffects();
private:
	static int _playerVoice;
	static int _dealerVoice;
};

class BoardSound
{
public:
	BoardSound();
	~BoardSound();
public:
	static std::string getRandom(std::string prefix, int number, std::string postfix);
	static void shuffle();
	static void dealCard();
	static void cardWithdraw();
	static void flipCard();
	static void deckCards();
	static void slideChip();
	static void tableChip(int number);
	static void trayChip(int number);
	static void allInDrop();
	static void allInDealerTray();
	static void allInPlayerTray();
	static void sieteYMedia();
	static void onStart();
	static void onTurn();
	static void cardPush();
	static void cardFlipDeal();
	static void effectResult(bool isWin);
	static void playPoint(int point);
	static void playBust();
	static void playDealer();
	static void playEnd();
	static void playWin();
	static void playLose();
	static void playJoinBoard();
	static void playLeaveBoard();


};
#endif // GameSound_h__
