/*
 * Lang.cpp
 *
 *  Created on: Jan 27, 2014
 *      Author: alexandru
 */

#include "astox.h"

namespace astox {

    std::ostream& operator<<(std::ostream& output, const Object& val) {
        output << lang::toJSON((Value*) & val, "", "");
        return output;
    }

    std::ostream& operator<<(std::ostream& output, const Object * val) {
        output << lang::toJSON((Value*) val, "", "");
        return output;
    }

    std::ostream& operator<<(std::ostream& output, const Array& val) {
        output << lang::toJSON((Value*) & val, "", "");
        return output;
    }

    std::ostream& operator<<(std::ostream& output, const Array * val) {
        output << lang::toJSON((Value*) val, "", "");
        return output;
    }

    namespace lang {

        using namespace std;

        HashParser::HashParser() {
            lastGivenType = hash_npos;
        }

        void HashParser::refresh() {
            result.clear();
        }

        void HashParser::trace() {
            cout << "from : \n" << in << "\nkeys:" << endl;
            std::map<std::string, astox::Value *>::iterator it;
            for (it = result.begin(); it != result.end(); it++) {
                cout << "     " << it->first << "=" << it->second->toString() << endl;
            }
        }

        void HashParser::receiveInput(std::string input, int type, int info) {
            string dbg = lastGivenType == hash_val ? "value" : (lastGivenType == hash_npos ? "npos" : "key");

            if (type == hash_val && lastGivenType == hash_prop) {
                result[lastReceivedInput] = ecma::fromStr(input);
                //cout << "ovv " << lastReceivedInput << " = "<< input << endl;
            } else if (type == hash_prop) {
                //cout << "key " << input << " = true "<< endl;
                result[input] = Boolean::New(true);
            } else {
                cout << "ERR" << endl;
            }

            lastReceivedInput = input;
            lastGivenType = type;
        };

        std::string HashParser::toHash(Object * object, char separator) {


            using namespace std;
            map<string, astox::Value *>::iterator it;
            string response;
            scannedObjects.clear();

            unsigned int cnt = 0;
            if (object->isObject()) {

                for (it = object->getObjectList()->begin(); it != object->getObjectList()->end(); it++) {
                    cnt++;
                    response.append(it->first);
                    response += '=';

                    if (it->second->isNumber() || it->second->isBoolean()) {
                        response.append(it->second->toString());
                    }
                    else {
                        response += '"';
                        response.append(it->second->toString());
                        response += '"';
                    }

                    if (cnt < object->getObjectList()->size()) {
                        response += separator;
                    }

                }
            }

            else if (object->isArray()) {
                Array * crnt = Array::cast(object);
                for (int i = 0; i < crnt->length(); i++) {

                    string val = crnt->itemAtIndex(i)->toString();

                    if (crnt->itemAtIndex(i)->getType() == ASTOX_VALUE_BOOL
                            || crnt->itemAtIndex(i)->getType() == ASTOX_VALUE_NUMBER) {
                        response.append(val);
                    }
                    else {
                        response += '"';
                        response.append(val);
                        response += '"';
                    }

                    if (i < crnt->length() - 1) {
                        response += separator;
                    }
                }
            }
            return response;
        }

        std::map<std::string, astox::Value *> HashParser::fromHash(std::string input) {

            in = input;
            int numq = 0;
            string value;
            string key;

            int keyStat = hash_prop;

            char b = '\0';

            for (unsigned int i = 0; i < input.size(); i++) {
                char c = input.at(i);
                if (i > 0) {
                    b = input.at(i - 1);
                }


                switch (c) {

                    case ' ':
                    case '\n':
                    case '\t':
                        //case '\r\n':  // leave this line only for windows
                        if (keyStat == hash_val) {
                            if (numq % 2 != 0) {
                                value += c;
                            } else {


                                value = astox::stringAlg::trim(value);

                                if (!value.empty()) {
                                    receiveInput(value, hash_val, 5);
                                    keyStat = hash_prop;
                                    value.clear();
                                }


                            }

                        } else if (keyStat == hash_prop) {
                            key = astox::stringAlg::trim(key);
                            if (!key.empty()) {
                                receiveInput(key, hash_prop, 1);
                                value.clear();
                                key.clear();
                            }

                        }

                        break;

                    case '"':
                        numq++;
                        if (numq % 2 == 0) {
                            value += c;
                            keyStat = hash_prop;
                            receiveInput(value, hash_val, 3);
                            value.clear();
                            key.clear();
                        } else if (keyStat == hash_val) {
                            value += c;
                        }
                        break;

                    case '=':
                        if (numq % 2 == 0) {
                            keyStat = hash_val;
                            key = astox::stringAlg::trim(key);
                            if (!key.empty()) {
                                receiveInput(key, hash_prop, 4);
                                value.clear();
                                key.clear();
                            }

                        } else {
                            value += c;
                        }
                        break;


                    default:
                        if (keyStat == hash_prop) {
                            key += c;
                        }
                        else {
                            value += c;
                        }
                        break;

                }

            }
            key = astox::stringAlg::trim(key);
            value = astox::stringAlg::trim(value);

            if (!key.empty()) {
                receiveInput(key, hash_prop, 7);
            }

            if (!value.empty()) {
                receiveInput(value, hash_val, 7);
            }

            return result;

        }

