#ifndef _DataStruct_h_
#define _DataStruct_h_

#include <cocos2d.h>
#include<functional>
#include <modules/core/ps/internal/ExtensionMath.hpp>
#include <string>


struct CameraProp {
	double fov;
	double ratio;
	double nearP;
	double farP;
};

struct CameraLookAtData {
	cocos2d::Vec3 position;
	cocos2d::Vec3 up;
};

struct CameraSnapShot {
	cocos2d::Vec3 position;
	CameraLookAtData lookAt;
};

template<typename T>
size_t getAddress(std::function<T()> f) {
	typedef T(fnType)();
	fnType** fnPointer = f.template target<fnType*>();
	return (size_t)*fnPointer;
}

// get address of a function 
template<typename T, typename... U>
size_t getAddress2(std::function<T(U...)> f) {
	typedef T(fnType)(U...);
	fnType ** fnPointer = f.template target<fnType*>();
	return (size_t)*fnPointer;
}

struct delete_ptr {//Helper function to ease cleanup of container
	template <typename P>
	void operator()(P p) {
		delete p;
	}
};

template <typename P>
void deleteVector(std::vector<P> v) {
	std::for_each(v.begin(), v.end(), delete_ptr());
	v.clear();
	v.shrink_to_fit();
}

struct MotionData {
	ps::ExtMath::vector position;
	ps::ExtMath::vector velocity;
};

struct BallIndicatorData {
	cocos2d::Vec2 offset;
	cocos2d::Vec2 beforePos;
};

struct CamCoord {
	ps::ExtMath::vector position;
	ps::ExtMath::vector lookAt;

	CamCoord() {};
	CamCoord(ps::ExtMath::vector position, ps::ExtMath::vector lookAt);
};

struct RectData {
	ps::ExtMath::vector topLeft;
	ps::ExtMath::vector topRight;
	ps::ExtMath::vector botLeft;
	ps::ExtMath::vector botRight;

	RectData() {};
	RectData(ps::ExtMath::vector topLeft, ps::ExtMath::vector topRight, ps::ExtMath::vector botLeft, ps::ExtMath::vector botRight);
};

struct ElipseData {
	double a;
	double b;

	double a2;
	double b2;
	double bpa;
	double a2b2;

	ElipseData() {};
	ElipseData(double a, double b) { this->a = a; this->b = b; };
	void updateValues();
};

struct ElipseBoundingData{
	ps::ExtMath::vector top;
	ps::ExtMath::vector bot;
	ps::ExtMath::vector left;
	ps::ExtMath::vector right;

	ElipseBoundingData() {};
	ElipseBoundingData(ps::ExtMath::vector top, ps::ExtMath::vector bot,
		ps::ExtMath::vector left, ps::ExtMath::vector right);
};

struct ElipsePhiData {
	double A1;
	double A2;
	double B1;
	double B2;
	double C1;
	double C2;
	double D1;
	double D2;
};

struct TrajectoryReflectData {
	ps::ExtMath::segment seg1;
	ps::ExtMath::segment seg2;
	int ballId;

	TrajectoryReflectData(ps::ExtMath::vector s1, ps::ExtMath::vector e1,
		ps::ExtMath::vector s2, ps::ExtMath::vector e2, int ballId);
	TrajectoryReflectData(ps::ExtMath::vector s1, ps::ExtMath::vector e1);
	TrajectoryReflectData() {};
};

enum BallType {
	NOT_DETERMINED,
	SOLID,
	STROKE
};

const int NUM_BALL_TYPE = 3;

struct PocketBallData {
	int number;
	int pocketId;
	bool foul;
	int player;

	PocketBallData() {};
	PocketBallData(int number, int pocketId) { this->number = number; this->pocketId = pocketId; };
};

struct BallPocketResult {
	bool hasBallTypePocketed;
	bool isCueBallFell;
	bool isEightBallFell;

	BallPocketResult(bool hasBallTypePocketed, bool isCueBallFell, bool isEightBallFell)
	{
		this->hasBallTypePocketed = hasBallTypePocketed;
		this->isCueBallFell = isCueBallFell;
		this->isEightBallFell = isEightBallFell;
	};

};

struct EnableBallData
{
	int ballId;
	bool isDisable;
	bool isPocketed;
	ps::ExtMath::vector pos;
};

struct CueChallengeData
{
	int pocketId;
	int cueType;
};

struct CardRewardData
{
	int type;
	int quantity;
};

struct CardData
{
	CardRewardData reward;
	bool isOpened;
	int userId;
	int openTime;
};

struct WinData 
{
	int pId;
	std::string message;

	WinData(int id, std::string message = "") { this->pId = id; this->message = message; };
};

struct ShootData 
{
	ps::ExtMath::vector direction;
	double offsetX;
	double offsetY;
	double force;

	ShootData() 
	{
		direction = ps::ExtMath::vector::ZERO;
		offsetX = 0;
		offsetY = 0;
		force = 0;
	};
};

struct ShootResultData 
{
	int userId;
	int phrase;
	int nextTurnPlayerId;
	bool isPlacingCueBall;
	std::vector<PocketBallData> ballsPocketed;
	std::vector<EnableBallData> ballsEnable;

	void setData(int userId, int phrase, int nextTurnPlayerId, bool isPlacingCueBall, const std::vector<PocketBallData> & ballsPocketed, const std::vector<EnableBallData> & ballsEnable);
};

struct ChangeCueAngleData {
	float lastY;

	ChangeCueAngleData() :lastY(0.f) {};
};

struct ModelPathData
{
	std::string model;
	std::string tex;
	std::string mrao;
};

struct ItemInfoData
{
	int type;
	long long num;

	ItemInfoData() {};

	ItemInfoData(int type, long long num) 
	{
		this->type = type;
		this->num = num;
	};
};

class CueInfoData
{
public:
	int id;
	int type;
	int star;
	int maxStar;
	int level;
	int battery;
	int configId;
	int aim;
	double accuracy;
	int point;
	int wonChallenges;
	bool autoCharge;
	std::string name;
	bool owned;

	double force;
	double tip_u;
	double angle[2];
	double angular;
	int tier;
	int charge_fee;

	CueInfoData();
	~CueInfoData();
};

struct CueBoxData 
{
	int type;
	int time;
};

struct MatData
{
	int id;
	int quantity;
};

struct CueRate 
{
	int cueId;
	int rate;
};

struct ChallengeData
{
	int id;
	int groupIdx;
};

struct CuePart {
	int id;
	int rank;
	int pos;
	int type;
	int equippedCueId;
	bool disabled;
};

#endif // !_DataStruct_h_
