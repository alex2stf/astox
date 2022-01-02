/*
 * stack.h
 *
 *  Created on: 19 May 2017
 *      Author: alex
 */

#ifndef STACK_H_
#define STACK_H_

#include "macros.h"
#include "lexic.h"

namespace astox {

class String;
class Number;
class Boolean;
class Array;
class Object;
class Function;
class Math;
class ObjectFactory;

#define object_map std::map<std::string, astox::Value *>
#define object_iterator std::map<std::string, astox::Value *>::iterator
#define object_list std::vector<astox::Value *>
#define vector_string  std::vector<std::string>
#define object_list_iterator std::vector<astox::Value *>::iterator

using namespace std;
using namespace scriptengine;
#define stx_strstr map < string, String * >
#define stx_dblnum map < double, Number * >
typedef map < lexic, stx_strstr > _strmap;
typedef map < lexic, stx_strstr > _nummap;
class Memoize {
	private:
		static Memoize * _in;
		_strmap _sm;
		_nummap _nm;
		map<lexic, Boolean **> _bm;

	public:
		static Memoize * getInstance();
		String * getString(const scriptengine::lexic &l, std::string value);
		Boolean * getBoolean(const scriptengine::lexic &l, bool value);
};


struct stack_node{
	std::string msg;
	stack_node * prev;
	stack_node();
};

class stack {
private:
	stack();
	static stack * _in;
	stack_node * first;
	stack_node * current;
public:
	static stack * getInstance();
	virtual ~stack();
	void add(std::string msg, std::string method, std::string file);
	void add(std::string method, std::string file, const char* format, ...);
	std::string toString();
};



void var_dump(stack * operation_node, bool bth = false);
void var_dump(stack * operation_node, char * fmt, ...);
stxtr blurt(stack * operation_node);

#define log_stack(...){ \
	printf("[%-23s|%05d|%-16s]", __FUNCTION__ , __LINE__, __FILENAME__);\
	printf(__VA_ARGS__); printf("\n"); \
}

#define method_not_implemented(){ \
	printf("[%-40s|% 5d|%-16s]\nMETHOD NOT IMPLEMENTED", ASTX_CURRENT_FUNCTION , __LINE__, __FILENAME__);\
	std::exit(-4);\
}

} /* namespace astox */
#endif /* STACK_H_ */
