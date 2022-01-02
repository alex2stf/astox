/*
 * String.cpp
 *
 *  Created on: Jan 26, 2014
 *      Author: Alexandru
 */

#include "astox.h"

namespace astox {

	using namespace std;
	using namespace serializers;

    String * String::New(stxtr value, const lexic &lxc) {
		return Memoize::getInstance()->getString(lxc, value);
    };

    String * String::New(char c, const lexic &lxc){
    	string s;
    	s+=c;
    	return String::New(s, lxc);
    }

    String * String::New(int c, const lexic &lxc){
    	stringstream s; s << c;
		return String::New(s.str(), lxc);
	}

    String::String(char charVal, const lexic lxc) : Value(lxc) {
        pvalue = charVal;
        valueType = ASTOX_VALUE_STRING;
        _proto = new Object(_lx);
        _proto->addKey(_lx.env.length,  Number::New((double)pvalue.size(), _lx));
    };


    String::String(char * value,  const lexic lxc) : Value(lxc) {
        pvalue = value;
        valueType = ASTOX_VALUE_STRING;
        _proto = new Object(_lx);
	    _proto->addKey(_lx.env.length,  Number::New((double)pvalue.size(), _lx));
    }


	Value * String::getValue(stxtr keyName) {
		if (astox::stringAlg::stringIsNumber(keyName)) {
			int index = atoi(keyName.c_str());
			return String::New(pvalue.at(index), _lx);
		}
		return objmap[keyName];
	}


    String::~String() {
        // TODO Auto-generated destructor stub
    }

    Boolean * String::negate() {
        return Boolean::bFalse(_lx);
    };

    Value * String::reverse() {
    	string cp = pvalue;
        std::reverse(cp.begin(), cp.end());
        return String::New(cp, _lx);
    }

