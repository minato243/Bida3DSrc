#ifndef StringUtility_h__
#define StringUtility_h__

#include "cocos2d.h"
#include "cocos/ui/CocosGUI.h"
#include "extensions/cocos-ext.h"
#include "cocostudio/CocoStudio.h"

class StringUtility {

public:
	static std::string standartNumber(long long number);
	static std::string formatNumberSymbol(long long number);
	static std::string numberConvert(long long number, long long div);
	static std::string pointNumber(long long number);
	static std::string replaceAll(std::string text, const std::string& searchText, const std::string& replaceText);
	static std::string replaceAll(std::string text, const std::string& searchText, int to);
	static std::string replaceAll(std::string text, const std::string& searchText, long long to);
	static std::string subStringText(std::string& text, cocos2d::ui::Text* lb);
	static std::string subStringTextLength(std::string& text, int length);
	static std::string longWordBreaker(std::string& text, int length);
	static int getLabelWidth(cocos2d::ui::Text* label);
	static std::string getDateString(long long time);
	static std::string getRemainTimeString(long long remainTime);
	static std::string md5(std::string input);
	static std::string encodeURIComponent(std::string s);
	static std::string getShortRemainTimeString(long long remainTime);

	static void fillChar(std::string & text, int length, char c);
	static std::vector<std::string> split(const std::string str, char c);

	static std::string getShortTimeString(long long time);
public:
	static const std::vector<std::string> LIST_MONTH_ES;
	static const std::vector<std::string> LIST_MONTH_EN;
};
#endif // StringUtility_h__
