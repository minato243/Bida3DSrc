#ifndef LanguageMgr_h__
#define LanguageMgr_h__
#include <string>

#include <map>
#include <string>
#include <vector>

#include "cocos/ui/CocosGUI.h"
#include "extensions/cocos-ext.h"
#include "cocostudio/CocoStudio.h"


class LocalizedFile {
public:
	LocalizedFile();
	~LocalizedFile();
public:
	void loadLocalized(std::string des);
	std::string getString(std::string key);
public:
	std::map<std::string, std::string> localizedStrings;
};

class LocalizedImage {
public:
	LocalizedImage();
	~LocalizedImage();
public:
	std::string getDir(std::string key);
	void loadLocalized(std::string path);
	void buttonChange(cocos2d::ui::Button* btn, std::string value);
	void imgChange(cocos2d::ui::ImageView* img, std::string value);
public:
	std::map<std::string, std::string> localizedStrings;
	std::string patchLanguage;
};

#define languageMgr LanguageMgr::getInstance()

class LanguageMgr {
public:
	struct Lang {
		Lang(std::string path_, std::string lang_) :path(path_), lang(lang_) {

		}
		std::string path;
		std::string lang;
	};
public:
	LanguageMgr();
	~LanguageMgr();
	static LanguageMgr* getInstance();
public:
	void init();
	void addLanguage(std::string lang);
	void addLocalizedText(std::string path, std::string lang = "");
	void changeLanguage(std::string lang);
	bool checkCurrentLanguage(std::string lang);
	bool validateLanguage(std::string lang);
	std::string localizedTextPath(std::string lang);
	std::string localizedImagePath(std::string lang);
	std::string localizedText(std::string key);
	std::string localizeImage(std::string res);
	std::string getLanguageDir(std::string dir);
	void changeLanguageButton(cocos2d::ui::Button* btn, std::string value);
	void changeLanguageImage(cocos2d::ui::ImageView* img, std::string value);
	std::string getConfig(std::string key);
	void saveCurrentLanguage();
	std::string loadCurrentLanguage();
public:
	static LanguageMgr* _instance;
	std::string cLang;
	bool hasInit;
	LocalizedFile* mTextConfig;
	std::vector<Lang> queueAddLocalizeds;
	std::vector<std::string> mLanguages;
	std::map<std::string, LocalizedFile*> mTextLocalized;
	std::map<std::string, LocalizedImage*> mImageLocalized;
};

class LocalizedString {
public:
	static std::string to(std::string key);
	static void add(std::string path);
	static void addLang(std::string path, std::string lang);
	static std::string config(std::string key);
};

class LocalizedSprite
{
public:
	static std::string getDir(std::string key);
	static void buttonLocalized(cocos2d::ui::Button* btn, std::string value);
	static void imgLocalized(cocos2d::ui::ImageView* img, std::string value);

};
static const std::string LANGUAGE_EN = "en";
static const std::string LANGUAGE_ES = "es";
static const std::string LANGUAGE_VI = "vi";
#endif // LanguageMgr_h__
