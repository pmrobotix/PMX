#ifndef COMMON_ASSERVINSA_ASSERVINSA_HPP_
#define COMMON_ASSERVINSA_ASSERVINSA_HPP_

//#define DEBUG_MOTION 1

#include <pthread.h>
#include <semaphore.h>
#include <sys/types.h>

#include "../Common/Asserv/Asserv.hpp"
#include "../Log/LoggerFactory.hpp"
#include "../Thread/Thread.hpp"
#include "../Common/Position.hpp"

struct Position;
class Robot;
struct RobotPosition;

class MovingBase;

//dupliquer de math.h car inexistant dans le compilo EV3
# define M_E		2.7182818284590452354	/* e */
# define M_LOG2E	1.4426950408889634074	/* log_2 e */
# define M_LOG10E	0.43429448190325182765	/* log_10 e */
# define M_LN2		0.69314718055994530942	/* log_e 2 */
# define M_LN10		2.30258509299404568402	/* log_e 10 */
# define M_PI		3.14159265358979323846	/* pi */
# define M_PI_2		1.57079632679489661923	/* pi/2 */
# define M_PI_4		0.78539816339744830962	/* pi/4 */
# define M_1_PI		0.31830988618379067154	/* 1/pi */
# define M_2_PI		0.63661977236758134308	/* 2/pi */
# define M_2_SQRTPI	1.12837916709551257390	/* 2/sqrt(pi) */
# define M_SQRT2	1.41421356237309504880	/* sqrt(2) */
# define M_SQRT1_2	0.70710678118654752440	/* 1/sqrt(2) */

# define M_2PI (2*M_PI)

//#define DEBUG_MOTION
//#define DEBUG_ENCODER
//#define DEBUG_PID
//#define LOG_PID
//#define LOG_SVG_APPENDER //log c++ pour activer le SVG
//#define LOG_PID_APPENDER  //log c++ pour activer le fileAppender

#define uint8 unsigned char
#define uint16 unsigned short
#define uint32 unsigned int
#define int8 char
#define int16 short
#define int32 int
#define BOOL bool

#define FALSE false
#define TRUE true

#define slippageThreshold 10

#define MAX_PID_SYSTEM_NUMBER	6

//every N period, we compute precise values for
//cos and sin of the robot angle
#define PRECISE_COMPUTATION_NTH_PERIOD		64

#define MAX_PERIOD		128

//LEFT - RIGHT
#define LEFT_MOTOR		0
#define RIGHT_MOTOR		1

//ALPHA - DELTA (virtual motors)
#define ALPHA_MOTOR		0
#define DELTA_MOTOR		1

//Two motors per set
#define MOTOR_PER_TYPE	2

//Number of period used to compute motor speed
#define MOTOR_SPEED_PERIOD_NB	9

//motion_priv.hpp
//! Macro that correct integer overflow for a difference of two value
//! typically when you calculate an error using error = new-old, and new has overflowed
//! the macro substract or add the maximum value if needed to compensate the overflow of new
#define CORRECT_DELTA_OVERFLOW(value, max_value) {if((value)>(max_value/2))value-=max_value; else if((value)<(-(max_value/2)))value+=max_value;}
//! Macro that bound an integer to a maximum absolute value
//! for instance :
//! BOUND_INT(i, 100) with i==143 set i=100
//! BOUND_INT(i, 100) with i==-160 set i=-100
//! BOUND_INT(i, 100) with i==-16 set i=-16
#define BOUND_INT(value, max_abs) {if(value>max_abs)value=max_abs; else if(value<-max_abs)value=-max_abs;}

//PID system are identified by a number
typedef uint8 PID_SYSTEM;
/*
typedef enum
{
	TRAJECTORY_RUNNING,
	ASSISTED_HANDLING,
	FREE_MOTION,
	DISABLE_PID,
} MOTION_STATE;*/

