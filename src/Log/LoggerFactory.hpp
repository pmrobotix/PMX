/*!
 * \file
 * \brief Définition de la classe LoggerFactory.
 */

#ifndef LOGS_LOGGERFACTORY_HPP_
#define LOGS_LOGGERFACTORY_HPP_

#include <map>
#include <string>

#include "Logger.hpp"

namespace utils
{
class Mutex;
} /* namespace utils */


namespace logs
{
    /*!
     * \brief Cette classe assure l'initialisation du système de trace et son utilisation.
     *
     * L'initialisation du système de traces est réalisé par la méthode initialize().
     * Cette méthode ne possède pas d'implémentation et doit être redéfini pour
     * que le système puisse fonctionner et compiler.
     *
     * L'utilisation du système de traces se fait au travers de la méthode statique
     * logger(). Cette méthode retourne un logger en fonction d'un nom.
     *
     * Cette classe implémente un Design Pattern Singleton. L'instance unique de la
     * classe est obtenue grace à la méthode statique instance().
     *
     * \todo Documenter et coder la notion de hierarchie.
     */
    class LoggerFactory  
    {
    public:
        
        /*!
         * \brief Cette méthode statique retourne l'instance unique de la classe
         * LoggerFactory.
         * \return L'instance unique de la classe.
         */
        static LoggerFactory & instance()
        {
            static LoggerFactory instance;
            
            return instance;
        }
        
        /*!
         * \brief Cette méthode statique retourne un Logger en fonction de son nom.
         *
         * Si le logger demandé a été créé lors de l'initialisation du système de traces,
         * celui-ci sera retourné tel qu'il a été configuré. Sinon, le Logger par
         * défaut (rootLogger) sera retourné.
         *
         * \param name
         *        Nom du logger a retourner
         * \return Le logger associé au paramètre \c nom.
         */
        static const Logger & logger(const std::string & name);
        
    private:

        utils::Mutex *lock;

        /*!
         * \brief Map contenant tous les appenders disponibles en fonction de leur nom.
         */
        std::map < std::string, Appender * >appenders_;
        
        /*!
         * \brief Map contenant tous les loggers déjà créés en fonction de leur nom.
         */
        std::map < std::string, Logger * >loggers_;
        
        /*!
         * \brief Logger principal. Il définit le niveau de trace par défaut.
         */
        Logger * rootLogger_;
        
        /*!
         * \brief constructeur de la classe.
         */
        LoggerFactory();
        

        
        /*!
         * \brief Méthode d'initialisation du système.
         *
         * \todo Documenter l'initialisation en fonction des noms.
         */
         void initialize();
        
    protected:
        
        /*!
                * \brief Cette méthode ajoute un appender au système.
                *
                * Cette méthode doit être utilisée par l'implémentation de la méthode
                * \c initialize().
                *
                * \param name
                *        Le nom de l'appender à ajouter au système.
                * \param appender
                *        Le logger à ajouter au système.
                */
               void add(const std::string & name, Appender * appender);

               /*!
                * \brief Cette méthode ajoute un logger au système.
                *
                * Cette méthode doit être utilisée par l'implémentation de la méthode
                * \c initialize().
                *
                * \param logger
                *        Le logger à ajouter au système.
                */
               void add(Logger* logger);

               /*!
                * \brief Cette méthode ajoute un logger au système.
                *
                * Cette méthode doit être utilisée par l'implémentation de la méthode
                * \c initialize().
                *
                * \param level
                *        Le niveau associé au logger.
                * \param loggerName
                *        Le nom du logger à ajouter au système.
                * \param appenderName
                *        Le nom de l'appender à utilisé pour ce logger.
                */
               void add(const Level & level, const std::string & loggerName, const std::string & appenderName);

        /*!
         * \brief Cette méthode retourne un Appender en fonction de son nom.
         *
         * \param name
         *        Nom de l'appender à retourner
         * \return L'objet Appender associé au paramètre \c nom.
         */
        Appender * appender(const std::string & name);
        
    public:
        
        /*!
         * \brief Destructeur de la classe.
         */
        virtual ~ LoggerFactory();
        
        /*!
         * \brief Cette méthode retourne le Logger principal qui définit le
         * niveau de trace par défaut.
         * \return Le logger principal du système.
         */
        Logger * rootLogger() const
        {
            return rootLogger_;
        }
    };
}

#endif

