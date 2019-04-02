/*
 * syntaxjstest.h
 *
 *  Created on: 5 Sep 2017
 *      Author: alex
 */

#ifndef ASTOX_TEST_SYNTAXJSTEST_H_
#define ASTOX_TEST_SYNTAXJSTEST_H_

#include "staticvars.h"

using namespace astox;
using namespace astox::scriptengine;
using namespace astox::testing;
using namespace astox::ecma::methods;
using namespace std;




static inline void test_serializers() {
	using namespace serializers;
	Array * arr = new Array(lxc);
	assertEq(to_csv(arr), "");
	arr->push(String::New("test", lxc));
	assertEq(to_csv(arr), "test");
	arr->push(String::New("unu", lxc));
	assertEq(to_csv(arr), "test,unu");
	arr->push(Number::New(23, lxc));
	assertEq(to_csv(arr), "test,unu,23");
	arr->push(Boolean::New(false, lxc));
	assertEq(to_csv(arr), "test,unu,23,false");
	arr->push(String::New("false", lxc));
	assertEq(to_csv(arr), "test,unu,23,false,false");
	arr->push(Object::New(lxc));
	assertEq(to_csv(arr), "test,unu,23,false,false,[object]");
	arr->push(Function::New(-1,lxc));
	assertEq(to_csv(arr), "test,unu,23,false,false,[object],[function]");
	Array *child = new Array(lxc);
	arr->push(child);
	assertEq(to_csv(arr), "test,unu,23,false,false,[object],[function],");
	child->push(Boolean::New(true, lxc));
	assertEq(to_csv(arr), "test,unu,23,false,false,[object],[function],true");
	child->push(new Array(lxc));
	child->push(new Array(lxc));
	child->push(new Array(lxc));
	assertEq(to_csv(arr), "test,unu,23,false,false,[object],[function],true,,,");
}
//TODO
//alert((![]+[])[+[]]+(![]+[])[+!+[]]+([![]]+[][[]])[+!+[]+[+[]]]+(![]+[])[!+[]+!+[]]);
//alert(++[[]][+[]]+[+[]]);
//see more at https://wtfjs.com/
// and http://www.jsfuck.com/
static inline void test_esoteric() {

	//{a:1}[['a']] => 1

	assertEq(eval("(![]+[])[+[]]+(![]+[])[+!+[]]+([![]]+[][[]])[+!+[]+[+[]]]+(![]+[])[!+[]+!+[]]", lxc, ctx), "fail", ASTX_LOCATION);
//	std::exit(-1);
	assertEq(eval("[+!+[]+[+[]]]", lxc, ctx), "['10']", ASTX_LOCATION);
	assertEq(eval("(![]+[])[+[]]+(![]+[])[+!+[]]+([![]]+[][[]])[+!+[]+[+[]]]", lxc, ctx), "fai", ASTX_LOCATION);

	assertEq(eval("(![]+[])[+[]]+(![]+[])[+!+[]]+([![]]+[][[]])[10]", lxc, ctx), "fai", ASTX_LOCATION);
	assertEq(eval("(![]+[])[+[]]+(![]+[])[+!+[]]+([![]]+[][[]])", lxc, ctx), "fafalseundefined", ASTX_LOCATION);
	assertEq(eval("[true] + function(){}", lxc, ctx), "truefunction (){}", ASTX_LOCATION);
	assertEq(eval("([![]]+[][[]])", lxc, ctx), "falseundefined", ASTX_LOCATION);

	//;
	assertEq(eval("(![]+[])[+[]]+(![]+[])[+!+[]]+([![]])", lxc, ctx), "fafalse", ASTX_LOCATION);
	assertEq(eval("[][[]]", lxc, ctx), "undefined", ASTX_LOCATION);
	assertEq(eval("[![]]", lxc, ctx), "[false]", ASTX_LOCATION);
	assertEq(eval("(![]+[])[+[]]+(![]+[])[+!+[]]", lxc, ctx), "fa", ASTX_LOCATION);
	assertEq(eval("+!+[]", lxc, ctx), "1", ASTX_LOCATION);
	assertEq(eval("(![]+[])[+[]]+(![]+[])[4]", lxc, ctx), "fe", ASTX_LOCATION);
	assertEq(eval("(![]+[])[+[]]+(![]+[])", lxc, ctx), "ffalse", ASTX_LOCATION);
	assertEq(eval("(![]+[])[+[]]+'unu'", lxc, ctx), "funu", ASTX_LOCATION);
	assertEq(eval("(![]+[])[+[]]", lxc, ctx), "f", ASTX_LOCATION);
	assertEq(eval("([1,2,3])[1]", lxc, ctx), "2", ASTX_LOCATION);
	assertEq(eval("[][+[]]", lxc, ctx), "undefined", ASTX_LOCATION);

	//square brackets selection tests:
	assertEq(eval("[1,2,[4]][2]", lxc, ctx), "[4]", ASTX_LOCATION);
	assertEq(eval("[1,2,3][2]", lxc, ctx), "3", ASTX_LOCATION);
	assertEq(eval("[1,[1,2,3]][1]", lxc, ctx), "[1,2,3]", ASTX_LOCATION);
	assertEq(eval("[1,[1,2,3]][1][2]", lxc, ctx), "3", ASTX_LOCATION);
	assertEq(eval("[1,[1,2,[false, 2]]][1][2][0]", lxc, ctx), "false", ASTX_LOCATION);

	assertEq(eval("[0,1,2,3][3]", lxc, ctx), "3", ASTX_LOCATION);
	assertEq(eval("[[],{}]", lxc, ctx), "[[],{}]", ASTX_LOCATION);
	assertEq(eval("[[]]", lxc, ctx), "[[]]", ASTX_LOCATION);
	assertEq(eval("[[]]", lxc, ctx), "[[]]", ASTX_LOCATION);
	assertEq(eval("+[]", lxc, ctx), "0", ASTX_LOCATION);
	assertEq(eval("'false'[1]", lxc, ctx), "a", ASTX_LOCATION);
	assertEq(eval("![]", lxc, ctx), "false", ASTX_LOCATION);
	assertEq(eval("(![]+[])", lxc, ctx), "false", ASTX_LOCATION);
	assertEq(eval("{a:1}['a']", lxc, ctx), "1", ASTX_LOCATION);

//	assertEq(eval("(++[])", lxc, ctx), "false", ASTX_LOCATION);
	//system("pause");
//	std::exit(-1);
}


#endif /* SYNTAXJSTEST_H_ */