/*
//! possible return state of a trajectory
typedef enum
{
	TRAJ_OK,				//trajectory successfully completed
	TRAJ_ERROR,				//unknown error (not implemented !)
	TRAJ_COLLISION,			//trajectory interrupted because of a collision
	TRAJ_NEAR_OBSTACLE,		//trajectory interrupted because of a near collision
	TRAJ_CANCELLED,			//trajectory cancelled by remote user (for debug only)
	TRAJ_INTERRUPTED,		//trajectory interrupted by software
	TRAJ_COLLISION_REAR
} TRAJ_STATE;
*/
//different kind of command generation
typedef enum
{
	POSITION_COMMAND,
	SPEED_COMMAND,
	STEP_COMMAND,
} MOTOR_COMMAND_TYPE;

//different kind of command interpretation (virtual motors)
typedef enum
{
	LEFT_RIGHT,
	ALPHA_DELTA,
	MAX_MOTION_CONTROL_TYPE_NUMBER,
} MOTION_CONTROL_TYPE;

//structure used to stock configuration values of the PID system
typedef struct
{
	float kP;
	float kI;
	float kD;
} pidConfig;

typedef struct
{
	pidConfig conf;					//pid system configuration
	unsigned long lastTime;
	float lastInput;
	float lastOutput;
	float ITerm;
} pidSystemValues;

//possible phase of a position trajectory :
typedef enum
{
	TR_PRE_PHASE,			//trajectory not yet started
	TR_ACCEL_PHASE,			//0 - acceleration phase to reach goal speed
	TR_CONSTANT_PHASE,		//1 - motor has reached final speed
	TR_DECEL_PHASE,		//2 - deceleration phase before reaching goal position
	TR_END_PHASE,			//3 - motor has reached desired position
} TRAJ_PHASE;

//precomputed values defining the complete trajectory
typedef struct
{
	TRAJ_PHASE phase;		//current phase

	int32 period0;     		//period number of the start of trajectory
	int32 order0;			//motor position at trajectory start

	int32 VMax;				//goal speed in vTops/sample
	int32 order3;			//goal position

	int32 T01;				//number of sample between accel and constant phase
	int32 T12;				//number of sample between contant and decel phase
	int32 T23;				//number of sample between decel and end phase

	int32 Acceleration;	//acceleration used in movement in vTops/sample/sample
	int32 Deceleration;	//deceleration used in movement in vTops/sample/sample

	int32 lastOrder;		//current ordered position of motor
} PositionCommand;

//possible phase of a speed trajectory :
typedef enum
{
	SP_PRE_PHASE,			//trajectory not yet started
	SP_ACCEL_PHASE,	//acceleration (or deceleration) phase to reach goal speed
	SP_CONSTANT_PHASE,		//motor has reached final speed
	SP_END_PHASE,			//trajectory ended
} SPEED_PHASE;

//precomputed values defining the complete trajectory
typedef struct
{
	SPEED_PHASE phase;		//current phase

	int32 order0;			//motor position at trajectory start

	int32 period0;			//nb of period at the start of trajectory
	int32 period1;			//nb of period at end of acceleration (deceleration)
	int32 period2;			//nb of period at end of trajectory

	int32 Acceleration;		//acceleration used in movement in vTops/sample/sample

	int32 V0;				//start speed in vTops/sample
	int32 VGoal;			//goal speed in vTops/sample

	int32 lastOrder;		//current ordered position of motor

} SpeedCommand;

typedef int32 StepCommand;

//! structure used internally for stocking current value for each motors
typedef struct
{
	PID_SYSTEM PIDSys;

	int32 lastPos;

	int posIndex;

	int32 prevPos[MOTOR_SPEED_PERIOD_NB];		//previous value of position for speed computation
} MOTOR;

typedef enum
{
	WP_NOT_SET,				//no waypoint set for the moment

	WP_POS_ALPHA_SUPERIOR,	//waypoint triggered when alpha value is above threshold value
	WP_POS_ALPHA_INFERIOR,	//waypoint triggered when alpha value is below threshold value
	WP_POS_DELTA_SUPERIOR,	//waypoint triggered when delta value is above threshold value
	WP_POS_DELTA_INFERIOR,	//waypoint triggered when delta value is below threshold value

	WP_END_OF_TRAJ,			//waypoint when both motor command are finished
} WAYPOINT_TYPE;

