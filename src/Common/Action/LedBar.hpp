#ifndef LEDBAR_HPP_
#define LEDBAR_HPP_

#include <sys/types.h>
#include <sstream>
#include <string>

#include "../../Log/LoggerFactory.hpp"
#include "../Action.Driver/ALedDriver.hpp"
#include "../Utils/Chronometer.hpp"
#include "../Action/ITimerPosixListener.hpp"
#include "AActionsElement.hpp"
#include "IAction.hpp"

/*!
 * \brief Enumération des libellés des actions de la barre de leds.
 */
enum LedBarTimerName {

    /*!
     * \brief Libellé de l'action permettant un clignotement à la "K2000".
     */
    LEDBARK2MIL,

    /*!
     * \brief Libellé de l'action permettant un clignotement de la barre de led.
     */
    LEDBARBLINK,
    /*!
     * \brief Libellé de l'action permettant un clignotement d'un pin de la barre de led.
     */
    LEDBARBLINKPIN,
    /*!
     * \brief Libellé de l'action permettant un clignotement alternatif sur 2 valeurs définies de la barre de led.
     */
    LEDBARALTERNATE
};

/*!
 * \brief Enumération des libellés des actions de la barre de leds.
 */
enum LedBarActionName {
    /*!
     * \brief Libellé de l'action permettant d'allumer/eteindre une led.
     */
    LEDBARSET,

    /*!
     * \brief Libellé de l'action permettant un reset de toute la barre de led.
     */
    LEDBARRESET,
    /*!
     * \brief Libellé de l'action permettant d'allumer toutes les leds.
     */
    LEDBARFLASH,
    /*!
     * \brief Libellé de l'action permettant d'allumer les leds par rapport à une valeur hexa.
     */
    LEDBARFLASHVALUE

};

class LedBar: public AActionsElement {
private:

    /*!
     * \brief Retourne le \ref Logger associé à la classe \ref LedBar.
     */
    static inline const logs::Logger & logger() {
        static const logs::Logger & instance = logs::LoggerFactory::logger("LedBar");
        return instance;
    }

    /*!
     * \brief ID du robot.
     */
    std::string botId_;

    /*!
     * Number of led in the LedBar.
     */
    int nbLed_;

//    /*!
//     * \brief Permet de stopper l'action et qu'elle se termine à la prochaine itération des actions.
//     */
//    bool actionStopped_;

    /*!
     * \brief Permet de savoir si une action ou un timer est en cours.
     */
    bool running_;

//    /*!
//     * \brief Position de la led.
//     */
//    ushort position_;
//
//    /*!
//     * \brief Color de la led a appliquer.
//     */
//    LedColor color_;

//    /*!
//     * \brief Nombre de clignotement à faire.
//     */
//    uint nb_;
//
//    /*!
//     * \brief Temps de pause (en us).
//     */
//    uint timeus_;

//    /*!
//     * \brief Première Valeur de clignotement.
//     */
//    uint hex_;
//
//    /*!
//     * \brief 2ème valeur de clignotement.
//     */
//    uint hexNext_;

    bool a_requestToStop_;

    bool t_requestToStop_;

    ALedDriver* leddriver_;

    /*!
     * \brief attente avec boucle d'echapement
     */
    void waiting(bool wait, uint limit_ms = 10000);

public:

    /*!
     * \brief Constructor.
     *
     * \param nb Number of leds in the LedBar.
     */
    LedBar(std::string botId, Actions & actions, int nb);

    /*!
     * \brief Destructor.
     */
    ~LedBar();

    std::string id() {
        return botId_;
    }

    void resetStop() {
        t_requestToStop_ = false;
        a_requestToStop_ = false;
    }

    bool hasToStop() {
        return t_requestToStop_;
    }

    void running(bool value) {
        this->running_ = value;

    }
    bool running() const {
        return running_;
    }

//    void position(ushort value) {
//        this->position_ = value;
//    }
//    ushort position() const {
//        return position_;
//    }
//
//    void color(LedColor value) {
//        this->color_ = value;
//    }
//    LedColor color() const {
//        return color_;
//    }
//
//    void hexValue(uint value) {
//        this->hex_ = value;
//    }
//    uint hexValue() const {
//        return hex_;
//    }
//
//    void hexValueNext(uint value) {
//        this->hexNext_ = value;
//    }
//    uint hexValueNext() const {
//        return hexNext_;
//    }
//
//    void timeus(uint value) {
//        this->timeus_ = value;
//    }
//    uint timeus() const {
//        return timeus_;
//    }
//
//    void nb(uint value) {
//        this->nb_ = value;
//    }
//    uint nb() const {
//        return nb_;
//    }

