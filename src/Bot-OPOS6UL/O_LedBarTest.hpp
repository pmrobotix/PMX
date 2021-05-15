/*!
 * \file
 * \brief Définition de la classe O_LedBarTest.
 */

#ifndef OPOS6UL_LEDBARTEST_HPP
#define	OPOS6UL_LEDBARTEST_HPP

#include "../Common/FunctionalTest.hpp"
#include "../Log/LoggerFactory.hpp"

/*!
 * \brief Effectue un test de clignotement des LEDs du tableau d'affichage.
 */
class O_LedBarTest: public FunctionalTest {
private:

    /*!
     * \brief Retourne le \ref Logger associé à la classe \ref O_LedBarTest.
     */
    static inline const logs::Logger & logger() {
        static const logs::Logger & instance = logs::LoggerFactory::logger("O_LedBarTest");
        return instance;
    }
public:

    /*!
     * \brief Constructeur de la classe.
     */
    O_LedBarTest() :
            FunctionalTest("LedBar", "Blink Leds")
    {
    }

    /*!
     * \brief Destructeur de la classe.
     */
    virtual ~O_LedBarTest() {
    }

    /*!
     * \brief Execute le test.
     */
    virtual void run(int argc, char** argv);
};

#endif
