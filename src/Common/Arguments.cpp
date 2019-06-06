// Arguments.cpp: implementation of the Arguments class.
// Base on:
// (C) 2001 NOVACOM GmbH, Berlin, www.novacom.net
// Author: Patrick Hoffmann, 03/21/2001
//
//////////////////////////////////////////////////////////////////////

#include "Arguments.hpp"

#include <sys/types.h>
#include <unistd.h>
#include <iterator>
#include <utility>

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

using namespace std;

Arguments::Option Arguments::Option::Empty(_T('\0'));
tstring Arguments::UnknownArgument(_T("<UnKnOwN>"));

Arguments::Arguments(tstring strCommandName, tstring strDescription, tstring strOptionmarkers) :
        m_strCommandName(strCommandName), m_strDescription(strDescription), m_strOptionmarkers(strOptionmarkers)
{
}

Arguments::~Arguments()
{
}

bool Arguments::parse(int argc, TCHAR *argv[], bool stopWithErrors)
{
    if (m_strCommandName.empty())
        m_strCommandName = argv[0];

    uint nArg = 0;

    for (int i = 1; i < argc; i++) {
        tstring strArgument = argv[i];

        // Option...?
        if (m_strOptionmarkers.find(strArgument.substr(0, 1)) != tstring::npos) {
            TCHAR chOptionName = strArgument[1];

            OptionMap::iterator it = m_mOptions.find(chOptionName);

            if (it == m_mOptions.end()) {
                if (stopWithErrors) {
                    cerr << m_strCommandName << " error: Unknown option " << strArgument << "." << endl;
                    usage();
                    return false;
                }
            } else {
                it->second.m_bSet = true;

                i++;
                {
                    uint nNonOptionalArgs = 0;
                    {
                        for (ArgVector::iterator itOptArg = it->second.m_vArguments.begin();
                                itOptArg != it->second.m_vArguments.end(); itOptArg++) {
                            if (!itOptArg->m_bOptional)
                                nNonOptionalArgs++;
                        }
                    }

                    for (uint nOptArg = 0; nOptArg < it->second.m_vArguments.size(); i++, nOptArg++) {
                        if (i >= argc || m_strOptionmarkers.find(tstring(argv[i]).substr(0, 1)) != tstring::npos) {
                            if (nOptArg < nNonOptionalArgs) {

                                cerr << m_strCommandName << " error: Too few arguments for option " << strArgument
                                        << "." << endl;
                                usage();
                                return false;

                            } else {
                                break;
                            }
                        }

                        it->second.m_vArguments[nOptArg].m_strValue = argv[i];
                    }
                }
                i--;
            }
        } else	// ...oder Argument
        {
            if (nArg >= m_vArguments.size()) {
                //DO Nothing - let many arguments for further functional tests
//                if (stopWithErrors) {
//                    cerr << m_strCommandName << " error: Too much arguments. " << endl;
//                    usage();
//                    return false;
//                }
            } else
            {
                m_vArguments[nArg++].m_strValue = strArgument;
            }
        }
    }

    {
        uint nNonOptionalArgs = 0;
        {
            for (ArgVector::iterator it = m_vArguments.begin(); it != m_vArguments.end(); it++) {
                if (!it->m_bOptional)
                    nNonOptionalArgs++;
            }
        }

        if (nArg != 0 && nNonOptionalArgs != 0)
            if (nNonOptionalArgs >= nArg) {
                cerr << m_strCommandName << " error: Too few arguments." << endl;
                usage();
                return false;
            }
    }

    return true;
}

bool Arguments::addOption(TCHAR chOption, tstring strDescription)
{
    m_mOptions.insert(pair<TCHAR, Option>(chOption, Option(chOption, strDescription)));

    return true;
}

bool Arguments::addOption(Option &option)
{
    m_mOptions.insert(pair<TCHAR, Option>(option.m_chName, option));

    return true;
}

