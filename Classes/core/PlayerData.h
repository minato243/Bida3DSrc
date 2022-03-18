#ifndef PlayerData_h__
#define PlayerData_h__
#include <string>

class PlayerData{
public:	
	PlayerData() {

	}
	~PlayerData()
	{

	}
public:
	std::string getAvatar() {
		return _avatar;
	}
	void setAvatar(std::string avatar) {
		_avatar = avatar;
	}
	std::string getDisplayName() {
		return _displayName;
	}
	void setDisplayName(std::string displayName) {
		_displayName = displayName;
	}
	std::string getZName() {
		return _zName;
	}
	void setZName(std::string zName) {
		_zName = zName;
	}
	long long getGold() {
		return _gold;
	}
	void setGold(long long gold) {
		_gold = gold;
	}
	long long getCoin() {
		return _coin;
	}
	void setCoin(long long coin) {
		_coin = coin;
	}
private:

	std::string _avatar;
	std::string _displayName;
	std::string _zName;

	std::string _name;
	long long _gold;//bean
	long long _coin;//xu
	int _levelScore;
	int _winCount;
	int _lostCount;
	int _star;
	std::string _uID;

	bool _isLikePage;
	bool _isRateApps;
	std::string _openID;
};
#endif // PlayerData_h__
