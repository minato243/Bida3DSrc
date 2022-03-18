#include "SpinMgr.h"
#include "../../../../data/config/ConfigMgr.h"
#include "../../../ui/minigame/spin/SpinUI.h"
#include "../../../ui/GuiMgr.h"
#include "../../../network/minigame/MiniGameHandler.h"

SpinMgr::SpinMgr()
{
	_numberOfSpin = 0;
}

SpinMgr::~SpinMgr()
{

}

WheelItemConfig SpinMgr::getCellById(int cellId)
{
	return configMgr->spinConfig->getItemConfig(cellId);
}

int SpinMgr::getNumberOfSpin()
{
	return _numberOfSpin;
}

float SpinMgr::getBisectorAngleByCellId(int cellId)
{
	return 360.f / SpinConstant::N_CELL * (cellId + 0.5f);
}

void SpinMgr::setNumberOfSpin(const int & nSpin)
{
	_numberOfSpin = nSpin;
}

void SpinMgr::sendSpinWheel()
{
	miniGameHandler->sendSpinWheel();
}

void SpinMgr::onResponeSpinWheel(const int & cellId)
{
	_numberOfSpin--;
	auto spinUI = dynamic_cast<SpinUI *>(guiMgr->getGui(Popup::SPIN_UI));
	if (spinUI && spinUI->isRunning())
	{
		spinUI->spinWheelToCellId(cellId);
		spinUI->updateNumberOfSpin();
	}
}

void SpinMgr::sendCheatSpin()
{
	miniGameHandler->sendCheatSpin();
}

SpinMgr *SpinMgr::_instance = NULL;
SpinMgr * SpinMgr::getInstance()
{
	if (_instance == NULL)
	{
		_instance = new SpinMgr();
	}

	return _instance;
}

const float SpinConstant::POINTER_ANGLE = 77.3;

const float SpinConstant::CELL_ANGLE = 360.f/ SpinConstant::N_CELL;

const float SpinConstant::BASE_ANGLE_SPEED = 360;
