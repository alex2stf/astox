/*
 * Number.cpp
 *
 *  Created on: Jan 26, 2014
 *      Author: Alexandru
 */

#include "astox.h"

#include <sstream>

namespace astox {

    Number * Number::ZERO = new Number();

    Number * Number::NaN = new Number(NAN);

    Number::Number() : Value() {
        pvalue = 0;
        valueType = ASTOX_VALUE_NUMBER;
    }

    Number::Number(double value) : Value() {
        pvalue = value;
        valueType = ASTOX_VALUE_NUMBER;
    }

    Number * Number::New(std::string strNum) {
        return Number::New(strNum.c_str());
    };

    Number * Number::New(const char * strNum) {
        return new Number(atof(strNum));
    };

    Number * Number::New(double value) {
        return new Number(value);
    }

    Number * Number::New() {
        return Number::ZERO;
    }

    Number::~Number() {
        // TODO Auto-generated destructor stub
    }

    Number * Number::cast(Value * value) {
#ifdef ASTX_VALUE_STATIC_CAST
        return static_cast<Number *> (value);
#else
        return dynamic_cast<Number *> (value);
#endif
    }

    void Number::updatePValue(int newInt) {
        pvalue = newInt;
    };




    std::string Number::toString() {
    
		return astox::stringAlg::ecmaToString(pvalue);

		



		//If m is less than zero, return the String concatenation of the String "-" and ToString(?m).

		



        
    }

    std::string Number::getStrType() {
        return ASTOX_VALUE_STR_NUMBER;
    }

    bool Number::toBoolean() {
        return pvalue != 0;
    };

    int Number::toInt() {
        return (int) pvalue;
    }

    double Number::toDouble() {
        return pvalue;
    }

    Boolean * Number::negate() {
        return Boolean::BFALSE;
    }

    bool Number::isString() {
        return false;
    }

    bool Number::isNumber() {
        return true;
    }

    bool Number::isBoolean() {
        return false;
    }

    bool Number::isArray() {
        return false;
    }

    bool Number::isObject() {
        return false;
    }

    bool Number::isUndefined() {
        return false;
    }

    Value * Number::plusPlus() {
        pvalue++;
        return &(*this);
    };

    Value * Number::minMin() {
        pvalue--;
        return &(*this);
    };

    astox::Value * Number::setRecursive(std::vector<SelectorNode> &list, int index, Value * newValue, Function *& context) {
        //	Error n(Error::ASSIGNMENT_ERROR, "unable to ovveride number value", __LINE__, __FILENAME__);
        return Value::npos;
    }



} /* namespace astox */
