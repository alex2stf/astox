#ifndef ASTOX_LIBTEST_H_
#define ASTOX_LIBTEST_H_
#include <astox.h>

using namespace astox;
using namespace astox::scriptengine;
using namespace astox::testing;
using namespace astox::ecma::methods;
using namespace std;

static inline void test_date() {
	Date * date2 = new Date(1986, 1, 1);
	    assert_equals(date2->getHours(), 0, "testing date object");
	    assert_equals(date2->getUTCHours(), 22, "getUTCHours");
	    assert_equals(date2->getTimezoneOffset(), -120, "testing date object");
	    assert_equals(date2->getUTCDate(), 31,"testing date object");
	    assert_equals(date2->getDate(), 1, "testing date object");
	//    assert_equals(date2->getDay(), 6, "testing date object");
	    assert_equals(date2->getUTCDay(), 5, "new Date(1986, 1, 1).getUTCDay");
	    assert_equals(date2->getHourOffset(), 200, "new Date(1986, 1, 1).getHourOffset");


	    Date * date3 = new Date(2015, 2, 31, 13, 9, 27);
	    assert_equals(date3->getTimezoneOffset(), -180, "Date test");
	    log_stack("------------------%i", date3->getDay());
	//    assert_equals(date3->getDay(), 2, "Date test");
	//    assert_equals(date3->getUTCDay(), 2, "Date test");
	//    assert_equals(date3->getDate(), 3, "Date test");
	//    assert_equals(date3->getUTCDate(), 31, "Date test");
	//    assert_equals(date3->getUTCHours(), 10, "Date test");
	    assert_equals(date3->getHours(), 13, "Date test");
	//    assert_equals(date3->getHourOffset(), 300, "Date test");
	//
	//
	//
	//    Date * date = new Date(1970, 1, 0);
	//    assertEq(date->getYear(), 70, "Date test");
	//    assertEq(date->getUTCFullYear(), 1970, "Date(1970, 0, 0).getUTCFullYear = 1969");
	//    assertEq(date->getTimezoneOffset(), -120, "Date test");



	//    date = new Date(1970, 0, 1);
	//    assertEq(date->getCommonStr(), "Thu Jan 01 1970 00:00:00");
	//    date = new Date(1970, 1, 1);
	//    assertEq(date->getCommonStr(), "Sun Feb 01 1970 00:00:00");
	//    date = new Date(1970, 0, 0);
	//    assertEq(date->getCommonStr(), "Wed Dec 31 1969 00:00:00");
	//
	//
	//    date = new Date(1960, 0, 1, 25, 67, 89, 1000 * 60);
	//    assertEq(date->getCommonStr(), "Sat Jan 02 1960 02:09:29");
	//    date = new Date(1960, 0, 1);
	//    assertEq(date->getCommonStr(), "Fri Jan 01 1960 00:00:00");
	//    date = new Date(1950, 0, 1);
	//    assertEq(date->getCommonStr(), "Sun Jan 01 1950 00:00:00");
	//
	//    date = new Date(1930, 11, 34);
	//    assertEq(date->getCommonStr(), "Sat Jan 03 1931 00:00:00");
	//
	//    date = new Date(1930, 1, 1, 1, 1, 1, -1000);
	//    assertEq(date->getCommonStr(), "Sat Feb 01 1930 01:01:00");
	//
	//
	//
	//    date = new Date(1930, -1, 1);
	//    assertEq(date->getCommonStr(), "Sun Dec 01 1929 00:00:00");
	//
	//    date = new Date(1930, -13, 1);
	//    assertEq(date->getCommonStr(), "Sat Dec 01 1928 00:00:00");
	//
	//    date = new Date(1930, 0);
	//    assertEq(date->getCommonStr(), "Wed Jan 01 1930 00:00:00");
	//
	//    date = new Date(1930, 0, 0, 0, 0, 0, -1000);
	//    assertEq(date->getCommonStr(), "Mon Dec 30 1929 23:59:59");
	//
	//    date = new Date(1930, 0, -42, 0, 0, 0, -1000);
	//    assertEq(date->getCommonStr(), "Mon Nov 18 1929 23:59:59");
	//
	//    date = new Date(1930, 0, -42, 0, 0, 0, -1000);
	//    assertEq(date->getCommonStr(), "Mon Nov 18 1929 23:59:59");
	//
	//    date = new Date(1, 0, -42, 0, 0, 0, -1000);
	//    assertEq(date->getCommonStr(), "Sun Nov 18 1900 23:59:59");
	//
	//    date = new Date(1910, 0, -42, 0, 0, 0, -2000);
	//    assertEq(date->getCommonStr(), "Thu Nov 18 1909 23:59:58");
	//
	//    date = new Date(1930, 0, 0);
	//    assertEq(date->getCommonStr(), "Tue Dec 31 1929 00:00:00");
	//
	//    date = new Date(1930, 0, -1);
	//    assertEq(date->getCommonStr(), "Mon Dec 30 1929 00:00:00");
	//
	//    date = new Date(1930, 0, -13);
	//    assertEq(date->getCommonStr(), "Wed Dec 18 1929 00:00:00");
	//
	//    date = new Date(1930, 0, -31);
	//    assertEq(date->getCommonStr(), "Sat Nov 30 1929 00:00:00");
	//
	//    date = new Date(1930, 0, -61);
	//    assertEq(date->getCommonStr(), "Thu Oct 31 1929 00:00:00");
	//
	//
	//    date = new Date(1920, 0, 0);
	//    assertEq(date->getCommonStr(), "Wed Dec 31 1919 00:00:00");
	//
	//    date = new Date(1910, 0, 0); //ok mac
	//    assertEq(date->getCommonStr(), "Fri Dec 31 1909 00:00:00");
	//    date = new Date(1909, 1, 1); //ok mac
	//    assertEq(date->getCommonStr(), "Mon Feb 01 1909 00:00:00");
	//    date = new Date(1902, 0, 1); //ok mac
	//    assertEq(date->getCommonStr(), "Wed Jan 01 1902 00:00:00");
	//    date = new Date(1901, 12, 31); //ok mac
	//    assertEq(date->getCommonStr(), "Fri Jan 31 1902 00:00:00");
	//    date = new Date(1, 12, 31); //ok mac
	//    assertEq(date->getCommonStr(), "Fri Jan 31 1902 00:00:00");
	//
	//    date = new Date(99, -12, -31);
	//    assertEq(date->getCommonStr(), "Sun Nov 30 1997 00:00:00");
	//
	//    date = new Date(1, 1, 1);
	//    assertEq(date->getCommonStr(), "Fri Feb 01 1901 00:00:00");
	//
	//    date = new Date(255, 1, 1);
	//    assertEq(date->getCommonStr(), "Thu Feb 01 255 00:00:00");
	//
	//    date = new Date(0, 0, 0);
	//    assertEq(date->getCommonStr(), "Sun Dec 31 1899 00:00:00");
	//
	//    date = new Date(100, 0, 0);
	//    assertEq(date->getCommonStr(), "Thu Dec 31 99 00:00:00");
	//
	//    date = new Date(100, 1, 1);
	//    assertEq(date->getCommonStr(), "Mon Feb 01 100 00:00:00");
	//
	//
	//    date = new Date(100, 1, 0);
	//    assertEq(date->getCommonStr(), "Sun Jan 31 100 00:00:00");
	//    date = new Date(-1232, 0, 0);
	//    assertEq(date->getCommonStr(), "Sat Dec 31 -1233 00:00:00");
	//    date = new Date(-2, 0, 0);
	//    assertEq(date->getCommonStr(), "Thu Dec 31 -3 00:00:00", "JS RETURNS Wed Dec 31 -3 00:00:00");
	//
	//
	//    date = new Date(1971, 0, 0);
	//    assertEq(date->getDate(), 31);
	//    assertEq(date->getUTCDate(), 30);
	//    assertEq(date->getTimezoneOffset(), -120);


}






#endif
