/*
 * ECMAScript.cpp
 *
 *  Created on: Oct 28, 2014
 *      Author: Alexandru
 */

#include "astox.h"
#include "Macros.h"

namespace astox {

    using namespace std;
    using namespace stringAlg;



    void ScriptValueNode::debug() {
#ifdef ASTX_DBG_ECMA
        cout << __LINE__ << DBG_FN(__FILENAME__);
        cout << " ScriptValueNode:: " << ecma::operatorToString(operation) << lang::toJSON(value) << endl;
#endif
    }

    SelectorNode::SelectorNode(std::string _data, int _type) {
        data = _data;
        type = _type;
        subType = -34;
        if (type != ST_ARG && stringAlg::stringIsNumber(_data)) {
            type = ST_NUMERIC;
        }
        if (type != ST_NUMERIC) {
            size_t instPoz = _data.find(" instanceof ");
            if (instPoz != string::npos && _data.find(".") == string::npos) {
                string _instBefore = _data.substr(0, instPoz);
                _instAfter = _data.substr(instPoz + strlen(" instanceof "), _data.size());
                cout << " " << _instBefore << "[ is instance of ]" << _instAfter << endl;
                data = _instBefore;
                subType = STSUB_INSTANCEOF;
            }
        }
    }


    namespace ecma {


        const char * stx_keywords[16][2] = {
            {" var", " "},
            {" function", " ("},
            {" else if", " ("},
            {" if", " ("},
            {" else", " ("},
            {" switch", " ("},
            {" while", " ("},
            {" for", " ("},
            {" case", " :"},
            {" return", " [{(;"},
            {" break", " ;"},
            {" try", " {"},
            {" catch", " ("},
            {" finally", " {"},
            {" delete", " "},
            {" throw", " "}
        };

        int stx_size = 16;

        std::vector<std::string> splitByComma(std::string &input, char comma) {
            vector<string> r;
            string buff;
            int e = 0, f = 0, g = 0, h = 0, j = 0;
            for (unsigned int i = 0; i < input.size(); i++) {
                char c = input.at(i);
                char p = (i > 0) ? input.at(i - 1) : '\0';
                if (!ecma::charIsInString(c, p, e, f) &&
                        !ecma::charIsEnclosedBy(c, g, '(', ')') &&
                        !ecma::charIsEnclosedBy(c, h, '{', '}') &&
                        !ecma::charIsEnclosedBy(c, j, '[', ']') &&
                        c == comma) {
                    buff = stringAlg::trim(buff);
                    if (!buff.empty()) {
                        r.push_back(buff);
                    }
                    buff.clear();
                } else {
                    buff += c;
                }
            }
            buff = stringAlg::trim(buff);
            if (!buff.empty()) {
                r.push_back(buff);
            }
            return r;
        }

        bool charIsEnclosedBy(char c, int &nump, char start, char end) {
            if (c == start) {
                if (nump == 0) {
                    nump++;
                    return true;
                }
                nump++;
            }
            if (c == end) {
                nump--;
                if (nump == 0) {
                    return true;
                }
            }
            return (nump != 0);
        }

        std::string getAllUntilChar(std::string &input, int &poz, char cp) {
            std::string strf;
            int e = 0, f = 0, g = 0, h = 0, j = 0;
            poz = input.size() - 1;
            for (unsigned int i = 0; i < input.size(); i++) {
                char c = input.at(i);
                char p = (i > 0) ? input.at(i - 1) : '\0';
                if (!charIsInString(c, p, e, f) &&
                        !charIsEnclosedBy(c, g, '(', ')') &&
                        !charIsEnclosedBy(c, h, '{', '}') &&
                        !charIsEnclosedBy(c, j, '[', ']') &&
                        c == cp) {
                    poz = i;
                    break;
                } else {
                    strf += c;
                }
            }
            return strf;
        }

        void detectKeyword(std::string &input, std::string &beforeKeyword, int &detectedType, int &pozLeft, int &pozRight) {
            std::string strf; //debug use TODO delete
            bool found = false;
            beforeKeyword.clear();

            if (input.at(0) != ' ') {
                input.insert(0, " ");
            }

            for (int i = 0; i < stx_size; i++) {
                size_t pos = input.find(stx_keywords[i][0]);

                if (pos != string::npos && pos == 0) {
                    strf = stx_keywords[i][0];
                    unsigned int nextp = pos + strlen(stx_keywords[i][0]);
                    if (nextp < input.size()) {
                        bool oki = false;
                        char ca = input.at(nextp);
                        for (unsigned int j = 0; j < strlen(stx_keywords[i][1]); j++) {
                            if (ca == stx_keywords[i][1][j]) {
                                oki = true;
                            }
                        }

                        if (!oki) {
                            cout << stx_keywords[i][0] << " failed with " << input.at(nextp) << endl;
                        } else {
                            found = true;
                            pozRight = strf.size() + 1;
                            pozLeft = pos;
                            detectedType = i;
                            break;
                        }//end if oki
                    }//end if has next char
                }//end if found
            }// end for

            if (!found) {
                detectedType = ASTXKT_BSLINE;

                int lpos = 0;
                beforeKeyword = getAllUntilChar(input, lpos, ';');
                pozRight = lpos + 1;
                pozLeft = lpos;
                //		cout << " beforeKeyword = " << beforeKeyword << endl;
            }


        }

        bool stringIsWellFormed(std::string &input, char quote) {
            for (unsigned int i = 0; i < input.size(); i++) {
                char c = input.at(i);
                char p = (i > 0) ? input.at(i - 1) : '\0';
                if (c == quote && p != '\\') {
                    return false;
                }
            }
            return true;
        }

        bool isComparisonTerm(int operation) {
            return (operation == OP_BIGGER || operation == OP_LESSER || operation == OP_EQUALS || operation == OP_EQUALS_STRICT || operation == OP_BIGGER_EQ || operation == OP_LESSER_EQ
                    || operation == OP_DIFFERS || operation == OP_DIFFERS_STRICT || operation == OP_OR || operation == OP_OR_STRICT
                    || operation == OP_AND || operation == OP_AND_STRICT);
        }

        bool isOperator(int operation) {
            return (operation == OP_ADDITION || operation == OP_SUBSTRACT || operation == OP_MULTIPLY || operation == OP_DIVIDE || operation == OP_MODULUS);
        }

        bool operatorHasPriority(int operation) {
            return (operation == OP_MULTIPLY || operation == OP_DIVIDE || operation == OP_MODULUS);
        }

        bool isShiftOperator(int operation) {
            return (operation == OP_SHIFT_LEFT || operation == OP_SHIFT_RIGHT);
        }

        std::string operatorToString(int oprt) {
            switch (oprt) {
                case OP_ADDITION:
                    return "+";
                case OP_SUBSTRACT:
                    return "-";
                case OP_MULTIPLY:
                    return "*";
                case OP_DIVIDE:
                    return "/";
                case OP_BIGGER:
                    return ">";
                case OP_LESSER:
                    return "<";
                case OP_LESSER_EQ:
                    return "<=";
                case OP_BIGGER_EQ:
                    return ">=";
                case OP_EQUALS:
                    return "==";
                case OP_EQUALS_STRICT:
                    return "===";
                case OP_MODULUS:
                    return "%";
                case OP_INSTRUCTION_END:
                    return ";";
                case OP_UNSET:
                    return "unset";
                case OP_ASSIGN:
                    return "=";
                case OP_DIFFERS:
                    return "!=";
                case OP_DIFFERS_STRICT:
                    return "!==";
                case OP_OR:
                    return "|";
                case OP_OR_STRICT:
                    return "||";
                case OP_AND:
                    return "&";
                case OP_AND_STRICT:
                    return "&&";
                case OP_SHIFT_RIGHT:
                    return ">>";
                case OP_SHIFT_LEFT:
                    return "<<";
                case OP_PLUSPLUS:
                    return "++";
                case OP_MINMIN:
                    return "--";
            }

            std::stringstream ss;
            ss << oprt;
            string r = "unknown operator";
            r.append(ss.str());
            return r;
        }

        Number * fromStrAsNumber(std::string input) {
            return Number::New(atof(input.c_str()));
        }

        String * fromStrAsString(std::string str) {
            if (str == "\"\"" || str == "''") {
                return new String();
            }
            str = astox::stringAlg::escapeStringQuotes(str);
            return new String(str);
        }

        astox::Array * fromStrAsArray(std::string str, Function * context, bool useStrict) {

            using namespace std;

            int nrAcol = 0;
            int nrPaDr = 0;
            int numQuotes = 0;
            bool skip = false;
            char c;
            std::string propNamT;
            std::string propValT;

            Array * response = new Array();

            astox::stringAlg::trim(str);

            if (str.empty()) {
                return response;
            }

            for (unsigned i = 0; i < str.length(); ++i) {
                c = str.at(i);

                switch (c) {
                    case '{':
                        nrAcol++;
                        skip = false;
                        break;

                    case '}':
                        nrAcol--;
                        skip = false;
                        break;

                    case '[':
                        nrPaDr++;
                        skip = false;
                        break;

                    case ']':
                        nrPaDr--;
                        skip = false;
                        break;

                    case '\"':
                        numQuotes++;
                        skip = false;
                        break;

                    case '\n':
                    case '\t':
                    case '\r':
                    case '\f':
                        skip = true;
                        break;

                    case ',':
                        if (nrAcol == 0 && nrPaDr == 0 && numQuotes % 2 == 0) {
                            skip = true;
                            //				std::cout << "PUSH: " << propValT << std::endl;
                            response->push(fromStr(propValT, context, useStrict));
                            propValT.clear();
                        }
                        break;

                    default:
                        skip = false;
                        break;
                } //exit switch

                if (skip == false) {
                    propValT += c;
                }

            } //exit for

            if (!propValT.empty()) {
                response->push(fromStr(propValT));
            }

            return response;
        }

