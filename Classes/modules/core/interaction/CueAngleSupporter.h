#ifndef CueAngleSupporter_h_
#define CueAngleSupporter_h_

class CueAngleSupporter
{
public:
	CueAngleSupporter();
	~CueAngleSupporter();

	void setTheta(double theta);

	void setOffsetV(double offsetV);

	void setMinValues(double minTheta, double minOffsetV);

	void reset();

	const double & getFinalTheta();

	const double &getFinalOffsetV();

private:
	double _theta;
	double _minTheta;
	double _finalTheta;

	double _offsetV;
	double _minOffsetV;
	double _finalOffsetV;
};
#endif