/*
 * Bam.cpp
 *
 *  Created on: Sep 4, 2014
 *      Author: alexandru
 */


#include "CssLinter.h"


using namespace std;

CssLintErrorThrower::CssLintErrorThrower() {

}

CssLinter::CssLinter() {
    numDefs = 0;
    numRight = 0;
    numEndRule = 0;
    numq = 0;
    numcomm = 0;
    numrbrac = 0;
    lastUsedStat = -1;

    queryDefStarted = false;
    waitForQueryFinish = false;
    numQueries = 0;
}

CssContext::CssContext(std::string in, int contextType) {
    _original = in;
    _contextType = contextType;
    _line = 0;
    _contextSubType = CSS_CONTEXT_SUBTYPE_INHERITABLE;
}

CssContext::CssContext() {
    _original = "rootCssContext";
    _contextType = CSS_CONTEXT_UNDEFINED;
    _line = 0;
    _contextSubType = CSS_CONTEXT_SUBTYPE_INHERITABLE;
}

std::string CssContext::selfCheck() {

    string response;

    if (_contextSubType == CSS_CONTEXT_SUBTYPE_INHERITABLE) {
        std::map<std::string, CssContext>::iterator chlds;
        std::map<std::string, std::string> _tmpBuf;
        for (chlds = childs.begin(); chlds != childs.end(); ++chlds) {
            if (chlds->second.getSubType() == CSS_CONTEXT_SUBTYPE_RULE) {
                string t = chlds->second.selfCheck();
                if (_tmpBuf.find(t) != _tmpBuf.end()) {
                    _err.duplicatedRuleInLastDef(chlds->second.getOriginalDef(), chlds->second.getLine(), t);
                }
                _tmpBuf[t] = t;
            }
        }
    }
    else if (_contextSubType == CSS_CONTEXT_SUBTYPE_RULE) {
        std::vector<std::string> splittedByComma = astox::stringAlg::splitByChar(_original, ',');
        //	cout << "    lastRule     " << splittedByComma.at(splittedByComma.size()-1) << definitions.size() <<  endl;
        response = splittedByComma.at(splittedByComma.size() - 1) + "{ ";
        for (std::map<std::string, std::string>::iterator dfsit = definitions.begin(); dfsit != definitions.end(); ++dfsit) {
            response.append(dfsit->first);
            response.append(":");
            response.append(dfsit->second);
        }
        response.append("}");
    }

    return response;

}

void CssContext::setDefinition(std::string input, int line) {
    using namespace std;

    _line = line;

    _original = input = astox::stringAlg::trim(input);
    //	cout << "CssContext::setDefinition( " << _original << "{ " << endl;

    if (_original.empty()) {
        _err.emptyDefContext(_line);
        return;
    }




    if (_original.size() > 5) {
        if (_original.find("@media") == 0) {
            _contextType = CSS_CONTEXT_MEDIA_QUERY;
            _contextSubType = CSS_CONTEXT_SUBTYPE_INHERITABLE;
            return;
        }
        else if (_original.find("@keyframes") == 0) {
            _contextType = CSS_CONTEXT_KEYFRAMES;
            _contextSubType = CSS_CONTEXT_SUBTYPE_INHERITABLE;
            if (_original.find(",") != std::string::npos) {
                _err.commaNotAllowedInDef(_line, _original);
            }
            return;
        }
        else if (_original.find("@font-face") == 0) {
            _contextType = CSS_DEFINITION;
            _contextSubType = CSS_CONTEXT_SUBTYPE_RULE;
            if (_original.find(",") != std::string::npos) {
                _err.commaNotAllowedInDef(_line, _original);
            }
            return;
        }
    }






    _contextType = CSS_CONTEXT_RULE;

    _contextSubType = CSS_CONTEXT_SUBTYPE_RULE;

    return;


}

