#include "LedBar.hpp"

#include <cmath>

#include "../../Thread/Thread.hpp"
#include "Actions.hpp"
#include "../../Common/Utils/json.hpp"

using namespace std;

LedBar::LedBar(std::string botId, Actions & actions, int nbLed) :
        AActionsElement(actions), botId_(botId), nbLed_(nbLed), running_(false),
                 a_requestToStop_(false), t_requestToStop_(false)
{
    leddriver_ = ALedDriver::create(botId, nbLed);
}

LedBar::~LedBar() {
    delete leddriver_;
}


void LedBar::stop(bool wait) {
    t_requestToStop_ = true;
    a_requestToStop_ = true;
    if (wait) {

        this->waiting(wait, 10000000);

        t_requestToStop_ = false;
        a_requestToStop_ = false;
    }
}

void LedBar::set(int pos, LedColor color) {
    leddriver_->setBit(pos, color);
    //telemetry log
    nlohmann::json j;
    j["pos"] = pos;
    j["color"] = color;
    logger().telemetry(j.dump());
}

void LedBar::flash(uint hexPosition, LedColor color) {
    leddriver_->setBytes(hexPosition, color);
    //telemetry log
    nlohmann::json j;
    j["hex"] = hexPosition;
    j["hexcolor"] = color;
    logger().telemetry(j.dump());
}

void LedBar::rainbow(uint nb, uint timeus) {
    running(true);
    for (uint i = 0; i < nb; i++) {
        for (int c = LED_BLACK; c < LED_OFF; c++) {
            for (int k = 0; k <= nbLed_ - 1; k++) {
                set(k, (LedColor) c);
                set(k, (LedColor) c);
            }
            utils::sleep_for_micros(timeus);
        }
        if (a_requestToStop_) break;
    }
    resetAll();
    running(false);
    a_requestToStop_ = false;
}

void LedBar::blink(uint nb, uint timeus, LedColor color) {
    running(true);
    for (uint i = 0; i < nb; i++) {
        resetAll();
        if (a_requestToStop_) break;
        if (i >= nb - 1) break;
        utils::sleep_for_micros(timeus);

        flashAll(color);
        utils::sleep_for_micros(timeus);
    }
    running(false);
    a_requestToStop_ = false;
}

void LedBar::resetAll() {
    flash(pow(2, nbLed_) - 1, LED_OFF);
}

void LedBar::flashAll(LedColor color) {
    flash(pow(2, nbLed_) - 1, color);
}

void LedBar::blinkPin(uint nb, uint timeus, int position, LedColor color) {
    logger().debug() << "blinkPin" << logs::end;
    running(true);
    for (uint i = 1; i <= nb; i++) {

        this->set(position, color);
        utils::sleep_for_micros(timeus);
        this->set(position, LED_OFF);
        if (a_requestToStop_) break;
        i++;
        if (i > nb) break;
        utils::sleep_for_micros(timeus);
    }
    running(false);
    a_requestToStop_ = false;
}

void LedBar::alternate(uint nb, uint timeus, uint beginVal, uint endVal, LedColor beginColor) {
    running(true);
    for (uint i = 1; i <= nb; i++) {

        flash(beginVal, beginColor);
        utils::sleep_for_micros(timeus);
        flash(endVal, LED_OFF);
        if (a_requestToStop_) break;
        i++;
        if (i > nb) break;
        utils::sleep_for_micros(timeus);
    }
    running(false);
    a_requestToStop_ = false;

}

void LedBar::k2mil(uint nb, uint timeus, LedColor color) {
    int j = -1;
    running(true);
    for (uint i = 1; i <= nb; i++) {
        if (a_requestToStop_) break;
        //inc
        for (int k = 0; k <= nbLed_ - 1; k++) {
            set(k, color);
            if (j >= 0) if (j != k) set(j, LED_OFF);
            j = k;
            utils::sleep_for_micros(timeus);
        }
        //dec
        for (int k = nbLed_ - 1; k >= 0; k--) {
            set(k, color);
            if (j != k) set(j, LED_OFF);
            j = k;
            utils::sleep_for_micros(timeus);
        }
    }
    //off
    set(0, LED_OFF);
    running(false);
    a_requestToStop_ = false;
}

void LedBar::startSet(uint index, LedColor color) {
    this->actions().addAction(new LedBarAction(*this, LEDBARSET, index, color));
}

void LedBar::startReset() {
    this->actions().addAction(new LedBarAction(*this, LEDBARRESET, 0, LedColor::LED_OFF));
}

void LedBar::startFlash() {
    this->actions().addAction(new LedBarAction(*this, LEDBARFLASH, 0, LedColor::LED_OFF));
}

void LedBar::startFlashValue(uint hexValue, LedColor color) {
    this->actions().addAction(new LedBarAction(*this, LEDBARFLASHVALUE, hexValue, color));
}

void LedBar::startTimerAlternate(uint nb, uint timeus, uint hexValue, uint hexValueNext, LedColor color, bool wait) {
    stop();
    this->running(true);
    this->actions().addTimer(new LedBarTimer(*this, LEDBARALTERNATE, timeus, nb, color, hexValue, hexValueNext));
    waiting(wait, timeus*nb/1000);
}

