/*!
 * \file
 * \brief Définition de l'interface ITimerListener.
 */

#ifndef COMMON_ITIMERLISTENER_HPP
#define COMMON_ITIMERLISTENER_HPP

#include <string>

namespace utils {
class Chronometer;
}

/*!
 * \brief Cette interface représente une action executée par un timer lorsqu'il
 * atteint son seuil d'execution.
 */
class ITimerListener
{
public:

    /*!
     * \brief Actions à executer pour le timer.
     */
    virtual void onTimer(utils::Chronometer chrono) = 0;

    /*!
     * \brief Actions de fin à executer pour le timer.
     */
    virtual void onTimerEnd(utils::Chronometer chrono) = 0;

    /*!
     * \brief Getter sur les infos permettant d'identifier le timer.
     */
    virtual std::string name()
    {
        return name_;
    }

    inline int timeSpan()
    {
        return timeSpan_ms_;
    }

    //last execution time
    inline int getLastTime()
    {
        return lasttime_;
    }

    inline void setLastTime(long l)
    {
        lasttime_ = l;
    }

    inline bool requestToStop()
    {
        return requestToStop_;
    }


    /*!
     * \brief Destructeur de la classe.
     */
    virtual inline ~ ITimerListener()
    {
    }

protected:

    bool requestToStop_;

    long timeSpan_ms_; //for Timer tasks

    long lasttime_;

    std::string name_;

    void init(std::string label, uint time_us)
    {
        name_ = label;
        timeSpan_ms_ = time_us / 1000.0;
    }

    /*!
     * \brief Constructeur de la classe.
     */
    ITimerListener()
    {

        timeSpan_ms_ = 0;
        lasttime_ = 0;
        name_ = "iTimerListener_default";
        requestToStop_ = false;
    }
};

#endif
