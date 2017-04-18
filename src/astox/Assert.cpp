#include "Assert.h"
#include "FileSystem.h"
#include <time.h>
#include <stdlib.h>



namespace astox {
	namespace testing {
		static int testCount = 0;


#ifdef __ANDROID_API__
        int dtClr(bool r) {
            if (r) {
                return ANDROID_LOG_DEBUG;
            }
            return ANDROID_LOG_ERROR;
        };
#else
		int dtClr(bool r) {
			if (r) {
				return BG_GREEN_1;
			}
			return BG_RED_1;
		};
#endif

		void printResult(int testCount, std::string in, std::string ex, std::string msg, bool r, std::string location) {
            #ifdef __ANDROID_API__
                if (!msg.empty()) {
                    __android_log_print(dtClr(r), location.c_str(), "TST%i) %s (%s==%s) ... \n",
                                        testCount, msg.c_str(), in.c_str(), ex.c_str());
                }
                else {
                    __android_log_print(dtClr(r), location.c_str(), "TST%i) [%s]==[%s] ... \n",
                                        testCount, in.c_str(), ex.c_str());
                }
            #else
                if (!msg.empty()) {
                        astox_console_write(dtClr(r), "TST%i) %s (%s==%s) ... %s\n",
                            testCount, msg.c_str(), in.c_str(), ex.c_str(), location.c_str());
                }
                else {
                        astox_console_write(dtClr(r), "TST%i) [%s]==[%s] ... %s\n",
                            testCount, in.c_str(), ex.c_str(), location.c_str());
                }
            #endif
		};

		void printResult(int testCount, double in, double ex, std::string msg, bool r, std::string location) {
            #ifdef __ANDROID_API__
                if (!msg.empty()) {
                    __android_log_print(dtClr(r), location.c_str(), "TST%i) %s (%f==%f) ... \n",
                                        testCount, msg.c_str(), in, ex);
                }
                else {
                    __android_log_print(dtClr(r), location.c_str(), "TST%i) [%f]==[%f] ... \n",
                                        testCount, in, ex);
                }
            #else
                if (!msg.empty()) {
                    astox_console_write(dtClr(r), "TST%i) %s (%f==%f) ... %s\n",
                        testCount, msg.c_str(), in, ex, location.c_str());
                }
                else {
                    astox_console_write(dtClr(r), "TST%i) [%f]==[%f] ... %s\n",
                        testCount, in, ex, location.c_str());
                }
            #endif
		};

		void printResult(int testCount, std::string msg, bool r, std::string location) {
			astox_console_write(dtClr(r), "TST%i) %s at %s\n", testCount, msg.c_str(), location.c_str());
		};


		void assertEq(std::string input, std::string expect) {
			assertEq(input, expect, "", "");
		};

		void assertTrue(bool expect, std::string message, std::string location) {
			testCount++;
			printResult(testCount, message, expect, location);
		};

		void assertEq(std::string in, std::string expect, std::string message, std::string location) {
			testCount++;
			printResult(testCount, in, expect, message, (in == expect), location);
		};

