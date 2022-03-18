#ifndef __EXT_MATH__
#define __EXT_MATH__
#include <vector>

#define POS_INFINITY DBL_MAX
#define NEG_INFINITY -DBL_MAX


namespace ps {
    namespace ExtMath {
        class vector;
        class plane;
        class quaternion;
        class segment;

        extern const double PI;
        extern const double TWO_PI;
		extern const double PI_P2;
		extern const double PI_3P2;

        bool isZeroNumber(double num);

        bool isLessThanZeroNumber(double num);

        bool isGreatThanZeroNumber(double num);

        double acos(double value);

        double asin(double value);

        double cos(double value);

        double sin(double value);

		double tan(const double & value);

		double atan(const double & value);

		double cot(double value);

		double interpolate(double min, double max, double current, double minInter, double maxInter);

        double minifyAngle(double value);

		template <typename T>
		T clamp(T number, T floor, T ceil)
		{
			T min = floor;
			T max = ceil;
			if (floor > ceil) 
			{
				min = ceil;
				max = floor;
			}

			if (number < min) return min;
			if (number > max) return max;
			return number;
		}

        double simplifyRadian(double value);

        class vector {
        public:
            vector();
            vector(double x, double y);
            vector(double x, double y, double z);
            vector(const vector & v);

            double x;
            double y;
            double z;

            vector & operator=(const vector & v);

            void reverse();
            void fixZeroVectorPrecision();
            void rotate2D(double angle);
            void applyQuaternion(const quaternion & q);
            void assignZero();

            // Immutable operators
            static bool equals(const vector & u, const vector & v);

            static bool equalsZero(const vector & u);

            static double dot(const vector & u, const vector & v);

            static vector cross(const vector & u, const vector & v);

            static double crossZ(const vector & u, const vector & v);

			static vector crossVectorWithZVec(const vector & u, double z);

			static vector crossZVecWithVector(double z, const vector & v);

            static vector add(const vector & u, const vector & v);

            static vector add(const vector & u, const vector & v, const vector & w);

            static vector sub(const vector & u, const vector & v);

            static vector multiply(double n, const vector & u);

            static vector reverse(const vector & u);

            // Project vector u on vector v
            static vector project(const vector & u, const vector & v);

            static double length(const vector & v);

            static double lengthSquare(const vector & v);

            static vector unit(const vector & v);

            static vector fromSegment(const segment & seg);

            static vector reflect(const vector & dir, const vector & nor);

            static double distance(const vector & u, const vector & v);

            static double distanceSquare(const vector & u, const vector & v);

            static vector applyQuaternion(const vector & v, const quaternion & q);

            static vector mix(const vector & u, const vector & v, double range);

            static vector normalize(const vector & v);

			static double angle2D(const vector &v1, const vector &v2);

			static vector rotate2D(const vector &v, double angle);

			static vector middle(const vector &v1, const vector &v2);

			static void clamp(vector &v, const vector &u1, const vector &u2);

			static const vector ZERO;

			static bool checkVectorsParallel(const vector & u, const vector & v);

			// return a point which give (A,p1) same direction with (p2, p1) and Length(A, p1) = length
			static vector getFixDistancePoint(const vector & p1, const vector & p2, double length);
        };
		
		struct Rpt {
			double r;
			double p;
			double t;
			double lr;

			Rpt();
			Rpt(double r, double p, double t);
            Rpt & operator=(const Rpt & rpt);
		};

        class plane {
        public:
            plane(const vector & p, const vector & n);

            vector p;
            vector n;

            // Immutable operators
            static vector projectPoint(const vector & q, const plane & pl);

            static vector projectVector(const vector & v, const plane & pl);
        };

        class quaternion {
        public:
            quaternion();
            quaternion(double w, double x, double y, double z);
            quaternion(const vector & v); // From Vector

            double w;
            double x;
            double y;
            double z;

            void reverse();

            // Immutable operators
            static quaternion multiply(const quaternion & p, const quaternion & q);

            static quaternion reverse(const quaternion & q);

            static bool equals(const quaternion & p, const quaternion & q);

			quaternion & operator=(const quaternion & q);

			static quaternion quatFromVec(const vector & v);
        };

