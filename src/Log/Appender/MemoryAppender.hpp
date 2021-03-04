/*!
 * \file
 * \brief Définition de la classe MemoryAppender.
 */

#ifndef LOGS_MEMORYAPPENDER_HPP_
#define	LOGS_MEMORYAPPENDER_HPP_

#include <chrono>
#include <list>
#include <string>

#include "../../Thread/Mutex.hpp"
#include "../Logger.hpp"

using namespace std::chrono;

namespace logs {
/*!
 * \brief Implémentation de Appender stockant les messages en mémoire.
 *
 * La méthode flush() permet d'effectuer un affichage des messages sur la
 * sortie standard. Cette méthode est automatiquement appelé lors de la
 * destruction de l'objet.
 */
class MemoryAppender: public Appender, public utils::Mutex
{
protected:

    /*!
     * \brief Liste des messages enregistrés.
     */
    std::list<std::string> messages_;
    long long duration_;
    void lockMessages();
    void unlockMessages();

    system_clock::time_point start_;

public:

    /*!
     * \brief Constructeur de la classe.
     */
    MemoryAppender();

    /*!
     * \brief Destructeur de la classe.
     */
    virtual ~MemoryAppender();

    /*!
     * \return Liste des messages enregistrés.
     */
    inline const std::list<std::string> & messages() const
    {
        return messages_;
    }

    /*!
     * \brief Méthode générique de trace d'un message. Ajoute le nom du log devant chaque entrée.
     * \param logger
     *        Logger de référence du message.
     * \param level
     *        Niveau de référence du message.
     * \param message
     *        Message à tracer.
     */
    virtual void writeMessage(const logs::Logger & logger, const logs::Level & level, const std::string & message);

    /*!
     * \brief Méthode générique de trace d'un message sans le nom du log devant chaque entrée.
     * \param message
     *        Message à tracer.
     */
    virtual void writeMessageOnly(const std::string & message);

    /*!
     * \brief Cette méthode affiche tous les messages engistrés sur le
     * flux de sortie standard.
     */
    virtual void flush();

};
}

#endif
