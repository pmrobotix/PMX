/*!
 * \file
 * \brief Définition de la classe Logger.
 */

#ifndef LOGS_LOGGER_HPP_
#define LOGS_LOGGER_HPP_


#include <sstream>
#include <string>

#include "Level.hpp"

namespace logs
{
    class Logger;
    
    /*!
     * \brief Classe de gestion des flux de sortie pour les logs.
     */
    class Appender
    {
    protected:

        /*!
         * \brief Constructeur de la classe.
         */
        Appender()
        {}
        
    public:
        
        /*!
         * \brief Destructeur de la classe.
         */
        virtual ~ Appender()
        {}
        
        /*!
         * \brief Méthode générique de trace d'un message.
         * \param logger
         *        Logger de référence du message.
         * \param level
         *        Niveau de référence du message.
         * \param message
         *        Message a tracer.
         */
        virtual void writeMessage(const logs::Logger & logger, const logs::Level & level, const std::string & message)=0;
    };

    /*!
     * \brief Classe de gestion des traces de l'application.
     *
     * Cette classe assure la génération des traces de l'application. L'obtention
     * d'une instance de cette classe doit toujours être faite via la méthode
     * LoggerFactory::logger().
     */
    class Logger
    {
    public:
        /*!
         * \brief Classe de délégation pour faciliter la création des traces.
         *
         * Cette classe est utilisée comme conteneur du message lors de sa
         * création via l'utilisation de l'opérateur << et des méthodes
         * Logger::debug(), Logger::info(), Logger::warn(), Logger::error().
         *
         * \warning Tous les messages construit avec cette classe doivent
         * se terminer par l'appel à la méthode LoggerBuffer::end() qui peut
         * être aussi réaliser par l'appel à la méthode
         * logs::end(LoggerBuffer).
         *
         * \sa Logger::info()
         */
        class LoggerBuffer
        {
        private:
            
            /*!
             * \brief Le logger de référence.
             */
            const Logger & logger_;
            
            /*!
             * \brief Le niveau de référence pour ce message.
             */
            const Level & level_;
            
            /*!
             * \brief Buffer pour la construction du message.
             */
            std::ostringstream * stream_;
            
        public:
            
            /*!
             * \brief Constructeur de la classe.
             * \param logger
             *        Le logger de référence.
             * \param level
             *        Le niveau de référence pour le message à construire.
             */
            LoggerBuffer(const Logger & logger, const Level & level);
            
            /*!
             * \brief Destructeur de la classe.
             */
            virtual ~ LoggerBuffer();
            
            /*!
             * \brief Cette méthode valide le message en cours de construction
             * et le transmet au Logger de référence.
             */
            inline void end()
            {
                if (stream_ != NULL)
                {
                    logger_.writeMessage(level_, stream_->str());
                    delete stream_;
                    stream_ = NULL;
                }
            }
            
            /*!
             * \brief Cet opérateur permet d'utiliser cette classe comme un flux
             * de sortie standard.
             *
             * Cette méthode générique retransmet, si nécessaire, ses infos à
             * l'opérateur \c << de la classe \c std::ostringstream.
             *
             * \param value La valeur a ajouter au message.
             * \return Un lien vers le \c LoggerBuffer pour permettre le
             * chainage des opérateurs \c <<.
             */
            template<class A>
            LoggerBuffer & operator << (const A & value)
            {
                if (stream_ != NULL)
                {
                    (*stream_) << value;
                }
                
                return *this;
            }
            
            /*!
             * \brief Cet opérateur permet à la méthode logs::end() de remplir
             * son role et d'être appelée.
             *
             * \param f
             *        Pointeur vers la fonction.
             * \return Un lien vers le \c LoggerBuffer pour permettre le
             * chainage des opérateurs \c <<.
             */
            LoggerBuffer & operator << (void (*f)(LoggerBuffer &))
            {
                (*f)(*this);
                return *this;
            }
        };
        
    private:
        
        /*!
         * \brief Niveau de référence de ce Logger.
         *
         * Tous les messages dont le niveau est inférieur à ce niveau ne sont pas tracés.
         */
        const Level & level_;
        
        /*!
         * \brief Nom associé à ce Logger.
         */
        std::string name_;
        
        /*!
         * \brief Appender associé.
         */
        Appender & appender_;
        
    public:
        
        /*!
         * \brief Constructeur de la classe.
         * \param level
         *        Niveau de référence du Logger.
         * \param name
         *        Nom de référence du Logger.
         * \param appender
         *        Appender associé.
         */
        Logger(const Level & level, const std::string & name, Appender & appender);
        
