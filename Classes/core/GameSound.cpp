#include "GameSound.h"
#include "cocos2d.h"
#include <vector>
#include "SimpleAudioEngine.h"
#include "GameData.h"
using namespace CocosDenshion;
using namespace std;
USING_NS_CC;
//lobby
const char* sound_backgroundMusic = "sounds/mobile/bg.mp3";
const char* sound_chupanh = "sounds/mobile/chupanh.mp3";
const char* sound_click = "sounds/mobile/click.mp3";
const char* sound_coinFall = "sounds/mobile/coinFall.mp3";
const char* sound_coin1 = "sounds/mobile/coin_01.mp3";
const char* sound_coin2 = "sounds/mobile/coin_02.mp3";
const char* sound_coin3 = "sounds/mobile/coin_03.mp3";
const char* sound_giftNormal = "sounds/mobile/giftOnl_normal.mp3";
const char* sound_giftBig = "sounds/mobile/giftOnl_big.mp3";
const char* sound_spinWheel = "sounds/mobile/giftOnl_spinWheel.mp3";
//sounds
const char* sound_jackpot = "sounds/mobile/jackpot.mp3";
const char* sound_rainGold = "sounds/mobile/coinFall.mp3";
const char* sound_yourturn = "sounds/mobile/next_turn.mp3";
const char* sound_specify = "sounds/mobile/s_bobai.mp3";

const char* sound_an_bai = "sounds/mobile/an_bai.mp3";
const char* sound_win = "sounds/mobile/music_win.mp3";
const char* sound_lose = "sounds/mobile/music_lose.mp3";
const char* sound_start = "sounds/mobile/start_game.mp3";
const char* sound_bocbai = "sounds/mobile/lat_bai.mp3";
const char* sound_cup_fly = "sounds/mobile/cup_fly.mp3";
const char* sound_danhbai = "sounds/mobile/ha_bai.mp3";
const char* sound_new_join = "sounds/mobile/new_join.mp3";
const char* sound_next_turn = "sounds/mobile/next_turn.mp3";
const char* sound_pop_up = "sounds/mobile/pop_up.mp3";
const char* sound_huythoat = "sounds/mobile/huythoat.mp3";
const char* sound_dangkythoat = "sounds/mobile/dangkythoat.mp3";
const char* sound_coin_start = "sounds/mobile/coin_start.mp3";
const char* sound_chia_bai = "sounds/mobile/chia_bai.mp3";
const char* sound_countdown = "sounds/mobile/countdown.mp3";
const char* sound_swift_card = "sounds/mobile/swift_card.mp3";
const char* sound_xep_bai = "sounds/mobile/xep_bai.mp3";
const char* sound_shuffleCard = "sounds/mobile/shuffle_card.mp3";
const char* sound_cardFinish = "sounds/mobile/card_finish.mp3";

const char* sound_bg_lobby = "sounds/mobile/bg_lobby.mp3";
const char* sound_bg_ingame = "sounds/mobile/bg_ingame.mp3";
//board sound
const vector<string> board_sound_voice_point = {
	"sounds/board/voice/0.5.mp3",
		"sounds/board/voice/1.mp3",
		"sounds/board/voice/1.5.mp3",
		"sounds/board/voice/2.mp3",
		"sounds/board/voice/2.5.mp3",
		"sounds/board/voice/3.mp3",
		"sounds/board/voice/3.5.mp3",
		"sounds/board/voice/4.mp3",
		"sounds/board/voice/4.5.mp3",
		"sounds/board/voice/5.mp3",
		"sounds/board/voice/5.5.mp3",
		"sounds/board/voice/6.mp3",
		"sounds/board/voice/6.5.mp3",
		"sounds/board/voice/7.mp3",
		"sounds/board/voice/7.5.mp3"
};
const vector<string> board_sound_voice_bust = {
	"sounds/board/voice/bust.mp3"
};
const vector<string> board_sound_voice_dealer = {
	"sounds/board/voice/dealer.mp3"
};
const vector<string> board_sound_voice_end = {
	"sounds/board/voice/end_0.mp3",
	"sounds/board/voice/end_1.mp3"
};
const vector<string> board_sound_voice_lose = {
	"sounds/board/voice/lose_0.mp3",
	"sounds/board/voice/lose_1.mp3"
};
const vector<string> board_sound_voice_win = {
	"sounds/board/voice/win_0.mp3",
	"sounds/board/voice/win_1.mp3"
};
const vector<string> board_sound_join_board_chip_sound = {
	"sounds/board/chip/tray_into1_01.mp3",
		"sounds/board/chip/tray_into1_02.mp3",
		"sounds/board/chip/tray_into1_03.mp3",
		"sounds/board/chip/tray_into1_04.mp3",
		"sounds/board/chip/tray_into1_05.mp3"
};
const vector<string> board_sound_leave_board_chip_sound = {
	"sounds/board/chip/chipSlide1.mp3",
		"sounds/board/chip/chipSlide2.mp3",
		"sounds/board/chip/chipSlide3.mp3",
		"sounds/board/chip/chipSlide4.mp3",
		"sounds/board/chip/chipSlide5.mp3",
		"sounds/board/chip/chipSlide6.mp3",
		"sounds/board/chip/chipSlide7.mp3"
};