//! structure used to represent trajectory waypoint
typedef struct
{
	WAYPOINT_TYPE type;int32 threshold;
} WAYPOINT;

//! structure used internally to define a robot trajectory command
typedef struct
{
	MOTOR_COMMAND_TYPE cmdType;
	MOTION_CONTROL_TYPE mcType;

	//anonymous union that stock data structure for every possible type of motor command
	//structures share the same memory (which you can interpret as needed)
	union
	{
		PositionCommand posCmd[MOTOR_PER_TYPE];
		SpeedCommand speedCmd[MOTOR_PER_TYPE];
		StepCommand stepCmd[MOTOR_PER_TYPE];
	} cmd;

} RobotCommand;

class AsservInsa: public utils::Thread
{
public:

protected:
	virtual void execute();

private:

	/*!
	 * \brief Retourne le \ref Logger associé à la classe \ref AsservInsa.
	 */
	static inline const logs::Logger & logger()
	{
		static const logs::Logger & instance = logs::LoggerFactory::logger("AsservInsa");
		return instance;
	}

	/*!
	 * \brief Retourne le \ref Logger svg associé à la classe \ref AsservInsa.
	 */
//	static inline const logs::Logger & loggerSvg()
//	{
//		static const logs::Logger & instance = logs::LoggerFactory::logger("Svg4AsservInsa");
//		return instance;
//	}
	/*!
	 * \brief Retourne le \ref Logger file associé à la classe \ref AsservInsa.
	 */
	static inline const logs::Logger & loggerFile()
	{
		static const logs::Logger & instance = logs::LoggerFactory::logger("logFileAsservInsa");
		return instance;
	}

	int oldPwmLeft;
	int oldPwmRight;

	int stop_motion_ITTask;

	//bool activate_;

	int32 rightTicksPerM_;int32 leftTicksPerM_;

	int useExternalEncoders_;int32 lastLeft_;int32 lastRight_;

	//encoder.c
	//ratio vTops/ticks for left encoder
	int32 leftEncoderRatio;
	//ratio vTops/ticks for right encoder
	int32 rightEncoderRatio;
	//distance between both encoder wheels in vTops
	int32 distEncoder;
	//distance between both encoder wheels in meters
	float distEncoderMeter;

	float valueVTops; // [meter/vTops]

	int defaultSamplingFreq;
	//float valueSample; //[sec/sample] //remplacer par loopDelayInMillis
	long loopDelayInMillis;

	//int SampleTime; //... millisec

	int32 vtopsPerTicks; //Sample per ticks
	int maxPwmValue_;

	float defaultVmax; //m/sec
	float defaultAcc;
	float defaultDec;

	long long timeOffset;

	//nb of period since the beginning
	uint32 periodNb; //static

	//mutex protecting the currently executed command
	pthread_mutex_t mtxMotionCommand; //static
	//pthread_t thread; //static
	//the currently executed command
	RobotCommand motionCommand;	//static

	pidSystemValues systemValues[MAX_PID_SYSTEM_NUMBER];
	//number of system created so far
	PID_SYSTEM pid_Nb;

	//well, genericity isn't always optimisation-friendly...
	long stopAt; //static

	WAYPOINT nextWaypoint;
	bool waitingSemaphore; //static
	sem_t semWaypoint; //static

	TRAJ_STATE trajState; //static

	float Theta;					//rad //static
	float cosTheta, sinTheta;	//no unit //static
	float xTops, yTops;		//encoder vTops //static

	//float pos_x; //static
	//float pos_y; //static
	//float pos_theta; //static

	int32 odoPeriodNb; //static

	int32 slippage[MOTOR_PER_TYPE];				//current slippage //static
	int32 values[MOTOR_PER_TYPE][MAX_PERIOD];		//all the previous values of slippage //static
	int32 index[MOTOR_PER_TYPE];					//index in the values table //static

