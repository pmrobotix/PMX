#include "Pll.h"
#include <cmath>

Pll::Pll(float bandwidth)
{
    setBandwidth(bandwidth);
    reset();
}

// Look at this thread for more informations
// https://discourse.odriverobotics.com/t/rotor-encoder-pll-and-velocity/224

void Pll::update(int16_t deltaTicks, float deltaT)
{
    // Prediction
    m_position += deltaT * m_speed;

    // Compute error between prediction and encoder information
    m_count += deltaTicks;
    float deltaPos = (float) (m_count - (int64_t) floor(m_position));

    // PLL correction
    m_position += deltaT * m_kp * deltaPos;
    m_speed += deltaT * m_ki * deltaPos;

    if (fabsf(m_speed) < 0.5f * deltaT * m_ki)
        m_speed = 0.0f;
}

void Pll::setBandwidth(float bandwidth)
{
    m_kp = 2.0f * bandwidth;
    m_ki = 0.25f * m_kp * m_kp;
}
