/*
 * Error.cpp
 *
 *  Created on: Sep 9, 2013
 *      Author: alexandru
 */

#include "astox.h"


namespace astox {


    int Error::RANGE_ERROR = BG_BLUE_1;
    int Error::PARSING_ERROR = BG_YELLOW_2;
    int Error::ASSIGNMENT_ERROR = BG_RED_1;

    Error::Error(int gravity, std::string message, int lineNumber, const char * fileName) {
        using namespace std;
        valueType = ASTOX_VALUE_CUSTOM;
        //	astox_console_write(gravity, "");

        //fprintf(stderr,  "ERROR:\n    GRAVITY: %i\n    MESSAGE: %s\n    STATUS CODE:%i\n    FILE: %s", gravity, message.c_str(), lineNumber, fileName);
        astox_console_write(gravity, "ERR #%i %s LINE: %i FILE: %s\n", gravity, message.c_str(), lineNumber, fileName);



        fprintf(stderr, "ERROR:\n    GRAVITY: %i\n    MESSAGE: %s\n    STATUS CODE:%i\n    FILE: %s\n", gravity, message.c_str(), lineNumber, fileName);



        _gravity = gravity;
        _message = message;
        _code = lineNumber;
        _fileName = fileName;
        string fileNameParsed = DBG_FN(fileName);
        if (fileNameParsed.at(0) == ASTOX_OS_SEPARATOR) {
            fileNameParsed.erase(fileNameParsed.begin());
        }

        size_t poz = fileNameParsed.find(".");
        if (poz != string::npos) {
            fileNameParsed = fileNameParsed.substr(0, poz);
        }
        std::transform(fileNameParsed.begin(), fileNameParsed.end(), fileNameParsed.begin(), ::tolower);

        objectList["gravity"] = Number::New(gravity);
        objectList["message"] = new String(message);
        objectList["code"] = Number::New(lineNumber);
        objectList["_failure"] = new String(fileNameParsed);

        std::stringstream ss;
        ss << lineNumber;
        _what = "Error #";
        _what.append(ss.str());
        _what.append(" ");
        _what.append(message);
        _what.append(" at [");
        _what.append(fileNameParsed);
        _what.append(".class]");
    }

    const char* Error::what() const throw () {
        return _what.c_str();
    }

    Error::~Error() throw () {

    }

    std::string Error::toString() {
        return _what;
    };

    bool RegExp::_regexMapCreated = false;

    std::map<char, AstoxRegexFlag> RegExp::getRegexMap() {
        std::map<char, AstoxRegexFlag> r;
        if (!_regexMapCreated) {
            r['g'] = JSREG_GLOBAL;
            r['i'] = JSREG_IGNORECASE;
            r['m'] = JSREG_MULTILINE;
            _regexMapCreated = true;
        }

        return r;
    };

    RegExp::RegExp() {
        valueType = ASTOX_VALUE_REGEXP;
        flags = JSREG_NONE;
    }

    RegExp::RegExp(std::string data, AstoxRegexFlag pflags) {
        flags = pflags;
        valueType = ASTOX_VALUE_REGEXP;
        regexStrValue = data;
    }

    std::string RegExp::toString() {
        return regexStrValue;
    };

    RegExp * RegExp::cast(Value * value) {
#ifdef ASTX_VALUE_STATIC_CAST
        return static_cast<RegExp *> (value);
#else
        return dynamic_cast<RegExp *> (value);
#endif
    };

    AstoxRegexFlag RegExp::digestMIG(std::string &in, bool scanStr, std::string allowedChars) {

        if (in == "i") {
            return JSREG_IGNORECASE;
        }
        if (in == "g") {
            return JSREG_GLOBAL;
        }
        if (in == "m") {
            return JSREG_MULTILINE;
        }

        if (in == "im" || in == "mi") {
            return JSREG_IGNORECASE | JSREG_MULTILINE;
        }
        if (in == "ig" || in == "gi") {
            return JSREG_IGNORECASE | JSREG_GLOBAL;
        }
        if (in == "mg" || in == "gm") {
            return JSREG_MULTILINE | JSREG_GLOBAL;
        }

        if (in == "img" || in == "igm" || in == "gim" || in == "gmi" || in == "mgi" || in == "mig") {
            return JSREG_IGNORECASE | JSREG_MULTILINE | JSREG_GLOBAL;
        }

        return JSREG_NONE;
    };

    AstoxRegexFlag RegExp::strIsRegexFormat(std::string input, int &plastPoz) {
        using namespace std;
        if (input.size() > 2) {
            if (input.at(0) == '/') {


                size_t lastPoz = input.find_last_of('/');


                if (lastPoz != string::npos && lastPoz != 0) {
                    plastPoz = lastPoz;
                    string afterLast = input.substr(lastPoz + 1, input.size());
                    //				cout << "afterLast = " << afterLast << endl;

                    if (!afterLast.empty()) {
                        //					cout << " scan afterlast" << endl;
                        return digestMIG(afterLast);
                    }
                    else {
                        //					cout << " BASIC REGEX " << input << endl;
                        return JSREG_BASIC;
                    }
                }
                else {
                    //				cout << " NONE REGEX " << input << endl;
                    return JSREG_NONE;
                }
            }
        }
        //	cout << " NOPE REGEX " << input << endl;
        return JSREG_NONE;
    };

    AstoxRegexFlag RegExp::getFlags() {
        return flags;
    };

    Value * RegExp::call(std::string &args, Function *& context) {
        using namespace std;
        regexStrValue = args;
        //	cout << " called with " << args << endl;
        int lastPoz = 0;
        AstoxRegexFlag flags = RegExp::strIsRegexFormat(args, lastPoz);

        if (flags != JSREG_NONE) {
            regexStrValue = regexStrValue.substr(1, lastPoz - 1);
            //		cout <<  "[" <<regexStrValue << "] input is regex, flags: " << flags << endl;
        }
        else {
            cout << args << " IS NOT REGEX STR" << endl;
        }
        return new RegExp(regexStrValue, flags);
    };

} /* namespace astox */
