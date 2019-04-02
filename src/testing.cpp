#include "astox.h"


namespace astox {
	namespace testing {
	using namespace std;

	ExpectStdout::ExpectStdout(): Function("", "", NULL, NULL, NULL, NULL, Syntax::JS, ASTOX_VALUE_CUSTOM){
	};

	static int testCount = 0;

	Value * ExpectStdout::call(std::string &args,
			const scriptengine::lexic &lexic,
			Function *&context){
		vector<string> parts = scriptengine::split_by_comma(args, lexic);
		Value * left = scriptengine::execute_operation(parts.at(0), lexic, context);
		if(!(left->str() == parts.at(1))){
			test_result_fail("FAILED!!! [%s] != [%s] expected [%s] \n",  parts.at(0).c_str(), left->str().c_str(), parts.at(1).c_str());
		} else {
			test_result_success("PASSED [%s]==[%s]\n", parts.at(0).c_str(), left->str().c_str());
		}
		return Boolean::New(left->str() == parts.at(1), _lx);
	};





		void assertEq(std::string in, std::string ex, std::string msg, std::string loc){
			msg.append(in);
			msg.append("====");
			msg.append(ex);

			assertTrue(in == ex, msg, loc);
		};

		void assertEq(std::string in, std::string ex, std::string loc){
			std::string msg;
			msg.append(in);
			msg.append("==");
			msg.append(ex);
			assertTrue(in == ex, msg, loc);
		};

		void assertEq(std::string in, std::string ex) {
			std::string msg;
			msg.append(in);
			msg.append("==");
			msg.append(ex);
			assertEq(in, ex, msg, ASTX_LOCATION);
		};

		void assertEq(Value * in, Value * ex, std::string msg, std::string loc){
			assertTrue(in == ex, "verify if value[" + blurt(in) + "] eq value [" + blurt(ex) + "]", loc);
		}

		void assertEq(Value * vl, std::string ex,  std::string msg, std::string loc){
			string s(vl->str());
			assertTrue(vl->str() == ex, "verify if value::str <" + s + "> --- eq <" + ex + "> at value " + blurt(vl) + msg, loc);
		};

		void assertEq(String * vl, std::string ex,  std::string msg, std::string loc){
			printf("%s", vl->strtype());
			assertTrue(vl->str() == ex, "verify if value::str " + vl->str() + " --- eq " + ex + " "+ msg, loc);
		};

		void assertIsNull(Value * value, string location){
			assertTrue(value == NULL, "assert is null", location);
		};

		void assertRegExpFlags(RegExp * regexp, stx_regex_flag regexFlags, string location){
			if(regexp == NULL){
				test_result_fail(" regex null")
			} else {
				std::stringstream ss(stringstream::in | stringstream::out);
				ss << regexp->getFlags() << " == " << regexFlags;
				assertTrue(regexp->getFlags() == regexFlags, ss.str(), location);
			}
		}

		void assertTrue(bool expect, std::string message, std::string location) {
			testCount++;
			if(expect){
				test_result_success("[%-23s|% 5d|TEST] ok [%s]\n", location.c_str(),testCount,
						message.c_str());
			} else {
				test_result_fail("FAILED [%s] at [%s]\n",
										message.c_str(),
										location.c_str());

			}
		};

		void assertFail(Error &e, stxtr expect, stxtr loc){
			testCount++;
			if(e.str().find(expect)!= std::string::npos){
				test_result_success("TST%i] success [%s] error message found\n", testCount, expect.c_str());
			} else {
				test_result_fail("[%s] expected message [%s] but retrieved\n%s [located at %s]\n",
						loc.c_str(), expect.c_str(), e.str().c_str(), ASTX_LOCATION);
			}
		};

		void assertEq(RegExpMatch match, std::string part, int start, int end, std::string loc){
			if(match.part != part){
				test_result_fail("RegExpMatch test failure %s != %s at %s\n", match.part.c_str(), part.c_str(), loc.c_str());
			}
			else if(match.start != start){
				test_result_fail("RegExpMatch test failure start index %i != %i at %s\n", (int)match.start, start, loc.c_str());
			}
			else if(match.end != end){
				test_result_fail("RegExpMatch test failure end index %i != %i at %s\n", (int)match.end, end, loc.c_str());
			}
			else {
				test_result_success("RegExpMatch[%s %i %i] success  %s\n",
						match.part.c_str(), (int)match.start, (int)match.end, loc.c_str());
			}
		};

