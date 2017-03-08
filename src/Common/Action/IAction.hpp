/*!
 * \file
 * \brief Définition de l'interface IAction.
 */

#ifndef COMMON_IACTION_HPP_
#define COMMON_IACTION_HPP_

#include <string>

/*!
 * \brief Cette interface représente une action executée.
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
	virtual std::string info()
	{
		return name_;
	}

	/*!
	 * \brief Destructeur de la classe.
	 */
	virtual inline ~ IAction()
	{
	}

protected:

	std::string name_;

	/*!
	 * \brief Constructeur de la classe.
	 */
	IAction()
	{
		name_ = "iAction_default";
	}
};

#endif
