#include <astox.h>
#include <sstream>
#include <iomanip>



namespace astox {

	RegExpMatch::RegExpMatch() {
		start = -1;
		end = -1;
	}

    namespace util {

            using namespace std;



			#ifdef ASTOX_OS_WINDOWS
				//LPCWSTR CHAR2LPCWSTR(const char* charArray) {
					//wchar_t wString[4096] = { 0 };
					//MultiByteToWideChar(0, 0, cs[1], strlen(cs[1]), filename, strlen(cs[1]));
					//MultiByteToWideChar(0, 0, cs, strlen(cs), filename, strlen(cs));
					//MultiByteToWideChar(CP_ACP, 0, charArray, -1, wString, 4096);
					//char text[] = "something";
					//wchar_t wtext[20];
					//mbstowcs(wtext, charArray, strlen(charArray) + 1);//Plus null
					//LPWSTR ptr = wtext;
					//return ptr;
				//}
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

            bool char_is_whitespace(char c) {
				return (isgraph(c) == false || isspace(c) || iswspace(c));
			}

            bool string_is_numeric(const std::string &in, char d){
				if(in.empty()){ return false; }
				for(int i = 0; i < in.size(); i++){
					char c = in.at(i);
					bool iv = (c >= '0' && c <= '9') || c == d || c == '+' || c == '-';
					if(!iv){
						return false;
					}
				}
				return true;
			};


#ifdef ASTOX_REGEX_SUPPORT_POSIX
		#ifndef REG_NOMATCH
			#define REG_NOMATCH 1
		#endif


		static inline int posix_compile_regex (regex_t * r, const char * regex_text, stx_regex_flag m){
			log_stack("%s", regex_text);
			int status = regcomp (r, regex_text, m);
			if (status != 0) {
			char error_message[0x1000];
			regerror (status, r, error_message, 0x1000);
				printf ("Regex error compiling '%s': %s\n",
						 regex_text, error_message);
				return 1;
			}
			return 0;
		}


		static inline int posix_match_regex (regex_t * r, std::string input, std::vector<RegExpMatch> &buff) {

		    const char * to_match = input.c_str();
			const char * p = input.c_str();
		    const int n_matches = 100;
		    regmatch_t m[n_matches];

		    while (1) {
		        int i = 0;
		        int nomatch = regexec (r, p, n_matches, m, 0);
		        if (nomatch) {
		            return nomatch;
		        }
		        for (i = 0; i < n_matches; i++) {
		            int start;
		            int finish;
		            if (m[i].rm_so == -1) {
		                break;
		            }
		            start = m[i].rm_so + (p - to_match);
		            finish = m[i].rm_eo + (p - to_match);
//		            printf ("'%.*s' (bytes %d:%d)\n", (finish - start),
//		            		                    to_match + start, start, finish);
		            RegExpMatch rem;
		            rem.part =  input.substr(start, finish-start);
		            rem.start = start;
		            rem.end = finish;
		            buff.push_back(rem);
//		            printf ("i = %s ", rem.part.c_str());
		        }
		        p += m[0].rm_eo;
		    }
		    return 0;
		}

        std::vector<RegExpMatch> regex_match_group(std::string regex_text, std::string find_text, stx_regex_flag rgx_flags) {
        	std::vector<RegExpMatch> response;
        	if(find_text.empty()){
        		throw_error("empty regex search key for input %s", regex_text.c_str());
        		return response;
        	}

            regex_t r;
		    log_stack ("Trying to find '%s' in '%s'\n", find_text.c_str(),  regex_text.c_str());
		    posix_compile_regex (&r, find_text.c_str(), rgx_flags);
		    posix_match_regex (&r, regex_text, response);
		    regfree (&r);
            return response;
        }

        std::string regex_replace_all(string in, string fnd, std::string ndl, stx_regex_flag rflg) {
			vector<RegExpMatch> mat = regex_match_group(in, fnd, rflg);
            if(!mat.empty()){
            	return regex_replace_matches(in, ndl, mat, 0, 0);
            } else {
            	log_stack("empty match");
            }

            return in;
        }

    #elif defined ASTOX_REGEX_SUPPORT_C11

