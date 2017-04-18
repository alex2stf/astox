/*
 * StringAlg.h
 *
 *  Created on: Nov 28, 2013
 *      Author: alexandru
 *      TODO work on C
 */

#include "Macros.h"


#ifdef ASTOX_REGEX_SUPPORT_POSIX
    #define JSREG_GLOBAL REG_EXTENDED
    #define JSREG_IGNORECASE  REG_ICASE
    #define JSREG_MULTILINE  REG_NEWLINE
    #define JSREG_BASIC  REG_EXTENDED
    #define JSREG_NONE -3
    #define AstoxRegexFlag int
#else
    #define AstoxRegexFlag std::regex_constants::syntax_option_type
    #define JSREG_GLOBAL std::regex_constants::extended
    #define JSREG_IGNORECASE  std::regex_constants::icase
    #define JSREG_MULTILINE  std::regex_constants::grep
    #define JSREG_BASIC   std::regex_constants::basic
    #define JSREG_NONE std::regex_constants::awk
#endif




#ifndef STRINGALG_H_
    #define STRINGALG_H_

    #ifdef ASTOX_OS_WINDOWS
        #define UTILS_OS_NEWLINE '\r\n'
        #define CHAR_OS_SEPARATOR '\\'
    #elif defined macintosh // OS 9
        #define UTILS_OS_NEWLINE '\r'
        #define CHAR_OS_SEPARATOR '/'
    #else
        #define UTILS_OS_NEWLINE '\n' // Mac OS X uses \n
        #define CHAR_OS_SEPARATOR '/'
    #endif

    #define ASTX_MAX_REGEX_MATCHES 1000

namespace astox {

    namespace util {
            std::string replace (std::string in, std::string fnd, std::string rpl);
			#ifdef ASTOX_OS_WINDOWS
				LPCWSTR CHAR2LPCWSTR(const char* cs);

				static inline std::wstring s2ws(const std::string& s)
				{
					int len;
					int slength = (int)s.length() + 1;
					len = MultiByteToWideChar(CP_ACP, 0, s.c_str(), slength, 0, 0);
					wchar_t* buf = new wchar_t[len];
					MultiByteToWideChar(CP_ACP, 0, s.c_str(), slength, buf, len);
					std::wstring r(buf);
					delete[] buf;
					return r;
				}
			#endif // ASTIX_OS_WINDOWS

    }

    namespace stringAlg {

    #ifdef UNICODE
        typedef std::wostringstream winapistringstream;
    #else
        typedef std::ostringstream winapistringstream;
    #endif

        class RegExpMatch {
        public:
            RegExpMatch();
            int start;
            int end;
            std::string part;
        };

    #ifdef ASTOX_REGEX_SUPPORT_POSIX
        std::string __regex_replace_posix(std::string input, std::string needle, std::vector<RegExpMatch> &matches);
        std::vector<RegExpMatch> regex_match_group(std::string input, std::string regexStr, AstoxRegexFlag rgx_flags = JSREG_GLOBAL);
        std::string regex_replace_all(std::string input, std::string foundable, std::string needle, AstoxRegexFlag rgx_flags = JSREG_GLOBAL);

    #elif defined ASTOX_REGEX_SUPPORT_C11

            static inline std::vector<RegExpMatch> regex_match_group(std::string input, std::string regexStr, AstoxRegexFlag rgx_flags = JSREG_GLOBAL) {
            std::vector<RegExpMatch> response;
            using namespace std::regex_constants;
            std::cmatch cm;
            std::cout << " start regex match " << regexStr << " for " << input << std::endl;
            std::regex r(regexStr.c_str(), rgx_flags);

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
            return response;
        };

        static inline std::string regex_replace_all(std::string input, std::string foundable, std::string needle, AstoxRegexFlag rgx_flags = JSREG_GLOBAL) {
            std::regex e(foundable.c_str(), rgx_flags);
            return std::regex_replace(input, e, needle);
        };

    #endif

		std::string ecmaToString(double value);


    #ifdef ASTOX_OS_WINDOWS
        std::wstring stdString2wstring(std::string& s);
        std::wstring string2wstring(const std::string& str);
    #endif

