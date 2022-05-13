#include "CommandManager.h"

#include "../blockingDetector/BlockingDetector.h"
#include "../Regulator.h"
#include "Commands/GotoAngle.h"
#include "Commands/StraitLine.h"
#include "Commands/Turn.h"

#define MAX(a,b) (((a)>(b))?(a):(b))
#define COMMAND_MAX_SIZE MAX( MAX( MAX( MAX(sizeof(StraitLine), sizeof(Turn)), sizeof(Goto)), sizeof(GotoAngle) ), sizeof(GotoNoStop) )

CommandManager::CommandManager(float straitLineArrivalWindows_mm, float turnArrivalWindows_rad,
        Goto::GotoConfiguration &preciseGotoConfiguration, Goto::GotoConfiguration &waypointGotoConfiguration, GotoNoStop::GotoNoStopConfiguration &gotoNoStopConfiguration,
        const Regulator &angle_regulator, const Regulator &distance_regulator,
        AccelerationDecelerationLimiter *accelerationDecelerationLimiter,
        BlockingDetector *blockingDetector):
        m_cmdList(32,COMMAND_MAX_SIZE),
        m_straitLineArrivalWindows_mm(straitLineArrivalWindows_mm), m_turnArrivalWindows_rad(turnArrivalWindows_rad),
        m_preciseGotoConfiguration(preciseGotoConfiguration), m_waypointGotoConfiguration(waypointGotoConfiguration), m_gotoNoStopConfiguration(gotoNoStopConfiguration),
        m_angle_regulator(angle_regulator), m_distance_regulator(distance_regulator)
{
    m_emergencyStop = false;
    m_currentCmd = nullptr;
    m_angleRegulatorConsign = 0;
    m_distRegulatorConsign = 0;
    m_accelerationDecelerationLimiter = accelerationDecelerationLimiter;
    m_blockingDetector = blockingDetector;
}

bool CommandManager::addStraightLine(float valueInmm)
{
    Command *ptr = m_cmdList.getFree();
    if(ptr == nullptr)
        return false;

    ptr = new StraitLine(valueInmm, m_straitLineArrivalWindows_mm);
    //new (ptr) StraitLine(valueInmm, m_straitLineArrivalWindows_mm);

    return m_cmdList.push();
}

bool CommandManager::addTurn(float angleInRad)
{
    Command *ptr = m_cmdList.getFree();
    if(ptr == nullptr)
        return false;

    ptr = new Turn(angleInRad, m_turnArrivalWindows_rad);
    //new (ptr) Turn(angleInRad, m_turnArrivalWindows_rad);
    m_cmdList.push();
    return true;
}

bool CommandManager::addGoTo(float posXInmm, float posYInmm)
{
    Command *ptr = m_cmdList.getFree();
    if(ptr == nullptr)
        return false;

    ptr = new Goto(posXInmm, posYInmm, &m_preciseGotoConfiguration);
    //new (ptr) Goto(posXInmm, posYInmm, &m_preciseGotoConfiguration);
    m_cmdList.push();
    return true;
}

bool CommandManager::addGoToWaypoint(float posXInmm, float posYInmm)
{
    Command *ptr = m_cmdList.getFree();
    if(ptr == nullptr)
        return false;

    ptr = new Goto(posXInmm, posYInmm, &m_waypointGotoConfiguration);
    //new (ptr) Goto(posXInmm, posYInmm, &m_waypointGotoConfiguration);
    m_cmdList.push();
    return true;
}

bool CommandManager::addGoToBack(float posXInmm, float posYInmm)
{
    Command *ptr = m_cmdList.getFree();
    if(ptr == nullptr)
        return false;

    ptr = new Goto(posXInmm, posYInmm, &m_preciseGotoConfiguration, true);
    //new (ptr) Goto(posXInmm, posYInmm, &m_preciseGotoConfiguration, true);
    m_cmdList.push();
    return true;
}

bool CommandManager::addGoToNoStop(float posXInmm, float posYInmm)
{
    Command *ptr = m_cmdList.getFree();
    if(ptr == nullptr)
        return false;

    ptr = new GotoNoStop(posXInmm, posYInmm, &m_gotoNoStopConfiguration, &m_preciseGotoConfiguration, false, m_accelerationDecelerationLimiter);
    //new (ptr) GotoNoStop(posXInmm, posYInmm, &m_gotoNoStopConfiguration, &m_preciseGotoConfiguration, false, m_accelerationDecelerationLimiter);
    m_cmdList.push();
    return true;
}

bool CommandManager::addGoToNoStopBack(float posXInmm, float posYInmm)
{
    Command *ptr = m_cmdList.getFree();
    if(ptr == nullptr)
        return false;

    ptr = new GotoNoStop(posXInmm, posYInmm, &m_gotoNoStopConfiguration, &m_preciseGotoConfiguration, true);
    //new (ptr) GotoNoStop(posXInmm, posYInmm, &m_gotoNoStopConfiguration, &m_preciseGotoConfiguration, true);
    m_cmdList.push();
    return true;
}

bool CommandManager::addGoToAngle(float posXInmm, float posYInmm)
{
    Command *ptr = m_cmdList.getFree();
    if(ptr == nullptr)
        return false;

    ptr = new GotoAngle(posXInmm, posYInmm, m_turnArrivalWindows_rad);
    //new (ptr) GotoAngle(posXInmm, posYInmm, m_turnArrivalWindows_rad);
    m_cmdList.push();
    return true;
}

void CommandManager::setEmergencyStop()
{
    m_angleRegulatorConsign = m_angle_regulator.getAccumulator();
    m_distRegulatorConsign = m_distance_regulator.getAccumulator();

    m_cmdList.flush();
    m_currentCmd = nullptr;

    m_emergencyStop = true;
}

void CommandManager::resetEmergencyStop()
{
    m_emergencyStop = false;
}

CommandManager::CommandStatus CommandManager::getCommandStatus()
{

    if( m_emergencyStop )
        return STATUS_HALTED;
    else if (m_currentCmd == nullptr)
        return STATUS_IDLE;
    else if( m_blockingDetector && m_blockingDetector->isBlocked())
        return STATUS_BLOCKED;
    else
        return STATUS_RUNNING;
}

uint8_t CommandManager::getPendingCommandCount()
{
    return m_cmdList.size();
}


void CommandManager::switchToNextCommand()
{
    if (m_currentCmd != nullptr)
       m_cmdList.pop();

    m_currentCmd = m_cmdList.getFirst();
}


void CommandManager::update(float X_mm, float Y_mm, float theta_rad)
{
    if (m_emergencyStop)
    {
        m_cmdList.flush();
        m_currentCmd = nullptr;
        return;
    }

    if (m_currentCmd != nullptr && !m_currentCmd->isGoalReached(X_mm, Y_mm, theta_rad, m_angle_regulator, m_distance_regulator, m_cmdList.getSecond()))
    {
        m_currentCmd->updateConsign(X_mm, Y_mm, theta_rad, &m_distRegulatorConsign, &m_angleRegulatorConsign, m_angle_regulator, m_distance_regulator);
    }
    else
    {
        switchToNextCommand();
        if( m_currentCmd != nullptr )
            m_currentCmd->computeInitialConsign(X_mm, Y_mm, theta_rad, &m_distRegulatorConsign, &m_angleRegulatorConsign, m_angle_regulator, m_distance_regulator);
    }
}




