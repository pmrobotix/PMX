#ifndef SRC_BLOCKINGDETECTION_H_
#define SRC_BLOCKINGDETECTION_H_

class BlockingDetector
{
public:
    virtual ~BlockingDetector()
    {
    }

    virtual void update() = 0;
    virtual bool isBlocked() = 0;
};

#endif /* SRC_BLOCKINGDETECTION_H_ */
