#ifndef SRC_ENCODER_H_
#define SRC_ENCODER_H_

#include <cstdint>

class Encoders
{
public:
    virtual ~Encoders()
    {
    }

    virtual void getValues(float *deltaEncoderRight, float *deltaEncoderLeft) = 0;
};

#endif /* SRC_ENCODER_H_ */