bool Arguments::usage()
{
    usleep(100000); //pour laisser les messages d'erreurs s'afficher auparavant.
    cerr << "Usage: " << m_strCommandName;

    for (OptionMap::iterator it = m_mOptions.begin(); it != m_mOptions.end(); it++) {
        cerr << " [" << m_strOptionmarkers[0] << it->second.getName();

        for (ArgVector::iterator itArg = it->second.m_vArguments.begin(); itArg != it->second.m_vArguments.end();
                itArg++) {
            if (itArg->m_bOptional)
                cerr << " [" << itArg->m_strName << "]";
            else
                cerr << " " << itArg->m_strName;
        }
        cerr << "]";
    }

    for (ArgVector::iterator itArg = m_vArguments.begin(); itArg != m_vArguments.end(); itArg++) {
        if (itArg->m_bOptional)
            cerr << " [" << itArg->m_strName << "]";
        else
            cerr << " " << itArg->m_strName;
    }

    cerr << endl;

    if (!m_mOptions.empty())
        cerr << endl << "Options:" << endl;

    for (OptionMap::iterator it = m_mOptions.begin(); it != m_mOptions.end(); it++) {
        cerr << "\t" << m_strOptionmarkers[0] << it->second.getName() << "\t  " << it->second.m_strDescription << endl;

        for (ArgVector::iterator itArg = it->second.m_vArguments.begin(); itArg != it->second.m_vArguments.end();
                itArg++) {
            cerr << "\t " << itArg->m_strName << "\t= " << itArg->m_strDescription << endl;

            if (itArg->m_bOptional)
                cerr << "\t\t  optional argument (default='" << itArg->m_strDefault << "')" << endl;
        }
    }

    if (!m_vArguments.empty())
        cerr << endl << "Arguments:" << endl;

    for (ArgVector::iterator itArg = m_vArguments.begin(); itArg != m_vArguments.end(); itArg++) {
        cerr << "\t" << itArg->m_strName << "\t= " << itArg->m_strDescription << endl;

        if (itArg->m_bOptional)
            cerr << "\t\t  optional argument (default='" << itArg->m_strDefault << "')" << endl;
    }

    cerr << endl;

    cerr << m_strDescription << endl;

    return true;
}

Arguments::Option::Option(TCHAR chName, tstring strDescription) :
        m_chName(chName), m_strDescription(strDescription), m_bSet(false)
{
}

bool Arguments::addArgument(tstring strName, tstring strDescription, tstring strDefault)
{
    m_vArguments.push_back(Argument(strName, strDescription, strDefault));
    return true;
}

bool Arguments::Option::addArgument(tstring strName, tstring strDescription, tstring strDefault)
{
    m_vArguments.push_back(Argument(strName, strDescription, strDefault));
    return true;
}

Arguments::Argument::Argument(tstring strName, tstring strDescription, tstring strDefault) :
        m_strName(strName), m_strDescription(strDescription), m_strValue(strDefault), m_strDefault(strDefault), m_bOptional(
                !strDefault.empty())
{

}

bool Arguments::isOption(TCHAR chOptionName)
{
    OptionMap::iterator it = m_mOptions.find(chOptionName);

    if (it == m_mOptions.end())
        return false;
    else
        return it->second.m_bSet;
}

Arguments::Option::operator bool()
{
    return m_bSet;
}

void Arguments::Option::set(bool bSet)
{
    m_bSet = bSet;
}

tstring &Arguments::operator[](int n)
{
    return m_vArguments[n].m_strValue;
}

tstring &Arguments::operator[](tstring strArgumentName)
{
    for (ArgVector::iterator it = m_vArguments.begin(); it != m_vArguments.end(); it++) {
        if (it->m_strName == strArgumentName)
            return it->m_strValue;
    }

    return UnknownArgument;
}

tstring &Arguments::Option::operator[](int n)
{
    return m_vArguments[n].m_strValue;
}

tstring &Arguments::Option::operator[](const TCHAR *pszArgumentName)
{
    return operator[]((tstring) pszArgumentName);
}

tstring &Arguments::Option::operator[](tstring strArgumentName)
{
    for (ArgVector::iterator it = m_vArguments.begin(); it != m_vArguments.end(); it++) {
        if (it->m_strName == strArgumentName)
            return it->m_strValue;
    }

    return UnknownArgument;
}

Arguments::Option &Arguments::operator[](TCHAR chOptionName)
{
    OptionMap::iterator it = m_mOptions.find(chOptionName);

    if (it == m_mOptions.end())
        return Option::Empty;
    else
        return it->second;
}

tstring Arguments::Option::getName()
{
    tstring str = _T(" ");

    str[0] = m_chName;

    return str;
}

void Arguments::setDescription(tstring desc)
{
    m_strDescription = desc;
}
