// Arguments.h: interface for the Arguments class.
//
// (C) 2001 NOVACOM GmbH, Berlin, www.novacom.net
// Author: Patrick Hoffmann, 03/21/2001
// http://www.codeproject.com/Articles/1242/Managing-argc-and-argv-with-STL-in-a-simple-way
//
// Modified by pmx
//////////////////////////////////////////////////////////////////////

#ifndef COMMON_ARGUMENTS_HPP_
#define	COMMON_ARGUMENTS_HPP_

#include <functional>
#include <iostream>
#include <map>
#include <string>
#include <vector>

using namespace std;

#ifndef TCHAR
typedef char TCHAR;
#define _T(a) a
typedef string tstring;
#else
typedef basic_string<TCHAR> tstring;
#endif

inline ostream &operator<<(ostream& ostr, tstring& tstr)
{
    ostr << tstr.c_str();
    return ostr;
}

class Arguments
{
    tstring m_strCommandName;
    tstring m_strDescription;
    tstring m_strOptionmarkers;

public:
    static tstring UnknownArgument;

    class Option;

    class Argument
    {
        friend class Arguments;
        friend class Option;

        tstring m_strName;
        tstring m_strDescription;
        tstring m_strValue;
        tstring m_strDefault;
        bool m_bOptional;

    public:
        Argument(tstring strName, tstring strDescription = _T(""), tstring strDefault = _T(""));
    };

    typedef vector<Argument> ArgVector;

    class Option
    {
        friend class Arguments;
        static Option Empty;

        TCHAR m_chName;
        ArgVector m_vArguments;
        tstring m_strDescription;
        bool m_bSet;

    public:
        Option(TCHAR chName, tstring strDescription = _T(""));
        bool addArgument(tstring strName, tstring strDescription = _T(""), tstring strDefault = _T(""));
        tstring &operator[](int n);
        tstring &operator[](tstring strArgumentName);
        tstring &operator[](const TCHAR *pszArgumentName);
        operator bool();
        void set(bool bSet = true);
        tstring getName();
    };

private:
    typedef std::map<TCHAR, Option, less<TCHAR>, allocator<std::map<TCHAR, Option>::value_type > > OptionMap;

    OptionMap m_mOptions;
    ArgVector m_vArguments;

public:
    bool isOption(TCHAR chOptionName);
    bool usage();
    bool addOption(TCHAR chOptionName, tstring strDescription = _T(""));
    bool addOption(Option &option);
    bool addArgument(tstring strName, tstring strDescription = _T(""), tstring strDefault = _T(""));
    bool parse(int argc, TCHAR* argv[], bool stopWithErrors = true);
    void setDescription(tstring desc);

    tstring &operator[](int n);
    tstring &operator[](tstring strArgumentName);
    Option &operator[](TCHAR chOptionName);

    Arguments(tstring strCommandName, tstring strDescription = _T(""), tstring strOptionmarkers = _T("-/"));
    virtual ~Arguments();
};

//#pragma warning (disable : 4786)

#endif