		void assertEq(double in, double ex, std::string msg, std::string loc) {
			ostringstream c;
			c << " with (" << in << " == " << ex << ")";

			msg.append( c.str());
			assertTrue( (in == ex), msg , loc);
		};


		void assertContains(stxtr in, std::string ex,  std::string location){
			bool r =(in.find(ex)!= std::string::npos);
			assertTrue( r, in + "contains" + ex, location);
		};

		void assertContains(Value * v, std::string ex,  std::string l){
			if(v == NULL){
				test_result_fail("input value is null");
			} else {
				assertContains(v->str(), ex, l);
			}
		};

		void assertEq(Value * in, double ex, std::string msg, std::string loc){
			ostringstream c;
			c << in->dbl() << " == " << ex;
			msg.append(" [");
			msg.append(c.str());
			msg.append("]");
			assertEq(in->dbl(), ex, msg, loc);
		}

		void assertExecuteOperation(std::string input, const scriptengine::lexic &lxc,
						Function *& ctx, double xpct, std::string loc){
			assertEq(execute_operation(input, lxc, ctx), xpct, "execute_operation " + input, loc);
		};

		void assertExecuteOperation(std::string input, const scriptengine::lexic &lxc,
								Function *& ctx, std::string xpct, std::string loc){
			assertEq(execute_operation(input, lxc, ctx), xpct, "execute_operation " + input, loc);
		};







		













































//		void run_suite_operators() {
//			printf("The following test suite may throw only compilation errors\n");
//
//			using namespace std;
//			Object fromStr = "{a:1, b:2, c:false, str:string, d : undefined long string}";
//			cout << "cout << Object << " << fromStr << " << endl;" << endl;
////			assertEq("{\"a\":1,\"b\":2,\"c\":false,\"d\":\"undefined long string\",\"str\":\"string\"}",
////				astox::lang::toJSON(&fromStr, "", ""), ASTX_LOCATION, "");
//
//			Array fromStrArr = "[1, 2, string long with spaces, false]";
//			cout << "cout << Array << " << fromStrArr << " << endl;" << endl;
//
//			Value * p_value = new Value();
//			cout << "p_value: " << p_value << endl;
//
//			Value r_value;
//			cout << "r_value: " << r_value << endl;
//
//			String * p_string = new String("dummy");
//			cout << "p_string: " << p_string << endl;
//
//			String r_string("reference");
//			cout << "r_string: " << r_string << endl;
//
//			Number * p_number = Number::New("27112004");
//			cout << "p_number: " << p_number << endl;
//
//
//			Number r_number(-455.5);
//			cout << "r_number: " << r_number << endl;
//			r_number++;
//			cout << "r_number++ " << r_number << endl;
//			++r_number;
//			cout << "++r_number " << r_number << endl;
//			r_number--;
//			cout << "r_number-- " << r_number << endl;
//			r_number--;
//			cout << "r_number-- " << r_number << endl;
//			r_number--;
//			cout << "r_number-- " << r_number << endl;
//			--r_number;
//			cout << "--r_number: " << r_number << endl;
//			--r_number;
//			cout << "--r_number: " << r_number << endl;
//			--r_number;
//			cout << "--r_number: " << r_number << endl;
//
//
//
//			Number data = 2;
//			cout << "Number data = " << data << endl;
//			data + 2;
//			cout << "      data+2: " << data << endl;
//			data - 1;
//			cout << "      data-1: " << data << endl;
//			data * 2;
//			cout << "      data*2: " << data << endl;
//			data / 2;
//			cout << "      data/2: " << data << endl;
//			cout << "      data+r_number " << data + r_number << endl;
//			data += 2;
//			cout << "      data+=2; " << data << endl;
//			data -= 2;
//			cout << "      data-=2 " << data << endl;
//			data *= 2;
//			cout << "      data*=2 " << data << endl;
//			data /= 2;
//			cout << "      data/=2 " << data << endl;
//			cout << "      data!=r_number " << (data != r_number) << endl;
//			cout << "      data==r_number " << (data == r_number) << endl;
//			cout << "      data<=r_number " << (data <= r_number) << endl;
//			cout << "      data>=r_number " << (data >= r_number) << endl;
//
//			Number dumnr = 3;
//			cout << "Number dumnr = " << data << endl;
//			cout << "      data!=dumnr; " << (data != dumnr) << endl;
//			cout << "      data==dumnr; " << (data == dumnr) << endl;
//
//			cout << "      data>dumnr; " << (data > dumnr) << endl;
//			cout << "      data<dumnr; " << (data < dumnr) << endl;
//
//			Number dataNegative = -23344.455;
//			cout << "Number dataNegative = " << dataNegative << endl;
//
//			double vald = 3455;
//			cout << "double vald = 3455;" << endl;
//			Number dVal = vald;
//			cout << "Number dVal = vald;" << dVal << endl;
//
//			cout << "dumnr+dval " << dumnr + dVal << endl;
//			cout << "dumnr-dval " << dumnr - dVal << endl;
//
//			cout << "dumnr*dval " << dumnr * dVal << endl;
//			cout << "dumnr/dval " << dumnr / dVal << endl;
//
//			Boolean * p_bool = new Boolean(true);
//			cout << "p_bool: " << p_bool << endl;
//
//			Boolean * p_bool_false = new Boolean(false);
//			cout << "p_bool_false: " << p_bool_false << endl;
//			cout << "Boolean::btrue = " << Boolean::BTRUE << endl;
//			cout << "Boolean::bfalse = " << Boolean::BFALSE << endl;
//
//			Boolean r_bool(false);
//			cout << "r_bool: " << r_bool << endl;
//
//			Array * p_arr = new Array();
//			cout << "p_arr: " << p_arr << endl;
//
//			Array r_arr;
//			cout << "r_arr: " << r_arr << endl;
//
//			Object * p_object = new Object();
//			p_object->addKey("number", p_number);
//			p_object->addKey("array", p_arr);
//			cout << "p_object: " << p_object << endl;
//
//			Object r_obj;
//			r_obj.addKey("string", &r_string);
//			r_obj.addKey("inheritObject", p_object);
//			cout << "r_object: " << r_obj << endl;
//
//
//#ifdef ASTX_ENABLE_QT
//			cout << "QString supported" << endl;
//			QString qstring1 = "qstring1";
//			String qstr1(qstring1);
//			cout << "qstr1: " << qstr1 << endl;
//
//			QString * qstring2 = new QString("asdasdas");
//			String * qstr2 = new String(qstring2);
//			cout << "qstr2: " << qstr2 << endl;
//#endif
//
//
//			String strOperators = "abcdefghijklmnopqrstuvxyz";
//			cout << "strOperators: " << strOperators << endl;
//////			strOperators++;
////			cout << "strOperators++: " << strOperators << endl;
//////			strOperators++;
////			cout << "strOperators++: " << strOperators << endl;
////			for (int i = 0; i < 5; i++) {
////				strOperators--;
////				cout << "strOperators--: " << strOperators << endl;
////			}
//
//			strOperators.reverse();
//
//			cout << "strOperators.reverse();  " << strOperators << endl;
//
//			for (int i = 0; i < 5; i++) {
////				strOperators--;
//				cout << "strOperators--: " << strOperators << endl;
//			}
//
//
//			strOperators.reverse();
//
//			//strOperators+='_';
////			strOperators += "|endOfString";
//
//			std::string f = "std::string";
////			strOperators += f;
//
//			String temp = ".....<><><><>";
//			cout << "String temp = \".....<><><><>\";" << endl;
//			Number nums(20);
//			cout << "Number nums(20);" << endl;
//			String n2(456.455);
//			cout << "String n2(456.455);" << endl;
////			n2 += nums;
//			cout << "n2+=nums;" << endl;
////			strOperators += nums;
//			cout << "strOperators+= nums; strOperators+= n2 => " << strOperators << endl;
////			strOperators += n2;
//			cout << "strOperators+temp = " << strOperators + temp << endl;
//		};


