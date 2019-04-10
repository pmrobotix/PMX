#ifndef LEGOEV3ASSERVESIALTEST_HPP_
#define	LEGOEV3ASSERVESIALTEST_HPP_

#include "../Common/FunctionalTest.hpp"
#include "../Log/LoggerFactory.hpp"

/*!
 * \brief Effectue un test de l'asservissement.
 */
class L_AsservEsialTest: public FunctionalTest
{
private:

    /*!
     * \brief Retourne le \ref Logger associé à la classe \ref L_AsservEsialTest.
     */
    static inline const logs::Logger & logger()
    {
        static const logs::Logger & instance = logs::LoggerFactory::logger("L_AsservEsialTest");
        return instance;
    }
public:

    /*!
     * \brief Constructeur de la classe.
     */
    L_AsservEsialTest() :
            FunctionalTest("Asserv_EsialR", "Reglages")
    {
    }

    /*!
     * \brief Destructeur de la classe.
     */
    virtual ~L_AsservEsialTest()
    {
    }

    /*!
     * \brief Execute le test.
     */
    virtual void run(int argc, char** argv);

    virtual void configureConsoleArgs(int argc, char** argv);

};

#endif
