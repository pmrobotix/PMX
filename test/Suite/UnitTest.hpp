/*!
 * \file
 * \brief Définition de la classe DevUnitTest.
 */

#ifndef SUITE_UNITTEST_HPP
#define SUITE_UNITTEST_HPP

#include <string>

#include "../../src/Log/LoggerFactory.hpp"

/*!
 * \brief Cette classe permet de créer des tests unitaires.
 *
 * Chaque classe de test devra hériter de cette classe et définir la méthode
 * UnitTest::suite() qui lancera les différents tests unitaires associés.
 *
 * Tous les tests doivent avoir un nom propre définit à la construction de
 * l'objet.
 *
 * Les méthodes UnitTest::assert() et UnitTest::fail() permettent de valider
 * les tests effectués. La première vérifie qu'un test booléen est vrai, la
 * seconde arrète le test en cours et retourne une erreur.
 *
 * Les classes héritant de la classe \c UnitTest devrait respecter les points
 * suivants :
 * - Le nom de la classe se compose du nom de la classe testée, suffixé par \c Test.
 * - Les méthodes de tests sont préfixées par \c test.
 * - Toutes les méthodes de tests sont appelées par la méthode \c suite().
 */
class UnitTest
{
private:

	/*!
	 * \brief Cette méthode retourne le logger associé à cette classe.
	 * \return Le logger de cette classe.
	 */
	static inline const logs::Logger & logger()
	{
		static const logs::Logger & instance = logs::LoggerFactory::logger("UnitTest");
		return instance;
	}

	/*!
	 * \brief Nom associé au test.
	 */
	std::string name_;

protected:
	/*!
	 * \brief Constructeur de la classe.
	 */
	UnitTest(const std::string & name);

public:

	/*!
	 * \brief Destructeur.
	 */
	virtual inline ~UnitTest()
	{
	}

	/*!
	 * \brief Retourne le nom du test.
	 * \return Le nom du test.
	 */
	inline const std::string & name() const
	{
		return name_;
	}

	/*!
	 * \brief Valide un test dans le cadre des tests unitaires.
	 *
	 * \param test
	 *        Test effectué, true si le test est correct.
	 * \param message
	 *        Message d'erreur si le test échoue.
	 */
	void assert(bool test, const std::string & message);

	/*!
	 * \brief Cette méthode fait échouer un test.
	 *
	 * \param message Message d'erreur.
	 */
	void fail(const std::string & message);

	/*!
	 * \brief Vérifie si un message d'erreur à bien été émis.
	 * \param expected
	 *        Le message d'erreur attendu intégrant le logger et le level.
	 */
	void expectedError(const std::string & expected);

	/*!
	 * \brief Vérifie si un message d'erreur à bien été émis.
	 * \param expected
	 *        Le message d'erreur attendu intégrant le logger et le level.
	 * \param message
	 *        Erreur associé si le message d'erreur n'a pas été émis.
	 */
	void expectedError(const std::string & expected, const std::string & message);

	/*!
	 * \brief Execute les tests associés à la classe.
	 */
	virtual void suite() = 0;
};

#endif
