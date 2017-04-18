/*
 * Function.cpp
 *
 *  Created on: Mar 13, 2014
 *      Author: alexandru
 */

#include "astox.h"

namespace astox {

    bool Function::_ecmaContextCreated = false;


    Function * Function::npos = new Function();
    Function * Function::_ecmaContext = NULL;

    Function * Function::getEcmaContext() {
        static Function f;
        f.addKey("print", new Function(ecma::methods::print));
        return &f;
        if (!_ecmaContextCreated) {
            _ecmaContext = new Function();
            _ecmaContext->addKey("print", new Function(ecma::methods::print));

            Function * arrayConstructor = new Function(ecma::methods::Array_constructor, new Object());
            _ecmaContext->addKey("Array", arrayConstructor);

            Function * stringConstructor = new Function(ecma::methods::String_constructor, new Object());
            _ecmaContext->addKey("String", stringConstructor);

            Function * numberConstructor = new Function(ecma::methods::Number_constructor, new Object());
            _ecmaContext->addKey("Number", numberConstructor);

            Function * objectConstructor = new Function(ecma::methods::Object_constructor, new Object());
            _ecmaContext->addKey("Object", objectConstructor);

            Function * booleanConstructor = new Function(ecma::methods::Boolean_constructor, new Object());
            _ecmaContext->addKey("Boolean", booleanConstructor);

            _ecmaContext->addKey("RegEx", new RegExp());
            _ecmaContext->addKey("astox_info", new Function(ecma::methods::astox_info));


            Function * dateConstructor = new Function(ecma::methods::Date_constructor, new Object());
            _ecmaContext->addKey("Date", dateConstructor);


            Function * setIntervalFunction = new Function(ecma::methods::setInterval, new Object());
            _ecmaContext->addKey("setInterval", setIntervalFunction);

            Function * requireConstructor = new Function(ecma::methods::Require_constructor, new Object());
            _ecmaContext->addKey("require", requireConstructor);


            _ecmaContext->addKey("Math", Math::getInstance());
            _ecmaContextCreated = true;
        }

        return _ecmaContext;
    }

    Function::Function() {
        valueType = ASTOX_VALUE_FUNCTION;
        protoype = Value::npos;
        _methodToCall = NULL;
        _prototype_methodToCall = NULL;
        _bodyContent = ASTOX_TOSTRING_NATIVE_FUNC;
        _proto_Object = NULL;
        objectList["prototype"] = protoype;
    }

    Function::Function(Function * parent) {
        _bodyContent = ASTOX_TOSTRING_NATIVE_FUNC;
        protoype = Value::npos;
        valueType = ASTOX_VALUE_FUNCTION;
        std::map<std::string, astox::Value *>::iterator it;
        for (it = parent->objectList.begin(); it != parent->objectList.end(); it++) {
            objectList[it->first] = it->second;
        }
        _methodToCall = NULL;
        _prototype_methodToCall = NULL;
        _proto_Object = NULL;
        objectList["prototype"] = protoype;
    }

    Function::Function(astox_function_type _pmethodToCall) {
        protoype = Value::npos;
        valueType = ASTOX_VALUE_FUNCTION;
        _bodyContent = ASTOX_TOSTRING_NATIVE_FUNC;
        _methodToCall = _pmethodToCall;
        _prototype_methodToCall = NULL;
        _proto_Object = NULL;
        objectList["prototype"] = protoype;
    };

    
    Function::Function(std::string str_body, std::string str_args, Function * parent) {
        protoype = Value::npos;
        valueType = ASTOX_VALUE_FUNCTION;
        std::map<std::string, astox::Value *>::iterator it;
        for (it = parent->objectList.begin(); it != parent->objectList.end(); it++) {
            objectList[it->first] = it->second;
        }
        _methodToCall = NULL;
        _argsNames = ecma::splitByComma(str_args);
        _bodyContent = str_body;
        _prototype_methodToCall = NULL;
        _proto_Object = NULL;
        objectList["prototype"] = protoype;
    }

    void Function::setParent(Function *& parent) {
        std::map<std::string, astox::Value *>::iterator it;
        for (it = parent->objectList.begin(); it != parent->objectList.end(); it++) {
            objectList[it->first] = it->second;
        }
    };

    Function::Function(astox_function_prototype _p_prototype_methodToCall, Value * _p_proto_object) {
        protoype = _p_proto_object;
        valueType = ASTOX_VALUE_FUNCTION;
        _methodToCall = NULL;
        _prototype_methodToCall = _p_prototype_methodToCall;
        _proto_Object = _p_proto_object;
        objectList["prototype"] = protoype;
    }

    Value * Function::call(std::string &args, Function *&context) {
        return call(ecma::runCommand(args, context));
    };

    Value * Function::call(Value * arguments) {

        if (_prototype_methodToCall != NULL && _proto_Object != NULL) {
            numCalls++;
            return _prototype_methodToCall(arguments, _proto_Object);
        }

        if (_methodToCall != NULL) {
            numCalls++;
            return _methodToCall(arguments);
        }

        Array * crnt;
        if (arguments->isArray()) {
            crnt = Array::cast(arguments);
        } else {
            crnt = new Array();
            crnt->push(arguments);
        }

        for (unsigned int i = 0; i < _argsNames.size(); i++) {
            objectList[_argsNames.at(i)] = crnt->itemAtIndex(i);
        }

        if (!_bodyContent.empty()) {
            #ifdef ASTX_DBG_ECMA
                cout << __LINE__ << DBG_FN(__FILENAME__) << " Function::call " << _bodyContent << lang::toJSON(arguments) << endl;
            #endif
            Function * cp = this;
            std::string copybody = _bodyContent;
            ecma::execScript(copybody, cp);
        }

        delete crnt;
        numCalls++;
        objectList["prototype"] = protoype;
        return protoype;
    };

    void Function::debug() {
        astox_debug(__LINE__, __FILENAME__, "   Function::debug");
        std::map<std::string, astox::Value *>::iterator it;
        for (it = objectList.begin(); it != objectList.end(); it++) {
            printf("   -- %s", it->first.c_str());
            if (it->second) {
                printf(" [%s] = %s\n\n", it->second->getStrType().c_str(),
                        astox::lang::toJSON(it->second, "", "").c_str());
            } else {
                printf("NULL\n\n");
            }
        }
    }

    std::string Function::toString() {
        std::string rsp = "function (";
        for (unsigned int i = 0; i < _argsNames.size(); i++) {
            if (i > 0) {
                rsp.append(",");
            }
            rsp.append(_argsNames.at(i));
        }
        rsp.append("){");
        rsp.append(_bodyContent);
        rsp.append("}");
        return rsp;
    }

    Function::~Function() {
        // TODO Auto-generated destructor stub
    }

    void Function::import(Function * parent) {
        using namespace std;
        for (std::map<string, Value *>::iterator it = parent->getObjectList()->begin();
                it != parent->getObjectList()->end(); ++it) {
            objectList[it->first] = it->second;
        }
    }

    void Function::updatePrototype(Value * val) {
        protoype = val;
        objectList["prototype"] = protoype;
    }

    bool Function::prototypeIsObject() {
        int protoValType = protoype->getType();
        return (protoValType == ASTOX_VALUE_OBJECT || protoValType == ASTOX_VALUE_ARRAY || protoValType == ASTOX_VALUE_FUNCTION);
    };

    astox::Function * Function::cast(Value * value) {
#ifdef ASTX_VALUE_STATIC_CAST
        return static_cast<Function *> (value);
#else
        return dynamic_cast<Function *> (value);
#endif
    };












} /* namespace astox */