std::string CssContext::toJson(bool asRoot) {
    std::map<std::string, std::string>::iterator strIt;
    std::map<std::string, CssContext>::iterator ctxIt;

    string r;
    unsigned int num = 0;
    if (!definitions.empty()) {
        r.append("{");
        //		r.append("\"originalSelector\":\"");
        //		r.append(_original);
        //		r.append("\", ");
        r.append("\"rules\": [");

        for (strIt = definitions.begin(); strIt != definitions.end(); ++strIt) {
            num++;
            r.append("{\"");
            r.append(strIt->first);
            r.append("\":\"");
            r.append(strIt->second);
            r.append("\"}");
            if (num < definitions.size() - 1) {
                r.append(",");
            }
        }
        r.append("] ");
        r.append("}");
    }

    if (!childs.empty()) {
        if (asRoot) {
            r.append("{");
        }

        for (ctxIt = childs.begin(); ctxIt != childs.end(); ++ctxIt) {
            num++;
            r.append("\"");
            r.append(ctxIt->first);
            r.append("\" : ");
            r.append(ctxIt->second.toJson(false));

            if (num < childs.size() - 1) {
                r.append(",\n");
            }
        }

        if (asRoot) {
            r.append("}");
        }
    }

    return r;
}

std::string CssContext::toString() {
    std::map<std::string, std::string>::iterator strIt;
    std::map<std::string, CssContext>::iterator ctxIt;

    string r;
    if (!definitions.empty()) {
        for (strIt = definitions.begin(); strIt != definitions.end(); ++strIt) {
            r.append(strIt->first);
            r.append(":");
            r.append(strIt->second);
            r.append("; ");
        }
    }

    if (!childs.empty()) {
        for (ctxIt = childs.begin(); ctxIt != childs.end(); ++ctxIt) {
            r.append(ctxIt->first);
            r.append("{");
            r.append(ctxIt->second.toString());
            r.append("}\n");
        }
    }

    return r;
}

