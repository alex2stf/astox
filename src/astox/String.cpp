/*
 * String.cpp
 *
 *  Created on: Jan 26, 2014
 *      Author: Alexandru
 */

#include "astox.h"

namespace astox {

    String * String::NaN = new String("NaN");

    std::map<std::string, String *> String::_memoizedStrings;

    String * String::New(std::string value) {
        if (_memoizedStrings.find(value) == _memoizedStrings.end()) {
            _memoizedStrings[value] = new String(value);
        }
        return _memoizedStrings[value];
    };

    String::String()
    : Value() {
        pvalue = "";
        valueType = ASTOX_VALUE_STRING;
        setGetters();
    }

    String::String(double type)
    : Value() {
        std::stringstream s;
        s << type;
        pvalue = s.str();
        valueType = ASTOX_VALUE_STRING;
        setGetters();
    }

    String::String(char charVal)
    : Value() {
        pvalue = charVal;
        valueType = ASTOX_VALUE_STRING;
        setGetters();
    };

    String::String(int type)
    : Value() {
        std::stringstream s;
        s << type;
        pvalue = s.str();
        valueType = ASTOX_VALUE_STRING;
        setGetters();
    }

    String::String(const char * value)
    : Value() {
        pvalue = value;
        valueType = ASTOX_VALUE_STRING;
        setGetters();
    }

    String::String(std::string value)
    : Value() {
        pvalue = value;
        valueType = ASTOX_VALUE_STRING;
        setGetters();
    }


#ifdef ASTX_ENABLE_QT

String::String(QString * qstring)
    : Value() {
        pvalue = qstring->toStdString();
        valueType = ASTOX_VALUE_STRING;
        setGetters();
    }

    String::String(QString qstring) {
        pvalue = qstring.toStdString();
        valueType = ASTOX_VALUE_STRING;
        setGetters();
    }
#endif

    String::~String() {
        // TODO Auto-generated destructor stub
    }

    Boolean * String::negate() {
        return Boolean::BFALSE;
    };

    Value * String::reverse() {
        std::reverse(pvalue.begin(), pvalue.end());
        return this;
    }

    std::string String::toString() {
        return pvalue;
    }

    astox::String * String::cast(Value * value) {
#ifdef ASTX_VALUE_STATIC_CAST
        return static_cast<String *> (value);
#else
        return dynamic_cast<String *> (value);
#endif
    }

    void String::append(astox::Value * data) {
        pvalue.append(data->toString());
    }

    /*ISSES:*/
    bool String::isString() {
        return true;
    }

    bool String::isNumber() {
        return false;
    }

    bool String::isBoolean() {
        return false;
    }

    bool String::isArray() {
        return false;
    }

    bool String::isObject() {
        return false;
    }

    bool String::isUndefined() {
        return false;
    }

    double String::toDouble() {
        if (stringAlg::stringIsNumber(pvalue)) {
            return atoi(pvalue.c_str());
        }
        return 0;
    }

    int String::toInt() {
        if (stringAlg::stringIsNumber(pvalue)) {
            return atoi(pvalue.c_str());
        }
        return 0;
    }

    std::string String::getStrType() {
        return ASTOX_VALUE_STR_STRING;
    }

    astox::Value * String::setRecursive(std::vector<SelectorNode> &list, int index, Value * newValue, Function *& context) {
        Error n(Error::ASSIGNMENT_ERROR, "unable to ovveride string value", __LINE__, __FILENAME__);
        return Value::npos;
    }

    //astox::String * String::cast(Value * value){
    //	return dynamic_cast<String *>(value);
    //}

    void String::setGetters() {
        objectList["length"] = Number::New(pvalue.size());
        objectList["toJSON"] = new Function(ecma::methods::proto_toJSON, this);
        objectList["reverse"] = new Function(ecma::methods::array_reverse, this);
        objectList["charAt"] = new Function(ecma::methods::string_charAt, this);
        objectList["trim"] = new Function(ecma::methods::string_trim, this);
        objectList["killWhiteSpace"] = new Function(ecma::methods::string_killWhiteSpace, this);
        objectList["split"] = new Function(ecma::methods::string_split, this);
        objectList["splitByWhiteSpace"] = new Function(ecma::methods::string_splitByWhiteSpace, this);
        objectList["splitByChar"] = new Function(ecma::methods::string_splitByChar, this);
        objectList["sort"] = new Function(ecma::methods::array_sort, this);
        objectList["indexOf"] = new Function(ecma::methods::array_indexOf, this);
        objectList["search"] = new Function(ecma::methods::array_indexOf, this);
        objectList["slice"] = new Function(ecma::methods::array_slice, this);
        objectList["substr"] = new Function(ecma::methods::array_splice, this);
        objectList["lastIndexOf"] = new Function(ecma::methods::array_lastIndexOf, this);
        objectList["match"] = new Function(ecma::methods::string_match, this);
        objectList["replace"] = new Function(ecma::methods::string_replace, this);
    }

    void String::updateGetters() {
        objectList["length"] = Number::New(pvalue.size());
    }

    Value * String::itemAtIndex(std::string indexStr) {
        int poz = 0;
        if (indexStr == "*") {
            poz = (pvalue.size() - 1);
        } else {
            poz = atoi(indexStr.c_str());
        }
        return itemAtIndex(poz);
    }

    bool String::toBoolean() {
        return pvalue.empty();
    };

    astox::Value * String::itemAtIndex(int index) {
        if (index < (int) pvalue.size() && !pvalue.empty()) {
            return new String(pvalue.at(index));
        }
        return Value::npos;
    };