		void assertEq(double in, double expect, std::string msg, std::string location) {
			testCount++;
			printResult(testCount, in, expect, msg, (in == expect), location);
		};


		
		void run_suite_operators() {
			printf("The following test suite may throw only compilation errors\n");
		
			using namespace std;
			Object fromStr = "{a:1, b:2, c:false, str:string, d : undefined long string}";
			cout << "cout << Object << " << fromStr << " << endl;" << endl;
			assertEq("{\"a\":1,\"b\":2,\"c\":false,\"d\":\"undefined long string\",\"str\":\"string\"}",
				astox::lang::toJSON(&fromStr, "", ""), ASTX_LOCATION);
			
			Array fromStrArr = "[1, 2, string long with spaces, false]";
			cout << "cout << Array << " << fromStrArr << " << endl;" << endl;

			Value * p_value = new Value();
			cout << "p_value: " << p_value << endl;

			Value r_value;
			cout << "r_value: " << r_value << endl;

			String * p_string = new String("dummy");
			cout << "p_string: " << p_string << endl;

			String r_string("reference");
			cout << "r_string: " << r_string << endl;

			Number * p_number = Number::New("27112004");
			cout << "p_number: " << p_number << endl;


			Number r_number(-455.5);
			cout << "r_number: " << r_number << endl;
			r_number++;
			cout << "r_number++ " << r_number << endl;
			++r_number;
			cout << "++r_number " << r_number << endl;
			r_number--;
			cout << "r_number-- " << r_number << endl;
			r_number--;
			cout << "r_number-- " << r_number << endl;
			r_number--;
			cout << "r_number-- " << r_number << endl;
			--r_number;
			cout << "--r_number: " << r_number << endl;
			--r_number;
			cout << "--r_number: " << r_number << endl;
			--r_number;
			cout << "--r_number: " << r_number << endl;



			Number data = 2;
			cout << "Number data = " << data << endl;
			data + 2;
			cout << "      data+2: " << data << endl;
			data - 1;
			cout << "      data-1: " << data << endl;
			data * 2;
			cout << "      data*2: " << data << endl;
			data / 2;
			cout << "      data/2: " << data << endl;
			cout << "      data+r_number " << data + r_number << endl;
			data += 2;
			cout << "      data+=2; " << data << endl;
			data -= 2;
			cout << "      data-=2 " << data << endl;
			data *= 2;
			cout << "      data*=2 " << data << endl;
			data /= 2;
			cout << "      data/=2 " << data << endl;
			cout << "      data!=r_number " << (data != r_number) << endl;
			cout << "      data==r_number " << (data == r_number) << endl;
			cout << "      data<=r_number " << (data <= r_number) << endl;
			cout << "      data>=r_number " << (data >= r_number) << endl;

			Number dumnr = 3;
			cout << "Number dumnr = " << data << endl;
			cout << "      data!=dumnr; " << (data != dumnr) << endl;
			cout << "      data==dumnr; " << (data == dumnr) << endl;

			cout << "      data>dumnr; " << (data > dumnr) << endl;
			cout << "      data<dumnr; " << (data < dumnr) << endl;

			Number dataNegative = -23344.455;
			cout << "Number dataNegative = " << dataNegative << endl;

			double vald = 3455;
			cout << "double vald = 3455;" << endl;
			Number dVal = vald;
			cout << "Number dVal = vald;" << dVal << endl;

			cout << "dumnr+dval " << dumnr + dVal << endl;
			cout << "dumnr-dval " << dumnr - dVal << endl;

			cout << "dumnr*dval " << dumnr * dVal << endl;
			cout << "dumnr/dval " << dumnr / dVal << endl;

			Boolean * p_bool = new Boolean(true);
			cout << "p_bool: " << p_bool << endl;

			Boolean * p_bool_false = new Boolean(false);
			cout << "p_bool_false: " << p_bool_false << endl;
			cout << "Boolean::btrue = " << Boolean::BTRUE << endl;
			cout << "Boolean::bfalse = " << Boolean::BFALSE << endl;

			Boolean r_bool(false);
			cout << "r_bool: " << r_bool << endl;

			Array * p_arr = new Array();
			cout << "p_arr: " << p_arr << endl;

			Array r_arr;
			cout << "r_arr: " << r_arr << endl;

			Object * p_object = new Object();
			p_object->addKey("number", p_number);
			p_object->addKey("array", p_arr);
			cout << "p_object: " << p_object << endl;

			Object r_obj;
			r_obj.addKey("string", &r_string);
			r_obj.addKey("inheritObject", p_object);
			cout << "r_object: " << r_obj << endl;


#ifdef ASTX_ENABLE_QT
			cout << "QString supported" << endl;
			QString qstring1 = "qstring1";
			String qstr1(qstring1);
			cout << "qstr1: " << qstr1 << endl;

			QString * qstring2 = new QString("asdasdas");
			String * qstr2 = new String(qstring2);
			cout << "qstr2: " << qstr2 << endl;
#endif


			String strOperators = "abcdefghijklmnopqrstuvxyz";
			cout << "strOperators: " << strOperators << endl;
			strOperators++;
			cout << "strOperators++: " << strOperators << endl;
			strOperators++;
			cout << "strOperators++: " << strOperators << endl;
			for (int i = 0; i < 5; i++) {
				strOperators--;
				cout << "strOperators--: " << strOperators << endl;
			}

			strOperators.reverse();

			cout << "strOperators.reverse();  " << strOperators << endl;

			for (int i = 0; i < 5; i++) {
				strOperators--;
				cout << "strOperators--: " << strOperators << endl;
			}


			strOperators.reverse();

			//strOperators+='_';
			strOperators += "|endOfString";

			std::string f = "std::string";
			strOperators += f;

			String temp = ".....<><><><>";
			cout << "String temp = \".....<><><><>\";" << endl;
			Number nums(20);
			cout << "Number nums(20);" << endl;
			String n2(456.455);
			cout << "String n2(456.455);" << endl;
			n2 += nums;
			cout << "n2+=nums;" << endl;
			strOperators += nums;
			cout << "strOperators+= nums; strOperators+= n2 => " << strOperators << endl;
			strOperators += n2;
			cout << "strOperators+temp = " << strOperators + temp << endl;
		};


