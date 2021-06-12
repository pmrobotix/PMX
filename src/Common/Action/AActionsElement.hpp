/*!
 * \file
 * \brief Définition de la classe AActionsElement.
 */

#ifndef AACTIONSELEMENT_HPP_
#define AACTIONSELEMENT_HPP_

// Prédéclaration de la classe Actions.
class Actions;

/*!
 * \brief Classe abstraite définissant la notion d'élément du robot.
 *
 * Chaque élément du robot possède un lien vers l'objet \ref Actions, quelque
 * soit son niveau dans la hiérarchie. Ceci permet à chaque élément
 * d'accéder simplement au manager associé et donc de pouvoir ajouter de nouvelles
 * actions dans la pile des actions à effectuer.
 */
class AActionsElement {
private:
    /*!
     * \brief Référence aux différentes Actions.
     */
    Actions & actions_;

protected:

    /*!
     * \brief Constructeur de la classe.
     * \param actions Référence à l'objet Actions associé.
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
    inline Actions & actions() {
        return actions_;
    }

    /*!
     * \brief Destructeur de la classe.
     */
    virtual inline ~AActionsElement() {
    }
};

#endif
