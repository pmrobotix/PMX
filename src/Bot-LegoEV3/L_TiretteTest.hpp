#ifndef LEGOEV3TIRETTETEST_HPP_
#define	LEGOEV3TIRETTETEST_HPP_

#include "../Common/FunctionalTest.hpp"
#include "../Log/LoggerFactory.hpp"

/*!
 * \brief Effectue un test sur la tirette.
 */
class L_TiretteTest: public FunctionalTest
{
private:

    /*!
     * \brief Retourne le \ref Logger associé à la classe \ref L_TiretteTest.
     */
    static inline const logs::Logger & logger()
    {
        static const logs::Logger & instance = logs::LoggerFactory::logger("L_TiretteTest");
        return instance;
    }
public:

    /*!
     * \brief Constructeur de la classe.
     */
    L_TiretteTest() :
            FunctionalTest("Tirette", "La tirette")
    {
    }

    /*!
     * \brief Destructeur de la classe.
     */
    virtual ~L_TiretteTest()
    {
    }

    /*!
     * \brief Execute le test.
     */
    virtual void run(int argc, char** argv);

};

#endif