	RobotCommand cmd; //static

	MOTION_STATE RobotMotionState;

	//Base roulante de ce robot

	MOTOR motors[MAX_MOTION_CONTROL_TYPE_NUMBER][MOTOR_PER_TYPE];

	//MovingBase *base_;
	Robot * robot_;

//---clothoid

//! Taylor decomposition of FresnelC( sqrt(x/Pi) ) / FresnelS( sqrt(x/Pi) )
//! at x = Pi/2 and 3*Pi/2
//! the function should only be used with abs(x) > 0.5
	float FresnelCSSqrt(float x);
//! Taylor decomposition of FresnelS( sqrt(x/Pi) )
//! at x = Pi
	float FresnelSSqrt(float x);
//! Create a clothoid trajectory made of two line segments, smoothly linked by
//! an arc of clothoid
//! This is a test function
	void CreateTwoSegmentTraj(float V0,
			float distD1,
			float distD2,
			float A0,
			float beta,
			float epsilon);

//---motion
	void checkRobotCommand(RobotCommand *cmd);
	void loadCommand(RobotCommand *cmd);
	//motion control task
	void initPWM(void);
	void signalEndOfTraj(void);

//---motor_PID
	//! \brief Initialisation of the PID system
	void pid_Init(void);
	//! \brief Create a new PID system
	//! \return The new system ID
	PID_SYSTEM pid_Create(void);
	//! \brief Configure kP value of the PID system
	void pid_Config(PID_SYSTEM system, float kp, float ki, float kd);
	//! \brief Compute the PID sum of a system
	//!
	//! \param system The system on which we compute the PID
	//! \param setpoint reference value to reach
	//! \param input mesured value
	//! \return The new command to apply on the system (% pwm) between -100 and +100
	float pid_Compute(PID_SYSTEM system, float setpoint, float input, float speed);

	int32 pid_Compute_rcva_chaff(PID_SYSTEM system, int32 error, double vitesse);

	int32 pid_ComputeRcva(PID_SYSTEM system, float error, float vitesse);

//---motor_positionCommand

	//! \brief Create a trajectory for one motor with a trapezoidal speed curve
	//!
	//! The trajectory start with a speed=0 then accelerate at a rate equal to
	//! 'Accel' until it reaches speed='VMax' before the end of the path
	//! the wheel decelerate with a rate of 'Decel' then stop.
	//!
	//! \param motor Id number of motor on wich to apply the trajectory
	//! \param out_command Result of the computation
	//! \param dist Distance to run in meters
	//! \param VMax Maximum speed at the wheel in meters/second
	//! \param Accel Acceleration in meters/second^2
	//! \param Decel Deceleration in meters/second^2
	void ComputePositionCommand(PositionCommand *out_command,
	int32 dist,
	int32 VMax,
	int32 Accel,
	int32 Decel);
	//! Configure the command for the current configuration of the motor.
	//! You must execute this function before you launch the command.
	//! \param out_command Result of the computation
	//! \param motor Id number of motor on wich to apply the trajectory
	void LoadPositionCommand(PositionCommand *out_command, MOTOR *motor,
	int32 periodNb);
	//! \brief Compute the current order to apply on motor
	//!
	//! \param motor Id number of motor
	//! \param posCommand The command that will be interpreted
	//! \param Tn Motion control period number
	//! \param out_Order Result of the computation
	//! \return Is the path completed ?
	bool GetPositionOrder(PositionCommand *posCommand,
	int32 Tn,
	int32 *out_Order);

//---motor_speedCommand

