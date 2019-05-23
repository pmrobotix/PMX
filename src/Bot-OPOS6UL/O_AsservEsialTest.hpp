#ifndef OPOS6UL_ASSERVESIALTEST_HPP_
#define	OPOS6UL_ASSERVESIALTEST_HPP_

#include "../Common/FunctionalTest.hpp"
#include "../Log/LoggerFactory.hpp"

/*!
 * \brief Effectue un test de l'asservissement.
 */
class O_AsservEsialTest: public FunctionalTest
{
private:

    /*!
     * \brief Retourne le \ref Logger associé à la classe \ref O_AsservEsialTest.
     */
    static inline const logs::Logger & logger()
    {
        static const logs::Logger & instance = logs::LoggerFactory::logger("O_AsservEsialTest");
        return instance;
    }
public:

    /*!
     * \brief Constructeur de la classe.
     */
    O_AsservEsialTest() :
            FunctionalTest("Asserv_EsialR", "Reglages")
    {
    }

    /*!
     * \brief Destructeur de la classe.
     */
    virtual ~O_AsservEsialTest()
    {
    }

    /*!
     * \brief Execute le test.
     */
    virtual void run(int argc, char** argv);

    virtual void configureConsoleArgs(int argc, char** argv);

};

#endif
