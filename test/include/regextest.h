#ifndef ASTOX_REGEX_TEST_H
#define ASTOX_REGEX_TEST_H

#include "staticvars.h"

static inline void test_regex(){
	std::string s; std::vector<std::string> vs;
	int dfz = 0;
	command_node cmd;

	vector<RegExpMatch> matches;
	Value * v = scriptengine::get_member("{a:1, b:true, c:'string', d:[2, true, 46], e : {ea:1, eb:3, ec: {ecp : 2, ecd:[1, 2, 3]}}}", ctx, lxc, NULL, true);

			s = serializers::to_json(v);
			assertEq(s,
					"{\"a\":1,\"b\":true,\"c\":\"string\",\"d\":[2,true,46],\"e\":{\"ea\":1,\"eb\":3,\"ec\":{\"ecd\":[1,2,3],\"ecp\":2}}}",
					ASTX_LOCATION);
			stxtr e = "";
			s = serializers::to_composed(v, e, e, false, false, false, '-', '-', '-', ',', '|', '|', ';');
			assertEq(s, "|a;1,b;true,c;-string-,d;-2,true,46-,e;|ea;1,eb;3,ec;|ecd;-1,2,3-,ecp;2|||", ASTX_LOCATION);

//			e = ".";
//			stxtr e2 = "-";
//			s = serializers::to_composed(v, e, e2, false, false, false);
//			assertEq(s, "{-..a:1,-..b:true,-..c:\"string\",-..d:[-....2,-....true,-....46-....],-..e:{-....ea:1,-....eb:3,-....ec:{-........ecd:[-................1,-................2,-................3-................],-........ecp:2-........}-....}-..}");
			s = serializers::to_js(v);
			assertEq(s, "{a:1,b:true,c:'string',d:[2,true,46],e:{ea:1,eb:3,ec:{ecd:[1,2,3],ecp:2}}}");

			assertEq(serializers::to_json(String::New("data", Syntax::JS)), "\"data\"");

			matches = util::regex_match_group("zzbzzbzzczzdz",  "zz", JSREG_GLOBAL);
			assertEq(
					 util::regex_replace_all("zzbzzbzzczzdz", "zz", "xxx", JSREG_GLOBAL),
					 util::regex_replace_matches("zzbzzbzzczzdz", "xxx", matches, 0, 0)
			);


			matches = util::regex_match_group("cat bat hat wat ???",  "[hcb]at", JSREG_GLOBAL);
			assertEq(
					 util::regex_replace_all("cat bat hat wat ???", "[hcb]at", "!", JSREG_GLOBAL),
					 util::regex_replace_matches("cat bat hat wat ???", "!", matches, 0, 0)
			);

			matches = util::regex_match_group("hat hat hat",  "[hcb]at", JSREG_GLOBAL);
			assertEq(
					 util::regex_replace_all("hat hat hat", "[hcb]at", "?", JSREG_GLOBAL),
					 util::regex_replace_matches("hat hat hat", "?", matches, 0, 0)
			);

			s = util::regex_replace_all("zzbzzbzzczzdz", "zz", "xxx", JSREG_GLOBAL);
			assertEq(s, "xxxbxxxbxxxcxxxdz", ASTX_LOCATION);
			s = util::regex_replace_all("zzbzzbzzczzdz", "zz", "xxx", JSREG_BASIC);
			assertEq(s, "xxxbxxxbxxxcxxxdz", ASTX_LOCATION);
			s = util::regex_replace_all("abacad", "a", "i", JSREG_BASIC);
			assertEq(s, "ibicid", ASTX_LOCATION);
			s = util::regex_replace_all("babacad", "a", "i", JSREG_BASIC);
			assertEq(s, "bibicid", ASTX_LOCATION);
			s = util::regex_replace_all("abcd", "a", "i", JSREG_BASIC);
			assertEq(s, "ibcd", ASTX_LOCATION);

			matches = util::regex_match_group("Thes es a es test eses", "es", JSREG_GLOBAL|JSREG_MULTILINE);
			assert_equals(matches.size(), 6, "");
			assertEq(matches.at(0), "es", 2, 4, ASTX_LOCATION);
			assertEq(matches.at(1), "es", 5, 7, ASTX_LOCATION);
			assertEq(matches.at(2), "es", 10, 12, ASTX_LOCATION);
			assertEq(matches.at(3), "es", 14, 16, ASTX_LOCATION);
			assertEq(matches.at(4), "es", 18, 20, ASTX_LOCATION);
			assertEq(matches.at(5), "es", 20, 22, ASTX_LOCATION);


			matches = util::regex_match_group("This is a test", "t", JSREG_GLOBAL|JSREG_MULTILINE);
			assert_equals(matches.size(), 2, "");
			assertEq(matches.at(0), "t", 10, 11, ASTX_LOCATION);
			assertEq(matches.at(1), "t", 13, 14, ASTX_LOCATION);

			assert_expression_failure(
					util::regex_match_group("This is a test", "", JSREG_GLOBAL | JSREG_MULTILINE),
					"empty regex search key"
			);

			matches = util::regex_match_group("aat, bat, cat, dat, hat", "[hc]at", JSREG_GLOBAL|JSREG_MULTILINE);
			assert_equals(matches.size(), 2, "");
			assertEq(matches.at(0), "cat", 10, 13, ASTX_LOCATION);
			assertEq(matches.at(1), "hat", 20, 23, ASTX_LOCATION);

			s = util::regex_replace_all("bzzzbz", "z", "x", JSREG_GLOBAL);
			assertEq(s, "bxxxbx", ASTX_LOCATION);


			//tests for non-posix regex
			#ifndef ASTOX_REGEX_SUPPORT_POSIX
				matches = util::regex_match_group("This is a test", "\\w+", JSREG_GLOBAL);
			#endif


}



#endif
