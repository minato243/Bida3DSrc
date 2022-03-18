#include "ModelOffset.h"
#include <cocos2d.h>
#include <json/document.h>
#include "../../../data/config/ConfigMgr.h"
#include "../../../data/Resource.h"

USING_NS_CC;
using namespace rapidjson;

ModelOffset::ModelOffset()
{
	initOffsetDefault();
}

ModelOffset::~ModelOffset()
{

}

void ModelOffset::initOffsetDefault()
{
	_modelWidthByPath.insert(std::pair<std::string, float>(Res::CUE_VANG_1, 248));
	_modelWidthByPath.insert(std::pair<std::string, float>(Res::CUE_VANG_2, 254));
	_modelWidthByPath.insert(std::pair<std::string, float>(Res::CUE_DO_1, 260));
	_modelWidthByPath.insert(std::pair<std::string, float>(Res::CUE_DO_2, 268));
	_modelWidthByPath.insert(std::pair<std::string, float>(Res::CUE_TIM_1, 236));
	_modelWidthByPath.insert(std::pair<std::string, float>(Res::CUE_TIM_2, 247));
	_modelWidthByPath.insert(std::pair<std::string, float>(Res::CUE_XANH_1, 226));
	_modelWidthByPath.insert(std::pair<std::string, float>(Res::CUE_XANH_2, 238));

	Document *configs = configMgr->cueConfig->cueModelData;
	for (rapidjson::Value::ConstMemberIterator itr = (*configs).MemberBegin(); itr != (*configs).MemberEnd(); itr++)
	{
		auto type = atoi(itr->name.GetString());

		for (rapidjson::Value::ConstMemberIterator itr2 = itr->value.MemberBegin(); itr2 != itr->value.MemberEnd(); itr2++)
		{
			for (rapidjson::Value::ConstMemberIterator itr3 = itr2->value.MemberBegin(); itr3 != itr2->value.MemberEnd(); itr3++)
			{
				auto model = itr3->value["model"].GetString();

				if (type == 1) {
					_modelWidthByPath.insert(std::pair<std::string, float>(model, 185));
				}
				if (type == 2) {
					_modelWidthByPath.insert(std::pair<std::string, float>(model, 223));
				}
				if (type == 3) {
					_modelWidthByPath.insert(std::pair<std::string, float>(model, 161));
				}
				if (type == 4) {
					_modelWidthByPath.insert(std::pair<std::string, float>(model, 137));
				}
			}
		}
	}
}

float ModelOffset::getModelWidthByPath(std::string model)
{
	if (_modelWidthByPath.find(model) == _modelWidthByPath.end())
		return 0;
	return _modelWidthByPath[model];
}

ModelOffset * ModelOffset::_instance = nullptr;

ModelOffset * ModelOffset::getInstance()
{
	if (_instance == nullptr)
	{
		_instance = new ModelOffset();
	}
	return _instance;
}

