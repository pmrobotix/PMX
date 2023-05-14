/*!
 * \file
 * \brief Définition de la classe ActionManager.
 */

#ifndef COMMON_ACTIONMANAGERTIMER_HPP_
#define COMMON_ACTIONMANAGERTIMER_HPP_

#include <semaphore.h>
#include <list>
#include <string>

#include "../../Log/Logger.hpp"
#include "../../Log/LoggerFactory.hpp"
#include "../Utils/Chronometer.hpp"
#include "../Utils/PointerList.hpp"
#include "IAction.hpp"
#include "ITimerListener.hpp"
#include "ITimerPosixListener.hpp"

/*!
 * \brief Classe de gestion des actions du robot et des actions par timer
 */
class ActionManagerTimer: public utils::Thread
{
private:

    /*!
     * \brief Retourne le \ref Logger associé à la classe \ref ActionManagerTimer.
     */
    static inline const logs::Logger& logger()
    {
        static const logs::Logger &instance = logs::LoggerFactory::logger("ActionManagerTimer");
        return instance;
    }

    /*!
     * \brief Liste des actions à executer.
     */
    utils::PointerList<IAction*> actions_;

    /*!
     * \brief Liste des timers (gestion par timings), prend 100% processor.
     */
    utils::PointerList<ITimerListener*> timers_;

    /*!
     * \brief Liste des timers posix en cours d'execution.
     */
    utils::PointerList<ITimerPosixListener*> ptimers_;
    /*!
     * \brief Liste des timers posix à démarrer au plus vite.
     */
    utils::PointerList<ITimerPosixListener*> ptimers_tobestarted_;

    /*!
     * \brief Vaut \c true si les actions et timers doivent s'arréter.
     * \sa ::stop()
     */
    bool stopActionsAndTimers_;

    /*!
     * \brief Vaut \c true si les actions doivent se mettre en pause.
     * \sa ::pause()
     */
    bool pause_;

    /*!
     *\brief Chronomètre lié au Minuteur.
     */
    utils::Chronometer chronoTimer_;

    /*!
     *\brief Mutex lié aux timers.
     */
    utils::Mutex mtimer_;
    /*!
     *\brief Mutex lié aux actions.
     */
    utils::Mutex maction_;

    /*!
     *\brief Semaphore de gestion de la boucle des actions.
     */
    sem_t AMT;
    /*!
     *\brief Valeur du semaphore de gestion de la boucle des actions.
     */
    int val;

protected:

    /*!
     * \brief Execute l'ensemble des actions enregistrées.
     */
    virtual void execute();

    void unblock(std::string debug = "ActionManagerTimer")
    {
        sem_getvalue(&AMT, &val);
        //logger().info() << debug << " val =" << val << logs::end;
        if (val > 1)
            logger().error() << "ERROR - la valeur de semaphore est > 1 !!" << val << logs::end;
        //dans le cas d'une attente, on débloque.
        if (val == 0)
            sem_post(&AMT);
    }

public:

    /*!
     * \brief Constructeur de la classe.
     */
    ActionManagerTimer();

    /*!
     * \brief Destructeur de la classe.
     */
    virtual ~ActionManagerTimer()
    {
        sem_destroy(&AMT);
    }

    /*!
     * \brief Retourne le nombre d'actions.
     */
    int countActions()
    {
        maction_.lock();
        int size = this->actions_.size();
        maction_.unlock();
        return size;
    }

    /*!
     * \brief Retourne le nombre de timers.
     */
    int countTimers()
    {
        mtimer_.lock();
        int size = this->timers_.size();
        mtimer_.unlock();
        return size;
    }
    /*!
     * \brief Retourne le nombre de timers posix.
     */
    int countPTimers()
    {
        mtimer_.lock();
        int size = this->ptimers_.size();
        mtimer_.unlock();
        return size;
    }

    /*!
     * \brief Ajout d'une action.
     * \param action
     *        L'action à ajouter.
     */
    void addAction(IAction *action)
    {
        maction_.lock();
        actions_.push_back(action);
        maction_.unlock();
        unblock("addAction");
    }

    /*!
     * \brief Ajout d'un timer. deprecated.
     * \param timer
     *        le timer à ajouter.
     */
    void addTimer(ITimerListener *timer)
    {
        if (timer->timeSpan() != 0) {
            mtimer_.lock();
            timers_.push_back(timer);
            mtimer_.unlock();
            unblock("addTimer");
        }
    }
    /*!
     * \brief Ajout d'un timer posix.
     * \param timer
     *        le timer posix à ajouter.
     */
    void addTimer(ITimerPosixListener *timer)
    {
        if (timer->timeSpan_us() != 0) {
            mtimer_.lock();
            ptimers_tobestarted_.push_back(timer);
            mtimer_.unlock();

            //give the signal
            unblock("addTimerPosix");

        } else {
            logger().error() << "timeSpan_us is 0 !!" << logs::end;
        }
    }

    /*!
     * \brief arrete un timer spécifique. Permet d'executer son action de fin puis le supprime de la liste.
     * \param name
     *        Le label du timer.
     */
    void stopTimer(std::string timerNameToDelete);
    /*!
     * \brief arrete un timer posix spécifique. Permet d'executer son action de fin puis le supprime de la liste.
     * \param name
     *        Le label du timer.
     */
    void stopPTimer(std::string timerNameToDelete);

    /*!
     * \brief trouve un timer posix spécifique.
     * \param name
     *        Le label du timer.
     */
    bool findPTimer(std::string timerNameToFind);

    /*!
     * \brief arrete tous les timers posix. Permet d'executer leur action de fin puis les supprime de la liste.
     */
    void stopAllPTimers();

    /*!
     * \brief Vide la liste des actions actuellement enregistrées.
     */
    void clearActions()
    {
        maction_.lock();
        actions_.clear();
        maction_.unlock();
    }

    /*!
     * \brief Vide la liste des timers actuellement enregistrés.
     */
    void clearTimers()
    {
        mtimer_.lock();
        timers_.clear();
        ptimers_.clear();
        ptimers_tobestarted_.clear();
        mtimer_.unlock();
    }

    /*!
     * \brief L'appel à cette méthode signale au thread qu'il doit s'arrêter
     * (proprement). Arrete aussi tous les timers posix en cours en lancant
     * leur tache de fin.
     *
     * L'utilisation de la méthode ActionManager::stop() permet de
     * savoir si le thread associé est arrêté.
     */
    void stop();

//    bool getEnd() {
//        return stop_;
//    }

    /*!
     * \brief Met en pause la boucle du manager d'actions et de timer.
     */
    void pause(bool value);

    /*!
     * \brief Affiche via le logger les différentes actions en cours.
     */
    void debugActions();
    void debugPTimers();
    void debugTimers();
};

#endif
