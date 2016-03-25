#ifndef LEDBAR_HPP_
#define LEDBAR_HPP_

#include <unistd.h>
#include <string>

#include "../../Log/LoggerFactory.hpp"
#include "../../Thread/Mutex.hpp"
#include "../Action.Driver/ALedDriver.hpp"
#include "../Utils/Chronometer.hpp"
#include "../Utils/Macro.hpp"
#include "AActionsElement.hpp"
#include "IAction.hpp"

/*!
 * \brief Enumération des libellés des actions de la barre de leds.
 */
enum LedBarActionName
{
	/*!
	 * \brief Libellé de l'action permettant d'allumer/eteindre une led.
	 */
	LEDBARSET,
	/*!
	 * \brief Libellé de l'action permettant un clignotement à la "K2000".
	 */
	LEDBARK2MIL,
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
	LEDBARFLASHVALUE,
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

class LedBar: public AActionsElement
{
private:

	/*!
	 * \brief Retourne le \ref Logger associé à la classe \ref LedBar.
	 */
	static inline const logs::Logger & logger()
	{
		static const logs::Logger & instance = logs::LoggerFactory::logger("LedBar");
		return instance;
	}

	/*!
	 * Number of led in the LedBar.
	 */
	int nbLed_;

	/*!
	 * \brief Permet de stopper l'action et qu'elle se termine à la prochaine itération des actions.
	 */
	bool actionStopped_;

	/*!
	 * \brief Permet de savoir si une action est en cours.
	 */
	bool actionRunning_;

	/*!
	 * \brief Position de la led.
	 */
	ushort position_;

	/*!
	 * \brief Color de la led a appliquer.
	 */
	LedColor color_;

	/*!
	 * \brief Nombre de clignotement à faire.
	 */
	uint nb_;

	/*!
	 * \brief Temps de pause (en us).
	 */
	uint timeus_;

	/*!
	 * \brief Première Valeur de clignotement.
	 */
	uint hex_;

	/*!
	 * \brief 2ème valeur de clignotement.
	 */
	uint hexNext_;

	utils::Mutex mutex_;

public:

	/*!
	 * \brief ID du robot.
	 */
	std::string botId_;

	ALedDriver* leddriver;

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

	inline void stop(bool value)
	{
		//mutex_.lock();
		this->actionStopped_ = value;
		//mutex_.unlock();
	}
	inline bool stop() const
	{
		return actionStopped_;
	}

	inline void stopAndWait(bool value)
	{
		//mutex_.lock();
		this->actionStopped_ = value;
		while (this->actionRunning_)
		{
			usleep(1000);
		}
		//mutex_.unlock();
	}

	inline void actionRunning(bool value)
	{
		this->actionRunning_ = value;

	}
	inline bool actionRunning() const
	{
		return actionRunning_;
	}

	inline void position(ushort value)
	{
		this->position_ = value;
	}
	inline ushort position() const
	{
		return position_;
	}

	inline void color(LedColor value)
	{
		this->color_ = value;
	}
	inline LedColor color() const
	{
		return color_;
	}

	inline void hexValue(uint value)
	{
		this->hex_ = value;
	}
	inline uint hexValue() const
	{
		return hex_;
	}

	inline void hexValueNext(uint value)
	{
		this->hexNext_ = value;
	}
	inline uint hexValueNext() const
	{
		return hexNext_;
	}

	inline void timeus(uint value)
	{
		this->timeus_ = value;
	}
	inline uint timeus() const
	{
		return timeus_;
	}

	inline void nb(uint value)
	{
		this->nb_ = value;
	}
	inline uint nb() const
	{
		return nb_;
	}

	inline uint nbLed() const
	{
		return nbLed_;
	}

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
	inline void setOn(int position)
	{
		this->set(position, LED_GREEN);
	}

	/*!
	 * \brief Désactive une led spécifique.
	 * \param position La position de la led.
	 */
	inline void setOff(int position)
	{
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
	 * \brief Lance l'action de changer le statut d'une led.
	 *
	 * \param position La position de la led (de 0 à 7).
	 * \param status Le statut de la led.
	 */
	void startSet(ushort position, LedColor color = LED_GREEN);

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
	void startFlashValue(uint hexValue);

	/*!
	 * \brief Lance l'action de faire alterner les leds selon les valeurs hexValue et hexValueNext.
	 *
	 */
	void startAlternate(uint nb, uint timeus, uint hexValue, uint hexValueNext, LedColor color =
			LED_GREEN, bool wait = false);

	/*!
	 * \brief Lance l'action de faire clignoter toutes les leds nb fois tous les timeus.
	 */
	void startBlink(uint nb, uint timeus, LedColor color, bool wait);

	/*!
	 * \brief Lance l'action de faire clignoter une led nb fois tous les timeus.
	 */
	void startBlinkPin(uint nb, uint timeus, int position, LedColor color, bool wait);

	/*!
	 * \brief Lance l'action d'allumer alternativement les leds à la "K2000".
	 */
	void startK2mil(uint nb, uint timeus, LedColor color, bool wait);

	/*!
	 * \brief Active une led spécifique.
	 * \param position La position de la led (de 0 à 7).
	 */
	inline void startSetOn(ushort position)
	{
		this->startSet(position, LED_GREEN);
	}

	/*!
	 * \brief Désactive une led spécifique.
	 * \param position La position de la led (de 0 à 7).
	 */
	inline void startSetOff(ushort position)
	{
		this->startSet(position, LED_OFF);
	}
};

/*!
 * \brief Cette action permet de definir les actions concernant la barre de leds.
 *
 */
class LedBarAction: public IAction
{
private:

	/*!
	 * \brief Retourne le \ref Logger associé à la classe \ref LedBarAction.
	 */
	static const logs::Logger & logger()
	{
		static const logs::Logger & instance = logs::LoggerFactory::logger("LedBarAction");
		return instance;
	}

	/*!
	 * \brief Référence vers la barre de Leds associée.
	 */
	LedBar & ledBar_;

	/*!
	 * \brief libellé de l'action à exécuter.
	 */
	LedBarActionName action_;

	utils::Chronometer chrono_;

	long lastTime_;

	uint i_;

	uint j_;

	uint k_;

	bool inc_;

public:

	/*!
	 * \brief Constructeur de la classe.
	 * \param ledBar
	 *        Reference vers la LedBar associée.
	 */
	LedBarAction(LedBar & ledBar, LedBarActionName action);

	/*!
	 * \brief Destructeur de la classe.
	 */
	virtual inline ~LedBarAction()
	{
		logger().debug() << "~LedBarAction() for " << ledBar_.botId_ << logs::end;
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
		return "LedBarAction";
	}
};

#endif
