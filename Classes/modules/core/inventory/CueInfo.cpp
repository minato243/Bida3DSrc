#include "CueInfo.h"
#include <cocos2d.h>
#include "modules/core/ps/internal/PhysicsConstants.hpp"
#include "../../../core/utils/LanguageMgr.h"
#include "../../../data/config/ConfigMgr.h"
#include "../../../data/Resource.h"


CueInfo::CueInfo()
{
	_type = 0;
}

CueInfo::~CueInfo()
{

}

CueInfo::CueInfo(const CueInfoData & data)
{
	initWithServerData(data);
}

void CueInfo::initWithData(const CueInfoData & data)
{
	_type = data.type;
	_configId = data.configId;

	_tier = MAX(1, MIN(5, data.star));
	_batteryDrainRate = 1;
	_level = data.level;

	_isInBag = false;
	_isUsing = false;
	_aim = data.aim;
	_accuracy = data.accuracy;
	_force = data.force;
	_angular = data.angular;
	_angle[0] = data.angle[0];
	_angle[1] = data.angle[1];
	
	_name = data.name;
}

void CueInfo::initWithServerData(const CueInfoData & data)
{
	_id = data.id;
	_type = data.type;
	_configId = data.configId;

	_tier = MAX(1, MIN(5, data.star));
	_batteryDrainRate = 1;
	_level = data.level;
	initWithConfig(data.configId);

	_isInBag = false;
	_isUsing = false;
	if(data.aim != 0)
		_aim = data.aim;

	_accuracy = data.accuracy;
}

void CueInfo::initWithConfig(int cueConfigId)
{
	auto config = CueInfo::getCueConfig(_type, (_level -1) /10 + 1, (_level - 1) % 10 + 1);

	_force = config.force;
	_tip_e = ps::PhysicsConstants::TIP_E;
	_tip_coef = config.tip_u;
	_accuracy = config.accuracy;
	_aim = config.aim;

	switch (_type) {
	case 1: _name = languageMgr->localizedText("name_cue_song_toan"); break;
	case 2: _name = languageMgr->localizedText("name_cue_boc_pha"); break;
	case 3: _name = languageMgr->localizedText("name_cue_cau_bong"); break;
	case 4: _name = languageMgr->localizedText("name_cue_xoan_oc"); break;
	}

	_angle[0] = config.angle[0];
	_angle[1] = config.angle[1];
		
	_angular = ceil(_tip_coef * 100);
}

bool CueInfo::isCue()
{
	return true;
}

bool CueInfo::isDisappeared()
{
	return _removed;
}

void CueInfo::use()
{
	if (_isInBag) {
		_isUsing = true;
	}
	else {
		CCLOG("Error not in bag");
	}
}

void CueInfo::stopUse()
{
	_isUsing = false;
}

void CueInfo::putToBag()
{
	_isInBag = true;
	_isUsing = false;
}

bool CueInfo::isUsing()
{
	return _isUsing;
}

bool CueInfo::isInBag()
{
	return _isInBag;
}

bool CueInfo::takeFromBag()
{
	auto ret = _isUsing;

	_isInBag = false;
	_isUsing = false;
	return ret;
}

void CueInfo::onShoot(double force)
{
}

int CueInfo::getBatteryPercentage()
{
	return 0;
}

int CueInfo::getMaxBattery()
{
	return 100;
}

bool CueInfo::isFull()
{
	return false;
}

void CueInfo::charge()
{
}

void CueInfo::upgrade_deprecated()
{
	_tier += 1;
}

void CueInfo::swap(CueInfo & dest)
{
	auto level = _level;
	_level = dest._level;
	dest._level = level;
}

double CueInfo::getForce()
{
	return _force;
}

double CueInfo::getAngular()
{
	return _angular;
}

std::string CueInfo::getAngleString()
{
	char text[50];
	sprintf(text, "%d - %d", ceil(_angle[0]), ceil(_angle[1]));
	return text;
}

double* CueInfo::getAngle()
{
	return _angle;
}

void CueInfo::remove()
{
	_removed = true;
}

TrajectoryLengthData CueInfo::getWorldTrajectoryLengths()
{
	auto oneHundred = ps::PhysicsConstants::TABLE_HEIGHT;
	auto realLength = _aim * oneHundred / 1000;
	TrajectoryLengthData trajectoryLenght(0.7 * realLength, 0.1 * realLength, 0.2 * realLength, 0.3 *realLength);
	return trajectoryLenght;
}

double CueInfo::getDustPercent()
{
	return 0.;
	//return 100 * dust / getDustNeed();
}

void CueInfo::upgradeLevel()
{
	_level += 1;
	initWithConfig(_configId);
	charge();
}

ModelPathData CueInfo::getModelPath()
{
	auto config =configMgr->cueConfig->getModelPath(_type, _tier, _configId);
	if (config.mrao.compare("") == 0) 
	{
		config.mrao = Res::CYAN_DOT_TEXTURE;
	}

	return config;
}

int CueInfo::getDustNeed()
{
	//auto data = Config.CueUpgrade;
	//auto i = 0;
	//for (; i < data.length && data[i].csc[1] < generalPoint; i++) {
	//}
	//return data[i].dust;
	return 0.;
}

int CueInfo::getRandomCueConfig(int type, int star)
{
	return configMgr->cueRangeConfig->getRandomCueConfig(type, star);
}

std::string CueInfo::getRandomCueName()
{
	std::string randomNames[] = {
		"Dem nam mo pho",
		"Thanh pho sau lung",
		"Di vang cuoc tinh",
		"Gia tu con mua",
		"Chuyen tau hoang hon",
		"Sau le bong",
		"Em oi Ha Noi pho"
	};
	return randomNames[rand()%7];
}

CueConfigData CueInfo::getCueConfig(int type, int star, int level)
{
	CCLOG("CueConfigData CueInfo::getCueConfig %d %d %d", type, star, level);
	return configMgr->cueConfig->getCueConfigData(type, star, level);	
}

AccuracyConfigData CueInfo::getCueAccuracyConfig(int type, int star)
{
	return configMgr->cueConfig->getAccuracyConfigData(type, star);
}

ModelPathData CueInfo::getCueModel(int configId, int type, int star, int level)
{
	auto config = configMgr->cueConfig->getModelPath(type, star, configId);
	if (config.mrao.compare("") == 0) {
		config.mrao = Res::CYAN_DOT_TEXTURE;
	}

	return config;
}

TrajectoryLengthData::TrajectoryLengthData()
	:trajectA(0), trajectB(0), trajectC(0), trajectE(0)
{
}

TrajectoryLengthData::TrajectoryLengthData(double trajectA, double trajectB, double trajectC, double trajectE)
	:trajectA(trajectA), trajectB(trajectB), trajectC(trajectC), trajectE(trajectE)
{

}
