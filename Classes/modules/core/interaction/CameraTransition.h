#ifndef CameraTransition_h_
#define CameraTransition_h_

#include "CameraPosition.h"
#include <modules/camera-view/GameCamera.h>
#include <modules/core/utils/LerpUtils.h>
#include <data/DataStruct.h>
#include <data/GameConstant.h>
#include <modules/core-implement/Cue.h>
#include <modules/core/utils/TriggerUtil.h>

class CameraTransition
{
public:
	typedef std::function<void(bool)> ROutOfRangeCallBack;

	CameraTransition();
	~CameraTransition();

	void update(float dt);

	void setCamera(GameCamera *camera);

	void setCue(Cue* cue);

	void transLerp(ps::ExtMath::Rpt & viewRPT, ps::ExtMath::vector & viewPos, double underPSign = 0, CUSTOM_CALLBACK callback = nullptr);

	void dispatchLerpImmediately();

	void lerpSimplifyRadian();

	void onROutRange(bool outRange);

	void subscribeOnROutOfRange(ROutOfRangeCallBack listener);

	bool isLockP();

	void releaseInteract(ps::ExtMath::Rpt & viewRPT, ps::ExtMath::vector viewPos);

	void transTrig(ps::ExtMath::Rpt viewRPT, ps::ExtMath::vector viewPos, double time, CUSTOM_CALLBACK callback);

	void makeTransTrig(CamCoord current, CamCoord target, double time);

	void onTransComplete(ps::ExtMath::Rpt camRPT, ps::ExtMath::vector viewPos);

	void lerpHandler(ps::ExtMath::Rpt rpt, ps::ExtMath::vector viewPos);

	void trigHandler(double dt, tu::TransitionData* data);

	void updateCameraCoord();

	ps::ExtMath::Rpt convertCoordToRPT(const CamCoord & coord);

	CamCoord convertRPTtoCoord(ps::ExtMath::Rpt rpt, ps::ExtMath::vector off);

	double calcMaxViewR(ps::ExtMath::Rpt viewRPT, ps::ExtMath::vector viewPos);

	void enable2DCam();

	void release2DCam(ps::ExtMath::Rpt viewRPT, ps::ExtMath::vector viewPos, CUSTOM_CALLBACK callback = nullptr);
	
	void release2DCamComplete();

	void lerp2DHandler(ps::ExtMath::Rpt rpt, ps::ExtMath::vector off);

	void subscribeOnUpdate(CUSTOM_CALLBACK callback);

	void dispatchUpdateListener();

	
private:

	GameCamera *_camera;
	RPTOLerp *_lerp;
	tu::LimitedTimeTrigger *_trig;
	CameraPosition *_cp;

	ps::ExtMath::Rpt _rpt;
	ps::ExtMath::vector _off;
	
	RPTOLerp *_2DLerp;
	bool _flag2DLerpCompleted;
	CUSTOM_CALLBACK _updateListener;
	
	Cue *_cue;
	double _currentP;
	bool _lockP;

	bool _rOutOfRange;
	ROutOfRangeCallBack _listenerROutOfRange;

	CUSTOM_CALLBACK _transCompleteCallback;

public:
	const static ps::ExtMath::Rpt TARGET_2D_CAM_RPT;
	const static int CUE_OPACITY_HIGH = 255;
	const static int CUE_OPACITY_LOW = 80;
	const static double LERP_VALUE;
	const static double LERP_2D_VALUE;
	const static double INIT_P;
};

#endif // !CameraTransition_h_

