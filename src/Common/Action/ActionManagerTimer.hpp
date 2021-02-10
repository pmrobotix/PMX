/*!
 * \file
 * \brief Définition de la classe ActionManager.
 */

#ifndef COMMON_ACTIONMANAGERTIMER_HPP_
#define COMMON_ACTIONMANAGERTIMER_HPP_

#include <cstdio>
#include <list>
#include <string>

#include "../../Log/Logger.hpp"
#include "../../Log/LoggerFactory.hpp"
#include "../Utils/Chronometer.hpp"
#include "../Utils/PointerList.hpp"
#include "IAction.hpp"
#include "ITimerListener.hpp"

/*!
 * \brief Classe de gestion des actions du robot et des actions par timer
 */
class ActionManagerTimer: public utils::Thread
{
private:

    /*!
     * \brief Retourne le \ref Logger associé à la classe \ref ActionManagerTimer.
     */
    static inline const logs::Logger & logger()
    {
        static const logs::Logger & instance = logs::LoggerFactory::logger("ActionManagerTimer");
        return instance;
    }

    /*!
     * \brief Liste des actions à executer.
     */
    utils::PointerList<IAction*> actions_;

    /*!
     * \brief Liste des actions à executer en mode timer.
     */
    utils::PointerList<ITimerListener*> timers_;

    /*!
     * \brief Vaut \c true si les actions doivent s'arréter.
     * \sa ::stop()
     */
    bool stop_;

    /*!
     * \brief Vaut \c true si les actions doivent se mettre en pause.
     * \sa ::pause()
     */
    bool pause_;

    /*!
     *\brief Chronomètre lié au Minuteur.
     */
    utils::Chronometer chronoTimer_;

    utils::Mutex mtimer_;
    utils::Mutex maction_;

protected:

    /*!
     * \brief Execute l'ensemble des actions enregistrées.
     */
    virtual void execute();

public:

    /*!
     * \brief Constructeur de la classe.
     */
    ActionManagerTimer();

    /*!
     * \brief Destructeur de la classe.
     */
    inline virtual ~ActionManagerTimer()
    {
    }

    /*!
     * \brief Retourne le nombre d'actions.
     */
    inline int countActions()
    {
        maction_.lock();
        int size = this->actions_.size();
        maction_.unlock();
        return size;
    }

    /*!
     * \brief Retourne le nombre d'actions.
     */
    inline int countTimers()
    {
        mtimer_.lock();
        int size = this->timers_.size();
        mtimer_.unlock();
        return size;
    }

    /*!
     * \brief Ajout d'une action.
     * \param action
     *        L'action à ajouter.
     */
    inline void addAction(IAction * action)
    {
        maction_.lock();
        actions_.push_back(action);
        maction_.unlock();
    }

    /*!
     * \brief Ajout d'une action en mode timer.
     * \param timer
     *        le timer à ajouter.
     */
    inline void addTimer(ITimerListener * timer)
    {
        if (timer->timeSpan() != 0) {
            mtimer_.lock();
            timers_.push_back(timer);
            mtimer_.unlock();
        }
    }

    /*!
     * \brief arrete un timer spécifique. Permet d'executer son action de fin puis le supprime de la liste.
     * \param name
     *        Le label du timer.
     */
    inline void stopTimer(std::string timerNameToDelete) //TODO Move in the loop
    {
        bool found = false;
        utils::PointerList<ITimerListener *>::iterator save;
        utils::PointerList<ITimerListener *>::iterator i = timers_.begin();
        mtimer_.lock();
        while (i != timers_.end()) {
            ITimerListener * timer = *i;

            if (timer->name() == timerNameToDelete) {

                save = i;
                found = true;
                timer->onTimerEnd(chronoTimer_);
            }
            i++;
        }
        if (found)
            timers_.erase(save);
        else
            logger().debug() << "Timer [" << timerNameToDelete << "] not found or already deleted !!" << logs::end;

        mtimer_.unlock();
    }

    /*!
     * \brief Vide la liste des actions actuellement enregistrées.
     */
    inline void clearActions()
    {
        maction_.lock();
        actions_.clear();
        maction_.unlock();
    }

    /*!
     * \brief Vide la liste des actions actuellement enregistrées.
     */
    inline void clearTimers()
    {
        maction_.lock();
        timers_.clear();
        maction_.unlock();
    }

    /*!
     * \brief L'appel à cette méthode signale au thread qu'il doit s'arrêter
     * (proprement).
     *
     * L'utilisation de la méthode ActionManager::stop() permet de
     * savoir si le thread associé est arrêté.
     */
    inline void stop()
    {
        logger().debug() << "stop true" << logs::end;
        this->stop_ = true;
    }

    inline bool getEnd()
    {
        return stop_;
    }


    inline void pause(bool value)
    {
        logger().debug() << "pause " << value << logs::end;
        this->pause_ = value;
    }

    /*!
     * \brief Affiche via le logger les différentes actions en cours.
     */
    void debugActions();

    void debugTimers();
};

#endif
