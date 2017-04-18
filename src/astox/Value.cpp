/*
 * Value.cpp
 *
 *  Created on: Jan 26, 2014
 *      Author: Alexandru
 */

#include "astox.h"

namespace astox {

    using namespace std;

    Value * Value::npos = new Value();

    Value::Value() {
        valueType = ASTOX_VALUE_UNDEFINED;
        //generate uid:
        parentUid = -1;
        static int ASTOX_NUM_INSTANCES = 1;
        uid = ASTOX_NUM_INSTANCES++;
        numCalls = 0;
    }

    Value::~Value() {
        // TODO Auto-generated destructor stub
    }

    bool Value::hasKey(std::string name) {
        return false;
    };

    void Value::addKey(std::string key, astox::Value * value) {
        key = astox::util::replace(key, "\"", "");
        key = astox::stringAlg::trim(key);
        value->setParentUid(getUid());
        objectList[key] = value;
    }

    int Value::getNumCalls() {
        return numCalls;
    };

    void Value::setNumCalls(int num) {
        numCalls = num;
    };

    void Value::dropData() {

    }

    Value * Value::call(Value * arguments) {
        return Value::npos;
    };

    Value * Value::call(std::string &args, Function *&context) {
        if (!args.empty()) {
            return ecma::runCommand(args, context);
        }
        return Value::npos;
    }

    bool Value::isEmpty() {
        return true;
    }

    void Value::updatePValue(std::string newString) {

    };

    void Value::updatePValue(int newInt) {

    };

    std::string Value::toString() {
        return ASTOX_TOSTRING_VALUE;
    }

    int Value::getType() {
        return valueType;
    }

    std::string Value::getStrType() {
        return ASTOX_VALUE_STR_UNDEFINED;
    }

    void Value::setParentUid(int uid) {
        //				std::cout << "my parent has uid " << uid  << " and I have uid   "<< getUid() << " and value " << toString() << std::endl;
        parentUid = uid;
    }

    Boolean * Value::contains(Value * term) {
        return Boolean::BFALSE;
    }

    int Value::getParentUid() {
        return parentUid;
    }

    int Value::getUid() {
        return uid;
    }

    int Value::toInt() {
        return 0;
    }

    double Value::toDouble() {
        return 0;
    }

    bool Value::isString() {
        return (valueType == ASTOX_VALUE_STRING);
    }

    bool Value::isNumber() {
        return (valueType == ASTOX_VALUE_NUMBER);
    }

    bool Value::isBoolean() {
        return (valueType == ASTOX_VALUE_BOOL);
    }

    bool Value::isArray() {
        return (valueType == ASTOX_VALUE_ARRAY);
    }

    bool Value::isObject() {
        return (valueType == ASTOX_VALUE_OBJECT);
    }

    bool Value::isUndefined() {
        return (valueType == ASTOX_VALUE_UNDEFINED);
    }

    astox::Value *& Value::getValue(std::string keyName) {
        return Value::npos;
    }

    astox::String * Value::getString(std::string keyName) {
        return new String(); //TODO String::npos
    }

    astox::Number * Value::getNumber(std::string keyName) {
        return Number::New(1);
    }

    astox::Boolean * Value::getBoolean(std::string keyName) {
        return Boolean::New(false);
    }

    astox::Object * Value::getObject(std::string keyName) {
        return new Object();
    }

    astox::Array * Value::getArray(std::string keyName) {
        return new Array();
    }

    astox::Value * Value::updateValue(std::string keyName, astox::Value * newValue) {
        return Value::npos;
    }

    int Value::length() {
        return 0;
    }

    Value * Value::itemAtIndex(int index) {
        return Value::npos;
    };

    Value * Value::itemAtIndex(std::string indexStr) {
        return Value::npos;
    };

    astox::Value *& Value::get(astox::Value * data) {
        return Value::npos;
    }

    Value * Value::plusPlus() {
        double val = toDouble();
        val++;
        return Number::New(val);
    };

    Value * Value::minMin() {
        double val = toDouble();
        val--;
        return Number::New(val);
    }

    bool Value::toBoolean() {
        return false;
    }

    astox::Value * Value::duplicate(Value * in) {
        switch (in->getType()) {
            case ASTOX_VALUE_BOOL:
                return Boolean::New(in->toBoolean());
            case ASTOX_VALUE_NUMBER:
                return Number::New(in->toDouble());
            case ASTOX_VALUE_STRING:
                return new String(in->toString());
        }
        return in;
    }

    Boolean * Value::isBiggerThan(Value * term) {
        bool comp = toDouble() > term->toDouble();
#ifdef ASTX_DBG_ECMA_COMPARE
        astox_debug(__LINE__, __FILENAME__, "Value::isBiggerThan this(%s) %s %s",
                lang::toJSON(this, "", "").c_str(),
                lang::toJSON(term, "", "").c_str(),
                Boolean::New(comp)->toString().c_str());
#endif
        return Boolean::New(comp);
    }