	//! \brief Create a trajectory for one motor with a trapezoidal speed curve
	//!
	//! The trajectory start with a speed='V0' then accelerate with a rate of
	//! 'Accel' until it reaches speed='VGoal', then when the total ran distance
	//! is equal to distMax, the motor stops.
	//!
	//! \param out_command Result of the computation
	//! \param V0 Speed at the start of the command
	//! \param VGoal Speed to attain in vTops/sample
	//! \param distMax Maximum distance to run in vTops
	//! \param Accel Acceleration in vTops/sample^2
	void ComputeSpeedCommand(SpeedCommand *out_command,
	int32 V0,
	int32 VGoal,
	int32 distMax,
	int32 Accel);
	//! \brief Create a trajectory for one motor with a trapezoidal speed curve
	//!
	//! The trajectory start with a speed='V0' then accelerate with a rate of
	//! 'Accel' during 'Time' periods
	//!
	//! \param out_command Result of the computation
	//! \param V0 Speed at the start of the command
	//! \param Time Number of period of the acceleration
	//! \param Accel Acceleration in vTops/sample^2
	void ComputeSpeedCommandTime(SpeedCommand *out_command,
	int32 V0,
	int32 Time,
	int32 Accel);
	//! \brief Create a trajectory for one motor with a trapezoidal speed curve
	//!
	//! The trajectory start with a speed='V0' then accelerate with a rate of
	//! 'Accel' but stop after 'Time' periods
	//!
	//! \param out_command Result of the computation
	//! \param V0 Speed at the start of the command
	//! \param Time Number of period of the acceleration
	//! \param Accel Acceleration in vTops/sample^2
	void ComputeSpeedCommandMaxTime(SpeedCommand *out_command,
	int32 V0,
	int32 VGoal,
	int32 Accel,
	int32 Time);
	//! Configure the command for the current configuration of the motor.
	//! You must execute this function before you launch the command.
	//! \param out_cmd Result of the computation
	//! \param lastPos Last position of motor
	//! \param periodNb Current integration period number
	void LoadSpeedCommand(SpeedCommand *out_cmd, int32 lastPos, int32 periodNb);
	//! \brief Compute the current order to apply on motor
	//!
	//! \param motor Id number of motor
	//! \param Tn Motion control period number
	//! \param order Output of the computation
	//! \return Is the path completed ?
	bool GetSpeedOrder(SpeedCommand *speedCommand, int32 Tn,
	int32 *out_order);

//---motor_stepCommand

	//! Set a step movement for a given motor
	//! (for PID tuning purpose)
	//!
	//! \param out_command Pointer to the StepOrder structure that will contain the order
	//! \param steporder Order that will be applied on the PID system
	//! \param seconds duration of the step (in seconds)
	void ComputeStepOrder(StepCommand *out_command, int32 steporder, int seconds);
	//! Return the current motor order for a step movement
	//! (for PID tuning purpose)
	//!
	//! \param motor Id number of motor
	//! \param order StepOrder that will be interpreted
	bool GetStepOrder(StepCommand *order, int32 *out_order);

//---motor

	//! initialisation of the motor structure
	void initMotor(MOTOR *motor);
	//config iMax for all motors
	//void motors_ConfigAllIMax(int32 imax);
	//! update motor position and store value for speed computation
	void updateMotor(MOTOR *motor, int32 delta);
	//! Compute current motor speed
	int32 getMotorSpeed(MOTOR *motor);

//---path_manager

	//! Set the next waypoint
	void path_SetNextWaypoint(WAYPOINT *wp);
	//! \brief Wait for the current waypoint
	//!
	//! You must have called path_SetNextWaypoint before
	TRAJ_STATE path_WaitWaypoint(void);
	//! \brief Wait for the end of the current trajectory
	//!
	//! This is an utility function that call
	//! path_SetNextWaypoint and path_WaitWaypoint()
	TRAJ_STATE path_WaitEndOfTrajectory(void);
	//! Execute trajectory, this is an asynchronous call
	void path_LaunchTrajectory(RobotCommand *traj);
	//! Cancel the current trajectory and stop the robot
	//!

	void path_Init(void);
	void path_TriggerWaypoint(TRAJ_STATE state);

//---robot_odometry

	//! \brief Compute robot position using odometry
	//!
	//! \param dTheta displacement (rotation) of the robot in radian
	//! \param dV displacement (forward) of the robot in vTops
	void odo_Integration(float dTheta, float dV);