    String * String::trim() {
        pvalue = astox::stringAlg::trim(pvalue);
        updateGetters();
        return this;
    };

    String * String::killWhiteSpace() {
        pvalue = stringAlg::replaceWhiteSpace(pvalue, "");
        updateGetters();
        return this;
    };

    Array * String::split(Value * arguments) {
        using namespace std;
        string separator = arguments->toString();
        Array * response = new Array();
        if (separator.empty()) {
            for (unsigned int i = 0; i < pvalue.size(); i++) {
                response->push(new String(pvalue.at(i)));
            }
        } else {
            vector<string> parts = stringAlg::split(pvalue, separator);

            for (unsigned int i = 0; i < parts.size(); i++) {
                response->push(new String(parts.at(i)));
            }
        }


        return response;
    };

    Array * String::splitByWhiteSpace(Value * arguments) {
        using namespace std;
        vector<string> parts = stringAlg::splitByWhiteSpace(pvalue);
        Array * response = new Array();
        for (unsigned int i = 0; i < parts.size(); i++) {
            response->push(new String(parts.at(i)));
        }
        return response;
    };

    Array * String::splitByChar(Value * arguments) {
        using namespace std;
        Array * response = new Array();
        if (!arguments->toString().empty()) {
            char separator = arguments->toString().at(0);
            vector<string> parts = stringAlg::splitByChar(pvalue, separator);
            for (unsigned int i = 0; i < parts.size(); i++) {
                response->push(new String(parts.at(i)));
            }
        }
        else {
            response->push(new String(pvalue));
        }
        return response;
    };

    String * String::sort() {
        using namespace std;
        string duplicate = pvalue;
        std::sort(duplicate.begin(), duplicate.end());
        return new String(duplicate);
    };

    Number * String::indexOf(std::string substr) {
        using namespace std;
        size_t poz = pvalue.find(substr);
        if (poz == string::npos) {
            return Number::New(-1);
        }
        return Number::New(poz);
    };

    Number * String::lastIndexOf(std::string substr) {
        using namespace std;
        size_t poz = pvalue.rfind(substr);
        cout << " last index of " << substr << " in " << pvalue << " is " << (poz - substr.size()) << endl;
        if (poz == string::npos) {
            return Number::New(-1);
        }
        return Number::New(poz);
    };

    String * String::slice(Value * arguments) {
        int start = arguments->itemAtIndex(0)->toInt();
        int expectEnd = arguments->itemAtIndex(1)->toInt();
        return slice(start, expectEnd);
    };

    String * String::substr(Value * arguments) {
        using namespace std;
        int start = arguments->itemAtIndex(0)->toInt();
        int expectEnd = arguments->itemAtIndex(1)->toInt();
        int end = start + expectEnd;
        cout << " substr " << start << " to " << end << endl;
        return slice(start, end);
    };

    String * String::slice(int start, int end) {
        using namespace std;
        string response;
        if (start < 0) {
            start = pvalue.size() + start;
        }

        if (end < 0) {
            end = pvalue.size() + end;
        }

        if (end > (int) pvalue.size()) {
            end = pvalue.size();
        }


        for (int i = start; i < end; i++) {
            response += pvalue.at(i);
        }
        return new String(response);
    };

    Array * String::match(Value * arguments) {
        using namespace stringAlg;
        using namespace std;
        Array * response = new Array();
        if (arguments->getType() == ASTOX_VALUE_REGEXP) {

            vector<RegExpMatch> matches = regex_match_group(toString(), arguments->toString(), RegExp::cast(arguments)->getFlags());
            for (unsigned int i = 0; i < matches.size(); i++) {
#ifdef ASTX_DBG_REGEX
                astox_debug(__LINE__, __FILENAME__, "Array * String::match:: parsed (%s)", matches[i].part.c_str());
#endif
                response->push(new String(matches[i].part));
            }
        }
#ifdef ASTX_DBG_REGEX
        else {
            astox_debug(__LINE__, __FILENAME__, "Array * String::match:: failed because argtype is (%s)", arguments->getStrType().c_str());
        }
#endif
        return response;
    };

    String * String::replace(Value * arguments) {
        using namespace std;
        if (arguments->getType() == ASTOX_VALUE_ARRAY) {

            Array * args = Array::cast(arguments);
            if (args->length() > 1) {

                if (args->itemAtIndex(0)->getType() == ASTOX_VALUE_STRING || args->itemAtIndex(0)->getType() == ASTOX_VALUE_NUMBER) {
                    string foundable = args->itemAtIndex(0)->toString();
                    string replaceble = args->itemAtIndex(1)->toString();
                    string pvalc = pvalue;
                    size_t pos = pvalc.find(foundable);

                    if (pos != string::npos) {
                        string r = pvalc.substr(0, pos);
                        r.append(replaceble);
                        string spr = pvalc.substr(pos + foundable.size(), pvalc.size());
                        r.append(spr);
                        return new String(r);
                    }
                    return new String(pvalc);
                }
                else if (args->itemAtIndex(0)->getType() == ASTOX_VALUE_REGEXP) {
                    AstoxRegexFlag regexFlags = RegExp::cast(arguments->itemAtIndex(0))->getFlags();
                    std::string result = stringAlg::regex_replace_all(pvalue, args->itemAtIndex(0)->toString(), args->itemAtIndex(1)->toString(), regexFlags);
                    return new String(result);
                }
            }
            else {
                Error n(Error::PARSING_ERROR, "insufficient arguments provided", __LINE__, __FILENAME__);
            }

        }
        return new String("dummy");
    };



} /* namespace astox */