        astox::Object * fromStrAsObject(std::string str, Function * context, bool useStrict) {

            using namespace std;
            int nrAcol = 0;
            int nrPaDr = 0;

            bool skip = false;
            char c;

            string propName;
            string propVal;

            bool isProp = true;
            int numQuotes = 0;
            //	cout << "--> return " << str << " AS object " << endl;

            //	std::cout << "str not trimmed:| " << str << std::endl;
            astox::stringAlg::trim(str);

            Object * response = new Object();
            if (str.empty()) {
                return response;
            }
            //	std::cout << "return as object:| " << str << std::endl;
            for (unsigned i = 0; i < str.length(); i++) {

                c = str.at(i);

                switch (c) {
                    case '{':
                        nrAcol++;
                        skip = false;
                        break;

                    case '}':
                        nrAcol--;
                        skip = false;
                        break;

                    case '[':
                        nrPaDr++;
                        skip = false;
                        break;

                    case ']':
                        nrPaDr--;
                        skip = false;

                        break;

                    case '"':
                        numQuotes++;
                        skip = false;
                        break;

                    case '\n':
                    case '\t':
                    case '\r':
                    case '\f':
                        skip = true;
                        break;

                    case ' ':
                        if (isProp && numQuotes % 2 != 0) {
                            skip = true;
                        }
                        break;

                    case ',':
                        if (nrAcol == 0 && nrPaDr == 0 && numQuotes % 2 == 0) { //daca {} [] si "" sunt inchise
                            //						cout << "000 name: "<< propName << "=="<<  propVal << endl;
                            response->addKey(propName, fromStr(propVal, context, useStrict));

                            propName.clear();
                            propVal.clear();
                            skip = true;
                            isProp = true;
                        }
                        break;

                    case ':':
                        if (nrAcol == 0 && nrPaDr == 0) {
                            isProp = false;
                            skip = true;
                        }
                        break;

                    default:
                        skip = false;
                        break;
                } //exit switch

                if (skip == false) {
                    if (isProp) {
                        propName += c;
                    } else {
                        propVal += c;
                    }
                }

            } //exit for

            astox::stringAlg::trim(propVal);
            //	cout << "111 name: "<< propName << "=="<<  propVal << endl;
            if (!propName.empty()) {

                if (propVal.empty()) {
                    response->addKey(propName, new Object());
                } else {
                    response->addKey(propName, fromStr(propVal, context, useStrict));
                }

            }

            //	trace(response);
            return response;
        }

        Value * fromStr(std::string str, Function * context, bool useStrict) {
            using namespace std;
            int strType = detectStrType(str, useStrict);
            std::string res, func_args, func_body;
            int func_lpoz = 0;
            switch (strType) {
                case STRIN_EMPTY:
                    return new Value();
                case STRIN_STRING:
                    return fromStrAsString(str);
                case STRIN_VARNAME:
                    return ecma::getMember(str, context);
                case STRIN_NUMBER:
                    return fromStrAsNumber(str);
                case STRIN_TRUE:
                    return Boolean::BTRUE;
                case STRIN_FALSE:
                    return Boolean::BFALSE;
                case STRIN_OBJECT:
                    res = str.substr(1, str.length() - 2);
                    res = str.substr(1, str.size()); // scot primul
                    res = res.substr(0, res.size() - 1); //scot ultimul
                    return fromStrAsObject(res, context, useStrict);
                case STRIN_LIST:
                    res = str.substr(1, str.length() - 1);
                    res = str.substr(1, str.size()); // scot primul
                    res = res.substr(0, res.size() - 1); //scot ultimul
                    return fromStrAsArray(res, context, useStrict);
                case STRIN_FUNCTION:
                    if (useStrict) {
                        str = str.substr(strlen("function"), str.size());
                        func_args = getExpressionBetween(str, '(', ')', func_lpoz);
                        str = str.substr(func_lpoz, str.size());
                        func_body = getExpressionBetween(str, '{', '}', func_lpoz);
                        return new Function(func_body, func_args, context);
                    }
                    break;
            }
            return new String(str);
        }

        int detectStrType(std::string &input, bool useStrict) {
            input = astox::stringAlg::replaceWhiteSpace(input);
            input = astox::stringAlg::trim(input);

            if (input.empty()) {
                return STRIN_EMPTY;
            }

            if (input == "undefined") {
                return STRIN_UNDEFINED;
            }

            if (input == "true") {
                return STRIN_TRUE;
            }

            if (input == "false") {
                return STRIN_FALSE;
            }

            if (astox::stringAlg::stringIsNumber(input)) {
                return STRIN_NUMBER;
            }


            if (input == "\"\"") {
                return STRIN_STRING;
            }
            if (input == "''") {
                return STRIN_STRING;
            }

            if (input.size() >= 2) {
                char first = input.at(0);
                char last = *input.rbegin();

                if (first == '"' && last == '"' && !stringIsWellFormed(input, '"')) {
                    //TODO check if string is not 'sdsad', 34, 'dasdasd'
                    return STRIN_STRING;
                }

                if (first == '\'' && last == '\'' && !stringIsWellFormed(input, '\'')) {
                    return STRIN_STRING;
                }

                if (first == '[' && last == ']') {
                    return STRIN_LIST;
                }

                if (first == '{' && last == '}') {
                    return STRIN_OBJECT;
                }
                //		if (first == '(' && last == ')' && useStrict) {
                //			return STRIN_ENCAPSULATED;
                //		}
                if (first == '(' && useStrict) {
                    return STRIN_ENCAPSULATED;
                }
            }

            if (!useStrict) {
                return STRIN_STRING;
            }

            if (input.find("function ") == 0 || input.find("function(") == 0) {
                return STRIN_FUNCTION;
            }




            return STRIN_VARNAME;
        }

        bool charIsInString(char c, char p, int &_numberOfDoubleQuotes, int &_numberOfSingleQuotes) {

            bool answ = true;

            if (c == '\'' && p != '\\' && _numberOfDoubleQuotes % 2 == 0) {
                _numberOfSingleQuotes++;
                answ = false;
            }

            if (c == '"' && p != '\\' && _numberOfSingleQuotes % 2 == 0) {
                _numberOfDoubleQuotes++;
                answ = false;
            }

            if (_numberOfDoubleQuotes % 2 == 0 && _numberOfSingleQuotes % 2 == 0) {
                answ = false;
            }

            return answ;
        }

        std::string getExpressionBetween(std::string input, char s, char e,
                int &lastPosition) {
            size_t position = input.find(s);
            string answer;
            int numInnerP = 0;
            int nd = 0;
            int ns = 0;
            for (unsigned int i = position + 1; i < input.size(); i++) {
                char c = input.at(i);
                char p = (i > 0) ? input.at(i - 1) : '\0';

                if (!charIsInString(c, p, nd, ns)) {
                    if (c == s) {
                        numInnerP++;
                    }
                    if (c == e) {
                        if (numInnerP == 0) {
                            lastPosition = position + answer.size() + 2;
                            return answer;
                        }
                        numInnerP--;

                    }
                    answer += c;
                } else {
                    answer += c;
                }
            }
            lastPosition = position + answer.size() + 2;
            return answer;
        }

        void generatePatternList(std::vector<SelectorNode> &list, std::string &input, char waitFor) {
            int lpoz = 0, nd = 0, ns = 0, nextType = 0;
            string buffer;
            bool visited = false;
            for (unsigned int i = 0; i < input.size(); i++) {
                char c = input.at(i);
                char p = (i > 0) ? input.at(i - 1) : '\0';

                if (!charIsInString(c, p, nd, ns)) {

                    if (c == '(') {
                        buffer = input.substr(0, i);
                        buffer = stringAlg::trim(buffer);
                        if (!buffer.empty()) {
                            SelectorNode n(buffer, ST_BASIC_STR);
                            list.push_back(n);
                        }

                        input = input.substr(i, input.size());
                        string args = getExpressionBetween(input, '(', ')', lpoz);
                        SelectorNode narg(args, ST_ARG);
                        list.push_back(narg);
                        visited = true;
                        goto checkNext;
                    }


                    if (c == '[') {
                        buffer = input.substr(0, i);
                        buffer = stringAlg::trim(buffer);

                        if (!buffer.empty()) {
                            SelectorNode n(buffer, ST_BASIC_STR);
                            list.push_back(n);
                        }

                        input = input.substr(i, input.size());
                        string args = getExpressionBetween(input, '[', ']', lpoz);
                        SelectorNode narg(args, ST_EVAL);
                        list.push_back(narg);
                        visited = true;
                        goto checkNext;
                    }

                    if (c == '.') {
                        buffer = input.substr(0, i);
                        buffer = stringAlg::trim(buffer);

                        SelectorNode n(buffer, ST_BASIC_STR);
                        list.push_back(n);


                        lpoz = i + 1;
                        visited = true;
                        goto checkNext;
                    }


                }
            }



checkNext:
            if (!input.empty() && lpoz <= (int) input.size() && visited) {
                input = input.substr(lpoz, input.size());
                input = stringAlg::trim(input);

                if (!input.empty() && input != ";") {
                    if (input.at(0) == '.') {
                        input.erase(input.begin());
                    }
                    return generatePatternList(list, input, waitFor);
                }

            }
            else if (!input.empty()) {
                SelectorNode n(input, ST_BASIC_STR);
                list.push_back(n);
            }

        }

        std::vector<SelectorNode> extractPatternList(std::string input) {
            if (input.at(0) == '.') {
                input.erase(input.begin());
            }
            std::vector<SelectorNode> answer;
            generatePatternList(answer, input);
            return answer;
        }

        Value * setMember(std::string KeyName, Value * newValue, Function *& context) {
            int type = detectStrType(KeyName, true);
            if (type == STRIN_FALSE || type == STRIN_TRUE || type == STRIN_NUMBER || type == STRIN_STRING) {
                Error n(Error::ASSIGNMENT_ERROR, "unable to set primitive value", __LINE__, __FILENAME__);
            }


            std::vector<SelectorNode> data = extractPatternList(KeyName);


            if (data.at(0).data == "this") {
                data.erase(data.begin());
#ifdef ASTX_DBG_ECMA
                astox_debug(__LINE__, __FILENAME__, "ecma::setMember(this)");
#endif
                if (data.empty()) {
                    return context->protoype = newValue;
                } else {
                    if (!context->prototypeIsObject()) {
                        context->updatePrototype(new Object());
                    }
                    return context->protoype->setRecursive(data, 0, newValue, context);
                }
            }
            return context->setRecursive(data, 0, newValue, context);
        };

