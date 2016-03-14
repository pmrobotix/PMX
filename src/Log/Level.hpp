/*!
 * \file
 * \brief Définition de la classe Level.
 */

#ifndef LOGS_LEVEL_HPP_
#define LOGS_LEVEL_HPP_

#include <string>

namespace logs
{
    /*!
     * \brief Cette classe représente un niveau de trace.
     *
     * 4 niveaux sont définis par défaut :
     * - DEBUG : Pour les messages destinés à faciliter le debuggage de l'application ;
     * - INFO  : Pour les messages d'informations sur l'état de l'objet et du système ;
     * - WARN  : Pour les messages d'avertissements ;
     * - ERROR : Pour les messages d'erreurs.
     */
    class Level
    {
    public:
        
        /*!
         * \brief Niveau pour les messages destinés à faciliter le debuggage de l'application.
         */
        static Level DEBUG;
        
        /*!
         * \brief Niveau pour les messages d'informations sur l'état de l'objet
         * et du système.
         */
        static Level INFO;
        
        /*!
         * \brief Niveau pour les messages d'avertissements.
         */
        static Level WARN;
        
        /*!
         * \brief Niveau pour les messages d'erreurs.
         */
        static Level ERROR;
        
        /*!
         * \brief Niveau utilisé pour activer toutes les traces pour un Logger donné.
         */
        static Level ALL;
        
        /*!
         * \brief Niveau utilisé pour désactiver les traces pour un Logger donné.
         */
        static Level NONE;
        
    private:
        
        /*!
         * \brief Valeur associé au niveau.
         *
         * Cette valeur permet de comparer les niveaux de trace entre eux.
         * Plus le niveau est bas, moins le niveau et les messages associés
         * à ce niveau sont importants.
         */
        short value_;
        
        /*!
         * \brief Nom associé à ce niveau.
         *
         * Ce nom est utilisé pour l'affichage des messages.
         */
        std::string name_;
        
    public:
        
        /*!
         * \brief Constructeur de la classe.
         * \param value
         *        Valeur associé au niveau.
         * \param name
         *        Nom associée à ce niveau.
         */
        Level(short value, const std::string & name): value_(value),
                name_(name)
        {}
        
        /*!
         * \brief Destructeur de la classe.
         */
        virtual ~ Level()
        {}
        
        /*!
         * \brief Cette méthode retourne le nom de ce niveau.
         * \return Le nom de ce niveau.
         */
        inline const std::string & name() const
        {
            return name_;
        }
        
        /*!
         * \brief Comparateur de supériorité.
         * \param level
         *        Niveau à comparer.
         * \retval true
         *         Si l'objet courant est supérieur ou égal à \c level.
         * \retval false
         *         Si l'objet courantest inférieur à \c level.
         */
        inline bool operator >= (const Level & level) const
        {
            return value_ >= level.value_;
        }
        
        /*!
         * \brief Comparateur d'égalité.
         * \param level
         *        Niveau à comparer.
         * \retval true
         *         Si l'objet courant est égal à \c level.
         * \retval false
         *         Si les deux objets sont différents.
         */
        inline bool operator == (const Level & level) const
        {
            return value_ == level.value_;
        }
    };
}

#endif
