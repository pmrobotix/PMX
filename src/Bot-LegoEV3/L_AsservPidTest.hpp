#ifndef LEGOEV3ASSERVRUNTEST_HPP_
#define	LEGOEV3ASSERVRUNTEST_HPP_

#include "../Common/FunctionalTest.hpp"
#include "../Log/LoggerFactory.hpp"


/*!
 * \brief Effectue un test de l'asservissement sur plusieurs positions.
 */
class L_AsservPidTest: public FunctionalTest
{
private:

    /*!
     * \brief Retourne le \ref Logger associé à la classe \ref L_AsservPidTest.
     */
    static inline const logs::Logger & logger()
    {
        static const logs::Logger & instance = logs::LoggerFactory::logger("L_AsservPidTest");
        return instance;
    }
public:

    /*!
     * \brief Constructeur de la classe.
     */
    L_AsservPidTest() :
            FunctionalTest("Asserv_Run", "Suite de points en xy avec/sans sensors.")
    {
    }

    /*!
     * \brief Destructeur de la classe.
     */
    virtual ~L_AsservPidTest()
    {
    }

    /*!
     * \brief Execute le test.
     */
    virtual void run(int argc, char** argv);

    virtual void configureConsoleArgs(int argc, char** argv);

};

#endif
