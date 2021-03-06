/*!
 * \file
 * \brief Définition de la classe AAsservElement.
 */

#ifndef COMMON_AASSERVELEMENT_HPP_
#define COMMON_AASSERVELEMENT_HPP_

// Prédéclaration de la classe Asserv.
class Asserv;

/*!
 * \brief Classe abstraite définissant la notion d'élément de l'asservissement.
 *
 * Chaque élément possède un lien vers l'objet \ref Asserv, quelque
 * soit son niveau dans la hiérarchie.
 */
class AAsservElement
{
private:
	/*!
	 * \brief Référence à l'objet Asserv.
	 */
	Asserv & asserv_;

protected:

	/*!
	 * \brief Constructeur de la classe.
	 * \param robot Référence à l'objet Asserv associé.
	 *
	 * Ce constructeur est \c protected pour éviter toute instantiation
	 * de la classe abstraite.
	 */
	AAsservElement(Asserv & asserv) :
		asserv_(asserv)
	{
	}

public:

	/*!
	 * \brief Retourne une référence sur l'objet Asserv associé.
	 */
	inline Asserv & asserv()
	{
		return asserv_;
	}

	/*!
	 * \brief Destructeur de la classe.
	 */
	virtual inline ~AAsservElement()
	{
	}
};

#endif
