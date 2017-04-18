/*
 * Xhtml.cpp
 *
 *  Created on: Jan 29, 2014
 *      Author: alexandru
 */

#include "astox.h"

namespace astox {
    namespace xhtml {

        XHTMLNode::XHTMLNode(std::string input, bool virtualNode, bool strict) {
            // TODO Auto-generated constructor stub

            hasAttributes = false;

            if (virtualNode) {
                nodeStat = ASTX_NODE_STAT_VIRTUAL;

                input = astox::stringAlg::replaceWhiteSpace(input);
                inputValue = input;
            } else {
                if (input.at(0) == '/' && input.at(input.size() - 1) != '/') { //closed
                    attributesLine = input.substr(1, input.size() - 1);
                    nodeStat = ASTX_NODE_STAT_CLOSING;
                } else if (input.at(input.size() - 1) == '/' && input.at(0) != '/') { //self closed
                    attributesLine = input.substr(0, input.size() - 1);
                    nodeStat = ASTX_NODE_STAT_SELF_CLOSED;
                } else if (input.at(0) == '!') { //comment
                    attributesLine = input;
                    nodeStat = ASTX_NODE_STAT_COMMENT;
                } else { //opened
                    attributesLine = input;
                    nodeStat = ASTX_NODE_STAT_OPENING;
                }
            }



            if (!virtualNode) {
                std::vector<std::string> result = astox::stringAlg::splitByWhiteSpace(attributesLine);
                pTagName = astox::stringAlg::getFirstTillWhitespace(attributesLine);
                attributesLine = attributesLine.substr(pTagName.length(), attributesLine.size() - 1);
            } else {
                pTagName = "@virtual";
            }


            //	std::cout << "node:|" << input << "|\n VIRTUAL: " << virtualNode;

            generateAttributes();
            //	traceInfo();


        }

        XHTMLNode::~XHTMLNode() {
            // TODO Auto-generated destructor stub
        }

        void XHTMLNode::generateAttributes() {
            attributesLine = astox::stringAlg::trim(attributesLine);

            if (!attributesLine.empty()) {
                astox::lang::HashParser p;
                attributes = p.fromHash(attributesLine);
                hasAttributes = true;
                //		p.trace();
            }
        }

        void XHTMLNode::traceInfo() {

            std::cout << "attributesLine: " << attributesLine << std::endl;
            std::cout << "tagName: " << pTagName << std::endl;

            std::cout << "______________" << std::endl;

        }

        std::vector<XHTMLNode *> parse(std::string input, bool strict) {
            using namespace std;

            astox::stringAlg::trim(input);

            std::vector<XHTMLNode *> response;

            if (input.empty() || input.size() <= 2) {
                Error e(Error::PARSING_ERROR, "XHTML_H_: unable to parseXml empty input", __LINE__, __FILENAME__);
                return response;
            }




            string temp;
            string text;
            int numQuotes = 0;
            string stat = ASTX_PARSE_STAT_INTXTMODE;

            for (unsigned int i = 0; i < input.length(); i++) {
                char c = input.at(i);

                switch (c) {

                    case '"':
                        temp += c;
                        numQuotes++;
                        break;

                    case '<':
                        if (stat == ASTX_PARSE_STAT_INTXTMODE && numQuotes % 2 == 0) {
                            astox::stringAlg::trim(temp);
                            if (!temp.empty()) {
                                response.push_back(new XHTMLNode(temp, true, strict));
                                temp.clear();
                                stat = ASTX_PARSE_STAT_INTAGMODE;
                            }
                        } else {
                            temp += c;
                        }
                        break;

                    case '>':
                        if (numQuotes % 2 == 0 && !temp.empty()) { //daca nu e intre ghilimele
                            astox::stringAlg::trim(temp);
                            if (!temp.empty()) {
                                response.push_back(new XHTMLNode(temp, false, strict));
                                temp.clear();
                                stat = ASTX_PARSE_STAT_INTXTMODE;
                            }
                        } else {
                            temp += c;
                        }
                        break;

                    default:
                        temp += c;
                        break;
                }






            }

            return response;

        };