    int nbLed() const {
        return nbLed_;
    }

    /*!
     * \brief Donne l'ordre d'arreter et attend au moins 1 ms
     */
    void stop(bool wait = true);

    /*!
     * \brief Change le statut d'une led.
     * \param position La position de la led.
     * \param color La couleur de la led, 0:OFF, 1:GREEN, and optional 2:RED 3:ORANGE)
     */
    void set(int position, LedColor color);

    void rainbow(uint nb, uint timeus);

    /*!
     * \brief Active une led spécifique.
     * \param position La position de la led.
     */
    inline void setOn(int position) {
        this->set(position, LED_GREEN);
    }

    /*!
     * \brief Désactive une led spécifique.
     * \param position La position de la led.
     */
    inline void setOff(int position) {
        this->set(position, LED_OFF);
    }

    /*!
     * \brief Allume/eteint les leds définit par hexValue, max = 0xFF.
     */
    void flash(uint hexPosition, LedColor color = LED_GREEN);

    /*!
     * \brief Eteint toutes leds.
     */
    void resetAll();

    /*!
     * \brief Allume toutes leds.
     */
    void flashAll(LedColor color = LED_GREEN);

    /*!
     * \brief Clignote toutes les leds nb fois.
     */
    void blink(uint nb, uint timeus, LedColor color = LED_GREEN);

    /*!
     * \brief Clignote une led nb fois pendant timeus.
     */
    void blinkPin(uint nb, uint timeus, int position, LedColor color = LED_GREEN);

    /*!
     * \brief Clignote toutes les leds nb fois sur les 2 valeurs hex val1 et val2.
     */
    void alternate(uint nb, uint timeus, uint beginVal, uint endVal, LedColor color = LED_GREEN);

    /*!
     * \brief Clignote toutes les leds nb fois sur les 2 valeurs hex val1 et val2.
     */
    void k2mil(uint nb, uint timeus, LedColor color = LED_GREEN);

    /*!
     * \brief Lance l'action de changer le statut d'une led en tache parallele sur l'actionmanager.
     *
     * \param position La position de la led (de 0 à 7).
     * \param status Le statut de la led.
     */
    void startSet(uint position, LedColor color = LED_GREEN);

    /*!
     * \brief Active une led spécifique.
     * \param position La position de la led (de 0 à 7).
     */
    inline void startSetOn(uint position) {
        this->startSet(position, LED_GREEN);
    }

    /*!
     * \brief Désactive une led spécifique.
     * \param position La position de la led (de 0 à 7).
     */
    inline void startSetOff(uint position) {
        this->startSet(position, LED_OFF);
    }

    /*!
     * \brief Lance l'action d'éteindre toutes les leds.
     */
    void startReset();

    /*!
     * \brief Lance l'action d'allumer toutes les leds.
     */
    void startFlash();

    /*!
     * \brief Lance l'action d'allumer les leds selon la valeur hexValue.
     */
    void startFlashValue(uint hexValue, LedColor color);

    /*!
     * \brief Lance l'action de faire alterner les leds selon les valeurs hexValue et hexValueNext.
     *
     */
    void startTimerAlternate(uint nb, uint timeus, uint hexValue, uint hexValueNext, LedColor color = LED_GREEN, bool wait = false);

    /*!
     * \brief Lance l'action de faire clignoter toutes les leds nb fois tous les timeus.
     */
    void startTimerBlink(uint nb, uint timeus, LedColor color = LED_GREEN, bool wait = false);

    /*!
     * \brief Lance l'action de faire clignoter une led nb fois tous les timeus.
     */
    void startTimerBlinkPin(uint nb, uint timeus, int position, LedColor color = LED_GREEN, bool wait = false);