        std::string toJsonLine(astox::Value * value, std::string prefix, std::string suffix, std::string parentPrefix) {
            Object * current;
            Array * carray;
            std::map<std::string, astox::Value *>::iterator it;
            std::string response;
            int counter = 0;


            if (value->getType() == ASTOX_VALUE_BOOL) {
                return value->toString();
            }

            if (value->getType() == ASTOX_VALUE_STRING) {
                response += '"';
                response.append(value->toString());
                response += '"';
                return response;
            }

            if (std::find(scannedObjects.begin(), scannedObjects.end(), value->getUid()) != scannedObjects.end()) {
                return "\"$this\"";
            }
            if (value->getType() != ASTOX_VALUE_BOOL ||
                    value->getType() != ASTOX_VALUE_STRING ||
                    value->getType() != ASTOX_VALUE_NUMBER) {
                scannedObjects.push_back(value->getUid());
            }


            std::string next = prefix;
            if (!prefix.empty()) {
                next += prefix.at(0);
            }




            switch (value->getType()) {

                case ASTOX_VALUE_NUMBER:
                case ASTOX_VALUE_BOOL:
                    //response.clear();
                    response.append(value->toString());
                    return response;

                case ASTOX_VALUE_STRING:
                    //response.clear();
                    response += '"';
                    response.append(value->toString());
                    response += '"';
                    return response;

                case ASTOX_VALUE_OBJECT:
                    current = Object::cast(value);
                    //response.clear();
                    //response.append(prefix);
                    response += '{';
                    response.append(suffix);

                    for (it = current->getObjectList()->begin(); it != current->getObjectList()->end(); it++) {
                        response.append(prefix);
                        response.append("\"");
                        response.append(it->first);
                        response.append("\":");
                        response.append(toJsonLine(it->second, next, suffix, prefix));
                        counter++;
                        if (counter < (int) current->getObjectList()->size()) {
                            response.append(",");

                        }
                        response.append(suffix);
                    }


                    response.append(parentPrefix);
                    response += '}';
                    return response.c_str();


                case ASTOX_VALUE_ARRAY:
                    carray = Array::cast(value);
                    //response.clear();
                    response += '[';
                    response.append(suffix);

                    for (int i = 0; i < carray->length(); i++) {
                        response.append(prefix);
                        response.append(toJsonLine(carray->itemAtIndex(i), next, suffix, prefix));
                        if (i < (carray->getItems()->size() - 1)) {
                            response.append(",");
                        }
                        response.append(suffix);
                    }

                    response.append(parentPrefix);
                    response += ']';
                    return response;

                case ASTOX_VALUE_UNDEFINED:
                    return ASTOX_VALUE_STR_UNDEFINED;

                case ASTOX_VALUE_FUNCTION:
                    return value->toString();

                default:
                    return "undefined value type";

            }//exit swicth


            return "erooro";
        }

        void iterateValue(astox::Value * value, void (onIterate) (std::string keyName, astox::Value * value), std::string pKeyName) {
            Object * current;
            Array * carray;



            std::map<std::string, astox::Value *>::iterator it;


            switch (value->getType()) {

                case ASTOX_VALUE_NUMBER:
                case ASTOX_VALUE_BOOL:
                case ASTOX_VALUE_STRING:
                    if (onIterate != NULL) {
                        onIterate(pKeyName, value);
                    }
                    return;

                case ASTOX_VALUE_OBJECT:
                    current = Object::cast(value);

                    if (onIterate != NULL) {
                        onIterate(pKeyName, value);
                    }

                    for (it = current->getObjectList()->begin(); it != current->getObjectList()->end(); it++) {
                        iterateValue(it->second, onIterate, it->first);
                    }


                    return;


                case ASTOX_VALUE_ARRAY:
                    carray = Array::cast(value);

                    if (onIterate != NULL) {
                        onIterate(pKeyName, carray);
                    }
                    for (unsigned int i = 0; i < carray->getItems()->size(); i++) {
                        iterateValue(carray->itemAtIndex(i), onIterate, pKeyName);
                    }


                    return;

                case ASTOX_VALUE_UNDEFINED:
                    return;

                default:
                    return;

            }//exit swicth


            return;
        }

        std::string toJSON(astox::Value * value, std::string prefix, std::string suffix) {
            scannedObjects.clear();
            return toJsonLine(value, prefix, suffix, "");
        }