		void run_suite_logical() {

		}



		Value * assertPrint(Value * arguments) {
//			std::string s = arguments->toString();
//			assertEq(s,
//				TestScriptSuite::getInstance()->getExpect(),
//				TestScriptSuite::getInstance()->getMessage(),
//				TestScriptSuite::getInstance()->getId()
//			);
			return Value::npos;
		}


		void run_suite_script() {
//			TestScriptSuite * t = TestScriptSuite::getInstance();
//			int zero = 0;
//
//			t->run(ASTX_LOCATION, "var a = false; print(!!a+\"{}\"+!a);", "false{}true", "scripting")
//				->run(ASTX_LOCATION, "f = function(data, one, two){ print(data); }; f('name');", "name")
//				->run(ASTX_LOCATION, "function test(){}; b = test(16); print(b);", "undefined")
//				->run(ASTX_LOCATION, "c=23; print(c);", "23")
//				->run(ASTX_LOCATION, "print(test);", "function (){}")
//				->run(ASTX_LOCATION, "print(new test);", "{}")
//				->runDbl(ASTX_LOCATION, "2", 2, "numeric operands")
//
//				->runDbl(ASTX_LOCATION, "", zero)
//				->runDbl(ASTX_LOCATION, "8&&2&&4", (8 && 2 && 4))
//
//				->runDbl(ASTX_LOCATION, "89 << 2", 356)
//				->runDbl(ASTX_LOCATION, "a = b = c = number = d = f = 5", 5, "alloc")
//				->runBool(ASTX_LOCATION, "f = 5 == 5", true, "booleans")
//				->runBool(ASTX_LOCATION, "g=\"5\"!=5", false)
//				->runBool(ASTX_LOCATION, "h=\"5\"==5", true)
//				->runBool(ASTX_LOCATION, "i=\"5\"!==5", false)
//				->runBool(ASTX_LOCATION, "j=\"5\"===5", false)
//				->runBool(ASTX_LOCATION, "k=\"6\"==3*2", true)
//				->run(ASTX_LOCATION, "object = {a: 1, b: \"some_string\", g:[1, 2, 3, 5, true, {a: b}, 5], d: { inh: {a:false} } }; print(object.g[5]a)", "5", "long selector")
//				->run(ASTX_LOCATION, "strVal=\"inh\"; print(strVal)", "inh", "var names conflict")
//				->runBool(ASTX_LOCATION, "object.d.inh.a", false, "props validity")
//				->runBool(ASTX_LOCATION, "object.d['inh']a", false)
//				->runBool(ASTX_LOCATION, "object.d['inh'].a", false)
//				->runBool(ASTX_LOCATION, "object.d[strVal].a", false)
//				->runBool(ASTX_LOCATION, "object.d[strVal]a", false)
//				->runDbl(ASTX_LOCATION, "object.g[0]", 1)
//				->run(ASTX_LOCATION, "object.g[0]=false; print(object.g[0]);", "false")
//				->run(ASTX_LOCATION, "print(2*3)", "6")
//				->runDbl(ASTX_LOCATION, "object.g[\"1\"]", 2)
//				->runBool(ASTX_LOCATION, "object.g[2 * 2]", true, "reflection")
//				->run(ASTX_LOCATION, "object.g[1+2] = {more: 1}; print(object.g[3])", "{\"more\":1}", "reassign")
//
			;

			
			
			
		};



