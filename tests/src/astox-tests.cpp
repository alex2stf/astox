//include header
#include "astoxtests.h"

//import most of necessary namespaces
using namespace astox;
using namespace astox::scriptengine;
using namespace astox::testing;
using namespace astox::ecma::methods;
using namespace std;


//main definition
int main(int argc, char* argv[]) {

	




	std::string s;
	std::vector<std::string> vs;
	vector<RegExpMatch> matches;
	vector<string_stat> stats;
	int dfz = 0;
	command_node cmd;

	lexic obfus = Syntax::OBFUS;

	//validate if lexic is correct
	validate_lexic(obfus);


	run_all_tests();




	//Array * arr = Array::cast(eval("[[]]",  lxc, ctx));
	//log_stack("%s", blurt(arr).c_str());
	
	//system("pause");

	assertEq(eval("'a'.toString()",  lxc, ctx), "a", ASTX_LOCATION);
	eval("String.prototype.test2 = function(){ return this + 'plus'; }",  lxc, ctx);
	assertEq(eval("'a'.test2()",  lxc, ctx), "aplus", ASTX_LOCATION);
	assertEq(eval("'b'.test2()",  lxc, ctx), "bplus", ASTX_LOCATION);
	assertEq(eval("'plus-'.test2()",  lxc, ctx), "plus-plus", ASTX_LOCATION);

	eval("var str = String('unu');",  lxc, ctx);
	assertEq(eval("str",  lxc, ctx), "unu", ASTX_LOCATION);
	assertEq(eval("str.constructor",  lxc, ctx), "function (){CNSTRCT}", ASTX_LOCATION);

	//test env methods for js
	eval("var a = [1,2,'a']; a.customFunc = function(){ return 4;}",  lxc, ctx);
	assertEq(eval("a.customFunc()",  lxc, ctx), 4, ASTX_LOCATION);
	assertEq(eval("[1,2,'a'].toString()",  lxc, ctx), "[1,2,'a']", ASTX_LOCATION);

	//no ovveride allowed for native methods
	eval("var a = [1,2,'a']; a.toString = function(){ return 4;}",  lxc, ctx);
	assertEq(eval("a.toString()",  lxc, ctx), "[1,2,'a']", ASTX_LOCATION);

	assertEq(eval("[1,2,'a'].toString()",  lxc, ctx), "[1,2,'a']", ASTX_LOCATION);
	assertEq(eval("{a:1}.toJSON()",  lxc, ctx), "{\"a\":1}", ASTX_LOCATION);
	assertEq(eval("'str'.toJSON()",  lxc, ctx), "\"str\"", ASTX_LOCATION);
	assertEq(eval("'str'.charAt(0)",  lxc, ctx), "s", ASTX_LOCATION);
	assertEq(eval("'str'.charAt(1)",  lxc, ctx), "t", ASTX_LOCATION);
	assertEq(eval("'str'.charAt(2)",  lxc, ctx), "r", ASTX_LOCATION);
	assertEq(eval("'str'.replace('s', 'p')",  lxc, ctx), "ptr", ASTX_LOCATION);
	assertEq(eval("'stsrsa'.split('s')",  lxc, ctx), "['t','r','a']", ASTX_LOCATION);
	assertEq(eval("'stsrsa'.splitByChar('s')",  lxc, ctx), "['t','r','a']", ASTX_LOCATION);
	assertEq(eval("'cbda'.sort()",  lxc, ctx), "abcd", ASTX_LOCATION);
	assertEq(eval("'abc'.split('')",  lxc, ctx), "['a','b','c']", ASTX_LOCATION);
	assertEq(eval("'  a\nb\tc  '.killWhiteSpace()",  lxc, ctx), "abc", ASTX_LOCATION);
	assertEq(eval("[1,2,'a'].toString()",  lxc, ctx), "[1,2,'a']", ASTX_LOCATION);

	assertEq(eval("1.toString()",  lxc, ctx), "1", ASTX_LOCATION);
	assertEq(eval("1.toString()",  lxc, ctx)->type(), ASTOX_VALUE_STRING, ASTX_LOCATION);
	assertEq(eval("true.toString()",  lxc, ctx), "true", ASTX_LOCATION);
	assertEq(eval("true.toString()",  lxc, ctx)->type(), ASTOX_VALUE_STRING, ASTX_LOCATION);
	assertEq(eval("false.toString()",  lxc, ctx), "false", ASTX_LOCATION);
	assertEq(eval("false.toString()",  lxc, ctx)->type(), ASTOX_VALUE_STRING, ASTX_LOCATION);

	assertEq(eval("{}.toString()",  lxc, ctx), "{}", ASTX_LOCATION);
	assertEq(eval("print.toString()",  lxc, ctx), "function (){[native function]}", ASTX_LOCATION);

	//test boolean operations:
	assertEq(eval("(true + true)",  lxc, ctx), 2, ASTX_LOCATION);
	assertEq(eval("(true + true) * (true + true)",  lxc, ctx), 4, ASTX_LOCATION);
	assertEq(eval("(true + true) * (true + true) - true",  lxc, ctx), 3, ASTX_LOCATION);

	//comparison of three numbers
	assertEq(eval("1 < 2 < 3",  lxc, ctx), "true", ASTX_LOCATION);
	assertEq(eval("3 > 2 > 1",  lxc, ctx), "false", ASTX_LOCATION);

	//private vs public simulation
	s = "var b = (function(){ function privateMethod(){ print(\"called from local context\"); return 98; } return { publicMethod: privateMethod }; })(); ";
	eval(s,  lxc, ctx);
	assertEq(eval(" b.publicMethod();",  lxc, ctx), 98, ASTX_LOCATION);
	s = "var a = (function(){ var prop = 24;";
	s.append("function local (){ print('private prop ' + prop); return 'p'+prop; } ");
	s.append("return { local: local }; })();");
	eval(s,  lxc, ctx);
	assertEq(eval("a.local()",  lxc, ctx), "p24", ASTX_LOCATION);
	assertEq(eval("'unu'.length",  lxc, ctx), 3, ASTX_LOCATION);
	assertEq(eval("'doi'.reverse()",  lxc, ctx), "iod", ASTX_LOCATION);
	assertEq(eval("' doi '.trim()",  lxc, ctx), "doi", ASTX_LOCATION);
	assertEq(eval("[1, 2, 3].reverse()",  lxc, ctx), "[3,2,1]", ASTX_LOCATION);

	//test strip_comments with OBFUS lexic
	s = strip_comments("--cmnt1\n data1 --c'm'nt2\ndata2 #=block=#", obfus, &stats);
	assertEq(s, " data1 data2 ", ASTX_LOCATION);
	assertEq(stats.at(0).data, "--cmnt1", ASTX_LOCATION);
	assertEq(stats.at(0).stat, COMMENT, ASTX_LOCATION);
	assertEq(stats.at(1).data, " data1 ", ASTX_LOCATION);
	assertEq(stats.at(1).stat, NONE, ASTX_LOCATION);
	assertEq(stats.at(2).data, "--c'm'nt2", ASTX_LOCATION);
	assertEq(stats.at(2).stat, COMMENT, ASTX_LOCATION);
	assertEq(stats.at(3).data, "data2 ", ASTX_LOCATION);
	assertEq(stats.at(3).stat, NONE, ASTX_LOCATION);
	assertEq(stats.at(4).data, "#=block=", ASTX_LOCATION);
	assertEq(stats.at(4).stat, COMMENT_BLOCK, ASTX_LOCATION);

	//test strip_comments with JS lexic
	s = strip_comments("//c'm'nt1\n d'a//'ta1 //cmnt1\ndata2 /*block*/", lxc, &stats);
	assertEq(s, " d'a//'ta1 data2 ", ASTX_LOCATION);
	assertEq(stats.at(0).data, "//c'm'nt1", ASTX_LOCATION);
	assertEq(stats.at(0).stat, COMMENT, ASTX_LOCATION);
	assertEq(stats.at(1).data, " d'a//'ta1 ", ASTX_LOCATION);
	assertEq(stats.at(1).stat, NONE, ASTX_LOCATION);
	assertEq(stats.at(2).data, "//cmnt1", ASTX_LOCATION);
	assertEq(stats.at(2).stat, COMMENT, ASTX_LOCATION);
	assertEq(stats.at(3).data, "data2 ", ASTX_LOCATION);
	assertEq(stats.at(3).stat, NONE, ASTX_LOCATION);
	assertEq(stats.at(4).data, "/*block*", ASTX_LOCATION);
	assertEq(stats.at(4).stat, COMMENT_BLOCK, ASTX_LOCATION);

	s = strip_comments("//cmnt1\n data1 //cmnt2\n data2 /*block*/data3", lxc, &stats);
	assertEq(s, " data1  data2 data3", ASTX_LOCATION);
	assertEq(stats.at(0).data, "//cmnt1", ASTX_LOCATION);
	assertEq(stats.at(0).stat, COMMENT, ASTX_LOCATION);
	assertEq(stats.at(1).data, " data1 ", ASTX_LOCATION);
	assertEq(stats.at(1).stat, NONE, ASTX_LOCATION);
	assertEq(stats.at(2).data, "//cmnt2", ASTX_LOCATION);
	assertEq(stats.at(2).stat, COMMENT, ASTX_LOCATION);
	assertEq(stats.at(3).data, " data2 ", ASTX_LOCATION);
	assertEq(stats.at(3).stat, NONE, ASTX_LOCATION);
	assertEq(stats.at(4).data, "/*block*", ASTX_LOCATION);
	assertEq(stats.at(4).stat, COMMENT_BLOCK, ASTX_LOCATION);
	assertEq(stats.at(5).data, "data3", ASTX_LOCATION);
	assertEq(stats.at(5).stat, NONE, ASTX_LOCATION);


	//conditional switch test
	s = "function conditionalSwitch( value, cond, callback ) { for (var i = 1; i < arguments.length; i += 2) { if (arguments[i](value) ) { arguments[i+1](value);return; } } }";
	s.append(" var width, height;");
	s.append(" conditionalSwitch( 5.1,");
	s.append("  function(val) { return val > 10 },");
	s.append("  function () { height = 48;  width = 36},");
	s.append("  function(val) { return val > 5 },");
	s.append("  function () { height = 40; width = 30 },");
	s.append("  function(val) { return true },");
	s.append("  function () {height = 16; width = 12;}");
	s.append(");");
	s.append("var raw_value = 11.0;");
	s.append("switch(raw_value) {");
	s.append(" case (raw_value > 10.0):  height = 48; width = 36; break;");
	s.append(" case (raw_value > 5.0): height = 40;  width = 30; break;");
	s.append(" default: height = 16;  width = 12; break;");
	s.append("}");
	eval(s, lxc, ctx);
	assertEq(eval("width", lxc, ctx), 12);
	assertEq(eval("height", lxc, ctx), 16);


	assertEq(eval("function t(a){ if(a > 5) {return 8;} return t(a+1) + 1} t(2)", lxc, ctx), 12);
	assertEq(eval("function t(a){ if(a > 5) {return 5;} return t(a+1) + t(a+a)} t(2)", lxc, ctx), 35);

	s = "var re = (function(){ return 1+46; })();";
	eval(s, lxc, ctx);
	assertEq(eval("re", lxc, ctx), "47", ASTX_LOCATION);

	s = util::regex_replace_all("bzzzbz", "z", "x", JSREG_GLOBAL);
	assertEq(s, "bxxxbx", ASTX_LOCATION);


	s = "var re = /[tv]iv/ig; var result = re.exec('tiv biv viv tac vac jac');";
	eval(s, lxc, ctx);
	assertEq(eval("result", lxc, ctx), "['tiv','viv']", ASTX_LOCATION);

	assertIsNull(build_regex_from_string("//", lxc), ASTX_LOCATION);
	assertRegExpFlags(build_regex_from_string("/g/", lxc), JSREG_BASIC, ASTX_LOCATION);
	assertRegExpFlags(build_regex_from_string("/g/g", lxc), JSREG_GLOBAL, ASTX_LOCATION);
	assertRegExpFlags(build_regex_from_string("/g/i", lxc), JSREG_IGNORECASE, ASTX_LOCATION);
	assertRegExpFlags(build_regex_from_string("/g/n", lxc), JSREG_NOSUBS, ASTX_LOCATION);
	assertRegExpFlags(build_regex_from_string("/g/y", lxc), JSREG_STICKY, ASTX_LOCATION);
	assertRegExpFlags(build_regex_from_string("/g/u", lxc), JSREG_UNICODE, ASTX_LOCATION);
	assertIsNull(build_regex_from_string("/g/t", lxc), ASTX_LOCATION);

	assertRegExpFlags(build_regex_from_string("/g/ui", lxc),
			JSREG_UNICODE | JSREG_IGNORECASE,
			ASTX_LOCATION);

	assertRegExpFlags(build_regex_from_string("/g/igm", lxc),
				JSREG_IGNORECASE | JSREG_GLOBAL | JSREG_MULTILINE,
				ASTX_LOCATION);

	assertRegExpFlags(build_regex_from_string("/g/mig", lxc),
					JSREG_MULTILINE | JSREG_IGNORECASE | JSREG_GLOBAL,
					ASTX_LOCATION);

	assertRegExpFlags(build_regex_from_string("/g/minguy", lxc),
						JSREG_MULTILINE | JSREG_IGNORECASE | JSREG_NOSUBS | JSREG_GLOBAL | JSREG_UNICODE | JSREG_STICKY,
						ASTX_LOCATION);

	assertRegExpFlags(build_regex_from_string("/g/m", lxc),
								JSREG_MULTILINE,
								ASTX_LOCATION);

	assertRegExpFlags(build_regex_from_string("/g/mmm", lxc),
							JSREG_MULTILINE | JSREG_MULTILINE | JSREG_MULTILINE,
							ASTX_LOCATION);


	s = "var a = []; for(i in {a:1,b:2,c:3} ){a.push(i); }";
	eval(s, lxc, ctx);
	assertEq(eval("print(a)", lxc, ctx), "['a','b','c']", ASTX_LOCATION);

	//std::exit(-1);
	s = "var a = []; b = {a:1,b:2,c:3}, c = [] ; for(i in b){c.push(i); a.push( { i  :b[i] } ) }";
	eval(s, lxc, ctx);
	assertEq(eval("c", lxc, ctx), "['a','b','c']", ASTX_LOCATION);
	assertEq(eval("a", lxc, ctx), "[{a:1},{b:2},{c:3}]", ASTX_LOCATION);
	//std::exit(-1);

	s = "unu in";
	assertTrue(string_ends_with(s, "in", lxc) == true, "", ASTX_LOCATION);
	assertTrue(string_ends_with(s, "instanceof", lxc) == false, "", ASTX_LOCATION);

	s = "unu instanceof";
	assertTrue(string_ends_with(s, "in", lxc) == false, "", ASTX_LOCATION);
	assertTrue(string_ends_with(s, "instanceof", lxc) == true, "", ASTX_LOCATION);

	assertEq(eval("'a' in {a:1}", lxc, ctx), "true", ASTX_LOCATION);
	assertEq(eval("'c' in {a:1}", lxc, ctx), "false", ASTX_LOCATION);

//
	eval("var a = 45; function ert(){ throw 'test failure' }; try { ert(); } catch(e){ a = 89 };", lxc, ctx);
	assertEq(eval("a", lxc, ctx), 89, ASTX_LOCATION);


//

	//delete
	s = "var a = 1; delete a;";
	eval(s, lxc, ctx);
	assertEq(eval("a", lxc, ctx), "undefined", ASTX_LOCATION);


	//iterative fibonacci:
	s = "function fibonacci(num){ var a = 1, b = 0, temp;  while (num >= 0){  temp = a; a = a + b; b = temp; num--; } return b; }";
	eval(s, lxc, ctx);
	assertEq(eval("fibonacci(5)", lxc, ctx), 8, "iterative fibonacci", ASTX_LOCATION);
	assertEq(eval("fibonacci(10)", lxc, ctx), 89,  "iterative fibonacci", ASTX_LOCATION);

	assert_expression_failure(eval("throw new Error('test')", lxc, ctx), "test");
	assert_expression_failure(eval("throw new Error('message', 'source', 'method', 4)", lxc, ctx), "message");
	assert_expression_failure(eval("throw new Error('no line', 'source', 'method')", lxc, ctx), "no line");
	assert_expression_failure(eval("throw new Error('no line,no method', 'source')", lxc, ctx), "no line,no method");
	assert_expression_failure(eval("throw new Error({message:'whaaat'})", lxc, ctx), "whaaat");
	assert_expression_failure(eval("throw new Error({message:'whaaat', line: 42})", lxc, ctx), "42");
	assert_expression_failure(eval("throw new Error({source:'src', line: 42})", lxc, ctx), "src");
	assert_expression_failure(eval("throw new Error({source:'src', method: 'my_mth'})", lxc, ctx), "my_mth");
	assert_expression_failure(eval("throw 'inline error definition'", lxc, ctx), "inline error definition");
	assert_expression_failure(eval("throw ['inline error array', 'source', 'method', 4]", lxc, ctx), "inline error array");
	assert_expression_failure(eval("throw {message:'inline error object'}", lxc, ctx), "inline error object");




	s = "var a = {b: 1, c: {d: 2}}; delete a.c.d;";
	eval(s, lxc, ctx);
	assertEq(eval("a.c.d", lxc, ctx), "undefined", ASTX_LOCATION);
	assertEq(eval("a.c", lxc, ctx), "{}", ASTX_LOCATION);
	assertEq(eval("a", lxc, ctx), "{b:1,c:{}}", ASTX_LOCATION);


	//try catch finally

	s = "function t(){var s = 5; try { anycode.do() } catch(e){ return e}}";
	eval(s, lxc, ctx);
	assertContains(eval("t()", lxc, ctx), "cannot access property", ASTX_LOCATION);

	s = "function t(){var s = 5; try { anycode.do() } catch(e){ return 4}}";
	eval(s, lxc, ctx);
	assertEq(eval("t()", lxc, ctx), 4, "try catch finally", ASTX_LOCATION);

	s = "function t(){var e = 5; try { anycode.do() } catch(e){ return e} finally {return e}}";
	eval(s, lxc, ctx);
	assertEq(eval("t()", lxc, ctx), 5, "try catch finally", ASTX_LOCATION);

	//try finally
	s = "function t(){var e = 23; try { anycode.do() } finally {return e}}";
	eval(s, lxc, ctx);
	assertEq(eval("t()", lxc, ctx), 23, "try  finally", ASTX_LOCATION);





	//while ... {}
	s = "var a = 10, b = []; while(a > 0) { a--; b.push(a); }";
	eval(s, lxc, ctx);
	assertEq(eval("b", lxc, ctx), "[9,8,7,6,5,4,3,2,1,0]");
	s = "var a = 10, b = []; while(a > 0) { a--; if(a < 5) { break; } b.push(a); }";
	eval(s, lxc, ctx);
	assertEq(eval("b", lxc, ctx), "[9,8,7,6,5]", ASTX_LOCATION);
	s = "function s(){ var a = 10, b = []; while(a > 0) { a--; if(a < 5) { return 21 } b.push(a); } return 14; }";
	eval(s, lxc, ctx);
	assertEq(eval("s()", lxc, ctx), "21");
	s = "function s(){ var a = 10, b = []; while(a > 0) { a--; if(a < 5) { continue; } b.push(a); } return b; }";
	eval(s, lxc, ctx);
	assertEq(eval("s()", lxc, ctx), "[9,8,7,6,5]", ASTX_LOCATION);

	//while ... do ...
	s = "var a = 10, b = []; while(a > 0) do { a--; b.push(a); }";
	eval(s, lxc, ctx);
	assertEq(eval("b", lxc, ctx), "[9,8,7,6,5,4,3,2,1,0]");
	s = "var a = 10, b = []; while(a > 0) do { a--; if(a < 5) { break; } b.push(a); }";
	eval(s, lxc, ctx);
	assertEq(eval("b", lxc, ctx), "[9,8,7,6,5]");
	s = "function s(){ var a = 10, b = []; while(a > 0) do { a--; if(a < 5) { return 21 } b.push(a); } return 14; }";
	eval(s, lxc, ctx);
	assertEq(eval("s()", lxc, ctx), "21");
	s = "function s(){ var a = 10, b = []; while(a > 0) do { a--; if(a < 5) { continue; } b.push(a); } return b; }";
	eval(s, lxc, ctx);
	assertEq(eval("s()", lxc, ctx), "[9,8,7,6,5]");

	//do ... while
	s = "var a = 10, b = []; do { a--; b.push(a); } while (a > 0)";
	eval(s, lxc, ctx);
	assertEq(eval("b", lxc, ctx), "[9,8,7,6,5,4,3,2,1,0]", "", ASTX_LOCATION);
	s = "var a = 10, b = []; do {a--; if(a < 5) { break; } b.push(a); } while (a > 0)";
	eval(s, lxc, ctx);
	assertEq(eval("b", lxc, ctx), "[9,8,7,6,5]", ASTX_LOCATION);
	s = "function s(){ var a = 10, b = [];  do { a--; if(a < 5) { return 21 } b.push(a); } while(a > 0) return 14; } ";
	eval(s, lxc, ctx);
	assertEq(eval("s()", lxc, ctx), "21");
	s = "function s(){ var a = 10, b = []; do { a--; if(a < 5) { continue; } b.push(a); } while(a > 0); return b; }";
	eval(s, lxc, ctx);
	assertEq(eval("s()", lxc, ctx), "[9,8,7,6,5]", ASTX_LOCATION);



	//recursive fibonacci
	s = "function fibonacci(num) { if (num <= 1){ return 1; }; return  fibonacci(num - 1) + fibonacci(num - 2); }";
	eval(s, lxc, ctx);
	assertEq(eval("fibonacci(9)", lxc, ctx), 55, "recursive fibonacci", ASTX_LOCATION);
	assertEq(eval("fibonacci(7)", lxc, ctx), 21, "recursive fibonacci", ASTX_LOCATION);




	//merge

	vs = split_by_keyword("case'case'case{case}(case)case[case]case\"case\"", "case", lxc);
	assert_equals(vs.size(), 4, "");
	assertEq(vs.at(0), "'case'");
	assertEq(vs.at(1), "{case}(case)");
	assertEq(vs.at(2), "[case]");
	assertEq(vs.at(3), "\"case\"");

	vs = split_by_keyword("case'case'case{case}(ca_se)case[case(){[case's']}]case", "case", lxc);

	assertEq(vs.at(0), "'case'");
	assertEq(vs.at(1), "{case}(ca_se)");
	assertEq(vs.at(2), "[case(){[case's']}]");

	vs = split_by_keyword("case 'case': blob case \"case\": 4, default: ssss", "case", lxc);
	assertEq(vs.at(0), " 'case': blob ");
	assertEq(vs.at(1), " \"case\": 4, default: ssss");

	vs.clear();
	vector<string> lefts;

	extract_case_statements("case 'case': blob case \"case\": 4, case do:   switch(b){ case 5: console.log(8);  case 3: console.log(3); break; } \t default: ssss",
			lxc, vs, lefts);
	assertEq(vs.at(0), "'case'", ASTX_LOCATION);
	assertEq(lefts.at(0), "blob", ASTX_LOCATION);
	assertEq(vs.at(1), "\"case\"", ASTX_LOCATION);
	assertEq(lefts.at(1), "4,", ASTX_LOCATION);
	assertEq(vs.at(2), "do", ASTX_LOCATION);
	assertEq(lefts.at(2), "switch(b){ case 5: console.log(8);  case 3: console.log(3); break; }",ASTX_LOCATION);
	string t67(lxc.indefault);
	assertEq(vs.at(3), t67, ASTX_LOCATION);
	assertEq(lefts.at(3), "ssss", ASTX_LOCATION);
	assert_equals(vs.size(), lefts.size(), "");


	assert_expression_failure(
			extract_case_statements("case 'case case': blob default: ssss default: ggg", lxc, vs, lefts),
			"cannot have more than 1 default statement");

	assert_expression_failure(
			extract_case_statements("case 1 case 2", lxc, vs, lefts),
			"invalid pair input");

	assert_expression_failure(
				pair_by_colon("unu :", lxc),
				"invalid pair input");

	assert_expression_failure(
					extract_case_statements("case 1: case 2: some code default: ", lxc, vs, lefts),
					"default statement cannot be empty");
	vs.clear(); lefts.clear();
//
	detect_instruction_type("switch(1){ case 2: block; default: loop} for(i=0; i < 9; i++){putc} rest", &cmd, lxc);
	assertEq(cmd.type, SWITCH_CASE, "", ASTX_LOCATION);
	assertEq(cmd.stmts.at(0), "1", ASTX_LOCATION);
	assertEq(cmd.blcks.at(0), lxc.inswitch, ASTX_LOCATION);
	assert_equals(cmd.stmts.size(), 3, "");
	assert_equals(cmd.stmts.size(), cmd.blcks.size(), "");
	assertEq(cmd.stmts.at(1), "2", ASTX_LOCATION);
	assertEq(cmd.blcks.at(1), "block;", ASTX_LOCATION);
	assertEq(cmd.stmts.at(2), lxc.indefault, ASTX_LOCATION);
	assertEq(cmd.blcks.at(2), "loop", ASTX_LOCATION);
	assertEq(cmd.next->type, FOR_LOOP, "", ASTX_LOCATION);
	assertEq(cmd.next->next->type, OPERATION, "", ASTX_LOCATION);

	vs.clear(); lefts.clear();
	extract_case_statements("case 1: case 2: case 3: action default: loop",
				lxc, vs, lefts);
	assertEq(vs.at(0), "1");
	assertEq(vs.at(1), "2");
	assertEq(vs.at(2), "3");
	assertEq(lefts.at(0), lxc.incase);
	assertEq(lefts.at(1), lxc.incase);
	assertEq(lefts.at(2), "action");
	assertEq(vs.at(3), lxc.indefault);
	assertEq(lefts.at(3), "loop");


	eval("var a = 1, b", lxc, ctx);
	eval("switch(a) { case 1: b = 2; case 2: b = 3; } ", lxc, ctx);
	assertEq(ctx->rget("b", lxc, ctx, RETURN_VAL), 3, "", ASTX_LOCATION);
	assertEq(ctx->rget("a", lxc, ctx, RETURN_VAL), 1, "", ASTX_LOCATION);
	eval("switch(a) { case 1: b = 49; break; case 2: b = 30} ", lxc, ctx);
	assertEq(ctx->rget("b", lxc, ctx, RETURN_VAL), 49, "", ASTX_LOCATION);
	assertEq(ctx->rget("a", lxc, ctx, RETURN_VAL), 1, "", ASTX_LOCATION);
	eval("var a = []; for(var i = 0; i < 10; i++){switch(i){ case i: a.push(i); }}", lxc, ctx);
	assertEq(ctx->rget("a", lxc, ctx, RETURN_VAL), "[0,1,2,3,4,5,6,7,8,9]", "", ASTX_LOCATION);









	eval("var a = [];",  lxc, ctx);
	assertEq(eval(" for(var i = 0; i < 6; i++){ if(i == 4){ continue; }; a.push(i); } print(a);", lxc, ctx), "[0,1,2,3,5]", "", ASTX_LOCATION);
//	assertEq(eval(" for(var i = 0; i < 6; i++){ if(i == 4){ print(\"break\"); break; } print(i); }", lxc, ctx), "[[['break']]]", "", ASTX_LOCATION);
	//TODO restore
	//assertEq(eval(" for(var i = 0; i < 6; i++){ if(i == 4){ print(\"break\"); break; } print(i); } print(i);", lxc, ctx), "[['break'],4]", "", ASTX_LOCATION);
	//assertEq(eval(" var a = [1,2,3]; for(var i = 0; i < 7; i++){ if(i == 4){ print(i+\"IF\") } else { print(i)} } print(a);", lxc, ctx), "[[1,2,3],[1,2,3]]", "", ASTX_LOCATION);
	eval("var a = [];",  lxc, ctx);
	assertEq(eval("for(var i = 0; i < 7; i++){ a.push(i) } print(a);", lxc, ctx), "[0,1,2,3,4,5,6]", "", ASTX_LOCATION);
	eval("a = []; a.push(1); a.push(1); a.push(2); ", lxc, ctx);
	assertEq(eval("a", lxc, ctx), "[1,1,2]", "", ASTX_LOCATION);

	//std::exit(-1);
	detect_instruction_type("25,5", &cmd, lxc);
	assertEq(cmd.type, OPERATION, "", ASTX_LOCATION);
	detect_instruction_type(" var a = 1;", &cmd, lxc);

	assertEq(cmd.type, VAR_ASSIGN, "", ASTX_LOCATION);

	assertEq(cmd.stmts.at(0), "a = 1", "", ASTX_LOCATION);
	detect_instruction_type(" var a = 1;;;;;;;;;;;;;;;;;;;;;;;", &cmd, lxc);
	assertEq(cmd.type, VAR_ASSIGN, "", ASTX_LOCATION);
	assertEq(cmd.stmts.at(0), "a = 1", "", ASTX_LOCATION);
	detect_instruction_type(" var a = 1, b =f, g, h, 's', \";;;;\" and ; space;;;;;;;;;;;;;;;;;;;;;;;", &cmd, lxc);
	assertEq(cmd.type, VAR_ASSIGN, "", ASTX_LOCATION);
	assertEq(cmd.stmts.at(0), "a = 1, b =f, g, h, 's', \";;;;\" and ; space", "", ASTX_LOCATION);
	assertEq(execute_command("\"a\"", lxc, ctx, dfz), "a", "", ASTX_LOCATION);
	detect_instruction_type(" return 1", &cmd, lxc);
	assertEq(cmd.stmts.at(0), "1", "", ASTX_LOCATION);
	assertEq(cmd.type, RETURN_VAL, "", ASTX_LOCATION);
	detect_instruction_type("for(1){do}", &cmd, lxc);
	assertEq(cmd.stmts.at(0), "1", "", ASTX_LOCATION);
	assertEq(cmd.blcks.at(0), "do", "", ASTX_LOCATION);
	assertEq(cmd.type, FOR_LOOP, "", ASTX_LOCATION);

	detect_instruction_type("for(({})){{}}", &cmd, lxc);
	assertEq(cmd.stmts.at(0), "({})", "", ASTX_LOCATION);
	assertEq(cmd.blcks.at(0), "{}", "", ASTX_LOCATION);
	assertEq(cmd.type, FOR_LOOP, "", ASTX_LOCATION);

	s = "if(1){2}else if(3){4}else if(5){6} else{7}";
	detect_instruction_type(s, &cmd, lxc);
	assertEq(cmd.stmts.at(0), "1", "", ASTX_LOCATION);
	assertEq(cmd.stmts.at(1), "3", "", ASTX_LOCATION);
	assertEq(cmd.stmts.at(2), "5", "", ASTX_LOCATION);
	assertEq(cmd.blcks.at(0), "2", "", ASTX_LOCATION);
	assertEq(cmd.blcks.at(1), "4", "", ASTX_LOCATION);
	assertEq(cmd.blcks.at(2), "6", "", ASTX_LOCATION);
	assertEq(cmd.blcks.at(3), "7", "", ASTX_LOCATION);

	detect_instruction_type("if(i > 20) { print(\"i > 20\");} else if(1) {2} else {print(\"i == 15\");}", &cmd, lxc);

	assertEq(cmd.stmts.at(0), "i > 20", "", ASTX_LOCATION);
	assertEq(cmd.blcks.at(0), " print(\"i > 20\");", "", ASTX_LOCATION);
	assertEq(cmd.blcks.at(2), "print(\"i == 15\");", "", ASTX_LOCATION);


	detect_instruction_type("if (", &cmd, lxc);
	assertEq(cmd.type, OPERATION, "testing detect_instruction_type OPERATION", ASTX_LOCATION);

	detect_instruction_type("if ;", &cmd, lxc);
	assertEq(cmd.type, OPERATION, "testing detect_instruction_type OPERATION", ASTX_LOCATION);

	detect_instruction_type("var a;", &cmd, lxc);
	assertEq(cmd.type, VAR_ASSIGN, "testing detect_instruction_type VAR_ASSIGN", ASTX_LOCATION);
	assertEq(cmd.sat(0), "a", ASTX_LOCATION);

	detect_instruction_type("var ;", &cmd, lxc);
	assertEq(cmd.type, OPERATION, "testing detect_instruction_type OPERATION", ASTX_LOCATION);
	assertEq(cmd.sat(0), "var", ASTX_LOCATION);

	detect_instruction_type("break;", &cmd, lxc);
	assertEq(cmd.type, BREAK_CTX, "testing detect_instruction_type BREAK_CTX", ASTX_LOCATION);

	detect_instruction_type("break ;", &cmd, lxc);
	assertEq(cmd.type, BREAK_CTX, "testing detect_instruction_type BREAK_CTX", ASTX_LOCATION);


	detect_instruction_type("return ;", &cmd, lxc);
	assertEq(cmd.type, RETURN_VAL, "testing detect_instruction_type RETURN_VAL", ASTX_LOCATION);
	detect_instruction_type("return xxxxxxxxx", &cmd, lxc);
	assertEq(cmd.type, RETURN_VAL, "testing detect_instruction_type RETURN_VAL", ASTX_LOCATION);

	detect_instruction_type("for ;", &cmd, lxc);
	assertEq(cmd.type, OPERATION, "testing detect_instruction_type OPERATION", ASTX_LOCATION);
	assertEq(cmd.sat(0), "for", ASTX_LOCATION);


	detect_instruction_type("function (){};data", &cmd, lxc);
	assertEq(cmd.type, FUNCTION_DEF, "testing detect_instruction_type FUNCTION_DEF", ASTX_LOCATION);
	assertEq(cmd.sat(0), "function (){}", ASTX_LOCATION);
	assertEq(cmd.next->type, OPERATION, "cmd.next->type OPERATION", ASTX_LOCATION);
	assertEq(cmd.next->sat(0), "data", ASTX_LOCATION);

	detect_instruction_type("function (", &cmd, lxc);
	assertEq(cmd.type, OPERATION, "testing detect_instruction_type OPERATION", ASTX_LOCATION);
	assertEq(cmd.sat(0), "function (", ASTX_LOCATION);

	detect_instruction_type("var", &cmd, lxc);
	assertEq(cmd.type, OPERATION, "testing detect_instruction_type OPERATION", ASTX_LOCATION);
	assertEq(cmd.sat(0), "var", ASTX_LOCATION);

	detect_instruction_type("switch ;", &cmd,  lxc);
	assertEq(cmd.type, OPERATION, "testing detect_instruction_type OPERATION", ASTX_LOCATION);
	assertEq(cmd.sat(0), "switch", ASTX_LOCATION);

	detect_instruction_type("switch{",  &cmd, lxc);
	assertEq(cmd.type, OPERATION, "testing detect_instruction_type OPERATION", ASTX_LOCATION);
	assertEq(cmd.sat(0), "switch{", ASTX_LOCATION);

	detect_instruction_type("try ;;;;",  &cmd, lxc);
	assertEq(cmd.type, OPERATION, "testing detect_instruction_type OPERATION", ASTX_LOCATION);
	assertEq(cmd.sat(0), "try", ASTX_LOCATION);

	detect_instruction_type("try {}",  &cmd, lxc);
	assertEq(cmd.type, OPERATION, "testing detect_instruction_type OPERATION", ASTX_LOCATION);
	assertEq(cmd.sat(0), "try {}", ASTX_LOCATION);

	detect_instruction_type("try + catch",  &cmd, lxc);
	assertEq(cmd.type, OPERATION, "testing detect_instruction_type OPERATION", ASTX_LOCATION);
	assertEq(cmd.sat(0), "try + catch", ASTX_LOCATION);

	detect_instruction_type("try()catch{",  &cmd, lxc);
	assertEq(cmd.type, OPERATION, "testing detect_instruction_type OPERATION", ASTX_LOCATION);
	assertEq(cmd.sat(0), "try()catch{", ASTX_LOCATION);


	detect_instruction_type("try{tryblock}catch(exception){catchblock}finally{finallyblock}neextcommand", &cmd, lxc);
	assertEq(cmd.sat(0), "tryblock", ASTX_LOCATION);
	assertEq(cmd.bat(0), "exception", ASTX_LOCATION);
	assertEq(cmd.sat(1), "catchblock", ASTX_LOCATION);
	assertEq(cmd.sat(2), "finallyblock", ASTX_LOCATION);
	assertEq(cmd.next->sat(0), "neextcommand", ASTX_LOCATION);


	assert_expression_failure(
			detect_instruction_type("try{tryblock}catch(exception)", &cmd, lxc),
			"invalid catch statement"
	);

	assert_expression_failure(
			detect_instruction_type("try{tryblock}", &cmd, lxc),
			"invalid [try{T}catch(E){C}] syntax"
	);

	detect_instruction_type("try { tryblock } catch ( exception ) { catchblock }  finally { finallyblock }\t neextcommand   ", &cmd, lxc);
	assertEq(cmd.sat(0), " tryblock ", ASTX_LOCATION);
	assertEq(cmd.bat(0), " exception ", ASTX_LOCATION);
	assertEq(cmd.sat(1), " catchblock ", ASTX_LOCATION);
	assertEq(cmd.sat(2), " finallyblock ", ASTX_LOCATION);
	assertEq(cmd.next->sat(0), "neextcommand", ASTX_LOCATION);


	detect_instruction_type("try { tryblock } catch ( exception ) { catchblock }\t neextcommand   ", &cmd, lxc);
	assertEq(cmd.sat(0), " tryblock ", ASTX_LOCATION);
	assertEq(cmd.bat(0), " exception ", ASTX_LOCATION);
	assertEq(cmd.sat(1), " catchblock ", ASTX_LOCATION);
	assertEq(cmd.next->sat(0), "neextcommand", ASTX_LOCATION);





	detect_instruction_type("try} catch(){} finally{}", &cmd, lxc);
	assertEq(cmd.type, OPERATION, "optype", ASTX_LOCATION);
	assertEq(cmd.sat(0), "try} catch(){} finally{}", ASTX_LOCATION);

	detect_instruction_type("atch(){} finally{}", &cmd, lxc);
	assertEq(cmd.type, OPERATION, "optype", ASTX_LOCATION);
	assertEq(cmd.sat(0), "atch(){} finally{}", ASTX_LOCATION);

	detect_instruction_type("var ", &cmd, lxc);
	assertEq(cmd.type, OPERATION, "optype", ASTX_LOCATION);
	assertEq(cmd.sat(0), "var", ASTX_LOCATION);

	detect_instruction_type("if{", &cmd, lxc);
	assertEq(cmd.type, OPERATION, "optype", ASTX_LOCATION);
	assertEq(cmd.sat(0), "if{", ASTX_LOCATION);

	detect_instruction_type("function(", &cmd, lxc);
	assertEq(cmd.type, OPERATION, "optype", ASTX_LOCATION);
	assertEq(cmd.sat(0), "function(", ASTX_LOCATION);

	detect_instruction_type("function s", &cmd, lxc);
	assertEq(cmd.type, OPERATION, "optype", ASTX_LOCATION);
	assertEq(cmd.sat(0), "function s", ASTX_LOCATION);

	detect_instruction_type("functionsss", &cmd, lxc);
	assertEq(cmd.type, OPERATION, "optype", ASTX_LOCATION);
	assertEq(cmd.sat(0), "functionsss", ASTX_LOCATION);

	detect_instruction_type("function-", &cmd, lxc);
	assertEq(cmd.type, OPERATION, "optype", ASTX_LOCATION);
	assertEq(cmd.sat(0), "function-", ASTX_LOCATION);


	detect_instruction_type("do -", &cmd, lxc);
	assertEq(cmd.type, OPERATION, "optype", ASTX_LOCATION);
	assertEq(cmd.sat(0), "do -", ASTX_LOCATION);

	detect_instruction_type("do {}", &cmd, lxc);
	assertEq(cmd.type, OPERATION, "optype", ASTX_LOCATION);
	assertEq(cmd.sat(0), "do {}", ASTX_LOCATION);


	detect_instruction_type("do ;", &cmd, lxc);
	assertEq(cmd.type, OPERATION, "optype", ASTX_LOCATION);
	assertEq(cmd.sat(0), "do ;", ASTX_LOCATION);

	detect_instruction_type("do;", &cmd, lxc);
	assertEq(cmd.type, OPERATION, "optype", ASTX_LOCATION);
	assertEq(cmd.sat(0), "do;", ASTX_LOCATION);


	detect_instruction_type("while ;", &cmd, lxc);
	assertEq(cmd.type, OPERATION, "optype", ASTX_LOCATION);
	assertEq(cmd.sat(0), "while ;", ASTX_LOCATION);

	detect_instruction_type("while (", &cmd, lxc);
	assertEq(cmd.type, OPERATION, "optype", ASTX_LOCATION);
	assertEq(cmd.sat(0), "while (", ASTX_LOCATION);

	detect_instruction_type("while{", &cmd, lxc);
	assertEq(cmd.type, OPERATION, "optype", ASTX_LOCATION);
	assertEq(cmd.sat(0), "while{", ASTX_LOCATION);


	detect_instruction_type("while ", &cmd, lxc);
	assertEq(cmd.type, OPERATION, "optype", ASTX_LOCATION);
	assertEq(cmd.sat(0), "while", ASTX_LOCATION);

	detect_instruction_type("for(", &cmd, lxc);
	assertEq(cmd.type, OPERATION, "optype", ASTX_LOCATION);
	assertEq(cmd.sat(0), "for(", ASTX_LOCATION);

	detect_instruction_type("for (){", &cmd, lxc);
	assertEq(cmd.type, FOR_LOOP, "optype", ASTX_LOCATION);
	assertEq(cmd.sat(0), "", ASTX_LOCATION);


	detect_instruction_type("switch(", &cmd, lxc);
	assertEq(cmd.type, OPERATION, "optype", ASTX_LOCATION);
	assertEq(cmd.sat(0), "switch(", ASTX_LOCATION);


	detect_instruction_type("do {doblock} while(whileblock)", &cmd, lxc);
	assertEq(cmd.type, DO_WHILE, "optype", ASTX_LOCATION);
	assertEq(cmd.sat(0), "whileblock", ASTX_LOCATION);
	assertEq(cmd.bat(0), "doblock", ASTX_LOCATION);


	detect_instruction_type("do {do{doblock}while(while)} while(while)", &cmd, lxc);
	assertEq(cmd.type, DO_WHILE, "optype", ASTX_LOCATION);
	assertEq(cmd.sat(0), "while", ASTX_LOCATION);
	assertEq(cmd.bat(0), "do{doblock}while(while)", ASTX_LOCATION);


	detect_instruction_type("break hhhhhhh ignore", &cmd, lxc);
	assertEq(cmd.type, BREAK_CTX, "optype", ASTX_LOCATION);

	detect_instruction_type("while(while(){}){{}()while}", &cmd, lxc);
	assertEq(cmd.type, WHILE_LOOP, "optype", ASTX_LOCATION);
	assertEq(cmd.sat(0), "while(){}", ASTX_LOCATION);
	assertEq(cmd.bat(0), "{}()while", ASTX_LOCATION);



	detect_instruction_type("while(()){try}while(yyy)do{4}do{5}while(xxx)", &cmd, lxc);
	assertEq(cmd.type, WHILE_LOOP, "optype", ASTX_LOCATION);
	assertEq(cmd.sat(0), "()", ASTX_LOCATION);
	assertEq(cmd.bat(0), "try", ASTX_LOCATION);
	assertEq(cmd.next->type, WHILE_LOOP, "optype", ASTX_LOCATION);
	assertEq(cmd.next->sat(0), "yyy", ASTX_LOCATION);
	assertEq(cmd.next->bat(0), "4", ASTX_LOCATION);
	assertEq(cmd.next->next->type, DO_WHILE, "optype", ASTX_LOCATION);
	assertEq(cmd.next->next->sat(0), "xxx", ASTX_LOCATION);
	assertEq(cmd.next->next->bat(0), "5", ASTX_LOCATION);








	vs = split_by_separator("some text; func(){}\"\";''\"\"", lxc);
	assertEq(vs.at(0), "some text");
	assertEq(vs.at(1), "func(){}\"\"");
	assertEq(vs.at(2), "''\"\"");
	vs = split_by_separator("unu(){;};doi(;);\"trei\"(;){;}\';\'\";\";patru;{a:[;],b:{;}};{}", lxc);

	assertEq(vs.at(0), "unu(){;}");
	assertEq(vs.at(1), "doi(;)");
	assertEq(vs.at(2), "\"trei\"(;){;}\';\'\";\"");
	assertEq(vs.at(3), "patru");
	assertEq(vs.at(4), "{a:[;],b:{;}}");
	assertEq(vs.at(5), "{}");

	vs = split_by_separator("end", lxc);
	assertEq(vs.at(0), "end");

	vs = split_by_separator("one;\\\";\"\\\";\"", lxc);
	assertEq(vs.at(0), "one");
	assertEq(vs.at(1), "\\\"");
	assertEq(vs.at(2), "\"\\\";\"");

	s = "try(";


	assertTrue(!string_starts_with(s, "try", lxc.l_bracket, lxc), "string_starts_with test", ASTX_LOCATION);
	s = "try{";
	assertTrue(string_starts_with(s, "try",  lxc.l_bracket, lxc), "string_starts_with", ASTX_LOCATION);
	s = "var a";
	assertTrue(string_starts_with(s, "var", '(', lxc), "string_starts_with", ASTX_LOCATION);
	s = "vara";
	assertTrue(!string_starts_with(s, "var", ' ', lxc), "string_starts_with", ASTX_LOCATION);
	s = "var ";
	assertTrue(string_starts_with(s, "var", ' ', lxc), "[var ] starts with [var]", ASTX_LOCATION);

int type = 0;



assertTrue(util::string_is_numeric("123.456"), "util::string_is_numeric", ASTX_LOCATION);
assertTrue(!util::string_is_numeric("123,456"), "util::string_is_numeric", ASTX_LOCATION);
assertTrue(util::string_is_numeric("163,456", ','), "util::string_is_numeric", ASTX_LOCATION);
assertTrue(!util::string_is_numeric("16s3,456", ','), "util::string_is_numeric", ASTX_LOCATION);


int zero = 0, buff = 0;
assertEq(get_member("true", ctx, lxc), Boolean::bTrue(lxc), "", ASTX_LOCATION);
assertEq(get_member("false", ctx, lxc), Boolean::bFalse(lxc), "", ASTX_LOCATION);
assertEq(get_member("undefined", ctx, lxc), Value::npos, "", ASTX_LOCATION);
assertEq(get_member("\"", ctx, lxc), "\"");
assertEq(get_member("1", ctx, lxc), 1.00);
assertEq(get_member("25", ctx, lxc), 25.00);
assertEq(get_member("25.5", ctx, lxc), 25.5);
assertEq(get_member("-25.5", ctx, lxc), -25.5, "", ASTX_LOCATION);
assertEq(get_member("+15.25", ctx, lxc), 15.25);
assertEq(get_member("25,5", ctx, lxc), Value::npos);
//assertEq(get_member("{...}", ctx, lxc), "25,5");

Object * obj = Object::New(lxc);
obj->addKey("childStr", String::New("string-type", Syntax::JS));
obj->addKey("childInt", Number::New(250, lxc));
obj->addKey("childBool", Boolean::bTrue(lxc));

Object * childObj = Object::New(lxc);
Number * a = Number::New(1, lxc);
String * b = String::New("str", Syntax::JS);
//assertEq(a, Number::New(1));//TODO
childObj->addKey("a", a);
childObj->addKey("b", b);

obj->addKey("childObj", childObj);


assertEq(obj->rget("this", lxc, Function::npos, RETURN_VAL), obj, "", ASTX_LOCATION);
assertEq(obj->rget("childObj.a", lxc, Function::npos, RETURN_VAL), a, "", ASTX_LOCATION);
assertEq(obj->rget("this.childObj.a", lxc, Function::npos, RETURN_VAL), a, "", ASTX_LOCATION);
assertEq(obj->rget("this.childObj.b", lxc, Function::npos, RETURN_VAL), b, "", ASTX_LOCATION);
assertEq(get_expression_between("((a)(b))()", '(', ')', buff, lxc), "(a)(b)", "", ASTX_LOCATION);
assertEq(buff, 8, "get_expression_between last position", ASTX_LOCATION);

assertEq(get_expression_between("(print)(\"a\");", '(', ')', buff, lxc), "print", "", ASTX_LOCATION);
Value * t2 = ctx->rget("print(\"a\");", lxc, ctx, RETURN_VAL);
printf("rzlt = %s\n", blurt(t2).c_str());

assertEq(t2, "a", "............", ASTX_LOCATION);
assertEq(ctx->rget("(print)(\"a\");", lxc, ctx, RETURN_VAL), "a", "", ASTX_LOCATION);
assertEq(ctx->rget("(print)(print);", lxc, ctx, RETURN_VAL), "function (){[native function]}", "", ASTX_LOCATION);
assertEq(ctx->rget("new print;", lxc, ctx, RETURN_VAL), Value::npos, "", ASTX_LOCATION);
assertEq(ctx->rget("new print(\"test\");", lxc, ctx, RETURN_VAL), String::New("test", Syntax::JS), "", ASTX_LOCATION);

assertEq(first_operation_index("+1", lxc), 1, "first_operation_index", ASTX_LOCATION);
assertEq(first_operation_index("++1", lxc), 2, "first_operation_index", ASTX_LOCATION);
assertEq(first_operation_index("--1", lxc), 2, "first_operation_index", ASTX_LOCATION);
assertEq(first_operation_index("-1", lxc), 1, "first_operation_index", ASTX_LOCATION);
assertEq(first_operation_index("1", lxc), 0, "first_operation_index", ASTX_LOCATION);
assertEq(first_operation_index("-", lxc), 0, "first_operation_index", ASTX_LOCATION);
assertEq(first_operation_index("++", lxc), 1, "first_operation_index", ASTX_LOCATION);


operation_node node;

extract_operation_nodes("1+-2", lxc, &node, ctx);
assertEq(node.operation, PLUS, "extract_operation_nodes 1+-2");
assertEq(node.value, 1);
assertEq(node.next->operation, NO_OPERATION, "extract_operation_nodes 1+-2");
assertEq(node.next->value, -2);
assertEq(node.next->previous->value, 1);
extract_operation_nodes("1-4*3", lxc, &node, ctx);
assertEq(node.operation, MINUS, "extract_operation_nodes 1-2*3");
assertEq(node.value, 1);
assertTrue(node.previous == NULL, "", ASTX_LOCATION);
assertEq(node.next->value, 4);
assertEq(node.next->previous->value, 1);
assertEq(execute_operation("1+3*2*2+2*3-15/5<<6<=7>>8", lxc, ctx), "false");


assertEq(execute_operation("1 + 2 * 3 * 2 + 4", lxc, ctx), 17, ASTX_LOCATION);
extract_operation_nodes("1 + +2", lxc, &node, ctx);
var_dump(&node, true);
assertEq(node.operation, PLUS, "extract_operation_nodes 1 + +2");

assertEq(execute_operation("5", lxc, ctx), 5);
assertEq(execute_operation("1+1", lxc, ctx), 2, ASTX_LOCATION);
assertEq(execute_operation("1+1+3+4+8+1", lxc, ctx), 18, ASTX_LOCATION);
assertEq(execute_operation("1 * 2 * 3 * 2 + 4", lxc, ctx), 16, ASTX_LOCATION);


extract_operation_nodes("4 / 2", lxc, &node, ctx);
assertEq(node.operation, DIVIDE, "extract_operation_nodes 4 / 2");
assertEq(node.next->operation, NO_OPERATION, "extract_operation_nodes 4 / 2", ASTX_LOCATION);

assertEq(execute_operation("4*2", lxc, ctx), 8, ASTX_LOCATION);
assertEq(execute_operation("4 / 2", lxc, ctx), 2, ASTX_LOCATION);
assertEq(execute_operation("2-5+5*4/4", lxc, ctx), 2, ASTX_LOCATION);
assertEq(execute_operation("8*9/3-3+5*2+40", lxc, ctx), 71, ASTX_LOCATION);
assertEq(execute_operation("8*9/3-3+5*2+40-8*9/3-3+5*2+40", lxc, ctx), 94, ASTX_LOCATION);
assertEq(execute_operation("8*9/3-3+5*2+40-8*9/3-3+5*2+40*8*9/3-3+5*2+40-8*9/3-3+5*2+40", lxc, ctx), 1084, ASTX_LOCATION);
assertEq(execute_operation("a=2", lxc, ctx), 2, ASTX_LOCATION);
assertEq(execute_operation("a=b=c=d=e=2", lxc, ctx), 2, ASTX_LOCATION);
assertEq(ctx->rget("a", lxc, ctx, RETURN_VAL), 2);
assertEq(ctx->rget("b", lxc, ctx, RETURN_VAL), 2);
assertEq(ctx->rget("c", lxc, ctx, RETURN_VAL), 2);
ASTX_BLURT_THIS_FUNC;
vs = pair_by_colon("a:1", lxc);
assertEq("a", vs.at(0), "vs.at(0)=a", ASTX_LOCATION);
assertEq("1", vs.at(1),  "vs.at(1)=1", ASTX_LOCATION);
vs = pair_by_colon("ssss:2:2", lxc);
assertEq("ssss", vs.at(0), "vs.at(0)=ssss", ASTX_LOCATION);
assertEq("2:2", vs.at(1),  "vs.at(1)=2:2", ASTX_LOCATION);

try {
	pair_by_colon("ssss", lxc);
} catch(Error &ex){
	assertFail(ex, "invalid pair input", ASTX_LOCATION);
}
execute_operation("object={a:1, b:2, c: false, d: 1+5}", lxc, ctx);
//printf("%s\n", ctx->rget("object", lxc, ctx, RETURN_VAL)->toString().c_str());



assertEq(ctx->rget("object", lxc, ctx, RETURN_VAL)->rget("b", lxc, ctx, RETURN_VAL), 2);
assertEq(ctx->rget("object", lxc, ctx, RETURN_VAL)->rget("d", lxc, ctx, RETURN_VAL), 6);
execute_operation("array=[1, 2, true, \"string\"]", lxc, ctx);
assertEq(ctx->rget("array[0]", lxc, ctx, RETURN_VAL), 1);
assertEq(ctx->rget("array[1]", lxc, ctx, RETURN_VAL), 2);
assertEq(ctx->rget("array[2]", lxc, ctx, RETURN_VAL), "true");
assertEq(ctx->rget("this[\"print\"](\"b\");", lxc, ctx, RETURN_VAL), String::New('b', Syntax::JS), "WTF", ASTX_LOCATION);
execute_operation("object.c = function(a, b, c){ print(a + b + c); }", lxc, ctx);
assertEq(execute_operation("object.c(1, 6, 3)", lxc, ctx), 10, ASTX_LOCATION);
vs = split_by_comma("a, b, c", lxc);
s = "a";
assertEq(vs.at(0), s);
s = "b";
assertEq(vs.at(1), s);
s = "c";
assertEq(vs.at(2), s);
assertEq(execute_operation("object.c(3, 4, 8)", lxc, ctx), 15, ASTX_LOCATION);
assertEq(eval("9", lxc, ctx), 9);
assertEq(execute_operation("4 > 3 + 4", lxc, ctx), "false");
assertEq(execute_operation("4 < 3 + 4", lxc, ctx), "true");
assertEq(execute_operation("1 > 2", lxc, ctx), "false", "1 > 2 false",  ASTX_LOCATION);
assertEq(execute_operation("1 < 2", lxc, ctx), "true");
assertEq(execute_operation("4 < 7 < 8", lxc, ctx), "true",
		"(4 < 3 + 4 < 8)", ASTX_LOCATION);
assertEq(execute_operation("4 < 7 < 8", lxc, ctx), "true", "", ASTX_LOCATION);
assertEq(execute_operation("true > false", lxc, ctx), "true", "", ASTX_LOCATION);
assertEq(execute_operation("true < false", lxc, ctx), "false", "", ASTX_LOCATION);
assertEq(eval("!true", lxc, ctx), "false", "", ASTX_LOCATION);
assertEq(eval("!!true", lxc, ctx), "true", "", ASTX_LOCATION);
assertEq(eval("!!!!true", lxc, ctx), "true", "", ASTX_LOCATION);
assertEq(eval("6 / 2 * (2 + 1)", lxc, ctx), 9, "", ASTX_LOCATION);
assertEq(eval("6 / 2 * 2 + 1", lxc, ctx), 7, "", ASTX_LOCATION);
assertEq(eval("6 / (2 * 2)+ 1", lxc, ctx), 2.5, "", ASTX_LOCATION);
assertEq(eval("(((print)))(print);", lxc, ctx), "function (){[native function]}", "", ASTX_LOCATION);
assertEq(eval("(1++) + (++1)", lxc, ctx), 4, "", ASTX_LOCATION);
assertEq(eval("1++ + ++1", lxc, ctx), 4, "", ASTX_LOCATION);
assertEq(get_member("++1", ctx, lxc, NULL), 2, "++1", ASTX_LOCATION);
assertEq(get_member("1++", ctx, lxc, NULL), 2, "there is no difference between 1++ and ++1", ASTX_LOCATION);
assertEq(get_member("2--", ctx, lxc, NULL), 1, "2--", ASTX_LOCATION);
assertEq(get_member("--2", ctx, lxc, NULL), 1, "there is no difference between 2-- and --2", ASTX_LOCATION);
assertEq(get_member("1--", ctx, lxc, NULL), zero, "1--", ASTX_LOCATION);
assertEq(eval("1++ + \"string\"", lxc, ctx), "2string", "concatenation", ASTX_LOCATION);
assertEq(execute_operation("1 <= 2", lxc, ctx), "true");
assertEq(execute_operation("2 <= 2", lxc, ctx), "true");
assertEq(execute_operation("2 =< 2", lxc, ctx), "true");
assertEq(execute_operation("2 <= 1", lxc, ctx), "false");
assertEq(execute_operation("2 =< 1", lxc, ctx), "false");
assertEq(execute_operation("2 <= 1+5", lxc, ctx), "true");
assertEq(execute_operation("98.2 >= 45.3", lxc, ctx), "true");
assertEq(execute_operation("65 % 38 * 22 / 21 + 23 % 2", lxc, ctx), 29.285714285714285);
assertEq(execute_operation("2 << 2", lxc, ctx), 8);
assertEq(execute_operation("2 << 2 << 2", lxc, ctx), 32);
assertEq(execute_operation("2 >> 2 >> 2", lxc, ctx), zero, "", ASTX_LOCATION);
assertEq(execute_operation("2 << 1 + 1", lxc, ctx), 8, "", ASTX_LOCATION);
assertEq(execute_operation("2.344555 << 2 > 9", lxc, ctx), "false");
assertEq(execute_operation("9 > 2 << 2", lxc, ctx), "true");
assertEq(execute_operation("3 | 12", lxc, ctx), 15);
assertEq(execute_operation("3 << 2", lxc, ctx), 12);
assertEq(execute_operation("3 + 3 << 2", lxc, ctx), 24);
assertEq(execute_operation("3 | 3 << 2", lxc, ctx), 15);
assertEq(execute_operation("2 & 2 > 2", lxc, ctx), zero);
assertEq(execute_operation("2 == 2", lxc, ctx), "true", "", ASTX_LOCATION);
assertEq(execute_operation("2 === 2", lxc, ctx), "true", "", ASTX_LOCATION);
assertEq(execute_operation("1+1 === 2", lxc, ctx), "true", "", ASTX_LOCATION);
assertEq(execute_operation("1+1 === \"2\"", lxc, ctx), "false", "", ASTX_LOCATION);
assertEq(execute_operation("1+1===2==2", lxc, ctx), "false", "1+1===2==2 returns false", ASTX_LOCATION );
assertEq(execute_operation("1+1===2===2", lxc, ctx), "false", "1+1===2===2 returns false", ASTX_LOCATION );
assertEq(eval("a = 1", lxc, ctx), "1", "", ASTX_LOCATION);
assertEq(execute_operation("a += 1", lxc, ctx), 2, "", ASTX_LOCATION);
assertEq(execute_operation("a += b = 3", lxc, ctx), 5, "", ASTX_LOCATION);
assertEq(execute_operation("a -= 1", lxc, ctx), 4, "", ASTX_LOCATION);
assertEq(execute_operation("a *= 2", lxc, ctx), 8, "", ASTX_LOCATION);
assertEq(execute_operation("a /= 2", lxc, ctx), 4, "", ASTX_LOCATION);
assertEq(execute_operation("a <<= 2", lxc, ctx), 16, "", ASTX_LOCATION);
assertEq(execute_operation("a >>= 1+1", lxc, ctx), 4, "", ASTX_LOCATION);
assertEq(execute_operation("a %= 1 + 2", lxc, ctx), 1, "", ASTX_LOCATION);
assertEq(execute_operation("a |= 1 + 1", lxc, ctx), 3, "", ASTX_LOCATION);
assertEq(execute_operation("a &= 2", lxc, ctx), 2, "", ASTX_LOCATION);
assertEq(execute_operation("a == 2", lxc, ctx), "true", "", ASTX_LOCATION);
assertEq(execute_operation("a != 4", lxc, ctx), "true", "", ASTX_LOCATION);
assertEq(execute_operation("a != 2", lxc, ctx), "false", "", ASTX_LOCATION);
assertEq(execute_operation("a != \"2\"", lxc, ctx), "false", "", ASTX_LOCATION);
assertEq(execute_operation("a !== \"2\"", lxc, ctx), "true", "", ASTX_LOCATION);
assertEq(execute_operation("1 & 2", lxc, ctx), zero, "", ASTX_LOCATION);
assertEq(execute_operation("4 & 4", lxc, ctx), 4, "", ASTX_LOCATION);
assertEq(execute_operation("5 & 5 & 3", lxc, ctx), 1, "", ASTX_LOCATION);
assertEq(execute_operation("1 & 2 | 3", lxc, ctx), 3, "", ASTX_LOCATION);
assertEq(execute_operation("4 | 3 > 5 & 2", lxc, ctx), 4, "", ASTX_LOCATION);
//Truth table of XOR:
assertEq(execute_operation("0 ^ 0", lxc, ctx), zero, "", ASTX_LOCATION);
assertEq(execute_operation("0 ^ 1", lxc, ctx), 1, "", ASTX_LOCATION);
assertEq(execute_operation("1 ^ 0", lxc, ctx), 1, "", ASTX_LOCATION);
assertEq(execute_operation("1 ^ 1", lxc, ctx), zero, "", ASTX_LOCATION);

//Truth table of OR:
assertEq(execute_operation("0 | 0", lxc, ctx), zero, "", ASTX_LOCATION);
assertEq(execute_operation("0 | 1", lxc, ctx), 1, "", ASTX_LOCATION);
assertEq(execute_operation("1 | 0", lxc, ctx), 1, "", ASTX_LOCATION);
assertEq(execute_operation("1 | 1", lxc, ctx), 1, "", ASTX_LOCATION);
assertExecuteOperation("2 | 4 ^ 3", lxc, ctx, 7, ASTX_LOCATION);

assertExecuteOperation("-1", lxc, ctx, -1, ASTX_LOCATION);
assertExecuteOperation("!-1", lxc, ctx, "false", ASTX_LOCATION);
assertExecuteOperation("!-0", lxc, ctx, "true", ASTX_LOCATION);
assertExecuteOperation("!+1", lxc, ctx, "false", ASTX_LOCATION);
assertExecuteOperation("!++1", lxc, ctx, "false", ASTX_LOCATION);
assertExecuteOperation("!++1", lxc, ctx, "false", ASTX_LOCATION);
assertExecuteOperation("!--1", lxc, ctx, "true", ASTX_LOCATION);
assertExecuteOperation("!--1 + 2", lxc, ctx, 3, ASTX_LOCATION);
assertExecuteOperation("!--0 + 2", lxc, ctx, 2, ASTX_LOCATION);
assertExecuteOperation("!--(-1) + 2", lxc, ctx, 2, ASTX_LOCATION);
assertExecuteOperation("1 || 2", lxc, ctx, 1, ASTX_LOCATION);
assertExecuteOperation("1 && 2", lxc, ctx,  (1 && 2), ASTX_LOCATION);
assertExecuteOperation("1 > 2 || 2 < 1", lxc, ctx,  "false", ASTX_LOCATION);
assertExecuteOperation("2 >= 2 && 3 <= 3 || 4 <= 2", lxc, ctx,  "true", ASTX_LOCATION);
assertExecuteOperation("true || 1", lxc, ctx,  "true", ASTX_LOCATION);
assertExecuteOperation("false || 1", lxc, ctx,  "1", ASTX_LOCATION);
assertExecuteOperation("true || false", lxc, ctx,  "true", ASTX_LOCATION);
assertExecuteOperation("0 || false", lxc, ctx,  "false", ASTX_LOCATION);
assertExecuteOperation("1 && 1", lxc, ctx,  1, ASTX_LOCATION);
assertExecuteOperation("0 && 1", lxc, ctx,  zero, ASTX_LOCATION);
assertExecuteOperation("1 && 0", lxc, ctx,  zero, ASTX_LOCATION);
assertExecuteOperation("0 && 0", lxc, ctx,  zero, ASTX_LOCATION);
assertExecuteOperation("0 && true", lxc, ctx,  zero, ASTX_LOCATION);
assertExecuteOperation("0 && false", lxc, ctx,  zero, ASTX_LOCATION);
assertExecuteOperation("false && 0", lxc, ctx,  "false", ASTX_LOCATION);
assertExecuteOperation("false && 1", lxc, ctx,  "false", ASTX_LOCATION);
assertExecuteOperation("true && 0", lxc, ctx,  zero, ASTX_LOCATION);
assertExecuteOperation("true && 1", lxc, ctx,  1, ASTX_LOCATION);
assertExecuteOperation("true && false", lxc, ctx,  "false", ASTX_LOCATION);
assertExecuteOperation("false && false", lxc, ctx,  "false", ASTX_LOCATION);
assertExecuteOperation("true && true", lxc, ctx,  "true", ASTX_LOCATION);
assertExecuteOperation("!-1", lxc, ctx,  "false", ASTX_LOCATION);
assertExecuteOperation("!0", lxc, ctx,  "true", ASTX_LOCATION);
assertExecuteOperation("~1", lxc, ctx, -2, ASTX_LOCATION);
assertExecuteOperation("~4", lxc, ctx, -5, ASTX_LOCATION);
assertExecuteOperation("~-4", lxc, ctx, 3, ASTX_LOCATION);
assertExecuteOperation("~0", lxc, ctx, -1, ASTX_LOCATION);
assertExecuteOperation("~1", lxc, ctx, -2, ASTX_LOCATION);
assertExecuteOperation("1 ^ 3", lxc, ctx, 2, ASTX_LOCATION);
assertExecuteOperation("1 ^ ~3", lxc, ctx, -3, ASTX_LOCATION);
assertEq(eval("(1++) + (++1)", lxc, ctx), 4, "(1++) + (++1) = 4", ASTX_LOCATION);
assertExecuteOperation("1 ? 2 : 3", lxc, ctx, 2, ASTX_LOCATION);
assertExecuteOperation("false ? 2 : 3", lxc, ctx, 3, ASTX_LOCATION);
assertExecuteOperation("false ? 2 : 1 ? 2 : 4", lxc, ctx, 2, ASTX_LOCATION);
assertExecuteOperation("1 * 7 + 3 > 8", lxc, ctx, "true", ASTX_LOCATION);
assertExecuteOperation("true ? 3 : 10 > 8 ? 9 : -3", lxc, ctx, 3, ASTX_LOCATION);
assertExecuteOperation("false ? 3 : true ? 9 : -3", lxc, ctx, 9, ASTX_LOCATION);
assertExecuteOperation("false ? 3 : 10 > 8 ? 9 : -3", lxc, ctx, 9, ASTX_LOCATION);
assertExecuteOperation("1 > 5 ? 3 : 1 * 7 + 3 > 8 ? 9 : -3", lxc, ctx, 9, ASTX_LOCATION);
assertExecuteOperation("1 > 2 + 3 ? 3 : 1 * (4+3) + 3 > (16/2) ? 8++ : -3", lxc, ctx, 9, ASTX_LOCATION);
s = "unu type";
assertTrue(string_ends_with(s, "type", lxc) == true, "", ASTX_LOCATION);
s = "unutype";
assertTrue(string_ends_with(s, "type", lxc) == false, "", ASTX_LOCATION);
s = "type";
assertTrue(string_ends_with(s, "type", lxc) == false, "", ASTX_LOCATION);
s = "ype";
assertTrue(string_ends_with(s, "type", lxc) == false, "", ASTX_LOCATION);
s = "1 typeof";
assertTrue(string_ends_with(s, "typeof", lxc) == true, "", ASTX_LOCATION);
assertExecuteOperation("typeof 1", lxc, ctx, "number", ASTX_LOCATION);
assertExecuteOperation("typeof 1 + 1", lxc, ctx, "number1", ASTX_LOCATION);
assertExecuteOperation("~-5 + !1", lxc, ctx, 4, ASTX_LOCATION);
assertExecuteOperation("1 instanceof 1", lxc, ctx, "true", ASTX_LOCATION);
assertExecuteOperation("1 instanceof \"1\"", lxc, ctx, "false", ASTX_LOCATION);
//assertExecuteOperation("[] instanceof Array", lxc, ctx, "true", ASTX_LOCATION);
//assertExecuteOperation("{} instanceof Object + 1", lxc, ctx, 2, ASTX_LOCATION);
try {
	exec_script_file("../tests/fnfound-pass.js", lxc, ctx);
} catch (Error &e) {
	assertFail(e, "script path ", ASTX_LOCATION);
}
assertExecuteOperation("1 >>> 2", lxc, ctx, zero, ASTX_LOCATION);
assertExecuteOperation("4 >>>= 2", lxc, ctx, 1, ASTX_LOCATION);
assertExecuteOperation("4 >>> 2 + 2 - 1 * 5", lxc, ctx, zero, ASTX_LOCATION);
s = "expect_stdout(1 + 2 * 3 / (2 + 1 - 1), 4)";
assertEq(get_expression_between(s, lxc.l_parenthesis, lxc.r_parenthesis, type, lxc), "1 + 2 * 3 / (2 + 1 - 1), 4", "", ASTX_LOCATION);
eval("expect_stdout( 1 + 2 * 3 / (2 + 1 - 1), 4)", lxc, ctx);

assertEq(eval("1 + 1; return 48; return 2;", lxc, ctx), 48, "", ASTX_LOCATION);
assertEq(eval("1 + 1; 2; 3", lxc, ctx), "[2,2,3]", "", ASTX_LOCATION);
assertEq(eval("1 + 1; 2; break; 3", lxc, ctx), "[2,2]", "", ASTX_LOCATION);
assertEq(eval("1 + 1; 2; continue; 3; 4", lxc, ctx), "[2,2]", "", ASTX_LOCATION);
vs = split_by_separator("var i = 0; i < 10; i++", lxc);
assertEq(vs.at(0), "var i = 0");
assertEq(vs.at(1), "i < 10");
assertEq(vs.at(2), "i++");

//???
//assertEq(eval("for(var i = 0; i < 10; i++) { print(i); } print(11);", lxc, ctx), "[9,11]", "", ASTX_LOCATION);
assertEq(eval("if(4 > 2) {print(5);}", lxc, ctx), 5, "", ASTX_LOCATION);
assertEq(eval(" if(2 > 4) {print(5);} else {print(8); }", lxc, ctx), 8, "", ASTX_LOCATION);

//this is the hardest:
s = "var a = []; for(var i = 0; i < 10; i++){ switch(i) { case 1: a.push(i); case 2: a.push(3); case 4: a.push(4); break; case 5: a.push(5); case 5: a.push(6); default: a.push(i) }};";
eval(s, lxc, ctx);
assertEq(ctx->rget("a", lxc, ctx, RETURN_VAL), "[0,1,3,4,3,4,3,4,5,6,5,6,7,8,9]", "", ASTX_LOCATION);

s = "var a = []; switch(0) {  default: a.push(0); case 1: a.push(0); case 2: a.push(3); case 4: a.push(4); break; case 5: a.push(5); case 5: a.push(6); }";
eval(s, lxc, ctx);
assertEq(ctx->rget("a", lxc, ctx, RETURN_VAL), "[0,0,3,4]", "", ASTX_LOCATION);

eval("function test(){ return arguments;}", lxc, ctx);
assertEq(eval("test(1, 2, 3)", lxc, ctx), "[1,2,3]", "", ASTX_LOCATION);




try {
	exec_script_file("tests/commands.js", lxc, ctx);
}catch (Error &e) {
	printf("exec_script_file failed with %s", e.what());
	system("pause");
}




	//exec_script_file("tests/operations.js", lxc, ctx);

	//






    return 0;
}
