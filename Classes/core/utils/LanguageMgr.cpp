#include "LanguageMgr.h"
#include "cocos2d.h"
#include "StringUtility.h"
#include "StorageUtil.h"

USING_NS_CC;
using namespace std;


static const std::string LANGUAGE_DEFAULT = LANGUAGE_VI;

static const vector<std::string> LANGUAGES_SUPPORT = { LANGUAGE_VI, LANGUAGE_EN };

static const std::string KEY_CURRENT_LANGUAGE = "game_current_language_select_2";

LocalizedFile::LocalizedFile()
{

}

LocalizedFile::~LocalizedFile()
{

}

void LocalizedFile::loadLocalized(std::string des)
{
	localizedStrings.clear();
	string line, contents;

	// Get data of file
	contents = FileUtils::getInstance()->getStringFromFile(des);

	// Create a string stream so that we can use getline( ) on it
	istringstream fileStringStream(contents);

	// Get file contents line by line
	while (std::getline(fileStringStream, line))
	{
		if (line.find("/*", 0) == string::npos &&
			line.find("//", 0) == string::npos &&
			line.find("*/", 0) == string::npos) //filter the valid string of one line
		{
			std::string::size_type validPos = line.find('=', 0);
			if (validPos != string::npos)
			{
				std::string keyStr = line.substr(0, validPos - 1);
				// get valid string
				std::string subStr = line.substr(validPos + 1, line.size() - 1);

				//trim space
				keyStr.erase(0, keyStr.find_first_not_of(" \t"));
				keyStr.erase(keyStr.find_last_not_of(" \t") + 1);

				subStr.erase(0, subStr.find_first_not_of(" \t"));
				subStr.erase(subStr.find_last_not_of(" \t") + 1);

				//trim \" 
				keyStr.erase(0, keyStr.find_first_not_of("\""));
				int findPosition = subStr.find('\"', 0);
				keyStr.erase(keyStr.find_last_not_of("\"") + 1);

				subStr.erase(0, subStr.find_first_not_of("\""));

				//trim ; character and last \" character
				subStr.erase(subStr.find_last_not_of(";") + 1);
				int position = subStr.find_last_not_of("\"") + 1;
				findPosition = subStr.find('\"', 0);
				if (findPosition != string::npos)
					subStr.erase(findPosition);

				//replace line feed with \n
				string::size_type pos(0);
				string old_value("\\n");
				if ((pos = subStr.find(old_value)) != string::npos)
				{
					for (; pos != string::npos; pos += 1)
					{
						if ((pos = subStr.find(old_value, pos)) != string::npos)
						{
							subStr.erase(pos, 2);
							subStr.insert(pos, 1, '\n');
						}
						else
							break;
					}
				}

				localizedStrings.insert(std::pair<std::string, std::string>(keyStr, subStr));
			}
		}
	}
}

std::string LocalizedFile::getString(std::string key)
{
	std::map<std::string, std::string>::iterator itr = localizedStrings.find(std::string(key));
	if (itr != localizedStrings.end()) {
		return (itr->second).c_str();
	}
	return key;
}


LocalizedImage::LocalizedImage()
{

}

LocalizedImage::~LocalizedImage()
{

}

std::string LocalizedImage::getDir(std::string key)
{
	std::string patchLoad = patchLanguage + localizedStrings[key];
	return patchLoad;
}

void LocalizedImage::loadLocalized(std::string path)
{
	patchLanguage = path;

	localizedStrings.clear();
	string line, contents;

	// Get data of file
	contents = FileUtils::getInstance()->getStringFromFile("lang/Localized_content.txt");

	// Create a string stream so that we can use getline( ) on it
	istringstream fileStringStream(contents);

	// Get file contents line by line
	while (std::getline(fileStringStream, line))
	{
		if (line.find("/*", 0) == string::npos &&
			line.find("//", 0) == string::npos &&
			line.find("*/", 0) == string::npos) //filter the valid string of one line
		{
			std::string::size_type validPos = line.find('=', 0);
			if (validPos != string::npos)
			{
				std::string keyStr = line.substr(0, validPos - 1);
				// get valid string
				std::string subStr = line.substr(validPos + 1, line.size() - 1);

				//trim space
				keyStr.erase(0, keyStr.find_first_not_of(" \t"));
				keyStr.erase(keyStr.find_last_not_of(" \t") + 1);

				subStr.erase(0, subStr.find_first_not_of(" \t"));
				subStr.erase(subStr.find_last_not_of(" \t") + 1);

				//trim \" 
				keyStr.erase(0, keyStr.find_first_not_of("\""));
				int findPosition = subStr.find('\"', 0);
				keyStr.erase(keyStr.find_last_not_of("\"") + 1);

				subStr.erase(0, subStr.find_first_not_of("\""));

				//trim ; character and last \" character
				subStr.erase(subStr.find_last_not_of(";") + 1);
				int position = subStr.find_last_not_of("\"") + 1;
				findPosition = subStr.find('\"', 0);
				if (findPosition != string::npos)
					subStr.erase(findPosition);

				//replace line feed with \n
				string::size_type pos(0);
				string old_value("\\n");
				if ((pos = subStr.find(old_value)) != string::npos)
				{
					for (; pos != string::npos; pos += 1)
					{
						if ((pos = subStr.find(old_value, pos)) != string::npos)
						{
							subStr.erase(pos, 2);
							subStr.insert(pos, 1, '\n');
						}
						else
							break;
					}
				}

				localizedStrings.insert(std::pair<std::string, std::string>(keyStr, subStr));
			}
		}
	}
}