    stxtr String::str() {
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
        pvalue.append(data->str());
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

    double String::dbl() {
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

    const char * String::strtype() {
        return _lx.env.string;
    }

    const char * String::c_str(){
    	return pvalue.c_str();
    };


    Value * String::at(std::string indexStr) {
        int poz = 0;
        if (indexStr == "*") {
            poz = (int) (pvalue.size() - 1);
        } else {
            poz = atoi(indexStr.c_str());
        }
        return at(poz);
    }

    bool String::bol() {
        return pvalue.empty();
    };

    Value * String::at(int index) {
        if (index < (int) pvalue.size() && !pvalue.empty()) {
            return String::New(pvalue.at(index), _lx);
        }
        return Value::npos;
    };

    String * String::trim() {
        string cp = pvalue;
        return String::New(stringAlg::trim(pvalue), _lx);
    };

    String * String::killWhiteSpace() {
        pvalue = stringAlg::replaceWhiteSpace(pvalue, "");
        return this;
    };

    Array * String::split(Value * arguments) {
        using namespace std;
        string separator = arguments->str();
        Array * response = Array::New(_lx); //TODO modify
        if (separator.empty()) {
            for (unsigned int i = 0; i < pvalue.size(); i++) {
                response->push(String::New(pvalue.at(i), _lx));
            }
        } else {
            vector<string> parts = stringAlg::split(pvalue, separator);

            for (unsigned int i = 0; i < parts.size(); i++) {
                response->push(String::New(parts.at(i), _lx));
            }
        }
        return response;
    };

    Array * String::splitByWhiteSpace(Value * arguments) {
        using namespace std;
        vector<string> parts = stringAlg::splitByWhiteSpace(pvalue);
        Array * response =  Array::New(_lx);
        for (unsigned int i = 0; i < parts.size(); i++) {
            response->push(String::New(parts.at(i), _lx));
        }
        return response;
    };

    Array * String::splitByChar(Value * arguments) {
        using namespace std;
        Array * response = Array::New(_lx);
        if (!arguments->str().empty()) {
            char separator = arguments->str().at(0);
            vector<string> parts = stringAlg::splitByChar(pvalue, separator);
            for (unsigned int i = 0; i < parts.size(); i++) {
            	if(!parts.at(i).empty()){
            		  response->push(String::New(parts.at(i), _lx));
            	}
            }
        }
        else {
            response->push(String::New(pvalue, _lx));
        }
        return response;
    };

    String * String::sort() {
        string duplicate = pvalue;
        std::sort(duplicate.begin(), duplicate.end());
        return String::New(duplicate, _lx);
    };

    Number * String::indexOf(std::string substr) {
        size_t poz = pvalue.find(substr);
        if (poz == string::npos) {
            return Number::New(-1, _lx);
        }
        return Number::New((int) poz, _lx);
    };

    Number * String::lastIndexOf(std::string substr) {
        size_t poz = pvalue.rfind(substr);
        cout << " last index of " << substr << " in " << pvalue << " is " << (poz - substr.size()) << endl;
        if (poz == string::npos) {
            return Number::New(-1, _lx);
        }
        return Number::New((int)poz, _lx);
    };

    String * String::slice(Value * arguments) {
        int start = arguments->at(0)->toInt();
        int expectEnd = arguments->at(1)->toInt();
        return slice(start, expectEnd);
    };

    String * String::substr(Value * arguments) {
        using namespace std;
        int start = arguments->at(0)->toInt();
        int expectEnd = arguments->at(1)->toInt();
        int end = start + expectEnd;
        cout << " substr " << start << " to " << end << endl;
        return slice(start, end);
    };

    String * String::slice(int start, int end) {
        string response;
        if (start < 0) {
            start = (int) pvalue.size() + start;
        }

        if (end < 0) {
            end = (int) (pvalue.size() + end);
        }

        if (end > (int) pvalue.size()) {
            end = (int) pvalue.size();
        }


        for (int i = start; i < end; i++) {
            response += pvalue.at(i);
        }
        return String::New(response, _lx);
    };

    Array * String::match(Value * arguments) {
        method_not_implemented();
        Array * response = Array::New(_lx);
        if (arguments->type() == ASTOX_VALUE_REGEXP) {

            vector<RegExpMatch> matches = util::regex_match_group(str(), arguments->str(), RegExp::cast(arguments)->getFlags());
            for (unsigned int i = 0; i < matches.size(); i++) {
//                response->push(String::New(matches[i].part));
            }
        }
        return response;
    };

    String * String::replace(Value * arguments) {
        if (arguments->type() == ASTOX_VALUE_ARRAY) {

            Array * args = Array::cast(arguments);
            if (args->length() > 1) {

                if (args->at(0)->type() == ASTOX_VALUE_STRING || args->at(0)->type() == ASTOX_VALUE_NUMBER) {
                    string foundable = args->at(0)->str();
                    string replaceble = args->at(1)->str();
                    string pvalc = pvalue;
                    size_t pos = pvalc.find(foundable);

                    if (pos != string::npos) {
                        string r = pvalc.substr(0, pos);
                        r.append(replaceble);
                        string spr = pvalc.substr(pos + foundable.size(), pvalc.size());
                        r.append(spr);
                        return String::New(r, _lx);
                    }
                    return String::New(pvalc, _lx);
                }
                else if (args->at(0)->type() == ASTOX_VALUE_REGEXP) {
                    stx_regex_flag regexFlags = RegExp::cast(arguments->at(0))->getFlags();
                    std::string result = util::regex_replace_all(pvalue, args->at(0)->str(), args->at(1)->str(), regexFlags);
                    return String::New(result, _lx);
                }
            }
            else {
            	throw_error("insufficient arguments provided");
            }

        }
        return String::New("", _lx);
    };


	Function * String::constructor(){
		if(_constr == NULL){
			_constr = Function::STRCNSTR;
		}
		return _constr;
	};


	Value * String::increment(Value * v){
		stxtr r;
		r.append(pvalue);
		if(v->isArray()){
			r.append(to_csv(v));
		} else {
			r.append(v->str());
		}
		return String::New(r, _lx);
	};

} /* namespace astox */
