#ifndef ASTX_ASSERT_H_
#define ASTX_ASSERT_H_

#include <iostream>
#include <exception>
#include "mingui.h"
#include "methods.h"


namespace astox {
	namespace testing {
		class ExpectStdout : public Function {
			public:
				ExpectStdout();
				Value * call(std::string &args, const scriptengine::lexic &lexic, Function *&context);
		};

		#ifndef EX_OSERR
			#define EX_OSERR        71      /* system error (e.g., can't fork) */
		#endif
		#define test_result_success(...) {astox_console_write(BG_GREEN_1, __VA_ARGS__)}	
		#ifdef ASTOX_COMPILER_MSC
			#define test_result_fail(...) { char argstr[ASTX_MAX_BUFSZ]; sprintf_s(argstr, ASTX_MAX_BUFSZ, __VA_ARGS__); fprintf(stderr, argstr); printf(argstr); system("pause"); std::exit(-1);}
        #elif __ANDROID_API__
			#define test_result_fail(...) { __android_log_print(ANDROID_LOG_DEBUG, "TEST_FAILURE", __VA_ARGS__);  }
        #else
			//ubuntu
			#define test_result_fail(...) {  printf( __VA_ARGS__);  std::exit(EX_OSERR); }
		#endif


		void assertEq(std::string input, std::string expect, std::string message, std::string location);
		void assertEq(std::string input, std::string expect, std::string location);
		void assertEq(std::string input, std::string expect);
		void assertEq(Value * input, Value * expect,  std::string msg = "", std::string location = "");
		void assertEq(Value * input, std::string expect,  std::string msg = "", std::string location = "");
		void assertEq(String * input, std::string expect,  std::string msg = "", std::string location = "");
		void assertEq(Value * input, double expect, std::string msg = "", std::string location = "");
		void assertEq(double input, double expect, std::string msg = "", std::string location = "");
		void assertEq(RegExpMatch match, std::string part, int start, int end, std::string location = "");
		void assertTrue(bool expect, std::string message, std::string location = "");
		void assertExecuteOperation(std::string input, const scriptengine::lexic &lexic,
				Function *& context, double expect, std::string location);
		void assertExecuteOperation(std::string input, const scriptengine::lexic &lexic,
						Function *& context, std::string expect, std::string location);
		void assertIsNull(Value * value, std::string location);
		void assertRegExpFlags(RegExp * regexp, stx_regex_flag regexFlags, std::string location = "");
		void assertFail(Error &n, stxtr expectmessage, stxtr location);
		void assertContains(stxtr input, stxtr expect,  stxtr location = "");
		void assertContains(Value * value, stxtr expect, stxtr location = "");

#define assert_expression_failure(expression, expected_message){\
			int n = 0; try {expression; } catch(astox::Error &e){ astox::testing::assertFail(e, expected_message, ASTX_LOCATION); n = 1; } catch(...) { printf("other exception executed");}\
			if(n == 0) { test_result_fail("Error expected but nothing thrown at %s", ASTX_LOCATION) }\
		}

#define assert_equals(left, right, message){\
			assertTrue(left == right, message, ASTX_LOCATION);\
		}



		void run_suite_operators();
		void run_suite_logical();
		void run_suite_script();
		void run_all();

		void run_icc_suite();
	}
}


#endif
