#ifndef SIMU_ASSERVDRIVER_HPP_
#define SIMU_ASSERVDRIVER_HPP_

#include "thread"
#include <unistd.h>
#include <iostream>

#include "../Common/Asserv.Driver/AAsservDriver.hpp"
#include "../Common/Utils/Chronometer.hpp"
#include "../Log/LoggerFactory.hpp"

using namespace std;

class AsservDriver: public AAsservDriver
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

	utils::Chronometer chrono_;

	double tLeft_ms_;
	double tRight_ms_;

	long rightCounter_;
	long leftCounter_;

	long currentRightCounter_;
	long currentLeftCounter_;

	std::thread twLeft_;
	std::thread twRight_;

protected:

public:
	int leftPower_;
	int rightPower_;

	void computeCounterL();
	void computeCounterR();

	virtual void setMotorLeftPosition(long ticks, int power);
	virtual void setMotorRightPosition(long ticks, int power);

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

	void member1left(const char *arg1, int timems)
	{
		/*
		std::cout << "i am member1 and my first arg is ("
				<< arg1
				<< ") and second arg is ("
				<< timems
				<< ")"
				<< std::endl;*/
		usleep(timems * 1000);
		asserv_->stopMotorLeft();
	}
	void member2right(const char *arg1, int timems)
	{
		/*
		std::cout << "i am member2 and my first arg is ("
				<< arg1
				<< ") and second arg is ("
				<< timems
				<< ")"
				<< std::endl;*/
		usleep(timems * 1000);
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
};

#endif
