#ifndef REGULATOR_H_
#define REGULATOR_H_

#include <cstdint>

class Regulator
{
public:
    explicit Regulator(float Kp, float max_output);
    virtual ~Regulator() {};

    void updateFeedback(float feedback);
    float updateOutput(float goal);

    float getAccumulator() const
    {
        return m_accumulator;
    };

    void setGain(float Kp)
    {
        m_Kp = Kp;
    }

    float getGain() const
    {
        return m_Kp;
    };

    void reset()
    {
        m_accumulator = 0;
    };

    float getError() const
    {
        return m_error;
    };

    float getOutput() const
    {
        return m_output;
    };

    void enable()
    {
        m_enabled = true;
    };

    void disable()
    {
        m_enabled = false;
    };


private:
    double m_accumulator;
    float m_Kp;
    float m_error;
    float m_output;
    float m_maxOutput;
    float m_enabled;
};

#endif /* REGULATOR_H_ */
