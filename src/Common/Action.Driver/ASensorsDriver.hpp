#ifndef ASENSORSDRIVER_HPP_
#define ASENSORSDRIVER_HPP_

#include <string>

class ASensorsDriver
{

public:
    /*!
     * \brief ASensorsDriver instance creation.
     */
    static ASensorsDriver * create(std::string botName);

    virtual int rightSide() = 0;
    virtual int leftSide() = 0;

    virtual int frontLeft() = 0;
    virtual int frontCenter() = 0;
    virtual int frontRight() = 0;

    virtual int backLeft() = 0;
    virtual int backCenter() = 0;
    virtual int backRight() = 0;

    /*!
     * \brief Destructor.
     */
    virtual ~ASensorsDriver()
    {
    }

protected:

    /*!
     * \brief Constructor.
     */
    ASensorsDriver()
    {
    }

};

#endif
