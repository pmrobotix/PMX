/*!
 * \file
 * \brief Définition de la classe Mutex.
 */

#ifndef UTILS_MUTEX_HPP
#define	UTILS_MUTEX_HPP

#include "pthread.h"

namespace utils
{
    /*!
     * \brief Implémentation d'une gestion de mutex.
     */
    class Mutex
    {
    private:

        /*!
         * \brief Reference du mutex associé.
         */
        pthread_mutex_t mutex_;

    public:

        /*!
         * \brief Constructeur de la classe.
         */
        Mutex();

        /*!
         * \brief Destructeur de la classe.
         */
        virtual ~Mutex()
        {
            pthread_mutex_destroy(& mutex_);
        }

        /*!
         * \brief Verrouille le mutex.
         * Cette méthode doit être appelée avant d'utiliser la ressource associée.
         * Si le mutex est déjà verrouillée, cette méthode attend qu'il se libère.
         */
        void lock();

        /*!
         * \brief Déverrouille le mutex.
         * Cette méthode doit être appelée après avoir utiliser la ressource associée.
         */
        void unlock();

        /*!
         * \brief Essaie de verrouiller le mutex.
         * \return \c true si le lock a pu être posée.
         */
        bool tryLock();
    };
}

#endif
