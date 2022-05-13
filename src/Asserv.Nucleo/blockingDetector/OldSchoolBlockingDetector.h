#ifndef SRC_BLOCKINGDETECTOR_OLDSCHOOLDETECTOR_H_
#define SRC_BLOCKINGDETECTOR_OLDSCHOOLDETECTOR_H_

#include "BlockingDetector.h"

class MotorController;
class Odometry;

class OldSchoolBlockingDetector : BlockingDetector
{
    public:
        explicit OldSchoolBlockingDetector(
                float dt, MotorController const *motorController, Odometry const *odometry,
                float block_angle_speed_threshold, float block_dist_speed_threshold, float blocking_detected_duration_threshold);
        virtual ~OldSchoolBlockingDetector();

        virtual bool isBlocked();
        virtual void update();

    private:
        float m_dt;
        MotorController const *m_motorController;
        Odometry const *m_odometry;

        float m_blocking_detected_duration;

        float m_block_angle_speed_threshold;
        float m_block_dist_speed_threshold;

        float m_blocking_detected_duration_threshold;
};

#endif /* SRC_BLOCKINGDETECTOR_OLDSCHOOLDETECTOR_H_ */