    /*!
     * \brief Lance l'action d'allumer alternativement les leds à la "K2000".
     */
    void startTimerK2mil(uint nb, uint timeus, LedColor color = LED_GREEN, bool wait = false);

};

/*!
 * \brief Cette action permet de definir les timers concernant la barre de leds.
 *
 */
class LedBarTimer: public ITimerPosixListener {
private:

    /*!
     * \brief Retourne le \ref Logger associé à la classe \ref LedBarTimer.
     */
    static const logs::Logger & logger() {
        static const logs::Logger & instance = logs::LoggerFactory::logger("LedBarTimer");
        return instance;
    }

    /*!
     * \brief Référence vers le ledbar.
     */
    LedBar & ledBar_;

    /*!
     * \brief libellé de l'action à exécuter sur le timer.
     */
    LedBarTimerName timerAction_;

    /*!
     * \brief Temps de pause (en us).
     */
    uint timeus_;

    /*!
     * \brief Nombre de clignotement à faire.
     */
    uint nb_;

    /*!
     * \brief Coleur de la led a setter.
     */
    LedColor color_;

    /*!
     * \brief Première Valeur de clignotement ou de position.
     */
    uint hex_;

    /*!
     * \brief 2ème valeur de clignotement.
     */
    uint hexNext_;

    /*!
     * \brief valeurs temporaire durant le clignotement.
     */
    uint tmp_nb_current_;
    int tmp_pos_current_;
    bool tmp_pos_inc_;

public:

    /*!
     * \brief Constructeur de la classe.
     * \param sensors
     *        Reference vers l'objet associée.
     * \param name
     *        Libellé du timer.
     * \param timeSpan_us
     *        Temps interval en microsecondes.
     * \param nb
     *        Le nombre d'execution à effectuer.
     * \param color
     *        La couleur de la led
     * \param hexValue
     *        La position ou la valeur en hexadecimale à appliquer
     * \param hexValueNext
     *        La 2eme valeur en hexa à appliquer
     */
    LedBarTimer(LedBar & ledBar, LedBarTimerName name, uint timeSpan_us, uint nb, LedColor color, uint hexValue, uint hexValueNext);

    /*!
     * \brief Destructeur de la classe.
     */
    virtual inline ~LedBarTimer() {
    }

    /*!
     * \brief fonction qui sera executer à chaque traitement du timer.
     */
    virtual void onTimer(utils::Chronometer chrono);

    /*!
     * \brief fonction qui sera executer en dernière action à faire pour ce timer.
     */
    virtual void onTimerEnd(utils::Chronometer chrono);

    /*!
     * \brief nom du timer.
     */
    virtual std::string info();

};

/*!
 * \brief Cette action permet de definir les actions concernant la barre de leds.
 *
 */
class LedBarAction: public IAction {
private:

    /*!
     * \brief Retourne le \ref Logger associé à la classe \ref LedBarAction.
     */
    static const logs::Logger & logger() {
        static const logs::Logger & instance = logs::LoggerFactory::logger("LedBarAction");
        return instance;
    }

    /*!
     * \brief Référence vers la barre de Leds associée.
     */
    LedBar & ledBar_;

    /*!
     * \brief libellé de l'action à exécuter et en cours.
     */
    LedBarActionName action_;

    /*!
     * \brief valeur temporaire d'incrementation ou decrémentation .
     */
    bool inc_;

    /*!
     * \brief Position de la led à setter.
     */
    ushort position_;

    /*!
     * \brief Coleur de la led a setter.
     */
    LedColor color_;

public:

    /*!
     * \brief Constructeur de la classe.
     * \param ledBar
     *        Reference vers la LedBar associée.
     */
    LedBarAction(LedBar & ledBar, LedBarActionName action, uint hex_pos, LedColor color);

    /*!
     * \brief Destructeur de la classe.
     */
    virtual inline ~LedBarAction() {
        //logger().debug() << "~LedBarAction() for " << ledBar_.id() << logs::end;
    }

    /*!
     * \brief Execution de l'action.
     */
    virtual bool execute();

    /*!
     * \brief Retourne la description du timer.
     */
    virtual inline std::string info() {
        std::ostringstream oss;
        oss << "LedBarAction for " << ledBar_.id();
        return oss.str();
    }
};

#endif