    #ifdef ASTX_ENABLE_NODEJS
        std::string v8Value2StdString(v8::Handle<v8::Value> value);
    #endif

        static unsigned int iter = 0;
        static std::string temp = "";
        static std::string response = "";



        std::string replace_allBetweenNeedles(std::string startNeedle, std::string endNeedle, std::string hayStack, std::string input);

        
        inline static bool startsWith(const std::string& str, const std::string& start) {
            if (&start == &str) return true; // str and start are the same string
            if (start.length() > str.length()) return false;
            for (size_t i = 0; i < start.length(); ++i) {
                if (start[i] != str[i]) return false;
            }
            return true;
        }
        
        
        inline static bool startsWithIgnoreCase(const std::string& str, const std::string& start) {
            if (&start == &str) return true; // str and start are the same string
            if (start.length() > str.length()) return false;
            for (size_t i = 0; i < start.length(); ++i) {
                if (std::tolower(start[i]) != std::tolower(str[i])) return false;
            }
            return true;
        }

        inline static char * subStr(const char* srcString, const int offset) {
            char * sub = (char*) malloc(strlen(srcString) + 1);
            memcpy(sub, srcString + offset, strlen(srcString));
            sub[strlen(srcString)] = 0;
            return sub;
        }

        inline static bool stringContainsString(std::string input, std::string search) {
    #ifdef BOOST_STRING_ALGO_HPP
            return boost::algorithm::find_first(input, search);
    #else
            if (input.find(search) != std::string::npos) {
                return true;
            }
            return false;
    #endif
        }

        inline static void throwWarning(const char * message, int line, const char * fileName) {
            fprintf(stderr, "\nWARNING #%i:  %s   \n    %s \n", line, fileName, message);
        }







        //TODO asta face probleme

        inline static std::vector<std::string> smartSplitKey(std::string input) {

            std::vector<std::string> response;
            std::string temp;
            int num = 0;

            for (unsigned int i = 0; i < input.size(); i++) {
                char c = input.at(i);

                switch (c) {
                    case '"':
                        num++;
                        break;

                    case '=':
                        //		case ' ': // asta produce erori la string de forma type = "text"
                        if (num % 2 == 0) {
                            //				std::cout << "temp: " << temp << std::endl;
                            response.push_back(temp);
                            temp.clear();
                        } else {
                            temp += c;
                        }


                        break;

                    default:
                        temp += c;
                        break;


                }

            }

            if (!temp.empty()) {
                //		std::cout << "temp: " << temp << std::endl;
                response.push_back(temp);
            }


            return response;
        }

        static inline std::string strEncode(std::string input, int shift = 3) {

            std::string response;
            for (unsigned int i = 0; i < input.size(); i++) {
                input.at(i) = input.at(i) + shift;
            }

            return input;
        }

        static inline std::string strDecode(std::string input, int shift = 3) {
            std::string response;
            for (unsigned int i = 0; i < input.size(); i++) {
                input.at(i) = input.at(i) - shift;
            }

            return input;
        }


        bool char_isWhitespace(char c);

        static inline std::string doubleToString(double x) {
            std::stringstream s;
            s << x;
            return s.str();
        }//exit double to string conversion

        inline static void saveStringToFile(std::string inputString, std::string filePath) {
            std::ofstream o(filePath.c_str());
            o << inputString << std::endl;
            o.close();
        }

    #include <stdlib.h>

        inline static std::string readFile(std::string filename) {
            //	FILE *ifp;
            //	ifp = fopen(filename.c_str(), "r");
            //	long input_file_size;
            //	char * buffer;
            //
            //	if (ifp != NULL) {
            //	  fprintf(stderr, "Open input file in readFile !\n");
            //
            //	  input_file_size = ftell(ifp);
            //	  buffer = malloc((input_file_size + 1) * (sizeof(char)));
            //	  fread((void *)buffer, sizeof(char), input_file_size, ifp);
            //
            //
            //	  buffer[input_file_size] = 0;
            //	  printf("%s\n", buffer);
            //	  fclose(ifp);
            //
            //	}else{
            //		fprintf(stderr, "Can't open input file in readFile !\n");
            //	}

            using namespace std;

    #ifdef ASTOX_OS_WINDOWS
            FILE* myfile;
            fopen_s(&myfile, filename.c_str(), "r");
    #else
            FILE* myfile = fopen(filename.c_str(), "r");
    #endif



            char ch;
            std::string result;

            if (myfile != NULL) {

                while ((ch = fgetc(myfile)) != EOF) {
                    result += ch;
                }


                fclose(myfile);
                return result;
            }

            return "ERROR REDDING FILE " + filename;
        }

