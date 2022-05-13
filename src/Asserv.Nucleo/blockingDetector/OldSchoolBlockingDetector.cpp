
#include "../Odometry.h"
#include "../motorController/MotorController.h"
#include <math.h>
#include "OldSchoolBlockingDetector.h"

OldSchoolBlockingDetector::OldSchoolBlockingDetector(
        float dt, MotorController const *motorController, Odometry const *odometry,
        float block_angle_speed_threshold, float block_dist_speed_threshold, float blocking_detected_duration_threshold)
{
    m_dt = dt;
    m_motorController = motorController;
    m_odometry = odometry;
    m_block_angle_speed_threshold = block_angle_speed_threshold;
    m_block_dist_speed_threshold = block_dist_speed_threshold;
    m_blocking_detected_duration = 0;
    m_blocking_detected_duration_threshold = blocking_detected_duration_threshold;
}

OldSchoolBlockingDetector::~OldSchoolBlockingDetector()
{
}


void OldSchoolBlockingDetector::update()
{
    if( m_motorController->getMotorLeftSpeed() != 0 && m_motorController->getMotorRightSpeed() != 0
        && fabs(m_odometry->getDeltaTheta() < m_block_angle_speed_threshold)
        && fabs(m_odometry->getDeltaDist() < m_block_dist_speed_threshold)    )
    {
        m_blocking_detected_duration += m_dt;
    }
    else
    {
        m_blocking_detected_duration = 0;
    }
}

bool OldSchoolBlockingDetector::isBlocked()
{
    return (m_blocking_detected_duration > m_blocking_detected_duration_threshold);
}