        Value * getMember(std::string keyName, Function * &context, int errFlags) {
			stx_dgb_script(ASTX_LOCATION, "getMember(%s)", keyName.c_str());
            Value * response = Value::npos;
            std::vector<SelectorNode> data;
            int lastPoz;
            string innerContent, rest;


            bool returnNew = false;
            int called = 0;
            if (keyName.empty()) {
                return Value::npos;
            }


            if (keyName.at(0) == '!') {
                keyName = keyName.substr(1, keyName.size());
                return getMember(keyName, context, errFlags)->negate();
            }


            if (keyName.find("typeof ") == 0) {
                keyName = keyName.substr(strlen("typeof "), keyName.size());
                return getMember(keyName, context, errFlags)->getTypeOf();
            }

            if (keyName.find("new ") == 0) {
                keyName = keyName.substr(4, keyName.size());
                returnNew = true;
				stx_trc_script(ASTX_LOCATION, "getMember(NEW %s)", keyName.c_str());
            }



            int type = detectStrType(keyName, context != Function::npos);

            string func_args;
            string func_body;
            int func_lpoz = 0;
            //	cout << "type: " << type << endl;

            switch (type) {
                case STRIN_NUMBER:
                    response = fromStrAsNumber(keyName);
                    break;
                case STRIN_TRUE:
                    response = Boolean::BTRUE;
                    break;
                case STRIN_FALSE:
                    response = Boolean::BFALSE;
                    break;
                case STRIN_STRING:
                    response = fromStrAsString(keyName);
                    break;
                case STRIN_OBJECT:
                case STRIN_LIST:
                    response = fromStr(keyName, context);
                    break;
                case STRIN_ENCAPSULATED:
                    //			cout << "ENCAPSULATED " << keyName << endl;
                    lastPoz = 0;
                    innerContent = getExpressionBetween(keyName, '(', ')', lastPoz);
                    innerContent = astox::stringAlg::trim(innerContent);
                    rest = keyName.substr(lastPoz, keyName.size());
                    rest = stringAlg::trim(rest);

					#ifdef ASTX_TRC_SCRIPT_ENGINE
						stx_trc_script(ASTX_LOCATION, "getMember exec (%s) with args (%s) rest type = (%i)", innerContent.c_str(), rest.c_str(), detectStrType(rest, context != Function::npos));
					#endif

                    if (!innerContent.empty() && rest.empty()) {
                        response = runCommand(innerContent, context);
                    } 
					else if (!innerContent.empty() && !rest.empty()) {
                    
						std::vector<SelectorNode> data = extractPatternList(rest);
						#ifdef ASTX_TRC_SCRIPT_ENGINE
							for (unsigned int i = 0; i < data.size(); i++) {
								data.at(i).debug();
							}
						#endif

                        if (data.size() == 1) {
                            response = runCommand(innerContent, context)->call(runCommand(data.at(0).data, context, errFlags));
                        }
                        else {
                            response = runCommand(innerContent, context);
                            if (data.at(0).type == ST_ARG) {
                                response = response->call(runCommand(data.at(0).data, context));
                                data.erase(data.begin());
                            }
                            if (!data.empty()) {
                                response = response->getRecursive(data, 0, context, errFlags);
                            }

                        }

                        //				response = runCommand(innerContent, context)->call(runCommand(rest, context));
                    }


                    break;
                case STRIN_FUNCTION:
                    keyName = keyName.substr(strlen("function"), keyName.size());
                    func_args = getExpressionBetween(keyName, '(', ')', func_lpoz);
                    keyName = keyName.substr(func_lpoz, keyName.size());
                    func_body = getExpressionBetween(keyName, '{', '}', func_lpoz);
                    response = new Function(func_body, func_args, context);
#ifdef ASTX_DBG_ECMA_GET_MEMBER
                    astox_debug(__LINE__, __FILENAME__, "ecma::getMember STRIN_FUNCTION args = %s body = ", func_args.c_str(), func_body.c_str());
#endif
                    break;

                default:
                    data = extractPatternList(keyName);
                    if (data.at(0).data == "this") {
                        data.erase(data.begin());
                        if (data.empty()) {
                            response = context->protoype;
                        } else {
                            response = context->protoype->getRecursive(data, 0, context, called);
                        }
                    } else {
                        if (data.at(0).type == ST_EVAL) {
#ifdef ASTX_DBG_ECMA_GET_MEMBER
                            astox_debug(__LINE__, __FILENAME__, "ecma::getMember starting with array ");
#endif
                            response = ecma::fromStrAsArray(data.at(0).data, context, context != Function::npos);
                            data.erase(data.begin());

                            if (!data.empty()) {
                                response = response->getRecursive(data, 0, context);
                            }
                        }
                        else {
                            //					cout << " la RegEx intru aici din " << lang::toJSON(response) << endl;
                            response = context->getRecursive(data, 0, context, errFlags); // pasesz errFlags pentru var suprim erori

                            if (data.at(data.size() - 1).subType == STSUB_INSTANCEOF) {

#ifdef ASTX_DBG_ECMA_GET_MEMBER
                                astox_debug(__LINE__, __FILENAME__, "ecma::getMember check if (%s) is instance of (%s)", lang::toJSON(response, "", "").c_str(), data.at(data.size() - 1)._instAfter.c_str());
#endif
                                return response->isInstanceOf(data.at(data.size() - 1)._instAfter);

                            }
                        }

                    }
                    break;

            }


            if (returnNew) {

                //		cout << "RETURN NEW" << response->getNumCalls() << lang::toJSON(response) << response->getStrType() << endl;

                int vltype = response->getType();
                bool typeIsAllowed = (vltype == ASTOX_VALUE_STRING || vltype == ASTOX_VALUE_NUMBER || vltype == ASTOX_VALUE_BOOL);
                if (typeIsAllowed && response->getNumCalls() > 0) {
                    return response;
                }

                if (response->getType() < ASTOX_VALUE_OBJECT || response->getType() == ASTOX_VALUE_FUNCTION) {
#ifdef ASTX_DBG_ECMA_GET_MEMBER
                    astox_debug(__LINE__, __FILENAME__, "ecma::getMember new object never called for type  (%s) ", response->getStrType().c_str());
#endif
                    response = response->call(Value::npos);
                    response->setNumCalls(0);
                }


                if (response->getType() < ASTOX_VALUE_OBJECT) {
#ifdef ASTX_DBG_ECMA_GET_MEMBER
                    astox_debug(__LINE__, __FILENAME__, "ecma::getMember new object is not object  for type  (%s) ", response->getStrType().c_str());
#endif
                    response = new Object();
                }
            }
            return response;
        }

        Value * operatePrioritized(std::vector<ScriptValueNode> items, Function * &context) {
            Value * result = Value::npos;
            int operation;

            for (unsigned int i = 0; i < items.size(); i++) {
                operation = items.at(i).operation;

                switch (operation) {
                    case OP_MODULUS:
                        result = result->modulus(items.at(i).value);
                        break;
                    case OP_MULTIPLY:
                        result = result->multiply(items.at(i).value);
                        break;
                    case OP_DIVIDE:
                        result = result->divide(items.at(i).value);
                        break;
                    default:
                        result = items.at(i).value;
                        break;
                }
            }
            return result;
        }

        Value * operate(std::vector<SScriptData *> &list, int index, Function * &context) {

            std::vector<ScriptValueNode> prioritized;
            std::vector<ScriptValueNode> localNodes;
            int currentOperation;
            int lastOperation = OP_UNSET;
            int lastLocalOperation = OP_UNSET;
            Value * currentVal = Value::npos;

            for (unsigned int i = 0; i < list.size(); i++) {
                //list.at(i)->debug();
                currentOperation = list.at(i)->getOperation();
                string buffer = list.at(i)->getBuffer();
                ScriptValueNode s;
                s.value = getMember(buffer, context);
                s.operation = lastOperation;
                prioritized.push_back(s);
                if (!operatorHasPriority(currentOperation)) {
                    currentVal = operatePrioritized(prioritized, context);
#ifdef ASTX_DBG_ECMA_OPERATE
                    astox_debug(__LINE__, __FILENAME__, "ecma::operate (%s) (%s) !operatorHasPriority", lang::toJSON(currentVal, "", "").c_str(), operatorToString(currentOperation).c_str());
#endif
                    ScriptValueNode local;
                    local.operation = lastLocalOperation;
                    local.value = currentVal;
                    localNodes.push_back(local);
                    lastLocalOperation = currentOperation;
                }

                lastOperation = currentOperation;
            }


            for (unsigned int i = 0; i < localNodes.size(); i++) {
                localNodes.at(i).debug();
                switch (localNodes.at(i).operation) {
                    case OP_ADDITION:
                        currentVal = currentVal->increment(localNodes.at(i).value);
                        break;
                    case OP_SUBSTRACT:
                        currentVal = currentVal->substract(localNodes.at(i).value);
                        break;
                    default:
                        currentVal = localNodes.at(i).value;
                        break;
                }
            }
            return currentVal;
        }

        Value * compare(astox::Value * leftOperand, astox::Value * rightOperand, int operation) {
#ifdef ASTX_DBG_ECMA_COMPARE
            astox_debug(__LINE__, __FILENAME__, "ecma::compare (%s) (%s) (%s)", lang::toJSON(leftOperand, "", "").c_str(),
                    operatorToString(operation).c_str(),
                    lang::toJSON(rightOperand, "", "").c_str());
#endif

            switch (operation) {
                case OP_BIGGER:
                    return leftOperand->isBiggerThan(rightOperand);
                case OP_LESSER:
                    return leftOperand->isLessThan(rightOperand);
                case OP_EQUALS:
                    return leftOperand->equals(rightOperand);
                case OP_EQUALS_STRICT:
                    return leftOperand->equalsStrict(rightOperand);
                case OP_BIGGER_EQ:
                    return leftOperand->isBiggerEqThan(rightOperand);
                case OP_LESSER_EQ:
                    return leftOperand->isLessEqThan(rightOperand);
                case OP_DIFFERS:
                    return leftOperand->differs(rightOperand);
                case OP_DIFFERS_STRICT:
                    return leftOperand->differsStrict(rightOperand);
                case OP_OR:
                    return leftOperand->orBase(rightOperand);
                case OP_OR_STRICT:
                    return leftOperand->orStrict(rightOperand);
                case OP_AND:
                    return leftOperand->andBase(rightOperand);
                case OP_AND_STRICT:
                    return leftOperand->andStrict(rightOperand);
                case OP_SHIFT_RIGHT:
                    return leftOperand->shiftRight(rightOperand);
                case OP_SHIFT_LEFT:
                    return leftOperand->shiftLeft(rightOperand);
            }
            return Value::npos;
        }

        Value * operateShifters(std::vector<ScriptValueNode> items, Function *& context) {
            Value * result = items.at(0).value;

            for (unsigned int i = 1; i < items.size(); i++) {
                result = result->modify(items.at(i).value, items.at(i - 1).operation);
            }

            return result;
        }