        inline static std::string toWebSocketSHA1Model(std::string data) {


            std::string response;
            for (unsigned int i = 0; i < data.size(); i++) {
                if (i % 2 == 0) {
                    response.append("0x");
                }
                response += data.at(i);

                if (i % 2 != 0) {
                    response.append(" ");
                }
            }

            return response;
        }

        inline static void appendStringFromFile(std::string filename, std::string& output, const bool keepLineEnd = true) {
            std::ifstream o;
            std::string line;
            std::string res;
            o.open(filename.c_str(), std::ios::in);
            if (o.is_open()) {
                while (getline(o, line)) {
                    //			std::cout << line << std::endl;
                    output.append(line);
                    if (line.size() > 0) {
                        if (keepLineEnd && line.at(line.size() - 1) != '\n') {
                            output.append("\n");
                        }
                    }
                }
            } else {
                printf("Unable to append from file %s\n", filename.c_str());
            }

            o.close();
        }

        static inline bool isAllWhiteSpace(const std::string &s) {
            return "??"; //std::all_of(s.begin(),s.end(),isspace);
        }

        // trim from start
        std::string &trim_left(std::string &s);

        // trim from end
        std::string &trim_right(std::string &s);

        // trim from both ends
        std::string &trim(std::string &s);

        //static inline void trim(std::string& input){
        //	if(input.empty() == false){
        //		char first = input.at(0);
        //		char last =  input.at(input.length()-1);
        //
        //
        //		std::cout << "last char: " << last  << isgraph(last) << isspace(last) << iswspace(first) << std::endl;
        //		if(isspace(first) || isgraph(first) == false  || iswspace(first) || first == ' ' || first == '\r' || first == '\n' || first == '\f' || first == '\t'){
        //			//input = input.substr(1, input.size());
        //			input.erase(1);
        //
        //			trim(input);
        //		}
        //
        //		if(isspace(last) || isgraph(last) == false  || iswspace(last) || last == ' ' || last == '\r' || last == '\n' || last == '\f' || last == '\t'){
        //			//input = input.substr(0, input.size()-1);
        //			input.resize (input.size () - 1);
        //			trim(input);
        //		}
        //	}

        //	input.erase(input.find_last_not_of(" \n\r\t")+1);
        //}//exit trim function

//        static inline void replace_all(std::string& input, std::string foundable, std::string replaceble) {
//
//
//            temp.clear();
//            response.clear();
//
//            for (iter = 0; iter < input.size(); iter++) {
//                temp += input.at(iter);
//                if (temp.size() == foundable.size()) {
//                    if (temp == foundable) {
//                        temp.clear();
//                        response.append(replaceble);
//                    } else {
//                        response += temp.at(0);
//                        temp = temp.substr(1, temp.length() - 1);
//                    }
//                }
//
//            }
//
//            if (temp.empty() == false) {
//                response.append(temp);
//            }
//
//            input = response;
//
//        }//exit replace_all function

        static std::vector<std::string> splitByWhiteSpace(std::string sentence) {
            using namespace std;
            std::vector<string> myvector;
            if (sentence.empty()) {
                return myvector;
            }

            istringstream iss(sentence);
            myvector = vector<string>(istream_iterator<string>(iss), istream_iterator<string>());

            if (myvector.empty()) {
                myvector.push_back(sentence);
            }
            return myvector;
        }

        inline std::string allSameChar(std::string input, char type) {
            std::string response;
            for (unsigned int i = 0; i < input.size(); i++) {
                response += type;
            }
            response += type;

            return response;
        }