        /*!
         * \brief Constructeur de la classe par copie des infos d'un autre logger.
         * \param parent
         *        Logger de référence.
         * \param name
         *        Nom du Logger.
         */
        Logger(const Logger & parent, const std::string & name);
        
        /*!
         * \brief Destructeur de la classe.
         */
        inline virtual ~ Logger()
        {}
        
        /*!
         * \brief Méthode générique de trace d'un message.
         * \param level
         *        Niveau de référence du message.
         * \param message
         *        Message a tracer.
         */
        void writeMessage(const logs::Level & level, const std::string & message) const;
        
        /*!
         * \brief Cette méthode vérifie si le logger est actif pour un niveau
         * donné.
         *
         * \param level
         *        Niveau de référence.
         * \return \c true si le logger est actif pour le niveau donné.
         */
        inline bool isActive(const logs::Level & level) const
        {
            return level >= level_;
        }
        
        /*!
         * \brief Cette méthode retourne le nom associé au Logger.
         */
        inline const std::string & name() const
        {
            return name_;
        }
        
        /*!
         * \brief Cette méthode retourne le niveau associé au Logger.
         */
        inline const logs::Level & level() const
        {
            return level_;
        }
        
        /*!
         * \brief Cette méthode retourne l'appender au Logger.
         */
        inline const logs::Appender & appender() const
        {
            return appender_;
        }
        
        /*!
         * \name Méthodes pour les textes simples
         * \{
         */
        
        /*!
         * \brief Trace un message de niveau DEBUG.
         * \param message
         *        Message de référence.
         */
        inline void debug(const std::string & message) const
        {
            writeMessage(Level::DEBUG, message);
        }
        
        /*!
         * \brief Trace un message de niveau INFO.
         * \param message
         *        Message de référence.
         */
        inline void info(const std::string & message) const
        {
            writeMessage(Level::INFO, message);
        }
        
        /*!
         * \brief Trace un message de niveau WARN.
         * \param message
         *        Message de référence.
         */
        inline void warn(const std::string & message) const
        {
            writeMessage(Level::WARN, message);
        }
        
        /*!
         * \brief Trace un message de niveau ERROR.
         * \param message
         *        Message de référence.
         */
        inline void error(const std::string & message) const
        {
            writeMessage(Level::ERROR, message);
        }
        
        /*!
         * \}
         *
         * \name Méthodes pour la construction de messages complexes.
         * \{
         */
        
        /*!
         * \brief Création d'un flux de construction pour les messages de type
         * DEBUG.
         * \return Le flux de construction devant se terminer par \code
         * << logs::end \endcode
         * \sa Logger::info()
         */
        inline LoggerBuffer debug() const
        {
            return LoggerBuffer(*this, Level::DEBUG);
        }
        
        /*!
         * \brief Création d'un flux de construction pour les messages de type
         * INFO.
         *
         * Cette méthode initialise une instance de \c LoggerBuffer pour
         * permettre la création d'un message comme pour un flux de sortie.
         *
         * \return Le flux de construction devant se terminer par \code
         * << logs::end \endcode
         */
        inline LoggerBuffer info() const
        {
            return LoggerBuffer(*this, Level::INFO);
        }
        
        /*!
         * \brief Création d'un flux de construction pour les messages de type
         * WARN.
         * \return Le flux de construction devant se terminer par \code
         * << logs::end \endcode
         * \sa Logger::info()
         */
        inline LoggerBuffer warn() const
        {
            return LoggerBuffer(*this, Level::WARN);
        }
        
        /*!
         * \brief Création d'un flux de construction pour les messages de type
         * ERROR.
         * \return Le flux de construction devant se terminer par \code
         * << logs::end \endcode
         * \sa Logger::info()
         */
        inline LoggerBuffer error() const
        {
            return LoggerBuffer(*this, Level::ERROR);
        }
        /*!
         * \}
         */
    };
    
    /*!
     * \brief Cette méthode permet de finir les flux de construction par la
     * notation \code << logs::end; \endcode.
     * \param buffer
     *        Le buffer de référence
     *
     * L'utilisation de cette méthode repose sur les concepts avancés de C++ et
     * en particulier le fait que lors de la compilation cette méthode sera
     * effectivement appelé comme si le code était ;
     * \code logs::end(logger.info() << "mon message"); \endcode
     *
     * Bien que ce concept puisse paraitre étrange, c'est sur cette même
     * fonctionnalité que std::endl est basée.
     */
    void end(logs::Logger::LoggerBuffer & buffer);

}

#endif