	//!
	//! \param x [out] Robot position on x axis in meters
	//! \param y [out] Robot position on y axis in meters
	//! \param theta [out] Robot orientation in radians ]-Pi..Pi]
	void odo_GetPositionXYTheta(float *x, float *y, float *theta);

//---robot_slippage

	//! Initialise module
	void slippage_Init(void);
	//! Update current slippage value for a motor
	//! \param motor Motor that will be updated (should be LEFT_MOTOR or RIGHT_MOTOR)
	//! \param dWheel displacement value of the wheel (motor encoder)
	//! \param dRobot displacement value of the robot (wheel encoder / mouse sensor)
	void slippage_Update(int motor, int32 dWheel, int32 dRobot);
	void resetSlippage(int motor);

//---robot_unitconversion

	// The following functions are helpers for convertion between units used
	// in the motion control program :
	//vTops is a virtual measure distance to avoid floating point computation
	//sample is a time unit corresponding to one motion control period

	//! \brief Compute the distance in vTops
	//!
	//! \param dist The distance in meter to convert
	int32 convertDistTovTops(float dist);
	//! \brief Compute the speed in vTops/sample
	//!
	//! \param speed The speed in meter/second to convert
	int32 convertSpeedTovTopsPerPeriod(float speed);
	//! \brief Compute the acceleration in vTops/sample^2
	//!
	//! \param accel The acceleration in meter/second^2 to convert
	int32 convertAccelTovTopsPerPeriodSqd(float accel);

	long currentTimeInMillis();
	long long currentTimeInMicros();

public:

	AsservInsa(Robot * robot);
	~AsservInsa();

	void motion_Init();

	void motion_SetSamplingFrequency(uint frequency);

	//! Mostly needed for remote control, use collision function
	//! when a collision is detected.
	void path_CancelTrajectory(void);
	//! Interrupt current trajectory and stop robot
	//!
	//! Used to stop on a particular event (for instance actuator event)
	void path_InterruptTrajectory(void);
	void path_CollisionOnTrajectory();
	void path_CollisionRearOnTrajectory();

	//motion

	//! \brief Set robot position used by odometry
	//!
	void odo_SetPosition(float x, float y, float theta);

	//! \brief Get current odometry robot position

	//! \brief Get current odometry robot position
	//!
	//! \return Robot position in meters and angle in radian
	RobotPosition odo_GetPosition(void);

	float pos_getX_mm();
	float pos_getY_mm();
	float pos_getTheta(); // angle in radian
	float pos_getThetaInDegree(); // angle in degrees

	void motion_setMaxPwmValue(int maxPwmValue);

	//! Load motion control module
	void motion_configureAlphaPID(float p, float i, float d);
	void motion_configureDeltaPID(float p, float i, float d);
	void motion_configureLeftPID(float p, float i, float d);
	void motion_configureRightPID(float p, float i, float d);
	//! Stop motion control for a moment
	void motion_FreeMotion(void);
	//! Set a robot command and execute it
	void motion_SetCurrentCommand(RobotCommand *cmd);
	//! Stop motion control and disable PID
	void motion_DisablePID(void);
	//! Assisted movement mode =)
	void motion_AssistedHandling(void);
	//! Stop motion control timer, used to shutdown motion control
	void motion_StopTimer(void);
	//! Directly set the pwm for the motor (used both internally and for test)
	void setPWM(int16 pwmLeft, int16 pwmRight);

	//---encoder

	uint32 encoder_GetLeftResolution();

	uint32 encoder_GetRightResolution();

