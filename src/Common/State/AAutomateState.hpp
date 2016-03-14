/*!
 * \file
 * \brief Définition de la classe AAutomateState.
 */

#ifndef AAUTOMATESTATE_HPP
#define AAUTOMATESTATE_HPP

#include <map>
#include <string>

#include "IAutomateState.hpp"

/*!
 * \brief Définit les méthodes communes à tous les états.
 */
class AAutomateState: public IAutomateState
{
private:

	/*!
	 * \brief Les transitions définies pour cet état.
	 */
	std::map<std::string, IAutomateState*> transitions_;

public:

	/*!
	 * \brief Constructeur de la classe.
	 */
	AAutomateState() :
			transitions_()
	{
	}
	virtual std::string name()
	{
		return "AAutomateState";
	}
	/*!
	 * \brief Destructeur de la classe.
	 */
	virtual ~AAutomateState()
	{
	}

	/*!
	 * \brief Ajoute une transition possible à cet état.
	 *
	 * \param name Nom de la transition
	 * \param state Etat associé à la transition
	 */
	inline void addState(const std::string & name, IAutomateState * state)
	{
		this->transitions_[name] = state;
	}

public:

	/*!
	 * \brief Retourne l'état associé à une transition spécifique.
	 *
	 * \param name Nom de la transition
	 * \return L'état associé à la transition ou \c NULL.
	 */
	inline IAutomateState* getState(const std::string & name)
	{
		return this->transitions_[name];
	}
};

#endif