int randInSize(int size)
{
	int value = CCRANDOM_0_1() * size;
	if (value == size)
		value = 0;
	return value;
}
unsigned int playEffect(const std::string &path, bool isLoop, float gain) {
	if (!gamedata->sound)
		return 0;
	return SimpleAudioEngine::getInstance()->playEffect(path.c_str(), isLoop, 1.0f, 0.0f, gain);
}
void playMusic(const std::string &path, bool isLoop = false) {
	if (!gamedata->music)
		return;
	SimpleAudioEngine::getInstance()->playBackgroundMusic(path.c_str(), isLoop);
}
void playRandom(vector<string> sounds, bool isLoop = false, float pitch = 1.0f)
{
	playEffect(sounds[randInSize(sounds.size())], isLoop, pitch);
}

GameSound::GameSound()
{
}

GameSound::~GameSound()
{
}



void GameSound::stopMusic()
{
	SimpleAudioEngine::getInstance()->stopBackgroundMusic();
}

void GameSound::playSoundSpinWheel()
{
	playEffect(sound_spinWheel);
}

void GameSound::playSoundGiftNormal()
{
	playEffect(sound_giftNormal);
}

void GameSound::playSoundGiftBig()
{
	playEffect(sound_giftBig);
}

std::string GameSound::getRandomSound(std::string path, int number, std::string duoi)
{
	int rd = floor(CCRANDOM_0_1() * number + 1);
	if (rd > number)
		rd = number;

	std::string ret = StringUtils::format("%s%d.%s", path.c_str(),rd, duoi.c_str());
	return ret;
}

bool GameSound::getRandom(int percent)
{
	int test = floor(CCRANDOM_0_1() * 100);
	return test <= percent;
}

void GameSound::playDkThoat()
{
	playEffect(sound_dangkythoat);
}

void GameSound::playJackpot()
{

}

void GameSound::playHuyThoat()
{
	playEffect(sound_jackpot);
	playEffect(sound_rainGold);
}

void GameSound::playClick()
{
	playEffect(sound_click);
}

void GameSound::playAnBai()
{
	playEffect(sound_an_bai);
}

void GameSound::playCardFinish()
{
	playEffect(sound_cardFinish);
}

void GameSound::playLose()
{
	playEffect(sound_lose);
}

void GameSound::playWin()
{
	playEffect(sound_win);
}

void GameSound::playStart()
{
	playEffect(sound_start);
}

void GameSound::shuffleCard()
{
	playEffect(sound_shuffleCard);
}

void GameSound::playChiaBai()
{
	playEffect(sound_chia_bai);
}

void GameSound::playBocBai()
{
	playEffect(sound_bocbai);
}

void GameSound::playCupFly()
{
	playEffect(sound_cup_fly);
}

void GameSound::playDanhBai()
{
	playEffect(sound_danhbai);
}

void GameSound::playNewJoin()
{
	playEffect(sound_new_join);
}

void GameSound::playNextTurn()
{
	playEffect(sound_next_turn);
}

void GameSound::playSwiftCard()
{
	playEffect(sound_swift_card);
}

void GameSound::playXepBai()
{
	playEffect(sound_xep_bai);
}

void GameSound::playPopUp()
{
	playEffect(sound_pop_up);
}

void GameSound::playCountDown()
{
	playEffect(sound_countdown);
}

void GameSound::playPotOut()
{
	playEffect(sound_coin_start);
}

void GameSound::playRegQuit(bool success)
{
	if (success) {
		playEffect(sound_dangkythoat);
	}
	else {
		playEffect(sound_huythoat);
	}
}

void GameSound::playVoiceDiscard()
{
	cocos2d::log("need playVoiceDiscard");
}

void GameSound::playVoiceEatPot()
{

}

