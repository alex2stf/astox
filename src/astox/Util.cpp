#include "Util.h"
#include <sstream>
#include <iomanip>

namespace astox {

    namespace util {

            using namespace std;

			#ifdef ASTOX_OS_WINDOWS
				LPCWSTR CHAR2LPCWSTR(const char* charArray) {
					wchar_t wString[4096] = { 0 };
					//MultiByteToWideChar(0, 0, cs[1], strlen(cs[1]), filename, strlen(cs[1]));
					//MultiByteToWideChar(0, 0, cs, strlen(cs), filename, strlen(cs));
					//MultiByteToWideChar(CP_ACP, 0, charArray, -1, wString, 4096);
					//char text[] = "something";
					wchar_t wtext[20];
					mbstowcs(wtext, charArray, strlen(charArray) + 1);//Plus null
					LPWSTR ptr = wtext;
					return ptr;
				}
			#endif // ASTOX_OS_WINDOWS

			

            std::string replace (std::string in, std::string fnd, std::string rpl) {
                string tmp, rs;
                for (unsigned int i = 0; i < in.size(); i++) {
                    tmp += in.at(i);
                    if (tmp.size() == fnd.size()) {
                        if (tmp == fnd) {
                            tmp.clear();
                            rs.append(rpl);
                        } else {
                            rs += tmp.at(0);
                            tmp = tmp.substr(1, tmp.length() - 1);
                        }
                    }
                }
                if (tmp.empty() == false) {
                    rs.append(tmp);
                }
                return rs;
            }


    }





















    namespace stringAlg {



		std::string ecmaToString(double value) {
			using namespace std;

#if __cplusplus >= 201103L
            if (std::isnan(value)) {
                return "NaN";
            }
#else
            if (isnan(value)) {
				return "NaN";
			}
#endif

			//If m is NaN, return the String "NaN".

			//If m is + 0 or ?0, return the String "0".
			if (value == 0) {
				return "0";
			}

                        #ifndef ASTOX_COMPILER_MSC
                            if (value == -INFINITY) {
                                    return "-Infinity";
                            }
                        #endif
			// If m is less than zero, return the String concatenation of the String "-" and ToString(?m).
			if (value < 0) {
				return "-" + ecmaToString(-1 * value);
			}

#if __cplusplus >= 201103L
            if (std::isinf(value)) {
                return "Infinity";
            }
#else
            if (isinf(value)) {
				return "Infinity";
			}
#endif

			std::stringstream ss(stringstream::in | stringstream::out);
			ss << setprecision(18) << value;
			return ss.str();


		}


#ifdef ASTOX_OS_WINDOWS

std::wstring stdString2wstring(std::string& s) {
            int len;
            int slength = (int) s.length() + 1;
            len = MultiByteToWideChar(CP_ACP, 0, s.c_str(), slength, 0, 0);
            wchar_t* buf = new wchar_t[len];
            MultiByteToWideChar(CP_ACP, 0, s.c_str(), slength, buf, len);
            std::wstring r(buf);
            delete[] buf;
            return r;
        }

        std::wstring string2wstring(const std::string& str) {
            int size_needed = MultiByteToWideChar(CP_UTF8, 0, &str[0], (int) str.size(), NULL, 0);
            std::wstring wstrTo(size_needed, 0);
            MultiByteToWideChar(CP_UTF8, 0, &str[0], (int) str.size(), &wstrTo[0], size_needed);
            return wstrTo;
        }

#endif

#ifdef ASTX_ENABLE_NODEJS

std::string v8Value2StdString(v8::Handle<v8::Value> value) {
            v8::String::Utf8Value uftValue(value->ToString());
            return std::string(*uftValue);
        }
#endif

        std::string replace_allBetweenNeedles(std::string startNeedle, std::string endNeedle, std::string hayStack, std::string input) {

            size_t start = input.find(startNeedle);
            size_t end = input.find(endNeedle);
            std::string rest;
            while (start != std::string::npos && end != std::string::npos) {
                std::string startStr = input.substr(0, start);
                std::string endStr = input.substr(end, input.size());
                rest = input.replace(start, end + endNeedle.size(), hayStack);
                start = input.find(startNeedle);
                end = input.find(endNeedle);
            }

            return rest;
        }

        bool char_isWhitespace(char c) {
            return (isgraph(c) == false || isspace(c) || iswspace(c));
        }

        std::string &trim_left(std::string &s) {
            s.erase(s.begin(), std::find_if(s.begin(), s.end(), std::not1(std::ptr_fun<int, int>(std::isspace))));
            return s;
        }

        std::string &trim_right(std::string &s) {
            s.erase(std::find_if(s.rbegin(), s.rend(), std::not1(std::ptr_fun<int, int>(std::isspace))).base(), s.end());
            return s;
        }

        std::string &trim(std::string &s) {
            return trim_left(trim_right(s));
        }

        RegExpMatch::RegExpMatch() {
        }







#ifdef ASTOX_REGEX_SUPPORT_POSIX

        std::vector<RegExpMatch> regex_match_group(std::string input, std::string regexStr, AstoxRegexFlag rgx_flags) {
            std::vector<RegExpMatch> response;
            regmatch_t matches[ASTX_MAX_REGEX_MATCHES];
            regex_t exp; //Our compiled expression
            int rv;
            rv = regcomp(&exp, regexStr.c_str(), rgx_flags);

            if (rv != 0) {
                printf("regcomp failed with %d\n", rv);
                return response;
            }

            int start, end, j;
            int offset = 0, found = 0;

            while (regexec(&exp, input.c_str() + offset, ASTX_MAX_REGEX_MATCHES, matches, 0) == 0) {
                start = matches[0].rm_so + offset;
                end = matches[0].rm_eo - 1 + offset;


    #ifdef ASTX_DBG_REGEX
                astox_debug(__LINE__, __FILENAME__, "regex_match_group:: regex /%s/ at bytes %d-%d", input.c_str(), start, end);
    #endif

                RegExpMatch rdata;
                rdata.start = start;
                rdata.end = end;

                if (start == end) {
                    rdata.part += input[start];
                }
                else {
                    for (j = start; j <= end; j++) {
                        rdata.part += input[j];
                    }
                }
                response.push_back(rdata);
                offset += matches[0].rm_eo;
                found = 1;
            }
            regfree(&exp);
            return response;
        }

        std::string regex_replace_all(std::string input, std::string foundable, std::string needle, AstoxRegexFlag rgx_flags) {
            using namespace std;
            std::vector<RegExpMatch> rmatches = regex_match_group(input, foundable, rgx_flags);
            return __regex_replace_posix(input, needle, rmatches);
        }

        std::string __regex_replace_posix(std::string input, std::string needle, std::vector<RegExpMatch> &matches) {

            using namespace std;

            string response;
            int lastPoz = 0;
            int nextPoz = matches.at(0).start;
            int vsize = matches.size();
            for (int i = 0; i < vsize; i++) {
                string tmp = input.substr(lastPoz, nextPoz - lastPoz);
                //		cout << "input= |" << input << endl;
                //		cout << "from " << lastPoz << " to " << nextPoz << endl;
                //		cout << "tmp= |" << tmp << endl;

                response.append(tmp);
                response.append(needle);
                lastPoz = matches.at(i).end + 1;
                nextPoz = (i < vsize - 1) ? matches.at(i + 1).start : vsize;
                //		cout << matches.at(i).start << endl;
                //		cout << matches.at(i).end << endl;
                //		cout << "part = |" << matches.at(i).part << endl;
                //		cout << "response= |" << response << endl;
            }

            //	cout << response << endl;
            return response;
        }

#endif

    }

}
