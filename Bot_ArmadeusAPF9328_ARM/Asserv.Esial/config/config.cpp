#include "config.h"

#include <stddef.h>
#include <stdio.h>
#include <fstream>
#include <iterator>
#include <sstream>
#include <string>

using std::endl;

#define PARAM(type, nom) type Config::nom;
#include "params.h"

const Parameter Config::params[] = {
#   define PARAM(type, nom) Parameter(#nom, &Config::nom),
#   include "params.h"
};
/*
const PinName Config::pinNameList[] =
{
    (PinName) NULL, // p0 n'existe pas
    (PinName) NULL, // p1 n'existe pas
    (PinName) NULL, // p2 n'existe pas
    (PinName) NULL, // p3 n'existe pas
    (PinName) NULL, // p4 n'existe pas
         p5,  p6,  p7,
    p8,  p9,  p10, p11,
    p12, p13, p14, p15,
    p16, p17, p18, p19,
    p20, p21, p22, p23,
    p24, p25, p26, p27,
    p28, p29, p30
};*/

const Parameter *Config::getParam(std::string name)
{
    for (size_t i = 0; i < sizeof(Config::params) / sizeof(*Config::params); i++) {
        const Parameter *p = &(Config::params[i]);

        if (p->is(name))
            return p;
    }

    return NULL;
}

const Parameter* Config::parseConfigLine(std::string line, std::string& name, std::string &val)
{
    bool isName = true;

    for (std::string::const_iterator it = line.begin(); it != line.end(); ++it) {
        if (*it == ' ' || *it == '\n' || *it == '\r')
            continue;

        if (isName && *it == '=') {
            isName = false;
            continue;
        }

        if (isName)
            name.push_back(*it);
        else
            val.push_back(*it);
    }

    if (name.empty()) {
        return NULL;
    }

    const Parameter *p = Config::getParam(name);

    if (p == NULL) {
        printf("Attention Config: Le paramètre '%s' line '%s' n'existe pas !\r\n", name.c_str() , line.c_str());
        return NULL;
    }
    return p;
}

void Config::loadFile(const char *filename)
{
    //printf("Chargement de '%s'...\r\n", filename);

    std::ifstream file(filename);

    if (!file) {
        printf("Erreur : Impossible de charger le fichier '%s'\r\n", filename);
        return;
    }

    while (!file.eof()) {
        std::string line, name, val;
        getline(file, line);

        if (!line.empty() && line[0] != '#' && line[0] != '/') {
            const Parameter *p = parseConfigLine(line, name, val);
            if (p != NULL) {
                p->setFromString(val);
            }
        }
    }

    file.close();
}

void Config::saveToFile(const char *template_filename, const char *filename) {
    printf("Chargement de '%s'...\r\n", template_filename);
    std::ifstream tmpl(template_filename);

    if (!tmpl) {
        printf("Erreur : Impossible de charger le fichier modèle '%s'\r\n", template_filename);
        return;
    }

    printf("Sauvegarde dans '%s'...\r\n", filename);
    std::ofstream file(filename);

    if (!file) {
        printf("Erreur : Impossible d'ouvrir le fichier de sortie '%s'\r\n", filename);
        tmpl.close();
        return;
    }

    while (!tmpl.eof()) {
        std::string line, name, val;
        getline(tmpl, line);

        if (line.empty() || line[0] == '#' || line[0] == '/') {
            file << line << endl;
        } else {
            const Parameter *p = parseConfigLine(line, name, val);

            if (p != NULL) {
                file << name << " = " << p->toString() << endl;
            } else {
                file << line << endl;
            }
        }
    }

    tmpl.close();
    file.close();
}

std::string Config::dumpConfig() {
    std::ostringstream out;
    for (size_t i = 0; i < sizeof(Config::params) / sizeof(*Config::params); i++) {
        const Parameter *p = &(Config::params[i]);
        out << p->getName() << " = " << p->toString() << "\r" << endl;
    }
    return out.str();
}
