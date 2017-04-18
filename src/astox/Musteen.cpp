/*
 * Musteen.cpp
 *
 *  Created on: Sep 26, 2014
 *      Author: alexandru
 */

#include "Musteen.h"

namespace astox {

    using namespace std;

    MusTag::MusTag(std::string input, bool asBracket) {
        tagType = 0;
        parse(input, asBracket);
    };

    int MusTag::getType() {
        return tagType;
    };

    void MusTag::parse(std::string input, bool asBracket) {
        original = input;

        if (!asBracket) {
            key = input;
            tagType = MST_TEXT_ONLY;
            return;
        }

        if (input.size() > 4) {

            if (input.at(2) == ':') {
                key = input.erase(2, 1);
                tagType = MST_TEXT_ONLY;
                return;
            }

            key = input.substr(2, input.size() - 4);

            switch (key.at(0)) {

                case '^':
                    key = key.substr(1, key.size() - 1);
                    tagType = MST_FALSE_KEY;
                    break;

                case '#':
                    key = key.substr(1, key.size() - 1);
                    tagType = MST_TRUE_KEY;
                    break;

                case '/':
                    key = key.substr(1, key.size() - 1);
                    tagType = MST_END_KEY;
                    break;

                case '.':
                    tagType = MST_LIST_INDEX;
                    break;

                case '!':
                    if (key.size() == 1) {
                        tagType = MST_LIST_NUM;
                    } else {
                        tagType = MST_DEFAULT;
                    }
                    break;

                    //			case '&':
                    //				key = key.substr(1, key.size()-1);
                    //				tagType = MST_PARTIAL;
                    //				break;

                    //			case '>':
                    //				key = key.substr(1, key.size()-1);
                    //				tagType = MST_PARTIAL;
                    //				break;

                    //			case ':':
                    //				key = key.substr(1, key.size()-1);
                    //				key = "{{" + key + "}}";
                    //				tagType = MST_TEXT_ONLY;
                    //				break;

                default:
                    tagType = MST_DEFAULT;
                    break;
            }
        }

        key = astox::stringAlg::trim(key);

        //	cout << "-----------------------> " << key << endl;


    }

    std::string MusTag::toString() {
        std::string response;
        switch (tagType) {
            case MST_FALSE_KEY:
                response = "<^" + key + ">";
                break;
            case MST_LIST_NUM:
                response = "<^" + key + ">";
                break;
            case MST_TRUE_KEY:
                response = "<#" + key + ">";
                break;
            case MST_LIST_INDEX:
                response = "<{{.}}>";
                break;
            case MST_END_KEY:
                response = "</" + key + ">";
                break;
            case MST_DEFAULT:
                response = "<" + key + "/>";
                break;
            case MST_TEXT_ONLY:
                response = "..." + key + "....";
                break;
        }

        return response;
    }

    Musteen::Musteen() {
        escapeDefaultToHTML = false;
    }

    Musteen::~Musteen() {
        // TODO Auto-generated destructor stub
    }

    std::string Musteen::parse(std::string input, Object * context) {
        _context = context;

        using namespace std;

        int numB = 0;
        string temp;

        for (unsigned int i = 0; i < input.size(); i++) {
            char c = input.at(i);


            switch (c) {
                case '{':
                    if (numB == 0) {



                        if (!temp.empty()) {
                            MusTag tag(temp, false);
                            _tagList.push_back(tag);
                        }
                        temp.clear();
                    }

                    temp += c;
                    numB++;
                    break;

                case '}':
                    temp += c;
                    numB--;
                    if (numB == 0) {
                        //result.append(context.get(temp));
                        if (!temp.empty()) {
                            //						cout << "BRACKET TAG:" << temp.at(0) << endl;
                            MusTag tag(temp, true);
                            _tagList.push_back(tag);
                        }
                        temp.clear();
                    }
                    break;
                default:
                    temp += c;
                    break;
            }


        }

        if (!astox::stringAlg::trim(temp).empty()) {
            MusTag tag(temp, false);
            _tagList.push_back(tag);
        }

        return iterateList(_tagList, _context, _context);

    }

    enum ITERATOR_STAT {
        IT_DEFAULT = 2,
        IT_LIST_REQUESTED,
        IT_ENCLOSION_STARTED,
        IT_OBJECT_JUST_PUSHED
    };