        inline std::string allSameChar(int max, char type) {
            std::string response;
            for (int i = 0; i < max; i++) {
                response += type;
            }
            response += type;

            return response;
        }

        inline static std::string getAllAfterChar(std::string input, char delimiter) {


            std::string response;
            bool allowPush = false;

            for (unsigned int i = 0; i < input.size(); i++) {
                if (input.at(i) == delimiter && allowPush == false) {
                    allowPush = true;
                }
                else if (allowPush) {
                    response += input.at(i);
                }
            }

            return response;
        }

        inline static std::vector<std::string> splitByChar(std::string input, const char delimiter) {
            std::vector<std::string> response;
            std::string temp;

            for (unsigned int i = 0; i < input.size(); i++) {
                if (input.at(i) == delimiter) {
                    response.push_back(temp);
                    temp.clear();
                } else {
                    temp += input.at(i);
                }
            }

            if (!temp.empty()) {
                response.push_back(temp);
            }
            return response;

        }

        inline static bool charIsNumber(char c) {
            return ((c >= '0' && c <= '9') || c == '.' || c == '-');
        }

        inline static bool stringIsNumber(std::string str) {
            str = trim(str);
            if (str.empty()) {
                return false;
            }
            for (unsigned i = 0; i < str.length(); ++i) {
                if (!charIsNumber(str.at(i))) {
                    return false;
                }
            }

            return true;
        }

        inline static std::string intToString(int n) {
            std::stringstream ss;
            ss << n;
            return ss.str();
        }

        inline static std::string pathRelativeToAbsolute(std::string input, std::string exeName, const char osDelimiter = CHAR_OS_SEPARATOR) {

            using namespace std;

            vector<string> dataOne = splitByChar(input, osDelimiter);
            vector<string> dataTwo = splitByChar(exeName, osDelimiter);
            string response;
            int numBacks = 0;
            unsigned int i;

            for (i = 0; i < dataOne.size(); i++) {
                if (dataOne.at(i) == "..") {
                    numBacks++;
                };
            };

            for (i = 0; i < dataTwo.size()-(numBacks + 1); i++) {
                response.append(dataTwo.at(i));
                response += osDelimiter;
            };

            for (i = numBacks; i < dataOne.size() - 1; i++) {
                response.append(dataOne.at(i));
                response += osDelimiter;
            };

            response.append(dataOne.at(dataOne.size() - 1));

            return response;
        }

        static inline void removeClosingComments(std::string& source) {
            while (source.find("/*") != std::string::npos) {
                size_t Beg = source.find("/*");
                source.erase(Beg, (source.find("*/", Beg) - Beg) + 2);
            }
        }

        static inline void removeInlineComments(std::string& source) {
            while (source.find("//") != std::string::npos) {
                size_t Beg = source.find("//");
                source.erase(Beg, source.find("\n", Beg) - Beg);
            }
        };

        inline static std::string currentDateTime(const char * pattern = "%Y_%m_%d_%H_%M_%S") {
            std::string response;
            std::time_t rawtime;
            char buffer [80];
    #ifdef ASTOX_OS_WINDOWS
            std::tm timeinfo;
            rawtime = time(0);
            localtime_s(&timeinfo, &rawtime);
            std::strftime(buffer, 80, pattern, &timeinfo);
    #else
            std::tm* timeinfo;
            std::time(&rawtime);
            timeinfo = std::localtime(&rawtime);
            std::strftime(buffer, 80, pattern, timeinfo);
    #endif
            response += buffer;
            return response;
        }

        inline static std::string replaceWhiteSpace(std::string sentence, const char * delimiter = " ") {

            using namespace std;
            sentence = astox::stringAlg::trim(sentence);
            std::string response;
            if (sentence.empty()) {
                //throwWarning("unable to replace empty input ", __LINE__, __FILENAME__);
                return response;
            }

            std::vector<string> myvector;

            std::stringstream ss;
            istringstream iss(sentence);
            copy(istream_iterator<string>(iss), std::istream_iterator<std::string>(), std::back_inserter(myvector));
            copy(myvector.begin(), myvector.end() - 1, ostream_iterator<string>(ss, delimiter));
            ss << myvector.at(myvector.size() - 1);
            return ss.str();
        }

