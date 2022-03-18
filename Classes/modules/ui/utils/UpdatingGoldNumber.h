#ifndef UpdatingGoldNumber_h_
#define UpdatingGoldNumber_h_

#include <cocos2d.h>
#include <cocos/ui/CocosGUI.h>

class UpdatingGoldNumber
{
public:

	UpdatingGoldNumber(cocos2d::ui::Text *lb, long long startNum, long long endNum, const std::string & key);
	~UpdatingGoldNumber();

	void startUpdate();

	void update(float dt);

	static UpdatingGoldNumber* create(cocos2d::ui::Text *lb, long long startNum, long long endNum, const std::string & key);

private:
	long long _startNumber;
	long long _endNumber;
	long long _curNumber;
	long long _d;
	std::string _key;
	cocos2d::ui::Text *_lb;
};
#endif // UpdatingGoldNumber_h_