	//! Initialisation
	void encoder_Init(void);
	//! Set distance between encoder
	//! \param dist Encoder distance in meters
	void encoder_SetDist(float dist);
	//! Set encoder resolution and recompute value of vTops etc...
	//! \param leftTicksPerM left encoder resolution in ticks per meter
	//! \param rightTicksPerM right encoder resolution in ticks per meter
	//! \param entraxe dist Encoder distance in millimeters
	void encoder_SetResolution(uint32 leftTicksPerM, uint32 rightTicksPerM, float entraxe_mm);
	//! Get new encoders values
	//!
	//! \param dLeft left motor motion
	//! \param dRight right motor motion
	//! \param dAlpha alpha motor motion
	//! \param dDelta delta motor motion
	void encoder_ReadSensor(int32 *dLeft,
			int32 *dRight,
			int32 *dAlpha,
			int32 *dDelta,
			int32 *left,
			int32 *right);

	//---robot_traj_wrappers

	TRAJ_STATE motion_findPidAD(float angle_rad, float meters, int sec);
	TRAJ_STATE motion_findPidLR(float posl, float posr, int sec);

	//! This module provide higher level control on the motion control module
	//! AI and strategy software can use this primitives directly
	//! Compute and launch trajectory
	//! \sa motion_Line
	TRAJ_STATE motion_DoLine(float dist);
	//! Compute and launch trajectory
	//! \sa motion_Rotate
	TRAJ_STATE motion_DoRotate(float angle);
	//! Compute and launch trajectory
	//! \sa motion_ArcRotate
	TRAJ_STATE motion_DoArcRotate(float angle, float radius);
	//! Compute the displacement needed on alpha and delta to reach the point x2, y2
	//! from the point x1, y1 starting with the orientation theta1
	void computeAlphaDelta(float x1,
			float y1,
			float theta1,
			float x2,
			float y2,
			float *out_angle,
			float *out_dist);
	//! macro that translate movement for arc circle trajectory
#define motion_DoArcLeftForward(angle, radius) motion_DoArcRotate(fabs(angle), fabs(radius))
#define motion_DoArcRightForward(angle, radius) motion_DoArcRotate(-fabs(angle), -fabs(radius))
#define motion_DoArcLeftBackward(angle, radius) motion_DoArcRotate(-fabs(angle), fabs(radius))
#define motion_DoArcRightBackward(angle, radius) motion_DoArcRotate(fabs(angle), -fabs(radius))
	//! Goto (x2,y2) from (x1,y1,theta1) using Turn-Stop-Line motion
	TRAJ_STATE motion_GoToTSLFrom(float x1, float y1, float theta1, float x2, float y2);
	//! Goto (x,y) using Turn-Stop-Line motion
	TRAJ_STATE motion_GoToTSL(float x, float y);
	//! Goto (x,y) using speed motion
	TRAJ_STATE motion_GoToSpeed(float x, float y);
	//! Goto a list of (x,y) position using speed motion
	TRAJ_STATE motion_GoToSpeedPath(const Position *pos, int nb);
	//! Orient toward the given angle
	TRAJ_STATE motion_OrientTo(float angle);

	//---robot_trajectory

