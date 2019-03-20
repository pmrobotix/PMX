#ifndef COMMON_ROBOT_HPP_
#define COMMON_ROBOT_HPP_

#include <string>

#include "../Log/LoggerFactory.hpp"
#include "../Log/SvgWriter.hpp"
#include "Arguments.hpp"
#include "ConsoleManager.hpp"
#include "State/Automate.hpp"
#include "Utils/Chronometer.hpp"

class SvgWriter;

class Asserv;

class Actions;
class Arguments;
class ConsoleManager;

enum RobotColor
{
    PMXNOCOLOR, PMXVIOLET, PMXYELLOW
};

class Robot
{
public:

    /*!
     * \brief Retourne le \ref Logger associé à la classe \ref Robot.
     * public car utilisé dans Main.
     */
    static inline const logs::Logger & logger()
    {
        static const logs::Logger & instance = logs::LoggerFactory::logger("Robot");
        return instance;
    }

protected:

    utils::Chronometer chrono_;

    RobotColor myColor_;

    Arguments cArgs_;

    ConsoleManager cmanager_;

    // Create the data used to run the automate
    //Data data_;

    // Create the automate associated to the robot
    Automate automate_;

    //id of the robot
    std::string id_;

    //DATA
    bool empty_ = true;
    int useExternalEncoder_ = 1;
    int skipSetup_ = 0;
    bool end90s_ = false;
    bool waitForInit_ = false;
    std::string strategy_ = "all";

public:
#ifdef SIMU
    int CLEF_REQUETES = 0x00012345;

    struct msgform2
    {
        long mtype;
        char mtext[512];
    } msg_ipc;
#endif

    //Action => RobotElement
    Actions * actions_default;

    //Asserv => asservissement
    Asserv * asserv_default;

    SvgWriter * svg_;

    //IA
    //TODO IA

    /*!
     * \brief Constructeur de la classe.
     */
    Robot();

    /*!
     * \brief Destructor.
     */
    virtual ~Robot()
    {
    }

    //DATA
    bool end90s() const
    {
        return this->end90s_;
    }
    void end90s(bool end)
    {
        this->end90s_ = end;
    }

    bool isEmpty() const
    {
        return this->empty_;
    }
    void isEmpty(bool empty)
    {
        this->empty_ = empty;
    }

    std::string strategy() const
    {
        return this->strategy_;
    }
    void strategy(std::string str)
    {
        this->strategy_ = str;
    }

    int useExternalEncoder() const
    {
        return this->useExternalEncoder_;
    }
    void useExternalEncoder(int useEncoder)
    {
        this->useExternalEncoder_ = useEncoder;
    }

    int skipSetup() const
    {
        return this->skipSetup_;
    }
    void skipSetup(int skip)
    {
        this->skipSetup_ = skip;
    }

    bool waitForInit() const
    {
        return this->waitForInit_;
    }
    void waitForInit(bool init)
    {
        this->waitForInit_ = init;
    }

    ///DATA

    std::string getID()
    {
        return id_;
    }

    inline SvgWriter& svgw()
    {
        SvgWriter & r_svg = *svg_;
        return r_svg;
    }

    void svgPrintPosition();

    void svgPrintEndOfFile();

    void operator=(Robot const&); // Don't implement

    inline ConsoleManager& getConsoleManager()
    {
        ConsoleManager& r_cmanager = cmanager_;
        return r_cmanager;
    }

    inline Arguments& getArgs()
    {
        Arguments& r_cargs = cArgs_;
        return r_cargs;
    }

    /*!
     * \brief Cette methode retourne l'objet de manipulation du chronometer.
     * \return Le chronometer.
     */
    utils::Chronometer & chrono()
    {
        return chrono_;
    }

    /*!
     * \brief Retourne la couleur du robot.
     */
    RobotColor getMyColor() const
    {
        return myColor_;
    }

    /*!
     * \brief Enregistre la couleur du robot.
     */
    void setMyColor(RobotColor color)
    {
        this->myColor_ = color;
    }

    void configureDefaultConsoleArgs();

    /*!
     * \brief Parse console parameters (console for tests or main program)..
     */
    void parseConsoleArgs(int argc, char** argv);

    /*!
     * \brief Start the robot (console for tests or main program)..
     */
    virtual void begin(int argc, char** argv);

    virtual void stopAll();

    virtual void freeMotion();

};

#endif
