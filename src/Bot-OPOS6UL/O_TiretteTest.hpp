#ifndef OPOS6UL_TIRETTETEST_HPP
#define	OPOS6UL_TIRETTETEST_HPP

#include "../Common/FunctionalTest.hpp"
#include "../Log/LoggerFactory.hpp"

/*!
 * \brief Effectue un test sur les buttons.
 */
class O_TiretteTest: public FunctionalTest {
private:

    /*!
     * \brief Retourne le \ref Logger associé à la classe \ref O_TiretteTest.
     */
    static inline const logs::Logger & logger() {
        static const logs::Logger & instance = logs::LoggerFactory::logger("O_TiretteTest");
        return instance;
    }
public:

    /*!
     * \brief Constructeur de la classe.
     */
    O_TiretteTest() :
            FunctionalTest("Tirette", "Tester la tirette.")
    {
    }

    /*!
     * \brief Destructeur de la classe.
     */
    virtual ~O_TiretteTest() {
    }

    /*!
     * \brief Execute le test.
     */
    virtual void run(int argc, char** argv);

};

#endif
