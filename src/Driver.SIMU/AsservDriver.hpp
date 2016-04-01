#ifndef SIMU_ASSERVDRIVER_HPP_
#define SIMU_ASSERVDRIVER_HPP_

#include <unistd.h>
#include <thread>

#include "../Common/Asserv.Driver/AAsservDriver.hpp"
#include "../Common/Utils/Chronometer.hpp"
#include "../Log/LoggerFactory.hpp"
#include "../Thread/Mutex.hpp"

using namespace std;

class AsservDriver: public AAsservDriver, utils::Mutex
{
private:

	/*!
	 * \brief Retourne le \ref Logger associé à la classe \ref AsservDriver(SIMU).
	 */
	static inline const logs::Logger & logger()
	{
		static const logs::Logger & instance = logs::LoggerFactory::logger("AsservDriver.SIMU");
		return instance;
	}

	Mutex mutexL_;

	Mutex mutexR_;

	utils::Chronometer chrono_;

	float tLeft_ms_;
	float tRight_ms_;

	long rightCounter_;
	long leftCounter_;

	long currentRightCounter_;
	long currentLeftCounter_;

	//Go to Project -> Properties -> C/C++ General -> Path and Symbols -> Tab [Symbols].
	//Add the symbol : __cplusplus with the value 201103L

	std::thread twLeft_;
	std::thread twRight_;

protected:

public:
	float leftSpeed_; //m/s
	float rightSpeed_;

	float convertPowerToSpeed(int power);

	void computeCounterL();
	void computeCounterR();

	virtual void setMotorLeftPosition(int power, long internal_ticks);
	virtual void setMotorRightPosition(int power, long internal_ticks);

	virtual void setMotorLeftPower(int power, int time);
	virtual void setMotorRightPower(int power, int time);

	virtual long getLeftExternalEncoder();
	virtual long getRightExternalEncoder();

	virtual long getLeftInternalEncoder();
	virtual long getRightInternalEncoder();

	virtual void resetEncoder();

	virtual void stopMotorLeft();
	virtual void stopMotorRight();

	virtual int getMotorLeftCurrent();
	virtual int getMotorRightCurrent();

	virtual void enableHardRegulation(bool enable);

	/*!
	 * \brief Constructor.
	 */
	AsservDriver();

	/*!
	 * \brief Destructor.
	 */
	virtual ~AsservDriver();

};

/*
 // http://stackoverflow.com/questions/10673585/start-thread-with-member-function
 Wrapper *w = new Wrapper();
 //std::thread tw1 = w->member1Thread();
 std::thread tw2 = w->member2Thread("hello", 100);
 //tw1.join();
 tw2.join();
 */
class AsservDriverWrapper
{
public:
	AsservDriverWrapper(AsservDriver * asserv)
	{
		asserv_ = asserv;
	}
	~AsservDriverWrapper()
	{
	}

	AsservDriver * asserv_;

	void member1left(const char *arg1, int time_ms)
	{
		/*
		 std::cout << "i am member1 and my first arg is ("
		 << arg1
		 << ") and second arg is ("
		 << timems
		 << ")"
		 << std::endl;*/

		//TODO asserv_->computeCounterL(); ??? à la place de le faire en high level ?
		//usleep(timems * 1000);
		utils::Chronometer chrono_member1left;
		chrono_member1left.start();
		while (chrono_member1left.getElapsedTimeInMilliSec() < time_ms)
		{
			asserv_->computeCounterL();
			usleep(1000);
		}

		asserv_->stopMotorLeft();
	}
	void member2right(const char *arg1, int time_ms)
	{
		/*
		 std::cout << "i am member2 and my first arg is ("
		 << arg1
		 << ") and second arg is ("
		 << timems
		 << ")"
		 << std::endl;*/
		usleep(time_ms * 1000);
		asserv_->stopMotorRight();
	}

	void positionLeft(const char *arg1, long internal_ticksToDo)
	{
		//stop when internal ticks is achieved..
		while (asserv_->getLeftInternalEncoder() < internal_ticksToDo)
		{
			asserv_->computeCounterL();
			usleep(1000);
		}
		asserv_->stopMotorLeft();
	}
	void positionRight(const char *arg1, long internal_ticks)
	{
		//stop when internal ticks is achieved..
		while (asserv_->getRightInternalEncoder() < internal_ticks)
		{
			asserv_->computeCounterR();
		}
		asserv_->stopMotorRight();
	}

	std::thread memberLeftThread(const char *arg1, int timems)
	{
		return std::thread([=]
		{	this->member1left(arg1, timems);});
	}

	std::thread memberRightThread(const char *arg1, int timems)
	{
		return std::thread([=]
		{	this->member2right(arg1, timems);});
	}

	std::thread positionLeftThread(const char *arg1, int internal_ticks)
	{
		return std::thread([=]
		{	this->positionLeft(arg1, internal_ticks);});
	}

	std::thread positionRightThread(const char *arg1, int internal_ticks)
	{
		return std::thread([=]
		{	this->positionRight(arg1, internal_ticks);});
	}
};

#endif
