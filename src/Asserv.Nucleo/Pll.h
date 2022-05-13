#ifndef SRC_PLL_H_
#define SRC_PLL_H_

#include <cstdint>

class Pll
{

public:
    explicit Pll(float bandwidth);

    void update(int16_t deltaPosition, float deltaT);

    void setBandwidth(float bandwidth);

    inline void reset()
    {
        m_position = 0;
        m_speed = 0;
        m_count = 0;
    }

    inline float getSpeed()
    {
        return m_speed;
    }

    inline float getPosition()
    {
        return m_position;
    }

private:
    float m_kp;
    float m_ki;
    float m_position;
    float m_speed;
    int64_t m_count;
};

#endif /* SRC_PLL_H_ */