        Value * executeNodes(std::vector<SScriptData *> &list, int index,
                std::vector<SScriptData *> &_operatorsBuffer,
                std::vector<ScriptValueNode> &_shiftersBuffer, int &hasShifters,
                Function * &context, int errFlags) {


            SScriptData * current;
            int currentOperation = OP_UNSET;
            //int lastOperationScanned = OP_UNSET;
            string keyName;
            int _lastOperatorScanned = OP_UNSET;
            int _nextOperation = OP_UNSET;

            if (index > 0) {
                _lastOperatorScanned = list.at(index - 1)->getOperation();
            }

            if (index + 1 < (int) list.size()) {
                _nextOperation = list.at(index + 1)->getOperation();
            }

            Value * leftOperand;

            if (index < (int) list.size()) {
                current = list.at(index);
                currentOperation = current->getOperation();
                keyName = current->getBuffer();


                if (currentOperation == OP_VAR_IN) {
                    cout << " OP_VAR_IN " << keyName << endl;

                    if (index + 1 < (int) list.size()) {
                        leftOperand = executeNodes(list, index + 1, _operatorsBuffer, _shiftersBuffer, hasShifters, context);
                        //getMember(keyName, context); //
                        cout << lang::toJSON(leftOperand) << endl;
                        return leftOperand->contains(getMember(keyName, context));
                    }
                    return Value::npos;
                }

#ifdef ASTX_DBG_ECMA
                cout << __LINE__ << DBG_FN(__FILENAME__);
                cout << " executeNodes operation = " << operatorToString(currentOperation) << endl;
#endif


                if (currentOperation == OP_PLUSPLUS) {
                    leftOperand = getMember(keyName, context)->plusPlus();
#ifdef ASTX_DBG_ECMA
                    cout << __LINE__ << DBG_FN(__FILENAME__);
                    cout << " " << lang::toJSON(leftOperand) << operatorToString(currentOperation) << endl;
#endif
                    return leftOperand;
                }

                if (currentOperation == OP_MINMIN) {
                    leftOperand = getMember(keyName, context)->minMin();
#ifdef ASTX_DBG_ECMA
                    cout << __LINE__ << DBG_FN(__FILENAME__);
                    cout << " " << lang::toJSON(leftOperand) << operatorToString(currentOperation) << endl;
#endif
                    return leftOperand;
                }

                if (currentOperation == OP_ASSIGN) {
                    _lastOperatorScanned = currentOperation;
                    Value * set = executeNodes(list, index + 1, _operatorsBuffer, _shiftersBuffer, hasShifters, context);

                    if (set->getType() < ASTOX_VALUE_OBJECT) {
                        set = Value::duplicate(set);
                    }
                    //			cout << "set "<< index << " -- |" << keyName << "|" << lang::toJSON(set, "", "") << endl;
                    //			return context->setMember(keyName, set);
                    return setMember(keyName, set, context);
                }



                if (isOperator(currentOperation)) {
                    _operatorsBuffer.push_back(list.at(index));
                }
                else if (isShiftOperator(currentOperation) && !isOperator(_lastOperatorScanned)) {
                    hasShifters++;
                    ScriptValueNode n;
                    n.operation = currentOperation;
                    n.value = getMember(keyName, context);
                    _shiftersBuffer.push_back(n);
                    return executeNodes(list, index + 1, _operatorsBuffer, _shiftersBuffer, hasShifters);
                }
                else {
                    if (isOperator(_lastOperatorScanned) && currentOperation != OP_ASSIGN) {
                        _operatorsBuffer.push_back(list.at(index));
                        //				for(unsigned int i = 0; i < _operatorsBuffer.size(); i++) {
                        //					_operatorsBuffer.at(i)->debug();
                        //				}
                        leftOperand = operate(_operatorsBuffer, 0, context);
#ifdef ASTX_DBG_ECMA
                        cout << "(hasShifters=" << hasShifters << ") FINAL DE OPERATIE< REZULTAT = " << lang::toJSON(leftOperand) << endl;
#endif

                        if (currentOperation == OP_INSTRUCTION_END) {
                            if (hasShifters > 0) {
                                ScriptValueNode n;
                                n.operation = currentOperation;
                                n.value = leftOperand;
                                _shiftersBuffer.push_back(n);
                                return operateShifters(_shiftersBuffer, context);
                            }
                            return leftOperand;
                        }
                        else if (isComparisonTerm(currentOperation)) {
                            return compare(leftOperand, executeNodes(list, index + 1, _operatorsBuffer, _shiftersBuffer, hasShifters, context), currentOperation);
                        }

                        if (isShiftOperator(currentOperation)) {
                            hasShifters++;
                            ScriptValueNode n;
                            n.operation = currentOperation;
                            n.value = leftOperand;
                            _shiftersBuffer.push_back(n);
                            return executeNodes(list, index + 1, _operatorsBuffer, _shiftersBuffer, hasShifters);
                        }
                    }
                    else if (isComparisonTerm(currentOperation)) {

                        if (hasShifters > 0) {
                            ScriptValueNode n;
                            n.operation = currentOperation;
                            n.value = getMember(keyName, context);
                            _shiftersBuffer.push_back(n);
                            leftOperand = operateShifters(_shiftersBuffer, context);
                            _shiftersBuffer.clear();
                            hasShifters = 0;
                            return compare(leftOperand, executeNodes(list, index + 1, _operatorsBuffer, _shiftersBuffer, hasShifters, context), currentOperation);
                        }
                        //
                        leftOperand = getMember(keyName, context);
                        int nextIndex = 1;
                        if (isComparisonTerm(_nextOperation)) {
                            //cout << "SUNT AICI, get "<< keyName << operatorToString(_nextOperation) << endl;
                            Value * rightOperand = getMember(list.at(index + 1)->getBuffer(), context);
                            //cout << lang::toJSON(leftOperand) << "=left si right=" << lang::toJSON(rightOperand) << endl;
                            leftOperand = compare(leftOperand, rightOperand, currentOperation);
                            nextIndex = 2;
                            //cout << "leftOperand =  "<< lang::toJSON(leftOperand) << endl;
                            return compare(leftOperand, executeNodes(list, index + 2, _operatorsBuffer, _shiftersBuffer, hasShifters, context, errFlags), _nextOperation);
                        }

                        return compare(leftOperand, executeNodes(list, index + 1, _operatorsBuffer, _shiftersBuffer, hasShifters, context, errFlags), currentOperation);
                    }


                    else if (currentOperation == OP_INSTRUCTION_END) {
                        if (hasShifters > 0) {
                            ScriptValueNode n;
                            n.operation = currentOperation;
                            n.value = getMember(keyName, context);
                            _shiftersBuffer.push_back(n);
                            return operateShifters(_shiftersBuffer, context);
                        }
                        //				cout << " FAC GET " << keyName << " flag:" << errFlags<< endl;
                        return getMember(keyName, context, errFlags);
                    }
                }

                //_lastOperatorScanned = currentOperation;
                return executeNodes(list, index + 1, _operatorsBuffer, _shiftersBuffer, hasShifters, context, errFlags);
            }


            return Value::npos;
        }

        Value * runCommand(std::string input, Function *& context, int errFlags) {
            using namespace std;
            //	cout << "runCommand: " << input << endl;
            std::vector<std::string> commands = splitByComma(input);

            if (commands.size() == 1) {
                std::vector<SScriptData*> list;
                splitCommandContent(input, list);
                std::vector<SScriptData *> _operatorsBuffer;
                std::vector<ScriptValueNode> _shiftersBuffer;
                int _hasShifters = 0;
                return executeNodes(list, 0, _operatorsBuffer, _shiftersBuffer, _hasShifters, context, errFlags);
            }

            Array * respList = new Array();

            for (unsigned int i = 0; i < commands.size(); i++) {
                //		cout << "commands.at("<< i << ") = " << commands.at(i) << endl;
                std::vector<SScriptData*> list;
                splitCommandContent(commands.at(i), list);
                std::vector<SScriptData *> _operatorsBuffer;
                std::vector<ScriptValueNode> _shiftersBuffer;
                int _hasShifters = 0;
                Value * cval = executeNodes(list, 0, _operatorsBuffer, _shiftersBuffer, _hasShifters, context, errFlags);
                respList->push(cval);
            }


            return respList;
        }

        std::string getExpressionUntil(std::string input, std::string keyword, int &found) {
            std::string buffer;
            std::string keyBuffer;
            int nd = 0, ns = 0;
            if (keyword.size() == 1) {
                char f = keyword.at(0);
                for (unsigned int i = 0; i < input.size(); i++) {
                    char c = input.at(i);
                    char p = (i > 0) ? input.at(i - 1) : '\0';
                    char n = (i < input.size() - 1) ? input.at(i + 1) : input.at(input.size() - 1);

                    if (f != ';' && !ecma::charIsInString(c, p, nd, ns) && c == f && c != n && c != p) {
                        return buffer;
                    } else if (f == ';' && !ecma::charIsInString(c, p, nd, ns)) {
                        return buffer;
                    } else {
                        buffer += c;
                    }
                }
                return buffer;
            }

            for (unsigned int i = 0; i < input.size(); i++) {
                char c = input.at(i);
                char p = (i > 0) ? input.at(i - 1) : '\0';
                if (!ecma::charIsInString(c, p, nd, ns)) {
                    keyBuffer += c;
                    cout << keyBuffer << endl;
                    if (keyBuffer == keyword) {
                        found = i;
                        return buffer;
                    }


                    if (keyBuffer.size() >= keyword.size()) {
                        buffer += keyBuffer.at(0);
                        keyBuffer = keyBuffer.substr(1, keyBuffer.size());
                    }
                } else {
                    buffer += c;
                }
            }
            return buffer;
        }

        bool expectKeyword(std::string &input, const char * keyword, std::string &beforeKeyword, int &found_pos, std::string allowedChars) {
            beforeKeyword.clear();

            size_t poz = input.find(keyword);
            found_pos = poz;
            if (poz == std::string::npos) {
                return false;
            }


            if (poz == 0) {
                if (!allowedChars.empty()) {
                    char after = input.at(strlen(keyword));
                    cout << after << endl;
                    for (unsigned int i = 0; i < allowedChars.size(); i++) {
                        if (after != allowedChars.at(i)) {
                            return false;
                        }
                    }
                }
                beforeKeyword = input.substr(0, poz);
                //		cout << "beforeKeyword = " << beforeKeyword << endl;
                return true;
            }


            return false;
        }