        class segment {
        public:
			segment() {};
            segment(const vector & s, const vector & e);

            vector s;
            vector e;

            // Immutable operators
            static bool projectPoint(const vector & p, const segment & seg, vector & output);

            static vector projectPointOnLine(const vector & p, const segment & seg);

            static double distance(const vector & p, const segment & seg);

            static double distanceSquare(const vector & p, const segment & seg);

            static bool isCoPlanar(const segment & s, const segment & t);

            static bool differentSide(const segment & seg, const vector & p1, const vector & p2);

            static bool intersect(const segment & s, const segment & t);
        };

        class Shape {
        public:
            virtual bool checkPointInside(const vector & p) = 0;
        };

        class AABB : public Shape {
        public:
            double minX = 0;
            double minY = 0;
            double maxX = 0;
            double maxY = 0;

			AABB() {};
			AABB(double minX, double maxX, double minY, double maxY);

            bool checkPointInside(const vector & p) override;
        };

        class Polygon2D : public Shape {
        public:
            Polygon2D();
            bool checkPointInside(const vector & p) override;
            std::vector<vector> & polygon();

        private:
            static bool ccw(const vector & a, const vector & b, const vector & c);

            static bool intersection(const vector & a, const vector & b, const vector & c, const vector & d);
        private:
            std::vector<vector> points;
            static const vector OUTSIDE_ORIGIN;
        };

        std::vector<double> resolveQuarticEquation(double a, double b, double c, double d, double e);
        double eq_resolveSimpleCubicEquation(double a, double b, double c, double d);

		class Quadratic {
		public:
			Quadratic();
			Quadratic(double a, double b, double c);

			double step(float dt);
			static double calculate(double t, double a, double b, double c);
			void reset(double a, double b, double c);
			static Quadratic smoothTransition(double current, double target, double time);

		public:
			double _a;
			double _b; 
			double _c;
			double _t;
		};

		class VectorQuadratic {
		public:
			VectorQuadratic();
			VectorQuadratic(vector a, vector b, vector c);

			static vector calculate(double t, vector a, vector b, vector c);
			static VectorQuadratic smoothTransition(vector current, vector target, double time);

		public:
			vector _a;
			vector _b;
			vector _c;
			double _t;
		};

		class Trigonometric {
		public:
			Trigonometric();
			Trigonometric(double a, double w, double p, double c);

			double step(float dt);

			void reset();

			static double calculate(double t, double a, double w, double p, double c);
			
			static Trigonometric smoothTransition(double current, double target, double time);

		public:
			double _a;
			double _w;
			double _p;
			double _c;
			double _t;
		};

		class VectorTrigonometric {
		public:
			VectorTrigonometric();
			VectorTrigonometric(vector a, double w, double p, vector c);

			vector step(float dt);

			static vector calculate(double t, vector a, double w, double p, vector c);

			static VectorTrigonometric smoothTransition(vector current, vector target, double time);

		public:
			vector _a;
			vector _c;
			double _w;
			double _p;
			double _t;
		};

		class RPTTrigonometric {
		public:
			RPTTrigonometric();
			RPTTrigonometric(Rpt a, double w, double p, Rpt c);

			Rpt step(float dt);

			static Rpt calculate(double t, Rpt a, double w, double p, Rpt c);
			static RPTTrigonometric smoothTransition(Rpt current, Rpt target, double time);
		
		public:
				Rpt _a;
				double _w;
				double _p;
				Rpt _c;
				double _t;
		};
    };

	class EquationResults
	{
	private:
		static const int MAX_RESULT = 4;

	public:
		double arr[MAX_RESULT];
		int size;

	public:
		EquationResults();

		void add(double result);

		static EquationResults empty();
	};

	class Equation
	{
	public:
		Equation();
		~Equation();

		static EquationResults resolveQuarticEquation(double a, double b, double c, double d, double e);
		
		static EquationResults applyNRMethodQuarticAll(EquationResults resuls, double a, double b, double c, double d, double e);

		static double resolveSimpleCubicEquation(double a, double b, double c, double d);

		static EquationResults resolveQuadraticEquation(double a, double b, double c);
	};
}
#endif