    Boolean * Value::isLessThan(Value * term) {
        bool comp = toDouble() < term->toDouble();
#ifdef ASTX_DBG_ECMA_COMPARE
        astox_debug(__LINE__, __FILENAME__, "Value::isLessThan this(%s) %s %s",
                lang::toJSON(this, "", "").c_str(),
                lang::toJSON(term, "", "").c_str(),
                Boolean::New(comp)->toString().c_str());
#endif
        return Boolean::New(comp);
    }

    Boolean * Value::isBiggerEqThan(Value * term) {
        bool comp = toDouble() >= term->toDouble();
#ifdef ASTX_DBG_ECMA_COMPARE
        astox_debug(__LINE__, __FILENAME__, "Value::isBiggerEqThan this(%s) %s %s",
                lang::toJSON(this, "", "").c_str(),
                lang::toJSON(term, "", "").c_str(),
                Boolean::New(comp)->toString().c_str());
#endif
        return Boolean::New(comp);
    }

    Boolean * Value::isLessEqThan(Value * term) {
        bool comp = toDouble() <= term->toDouble();
#ifdef ASTX_DBG_ECMA_COMPARE
        astox_debug(__LINE__, __FILENAME__, "Value::isLessEqThan this(%s) %s %s",
                lang::toJSON(this, "", "").c_str(),
                lang::toJSON(term, "", "").c_str(),
                Boolean::New(comp)->toString().c_str());
#endif
        return Boolean::New(comp);
    }

    Value * Value::increment(Value * term) {
        //	cout << "increment " << lang::toJSON(this) << " + " << lang::toJSON(term) << endl;
        if (valueType == ASTOX_VALUE_UNDEFINED && term->getType() == ASTOX_VALUE_STRING) {
            return String::NaN;
        }

        if (valueType == ASTOX_VALUE_STRING || term->getType() == ASTOX_VALUE_STRING) {
            return new String(toString().append(term->toString()));
        }

        double sum = toDouble() + term->toDouble();
        return Number::New(sum);
    }

    Value * Value::shiftRight(Value * term) {
        int left = toInt();
        int right = term->toInt();
        int result = left >> right;
        return Number::New(result);
    }

    Value * Value::shiftLeft(Value * term) {
        int left = toInt();
        int right = term->toInt();
        int result = left << right;
        return Number::New(result);
    }

    Value * Value::substract(Value * term) {
        double sub = toDouble() - term->toDouble();
        return Number::New(sub);
    };

    Value * Value::multiply(Value * term) {
        double mult = toDouble() * term->toDouble();
        return Number::New(mult);
    };

    Value * Value::divide(Value * term) {
        double div = toDouble() / term->toDouble();
        return Number::New(div);
    };

    Value * Value::modulus(Value * term) {
        int left = toInt();
        int right = term->toInt();
#ifdef ASTX_DBG_ECMA_COMPARE
        int r = left % right;
        astox_debug(__LINE__, __FILENAME__, "Value::modulus this(%i  %i) = %i",
                left,
                right,
                r);
#endif
        return Number::New(left % right);
    }

    Boolean * Value::equals(Value * term) {
        bool comp = toString() == term->toString();
        if (valueType == ASTOX_VALUE_NUMBER && term->getType() == ASTOX_VALUE_NUMBER) {
            comp = toDouble() == term->toDouble();
        }
#ifdef ASTX_DBG_ECMA_COMPARE
        astox_debug(__LINE__, __FILENAME__, "Value::equals this(%s) %s %s",
                lang::toJSON(this, "", "").c_str(),
                lang::toJSON(term, "", "").c_str(),
                Boolean::New(comp)->toString().c_str());
#endif
        return Boolean::getStatic(comp);
    };

    Boolean * Value::equalsStrict(Value * term) {
        bool comp = (toDouble() == term->toDouble()) && (getType() == term->getType());
#ifdef ASTX_DBG_ECMA_COMPARE
        astox_debug(__LINE__, __FILENAME__, "Value::equalsStrict this(%s) %s %s",
                lang::toJSON(this, "", "").c_str(),
                lang::toJSON(term, "", "").c_str(),
                Boolean::New(comp)->toString().c_str());
#endif
        return Boolean::getStatic(comp);
    };

    Boolean * Value::differs(Value * term) {
        bool div = toInt() != term->toInt();
#ifdef ASTX_DBG_ECMA_COMPARE
        astox_debug(__LINE__, __FILENAME__, "Value::differs this(%s) %s %s",
                lang::toJSON(this, "", "").c_str(),
                lang::toJSON(term, "", "").c_str(),
                Boolean::New(div)->toString().c_str());
#endif
        return Boolean::New(div);
    }

