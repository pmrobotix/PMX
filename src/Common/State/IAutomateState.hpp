/*!
 * \file
 * \brief Définition de la classe IAutomateState.
 */

#ifndef IAUTOMATESTATE_HPP
#define	IAUTOMATESTATE_HPP

#include <string>

class Robot;

/*!
 * \brief Définit l'état d'un automate.
 *
 * Un état posséde une méthode execute() qui va effectuer la ou les opérations
 * associés à l'état de l'automate et retourner l'état suivant.
 *
 * \see pmx::Automate
 */
class IAutomateState
{
public:

	/*!
	 * \brief Traite l'état actuel et renvoie l'état suivant.
	 *
	 * Cette méthode doit être bloquante pour le processus et ne retourner
	 * un résultat qu'une fois l'une de ses transitions actives.
	 *
	 * \param robot
	 *        Le robot à manipuler.
	 * \param data
	 *        Les données associées à l'execution de l'automate.
	 * \return L'état suivant ou \c NULL si la fin de l'automate est atteinte.
	 */
	virtual IAutomateState* execute(Robot & robot, void * data) = 0;

	/*!
	 * \brief Destructeur de la classe.
	 */
	virtual ~IAutomateState()
	{
	}

	/*!
	 * \brief Ajoute une transition possible à cet état.
	 *
	 * \param name Nom de la transition
	 * \param state Etat associé à la transition
	 */
	virtual void addState(const std::string & name, IAutomateState * state) = 0;

	/*!
	 * \brief Retourne l'état associé à une transition spécifique.
	 *
	 * \param name Nom de la transition
	 * \return L'état associé à la transition ou \c NULL.
	 */
	virtual IAutomateState* getState(const std::string & name) = 0;
	virtual std::string name() =0;
protected:

	/*!
	 * \brief Constructeur de la classe.
	 */
	IAutomateState()
	{
	}
};

#endif
