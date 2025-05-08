#ifndef BOT_OPOS6UL_O_ASSERVXYROTATETEST_HPP_
#define BOT_OPOS6UL_O_ASSERVXYROTATETEST_HPP_

#include "../Common/FunctionalTest.hpp"
#include "../Log/LoggerFactory.hpp"

/*!
 * \brief Effectue un test de l'asservissement sur une ligne droite et une rotation.
 */
class O_AsservXYRotateTest: public FunctionalTest
{
private:

    /*!
     * \brief Retourne le \ref Logger associé à la classe \ref O_AsservXYRotateTest.
     */
    static inline const logs::Logger & logger()
    {
        static const logs::Logger & instance = logs::LoggerFactory::logger("O_AsservXYRotateTest");
        return instance;
    }
public:

    /*!
     * \brief Constructeur de la classe.
     */
    O_AsservXYRotateTest() :
            FunctionalTest("AsservXYRotate", "Effectue une liste de point par coordonnées")
    {
    }

    /*!
     * \brief Destructeur de la classe.
     */
    virtual ~O_AsservXYRotateTest()
    {
    }

    /*!
     * \brief Execute le test.
     */
    virtual void run(int argc, char** argv);

    virtual void configureConsoleArgs(int argc, char** argv);

};


#endif /* BOT_OPOS6UL_O_ASSERVXYROTATETEST_HPP_ */