        std::string stripComments(std::string input) {
            string buff;
            bool commentBlock = false;
            bool commentLine = false;
            int nd = 0, ns = 0;
            for (unsigned int i = 0; i < input.size(); i++) {
                char c = input.at(i);
                char p = (i > 0) ? input.at(i - 1) : '\0';
                char n = (i < input.size() - 1) ? input.at(i + 1) : input.at(input.size() - 1);
                if (!charIsInString(c, p, nd, ns)) {
                    if (c == '/' && n == '*' && !commentBlock && !commentLine) {
                        commentBlock = true;
                    }
                    else if (c == '/' && p == '*' && commentBlock) {
                        commentBlock = false;
                        buff += ' ';
                    }
                    else if (c == '/' && n == '/' && !commentLine && !commentBlock) {
                        commentLine = true;
                    }
                    else if (c == '\n' && commentLine) {
                        commentLine = false;
                        buff += ' ';
                    }
                    else if (!commentBlock && !commentLine) {
                        buff += c;
                    }

                } else if (!commentBlock && !commentLine) {
                    buff += c;
                }
            }

            return buff;
        }

        bool isAllowedNameChar(char c) {
            return ( c != '{' && c != '}' && c != '(' && c != ')' && c != '"');
        }

        ComposedString splitIntoComposition(std::string &input, int substrPos) {
            ComposedString s;
            std::string functionName, arguments, body;
            int stat = 0;
            unsigned int i;
            char c;

            for (i = 0; i < input.size(); i++) {
                c = input.at(i);
                if (c == '(') {
                    break;
                }
                if (isAllowedNameChar(c)) {
                    s.name += c;
                } else {
                    string emessage = input;
                    emessage.append("| disallowed char in member name: (");
                    emessage += c;
                    emessage += ')';
                    Error n(Error::PARSING_ERROR, emessage, __LINE__, __FILENAME__);
                }
            }

            s.arguments = getExpressionBetween(input, '(', ')', stat);
            input = input.substr(stat, input.size());
            s.body = getExpressionBetween(input, '{', '}', stat);
            s.lastScannedPoz = stat;
            return s;
        }

        void detectNodes(std::string &input, std::vector<ecma::ComposedString> &data) {
            if (input.at(0) != ' ') {
                input.insert(0, " ");
            }

            string firstBefore;
            int foundLeft = 0, foundRight = 0, detected_type = ASTXKT_NONE;
            detectKeyword(input, firstBefore, detected_type, foundLeft, foundRight);
#ifdef ASTX_DBG_ECMA_DETECT_NODES
            astox_debug(__LINE__, __FILENAME__, "ecma::detectNodes TYPE (%i) LEFT (%i) RIGHT(%i) BEFORE(%s)", detected_type, foundLeft, foundRight, firstBefore.c_str());
#endif

            int substrPoz = 0, foundPos = 0;

            ComposedString cit;
            cit.type = detected_type;

            switch (detected_type) {
                case ASTXKT_BSLINE:
#ifdef ASTX_DBG_ECMA_DETECT_NODES
                    astox_debug(__LINE__, __FILENAME__, "ecma::detectNodes ASTXKT_BSLINE (%s)", input.c_str());
#endif
                    firstBefore = stringAlg::trim(firstBefore);
                    input = input.substr(foundRight, input.size());
                    cit.body = firstBefore;
                    break;
                case ASTXKT_VAR:
#ifdef ASTX_DBG_ECMA_DETECT_NODES
                    astox_debug(__LINE__, __FILENAME__, "ecma::detectNodes ASTXKT_VAR (%s)", input.c_str());
#endif
                    firstBefore = ecma::getAllUntilChar(input, foundPos, ';');
                    substrPoz = firstBefore.size() + 1;
                    firstBefore = firstBefore.substr(foundRight, firstBefore.size());
                    cit.body = firstBefore;

                    if (substrPoz <= (int) input.size()) {
                        input = input.substr(substrPoz, input.size());
                    }
                    else {
                        Error n(Error::PARSING_ERROR, "missing ; separator", __LINE__, __FILENAME__);
#ifndef ASTX_DISABLE_EXCEPTIONS
                        throw n;
#endif
                        return;
                    }
                    break;

                case ASTXKT_ELSE:
                    input = input.substr(foundRight, input.size());
                    cit.body = getExpressionBetween(input, '{', '}', foundRight);
                    input = input.substr(foundRight, input.size());
#ifdef ASTX_DBG_ECMA_DETECT_NODES
                    astox_debug(__LINE__, __FILENAME__, "ecma::detectNodes ASTXKT_ELSE (%s)", input.c_str());
#endif
                    break;

                case ASTXKT_IF:
                case ASTXKT_ELIF:
                case ASTXKT_FOR:
                case ASTXKT_WHILE:
                case ASTXKT_SWITCH:
                    cit = splitIntoComposition(input, foundRight - 1);
                    cit.type = detected_type;
                    input = input.substr(cit.lastScannedPoz, input.size());
#ifdef ASTX_DBG_ECMA_DETECT_NODES
                    astox_debug(__LINE__, __FILENAME__, "ecma::detectNodes ASTXKT_IF|ELIF|FOR|WHILE|SWITCH (%s)", input.c_str());
#endif
                    break;

                case ASTXKT_FUNCTION:
                    input = input.substr(foundRight, input.size());
                    cit = splitIntoComposition(input, foundRight - 1);
#ifdef ASTX_DBG_ECMA
                    astox_debug(__LINE__, __FILENAME__, "ecma::detectNodes ASTXKT_FUNCTION name:(%s) body(%s) args(%s)", cit.name.c_str(), cit.body.c_str(), cit.arguments.c_str());
#endif
                    cit.type = detected_type;
                    input = input.substr(cit.lastScannedPoz, input.size());
                    break;

                case ASTXKT_BREAK:
                    cit.type = detected_type;
                    input = input.substr(foundRight, input.size());
#ifdef ASTX_DBG_ECMA_DETECT_NODES
                    astox_debug(__LINE__, __FILENAME__, "ecma::detectNodes ASTXKT_BREAK (%s)", input.c_str());
#endif
                    break;

                case ASTXKT_RETURN:
                    input = input.substr(foundRight, input.size());
                    firstBefore = ecma::getAllUntilChar(input, foundPos, ';');
                    cit.type = detected_type;
                    cit.body = firstBefore;
                    input = input.substr(foundPos + 1, input.size());
                    input = stringAlg::trim(input);
#ifdef ASTX_DBG_ECMA_DETECT_NODES
                    astox_debug(__LINE__, __FILENAME__, "ecma::detectNodes ASTXKT_RETURN (%s)", input.c_str());
#endif
                    break;

                case ASTXKT_TRY:
                    input = input.substr(4, input.size());
                    cit.type = detected_type;
                    cit.body = ecma::getExpressionBetween(input, '{', '}', foundPos);
                    input = input.substr(foundPos, input.size());
                    detectKeyword(input, firstBefore, detected_type, foundLeft, foundRight);

#ifdef ASTX_DBG_ECMA_DETECT_NODES
                    astox_debug(__LINE__, __FILENAME__, "ecma::detectNodes ASTXKT_TRY (%s) (%s)", cit.body.c_str(), input.c_str());
#endif

                    if (detected_type == ASTXKT_CATCH) {
                        cit.name = ecma::getExpressionBetween(input, '(', ')', foundPos);
                        input = input.substr(foundPos, input.size());
                        cit.arguments = ecma::getExpressionBetween(input, '{', '}', foundPos);
#ifdef ASTX_DBG_ECMA_DETECT_NODES
                        astox_debug(__LINE__, __FILENAME__, "ecma::detectNodes catch detected (%s) with args (%s)", cit.name.c_str(), cit.arguments.c_str());
#endif

                        if (foundPos <= (int) input.size()) {
                            input = input.substr(foundPos, input.size());
#ifdef ASTX_DBG_ECMA_DETECT_NODES
                            astox_debug(__LINE__, __FILENAME__, "ecma::detectNodes catch rest detected (%s)", input.c_str());
#endif
                        }

                        if (!input.empty()) {
                            detectKeyword(input, firstBefore, detected_type, foundLeft, foundRight);

                            if (detected_type == ASTXKT_FINALLY) {
                                input = input.substr(foundLeft, input.size());
                                cit.fourth = ecma::getExpressionBetween(input, '{', '}', foundPos);
#ifdef ASTX_DBG_ECMA_DETECT_NODES
                                astox_debug(__LINE__, __FILENAME__, "ecma::detectNodes finally detected (%s)", cit.fourth.c_str());
#endif
                                if (foundPos <= (int) input.size()) {
#ifdef ASTX_DBG_ECMA_DETECT_NODES
                                    astox_debug(__LINE__, __FILENAME__, "ecma::detectNodes finally rest detected (%s)", input.c_str());
#endif
                                    input = input.substr(foundPos, input.size());
                                }
                            }
                        }

                    }

                    else {
                        String dbg(detected_type);
                        string dbg2 = "malformed try catch syntax, detected type = ";
                        dbg2.append(dbg.toString());

                        Error n(Error::PARSING_ERROR, dbg2, __LINE__, __FILENAME__);
#ifndef ASTX_DISABLE_EXCEPTIONS
                        throw n;
#endif
                        return;
                    }
                    break;

                default:
                    printf("    DEFAULT %i NOT ALLOWED IN THIS CONTEXT\n", detected_type);
                    Error n(Error::PARSING_ERROR, " instruction out of context ", __LINE__, __FILENAME__);
#ifndef ASTX_DISABLE_EXCEPTIONS
                    throw n;
#endif
                    //			astox_system_pause();
                    std::exit(0);
                    break;
            }


            data.push_back(cit);


            if (!input.empty()) {
                //cout << "CONTINUE WITH:" << input << endl;
                detectNodes(input, data);
            }

        }

        int execIfContext(std::vector<ConditionStruct> &args, Function *& context) {
#ifdef	ASTX_DBG_ECMA_CONTEXTS
            astox_debug(__LINE__, __FILENAME__, "ecma::execIfContext (%s)", args.at(0).condition ? "true" : "false");
#endif
            int response = 0;
            for (unsigned int i = 0; i < args.size(); i++) {
                if (args.at(i).condition) {
                    response = execScript(args.at(i).body, context);
                    break;
                }
            }
            args.clear();
            return response;
        }

        int execForInContext(std::string &valueName, std::string &body, std::vector<std::string> &keyNames, int index, Function *& context) {
            setMember(valueName, new String(keyNames.at(index)), context);
            string copyBody = body;
            int nrr = execScript(copyBody, context);
            if (nrr == 0 && index + 1 < (int) keyNames.size()) {
                return execForInContext(valueName, body, keyNames, index + 1, context);
            }
            return nrr;
        }