		void run_suite_logical() {
			Path p1("directory/dir2/file/");
			assertEq(p1.getTokens().at(1), "dir2", "path.getTokens().at(0)", ASTX_LOCATION);
			assertEq(p1.getFileName(), "file", "path.getFileName()", ASTX_LOCATION);

			Path path("../../");
			assertEq(path.getTokens().at(0), "..", "path.getTokens().at(0)", ASTX_LOCATION);
			assertEq(path.getTokens().at(1), "..", "path.getTokens().at(1)", ASTX_LOCATION);
			assertTrue(path.isDot(), "path.isDot()", ASTX_LOCATION);
			assertEq(path.resolve("root/dir/altul"), Path::separatorFix("root/"), "path.resolve", ASTX_LOCATION);
			assertTrue(Date::isLeapYear(2000), "2000 is leap year", ASTX_LOCATION);
			assertTrue(!Date::isLeapYear(1900), "1900 is NOT leap year", ASTX_LOCATION);

			Date d(2015, 1);

			printf("%s ---- %i", d.toString().c_str(), d.getDay());
			assertTrue(astox::stringAlg::startsWith(d.toString(), "Sun Feb 01 2015 00:00:00"),
				"check if Date d(2015, 1).toString() starts with Sun Feb 01 2015 00:00:00", ASTX_LOCATION);
			assertEq(d.getFullYear(), 2015, "d.getYear() == 2015", ASTX_LOCATION);
			d.setMilliseconds(50000000);

			assertTrue(astox::stringAlg::startsWith(d.toString(), "Sun Feb 01 2015 13:53:20"),
				"check if after [d.setMilliseconds(50000000)] starts with Sun Feb 01 2015 13:53:20");

			d.setMilliseconds(-1000);
			assertEq(d.getSeconds(), 19, "d.getSeconds() == 19", ASTX_LOCATION);
			assertEq(d.getMilliseconds(), 0, "d.getMilliseconds() == 0", ASTX_LOCATION);

			d.setMilliseconds(-2222);
			assertEq(d.getMilliseconds(), 778, "d.getMilliseconds() == 778", ASTX_LOCATION);
			assertEq(d.getSeconds(), 16, "d.getSeconds() == 16", ASTX_LOCATION);

			d.setMilliseconds(-1);
			assertEq(d.getMilliseconds(), 999, "d.getMilliseconds() == 999", ASTX_LOCATION);
			assertEq(d.getSeconds(), 15, "d.getSeconds() == 15");

			d.setMilliseconds(-16000);
			assertEq(d.getMilliseconds(), 0, "d.getMilliseconds() == 0", ASTX_LOCATION);
			assertEq(d.getSeconds(), 59, "d.getSeconds() == 59", ASTX_LOCATION);

			printf("%s]]]\n", d.toString().c_str());
			printf("TODO........................... %i\n", d.getMilliseconds());

		}

		TestScriptSuite * TestScriptSuite::inst = new TestScriptSuite();

		std::string TestScriptSuite::getExpect() {
			return lxpct;
		};
		std::string TestScriptSuite::getId() {
			return lin;
		};
		std::string TestScriptSuite::getMessage() {
			return lmsg;
		};

		Value * assertPrint(Value * arguments) {
			std::string s = arguments->toString();
			assertEq(s, 
				TestScriptSuite::getInstance()->getExpect(), 
				TestScriptSuite::getInstance()->getMessage(),
				TestScriptSuite::getInstance()->getId()
			);
			return Value::npos;
		}