    std::string Musteen::iterateList(std::vector<MusTag> _tagList, Object * context, Object * _globalContext, std::string debugName, bool trackAsArray, int numArrayIndeces, int nextNumChilds) {
        using namespace std;
        std::string _response;

        bool allowNode = true;
        int numChilds = 0; //numara child-urile urmatorului posibil array
        int numOpened = 0;
        std::vector<MusTag> _tempBuffer;
        Value * _nextContext = Value::npos;

        if (trackAsArray) {

            cout << nextNumChilds << endl;
            if (nextNumChilds) {
                Array * _carray = Array::cast(context);
                for (unsigned int j = 0; j < _carray->getItems()->size(); j++) {
                    if (_carray->itemAtIndex(j)->isObject()) {
                        _response.append(iterateList(_tagList, Object::cast(_carray->itemAtIndex(j)), _globalContext, "root", false, j));
                    } else {
                        _response.append(iterateList(_tagList, _carray, _globalContext, "root", false, j));
                    }
                }
            }
            else {
                _response.append(iterateList(_tagList, _globalContext, _globalContext, "root", false, 0));
            }
            return _response;
        }


        for (unsigned int i = 0; i < _tagList.size(); i++) {
            int type = _tagList.at(i).getType();
            //			cout << _tagList.at(i).toString() << type << endl;

            switch (type) {
                case MST_TEXT_ONLY:
                    if (numOpened == 0) {
                        _response.append(_tagList.at(i).key);
                    } else {
                        _tempBuffer.push_back(_tagList.at(i));
                    }

                    break;

                case MST_LIST_INDEX:
                    numChilds++;
                    if (numOpened > 0) {
                        _tempBuffer.push_back(_tagList.at(i));
                    } else {
                        std::ostringstream oss;
                        oss << numArrayIndeces;
                        _response.append(oss.str());
                    }
                    break;

                case MST_LIST_NUM:
                    numChilds++;
                    if (numOpened > 0) {
                        _tempBuffer.push_back(_tagList.at(i));
                    } else if (context->isArray()) {
                        _response.append(Array::cast(context)->itemAtIndex(numArrayIndeces)->toString());
                    } else {
                        _response.append(context->toString());
                    }
                    break;

                case MST_DEFAULT:
                    //numChilds++;
                    if (numOpened == 0) {
                        _response.append(returnDefaultKey(_tagList.at(i), context, _globalContext));
                    } else {
                        _tempBuffer.push_back(_tagList.at(i));
                    }

                    break;

                case MST_TRUE_KEY:
                    //numChilds++;
                    if (numOpened == 0) {
                        _nextContext = context->getValue(_tagList.at(i).key);
                        allowNode = (_nextContext != Value::npos);


                        //					cout << _tagList.at(i).key << " MUST EXIST " << "allowNode = " << allowNode << endl;
                        //					cout << _tagList.at(i).key << " = " << astox::lang::toJSON(_nextContext) << endl;
                        //					cout << debugName << " = " << astox::lang::toJSON(context) << endl;
                    } else {
                        _tempBuffer.push_back(_tagList.at(i));
                    }

                    numOpened++;
                    break;

                case MST_FALSE_KEY:


                    if (numOpened == 0) {
                        _nextContext = context->getValue(_tagList.at(i).key);
                        allowNode = (_nextContext == Value::npos);
                        //					cout << _tagList.at(i).key << " MUST NOT EXIST " << "allowNode = " << allowNode << endl;
                    } else {
                        _tempBuffer.push_back(_tagList.at(i));
                    }

                    numOpened++;

                    break;

                case MST_END_KEY:
                    numOpened--;

                    if (numOpened == 0) {
                        if (allowNode) {

                            if (_nextContext->isObject()) {
                                cout << _tagList.at(i).key << " MUSTEXIT|NOT " << "allowNode = " << allowNode << _nextContext->getStrType() << endl;

                                _response.append(iterateList(_tempBuffer, Object::cast(_nextContext), _globalContext, _tagList.at(i).key, _nextContext->isArray(), 0, numChilds));
                                numChilds = 0;
                                _tempBuffer.clear();
                            } else {
                                //_response.append(returnDefaultKey(_tagList.at(i), context, _globalContext));
                                _response.append(iterateList(_tempBuffer, context, _globalContext, "root", context->isArray(), 0));
                                _tempBuffer.clear();
                            }
                        } else {
                            _tempBuffer.clear();
                        }
                    }
                    else {
                        _tempBuffer.push_back(_tagList.at(i));
                    }


                    break;


                default:
                    if (numOpened == 0) {
                        _response.append(_tagList.at(i).key);
                    }
                    else {
                        _tempBuffer.push_back(_tagList.at(i));
                    }
                    break;


            }



        }



        return _response;
    }

    std::string Musteen::htmlEscape(std::string input) {

        std::string::size_type pos = 0;
        for (;;) {
            pos = input.find_first_of("\"&<>", pos);
            if (pos == std::string::npos) break;
            std::string replacement;
            switch ((input)[pos]) {
                case '\"': replacement = "&quot;";
                    break;
                case '&': replacement = "&amp;";
                    break;
                case '<': replacement = "&lt;";
                    break;
                case '>': replacement = "&gt;";
                    break;
                default:;
            }
            input.replace(pos, 1, replacement);
            pos += replacement.size();
        };

        return input;
    }

    std::string Musteen::returnDefaultKey(MusTag & _tag, Object * context, Object * _globalContext) {

        std::string _cstr;
        bool returnEscaped = false;

        if (_tag.key.at(0) == '&') {
            returnEscaped = true;
            _cstr = _tag.key.substr(1, _tag.key.size() - 1);
            _cstr = astox::stringAlg::trim(_cstr);
        } else {
            _cstr = _tag.key;
        }


        std::vector<SelectorNode> data = ecma::extractPatternList(_cstr);

        //TODO pus un context
        int st = 0;
        Value * resVal = context->getRecursive(data, st, Function::npos, GETR_SAFE_STR);

        //Value::npos; //basicParser.getCommand(_cstr, context);

        if (resVal == Value::npos) {
            resVal = _globalContext->getRecursive(data, st, Function::npos, GETR_SAFE_STR);
            //Value::npos; //basicParser.getCommand(_cstr, _globalContext);
        }

        if (resVal == Value::npos) {
            return _tag.original;
        }

        if (escapeDefaultToHTML) {
            if (!returnEscaped) {
                return htmlEscape(resVal->toString());
            }

        }

        return resVal->toString();
    }

} /* namespace astox */
