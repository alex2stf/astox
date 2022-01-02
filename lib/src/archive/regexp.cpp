/*
 * regexp.cpp
 *
 *  Created on: 27 Jun 2017
 *      Author: alex
 */

#include <astox.h>


namespace astox {

using namespace std;
using namespace util;

RegExp::RegExp(const lexic l): Value(l) {
    valueType = ASTOX_VALUE_REGEXP;
    flags = JSREG_NONE;
    objmap["exec"] = Function::New(ecma::methods::regexp_exec, this, l);
}

RegExp::RegExp(string data, stx_regex_flag pflags, const lexic l): Value(l) {
	valueType = ASTOX_VALUE_REGEXP;
	flags = pflags;
	rstr = data;
	objmap["exec"] = Function::New(ecma::methods::regexp_exec, this, l);
}

stxtr RegExp::str() {
	return rstr;
};

const char * RegExp::strtype(){
	return _lx.env.regexp;
};



Value * RegExp::exec(Value * val){
	log_stack("--------------------------- exec %s", blurt(val).c_str());
	if(val->isString()){
		 vector<RegExpMatch> m = regex_match_group(val->str(), str(), getFlags());
		 Array * r = new Array(_lx);

		 for (unsigned int i = 0; i < m.size(); i++) {
			r->push(String::New(m.at(i).part, _lx));
		 }
		 return r;
	}

	return Value::npos;
}

RegExp * RegExp::cast(Value * value) {
#ifdef ASTX_VALUE_STATIC_CAST
    return static_cast<RegExp *> (value);
#else
    return dynamic_cast<RegExp *> (value);
#endif
};

stx_regex_flag RegExp::digestMIG(std::string &in, bool scanStr, std::string allowedChars) {

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



stx_regex_flag RegExp::getFlags() {
    return flags;
};

Value * RegExp::call(std::string &args, Function *& context) {
    using namespace std;
   // rstr = args.c_str();
    //	cout << " called with " << args << endl;
    int lastPoz = 0;
    //stx_regex_flag flags = RegExp::strIsRegexFormat(args, lastPoz);

//    if (flags != JSREG_NONE) {
//        regexStrValue = regexStrValue.substr(1, lastPoz - 1);
//    }
//    else {
//        cout << args << " IS NOT REGEX STR" << endl;
//    }
//    return new RegExp(regexStrValue, flags);
	return this;
};

}


