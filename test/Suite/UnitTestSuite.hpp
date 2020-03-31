/*!
 * \file
 * \brief Définition de la classe UnitTestSuite.
 */

#ifndef SUITE_UNITTESTSUITE_HPP
#define SUITE_UNITTESTSUITE_HPP

#include <list>

#include "../../src/Common/Utils/PointerList.hpp"
#include "../../src/Log/LoggerFactory.hpp"
#include "UnitTest.hpp"

/*!
 * \brief Cette classe assure la gestion d'une suite de tests unitaires.
 */
class UnitTestSuite
{
private:

    /*!
     * \brief Cette méthode retourne le logger associé à cette classe.
     * \return Le logger de cette classe.
     */
    static inline const logs::Logger& logger()
    {
        static const logs::Logger &instance = logs::LoggerFactory::logger("UnitTestSuite");
        return instance;
    }

    /*!
     * \brief Ensemble des tests associés à cette suite.
     */
    utils::PointerList<UnitTest*> tests_;

public:
    /*!
     * \brief Constructeur de la classe.
     */
    UnitTestSuite();

    /*!
     * \brief Destructeur de la classe.
     */
    virtual ~UnitTestSuite()
    {
        //Tue le log s'il existe (core dump sinon)
        logs::LoggerFactory::instance().stopLog();
    }

    /*!
     * \brief Ajoute un objet de test à cette suite sous la forme d'un
     * pointeur qui sera détruit en même temps que cette suite.
     *
     * \param unitTest
     *        Pointeur vers l'objet de test.
     */
    inline void addTest(UnitTest *unitTest)
    {
        tests_.push_back(unitTest);
    }

    /*!
     * \brief Execution de tous les tests.
     */
    void run();
};

#endif
