#ifndef CameraPosition_h_
#define CameraPosition_h_

#include <data/DataStruct.h>
#include <modules/core/ps/internal/ExtensionMath.hpp>

struct RestrainData {
	ps::ExtMath::vector low;
	ps::ExtMath::vector high;

	RestrainData() {};
	RestrainData(ps::ExtMath::vector low, ps::ExtMath::vector high)
	{
		this->low = low;
		this->high = high;
	};
};

struct RestrainCacheData {
	ps::ExtMath::vector rootPos;
	double p;
	RestrainData restrains;

	RestrainCacheData() {};
};

class BorderedRectangle {
public:
	BorderedRectangle(double width, double height, double radius);
	BorderedRectangle();
	~BorderedRectangle();

	double calcCircleIntersection(double a, double b, ps::ExtMath::vector start, ps::ExtMath::vector dir);

	ps::ExtMath::vector calcIntersection(ps::ExtMath::vector start, ps::ExtMath::vector dir);

	double _w;
	double _h;
	double _a;
	double _b;
	double _r2;

};

class RestrainValues {
public:
	RestrainValues();
	RestrainValues(double beta);
	RestrainValues(double beta, double fovx, double fovy);
	~RestrainValues();

	void updateValues();

	void setBeta(double beta);

	void setFov(double fovx, double fovy);

	CamCoord calcCamCoordinate(ps::ExtMath::vector lowRestrain, ps::ExtMath::vector highRestrain);

	void clearCache();

	double calcLowRestrainDistance(ps::ExtMath::vector low, ps::ExtMath::vector high, ps::ExtMath::vector viewPos, ps::ExtMath::vector T);
private:
	double _beta;
	double _fovx;
	double _fovy;

	double _kCD;
	double _kAM;
	double _kMD;

	double nAN;
	double nBP;
	double nAB;
	double nAL;
	double nJB;
	double nJL;
	double nBP_nJL;
};

class ElipseCombination
{
public:
	ElipseCombination(double width, double height);
	ElipseCombination();
	~ElipseCombination();

	ps::ExtMath::vector calcTangentPoint(ps::ExtMath::vector normalDir);

private:
	double _width;
	double _height;

	RectData _rect;
	std::vector<ElipseData> _elipses;
	ElipseBoundingData _elipseBounding;
	ElipsePhiData _phi;
};

class CameraPosition
{
public:
	enum class Mode
	{
		NORMAL = 1,
		PTB = 2
	};

	struct OptionalOutputs
	{
		bool rOutRange;
	};

	CameraPosition();
	~CameraPosition();

	void setMode(Mode mode);

	void setFov(double fovx, double fovy);

	RestrainData calcRestrains(ps::ExtMath::vector viewPos, double p);

	CamCoord calcCamCoord(ps::ExtMath::Rpt rpt, ps::ExtMath::vector viewPos);

	double interpolateT(double r);

	ps::ExtMath::Rpt calcCamRPT(ps::ExtMath::Rpt & viewRPT, ps::ExtMath::vector & viewPos, double underPSign = 0, OptionalOutputs *optionalOutputs = nullptr);

	bool isOverR(ps::ExtMath::Rpt viewRPT, ps::ExtMath::vector viewPos);

	bool isUnderR(ps::ExtMath::Rpt viewRPT, ps::ExtMath::vector viewPos);

	double calcExtendedR(RestrainData restrains, ps::ExtMath::Rpt rpt, ps::ExtMath::vector viewPos);
public:
	RestrainValues _rv;
	ElipseCombination _ec;
	ElipseCombination _ptbEC;
	BorderedRectangle _br;
	Mode _mode;
	RestrainCacheData _restrainCache;

public:
	static const double ELIPSE_WIDTH;
	static const double ELIPSE_HEIGHT;
	static const double RECT_WIDTH;
	static const double RECT_HEIGHT;
	static const double RECT_RADIUS;

	static const double MIN_RESTRAIN_LOW;
	static const double MAX_RESTRAIN_HIGH;
	static const double MIN_INTERACT_T;
	static const double MAX_INTERACT_T;
	static const double DELTA_CAM_CUE_T_MIN;
	static const double DELTA_CAM_CUE_T_MAX;
	static const double DELTA_CAM_CUE_P_HIGH;

	static const double m_min;
	static const double m_max;

	static const double OVER_R_T_SPEED;
	static const double MAX_OVER_T;
	static const double MIN_UNDER_R;
	static const double UNDER_DR_DP;
	static const double UNDER_DR_DT;

	static const double UNDER_DT;
	static const double UNDER_DP;

	static const double MAX_T;

	static const double SNAP_THRESHOLD;
	static const double DOUBLE_SNAP_THRESHOLD;

	static const double MIN_VIEW_R;

	static const double INTERPOLATE_R_RATIO;

	static const ps::ExtMath::vector TOP_LEFT;
	static const ps::ExtMath::vector TOP_RIGHT;
	static const ps::ExtMath::vector BOT_LEFT;
	static const ps::ExtMath::vector BOT_RIGHT;
};
#endif // !CameraPosition_h_

