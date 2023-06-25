#ifndef ASENSORSDRIVER_HPP_
#define ASENSORSDRIVER_HPP_

#include <string>
#include <vector>

class Robot;

class RobotPos
{
public:
    int nbDetectedBots; //nb de position de robot detectée (dupliqué dans chaque)
    float x; // position x de l'ADV en millimetres
    float y; // position y de l'ADV en millimetres
    float theta_deg; //degre // angle de l'ADV par rapport à l'avant du robot (devant axe x = 0 degré, gauche (axe y) = >0deg, droite (axe y) = <0deg)
    float d; //dist en mm à partir du centre du robot jusquau centre du robot ADV

    RobotPos(int nb, float x_, float y_, float a_, float d_)
    {
        nbDetectedBots = nb;
        x = x_;
        y = y_;
        theta_deg = a_;
        d = d_;

    }
    //void show() { std::cout<<x<<std::endl; }
};

class ASensorsDriver
{

public:

    /*!
     * \brief Type associé aux stockages des tests.
     */
    typedef std::vector<RobotPos> bot_positions;

    /*!
     * \brief ASensorsDriver instance creation.
     */
    static ASensorsDriver* create(std::string botName, Robot * robot);

    virtual bool is_connected() = 0;
    /*!
     * \brief return true if communication is alive.
     */
    //virtual bool is_alive() = 0;

    /*!
     * \brief synchronise les données de la balise.
     * \return 0 if success, -1 if error.
     */
    virtual int sync() = 0;

    virtual bot_positions getvPositionsAdv() = 0;

    virtual int frontLeft() = 0;
    virtual int frontCenter() = 0;
    virtual int frontRight() = 0;

    virtual int backLeft() = 0;
    virtual int backCenter() = 0;
    virtual int backRight() = 0;

    virtual int rightSide() = 0;
    virtual int leftSide() = 0;

    virtual void displayNumber(int number) = 0;

    //virtual int getAnalogPinData() = 0; //deprecated

    /*!
     * \brief Destructor.
     */
    virtual ~ASensorsDriver()
    {
    }

protected:

    /*!
     * \brief Constructor.
     */
    ASensorsDriver()
    {
    }

};

#endif
