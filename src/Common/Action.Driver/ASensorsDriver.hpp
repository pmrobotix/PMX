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

    virtual int right() = 0;

    virtual int left() = 0;

    virtual bool front() = 0;

    virtual bool rear() = 0; //TODO renommer back ??

    virtual bool frontVeryClosed() = 0;

    virtual bool rearVeryClosed() = 0;

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