void CssLinter::parse(std::string input, void (_onIterate) (int status, std::string _currentBuffer, int line), int lineNo) {
    //astox::stringAlg::removeClosingComments(input);

    for (unsigned int i = 0; i < input.size(); i++) {
        char c = input.at(i);
        //		cout << i << endl;
        switch (c) {

            case '@':
                if (numq % 2 == 0) {
                    waitForQueryFinish = true;
                } else {
                    _buffer += c;
                }
                break;

            case '*':
                if (waitForQueryFinish) {
                    _buffer += c;
                } else {
                    if (numq % 2 == 0) {
                        if ((i > 0) && input.at(i - 1) == '/') {
                            numcomm++;
                            _buffer += c;
                            //						cout  << "COMMENT STARTED"<< numcomm << _buffer << endl;
                        } else {
                            _buffer += c;
                        }
                    } else {
                        _buffer += c;
                    }
                }

                break;

            case '/':
                if (numq % 2 == 0 && numrbrac == 0) {
                    if ((i > 0) && input.at(i - 1) == '*') {
                        numcomm--;
                        _buffer += c;
                        lastUsedStat = CSS_COMMENT_BLOCK;
                        //					cout << _buffer  << "COMMENT FINISHED" << numcomm  << endl;
                        if (_onIterate != NULL) {
                            _onIterate(CSS_COMMENT_BLOCK, _buffer, lineNo);
                        } else {
                            onIterate(CSS_COMMENT_BLOCK, _buffer, lineNo);
                        }

                        if (numcomm < 0) {
                            _err.parsingWarning(lineNo, _buffer);
                            numcomm = 0;
                        }

                        _buffer.clear();
                    } else {
                        _buffer += c;
                    }
                } else {
                    _buffer += c;
                }
                break;

            case '(':
                if (numq % 2 == 0 && !waitForQueryFinish) {
                    numrbrac++;
                    _buffer += c;
                } else {
                    _buffer += c;
                }
                break;
            case ')':
                if (numq % 2 == 0 && !waitForQueryFinish) {
                    numrbrac--;
                    _buffer += c;
                } else {
                    _buffer += c;
                }
                break;
            case '"':
            case '\'':
                _buffer += c;
                if (numcomm == 0) { //daca nu e in comment
                    numq++;
                }


                break;


            case ';':


                if (numq % 2 == 0 && numcomm == 0) {

                    if (waitForQueryFinish) {
                        lastUsedStat = CSS_INHERITABLE;
                        waitForQueryFinish = false;
                    } else {
                        lastUsedStat = CSS_RIGHT_DATA;
                    }

                    if (_onIterate != NULL) {
                        _onIterate(lastUsedStat, _buffer, lineNo);
                    } else {
                        onIterate(lastUsedStat, _buffer, lineNo);
                    }
                    _buffer.clear();
                } else {
                    _buffer += c;
                }
                break;


            case ':':
                if (waitForQueryFinish) {
                    _buffer += c;
                } else if (lastUsedStat == CSS_END_BRACKET || lastUsedStat == CSS_COMMENT_BLOCK) {
                    _buffer += c;
                } else if (numq % 2 == 0 && numcomm == 0) {
                    lastUsedStat = CSS_LEFT_DATA;
                    if (_onIterate != NULL) {
                        _onIterate(CSS_LEFT_DATA, _buffer, lineNo);
                    } else {
                        onIterate(CSS_LEFT_DATA, _buffer, lineNo);
                    }
                    _buffer.clear();
                } else {
                    _buffer += c;
                }
                break;

            case '{':

                if (numq % 2 == 0 && numcomm == 0) {
                    if (waitForQueryFinish) {
                        lastUsedStat = CSS_INHERITABLE;
                        waitForQueryFinish = false;
                    } else {
                        lastUsedStat = CSS_DEFINITION;
                    }

                    if (_onIterate != NULL) {
                        _onIterate(lastUsedStat, _buffer, lineNo);
                    } else {
                        onIterate(lastUsedStat, _buffer, lineNo);
                    }
                    _buffer.clear();
                }
                else {
                    _buffer += c;
                }
                break;

            case '}':
                if (numq % 2 == 0 && numcomm == 0) {
                    lastUsedStat = CSS_END_BRACKET;
                    if (_onIterate != NULL) {
                        _onIterate(CSS_END_BRACKET, _buffer, lineNo);
                    } else {
                        onIterate(CSS_END_BRACKET, _buffer, lineNo);
                    }
                    _buffer.clear();
                } else {
                    _buffer += c;
                }
                break;

            default:
                _buffer += c;
                break;
        }

    }

}

void CssLinter::onIterate(int status, std::string _currentBuffer, int line) {
    _currentBuffer = astox::stringAlg::trim(_currentBuffer);

    if (status == CSS_INHERITABLE && (_currentBuffer.size() > 5) && _currentBuffer.find("import") == 0) {
        status = CSS_IMPORT_DIRECTIVE;
        CssLine currentLine(status, _currentBuffer, line);
        _lines.push_back(currentLine);
    }
    else if (status == CSS_END_BRACKET) {
        //verifica daca s-a incheiat ok cu ";" linia
        if (!_currentBuffer.empty()) {
            //cout << "ERROR, ; required at line " << line  << endl;
            _err.endLineRequired(line);
            //system("pause");
        }
        CssLine currentLine(status, _currentBuffer, line);
        _lines.push_back(currentLine);

    }
    else if (!_currentBuffer.empty()) {
        CssLine currentLine(status, _currentBuffer, line);
        _lines.push_back(currentLine);

    }

    cout << CSS_STATUS[status] << _currentBuffer << endl;
}

