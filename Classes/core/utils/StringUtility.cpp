#include "StringUtility.h"
#include "LanguageMgr.h"
#include "CMD5Checksum.h"
#include <regex>

std::string StringUtility::standartNumber(long long number)
{
	std::string tmp = std::to_string(number);
	if (tmp.size() < 4) {
		return tmp;
	}
	std::string tmp2 = "";
	for (int i = 0; i < tmp.size() - 1; i++) {
		if (((i + 1) % 3) == 0) {
			tmp2 =  cocos2d::StringUtils::format(".%c%s", tmp[tmp.size() - i - 1],tmp2.c_str());
		}
		else {
			tmp2 = cocos2d::StringUtils::format("%c%s", tmp[tmp.size() - i - 1], tmp2.c_str());
		}
	}
	tmp2 = cocos2d::StringUtils::format("%c%s", tmp[0],tmp2.c_str());
	return tmp2;
}

std::string StringUtility::formatNumberSymbol(long long number)
{
	std::string retVal = "";
	if (number < 0)
		retVal = "-";
	number = abs(number);
	if (number >= 1000000000) {
		return cocos2d::StringUtils::format("%s%sB", retVal.c_str(), numberConvert(number, 1000000000).c_str());
	}
	else if (number >= 1000000) {
		return cocos2d::StringUtils::format("%s%sM", retVal.c_str(), numberConvert(number, 1000000).c_str());
	}
	else if (number >= 1000) {
		return cocos2d::StringUtils::format("%s%sK", retVal.c_str(), numberConvert(number, 1000).c_str());
	}
	else {
		return cocos2d::StringUtils::format("%s%lld", retVal.c_str(), number);
	}
}

std::string StringUtility::numberConvert(long long number, long long div) {
	long long a = number / (div / 100);
	long long b = a / 100;

	a = a - b * 100;
	if (a == 0) {
		return cocos2d::StringUtils::format("%ld", b);
	}
	else {
		if (a > 9) {
			if (a % 10 == 0) {
				return cocos2d::StringUtils::format("%ld.%ld", b, a / 10);
			}
			else {
				return cocos2d::StringUtils::format("%ld.%ld", b, a);
			}
		}
		else {
			return cocos2d::StringUtils::format("%ld.0%ld", b, a);
		}
	}
}

std::string StringUtility::pointNumber(long long number)
{
	return standartNumber(number);
}

std::string StringUtility::replaceAll(std::string text, const std::string& searchText, const std::string& replaceText)
{
	std::string txt = text;
	if (searchText.empty())
		return text;
	size_t start_pos = 0;
	while ((start_pos = txt.find(searchText, start_pos)) != std::string::npos) {
		txt.replace(start_pos, searchText.length(), replaceText);
	}
	return txt;
}

std::string StringUtility::replaceAll(std::string text, const std::string& searchText, int to) {
	char s_to[12];
	sprintf(s_to, "%d", to);
	return replaceAll(text, searchText, std::string(s_to));
}
std::string StringUtility::replaceAll(std::string text, const std::string& searchText, long long to)
{
	char s_to[30];
	sprintf(s_to, "%lld", to);
	return replaceAll(text, searchText, std::string(s_to));
}
std::string StringUtility::subStringText(std::string& text, cocos2d::ui::Text* lb)
{
	std::string str = lb->getString();
	if (text.size() <= str.size()) return text;

	return cocos2d::StringUtils::format("%s...",text.substr(0, str.size() - 3).c_str());
}

std::string StringUtility::subStringTextLength(std::string& text, int length)
{
	if (text.size() <= length) 
		return text;

	return text.substr(0, length - 2) + "...";
}

std::string StringUtility::longWordBreaker(std::string& text, int length)
{
	int count = 0;
	std::string resultText = "";
	for (int i = 0; i < text.size(); i++) {
		if (text[i] == ' ') count = 0; else count++;
		if (count > length) {
			resultText += " -" + text[i];
			count = 1;
		}
		else
			resultText += text[i];
	}
	return resultText;
}

int StringUtility::getLabelWidth(cocos2d::ui::Text* label)
{
	cocos2d::ui::Text* tempStr = cocos2d::ui::Text::create();
	tempStr->setFontName(label->getFontName());
	tempStr->setFontSize(label->getFontSize());
	tempStr->setString(label->getString());
	return tempStr->getContentSize().width;
}

std::string StringUtility::getDateString(long long time)
{
	return cocos2d::StringUtils::format("%lld", time);
}