        int prepareForContext(std::string &args, std::string &body, Function *& context) {

#ifdef ASTX_DBG_ECMA_CONTEXTS
            astox_debug(__LINE__, __FILENAME__, "ecma::prepareForContext (%s) (%s)", body.c_str(), args.c_str());
#endif

            vector<string> datas = splitByComma(args, ';');

            if (!datas.empty()) {

                if (datas.size() > 1) {
                    if (datas.at(0).find("var ") == 0) {
                        datas.at(0) = datas.at(0).substr(4, datas.at(0).size());
                    }
                    //			cout << datas.at(0) << endl;
                    ecma::runCommand(datas.at(0), context);
                    datas.erase(datas.begin());
                    if (!datas.empty() && datas.size() == 2) {
                        return execForContext(datas, context, body);
                    } else {
                        Error n(Error::PARSING_ERROR, "malformed for statement: for (initialCode; iteratingCondition; repeatingExpression) { repeatingCode }", __LINE__, __FILENAME__);
                    }

                }
                else {
                    //			cout << "for args: " << datas.at(0) << endl;
                    if (datas.at(0).find("var ") == 0) {
                        datas.at(0) = datas.at(0).substr(4, datas.at(0).size());
                        //				cout << "var assign: " << datas.at(0) << endl;
                        std::vector<SScriptData*> list;
                        splitCommandContent(datas.at(0), list);
                        if (list.size() == 2 && list.at(0)->getOperation() == OP_VAR_IN) {
                            vector<string> keynames = getMember(list.at(1)->getBuffer(), context)->getKeyNameList();
                            string valueName = list.at(0)->getBuffer();
                            int response = execForInContext(valueName, body, keynames, 0, context);
                            list.clear();
                            keynames.clear();
                            return response;
                        } else {
                            Error n(Error::PARSING_ERROR, "malformed for statement", __LINE__, __FILENAME__);
                        }
                    }
                }
            }

#ifdef ASTX_DBG_ECMA_CONTEXTS
            astox_debug(__LINE__, __FILENAME__, "ecma::prepareForContext:");
            context->debug();
#endif
            return 0;
        }

        int execForContext(vector<string> &argsToExec, Function *& localContext, std::string &body) {
            int status = 0;

            bool response = ecma::runCommand(argsToExec.at(0), localContext)->toBoolean();
            if (!response) {
                body.clear();
                argsToExec.clear();
                return status;
            }

            std::string copyBody = body;
            status = execScript(copyBody, localContext);

#ifdef ASTX_DBG_ECMA_CONTEXTS
            astox_debug(__LINE__, __FILENAME__, "ecma::execForContext with status (%i)", status);
#endif
            if (status == 0) {
                ecma::runCommand(argsToExec.at(1), localContext);
                return execForContext(argsToExec, localContext, body);
            }

            return status;
        }

        std::vector<int> findPosOfKeyword(std::string &input, std::string keyword) {
            std::vector<int> result;
            int e = 0, f = 0, g = 0, h = 0, j = 0;
            unsigned int keySize = keyword.size();
            string tmpBuff;
            for (unsigned int i = 0; i < input.size(); i++) {
                char c = input.at(i);
                char p = (i > 0) ? input.at(i - 1) : '\0';
                if (!charIsInString(c, p, e, f) &&
                        !charIsEnclosedBy(c, g, '(', ')') &&
                        !charIsEnclosedBy(c, h, '{', '}') &&
                        !charIsEnclosedBy(c, j, '[', ']')) {
                    tmpBuff += c;

                    if (tmpBuff == keyword) {
                        result.push_back(i + 1);
                        tmpBuff.clear();
                    }
                    else if (tmpBuff.size() >= keySize) {
                        tmpBuff.erase(tmpBuff.begin());
                    }
                }
            }


            tmpBuff.clear();
            return result;
        }

        std::vector<std::string> splitByKeyword(std::string &input, std::string keyword) {
            vector<string> response;
            int keySize = keyword.size();
            std::vector<int> positions = findPosOfKeyword(input, keyword);
            positions.insert(positions.begin(), 0);
            for (unsigned int i = 0; i < positions.size(); i++) {
                int current = positions.at(i);
                int nextPoz = i < positions.size() - 1 ? positions.at(i + 1) - current - keySize : input.size();
                if (nextPoz > 0) {
                    //				cout << " from " << positions.at(i) << " to "<< nextPoz << " in " << input << endl;
                    response.push_back(input.substr(current, nextPoz));
                }

            }
            positions.clear();
            return response;
        }

        int prepareSwitchContext(std::string &args, std::string &body, Function *& context) {
            body = stringAlg::trim(body);
#ifdef ASTX_DBG_ECMA_CONTEXTS
            astox_debug(__LINE__, __FILENAME__, "ecma::prepareSwitchContext with args (%s) body (%s)", args.c_str(), body.c_str());
#endif

            unsigned int i;
            std::vector<std::string> cases = splitByKeyword(body, "case ");

            if (!cases.empty()) {
                std::vector<ComposedString> condcases;


                for (i = 0; i < cases.size(); i++) {
                    int lcp = 0;
                    string cstr = cases.at(i);
                    ComposedString ccomp;
                    ccomp.arguments = ecma::getAllUntilChar(cstr, lcp, ':');
                    ;
                    ccomp.body = cstr.substr(lcp + 1, cstr.size());
                    condcases.push_back(ccomp);
                }

                std::vector<std::string> searchDefault = splitByKeyword(condcases.at(condcases.size() - 1).body, "default:");

                if (searchDefault.size() == 2) {
                    condcases.at(condcases.size() - 1).body = searchDefault.at(0);
                    ComposedString ccomp;
                    ccomp.arguments = args;
                    ccomp.body = searchDefault.at(1);
                    condcases.push_back(ccomp);
                }

#ifdef ASTX_DBG_ECMA_CONTEXTS
                astox_debug(__LINE__, __FILENAME__, "ecma::prepareSwitchContext with body (%s)", body.c_str());
                for (unsigned int j = 0; j < condcases.size(); j++) {
                    astox_debug(__LINE__, __FILENAME__, "ecma::prepareSwitchContext if (%s)==(%s) then (%s)", args.c_str(), condcases.at(j).arguments.c_str(), condcases.at(j).body.c_str());
                }
#endif
                return execSwitchContext(args, condcases, 0, context);

            }
            else {
                Error n(Error::PARSING_ERROR, "no case in switch statement", __LINE__, __FILENAME__);
            }

            return ASTXKT_SWITCH;
        }

        int execSwitchContext(std::string &args, std::vector<ComposedString> &condcases, unsigned int index, Function *& context) {
            int execStatus = 0;
            if (index < condcases.size()) {
                bool allow = ecma::runCommand(args, context)->equals(ecma::runCommand(condcases.at(index).arguments, context))->toBoolean();
#ifdef ASTX_DBG_ECMA_CONTEXTS
                astox_debug(__LINE__, __FILENAME__, "ecma::execSwitchContext for (%s)", condcases.at(index).arguments.c_str());
#endif


                if (allow) {
                    execStatus = ecma::execScript(condcases.at(index).body, context);
#ifdef ASTX_DBG_ECMA
                    astox_debug(__LINE__, __FILENAME__, "ecma::execSwitchContext run (%s) exit(%i)", condcases.at(index).body.c_str(), execStatus);
#endif
                }

                if (index + 1 < condcases.size() && execStatus == 0) {
                    return execSwitchContext(args, condcases, index + 1, context);
                }
            }
            return execStatus;
        }

        int execWhileContext(std::string &args, std::string &body, Function *& context) {
            int response = 0;
            string copyArgs = args;
            string copyBody = body;
            bool allow = ecma::runCommand(copyArgs, context)->toBoolean();
            if (allow) {
                response = ecma::execScript(copyBody, context);
                if (response == 0) {
                    return execWhileContext(args, body, context);
                }
            }

            return response;
        }

        int execTryCatchContext(ComposedString &compString, Function *&context) {
#ifdef ASTX_DBG_ECMA_CONTEXTS
            astox_debug(__LINE__, __FILENAME__, "ecma::execTryCatchContext try (%s) catch (%s) exec (%s) finally  (%s)",
                    compString.body.c_str(), compString.name.c_str(), compString.arguments.c_str(), compString.fourth.c_str());
#endif
            int response;
#ifndef ASTX_DISABLE_EXCEPTIONS
            try {
                response = ecma::execScript(compString.body, context);
            } catch (Error &ex) {
    #ifdef ASTX_DBG_ECMA_CONTEXTS
                astox_debug(__LINE__, __FILENAME__, "ecma::execTryCatchContext caugth (%s)", ex.what());
    #endif
                Function * localContext = new Function(context);
                localContext->addKey(compString.name, &ex);
                response = ecma::execScript(compString.arguments, localContext);
                localContext->eraseKey(compString.name);
                context->import(localContext);
                if (response == ASTXKT_RETURN) {
                    context->updatePrototype(localContext->protoype);
                }
                delete localContext;
            }
#else
            ecma::execScript(compString.body, context);
#endif

            if (!compString.fourth.empty() && response != ASTXKT_RETURN) {
                response = ecma::execScript(compString.fourth, context);
            }
            return response;
        }

