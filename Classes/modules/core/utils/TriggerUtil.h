#ifndef TRIGGERUTIL_H_
#define TRIGGERUTIL_H_

#include <functional>
#include <vector>
#include <cocos2d.h>
#include <modules/core/ps/internal/ExtensionMath.hpp>

namespace tu {

	class TransitionData {
	public:
		TransitionData() {};

	public:
		ps::ExtMath::RPTTrigonometric rptTrans;
		ps::ExtMath::VectorTrigonometric oTrans;
	};

	class TimeTrigger {
	public:
		typedef std::function<void(float)> TimeTriggerCallBack;

		TimeTrigger(const TimeTriggerCallBack &callback, double period = 0);
		~TimeTrigger();

		void setEnabled(bool enabled);

		void update(float dt);

	private:
		TimeTriggerCallBack _callback;
		double _time;
		double _period;
		bool _enabled;

	public:
		static TimeTrigger *makeTrigger(const TimeTriggerCallBack &callback, double period);
	};

	class LimitedTimeTrigger {
	public:
		typedef std::function<void(float, TransitionData*)> LimitedTimeTriggerrCallBack;
		typedef std::function<void()> LimitedTimeTriggerrCompleteCallBack;

		LimitedTimeTrigger(LimitedTimeTriggerrCallBack callback, double period = 0);
		~LimitedTimeTrigger();

		void enable(double time);

		void disable();

		void update(float dt);

		void onceCompleted(const LimitedTimeTriggerrCompleteCallBack & callback);

		TransitionData *data();

		bool isEnable();

	private:
		LimitedTimeTriggerrCallBack _callback;
		double _time;
		double _period;
		bool _enabled;
		double _remainTime;
		std::vector<LimitedTimeTriggerrCompleteCallBack> _completeHandlers;
		
		TransitionData _data;

	public:
		static LimitedTimeTrigger *makeTrigger(LimitedTimeTriggerrCallBack callback, double period = 0);
	};

	class TimeoutTrigger {
	public:
	
		typedef std::function<void()> TimeoutTriggerCallBack;

		TimeoutTrigger(TimeoutTriggerCallBack callback);
		~TimeoutTrigger();

		void enable(double timeout);

		void disable();

		void update(float dt);

	private:
		TimeoutTriggerCallBack _callback;
		bool _enabled;
		double _remainTime;

	public:
		static TimeoutTrigger* makeTrigger(const TimeoutTriggerCallBack &callback);
	};

	typedef std::function<void()> TriggerCallBack;

	struct Trigger {
		bool run;
		TriggerCallBack callback;

		Trigger(const TriggerCallBack & callback);
	};

	class TriggerUtil
	{
	public:
		TriggerUtil();
		~TriggerUtil();

		Trigger registerOnce(const TriggerCallBack &callback);
		void trigger(Trigger trig);
	};

	template<class T>
	class Dispatcher 
	{
	public:
		typedef std::function<void(T)> DispatcherCallBack;

		void subscribe(const DispatcherCallBack & callback);
	
		void unsubscribe(const DispatcherCallBack & callback);

		void dispatch(T data);

	private:
		std::vector<DispatcherCallBack> _subscribers;
	};

	class PositionDispacher
	{
	public:
		typedef std::function<void(cocos2d::Vec3)> PosDispatcherCallBack;

		void subscribe(const PosDispatcherCallBack & callback);

		void unsubscribe(const PosDispatcherCallBack & callback);

		void dispatch(cocos2d::Vec3 data);

	private:
		std::vector<PosDispatcherCallBack> _subscribers;
	};
};
#endif // !TRIGGERUTIL_H_


