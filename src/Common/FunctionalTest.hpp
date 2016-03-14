/*!
 * \file
 * \brief Class definition for functional tests.
 */

#ifndef COMMON_FUNCTIONALTEST_HPP_
#define	COMMON_FUNCTIONALTEST_HPP_

#include <string>

class Arguments;

/*!
 * \brief Abstract class to implement functional tests using ConsoleManager.
 */
class FunctionalTest
{
private:

	/*!
	 * \brief Nom du test.
	 */
	std::string name_;
	std::string description_;

protected:

	/*!
	 * \brief Constructeur de la classe.
	 * \param name
	 *        Nom du test.
	 */
	FunctionalTest(const std::string & name, const std::string & desc) :
			name_(name), description_(desc)
	{
	}

public:

	/*!
	 * Destructeur de la classe.
	 */
	virtual inline ~ FunctionalTest()
	{
	}

	/*!
	 * \return Nom du test.
	 */
	inline const std::string & name() const
	{
		return name_;
	}

	/*!
	 * \return Description du test.
	 */
	inline const std::string & desc() const
	{
		return description_;
	}

	/*!
	 * Méthode exécutant le test associé.
	 */
	virtual void run(Arguments *args) = 0;
};

#endif
