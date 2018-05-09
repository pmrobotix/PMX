#ifndef EV3_SOUNDDRIVER_HPP_
#define EV3_SOUNDDRIVER_HPP_

#include <string>
#include <vector>

#include "../Common/Action.Driver/ASoundDriver.hpp"

class SoundDriver: public ASoundDriver
{

public:

    void beep(const std::string &args, bool bSynchronous);

    void tone(unsigned frequency, unsigned ms, bool bSynchronous);

    void tone(const std::vector<std::vector<float> > &sequence, bool bSynchronous);

    void play(const std::string &soundfile, bool bSynchronous);

    void speak(const std::string &text, bool bSynchronous);

    /*!
     * \brief Constructor.
     */
    SoundDriver();

    /*!
     * \brief Destructor.
     */
    ~SoundDriver();

};

#endif
