#include "SpeedErrorBlockingDetector.h"

#include "../SpeedController/SpeedController.h"

SpeedErrorBlockingDetector::SpeedErrorBlockingDetector(
        float dt, SpeedController & rightSpeedController, SpeedController & leftSpeedController,
        float movingIntegralDuration, float movingIntegralErrorThreshold)
: m_rightSpeedController(rightSpeedController), m_leftSpeedController(leftSpeedController)
{
    m_dt = dt;
    m_nbValues = movingIntegralDuration*m_dt;
    m_currentIdx = 0;
    m_errorValues = new float[m_nbValues];
    m_movingIntegralError = 0;
    m_movingIntegralErrorThreshold  = movingIntegralErrorThreshold;
}

SpeedErrorBlockingDetector::~SpeedErrorBlockingDetector()
{
    delete m_errorValues;
}


void SpeedErrorBlockingDetector::update()
{
    float currentError = (m_leftSpeedController.getSpeedError() + m_rightSpeedController.getSpeedError()) / 2.0f;
    float outError = m_errorValues[m_currentIdx];
    m_errorValues[m_currentIdx] = currentError;
    m_movingIntegralError += (currentError-outError)*m_dt;
}

bool SpeedErrorBlockingDetector::isBlocked()
{
    return m_movingIntegralError > m_movingIntegralErrorThreshold;
}