	//! Order a straight line movement of the specified distance
	//! \param out_cmd Resulting command that will be interpreted by path_manager
	//! \param dist Distance to run in meter
	void motion_Line(RobotCommand *out_cmd, float dist);
	//! Order a straight line movement of the specified distance with a specific speed
	//! \param out_cmd Resulting command that will be interpreted by path_manager
	//! \param dist Distance to run in meter
	//! \param VMax Maximum goal speed of each encoder wheel in meter/second
	void motion_LineSpeed(RobotCommand *out_cmd, float dist, float VMax);
	//! Order a straight line movement of the specified distance with a specific speed and acceleration
	//! \param out_cmd Resulting command that will be interpreted by path_manager
	//! \param dist Distance to run in meter
	//! \param VMax Maximum goal speed of each encoder wheel in meter/second
	//! \param Accel Acceleration used to attain the VMax speed in m/s/s
	//! \param Decel Deceleration used to stop the movement in m/s/s
	void motion_LineSpeedAcc(RobotCommand *out_cmd,
			float dist,
			float VMax,
			float Accel,
			float Decel);
	//! Order a pure rotation movement of the specified angle
	//! \param out_cmd Resulting command that will be interpreted by path_manager
	//! \param angle Angle to run in radian
	void motion_Rotate(RobotCommand *out_cmd, float angle);
	//! Order a pure rotation movement of the specified angle with a specific speed
	//! \param out_cmd Resulting command that will be interpreted by path_manager
	//! \param angle Angle to run in radian
	//! \param VMax Maximum goal speed of each encoder wheel in meter/second
	void motion_RotateSpeed(RobotCommand *out_cmd, float angle, float VMax);
	//! Order a pure rotation movement of the specified angle with a specific speed and acceleration
	//! \param out_cmd Resulting command that will be interpreted by path_manager
	//! \param angle Angle to run in radian
	//! \param VMax Maximum goal speed of each encoder wheel in meter/second
	//! \param Accel Acceleration used to attain the VMax speed in m/s/s
	//! \param Decel Deceleration used to stop the movement in m/s/s
	void motion_RotateSpeedAcc(RobotCommand *out_cmd,
			float angle,
			float VMax,
			float Accel,
			float Decel);
	//! Order a arc-circle curve movement of the specified angle and radius
	//! \param out_cmd Resulting command that will be interpreted by path_manager
	//! \param angle Angle of the arc-circle curve in radian (2*Pi make a complete circle)
	//! \param radius Radius of the arc-circle in meter
	void motion_ArcRotate(RobotCommand *out_cmd, float angle, float radius);
	void motion_ArcRotateSpeed(RobotCommand *out_cmd, float angle, float radius, float VMax);
	void motion_ArcRotateSpeedAcc(RobotCommand *out_cmd,
			float angle,
			float radius,
			float VMax,
			float Accel,
			float Decel);
	//! Control the speed of both motors
	//! \param out_cmd Resulting command that will be interpreted by path_manager
	void motion_SpeedControlLR(RobotCommand *out_cmd,
			float spLeft,
			float distLeft,
			float accLeft,
			float spRight,
			float distRight,
			float accRight);
	void motion_SpeedControlLRDecel(RobotCommand *out_cmd,
			float spLeft,
			float distLeft,
			float accLeft,
			float decLeft,
			float spRight,
			float distRight,
			float accRight,
			float decRight);
	void motion_SpeedControlLRV0(RobotCommand *out_cmd,
			float V0Left,
			float spLeft,
			float distLeft,
			float accLeft,
			float V0Right,
			float spRight,
			float distRight,
			float accRight);
	void motion_SpeedControlLRTime(RobotCommand *out_cmd,
			float V0Left,
			float TLeft,
			float accLeft,
			float V0Right,
			float TRight,
			float accRight);
	//! Control the speed of alpha-delta virtual motors
	//! \param out_cmd Resulting command that will be interpreted by path_manager
	void motion_SpeedControlAD(RobotCommand *out_cmd,
			float spAlpha,
			float distAlpha,
			float accAlpha,
			float spDelta,
			float distDelta,
			float accDelta);
	//! Control the speed of alpha-delta virtual motors during a few ms
	//! \param out_cmd Resulting command that will be interpreted by path_manager
	void motion_SpeedControlADMaxTime(RobotCommand *out_cmd,
			float spAlpha,
			float accAlpha,
			float spDelta,
			float accDelta,
			int duringMs);
	//! Test PID system with step order response
	//! \param out_cmd Resulting command that will be interpreted by path_manager
	void motion_StepOrderLR(RobotCommand *out_cmd, int32 posLeft, int32 posRight, int seconds);
	//! Test PID system with step order response
	//! \param out_cmd Resulting command that will be interpreted by path_manager
	void motion_StepOrderAD(RobotCommand *out_cmd, int32 posAlpha, int32 posDelta, int seconds);

	//void pos_SetPosition(float x, float y, float theta);

	float motion_GetDefaultVmax();
	float motion_GetDefaultAccel();
	float motion_GetDefaultDecel();

	void motion_SetDefaultVmax(float speed_m_sec); //en m/s
	void motion_SetDefaultAccel(float accel);
	void motion_SetDefaultDecel(float decel);

};

#endif
