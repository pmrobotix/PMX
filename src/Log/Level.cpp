/*!
 * \file
 * \brief Implémentation de la classe Level.
 */

#include "Level.hpp"

logs::Level logs::Level::DEBUG(5, "DEBUG");

logs::Level logs::Level::INFO(10, "INFO");

logs::Level logs::Level::WARN(15, "WARN");

logs::Level logs::Level::ERROR(20, "ERROR");

logs::Level logs::Level::ALL(0, "ALL");

logs::Level logs::Level::NONE(100, "NONE");
