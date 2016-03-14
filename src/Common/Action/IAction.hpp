/*!
 * \file
 * \brief Définition de l'interface IAction.
 */

#ifndef COMMON_IACTION_HPP_
#define COMMON_IACTION_HPP_

#include <string>

/*!
 * \brief Cette interface représente une action executée sur la base
 * électronique du moteur.
 */
class IAction
{
public:

	/*!
	 * \brief Execute une action.
	 * \retval true
	 *         Si l'éxecution doit être conservée.
	 * \retval false
	 *         Si l'action doit être supprimée après son execution.
	 */
	virtual bool execute() = 0;

	/*!
	 * \brief Getter sur les infos permettant d'identifier l'action.
	 */
	virtual std::string info() = 0;

	/*!
	 * \brief Destructeur de la classe.
	 */
	virtual inline ~ IAction()
	{
	}

protected:

	/*!
	 * \brief Constructeur de la classe.
	 */
	IAction()
	{
	}
};

#endif
