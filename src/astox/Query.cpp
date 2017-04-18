/*
 * Query.cpp
 *
 *  Created on: Jul 28, 2014
 *      Author: alexandru
 */


//TODO USE std::compare in query
#include "astox.h"


namespace astox {

    using namespace std;

    //const AstxKeywordsMap::value_type x[] = {
    //		std::make_pair("<", K_LESS),
    //		std::make_pair(">", K_GREATER),
    //		std::make_pair("<=", K_LESSOREQ),
    //		std::make_pair(">=", K_GREATOREQ),
    //		std::make_pair("==", K_EQ),
    //		std::make_pair("~", K_LIKE),
    //		std::make_pair("ANY", K_VANY),
    //		std::make_pair("STR", K_VSTR),
    //		std::make_pair("NUM", K_VNUM),
    //		std::make_pair("BOL", K_VBOL),
    //		std::make_pair("OBJ", K_VOBJ),
    //		std::make_pair("ARR", K_VARR),
    //		std::make_pair("*", K_WILDCARD)
    //};

    //AstxKeywordsMap Query::AstxKeywords(x, x + sizeof x / sizeof x[0]);

    //map<int, int> m = {{1,2}, {3,4}};

    //Query::Query(Object * object)
    //{
    //	cobject = object;
    //}


    //bool Query::isLike(std::string leftOperand, std::string rightOperand)
    //{
    //	if(rightOperand == "*")
    //	{
    //		return true;
    //	}
    //
    //	if(rightOperand.size() > 0)
    //	{
    //		if(rightOperand.at(0) == '*' && rightOperand.at(rightOperand.size()-1) == '*')
    //		{
    //			rightOperand = rightOperand.substr(1, rightOperand.size()-2);
    //			rightOperand = astox::stringAlg::escapeStringQuotes(rightOperand);
    //			return leftOperand.find(rightOperand) > 0;
    //		}
    //
    //		if(rightOperand.at(0) == '*')
    //		{
    //			rightOperand = rightOperand.substr(1, rightOperand.size());
    //			rightOperand = astox::stringAlg::escapeStringQuotes(rightOperand);
    //			return (leftOperand.substr(leftOperand.size()-rightOperand.size(), leftOperand.size()) == rightOperand);
    //		}
    //
    //		if(rightOperand.at(rightOperand.size()-1) == '*')
    //		{
    //			rightOperand = rightOperand.substr(0, rightOperand.size()-1);
    //			rightOperand = astox::stringAlg::escapeStringQuotes(rightOperand);
    //			return  (leftOperand.substr(0, rightOperand.size()) == rightOperand);
    //		}
    //
    //	}
    //	rightOperand = astox::stringAlg::escapeStringQuotes(rightOperand);
    //	return (leftOperand == rightOperand);
    //}
    //
    //
    //bool Query::compare(Value* value, std::string comparisonSign, std::string rightTerm)
    //{
    //
    //	cout << "|" << value->toString() << comparisonSign << rightTerm << "|" << endl;
    //
    //	int sign = Query::AstxKeywords[comparisonSign];
    //
    //
    //	switch(sign)
    //	{
    //		case K_LESS:
    //			if(value->isNumber())
    //			{
    //				return (value->toDouble() < atof(rightTerm.c_str()));
    //			}
    //
    //			if(astox::stringAlg::stringIsNumber(rightTerm))
    //			{
    //				if(value->isObject() || value->isArray())
    //				{
    //					return (value->length() < atoi(rightTerm.c_str()));
    //				}
    //			}
    //			return value->toString() < rightTerm;
    //			break;
    //
    //		case K_GREATER:
    //			if(value->isNumber())
    //			{
    //				return (value->toDouble() > atof(rightTerm.c_str()));
    //			}
    //
    //			if(astox::stringAlg::stringIsNumber(rightTerm))
    //			{
    //				if(value->isObject() || value->isArray())
    //				{
    //					return (value->length() > atoi(rightTerm.c_str()));
    //				}
    //			}
    //			return value->toString() > rightTerm;
    //			break;
    //
    //		case K_EQ:
    //			if(value->isNumber())
    //			{
    //				return (value->toDouble() == atof(rightTerm.c_str()));
    //			}
    //
    //			if(astox::stringAlg::stringIsNumber(rightTerm))
    //			{
    //				if(value->isObject() || value->isArray())
    //				{
    //					return (value->length() == atoi(rightTerm.c_str()));
    //				}
    //			}
    //
    //			return value->toString() == rightTerm;
    //			break;
    //
    //
    //
    //		case K_LESSOREQ:
    //			if(value->isNumber())
    //			{
    //				return (value->toDouble() <= atof(rightTerm.c_str()));
    //			}
    //
    //			if(astox::stringAlg::stringIsNumber(rightTerm))
    //			{
    //				if(value->isObject() || value->isArray())
    //				{
    //					return (value->length() <= atoi(rightTerm.c_str()));
    //				}
    //			}
    //
    //			return value->toString() <= rightTerm;
    //			break;
    //
    //
    //		case K_GREATOREQ:
    //			if(value->isNumber())
    //			{
    //				return (value->toDouble() >= atof(rightTerm.c_str()));
    //			}
    //
    //			if(astox::stringAlg::stringIsNumber(rightTerm))
    //			{
    //				if(value->isObject() || value->isArray())
    //				{
    //					return (value->length() >= atoi(rightTerm.c_str()));
    //				}
    //			}
    //
    //			return value->toString() >= rightTerm;
    //			break;
    //
    //
    //		case K_LIKE:
    //			return Query::isLike(value->toString(), rightTerm);
    //			break;
    //	}
    //
    //
    //
    //
    //	return false;
    //}
    //
    //void Query::run(std::string in)
    //{
    //	int numq = 0;
    //	string temp;
    //	vector<string> container;
    //	for(unsigned int i = 0 ; i < in.size(); i++)
    //	{
    //		char c = in.at(i);
    //		switch(c)
    //		{
    //		case '\"':
    //		case '\'':
    //			numq++;
    //			temp+=c;
    //			break;
    //
    //		case ' ':
    //		case '\n':
    //			if(numq%2==0)
    //			{
    //				container.push_back(temp);
    //				temp.clear();
    //			}
    //			else
    //			{
    //				temp+=c;
    //			}
    //			break;
    //
    //		default:
    //			temp+=c;
    //			break;
    //		}
    //	}
    //
    //	if(!temp.empty())
    //	{
    //		container.push_back(temp);
    //	}
    //}
    //
    //void Query::describe()
    //{
    //	printf("query runs as follows:\n");
    //	printf("ACTION | RANGE | VALUETYPE | SEARCHED | CONDITION | TERM  | OPTIONS | \n");
    //	printf("  GET  |  *    |    ANY    |    KEY   |     =     | \"any\" |   AND   | \n");
    //	printf("  SET  | [x-x] |    STR    |    VAL   |     ~     |  any  |  JOIN   | \n");
    //	printf("  DEL  |       |    BOL    |          |     >     | *any  |         | \n");
    //	printf(" RGET  |       |    NUM    |          |     <     | any*  |         | \n");
    //	printf(" RSET  |       |    OBJ    |          |    >=     | *any* |         | \n");
    //	printf(" RDEL  |       |    ARR    |          |    <=     |  123  |         | \n");
    //}

}
