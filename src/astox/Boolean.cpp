/*
 * Boolean.cpp
 *
 *  Created on: Jan 26, 2014
 *      Author: Alexandru
 */

#include "astox.h"

namespace astox {

    Boolean * Boolean::BFALSE = new Boolean(false);
    Boolean * Boolean::BTRUE = new Boolean(true);

    Boolean * Boolean::New(bool data) {
        if (data) {
            return Boolean::BTRUE;
        }
        return Boolean::BFALSE;
    };

    Boolean::Boolean(bool value) : Value() {
        valueType = ASTOX_VALUE_BOOL;
        pvalue = value;
    }

    Boolean::~Boolean() {

    }

    bool Boolean::toBoolean() {
        return pvalue;
    }

    std::string Boolean::toString() {
        return pvalue ? "true" : "false";
    }

    Boolean * Boolean::getStatic(bool data) {
        if (data) {
            return Boolean::BTRUE;
        }
        return Boolean::BFALSE;
    }

    std::ostream& operator<<(std::ostream& output, const Boolean& val) {
        if (val.pvalue) {
            output << "true";
        } else {
            output << "false";
        }
        return output;
    }

    std::ostream& operator<<(std::ostream& output, const Boolean * val) {

        if (val->pvalue) {
            output << "true";
        } else {
            output << "false";
        }
        return output;
    }

    astox::Value * Boolean::setRecursive(std::vector<SelectorNode> &list, int index, Value * newValue, Function *& context) {
        Error n(Error::ASSIGNMENT_ERROR, "unable to ovveride boolean value", __LINE__, __FILENAME__);
        return Value::npos;
    }

    std::string Boolean::getStrType() {
        return ASTOX_VALUE_STR_BOOL;
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
        return new Boolean(!pvalue);
    };



} /* namespace astox */