void LedBar::startTimerBlinkPin(uint nb, uint timeus, int position, LedColor color, bool wait) {
    stop();//pour eviter d'avoir 2 timers en parallele sur le ledbar
    this->running(true);
    this->actions().addTimer(new LedBarTimer(*this, LEDBARBLINKPIN, timeus, nb, color, position, 0));
    waiting(wait,timeus*nb/1000);
}

void LedBar::startTimerK2mil(uint nb, uint timeus, LedColor color, bool wait) {
    stop();
    this->running(true);
    this->actions().addTimer(new LedBarTimer(*this, LEDBARK2MIL, timeus, nb, color, 0, 0));
    waiting(wait, timeus*((nb+1)*(this->nbLed()+1))/1000);
}

void LedBar::waiting(bool wait, uint timeout_ms) {
    uint echap = 0;
    if (wait) {
        while (this->running_) {
            //logger().error() << "echap=" << echap<< logs::end;
            echap++;
            utils::sleep_for_micros(1000);
            if (echap > timeout_ms)
            {
                logger().error() << " waiting a timer indefinitely !!! so break it. " << logs::end;
                break;
            }
        }
    }
}

LedBarAction::LedBarAction(LedBar & ledBar, LedBarActionName action, uint pos, LedColor color) :
        ledBar_(ledBar), action_(action), inc_(true), position_(pos), color_(color)
{
}

bool LedBarAction::execute() {
    switch (this->action_) {
        case LEDBARSET:
            ledBar_.running(true);
            ledBar_.set(position_, color_);
            break;

        case LEDBARRESET:
            ledBar_.running(true);
            ledBar_.resetAll();
            break;

        case LEDBARFLASH:
            ledBar_.running(true);
            ledBar_.flashAll();
            break;

        case LEDBARFLASHVALUE:
            ledBar_.running(true);
            ledBar_.flash(position_, color_);
            break;

        default:
            logger().error() << "Bad execute command !!" << logs::end;

            break;
    }
    ledBar_.running(false);
    return false; //suppression de l'action si false
}



LedBarTimer::LedBarTimer(LedBar & ledbar, LedBarTimerName name, uint timeSpan_us, uint nb, LedColor color, uint hexValue, uint hexValueNext) :
        ledBar_(ledbar), timerAction_(name), timeus_(timeSpan_us), nb_(nb), color_(color), hex_(hexValue), hexNext_(hexValueNext)
, tmp_nb_current_(0), tmp_pos_current_(0), tmp_pos_inc_(true)
{
    name_ = name;
    this->init(name_, timeSpan_us);
}



void LedBarTimer::onTimer(utils::Chronometer chrono) {

    if (ledBar_.hasToStop()) {

        requestToStop_ = true;
        ledBar_.running(false);
        ledBar_.resetStop();
        //logger().error() << "1 onTimer ledBar_.hasToStop()... nb_current_=" << ledBar_.hasToStop() << " running=" << ledBar_.running() << logs::end;

    }
    else switch (this->timerAction_) {
        case LEDBARALTERNATE:
            tmp_nb_current_++;
            ledBar_.running(true);
            if (tmp_nb_current_ % 2) {
                ledBar_.flash(hex_, color_);
            }
            else {
                ledBar_.flash(hexNext_, color_);
            }

            break;

        case LEDBARK2MIL:

            ledBar_.running(true);

            ledBar_.set(tmp_pos_current_, color_);
            //tmp_nb_current_++;
            if (tmp_pos_inc_ && (tmp_pos_current_ - 1) >= 0) ledBar_.setOff(tmp_pos_current_ - 1);
            else if (!tmp_pos_inc_ && ((tmp_pos_current_ + 1) <= ledBar_.nbLed() - 1)) ledBar_.setOff(tmp_pos_current_ + 1);

            if (tmp_pos_current_ >= (ledBar_.nbLed() - 1)) {
                tmp_pos_current_ = ledBar_.nbLed() - 1;
                tmp_pos_inc_ = false;
            }
            if (tmp_pos_current_ <= 0) {
                tmp_pos_current_ = 0;
                tmp_pos_inc_ = true;
                tmp_nb_current_++;
            }

            if (tmp_pos_inc_) tmp_pos_current_++;
            else tmp_pos_current_--;

            break;

        case LEDBARBLINKPIN:
            tmp_nb_current_++;
            ledBar_.running(true);
            LedColor temp;
            if (tmp_nb_current_ % 2) temp = color_;
            else temp = LED_OFF;

            ledBar_.set(hex_, temp);
            break;

        default:
            ledBar_.running(false);
            logger().error() << "Bad execute command !!" << logs::end;
            break;
    }
    if (nb_ != 0) {
        if (tmp_nb_current_ > nb_) {
            ledBar_.stop(false);
        }
    }
    if (ledBar_.hasToStop()) {
        requestToStop_ = true;
        ledBar_.running(false);
        ledBar_.resetStop();
        //logger().error() << "2 onTimer ledBar_.hasToStop()... nb_current_=" << ledBar_.hasToStop() << " running=" << ledBar_.running() << logs::end;

    }
}
void LedBarTimer::onTimerEnd(utils::Chronometer chrono) {
    logger().debug() << "onTimerEnd...executing..." << logs::end;
    ledBar_.resetAll();
}

std::string LedBarTimer::info() {
    std::ostringstream oss;
    oss << "LedBarTimer [" << name() << "] for " << ledBar_.id();
    return oss.str();
}

