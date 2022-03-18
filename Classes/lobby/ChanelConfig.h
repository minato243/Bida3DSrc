#ifndef ChanelConfig_h__
#define ChanelConfig_h__
#include <string>
#include <vector>

class ChanelConfig {
public:
	struct Config {
		long long minGold;
		long long maxGold;
		std::vector<long long> bet;
		std::string name;
	};
public:
	ChanelConfig();
	~ChanelConfig();
	static ChanelConfig* instance();
public:
	void setConfig(std::string data);
	int getCurChanel();
	long long getMinBet();
	long long getBet(int id);
	int betLevel(long long gold);
	long long getSuitableBet();
	int getCurrentChannel();
	long long getMaxGoldCanPlayInChannel();
public:
	int betTime;
	std::vector<Config> chanelConfig;
public:
	static bool firstInit;
	static ChanelConfig* _inst;

};
#endif // ChanelConfig_h__
