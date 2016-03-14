/*!
 * \file
 * \brief Définition de la classe ARobotElement.
 */

#ifndef AACTIONSELEMENT_HPP_
#define AACTIONSELEMENT_HPP_

// Prédéclaration de la classe Robot.
/*!
 * \todo Trouver une méthode plus "standardisé" pour la résolution des noms.
 */
class Actions;

/*!
 * \brief Classe abstraite définissant la notion d'élément du robot.
 *
 * Chaque élément du robot possède un lien vers l'objet \ref Robot, quelque
 * soit son niveau dans la hiérarchie. Ceci permet à chaque élément
 * d'accéder simplement au manager et donc de pouvoir ajouter de nouvelles
 * actions.
 */
class AActionsElement
{
private:
	/*!
	 * \brief Référence à l'objet Robot.
	 */
	Actions & actions_;

protected:

	/*!
	 * \brief Constructeur de la classe.
	 * \param robot Référence à l'objet Actions associé.
	 *
	 * Ce constructeur est \c protected pour éviter toute instantiation
	 * de la classe abstraite.
	 */
	AActionsElement(Actions & actions) :
		actions_(actions)
	{
	}

public:

	/*!
	 * \brief Retourne une référence sur le Actions associé.
	 */
	inline Actions & actions()
	{
		return actions_;
	}

	/*!
	 * \brief Destructeur de la classe.
	 */
	virtual inline ~AActionsElement()
	{
	}
};

#endif
