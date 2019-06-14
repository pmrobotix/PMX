#ifndef OPOS6UL_ASSERVLINEROTATETEST_HPP_
#define	OPOS6UL_ASSERVLINEROTATETEST_HPP_

#include "../Common/FunctionalTest.hpp"
#include "../Log/LoggerFactory.hpp"

/*!
 * \brief Effectue un test de l'asservissement sur une ligne droite et une rotation.
 */
class O_AsservLineRotateTest: public FunctionalTest
{
private:

    /*!
     * \brief Retourne le \ref Logger associé à la classe \ref O_AsservLineRotateTest.
     */
    static inline const logs::Logger & logger()
    {
        static const logs::Logger & instance = logs::LoggerFactory::logger("O_AsservLineRotateTest");
        return instance;
    }
public:

    /*!
     * \brief Constructeur de la classe.
     */
    O_AsservLineRotateTest() :
            FunctionalTest("Asserv_LineRotate", "Effectue une ligne droite et une rotation eventuelle.")
    {
    }

    /*!
     * \brief Destructeur de la classe.
     */
    virtual ~O_AsservLineRotateTest()
    {
    }

    /*!
     * \brief Execute le test.
     */
    virtual void run(int argc, char** argv);

    virtual void configureConsoleArgs(int argc, char** argv);

};

#endif
