#ifndef ModelOffset_h_
#define ModelOffset_h_

#include <map>

#define modelOffset ModelOffset::getInstance()

class ModelOffset
{
public:
	ModelOffset();
	~ModelOffset();

	void initOffsetDefault();

	float getModelWidthByPath(std::string model);

private:
	std::map<std::string, float> _modelWidthByPath;

public:
	static ModelOffset *_instance;
	static ModelOffset *getInstance();

};
#endif
