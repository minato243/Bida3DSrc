#ifndef LERPUTILS_H_
#define LERPUTILS_H_

#include <modules/core/ps/internal/ExtensionMath.hpp>
#include <functional>
#include <cocos2d.h>
#include <data/DataStruct.h>
#include <data/GameConstant.h>

extern const double NUMBER_LERP_STOP_RPT;

template<typename T>
class BaseLerp
{
public:
	virtual void dispatch() {};
	void setEnabled(bool enabled) { _enabled = enabled; };
	void setCurent(T current) { _current = current; };
	T getCurrent() { return _current; };
protected:
	bool _enabled = false;
	T _current;
};

class SphericalLerp : public BaseLerp<ps::ExtMath::Rpt>{
public:
	typedef std::function<void(ps::ExtMath::Rpt)> SphericalLerpCallBack;
	SphericalLerp(double lerp, const SphericalLerpCallBack &listener);
	~SphericalLerp();
	void setCurrent(ps::ExtMath::Rpt current);
	void setTarget(ps::ExtMath::Rpt target);
	void fastForward();
	void dispatch();
	void update(float dt);
	void simplifyRadian();
	ps::ExtMath::Rpt getCurrent();
	void onceCompleted(std::function<void()> callback);
	static SphericalLerp* makeLerp(double lerp, const SphericalLerpCallBack &listener);

private:
	double _lerp;
	SphericalLerpCallBack _listener;
	int _context;

public:
	ps::ExtMath::Rpt _current;
	ps::ExtMath::Rpt _target;
};

class VectorLerp: public BaseLerp<ps::ExtMath::vector> {
public:
	typedef std::function<void(ps::ExtMath::vector)> VectorLerpCallBack;

	VectorLerp(double lerp, VectorLerpCallBack listener);
	~VectorLerp();

	void setCurrent(ps::ExtMath::vector current);
	void setTarget(ps::ExtMath::vector current);
	void fastForward();
	virtual void dispatch();
	void update(float dt);
	const ps::ExtMath::vector & getCurrent();

	static VectorLerp *makeLerp(double lerp, const VectorLerpCallBack &listener);

private:
	double _lerp;
	ps::ExtMath::vector _current;
	ps::ExtMath::vector _target;
	
	VectorLerpCallBack _listener;
	int _context;
};

class NumberLerp: public BaseLerp<double>{
public:
	typedef std::function<void(double)> NumberLerpCallBack;

	NumberLerp(double lerp, const NumberLerpCallBack &listener);
	~NumberLerp();

	void setCurrent(double current);
	void setTarget(double target);
	void fastForward();
	void dispatch();
	void update(float dt);
	static NumberLerp* makeLerp(double lerp, const NumberLerpCallBack &listener);
private:
	double _lerp;
	double _current;
	double _target;
	NumberLerpCallBack _listener;
	int _context;
};

class RPTOLerp{
public:
	typedef std::function<void(ps::ExtMath::Rpt, ps::ExtMath::vector)> RPTOLerpCallBack;

	RPTOLerp(double lerp, const RPTOLerpCallBack &listener);
	~RPTOLerp();

	void setCurrent(ps::ExtMath::Rpt currentRPT, ps::ExtMath::vector currentOff);
	
	void setTarget(const ps::ExtMath::Rpt & targetRPT, const ps::ExtMath::vector & targetOff);
	void setTarget(ps::ExtMath::Rpt targetRPT);

	void setEnabled(bool enabled);
	
	void fastForward();
	
	void dispatch();

	void update(float dt);

	void handleRPT(ps::ExtMath::Rpt value);
	void handleOff(ps::ExtMath::vector value);
	void simplifyRadian();
	void onceCompleted(CUSTOM_CALLBACK callback);
	void clearOnceCompletedListener();
	void _elementOnceCompleted();
	static RPTOLerp* makeLerp(double lerp, const RPTOLerpCallBack &listener);
private:
	ps::ExtMath::Rpt _resultRPT;
	ps::ExtMath::vector _resultOff;

	bool _updated;
	bool _enabled = false;

	RPTOLerpCallBack _listener;
	CUSTOM_CALLBACK _completedListener;
public:
	SphericalLerp* _rptLerp;
	VectorLerp* _offLerp;
};

#endif // !LERPUTILS_H_
