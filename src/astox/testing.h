#ifndef ASTX_ASSERT_H_
#define ASTX_ASSERT_H_

#include <iostream>
#include "Mingui.h"

namespace astox {
	namespace testing {

		class TestScriptSuite  {
		private:
			std::string lxpct;
			std::string lin;
			std::string lmsg;
			Function *  ctx;
			TestScriptSuite();
			TestScriptSuite * tst;
		public:
			static TestScriptSuite * inst;
			std::string getExpect();
			std::string getId();
			std::string getMessage();
			TestScriptSuite * run(std::string id, std::string input, std::string expect, std::string msg="");
			TestScriptSuite * runDbl(std::string id, std::string input, double expect, std::string msg="");
			TestScriptSuite * runBool(std::string id, std::string input, bool expect = true, std::string msg = "");
		
			static TestScriptSuite * getInstance();
		};

		Value * assertPrint(Value * arguments);
		
		int dtClr(bool r);
		void printResult(int testCount, std::string in, std::string ex, std::string message, bool result, std::string location = "");
		void printResult(int testCount, double in, double ex, std::string message, bool result, std::string location = "");
		void printResult(int testCount, std::string message, bool result, std::string location = "");
		void assertEq(std::string input, std::string expect, std::string message, std::string location = "");
		void assertEq(std::string input, std::string expect);
		void assertEq(double input, double expect, std::string msg, std::string location = "");
		void assertTrue(bool expect, std::string message, std::string location = "");


		void run_suite_operators();
		void run_suite_logical();
		void run_suite_script();
		void run_all();

		void run_icc_suite();
	}
}


#endif