        int execScript(std::string &input, Function *& context, int errFlags) {

            std::vector<ecma::ComposedString> data;
            ecma::detectNodes(input, data);
            std::vector<ConditionStruct> ifelseArgs;
            Value * r;
            Function * temp = NULL;
            int response = 0;

            for (unsigned int i = 0; i < data.size(); i++) {
                ConditionStruct n;
                switch (data.at(i).type) {
                    case ecma::ASTXKT_BSLINE:
                        r = ecma::runCommand(data.at(i).body, context, GETR_STRICT);
                        break;
                    case ecma::ASTXKT_VAR:
                        r = ecma::runCommand(data.at(i).body, context, GETR_SAFE_NPOS);
                        break;
                    case ecma::ASTXKT_FUNCTION:
                        temp = new Function(data.at(i).body, data.at(i).arguments, context);
                        context->addKey(data.at(i).name, temp);
                        temp->setParent(context);
                        #ifdef ASTX_DBG_ECMA_EXEC_SCRIPT
                            astox_debug(__LINE__, __FILENAME__, "ecma::execScript ASTXKT_FUNCTION add func (%s)", data.at(i).name.c_str());
                        #endif
                        break;

                    case ecma::ASTXKT_IF:
                        n.condition = ecma::runCommand(data.at(i).arguments, context)->toBoolean();
                        n.body = data.at(i).body;
                        #ifdef ASTX_DBG_ECMA_EXEC_SCRIPT
                            astox_debug(__LINE__, __FILENAME__, "ecma::execScript ASTXKT_IF (%i)", n.condition);
                        #endif
                        ifelseArgs.push_back(n);
                        break;

                    case ecma::ASTXKT_ELIF:
                        if (ifelseArgs.size() > 0) {
                            n.condition = ecma::runCommand(data.at(i).arguments, context)->toBoolean();
                            n.body = data.at(i).body;
                            ifelseArgs.push_back(n);
                        } else {
                            Error n(Error::PARSING_ERROR, "syntax error, else if without if", __LINE__, __FILENAME__);
                        }
                        break;

                    case ecma::ASTXKT_ELSE:
                        if (ifelseArgs.size() > 0) {
                            n.condition = true;
                            n.body = data.at(i).body;
                            ifelseArgs.push_back(n);

                            if (!ifelseArgs.empty()) {
                                response = execIfContext(ifelseArgs, context);
                                ifelseArgs.clear();
                                if (response == ASTXKT_RETURN) {
                                    goto exitFunc;
                                }
                            }
                        } else {
                            Error n(Error::PARSING_ERROR, "syntax error, else without if", __LINE__, __FILENAME__);
                        }

                        break;

                    case ecma::ASTXKT_RETURN:
                        if (!ifelseArgs.empty()) {
                            response = execIfContext(ifelseArgs, context);
                            ifelseArgs.clear();
                            if (response == ASTXKT_RETURN) {
                                goto exitFunc;
                            }
                        }
                        #ifdef ASTX_DBG_ECMA_EXEC_SCRIPT
                            astox_debug(__LINE__, __FILENAME__, "ecma::execScript ASTXKT_RETURN (%s) exit(%i)", data.at(i).body.c_str(), ASTXKT_RETURN);
                        #endif
                        if (!data.at(i).body.empty()) {
                            context->updatePrototype(ecma::runCommand(data.at(i).body, context));
                        }
                        response = ASTXKT_RETURN;
                        goto exitFunc;
                        break;

                    case ASTXKT_WHILE:
                        cout << "ASTXKT_WHILE : (" << data.at(i).arguments << ") {{ " << data.at(i).body << endl;
#ifdef ASTX_DBG_ECMA_EXEC_SCRIPT
                        astox_debug(__LINE__, __FILENAME__, "ecma::execScript ASTXKT_WHILE (%s) do (%s)", data.at(i).arguments.c_str(), data.at(i).body.c_str());
#endif
                        response = execWhileContext(data.at(i).arguments, data.at(i).body, context);
                        if (response == ASTXKT_RETURN) {
                            goto exitFunc;
                        }
                        break;

                    case ecma::ASTXKT_BREAK:
                        return ASTXKT_BREAK;

                    case ASTXKT_SWITCH:
#ifdef ASTX_DBG_ECMA_EXEC_SCRIPT
                        astox_debug(__LINE__, __FILENAME__, "ecma::execScript ASTXKT_SWITCH (%s)", data.at(i).body.c_str());
#endif
                        response = prepareSwitchContext(data.at(i).arguments, data.at(i).body, context);
                        if (response == ASTXKT_RETURN) {
                            goto exitFunc;
                        }
                        break;

                    case ASTXKT_FOR:
                        response = prepareForContext(data.at(i).arguments, data.at(i).body, context);
                        if (response == ASTXKT_RETURN) {
                            goto exitFunc;
                        }
                        break;

                    case ASTXKT_TRY:
                        response = execTryCatchContext(data.at(i), context);
                        cout << " ASTXKT_TRY executed " << response << endl;
#ifdef ASTX_DBG_ECMA_EXEC_SCRIPT
                        astox_debug(__LINE__, __FILENAME__, "ecma::execScript ASTXKT_TRY DONE");
#endif
                        if (response == ASTXKT_RETURN) {
#ifdef ASTX_DBG_ECMA_EXEC_SCRIPT
                            astox_debug(__LINE__, __FILENAME__, "ecma::execScript ASTXKT_TRY EXIT(%s)", lang::toJSON(context->protoype, "", "").c_str());
#endif
                            goto exitFunc;
                        }
                        break;

                    default:
                        if (!ifelseArgs.empty()) {
                            response = execIfContext(ifelseArgs, context);
                            ifelseArgs.clear();
                            if (response == ASTXKT_RETURN) {
                                goto exitFunc;
                            }
                        }
#ifdef ASTX_DBG_ECMA_EXEC_SCRIPT
                        else {
                            astox_debug(__LINE__, __FILENAME__, "ecma::execScript default statement (%i)\n\ttype(%i)\n\tname(%s)\n\targs(%s) body(%s)",
                                    i, data.at(i).type, data.at(i).name.c_str(), data.at(i).arguments.c_str(), data.at(i).body.c_str());
                        }
#endif

                        break;
                }

            }
exitFunc:
            if (!ifelseArgs.empty() && response == 0) {
                response = execIfContext(ifelseArgs, context);
                ifelseArgs.clear();
            }

            return response;
        }

        void splitCommandContent(std::string input, std::vector<SScriptData *> &list) {
            using namespace std;

            int lastPoz = 0;
            std::string buffer;


            input = astox::stringAlg::trim(input);

            if (input.empty()) {
                return;
            }

            std::string rest;
            int numpar = 0;
            int numbrac = 0;
            int numpdr = 0;



            if (input.size() == 1) {
                list.push_back(new SScriptData(input, OP_INSTRUCTION_END));
                return;
            }
            int nd = 0, ns = 0;
            for (unsigned int i = 0; i < input.size(); i++) {
                char c = input.at(i);
                char p = (i > 0) ? input.at(i - 1) : '\0';
                char n = (i < input.size() - 1) ? input.at(i + 1) : '\0';
                char t = (i < input.size() - 2) ? input.at(i + 2) : '\0';

                //		cout << p << c << n << t << endl;
                if (!ecma::charIsInString(c, p, nd, ns)) {
                    switch (c) {
                        case '(':
                            numpar++;
                            break;
                        case ')':
                            numpar--;
                            break;
                        case '{':
                            numbrac++;
                            break;
                        case '}':
                            numbrac--;
                            break;
                        case '[':
                            numpdr++;
                            break;
                        case ']':
                            numpdr--;
                            break;

                    }

                    if (numbrac == 0 && numpar == 0 && numpdr == 0) {

                        //=
                        if (p != '=' && p != '!' && c == '=' && n != '=') {
                            list.push_back(new SScriptData(buffer, OP_ASSIGN));
                            lastPoz = i + 1;
                            goto checkRest;
                            break;
                        }                            //!=
                        else if (c == '!' && n == '=' && t != '=') {
                            list.push_back(new SScriptData(buffer, OP_DIFFERS));
                            lastPoz = i + 2;
                            goto checkRest;
                            break;
                        }                            //
                        else if (c == '!' && n == '=' && t == '=') {
                            list.push_back(new SScriptData(buffer, OP_DIFFERS_STRICT));
                            lastPoz = i + 3;
                            goto checkRest;
                            break;
                        }                            //==
                        else if (p != '=' && p != '!' && c == '=' && n == '=' && t != '=') {
                            list.push_back(new SScriptData(buffer, OP_EQUALS));
                            lastPoz = i + 2;
                            goto checkRest;
                        }                            //===
                        else if (c == '=' && n == '=' && t == '=') {
                            list.push_back(new SScriptData(buffer, OP_EQUALS_STRICT));
                            lastPoz = i + 3;
                            goto checkRest;
                        }                            //+
                        else if (c == '+' && n != '+') {
                            list.push_back(new SScriptData(buffer, OP_ADDITION));
                            lastPoz = i + 1;
                            goto checkRest;
                        }                            //++
                        else if (c == '+' && n == '+') {
                            list.push_back(new SScriptData(buffer, OP_PLUSPLUS));
                            lastPoz = i + 2;
                            goto checkRest;
                        }                            //-
                        else if (c == '-' && n != '-') {
                            list.push_back(new SScriptData(buffer, OP_SUBSTRACT));
                            lastPoz = i + 1;
                            goto checkRest;
                        }                            //--
                        else if (c == '-' && n == '-') {
                            list.push_back(new SScriptData(buffer, OP_MINMIN));
                            lastPoz = i + 2;
                            goto checkRest;
                        }                            //*
                        else if (c == '*') {
                            list.push_back(new SScriptData(buffer, OP_MULTIPLY));
                            lastPoz = i + 1;
                            goto checkRest;
                        }                            //\/
                        else if (c == '/') {
                            list.push_back(new SScriptData(buffer, OP_DIVIDE));
                            lastPoz = i + 1;
                            goto checkRest;
                        }                            //>
                        else if (c == '>' && n != '=' && n != '>') {
                            list.push_back(new SScriptData(buffer, OP_BIGGER));
                            lastPoz = i + 1;
                            goto checkRest;
                        }                            //>=
                        else if (c == '>' && n == '=' && n != '>') {
                            list.push_back(new SScriptData(buffer, OP_BIGGER_EQ));
                            lastPoz = i + 2;
                            goto checkRest;
                        }                            //%
                        else if (c == '%') {
                            list.push_back(new SScriptData(buffer, OP_MODULUS));
                            lastPoz = i + 1;
                            goto checkRest;
                        }                            //&
                        else if (c == '&' && n != '&') {
                            list.push_back(new SScriptData(buffer, OP_AND));
                            lastPoz = i + 1;
                            goto checkRest;
                        }                            //&&
                        else if (c == '&' && n == '&') {
                            list.push_back(new SScriptData(buffer, OP_AND_STRICT));
                            lastPoz = i + 2;
                            goto checkRest;
                        }                            //|
                        else if (c == '|' && n != '|') {
                            list.push_back(new SScriptData(buffer, OP_OR));
                            lastPoz = i + 1;
                            goto checkRest;
                        }                            //||
                        else if (c == '&' && n == '|') {
                            list.push_back(new SScriptData(buffer, OP_OR_STRICT));
                            lastPoz = i + 2;
                            goto checkRest;
                        }                            //<
                        else if (c == '<' && n != '=' && n != '<') {
                            list.push_back(new SScriptData(buffer, OP_LESSER));
                            lastPoz = i + 1;
                            goto checkRest;
                        }                            //<=
                        else if (c == '<' && n == '=' && n != '<') {
                            list.push_back(new SScriptData(buffer, OP_LESSER_EQ));
                            lastPoz = i + 2;
                            goto checkRest;

                        }                            //<<
                        else if (c == '<' && n == '<') {
                            list.push_back(new SScriptData(buffer, OP_SHIFT_LEFT));
                            lastPoz = i + 2;
                            goto checkRest;
                        }                            //>>
                        else if (c == '>' && n == '>') {
                            list.push_back(new SScriptData(buffer, OP_SHIFT_RIGHT));
                            lastPoz = i + 2;
                            goto checkRest;
                        }
                            //[ ]in[ ]
                        else if (p == ' ' && c == 'i' && n == 'n' && t == ' ') {
                            list.push_back(new SScriptData(buffer, OP_VAR_IN));
                            lastPoz = i + 3;
                            goto checkRest;
                        }
                        else {
                            buffer += c;
                        }
                    }
                    else {
                        buffer += c;
                    }
                } else {
                    buffer += c;
                }
            }

checkRest:
            if (lastPoz > 0) {
                if (lastPoz < (int) input.size()) {
                    rest = input.substr(lastPoz, input.size());
                    astox::stringAlg::trim(rest);
                    if (!rest.empty()) {
                        splitCommandContent(rest, list);
                    }
                }
#ifdef ASTX_DBG_ECMA_SPLIT_COMMAND
                else {
                    cout << __LINE__ << DBG_FN(__FILENAME__);
                    cout << "splitCommandContent index " << lastPoz << " incompatible " << endl;
                }
#endif
            } else {

                buffer = astox::stringAlg::trim(buffer);
                if (!buffer.empty()) {
                    list.push_back(new SScriptData(buffer, OP_INSTRUCTION_END));
                }
            }

        }

