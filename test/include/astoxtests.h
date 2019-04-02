#ifndef ASTOX_TEST_ROOT_H_
#define ASTOX_TEST_ROOT_H_

#include "datetest.h"
#include "regextest.h"
#include "syntaxjstest.h"
#include "threadtest.h"


static inline void run_all_tests(){
	test_date();
	test_regex();
	test_esoteric();
	test_serializers();
};


#endif /* SYNTAXJSTEST_H_ */