void LocalizedImage::buttonChange(cocos2d::ui::Button* btn, std::string value)
{
	std::string patchLoad = value;
	btn->loadTextures(patchLoad, patchLoad);
	btn->setSize(btn->getVirtualRendererSize());
}

void LocalizedImage::imgChange(cocos2d::ui::ImageView* img, std::string value)
{
	std::string patchLoad = value;
	img->loadTexture(patchLoad);
	img->setSize(img->getVirtualRendererSize());
}


LanguageMgr::LanguageMgr()
{
	hasInit = false;
}

LanguageMgr::~LanguageMgr()
{

}

LanguageMgr* LanguageMgr::getInstance()
{
	if (!_instance) {
		_instance = new LanguageMgr();
	}
	return _instance;
}

void LanguageMgr::init()
{
	hasInit = true;
	for (auto lang : LANGUAGES_SUPPORT){
		addLanguage(lang);
	}

	changeLanguage(loadCurrentLanguage());

	mTextConfig = new LocalizedFile();
	mTextConfig->loadLocalized("GameConfig.txt");

	if (!queueAddLocalizeds.empty()) {

		for (Lang obj : queueAddLocalizeds) {
			addLocalizedText(obj.path, obj.lang);
		}
	}
	queueAddLocalizeds.clear();
}

void LanguageMgr::addLanguage(std::string lang)
{
	if (validateLanguage(lang)) {
		mLanguages.push_back(lang);

		LocalizedFile* objLocal = new LocalizedFile();
		objLocal->loadLocalized(localizedTextPath(lang));
		mTextLocalized[lang] = objLocal;

		LocalizedImage* imgLocal = new LocalizedImage();
		imgLocal->loadLocalized(localizedImagePath(lang));
		mImageLocalized[lang] = imgLocal;
	}
}

void LanguageMgr::addLocalizedText(std::string path, std::string lang)
{
	if (lang.empty()) {
		lang = cLang;
	}

	if (hasInit) {
		if (validateLanguage(lang)) {
			mTextLocalized[lang]->loadLocalized(path);
		}
	}
	else {
		queueAddLocalizeds.clear();
		// cc.log("Queue-",path,lang);
		queueAddLocalizeds.push_back(Lang(path, lang));
	}
}

void LanguageMgr::changeLanguage(std::string lang)
{
	if (validateLanguage(lang)) {
		cLang = lang;
		saveCurrentLanguage();
	}
}

bool LanguageMgr::checkCurrentLanguage(std::string lang)
{
	return (cLang.compare(lang) == 0);
}

bool LanguageMgr::validateLanguage(std::string lang)
{
	for (auto item:LANGUAGES_SUPPORT) {
		if (lang.compare(item) == 0 ) 
			return true;
	}
	return false;
}

std::string LanguageMgr::localizedTextPath(std::string lang)
{
	return cocos2d::StringUtils::format("res/localize/%s.txt", lang.c_str());
}

std::string LanguageMgr::localizedImagePath(std::string lang)
{
	if (lang.compare(LANGUAGE_DEFAULT) == 0)
		return "res";
	return "lang/" + lang;
}

std::string LanguageMgr::localizedText(std::string key)
{
	std::string translated = mTextLocalized[cLang]->getString(key);
	translated = StringUtility::replaceAll(translated, "\\n", "\n");
	return translated;
}

std::string LanguageMgr::localizeImage(std::string res)
{
	if (cLang.compare(LANGUAGE_DEFAULT) == 0) 
		return res;

	std::string sFile = res.substr(0, res.find_last_of("/"));
	return "lang/" + cLang + "/" + sFile;
}

std::string LanguageMgr::getLanguageDir(std::string key)
{
	return mImageLocalized[cLang]->getDir(key);
}

void LanguageMgr::changeLanguageButton(cocos2d::ui::Button* btn, std::string value)
{
	return mImageLocalized[cLang]->buttonChange(btn, value);
}

void LanguageMgr::changeLanguageImage(cocos2d::ui::ImageView* img, std::string value)
{
	return mImageLocalized[cLang]->imgChange(img, value);
}

std::string LanguageMgr::getConfig(std::string key)
{
	return mTextConfig->getString(key);
}

void LanguageMgr::saveCurrentLanguage()
{
	StorageUtil::setString(KEY_CURRENT_LANGUAGE, cLang);
}

std::string LanguageMgr::loadCurrentLanguage()
{
	return StorageUtil::getString(KEY_CURRENT_LANGUAGE, LANGUAGE_DEFAULT);
}

LanguageMgr* LanguageMgr::_instance = nullptr;

std::string LocalizedString::to(std::string key)
{
	return languageMgr->localizedText(key);
}

void LocalizedString::add(std::string path)
{
	languageMgr->addLocalizedText(path, LANGUAGE_EN);
	languageMgr->addLocalizedText(path, LANGUAGE_ES);
}

void LocalizedString::addLang(std::string path, std::string lang)
{
	languageMgr->addLocalizedText(path, lang);
}

std::string LocalizedString::config(std::string key)
{
	return languageMgr->getConfig(key);
}

std::string LocalizedSprite::getDir(std::string key)
{
	return languageMgr->getLanguageDir(key);
}

void LocalizedSprite::buttonLocalized(cocos2d::ui::Button* btn, std::string value)
{
	languageMgr->changeLanguageButton(btn, value);
}

void LocalizedSprite::imgLocalized(cocos2d::ui::ImageView* img, std::string value)
{
	languageMgr->changeLanguageImage(img, value);
}
