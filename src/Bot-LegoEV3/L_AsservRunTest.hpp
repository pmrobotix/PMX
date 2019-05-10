#ifndef LEGOEV3ASSERVRUNTEST_HPP_
#define	LEGOEV3ASSERVRUNTEST_HPP_

#include <string>

#include "../Common/Action/IAction.hpp"
#include "../Common/FunctionalTest.hpp"
#include "../Common/Utils/Chronometer.hpp"
#include "../Log/Logger.hpp"
#include "../Log/LoggerFactory.hpp"

class L_ActionManagerTimerTest;

/*!
 * \brief Effectue un test de l'asservissement sur plusieurs positions.
 */
class L_AsservRunTest: public FunctionalTest
{
private:

    /*!
     * \brief Retourne le \ref Logger associé à la classe \ref LegoEV3AsservRunTest.
     */
    static inline const logs::Logger & logger()
    {
        static const logs::Logger & instance = logs::LoggerFactory::logger("L_AsservRunTest");
        return instance;
    }
public:

    /*!
     * \brief Constructeur de la classe.
     */
    L_AsservRunTest() :
            FunctionalTest("Asserv_Run", "AV/AR en xy avec sensors.")
    {
    }

    /*!
     * \brief Destructeur de la classe.
     */
    virtual ~L_AsservRunTest()
    {
    }

    /*!
     * \brief Execute le test.
     */
    virtual void run(int argc, char** argv);

    virtual void configureConsoleArgs(int argc, char** argv);

};

/*!
 * \brief Cette action permet de
 *
 */
class TestActionRun: public IAction
{
private:

    /*!
     * \brief Retourne le \ref Logger associé à la classe \ref TestActionRun.
     */
    static const logs::Logger & logger()
    {
        static const logs::Logger & instance = logs::LoggerFactory::logger("TestActionRun");
        return instance;
    }

    /*!
     * \brief Référence vers le test.
     */
    L_AsservRunTest & amt_;

    utils::Chronometer chrono_;

    int i_;

public:

    /*!
     * \brief Constructeur de la classe.
     * \param amt
     *        Reference vers l'objet associée.
     */
    TestActionRun(L_AsservRunTest & amt);

    /*!
     * \brief Destructeur de la classe.
     */
    virtual inline ~TestActionRun()
    {
    }

    /*!
     * \brief Execution de l'action.
     */
    virtual bool execute();

    /*!
     * \brief Retourne la description de l'action.
     */
    virtual inline std::string info()
    {
        return "TestAction";
    }
};

#endif
