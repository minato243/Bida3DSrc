#include "Utility.h"
#include <sstream>
#include <iosfwd>
#include <fstream>
#include <string>
#include <iostream>

void Utility::writeToFile(const char * fileName, const char * content, bool writeDevice)
{
#if (CC_TARGET_PLATFORM == CC_PLATFORM_WIN32)
	char path[500];
	sprintf(path, "D:/Log/%s", fileName);
	FILE *fp = fopen(path, "a");
	if (fp)
	{
		fputs(content, fp);
		fclose(fp);
	}
	return;
#endif

	//if (writeDevice)
	//{
	//	char path[200];
	//	PlatformUtil::createResFolder();
	//	sprintf(path, (PlatformUtil::getExternalStorageDirectory() + "%s").c_str(), fileName);
	//	FILE *fp = fopen(path, "a");
	//	if (fp)
	//	{
	//		fputs(content, fp);
	//		fclose(fp);
	//	}
	//}
}
