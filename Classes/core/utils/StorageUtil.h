#ifndef StorageUtil_h__
#define StorageUtil_h__
#include <string>

class StorageUtil
{
public:
	StorageUtil();
	~StorageUtil();

public:
	static std::string getString(std::string key, std::string defValue = "");
	static int getInt(std::string key, int defValue = 0);
	static bool getBool(std::string key, bool defValue = false);
	static void setInt(std::string key, int value);
	static void setString(std::string key, std::string value);
	static void setBool(std::string key, bool value);
};
#endif // StorageUtil_h__