void GameSound::playVoiceThinkSlow()
{

}

void GameSound::playVoiceWin()
{

}

void GameSound::playVoiceWinTrophy()
{

}

void GameSound::playVoiceEatCard()
{

}

void GameSound::playVoiceDraw()
{

}

void GameSound::playVoicePass()
{

}

void GameSound::playVoiceWelcome()
{

}

void GameSound::playBackgroundMusic()
{
	playMusic(sound_bg_lobby, true);
	
}

void GameSound::playInGameBackgroundMusic()
{
	playMusic(sound_bg_ingame, true);
}

void GameSound::stopBackgroundMusic()
{
	stopMusic();
}

void GameSound::stopAllEffects()
{
	SimpleAudioEngine::getInstance()->stopAllEffects();
}

int GameSound::_playerVoice = 0;
int GameSound::_dealerVoice = 0;

BoardSound::BoardSound()
{

}

BoardSound::~BoardSound()
{

}

std::string BoardSound::getRandom(std::string prefix, int number, std::string postfix)
{
	return  StringUtils::format("%s%d%s", prefix.c_str(), number, postfix.c_str());
}

void BoardSound::shuffle()
{
	playEffect("sounds/board/card/cardShuffle.mp3", false, 0.1f);
}

void BoardSound::dealCard()
{
	playEffect(BoardSound::getRandom("sounds/board/card/cardDeal", 7, ".mp3"), false, 0.2f);

}

void BoardSound::cardWithdraw()
{
	playEffect(BoardSound::getRandom("sounds/board/card/cardWithdraw", 2, ".mp3"), false, 0.05f);
}

void BoardSound::flipCard()
{
	playEffect("sounds/board/card/cards_flip.mp3");
}

void BoardSound::deckCards()
{
	playEffect(BoardSound::getRandom("sounds/board/card/deck_cards_", 4, ".mp3"), false, 0.05f);
}

void BoardSound::slideChip()
{
	playEffect(BoardSound::getRandom("sounds/board/card/chipSlide", 7, ".mp3"));
}

void BoardSound::tableChip(int number)
{
	playEffect(BoardSound::getRandom(StringUtils::format("sounds/board/chip/put%dchip_0", number), 7, ".mp3"), false, 0.25f);
}

void BoardSound::trayChip(int number)
{
	playEffect(BoardSound::getRandom(StringUtils::format("sounds/board/chip/tray_into%d_0", number), 5, ".mp3"));
}

void BoardSound::allInDrop()
{
	playEffect("sounds/board/allIn/bigChipDrop.mp3");

}

void BoardSound::allInDealerTray()
{
	playEffect("sounds/board/allIn/dealer_chipToTray.mp3");
}

void BoardSound::allInPlayerTray()
{
	playEffect("sounds/board/allIn/player_chipToTray.mp3");
}

void BoardSound::sieteYMedia()
{
	playEffect("sounds/board/other/maxHand.mp3");
}

void BoardSound::onStart()
{
	playEffect("sounds/board/other/onStart.mp3");
}

void BoardSound::onTurn()
{
	playEffect("sounds/board/other/onTurn.mp3");
}

void BoardSound::cardPush()
{
	playEffect("sounds/board/card/cardPush.mp3", false, 0.1);
}

void BoardSound::cardFlipDeal()
{
	playEffect("sounds/board/card/cardFlipDeal.mp3", false, 0.1);
}

void BoardSound::effectResult(bool isWin)
{
	if (isWin) {
		playEffect("sounds/board/other/music_win.mp3", false, 0.5);
	}
	else {
		playEffect("sounds/board/other/music_lose.mp3", false, 0.5);
	}
}

void BoardSound::playPoint(int point)
{
	point = round(point * 2) - 1;
	if (point >= board_sound_voice_point.size()) {
	}
	else
		playEffect(board_sound_voice_point[point]);
}

void BoardSound::playBust()
{
	playRandom(board_sound_voice_bust);
}

void BoardSound::playDealer()
{
	playRandom(board_sound_voice_dealer);
}

void BoardSound::playEnd()
{
	playRandom(board_sound_voice_end);
}

void BoardSound::playWin()
{
	playRandom(board_sound_voice_win);
}

void BoardSound::playLose()
{
	playRandom(board_sound_voice_lose);
}

void BoardSound::playJoinBoard()
{
	playRandom(board_sound_join_board_chip_sound, false, 0.2f);
}

void BoardSound::playLeaveBoard()
{
	playRandom(board_sound_leave_board_chip_sound);
}