		TestScriptSuite::TestScriptSuite(){
			ctx = Function::getEcmaContext();
			ctx->addKey("print", new Function(assertPrint));
		}

		TestScriptSuite * TestScriptSuite::run(std::string id, std::string input, std::string expect, std::string msg) {
			lxpct = expect;
			lin = id;
			if (!msg.empty()) {
				lmsg = msg;
			}
			
			ecma::execScript(input, ctx, 6);
			return this;
		}


		TestScriptSuite *  TestScriptSuite::runDbl(std::string id, std::string input, double expect, std::string msg) {
			Value * valr = ecma::runCommand(input, ctx);
			double result = valr->toDouble();
			if (!msg.empty()) {
				lmsg = msg;
			}
			assertEq(result, expect, lmsg, id);
			return this;
		}

		TestScriptSuite * TestScriptSuite::runBool(std::string id, std::string input, bool expect, std::string msg) {
			Value * valr = ecma::runCommand(input, ctx);
			bool result = valr->toBoolean();
			if (!msg.empty()) {
				lmsg = msg;
				
			}
			assertTrue(result == expect, lmsg, id);
			
			return this;
		}

		TestScriptSuite * TestScriptSuite::getInstance() {
			return inst;
		};
		

		void run_suite_script() {
			TestScriptSuite * t = TestScriptSuite::getInstance();
			int zero = 0;

			t->run(ASTX_LOCATION, "var a = false; print(!!a+\"{}\"+!a);", "false{}true", "scripting")
				->run(ASTX_LOCATION, "f = function(data, one, two){ print(data); }; f('name');", "name")
				->run(ASTX_LOCATION, "function test(){}; b = test(16); print(b);", "undefined")
				->run(ASTX_LOCATION, "c=23; print(c);", "23")
				->run(ASTX_LOCATION, "print(test);", "function (){}")
				->run(ASTX_LOCATION, "print(new test);", "{}")
				->runDbl(ASTX_LOCATION, "2", 2, "numeric operands")
				->runDbl(ASTX_LOCATION, "2|5", 7)
				->runDbl(ASTX_LOCATION, "494847", 494847)
				->runDbl(ASTX_LOCATION, "-2 + 1", -1)
				->runDbl(ASTX_LOCATION, "25 % 3 + 4 % 5 - 2", 3)
				->runDbl(ASTX_LOCATION, "7 * 2 / 2 - 3 % 5", 4)
				->runDbl(ASTX_LOCATION, "1 + 2 * 3 / (2 + 1 - 1)", 4)
				->runDbl(ASTX_LOCATION, "(1+2) * 2", 6)
				->runDbl(ASTX_LOCATION, "1+2*2", 5)
				->runDbl(ASTX_LOCATION, "5 + 2 * 3 / 2 + 1 - 1 ", 8)
				->runDbl(ASTX_LOCATION, "(5 + 2 * 3) / 2 + 1 - 1", 5.5)
				->runDbl(ASTX_LOCATION, "9 - 2 * 2", 5) //merge
				->runDbl(ASTX_LOCATION, "23/5*(3)", 13.799999999999999)
				->runDbl(ASTX_LOCATION, "1 + 2", 3)
				->runDbl(ASTX_LOCATION, "1 + 2 + 3", 6)
				->runDbl(ASTX_LOCATION, "9 - (7 + 1)", 1)
				->runDbl(ASTX_LOCATION, "9 - 7 + 1", 3)
				->runDbl(ASTX_LOCATION, "5 + 2 * 3 / 2 + 1 - 1", 8)
				->runDbl(ASTX_LOCATION, "7 % 2", 1)
				->runDbl(ASTX_LOCATION, "15 % 2 * 2", 2)
				->runDbl(ASTX_LOCATION, "2 * 3 + 5 * 2", 16)
				->runDbl(ASTX_LOCATION, "7 * 2 / 2", 7)
				->runDbl(ASTX_LOCATION, "7 * 2 / 2 + 3 * 5", 22)
				->runDbl(ASTX_LOCATION, "7 * 2 / 2 + 3 % 5", 10)
				->runDbl(ASTX_LOCATION, "7 * 2 / 2 - 3 % 5", 4)
				->runDbl(ASTX_LOCATION, "25 % (3 + 4) % 5", 4)
				->runDbl(ASTX_LOCATION, "25 % (3 + 4) % 5 + 9", 13)
				->runDbl(ASTX_LOCATION, "25 % (3 + 4) % (5 + 9)", 4)
				->runDbl(ASTX_LOCATION, "25 % 3 + 4 % (5 + 9)", 5)
				->runDbl(ASTX_LOCATION, "25 % 3 + 4 % 5", 5)
				->runDbl(ASTX_LOCATION, "25", 25)
				->runDbl(ASTX_LOCATION, "2+5", 7)
				->runDbl(ASTX_LOCATION, "a = 4", 4)
				->runDbl(ASTX_LOCATION, "a + 2", 6)
				->runDbl(ASTX_LOCATION, "a++", 5)
				->runDbl(ASTX_LOCATION, "a--", 4)
				->runDbl(ASTX_LOCATION, "2+5-3", 4)
				->runDbl(ASTX_LOCATION, "2<<4", 32, "shifting")
				->runDbl(ASTX_LOCATION, "1+1<<4", 32)
				->runDbl(ASTX_LOCATION, "2<<4+1", 64)
				->runDbl(ASTX_LOCATION, "2<<4<<3", 256)
				->runDbl(ASTX_LOCATION, "2 << 4", 32)
				->runDbl(ASTX_LOCATION, "2 | 5 | 8", 15)
				->runDbl(ASTX_LOCATION, "2>>4", zero)
				->runDbl(ASTX_LOCATION, "8&&2&&4", (8 && 2 && 4))

				->runDbl(ASTX_LOCATION, "89 << 2", 356)
				->runDbl(ASTX_LOCATION, "a = b = c = number = d = f = 5", 5, "alloc")
				->runBool(ASTX_LOCATION, "f = 5 == 5", true, "booleans")
				->runBool(ASTX_LOCATION, "g=\"5\"!=5", false)
				->runBool(ASTX_LOCATION, "h=\"5\"==5", true)
				->runBool(ASTX_LOCATION, "i=\"5\"!==5", false)
				->runBool(ASTX_LOCATION, "j=\"5\"===5", false)
				->runBool(ASTX_LOCATION, "k=\"6\"==3*2", true)
				->run(ASTX_LOCATION, "object = {a: 1, b: \"some_string\", g:[1, 2, 3, 5, true, {a: b}, 5], d: { inh: {a:false} } }; print(object.g[5]a)", "5", "long selector")
				->run(ASTX_LOCATION, "strVal=\"inh\"; print(strVal)", "inh", "var names conflict")
				->runBool(ASTX_LOCATION, "object.d.inh.a", false, "props validity")
				->runBool(ASTX_LOCATION, "object.d['inh']a", false)
				->runBool(ASTX_LOCATION, "object.d['inh'].a", false)
				->runBool(ASTX_LOCATION, "object.d[strVal].a", false)
				->runBool(ASTX_LOCATION, "object.d[strVal]a", false)
				->runDbl(ASTX_LOCATION, "object.g[0]", 1)
				->run(ASTX_LOCATION, "object.g[0]=false; print(object.g[0]);", "false")
				->run(ASTX_LOCATION, "print(2*3)", "6")
				->runDbl(ASTX_LOCATION, "object.g[\"1\"]", 2)
				->runBool(ASTX_LOCATION, "object.g[2 * 2]", true, "reflection")
				->run(ASTX_LOCATION, "object.g[1+2] = {more: 1}; print(object.g[3])", "{\"more\":1}", "reassign")
			    
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

			GUIRoot * window = GUIRoot::getInstance();
			window->setTitle("window title");
			window->set("width", 900)
				->set("height", 400);
			;

			window->addMenu("&root", menuClicked);
			window->addMenu("&second", menuClicked);
			window->addSubMenu("submne")->addMenu("submn2", menuClicked);
			window->addEventListener("opened", winOpened);

			window->display();
			printf("screen size: %d & %d\n", getScreenSize().width, getScreenSize().height);
			printf("monitors: %d", countMonitors());
		};


		void run_all() {

			clock_t tStart = clock();
			run_suite_operators();
			run_suite_logical();
			
			run_suite_script();
			

			printf("%s", astox::lang::toJSON(ecma::methods::astox_info(NULL)).c_str());




			printf("Time taken: %.2fs\n", (double)(clock() - tStart) / CLOCKS_PER_SEC);
		};
	}
}