std::string StringUtility::getRemainTimeString(long long remainTime)
{
	int totalSeconds = floor(remainTime / 1000);
	int numSeconds = totalSeconds % 60;
	int totalMinutes = floor(totalSeconds / 60);
	int numMinutes = totalMinutes % 60;
	int totalHour = floor(totalMinutes / 60);
	int numHours = totalHour % 24;
	int totalDay = floor(totalHour / 24);

	std::string keyDay = "DAY_TEXT";
	std::string keyHour = "HOUR_TEXT";
	std::string keyMinute = "MINUTE_TEXT";
	std::string keySecond = "SECOND_TEXT";
	std::string strDays = StringUtility::replaceAll(LanguageMgr::getInstance()->localizedText(keyDay), "@day", totalDay);
	std::string strHours = StringUtility::replaceAll(LocalizedString::to(keyHour), "@hour", numHours);
	std::string strMinutes = StringUtility::replaceAll(LocalizedString::to(keyMinute), "@minute", numMinutes);
	std::string strSeconds = StringUtility::replaceAll(LocalizedString::to(keySecond), "@second", numSeconds);

	std::string remainTimeStr = "";
	bool enoughInfoTime = false;
	if (totalDay > 0) {
		remainTimeStr += strDays;
		enoughInfoTime = true;
	}

	if (numHours > 0) {
		remainTimeStr += strHours;
		enoughInfoTime = true;
	}

	if (numMinutes > 0) {
		remainTimeStr += strMinutes;
		enoughInfoTime = true;
	}

	if (numSeconds > 0) {
		remainTimeStr += strSeconds;
	}
	
	if (remainTimeStr.compare("") == 0) {
		remainTimeStr = LocalizedString::to("TIME_UP");
	}

	return remainTimeStr;
}

std::string StringUtility::md5(std::string input)
{
	return CMD5Checksum::GetMD5OfString(input);
}
std::string char2hex(char dec)
{
	char dig1 = (dec & 0xF0) >> 4;
	char dig2 = (dec & 0x0F);
	if (0 <= dig1 && dig1 <= 9) dig1 += 48;    //0,48inascii
	if (10 <= dig1 && dig1 <= 15) dig1 += 97 - 10; //a,97inascii
	if (0 <= dig2 && dig2 <= 9) dig2 += 48;
	if (10 <= dig2 && dig2 <= 15) dig2 += 97 - 10;

	std::string r;
	r.append(&dig1, 1);
	r.append(&dig2, 1);
	return r;
}

std::string StringUtility::encodeURIComponent(std::string decoded)
{
    std::ostringstream oss;
        std::regex r("[!'\\(\\)*-.0-9A-Za-z_~]");

        for (char &c : decoded)
        {
            if (std::regex_match(std::string(1, c), r))
            {
                oss << c;
            }
            else
            {
                oss << "%" << std::uppercase << std::hex << (0xff & c);
            }
        }
        return oss.str();
}

std::string StringUtility::getShortRemainTimeString(long long remainTime)
{
	int totalSeconds = floor(remainTime);
	int numSeconds = totalSeconds % 60;
	int totalMinutes = floor(totalSeconds / 60);
	int numMinutes = totalMinutes % 60;
	int totalHour = floor(totalMinutes / 60);

	char hourStr[10];
	char minuteStr[10];
	char secondStr[10];
	sprintf(hourStr,"%d:", totalHour);
	if (numMinutes < 10) sprintf(minuteStr, "0%d:", numMinutes);
	else sprintf(minuteStr, "%d:", numMinutes);

	if (numSeconds < 10) sprintf(secondStr, "0%d", numSeconds);
	else sprintf(secondStr, "%d", numSeconds);

	std::string remainTimeStr = "";
	bool enoughInfoTime = false;

	if (totalHour > 0) {
		remainTimeStr += hourStr;
		enoughInfoTime = true;
	}

	if (numMinutes > 0) {
		remainTimeStr += minuteStr;
		enoughInfoTime = true;
	}

	if (numSeconds > 0) {
		remainTimeStr += secondStr;
	}

	if (remainTimeStr.compare("") == 0) {
		remainTimeStr = LocalizedString::to("TIME_UP");
	}

	return remainTimeStr;
}

void StringUtility::fillChar(std::string & text, int length, char c)
{
	int textLenght = text.size();
	int num = length - textLenght;
	if(num > 0) text.insert(text.begin(), num, c);
}

std::vector<std::string> StringUtility::split(const std::string str, char c)
{
	std::vector<std::string> ret;
	int start = 0;
	int end = str.find(c);
	while (end != -1) {
		ret.push_back(str.substr(start, end - start));
		start = end + 1;
		end = str.find(c, start);
	}
	ret.push_back(str.substr(start, end - start));
	return ret;
}

std::string StringUtility::getShortTimeString(long long time)
{
	int totalSeconds = time/1000;
	int numSeconds = totalSeconds % 60;
	int totalMinutes = floor(totalSeconds / 60);
	int numMinutes = totalMinutes % 60;
	int totalHour = floor(totalMinutes / 60);

	char secondText[10];
	if (numSeconds < 10) sprintf(secondText, "0%d", numSeconds);
	else sprintf(secondText, "%d", numSeconds);

	char minuteText[10];
	if (numMinutes < 10) sprintf(minuteText, "0%d:", numMinutes);
	else sprintf(minuteText, "%d:", numMinutes);
	
	char hourText[10];
	sprintf(hourText, "%d:", totalHour);

	std::string remainTimeStr = "";
	bool enoughInfoTime = false;

	if (totalHour > 0) {
		remainTimeStr += hourText;
	}

	remainTimeStr += minuteText;
	remainTimeStr += secondText;

	return remainTimeStr;
}

const std::vector<std::string> StringUtility::LIST_MONTH_ES = {
	"Enero", "Febrero", "Marzo", "Abril", "Mayo", "Junio", "Julio", "Agosto", "Septiembre", "Octubre", "Noviembre", "Diciembre"
};

const std::vector<std::string> StringUtility::LIST_MONTH_EN = {
	"January", "February", "March", "April", "May", "June", "July", "August", "September", "October", "November", "December"
};

