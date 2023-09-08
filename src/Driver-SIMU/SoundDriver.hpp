#ifndef SIMU_SOUNDDRIVER_HPP_
#define SIMU_SOUNDDRIVER_HPP_

#include <string>
#include <vector>

#include "../Common/Interface.Driver/ASoundDriver.hpp"
#include "../Log/LoggerFactory.hpp"

class SoundDriver: public ASoundDriver
{
private:

    /*!
     * \brief Retourne le \ref Logger associé à la classe \ref SoundDriver(SIMU).
     */
    static inline const logs::Logger& logger()
    {
        static const logs::Logger &instance = logs::LoggerFactory::logger("SoundDriver.SIMU");
        return instance;
    }

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
