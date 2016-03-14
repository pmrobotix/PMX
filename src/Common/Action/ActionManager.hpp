/*!
 * \file
 * \brief Définition de la classe ActionManager.
 */

#ifndef ACTIONMANAGER_HPP_
#define ACTIONMANAGER_HPP_

#include <list>

#include "../../Log/LoggerFactory.hpp"
#include "../../Thread/Thread.hpp"
#include "../Utils/PointerList.hpp"
#include "IAction.hpp"

/*!
 * \brief Classe de gestion des actions du robot.
 */
class ActionManager: public utils::Thread
{
private:

	/*!
	 * \brief Retourne le \ref Logger associé à la classe \ref ActionManager.
	 */
	static inline const logs::Logger & logger()
	{
		static const logs::Logger & instance = logs::LoggerFactory::logger("ActionManager");
		return instance;
	}

	/*!
	 * \brief Liste des actions à executer.
	 */
	utils::PointerList<IAction*> actions_;

	/*!
	 * \brief Vaut \c true si le système doit s'arréter.
	 * \sa ::stop()
	 */
	bool stop_;

protected:

	/*!
	 * \brief Execute l'ensemble des actions enregistrées.
	 */
	virtual void execute();

public:

	/*!
	 * \brief Constructeur de la classe.
	 */
	ActionManager();

	/*!
	 * \brief Destructeur de la classe.
	 */
	inline virtual ~ActionManager()
	{
	}

	/*!
	 * \brief Retourne le nombre d'actions.
	 */
	inline int count()
	{
		lock();
		int size = this->actions_.size();
		unlock();
		return size;
	}

	/*!
	 * \brief Ajout d'une action.
	 * \param action
	 *        L'action à ajouter.
	 */
	inline void addAction(IAction * action)
	{
		lock();
		actions_.push_back(action);
		unlock();
	}

	/*!
	 * \brief Vide la liste des actions actuellement enregistrées.
	 */
	inline void clearActions()
	{
		lock();
		actions_.clear();
		unlock();
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
		this->stop_ = true;
	}

	/*!
	 * \brief Affiche via le logger les différentes actions en cours.
	 */
	void debug();
};

#endif
