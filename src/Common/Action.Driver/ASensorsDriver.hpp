#ifndef ASENSORSDRIVER_HPP_
#define ASENSORSDRIVER_HPP_

#include <string>
#include <vector>

#include "../Utils/PointerList.hpp"

class RobotPos {
public:
    int nbDetectedBots; //nb de position de robot detectée (dupliqué dans chaque)
    int x; //millimetres
    int y; //millimetres
    float theta_deg; //degre // angle de déplacement par rapport à l'avant derniere position
    int d; //dist en mm à partir du robot

    RobotPos(int x_, int y_, float a_, int d_, int nb) {
        x = x_;
        y = y_;
        theta_deg = a_;
        d = d_;
        nbDetectedBots = nb;
    }
    //void show() { std::cout<<x<<std::endl; }
};

class ASensorsDriver {

public:

    /*!
     * \brief Type associé aux stockages des tests.
     */
    typedef std::vector<RobotPos> bot_positions;

    /*!
     * \brief ASensorsDriver instance creation.
     */
    static ASensorsDriver * create(std::string botName);


    virtual bool is_connected() = 0;

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

    virtual int getAnalogPinData() = 0;

    /*!
     * \brief Destructor.
     */
    virtual ~ASensorsDriver() {
    }

protected:

    /*!
     * \brief Constructor.
     */
    ASensorsDriver() {
    }

};

#endif
