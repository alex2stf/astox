/*
 * util.h
 *
 *  Created on: Nov 28, 2013
 *      Author: alex2stf
 */

#include "macros.h"








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

	#ifdef ASTOX_REGEX_SUPPORT_POSIX
		//g
		#define JSREG_GLOBAL  REG_EXTENDED
		//i
		#define JSREG_IGNORECASE  REG_ICASE
		//m
		#define JSREG_MULTILINE  REG_NEWLINE
		//n
		#define JSREG_NOSUBS REG_NOSUB
		//u
		#ifndef REG_DUMP
			#define	JSREG_UNICODE	0000
		#else
			#define JSREG_UNICODE  REG_DUMP
		#endif

		//y
		#ifndef REG_PEND
			#define	JSREG_STICKY	0000
		#else
			#define JSREG_STICKY  REG_PEND
		#endif

		#ifndef REG_BASIC
			#define	JSREG_BASIC	0000
		#else
			#define JSREG_BASIC  REG_BASIC
		#endif


		#define JSREG_NONE -3
		#define stx_regex_flag int
	#else
		#define stx_regex_flag std::regex_constants::syntax_option_type
		//g
		#define JSREG_GLOBAL std::regex_constants::grep
		//i
		#define JSREG_IGNORECASE  std::regex_constants::icase
		//m
		#define JSREG_MULTILINE  std::regex_constants::extended
		//n
		#define JSREG_NOSUBS  std::regex_constants::nosubs
		//u
		#define JSREG_UNICODE  std::regex_constants::ECMAScript
		//y
		#define JSREG_STICKY  std::regex_constants::awk
		#define JSREG_BASIC   std::regex_constants::egrep
		#define JSREG_NONE std::regex_constants::awk
	#endif


	struct RegExpMatch {
            RegExpMatch();
            size_t start;
            size_t end;
            stxtr part;
	};


    namespace util {
			//#ifdef ASTOX_OS_WINDOWS
				//LPCWSTR CHAR2LPCWSTR(const char* cs);
			//#endif // ASTIX_OS_WINDOWS

			stxtr replace (std::string in, std::string fnd, std::string rpl);
            bool char_is_whitespace(char c);
            /**
             * determine numerical strings formatted as 123.456
             * @param in
             * @param c dot character
             * @return
             */
            bool string_is_numeric(const std::string &in, char c = '.');



            stxtr regex_replace_matches(std::string input, std::string needle, std::vector<RegExpMatch> &matches, size_t vectorIndex, size_t stringIndex);
			std::vector<RegExpMatch> regex_match_group(std::string input, std::string regexStr, stx_regex_flag rgx_flags = JSREG_GLOBAL);
			stxtr regex_replace_all(std::string input, std::string foundable, std::string needle, stx_regex_flag rgx_flags = JSREG_GLOBAL);
			stxtr read_file_to_string(std::string path);
			void write_string_to_file(std::string inputString, std::string filePath);
    }

    /**
     * @Deprecated
     */
    namespace stringAlg {

    #ifdef UNICODE
        typedef std::wostringstream winapistringstream;
    #else
        typedef std::ostringstream winapistringstream;
    #endif





		const char * ecmaToString(double value);


    #ifdef ASTOX_OS_WINDOWS
        std::wstring stdString2wstring(std::string& s);
        std::wstring string2wstring(const std::string& str);
    #endif

    #ifdef ASTX_ENABLE_NODEJS
        std::string v8Value2StdString(v8::Handle<v8::Value> value);
    #endif

//        static unsigned int iter = 0;
//        static std::string temp = "";
//        static std::string response = "";



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




        static inline std::string doubleToString(double x) {
            std::stringstream s;
            s << x;
            return s.str();
        }//exit double to string conversion





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

        /**
         * @deprecated
         * @param str
         * @return
         */
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
    #ifdef ASTOX_COMPILER_MSC
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