        static inline void getBetweenDelimiters(std::string inputFile, std::string start, std::string end, std::vector<std::string>& pushable) {

            std::string tempFile;
            std::string insideStr;

            inputFile = astox::stringAlg::replaceWhiteSpace(inputFile);

            size_t first = inputFile.find(start);
            size_t last;


            while (inputFile.find(start) != std::string::npos) {
                first = inputFile.find(start);
                last = inputFile.find(end);

                if (last != std::string::npos) {
                    std::string strNew = inputFile.substr(first + start.size(), last - first - end.length());
                    pushable.push_back(strNew);
                    std::cout << "found : " << strNew << std::endl;
                    inputFile = inputFile.substr(last, inputFile.size());
                    std::cout << "rest: " << inputFile << std::endl;
                }

                //		inputFile = inputFile.erase(first, (inputFile.find(end, first) - first)+end.size());


            }

        }

        inline static std::vector<std::string> split(const std::string& s, const std::string& delim) {
            std::vector<std::string> result;
            if (delim.empty()) {
                result.push_back(s);
                return result;
            }
            std::string::const_iterator substart = s.begin(), subend;
            while (true) {
                subend = search(substart, s.end(), delim.begin(), delim.end());
                std::string temp(substart, subend);
                if (!temp.empty()) {
                    result.push_back(temp);
                }
                if (subend == s.end()) {
                    break;
                }
                substart = subend + delim.size();
            }
            return result;
        }//exit split function

        inline static std::string getFirstTillWhitespace(std::string input) {
            if (!input.empty()) {
                std::vector<std::string> myvector;
                myvector = astox::stringAlg::splitByWhiteSpace(input);
                return myvector.at(0);
            }
            return "";
        }

        /*
        template<class OutIter>
        OutIter escapeStringQuotes(std::string const& s) {
                OutIter out;
         *out++ = '"';
          for (std::string::const_iterator i = s.begin(), end = s.end(); i != end; ++i) {
            unsigned char c = *i;
            if (' ' <= c and c <= '~' and c != '\\' and c != '"') {
         *out++ = c;
            }
            else {
         *out++ = '\\';
              switch(c) {
              case '"':  *out++ = '"';  break;
              case '\\': *out++ = '\\'; break;
              case '\t': *out++ = 't';  break;
              case '\r': *out++ = 'r';  break;
              case '\n': *out++ = 'n';  break;
              default:
                char const* const hexdig = "0123456789ABCDEF";
         *out++ = 'x';
         *out++ = hexdig[c >> 4];
         *out++ = hexdig[c & 0xF];
              }
            }
          }
         *out++ = '"';
          return out;
        }
         */




        static inline bool stringIsInList(std::string searchable, std::vector<std::string> lista) {
            for (int i = 0; i < (int) lista.size(); i++) {
                if (searchable == lista.at(i)) {
                    return true;
                }
            }
            return false;
        }

        //TODO una de trimQuotes, alta de escape

        inline static std::string escapeStringQuotes(std::string input) {

            if (input.size() > 1) {
                if (input[0] == '\"' && input[input.length() - 1] == '\"') {
                    input = input.substr(1, input.length() - 2);
                }
                if (input[0] == '"' && input[input.length() - 1] == '"') {
                    input = input.substr(1, input.length() - 2);
                }

                if (input[0] == '\'' && input[input.length() - 1] == '\'') {
                    input = input.substr(1, input.length() - 2);
                }
            }


            return input;
        }

        inline static std::string doAstoxTimePattern(std::string input) {
            std::string result = input;
            astox::util::replace(result, "{astoxDate}", astox::stringAlg::currentDateTime("%c"));
            //	replace_all(result, "{astoxDate}", "asdsd");
            return result;
        }

        static inline bool stringIsWhiteSpaced(std::string &in) {
            in = astox::stringAlg::trim(in);
            return in.empty();
        }




    }//exit namespace stringAlg


}



#endif /* STRINGALG_H_ */