		static inline Value * winOpened(Value * args) {
			printf("opened");
			return Value::npos;
		}

		static inline Value * menuClicked(Value * args) {
			printf("menuClicked");
			astox::icc::GUIRoot::getInstance()->setUndecorated(true);
			return Value::npos;
		}

		void run_icc_suite() {
			using namespace astox::icc;

		};


		void run_all() {

			clock_t tStart = clock();
//			run_suite_operators();
//			run_suite_logical();
//
			run_suite_script();
			
//			TestScriptSuite *s = TestScriptSuite::getInstance();
//			s->run(ASTX_LOCATION, "print(-3);", "-3")
////					->run(ASTX_LOCATION, "var a = false; print(!!a+\"{}\"+!a);", "false{}true", "scripting")
////					->run(ASTX_LOCATION, "print(-3);", "-3", "negative numbers")
//					->run(ASTX_LOCATION, "print(-3+'text');", "-3text", "new operation tests")
//					->run(ASTX_LOCATION, "print(1+-3);", "-2")
//					//->run(ASTX_LOCATION, "print(-(-3));", "3")
////					->run(ASTX_LOCATION, "var b=45; while(b>-3){b--;} print(b);", "-3", "while loop")
////					->run(ASTX_LOCATION, "b=45; while(b>-3){--b;} print(b);", "-3", "while loop 2")
//					;
//			printf("%s", astox::lang::toJSON(ecma::methods::astox_info(NULL)).c_str());




			printf("Time taken: %.2fs\n", (double)(clock() - tStart) / CLOCKS_PER_SEC);
		};
	}
}