       std::vector<RegExpMatch> regex_match_group(std::string input, std::string regexStr, stx_regex_flag rgx_flags) {
            std::vector<RegExpMatch> response;
			if (regexStr.empty()) {
				throw_error("empty regex search key for input %s", regexStr.c_str());
				return response;
			}
            using namespace std::regex_constants;

			try {
				std::regex re(regexStr.c_str());
				std::sregex_iterator next(input.begin(), input.end(), re);
				std::sregex_iterator end;
				while (next != end) {
					std::smatch match = *next;
					RegExpMatch rdata;
					rdata.part = match.str();
					rdata.start =  match.position();
					rdata.end = (rdata.start + rdata.part.size());
					response.push_back(rdata);
					log_stack("match %s", match.str().c_str());
					
					next++;

				}
			}
			catch (std::regex_error& e) {
				log_stack("Syntax error in the regular expression, %s", e.what());
				system("pause");
				// Syntax error in the regular expression
			}
			/*
            std::cmatch cm;
            std::cout << " start regex match " << regexStr << " for " << input << std::endl;
            std::regex re(regexStr.c_str(), rgx_flags);
			std::sregex_iterator next(input.begin(), input.end(), re);

            for (std::sregex_iterator i = std::sregex_iterator(input.begin(), input.end(), r); i != std::sregex_iterator(); ++i) {
                std::smatch m = *i;
                RegExpMatch rdata;
                rdata.part = m.str();
                rdata.start = m.position();
                rdata.end = rdata.start + rdata.part.size() - 1;
                response.push_back(rdata);
                std::cout << input << std::endl;
                std::cout << m.str() << " from " << rdata.start << " to " << rdata.end << '\n';
            }
			*/
            return response;
        };

       	std::string regex_replace_all(std::string input, std::string foundable, std::string needle, stx_regex_flag rgx_flags) {
            std::regex e(foundable.c_str(), rgx_flags);
            return std::regex_replace(input, e, needle);
        };

    #endif




	string regex_replace_matches(string in, string ndl, vector<RegExpMatch> &mat, size_t vindex, size_t sindex) {

        	log_stack("init %s", in.c_str());
        	string r;

        	RegExpMatch m = mat.at(vindex);

        	log_stack("%d: %s", (int)vindex, blurt(Value::convert(m, Syntax::JS)).c_str());
        	size_t i = 0;

			for(i = sindex; i < m.start; i++){
				r+=in.at(i);
			}

        	r.append(ndl);
        	log_stack("r = %s", r.c_str());

        	if(vindex + 1 < mat.size()){
        		string s = regex_replace_matches(in, ndl, mat, vindex+1, m.end);
        		r.append(s);
        	} else {
        		for(i = m.end; i < in.size(); i++){
					r+=in.at(i);
				}
        	}

            return r;
	}



	std::string read_file_to_string(std::string path){
		std::ifstream t(path.c_str());
		std::string str((std::istreambuf_iterator<char>(t)),
		                 std::istreambuf_iterator<char>());
		return str;
	};


	void write_string_to_file(std::string inputString, std::string filePath) {
		std::ofstream o(filePath.c_str());
		o << inputString;
		o.close();
	}

    } //exit util namespace





















    namespace stringAlg {



		const char * ecmaToString(double value) {
			using namespace std;
//1. If m is NaN, return the string "NaN".
			#if __cplusplus >= 201103L
				if (std::isnan(value)) { return "NaN";}
			#else
				if (isnan(value)) { return "NaN"; }
			#endif

			//If m is + 0 or ?0, return the String "0".
			if (value == 0) { return "0"; }

			//If m is infinity, return the string "Infinity".
			#ifndef ASTOX_COMPILER_MSC
				if (value == -INFINITY) { return "-Infinity"; }
			#endif

			// If m is less than zero, return the string concatenation of the string "-" and toString(-m)
			if (value < 0) {
				string s = "-";
				s.append(ecmaToString(-1 * value));
				return s.c_str();
			}

			//If m is infinity, return the string "Infinity".
			#if __cplusplus >= 201103L
				if (std::isinf(value)) { return "Infinity"; }
			#else
				if (isinf(value)) { return "Infinity"; }
			#endif

			std::stringstream ss(stringstream::in | stringstream::out);
			ss << value;
			return ss.str().c_str();
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











    }

}
