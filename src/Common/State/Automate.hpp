/*!
 * \file
 * \brief Définition de la classe Automate.
 */

#ifndef AUTOMATE_HPP
#define	AUTOMATE_HPP

#include <stddef.h>

#include "../../Log/LoggerFactory.hpp"

class IAutomateState;
class Robot;

/*!
 * \brief Gère un automate simple.
 *
 * Un automate "simple" ne possède au plus qu'un état actif. Il n'est donc
 * pas possible de gérer un enbranchement avec cette classe.
 */
class Automate
{
private:

	/*!
	 * \brief Retourne le \ref Logger associé à la classe \ref Automate.
	 */
	static const logs::Logger & logger()
	{
		static const logs::Logger & instance = logs::LoggerFactory::logger("Automate");
		return instance;
	}

	/*!
	 * Etat courant de l'automate.
	 */
	IAutomateState* current_;

public:
	/*!
	 * \brief Constructeur de la classe.
	 */
	Automate();

	/*!
	 * \brief Destructeur de la classe.
	 */
	virtual ~Automate()
	{
		current_ = NULL;
	}

	/*!
	 * \brief Lance l'automate.
	 *
	 * L'automate n'est PAS lancé sur un nouveau thread, cette méthode est
	 * donc bloquante pour le processus actuel.
	 *
	 * \param robot
	 *        Le robot à manipuler.
	 * \param first
	 *        Etat initial de l'automate.
	 * \param data
	 *        Les données associées à l'execution de l'automate.
	 */
	void run(Robot& robot, IAutomateState* first, void* data);

protected:

	/*!
	 * \brief Retourne l'état courant de l'automate.
	 */
	inline IAutomateState* current() const
	{
		return this->current_;
	}

	/*!
	 * \brief Définie l'état courant de l'automate.
	 *
	 * \param current
	 *        Le nouvel état courant de l'automate.
	 */
	inline void current(IAutomateState* current)
	{
		this->current_ = current;
	}
};

#endif