    Boolean * Value::differsStrict(Value * term) {
        bool div = (toDouble() != term->toDouble()) && (getType() != term->getType());
#ifdef ASTX_DBG_ECMA_COMPARE
        astox_debug(__LINE__, __FILENAME__, "Value::differsStrict this(%s) %s %s",
                lang::toJSON(this, "", "").c_str(),
                lang::toJSON(term, "", "").c_str(),
                Boolean::New(div)->toString().c_str());
#endif
        return Boolean::New(div);
    }
    //TODO check for string and int types

    Value * Value::orBase(Value * term) {
        return Number::New((toInt() | term->toInt()));
    }

    Value * Value::orStrict(Value * term) {
        return Number::New((toInt() || term->toInt()));
    }

    Value * Value::andBase(Value * term) {
        return Number::New((toInt() & term->toInt()));
    }

    Value * Value::andStrict(Value * term) {
#ifdef ASTX_DBG_ECMA
        cout << "andStrict: " << lang::toJSON(this) << "&&" << lang::toJSON(term) << " = " << (toInt() && term->toInt()) << endl;
#endif
        if (valueType == ASTOX_VALUE_BOOL && term->getType() == ASTOX_VALUE_BOOL) {
            return Boolean::New(toBoolean() && term->toBoolean());
        }

        return Number::New((toInt() && term->toInt()));
    }

    Value * Value::modify(Value * term, int operation) {
        switch (operation) {
            case OP_SHIFT_LEFT:
                return shiftLeft(term);
            case OP_SHIFT_RIGHT:
                return shiftRight(term);
        }
        return Value::npos;
    }

    Boolean * Value::negate() {
        return Boolean::BFALSE;
    }


    //TODO create statics

    String * Value::getTypeOf() {
        switch (valueType) {
            case ASTOX_VALUE_UNDEFINED:
                return new String(ASTOX_VALUE_STR_UNDEFINED);
            case ASTOX_VALUE_ARRAY:
            case ASTOX_VALUE_OBJECT:
                return new String(ASTOX_VALUE_STR_OBJECT);
            case ASTOX_VALUE_BOOL:
                return new String(ASTOX_VALUE_STR_BOOL);
            case ASTOX_VALUE_NUMBER:
                return new String(ASTOX_VALUE_STR_NUMBER);
            case ASTOX_VALUE_STRING:
                return new String(ASTOX_VALUE_STR_STRING);
        }
        return new String(ASTOX_VALUE_STR_OBJECT);
    }

    Boolean * Value::isInstanceOf(std::string keyName) {
        keyName = stringAlg::trim(keyName);

        std::transform(keyName.begin(), keyName.end(), keyName.begin(), ::tolower);

        cout << " keyname = " << keyName << endl;
        switch (valueType) {

            case ASTOX_VALUE_STRING:
                if (keyName == "string") {
                    return Boolean::New(true);
                }
                break;
            case ASTOX_VALUE_ARRAY:
                if (keyName == "array") {
                    return Boolean::New(true);
                }
                break;

        }


        switch (valueType) {
            case ASTOX_VALUE_OBJECT:
            case ASTOX_VALUE_ARRAY:
            case ASTOX_VALUE_FUNCTION:
            case ASTOX_VALUE_CUSTOM:
                if (keyName == "object") {
                    return Boolean::New(true);
                }
                break;
        }

        return Boolean::New(false);
    };

    std::vector<std::string> Value::getKeyNameList() {
        vector<string> response;
        std::map<std::string, astox::Value *>::iterator it;
        for (it = objectList.begin(); it != objectList.end(); it++) {
            response.push_back(it->first);
        }
        return response;
    }

    int Value::eraseKey(std::string keyName) {
        if (objectList.find(keyName) != objectList.end()) {
            objectList.erase(objectList.find(keyName));
            return 1;
        }
        return 0;
    }

    void Value::updateGetters() {
        ;
        ;
    };

    void Value::setGetters() {
        ;
        ;
    };

    //Value * Value::getParsedArgs(std::string &args, Function *& context){
    //	return ecma::runCommand(args, context);
    //}