        std::string fixInput(std::string &inputStr) {
            inputStr = stripComments(inputStr);
            inputStr = astox::stringAlg::replaceWhiteSpace(inputStr);
            inputStr.insert(0, " ");
            return inputStr;
        }

        
        std::string readFile(std::string fileName) {
            std::ifstream o;
            std::string line;
            string _mainInput;
            o.open(fileName.c_str(), std::ios::in);
            if (o.is_open()) {
                while (getline(o, line)) {
                    if (ECMAScript::fixLineEnding(line)) {
                        _mainInput.append(line);
                    }
                }
            } else {
                astox_debug(__LINE__, __FILE__, "readFile %s NOT FOUND\n", fileName.c_str());
            }
            o.close();
            return _mainInput;
        }



    } /*end ecma namespace*/

    bool ECMAScript::fixLineEnding(string& in) {
        in = trim_left(in);
        if (!in.empty()) {
            if (in.size() > 0 && in.at(in.size() - 1) != '\n') {
                in += '\n';
            }
            return true;
        }

        return false;
    }

    ECMAScript::ECMAScript() {
        _ecmaKeys["var"] = 1;
        _emptyAllowedVarChars = " \n\t";
        cout << _emptyAllowedVarChars << endl;

    }

    ECMAScript::~ECMAScript() {
        // TODO Auto-generated destructor stub
    }

    bool ECMAScript::lineIsComment(string in) {
        if (!in.empty() && in.size() > 2) {
            return (in.at(0) == '/' && in.at(1) == '/');
        }
        return false;
    }

    bool ECMAScript::charIsEmpty(char c) {
        return (c == ' ' || c == '\t');
    }

    bool ECMAScript::checkSubstr(std::string needle, std::string input,
            size_t& position) {
        position = input.find(needle);
        if (position != string::npos) {
            return true;
        }
        return false;
    }

    bool ECMAScript::stringContext(char quote, string &input, size_t& position) {
        input = astox::stringAlg::trim_left(input);

        if (!input.empty() && input.at(0) == quote) {
            string content;
            for (unsigned int i = 1; i < input.size(); i++) {
                char p = input.at(i - 1);

                if (input.at(i) == quote && p != '\\') {

                    position = i + 1;
                    break;
                } else {
                    content += input.at(i);
                }

            }
            cout << "string " << content << endl;
            return true;

        }

        return false;
    }

    bool ECMAScript::checkPattern(string keyword, string any_of, std::string input,
            size_t& position) {

        return true;
    }

    void ECMAScript::variableContext(string &input, bool settedByThis) {
        //	cout << "VAR: " <<  input << endl;
        input = astox::stringAlg::trim_left(input);
        string _tmpbuf;
        int nump = 0;
        int numb = 0;
        int numdq = 0;
        int numsq = 0;

        for (unsigned int i = 0; i < input.size(); i++) {
            char c = input.at(i);

            _tmpbuf += c;
            char p = (i > 0) ? input.at(i) < 1 : ' ';

            switch (c) {
                case '"':
                    if (numsq % 2 == 0 && p != '\\') {
                        numdq++;
                    }

                    break;
                case '\'':
                    if (numdq % 2 == 0 && p != '\\') {
                        numsq++;
                    }
                    break;

                case '(':
                    if (numsq % 2 == 0 && numsq == 0) {
                        nump++;
                    }
                    break;
                case ')':
                    if (numsq % 2 == 0 && numsq == 0) {
                        nump++;
                    }
                    break;

                case '{':
                    if (numsq % 2 == 0 && numsq == 0) {
                        numb++;
                    }
                    break;

                case '}':
                    if (numsq % 2 == 0 && numsq == 0) {
                        numb++;
                    }
                    break;

                case ',':
                    if (numdq % 2 == 0 && numb % 2 == 0 && numsq % 2 == 0
                            && nump % 2 == 0) {
                        printf("VAR CONTEXT: %s ...  ,\n", _tmpbuf.c_str());

                        input = input.substr(_tmpbuf.size(), input.size());
                        _tmpbuf.clear();
                        variableContext(input);
                        return;
                        //system("pause");
                    }
                    break;

                    break;

                case ';':

                    if (numdq % 2 == 0 && numb % 2 == 0 && numsq % 2 == 0
                            && nump % 2 == 0) {
                        printf("VAR CONTEXT: %s ...;\n", _tmpbuf.c_str());
                        input = input.substr(i + 1, input.size());
                        //				rootContext(input);
                        //system("pause");
                        return;
                    }
                    break;

                default:

                    break;
            }
        }
    }

    bool ECMAScript::expectKeyword(std::string &input, std::string keyword,
            std::string allowedChars) {
        input = astox::stringAlg::trim_left(input);

        size_t poz = input.find(keyword);

        if (poz == 0) {
            if (!allowedChars.empty()) {
                char after = input.at(keyword.size());
                for (unsigned int i = 0; i < allowedChars.size(); i++) {
                    if (after == allowedChars.at(i)) {
                        return true;
                    }
                }
            }
            return true;
        }

        return false;
    }

    void ECMAScript::instructionalContext(std::string input, string type) {
        cout << type << " CONTEXT: " << endl;
        int lastPoz = 0;
        string condition = ecma::getExpressionBetween(input, '(', ')', lastPoz);
        cout << " CONDITION:" << condition << endl;
        input = input.substr(lastPoz, input.size());
        //	rootContext(input);
    }

    void ECMAScript::functionContext(std::string input, std::string functionName) {
        //	cout << "FUNCTION CONTEXT : " << input << endl;
        input = astox::stringAlg::trim_left(input);
        cout << "FUNCTION: " << endl;

        if (!functionName.empty()) {
            cout << "      NAME: " << functionName << endl;
        }
        int lastPoz = 0;
        //	cout << "INPUT: " << input << endl;
        string arguments = ecma::getExpressionBetween(input, '(', ')', lastPoz);
        input = input.substr(lastPoz, input.size());
        string block = ecma::getExpressionBetween(input, '{', '}', lastPoz);
        cout << " ARGUMENTS: " << arguments << endl;
        cout << "     BLOCK: " << block << endl;
        string rest = input.substr(lastPoz, input.size());
        //	rootContext(rest);
    }




    SScriptData * SScriptData::npos = new SScriptData("", OP_UNSET);

    SScriptData::SScriptData(std::string leftOperand, int operationAfter) {
        _operationAfter = operationAfter;
        _buffer = astox::stringAlg::trim(leftOperand);
    }

    void SScriptData::setOperationAfter(int _after) {
        _operationAfter = _after;
    }

    std::string SScriptData::getBuffer() {
        return _buffer;
    }

    int SScriptData::getOperation() {
        return _operationAfter;
    }

    void SScriptData::debug() {
        using namespace std;
        string text = _buffer;
        text.append("> (type: ");

        switch (_operationAfter) {
            case OP_RETURN:
                text.append(" return");
                break;
            case OP_ASSIGN:
                text.append(" assign");
                break;
            case OP_CALEE:
                text.append(" calee");
                break;
            case OP_ADDITION:
                text.append(" addition");
                break;
            case OP_SUBSTRACT:
                text.append(" substract");
                break;
            case OP_MULTIPLY:
                text.append(" multiply");
                break;
            case OP_DIVIDE:
                text.append(" divide");
                break;
            case OP_BIGGER:
                text.append(" bigger");
                break;
            case OP_LESSER:
                text.append(" lesser");
                break;
            case OP_INHERITED:
                text.append(" inherited/executable");
                break;
            case OP_TEXT_ONLY:
                text.append(" text only");
                break;
            case OP_MODULUS:
                text.append(" modulus");
                break;
            case OP_INSTRUCTION_END:
                text.append(" instruction end");
                break;
            case OP_EQUALS:
                text.append(" equals");
                break;
            case OP_EQUALS_STRICT:
                text.append(" equals strict");
                break;
            case OP_BIGGER_EQ:
                text.append(" bigger eq");
                break;
            case OP_LESSER_EQ:
                text.append(" lesser eq");
                break;
            case OP_DIFFERS:
                text.append(" differs");
                break;
            case OP_DIFFERS_STRICT:
                text.append(" differs strict");
                break;
            case OP_OR:
                text.append(" or");
                break;
            case OP_OR_STRICT:
                text.append(" or strict");
                break;
            case OP_AND:
                text.append(" and");
                break;
            case OP_AND_STRICT:
                text.append(" and strict");
                break;
            case OP_SHIFT_LEFT:
                text.append(" shift left");
                break;
            case OP_SHIFT_RIGHT:
                text.append(" shift right");
                break;
            default:
                cout << " UNDEFINED OPERATION: " << _operationAfter << endl;
                break;
        }
        text.append(")");
        cout << text << endl;
    }





} /* namespace astox */