CssContext CssLinter::digestLines() {
    if (lastUsedStat != CSS_END_BRACKET || lastUsedStat != CSS_COMMENT_BLOCK) {
        _err.parsingError(_lines.at(_lines.size() - 1).line, astox::stringAlg::trim(_buffer));
    }

    CssContext r = _rootContext.digestLineList(_lines);


    //cout << r.toString() << endl;
    //	 astox::stringAlg::saveStringToFile( r.toString(), "parsed.css");

    r.selfCheck();

    return r;

}

CssContext CssContext::digestLineList(std::vector<CssLine> _lines) {
    string _cstr;
    std::vector<std::string> _lastCreatedRules;
    std::string _lastLeftData;


    int numOpenedDefs = 0;

    bool inheritableStarted = false;
    std::vector<CssLine> _tempBuffer;
    CssContext _returnContext;

    std::string inheritableName;
    std::string _lastDefinitionGiven;

    for (unsigned int i = 0; i < _lines.size(); i++) {


        _cstr = _lines.at(i).buffer;

        //cout  << CSS_STATUS[_lines.at(i).status] << _cstr  << endl;

        switch (_lines.at(i).status) {
            case CSS_IMPORT_DIRECTIVE:
                _imports.push_back(_lines.at(i).buffer);
                //			 CssContext c(_cstr, CSS_IMPORT_DIRECTIVE);

                //			 cout << "@import directive: " << _cstr << endl;
                break;

            case CSS_COMMENT_BLOCK:
                _comments.push_back(_lines.at(i).buffer);
                //			 cout << "comment block: " << _cstr << endl;
                break;



            case CSS_LEFT_DATA:
                if (inheritableStarted) {
                    _tempBuffer.push_back(_lines.at(i));
                } else {
                    _lastLeftData = _lines.at(i).buffer;
                }
                break;

            case CSS_RIGHT_DATA:
                if (inheritableStarted) {
                    _tempBuffer.push_back(_lines.at(i));
                } else {
                    if (!_lastDefinitionGiven.empty()) {
                        _returnContext.childs[_lastDefinitionGiven].definitions[_lastLeftData] = _lines.at(i).buffer;
                    } else {
                        _returnContext.definitions[_lastLeftData] = _lines.at(i).buffer;
                        //					 cout << "rule: " <<  _lastLeftData << ":" << _lines.at(i).buffer << "; "<< endl;
                    }
                }

                break;

            case CSS_DEFINITION:
                //			 cout << "RULE FOUND: " << numOpenedDefs << endl;
                if (inheritableStarted) {
                    _tempBuffer.push_back(_lines.at(i));
                } else {
                    CssContext child;
                    child.setDefinition(_cstr, _lines.at(i).line);
                    _returnContext.childs[_cstr] = child;
                    _lastDefinitionGiven = _cstr;
                }

                numOpenedDefs++;

                break;

            case CSS_INHERITABLE:

                if (inheritableStarted) {
                    _tempBuffer.push_back(_lines.at(i));
                } else {
                    inheritableStarted = true;
                    inheritableName = _lines.at(i).buffer;
                }
                numOpenedDefs++;
                //			 cout << "   >>>>inherit started" << inheritableName << endl;
                break;

            case CSS_END_BRACKET:
                numOpenedDefs--;
                //			 cout << "   < end brcket" << numOpenedDefs << endl;
                if (inheritableStarted && numOpenedDefs == 0) {
                    CssContext child = digestLineList(_tempBuffer);
                    string childName = "@" + inheritableName;
                    child.setDefinition(childName, _lines.at(i).line);

                    if (_returnContext.childs.find(childName) != _returnContext.childs.end()) {
                        merge(child);
                        _err.definitionDuplicated(childName, _lines.at(i).line);
                    } else {

                        _returnContext.childs[childName] = child;
                    }




                    _tempBuffer.clear();
                    inheritableStarted = false;
                    //				 cout << "   <<<<inherit finished" << numOpenedDefs << endl;
                    //				 numOpenedDefs = 0;
                }
                break;

        }


    }


    return _returnContext;

}