    astox::Value * Value::getRecursive(std::vector<SelectorNode> &list, int index, Function *& context, int getType) {
        string keyName;


        if (index < (int) list.size()) {
            keyName = list.at(index).data;
            //		cout << " get prop " << keyName << " for (" << lang::toJSON(this)<< ")" <<  endl;
            //		if(valueType == ASTOX_VALUE_STRING){
            //			cout << " get prop " << keyName << endl;
            //		}
            int type = list.at(index).type;




            if (type == ST_EVAL) {
                keyName = ecma::runCommand(keyName, context)->toString();
                if (stringAlg::stringIsNumber(keyName)) {
                    type = ST_NUMERIC;
                }
            }
#ifdef ASTX_DBG_ECMA
            cout << __LINE__ << DBG_FN(__FILENAME__) << " ";
            cout << lang::toJSON(&(*this), "", "") << " get keyName " << keyName << endl;
#endif

            if (index + 1 < (int) list.size()) {
                int nextType = list.at(index + 1).type;
                //			cout << "ASTOX_VALUE_ARRAY" << keyName << " next is [" << list.at(index+1).data  << "]"<< endl;

                //daca urmeaza argumente
                if (nextType == ST_ARG) {
                    //TODO get next

                    list.at(index + 1).data = stringAlg::trim(list.at(index + 1).data);
                    //				cout << " call " << keyName << " with args " << list.at(index+1).data << endl;
                    //				Value * args = Value::npos;
                    //				if(!list.at(index+1).data.empty()) {
                    //					args = getParsedArgs(list.at(index+1).data, context);
                    //				}

                    if (index + 2 < (int) list.size()) {
                        //					cout << " third data: [" << list.at(index+2).data << "]" << endl;
                        if (type == ST_NUMERIC) {
                            return itemAtIndex(keyName)->call(list.at(index + 1).data, context)->getRecursive(list, index + 2, context, getType);
                        }

                        if (objectList.find(keyName) != objectList.end()) {
                            return objectList[keyName]->call(list.at(index + 1).data, context)->getRecursive(list, index + 2, context, getType);
                        }
                    }
                    else {

                        if (type == ST_NUMERIC) {
                            return itemAtIndex(keyName)->call(list.at(index + 1).data, context);
                        }

                        if (objectList.find(keyName) != objectList.end()) {
                            //						cout << "CALL METHOD " << lang::toJSON(objectList[keyName]) << " with args " << lang::toJSON(args) << endl;
                            return objectList[keyName]->call(list.at(index + 1).data, context);
                        }
                    }

                    goto exitInErr;
                }

                if (type == ST_NUMERIC) {
                    return itemAtIndex(keyName)->getRecursive(list, index + 1, context, getType);
                }

                if (objectList.find(keyName) != objectList.end()) {
                    return objectList[keyName]->getRecursive(list, index + 1, context, getType);
                }
            }

            if (type == ST_NUMERIC) {
                return itemAtIndex(keyName);
            }

            if (objectList.find(keyName) != objectList.end()) {
                return objectList[keyName];
            }
        }

exitInErr:
        if (getType == GETR_SAFE_STR) {
            objectList[keyName] = new String(keyName);
            return objectList[keyName];
        }


        if (getType == GETR_SAFE_NPOS) {
            return objectList[keyName] = Value::npos;
        }


        if (valueType == ASTOX_VALUE_FUNCTION) {
            std::string err_mesage = lang::toJSON(this);
            err_mesage.append(" has no prop ");
            err_mesage.append(keyName);
            Function::cast(this)->debug();
            Error err(Error::PARSING_ERROR, err_mesage, 504, __FILENAME__);
#ifndef ASTX_DISABLE_EXCEPTIONS
            throw err;
#endif
        }



        return objectList[keyName] = Value::npos;
    }

    astox::Value * Value::setRecursive(std::vector<SelectorNode> &list, int index, Value * newValue, Function *& context) {

        string keyName;
        if (index < (int) list.size()) {
            keyName = list.at(index).data;
            int type = list.at(index).type;

            if (type == ST_EVAL) {
                keyName = ecma::runCommand(keyName, context)->toString();
                if (stringAlg::stringIsNumber(keyName)) {
                    type = ST_NUMERIC;
                }
            }
            //daca mai are in lista:
            if (index + 1 < (int) list.size()) {
                if (type == ST_NUMERIC) {
                    return itemAtIndex(keyName)->setRecursive(list, index + 1, newValue, context);
                }
                return objectList[keyName]->setRecursive(list, index + 1, newValue, context);
            }

            if (type == ST_NUMERIC) {
                return updateValue(keyName, newValue);
            }
            //		if(objectList.find(keyName) != objectList.end()) {
            //			delete objectList[keyName];
            //		}
            return objectList[keyName] = newValue;

        }
        else {
            std::string err_mesage = lang::toJSON(this);
            err_mesage.append(" has no prop ");
            err_mesage.append(keyName);
            Error err(Error::PARSING_ERROR, err_mesage, __LINE__, __FILENAME__);
            if (valueType == ASTOX_VALUE_FUNCTION) {
                Function::cast(this)->debug();
            }
#ifndef ASTX_DISABLE_EXCEPTIONS
            throw err;
#endif
        }
        return Value::npos;
    }











































} /* namespace astox */
