#ifndef ASOUNDDRIVER_HPP_
#define ASOUNDDRIVER_HPP_

#include <string>
#include <vector>

class ASoundDriver
{

public:
    /*!
     * \brief ASoundDriver instance creation.
     */
    static ASoundDriver * create();

    /*!
     * \brief Beep.
     */
    virtual void beep(const std::string &args, bool bSynchronous = true) = 0;

    virtual void tone(unsigned frequency, unsigned ms, bool bSynchronous = true) = 0;

    virtual void tone(const std::vector<std::vector<float> > &sequence, bool bSynchronous = true) = 0;

    virtual void play(const std::string &soundfile, bool bSynchronous = true) = 0;

    virtual void speak(const std::string &text, bool bSynchronous = true) = 0;

    /*!
     * \brief Destructor.
     */
    virtual ~ASoundDriver()
    {
    }

protected:

    /*!
     * \brief Constructor.
     */
    ASoundDriver()
    {
    }

};

#endif
