#ifndef LEGOEV3LCDTEST_HPP_
#define	LEGOEV3LCDTEST_HPP_

#include "../Common/FunctionalTest.hpp"
#include "../Log/LoggerFactory.hpp"

/*!
 * \brief Effectue un test sur le LCD EV3.
 */
class L_LcdTest: public FunctionalTest
{
private:

    /*!
     * \brief Retourne le \ref Logger associé à la classe \ref L_LcdTest.
     */
    static inline const logs::Logger & logger()
    {
        static const logs::Logger & instance = logs::LoggerFactory::logger("L_LcdTest");
        return instance;
    }
public:

    /*!
     * \brief Constructeur de la classe.
     */
    L_LcdTest() :
            FunctionalTest("Lcd", "Ecrit sur le LCD")
    {
    }

    /*!
     * \brief Destructeur de la classe.
     */
    virtual ~L_LcdTest()
    {
    }

    /*!
     * \brief Execute le test.
     */
    virtual void run(int argc, char** argv);

};

#endif
