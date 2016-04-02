/*!
 * \file
 * \brief Définition de la classe ConsoleManager.
 */

#ifndef COMMON_CONSOLEMANAGER_HPP_
#define	COMMON_CONSOLEMANAGER_HPP_

#include <string>
#include <vector>

#include "FunctionalTest.hpp"


/*!
 * \brief Classe de gestion de l'ensemble des tests de fonctionnement du
 * robot.
 */
class ConsoleManager
{
public:
	/*!
	 * \brief Type associé aux stockages des tests.
	 */
	typedef std::vector<FunctionalTest *> data_type;

private:

	/*!
	 * \brief Liste des tests connus par le manager.
	 */
	data_type tests_;

public:

	/*!
	 * \brief Constructeur de la classe.
	 */
	ConsoleManager();

	/*!
	 * \brief Destructeur de la classe.
	 */
	virtual inline ~ ConsoleManager()
	{
	}

	/*!
	 * \brief Ajoute un test au gestionnaire.
	 * \param test
	 *        Le nouveau test associé au gestionnaire.
	 */
	void add(FunctionalTest *test);

	/*!
	 * \brief Affiche un menu de sélection des tests.
	 */
	void displayMenuFunctionalTestsAndRun(int argc, char** argv);


	std::string * displayAvailableTests(std::string color = "\033[0m", int selected = -1);

	std::string displayMenuFirstArgu();


	/*!
	 * \brief Détermine un test a lancer en fonction des paramètres
	 * de la ligne de commande.
	 */
	void run(uint nTest, int argc, char** argv);

protected:

	/*!
	 * \brief Execute un test en particulier en fonction de son identifiant.
	 * \param nTest
	 *        Numéro de test à exécuter (à partir de 1).
	 * \param args
	 *        Tableau d'argument.
	 */
	void executeTest(uint nTest, int argc, char** argv);

};

#endif
