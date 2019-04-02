/*
 * stack.cpp
 *
 *  Created on: 19 May 2017
 *      Author: alex
 */

#include "astox.h"

namespace astox {
using namespace std;
using namespace scriptengine;




stack * stack::_in = new stack();
Memoize * Memoize::_in = new Memoize();

stack_node::stack_node(){
	prev = NULL;
};

stack::stack() {
	first = new stack_node;
	first->msg = "init";
	current = first;
}

stack::~stack() {

}

stack * stack::getInstance(){
	return _in;
};

Memoize * Memoize::getInstance(){
	return _in;
};

String * Memoize::getString(const lexic &l, string c){
	if(_sm[l].find(c) ==_sm[l].end()){
		char * m = const_cast<char*> (c.c_str());
		_sm[l][c] = new String(m, l);
		return _sm[l][c];
	}

	if(_sm[l][c] == NULL){
		char * m = const_cast<char*> (c.c_str());
		_sm[l][c] = new String(m, l);
		return _sm[l][c];
	}

	return _sm[l][c];
};

Boolean * Memoize::getBoolean(const scriptengine::lexic &l, bool v){
	if(_bm.find(l) == _bm.end()){
		_bm[l] = new Boolean*[2];
		_bm[l][0] = new Boolean(true, l);
		_bm[l][1] = new Boolean(false, l);
	}
	if(v){
		return _bm[l][0];
	}
	return _bm[l][1];
};

void stack::add(std::string method, std::string file, const char* format, ...){
	stack_node * sn = new stack_node;
	char * dest;
	va_list argptr;
	va_start(argptr, format);
#ifdef ASTOX_COMPILER_MSC 
	int len;
	len = _vscprintf(format, argptr) + 1;
	dest = (char *)malloc(len * sizeof(char));
	vsprintf_s(dest, len, format, argptr);
#else
	dest = (char *)malloc(1024 * sizeof(char));
	vsprintf(dest, format, argptr);
#endif
	va_end(argptr);
	sn->msg.append(dest);
	sn->msg.append(" [");
	sn->msg.append(method);
	sn->msg.append(" ");
	sn->msg.append(file);
	sn->msg.append("]");
	sn->prev = current;
	current = sn;
};

void stack::add(std::string msg, std::string method, std::string file){
	stack_node * sn = new stack_node;
	sn->msg.append(msg);
	sn->msg.append(" [");
	sn->msg.append(method);
	sn->msg.append(" ");
	sn->msg.append(file);
	sn->msg.append("]");
	sn->prev = current;
	current = sn;
};

stxtr stack::toString(){
	stack_node * c = current;
	stxtr r = "\n";

	int cnt = 0;
	while(c != NULL){
//		string s = string(cnt, ' ');
//		r.append(s);
		r.append("|_");
		r.append(c->msg);
		r.append("\n");
		c = c->prev;
		cnt++;
	}
	return r;
};

stxtr blurt(stack * root){
	return root->toString();
};

void var_dump(stack * op, bool bth){
	if(bth){
		ASTX_BLURT_THIS_FUNC;
		printf("%s", blurt(op).c_str());
	} else {
		printf("stack %s ", blurt(op).c_str());
	}
};

void var_dump(stack * op, char * fmt, ...){
	 printf("%s", blurt(op).c_str());
	 va_list args;
	 va_start(args, fmt);
	 vprintf(fmt,args);
	 va_end(args);
}

} /* namespace astox */
