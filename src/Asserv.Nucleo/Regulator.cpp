#include "Regulator.h"

Regulator::Regulator(float Kp, float max_output)
{
    m_accumulator = 0;
    m_Kp = Kp;
    m_error = 0;
    m_output = 0;
    m_maxOutput = max_output;
    m_enabled = true;
}

void Regulator::updateFeedback(float feedback)
{
    m_accumulator += feedback;
}

/*
 * simple proportional regulator
 */

float Regulator::updateOutput(float goal)
{
    m_error = goal - m_accumulator;
    m_output = m_error * m_Kp;

    if( !m_enabled) // When this regulator is disabled, just set output to zero
        m_output = 0;

    if (m_output < -m_maxOutput)
        m_output = -m_maxOutput;
    else if (m_output > m_maxOutput)
        m_output = m_maxOutput;

    return m_output;
}