        astox::Object * fromXHTMLNode(XHTMLNode * node, astox::xhtml::XMLKeyNames names) {
            Object * resp = new Object();

            if (node->getStat() == ASTX_NODE_STAT_VIRTUAL) {
                resp->addKey(names.value, new String(node->inputValue));
                resp->addKey(names.isVirtual, Boolean::New(true));
            } else {
                resp->addKey(names.tagName, new String(node->tagName()));
            }

            resp->addKey(names.type, Number::New(node->getStat()));

            if (node->hasAttributes) {
                Object * attributes = new Object();
                attributes->setKeys(node->attributes);
                resp->addKey(names.attributes, attributes);
            }
            return resp;
        }

        void fromXHTMLNodes(std::vector<XHTMLNode *> nodes, astox::Object * buffer) {
            XMLKeyNames names;
            fromXHTMLNodes(nodes, buffer, names);
        }

        void fromXHTMLNodes(std::vector<XHTMLNode *> nodes, astox::Object * buffer, astox::xhtml::XMLKeyNames names) {
            using namespace std;


            Object * current = Object::npos;
            Array * childs = new Array();
            std::vector<XHTMLNode *> listBuffer;

            int closed = 0;
            for (unsigned int i = 0; i < nodes.size(); i++) {
                XHTMLNode * currentNode = nodes.at(i);
                //		cout << currentNode->getStatStr() <<  "  " << currentNode->tagName() << endl;

                if (currentNode->getStat() == ASTX_NODE_STAT_OPENING) {


                    closed++;
                    if (closed == 1) {
                        //				cout << "start " << currentNode->tagName() << " " << closed << endl;
                        current = fromXHTMLNode(currentNode, names);
                    } else {
                        //				cout << "  opened c> " << currentNode->tagName() << " " << closed << endl;
                        listBuffer.push_back(currentNode);
                    }

                } else if (currentNode->getStat() == ASTX_NODE_STAT_CLOSING) {
                    closed--;
                    if (closed == 0) {
                        //				cout << "end " << currentNode->tagName() << " " << closed << endl;
                        childs->push(current);
                        fromXHTMLNodes(listBuffer, current, names);
                        listBuffer.clear();
                    } else {
                        //				cout << "   closed c> " << currentNode->tagName() << " " << closed << endl;
                        listBuffer.push_back(currentNode);
                    }

                } else {
                    if (closed == 0) {
                        if (currentNode->getStat() == ASTX_NODE_STAT_SELF_CLOSED ||
                                currentNode->getStat() == ASTX_NODE_STAT_VIRTUAL) {
                            //					cout << "self closed " << currentNode->tagName() << " " << closed << endl ;
                            childs->push(fromXHTMLNode(currentNode, names));
                        }
                    } else {
                        //				cout << "  tmp c> " << currentNode->tagName() << " " << closed << endl;
                        listBuffer.push_back(currentNode);
                    }
                }


            }

            buffer->addKey(names.childNodes, childs);


        }

        std::string XHTMLNode::tagName() {
            return pTagName;
        }

        int XHTMLNode::getStat() {
            return nodeStat;
        };

        std::string XHTMLNode::getStatStr() {
            switch (nodeStat) {
                case ASTX_NODE_STAT_CLOSING:
                    return "closing";
                    break;

                case ASTX_NODE_STAT_DOCTYPE:
                    return "doctype";
                    break;


                case ASTX_NODE_STAT_OPENING:
                    return "opening";
                    break;

                case ASTX_NODE_STAT_SELF_CLOSED:
                    return "self closed";
                    break;


                case ASTX_NODE_STAT_VIRTUAL:
                    return "virtual";
                    break;

                default:
                    return "UNDEFINED";
                    break;
            }
            return "UNDEFINED";
        }


    } /* namespace xhtml */
} /* namespace astox */
