#ifndef SIMU_ASSERVDRIVER_HPP_
#define SIMU_ASSERVDRIVER_HPP_

#include <unistd.h>
#include <thread>

#include "../Common/Asserv.Driver/AAsservDriver.hpp"
#include "../Common/Utils/Chronometer.hpp"
#include "../Log/LoggerFactory.hpp"

//using namespace std;

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

	//Go to Project -> Properties -> C/C++ General -> Path and Symbols -> Tab [Symbols]. Add the symbol : __cplusplus with the value 201103L

	std::thread twLeft_;
	std::thread twRight_;

protected:

public:
	int leftPower_;
	int rightPower_;

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

	void positionLeft(const char *arg1, long internal_ticks)
	{
		//stop when internal ticks is achieved..
		while (asserv_->getLeftInternalEncoder() < internal_ticks)
		{
			usleep(500);
		}
		asserv_->stopMotorLeft();
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
};

#endif
