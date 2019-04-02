/*
 * Boolean.cpp
 *
 *  Created on: Jan 26, 2014
 *      Author: Alexandru
 */

#include "astox.h"


namespace astox {



    Boolean * Boolean::New(bool d, const lexic &l) {
        return Memoize::getInstance()->getBoolean(l, d);
    };

    Boolean * Boolean::bTrue( const lexic &l){
    	return Boolean::New(true, l);
    };

    Boolean * Boolean::bFalse( const lexic &l){
    	return Boolean::New(false, l);
    };

    Boolean::Boolean(bool value, const lexic &l) : Value(l) {
        valueType = ASTOX_VALUE_BOOL;
        pvalue = value;
    }

    Boolean::~Boolean() {

    }

    bool Boolean::bol() {
        return pvalue;
    }

    stxtr Boolean::str() {
        return pvalue ? _lx.vtrue : _lx.vfalse;
    }


    Function * Boolean::constructor(){
    	if(_constr == NULL){
    		_constr = Function::BOLCNSTR;
    	}
    	return _constr;
    };


   	const char * Boolean::strtype() {
        return _lx.env.boolean;
    }

    bool Boolean::isString() {
        return false;
    }

    bool Boolean::isNumber() {
        return false;
    }

    bool Boolean::isBoolean() {
        return true;
    }

    bool Boolean::isArray() {
        return false;
    }

    bool Boolean::isObject() {
        return false;
    }

    bool Boolean::isUndefined() {
        return false;
    }

    Boolean * Boolean::negate() {
        return Boolean::New(!pvalue, _lx);
    };

    int Boolean::toInt(){
    	if(pvalue) { return 1; }
    	return 0;
    };
    double Boolean::dbl(){
    	if(pvalue) { return 1; }
		return 0;
    };


} /* namespace astox */
