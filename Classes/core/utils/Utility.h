#ifndef UTILITY_H_
#define UTILITY_H_

#include <stdio.h>

class Utility
{
public:
	static void writeToFile(const char *fileName, const char * content, bool writeDevice = false);
};

#define WRITE_FILE_LOG true
template <typename... REST> void CUSTOMLOG(const char *formart, const REST...rest)
{
#if (WRITE_FILE_LOG)
	char text[500];
	char textEndLine[500];
	sprintf(text, formart, rest...);
	sprintf(textEndLine, "%s\n", text);
	Utility::writeToFile("CustomLog", textEndLine);
#endif
	CCLOG(formart, rest...);
}


#endif // !UTILITY_H_


