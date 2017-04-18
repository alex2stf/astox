/*
 * Object.cpp
 *
 *  Created on: Jan 26, 2014
 *      Author: Alexandru
 */

#include "astox.h"

namespace astox {

    Object * Object::npos = new Object();

    Object::Object()
    : Value() {
        valueType = ASTOX_VALUE_OBJECT;
    }

    Object::Object(const char * value)
    : Value() {
        std::string s(value);
        s = astox::stringAlg::trim(s);

        if (s.size() > 2 && s.at(0) == '{' && s.at(s.size() - 1) == '}') {
            s = s.substr(1, s.size()); // scot primul
            s = s.substr(0, s.size() - 1); //scot ultimul
            *this = *(ecma::fromStrAsObject(s));
        }
        valueType = ASTOX_VALUE_OBJECT;
    }

    Object::~Object() {
        // TODO Auto-generated destructor stub
    }

    void Object::updateGetters() {

    };

    void Object::setGetters() {

    };

    std::string Object::getStrType() {
        return ASTOX_VALUE_STR_OBJECT;
    }

    std::string Object::toString() {
        return astox::lang::toJSON(&(*this), "", "");
    }

    void Object::setKeys(std::map<std::string, astox::Value *> keys) {
        objectList = keys;
    }

    int Object::length() {
        return objectList.size();
    }

    astox::Value *& Object::getValue(std::string keyName) {
        it = objectList.find(keyName);
        if (it == objectList.end()) { // not found
            //Error err(Error::RANGE_ERROR, "key: '"+keyName+"' not found", __LINE__, __FILENAME__);
            return Value::npos;
        }

        //	return (&it->second);
        //	return *(&it->second);//objectList[keyName];
        return objectList[keyName];
    }

    astox::Value * Object::getKeyLoose(std::string keyName) {
        it = objectList.find(keyName);
        if (it == objectList.end()) { // not found
            objectList[keyName] = Value::npos;
            return objectList[keyName];
        }
        return objectList[keyName];
    }

    astox::Value *& Object::get(astox::Value * data) {
        using namespace std;
        string keyName = data->toString();
        return getValue(keyName);
    }

    Boolean * Object::contains(Value * term) {
        if (objectList.find(term->toString()) != objectList.end()) {
            return Boolean::BTRUE;
        }
        return Boolean::BFALSE;
    };

    bool Object::hasKey(std::string name) {
        return (objectList.find(name) != objectList.end());
    };

    astox::Object * Object::getObject(std::string keyName) {
        it = objectList.find(keyName);
        if (it == objectList.end()) { // not found
            Error err(Error::RANGE_ERROR, "key: '" + keyName + "' not found", __LINE__, __FILENAME__);
            return Object::npos;
        }
#ifdef ASTX_VALUE_STATIC_CAST
        return static_cast<Object *> (it->second);
#else
        return dynamic_cast<Object *> (it->second);
#endif
    }

    astox::Array * Object::getArray(std::string keyName) {
        it = objectList.find(keyName);
        if (it == objectList.end()) { // not found
            Error err(Error::RANGE_ERROR, "key: '" + keyName + "' not found", __LINE__, __FILENAME__);
            return new Array();
        }
#ifdef ASTX_VALUE_STATIC_CAST
        return static_cast<Array *> (it->second);
#else
        return dynamic_cast<Array *> (it->second);
#endif
    }

    astox::Number * Object::getNumber(std::string keyName) {
        it = objectList.find(keyName);
        if (it == objectList.end()) { // not found
            Error err(Error::RANGE_ERROR, "key: '" + keyName + "' not found", __LINE__, __FILENAME__);
            return Number::NaN;
        }
#ifdef ASTX_VALUE_STATIC_CAST
        return static_cast<Number *> (it->second);
#else
        return dynamic_cast<Number *> (it->second);
#endif
    }



    //TODO parser pointe based for mustache

    std::string Object::getKeyToStdString(std::string keyName, bool pointSensitive) {
        it = objectList.find(keyName);
        if (it == objectList.end()) { // not found
            return keyName;
        }
        return it->second->toString();
    }

    astox::Object * Object::cast(Value * value) {
#ifdef ASTX_VALUE_STATIC_CAST
        return static_cast<Object *> (value);
#else
        return dynamic_cast<Object *> (value);
#endif
    }

    std::map<std::string, astox::Value *> * Object::getObjectList() {
        return &objectList;
    }

    void Object::iterate(void (onIterate) (std::string keyName, astox::Value * value)) {
        for (it = objectList.begin(); it != objectList.end(); it++) {
            onIterate(it->first, it->second);
        }
    };

    astox::Value * Object::updateValue(std::string keyName, astox::Value * newValue) {
        newValue->setParentUid(getUid());
        it = objectList.find(keyName);
        if (it == objectList.end()) {
            return objectList[keyName] = newValue;
        }

        delete objectList[keyName];
        return objectList[keyName] = newValue;
    }

    /*ISSES:*/
    bool Object::isString() {
        return false;
    }

    bool Object::isNumber() {
        return false;
    }

    bool Object::isBoolean() {
        return false;
    }

    bool Object::isArray() {
        return false;
    }

    bool Object::isObject() {
        return true;
    }

    bool Object::isUndefined() {
        return false;
    }

    Boolean * Object::negate() {
        return Boolean::BFALSE;
    }



    //Object& Object::operator=(const std::string &rhs)
    //{
    //	return *this;
    //}




} /* namespace astox */
