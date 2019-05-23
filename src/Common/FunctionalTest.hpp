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
    /*!
     * \brief Numero du test dans la liste.
     */
    int num_;

protected:

    /*!
     * \brief Constructeur de la classe.
     * \param name
     *        Nom du test.
     */
    FunctionalTest(const std::string & name, const std::string & desc) :
            name_(name), description_(desc), num_(0)
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
     * \return Numero/position du test.
     */
    inline int position()
    {
        return num_;
    }

    /*!
     * \return Numero/position du test.
     */
    inline void setPos(int num)
    {
        num_ = num;
    }

    /*!
     * Méthode exécutant le test associé.
     */
    virtual void run(int argc, char** argv) = 0;

    virtual void configureConsoleArgs(int, char**) // A surcharger par le test en question pour ajouter ses parametres specifiques
    {
    }
};

#endif
