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

    virtual bool front() = 0;

    virtual bool rear() = 0; //TODO renommer back ??

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