        std::string toXMLLine(astox::Value * value, astox::xhtml::XMLKeyNames names, std::string nodeName, std::string prefix, std::string suffix) {
            using namespace std;
            map<string, Value *>::iterator it;

            string response;
            string tagName;
            string next = prefix;
            if (!prefix.empty()) {
                next += (prefix.at(0));
            }


            if (!nodeName.empty()) {
                tagName = nodeName;
            } else {
                tagName = value->getStrType();
            }




            switch (value->getType()) {

                case ASTOX_VALUE_STRING:
                case ASTOX_VALUE_NUMBER:
                case ASTOX_VALUE_BOOL:
                case ASTOX_VALUE_UNDEFINED:
                    response.append(prefix);
                    response += '<';
                    response.append(tagName);
                    response += '>';
                    response.append(value->toString());
                    response += '<';
                    response += '/';
                    response.append(tagName);
                    response += '>';
                    response.append(suffix);

                    break;

                case ASTOX_VALUE_OBJECT:
                    if (names.run_in_OBJ) {
                        //cout << "il vom folosi" << endl;
                        //
                        Object * current = Object::cast(value);

                        if (current->getValue(names.isVirtual) != Value::npos) {
                            response = (prefix);
                            response.append(current->getValue(names.value)->toString());
                            response.append(suffix);
                            return response;
                        }

                        if (current->getValue(names.tagName) != Value::npos) {
                            tagName = current->getValue(names.tagName)->toString();
                            //					cout << "tag " << tagName << endl;
                        }
                        response.append(prefix);
                        response += '<';
                        response.append(tagName);

                        if (current->getValue(names.attributes) != Value::npos) {


                            Object * attrs = current->getObject(names.attributes);
                            string data = astox::lang::HashParser::toHash(attrs);
                            response.append(" ");
                            response.append(data);
                        }

                        if (current->getValue(names.type) != Value::npos) {
                            int type = current->getValue(names.type)->toInt();
                            if (type == ASTX_NODE_STAT_SELF_CLOSED) {

                                response.append("/>");
                                response.append(suffix);
                                return response;
                            }
                        }

                        response += '>';
                        response.append(suffix);

                        if (current->getValue(names.childNodes) != Value::npos) {
                            Array * childs = current->getArray(names.childNodes); //Array::cast(current->getValue(names.childNodes));
                            for (unsigned int i = 0; i < childs->getItems()->size(); i++) {
                                response.append(toXMLLine(childs->getItems()->at(i), names, "", next, suffix));
                            }
                        }

                        response.append(prefix);
                        response += '<';
                        response += '/';
                        response.append(tagName);
                        response.append(">");
                        response.append(suffix);

                    }
                    else {
                        response.append(prefix);
                        response += '<';
                        response.append(tagName);
                        response += '>';
                        response.append(suffix);
                        std::map<std::string, astox::Value *>::iterator it;
                        Object * current = Object::cast(value);
                        for (it = current->getObjectList()->begin(); it != current->getObjectList()->end(); it++) {
                            response.append(toXMLLine(it->second, names, it->first, next, suffix));
                        }

                        response.append(prefix);
                        response += '<';
                        response += '/';
                        response.append(tagName);
                        response.append(">");
                        response.append(suffix);
                        //cout << "nu il vom folosi" << endl;
                    }
                    break;


                case ASTOX_VALUE_ARRAY:

                    Array * thisArr = Array::cast(value);
                    response.append(prefix);
                    response += '<';
                    response.append(tagName);
                    response += '>';
                    response.append(suffix);
                    for (int i = 0; i < thisArr->length(); i++) {
                        response.append(toXMLLine(thisArr->itemAtIndex(i), names, "", next, suffix));
                        //response.append(toXMLLine(thisArr->itemAtIndex(i), names, astox::stringAlg::doubleToString(i)));
                    }
                    response.append(prefix);
                    response += '<';
                    response += '/';
                    response.append(tagName);
                    response.append(">");
                    response.append(suffix);
                    break;
            }

            return response;
        }

        std::string toXML(astox::Value * value, bool scan_as_xhtml) {
            astox::xhtml::XMLKeyNames n;
            n.run_in_OBJ = scan_as_xhtml;
            return toXMLLine(value, n);
        }

        std::string toCSS(astox::Value * value, bool isRoot) {
            using namespace std;
            string response;
            std::map<std::string, astox::Value *>::iterator it;
            Object * current;

            switch (value->getType()) {

                case ASTOX_VALUE_OBJECT:
                    if (!isRoot) {
                        response.append("{");
                    }
                    current = Object::cast(value);
                    for (it = current->getObjectList()->begin(); it != current->getObjectList()->end(); it++) {
                        response.append(it->first);
                        response.append(":");
                        response.append(toCSS(it->second, false));
                    }

                    if (!isRoot) {
                        response.append("}\n");
                    }
                    break;

                default:
                    response.append(value->toString());
                    response.append(";");
                    break;
            }

            return response;
        }





    } /* namespace lang */
} /* namespace astox */
