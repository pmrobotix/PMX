/*!
 * \file
 * \brief Définition de la classe O_IAByZoneTest.
 */

#ifndef OPOS6UL_IABYZONETEST_HPP_
#define	OPOS6UL_IABYZONETEST_HPP_

#include "../Common/FunctionalTest.hpp"
#include "../Log/LoggerFactory.hpp"

/*!
 * \brief Effectue un test de l'IAByPath.
 */
class O_IAByZoneTest: public FunctionalTest
{
private:

    /*!
     * \brief Retourne le \ref Logger associé à la classe \ref O_IAByZoneTest.
     */
    static inline const logs::Logger & logger()
    {
        static const logs::Logger & instance = logs::LoggerFactory::logger("O_IAByZoneTest");
        return instance;
    }
public:

    /*!
     * \brief Constructeur de la classe.
     */
    O_IAByZoneTest() :
            FunctionalTest("IAByZone", "test l'ia demo IAByZone")
    {
    }

    /*!
     * \brief Destructeur de la classe.
     */
    virtual ~O_IAByZoneTest()
    {
    }

    /*!
     * \brief Execute le test.
     */
    virtual void run(int argc, char** argv);



};

#endif
