#include "astox/Assert.h"
#include "astox/EcmaMethods.h"
#include "astox/Autoconf.h"

using namespace astox;
using namespace std;



void loadbar(unsigned int x, unsigned int n, unsigned int w = 50) {
    if ((x != n) && (x % (n / 100 + 1) != 0)) return;

    float ratio = x / (float) n;
    int c = ratio * w;

    cout << setw(3) << (int) (ratio * 100) << "% [";
    for (int x = 0; x < c; x++) cout << "=";
    for (int x = c; x < w; x++) cout << " ";
    cout << "]\r" << flush;
}


Value * test(Value * object) {
    printf("calling callback2");
    cout << lang::toJSON(object) << "2" << endl;
    return new String("done");
};

Value * test2(Value * object) {
    printf("calling callback");
    cout << lang::toJSON(object) << endl;
    return new String("done");
};

std::string convertAlg(double dbl) {

    //	do {
    //	    int digit = (dbl % 10);
    //	    putchar('0' + digit);
    //	    dbl /= 10;
    //	} while (dbl > 0);

    int decimal_point = static_cast<int> (dbl * 10) % 10;
    std::cout << "number " << dbl << std::endl;
    std::cout << "decimal_point" << decimal_point << " as s" << std::endl;

    std::ostringstream strs;
    strs << std::setprecision(18) << dbl;
    std::string str = strs.str();
    printf("%s\n", str.c_str());

    return str;
    //	char str[50];
    //	sprintf(str,"%lf", dbl);
    //	return str;
}

void ASTX_EXPECT_NUMBER_TO_STRING(double d, std::string value) {

    std::string converted = convertAlg(d);
    //	if(value == converted) {
    //		printf("--- %lf = %s SUCCESS\n", d, converted.c_str());
    //	} else {
    //		printf("--- %lf != %s FAIL\n", d, converted.c_str());
    //	}


}

void ASTX_TEST_NUMBERS() {
    ASTX_EXPECT_NUMBER_TO_STRING(20.0, "20");
    ASTX_EXPECT_NUMBER_TO_STRING(98484894894984984984.0, "20");
    ASTX_EXPECT_NUMBER_TO_STRING(3.141592653589793, "3.141592653589793");
}






int main(int argc, char* argv[]) {

    std::string serviceScriptEnd = "-serviceScript.js";


using namespace astox::testing;
astox::testing::run_all();



    if (argc == 1) {
        printf("no arguments provided, type \"--help\" or \"-h\" to see usage\n");
        return 0;
    }

    if (argc > 0) {
        if (string(argv[1]) == "--help" || string(argv[1]) == "-h") {
            printf(" --service       (-s) ... specify a service name to run (use \"--service-dbg\" to see method list)\n"
                    " --help          (-h) ... display help info\n"
                    " --info          (-i) ... display astox_info data\n"
                    " --self-config        ... run the self configure tool with optional args:\n"
                    "   --src-dir          ... path to source directory containing at least the \"astox\" source folder\n"
                    "   --check-sys        ... default is true, type false to ovveride default behaviour\n"
                    "   --scan-dirs        ... (anything after will be considered scannable directory)\n"
                    " .. run an astox script no matter the extension\n");
            return 0;
        }
    }



    Function * context = ecma::methods::externs::getExternContext();
    std::string input;
    int result = 0;

    if (argc > 1) {
        if (string(argv[1]) == "--service" ||
                string(argv[1]) == "-s" ||
                string(argv[1]) == "--service-dbg") {

            string file = astox::FileSystem::getProcessPath().append(serviceScriptEnd);


            if (argc > 2) {

                cout << "file IS: " << file << endl;
                input = ecma::readFile(file);

                input.append("\n  ");
                input.append(argv[2]);
                input.append("();");


                input = ecma::fixInput(input);
                result = ecma::execScript(input, context);
                if (string(argv[1]) == "--service-dbg") {
                    context->debug();
                }
                return result;
            } else if (string(argv[1]) == "--service-dbg") {
                input = ecma::readFile(file);
                input = ecma::fixInput(input);
                result = ecma::execScript(input, context);
                context->debug();
                return result;
            } else {
                printf("service name required, see \"--help\" or \"-h\"\n");
            }
        } else if (string(argv[1]) == "--info" || string(argv[1]) == "-i") {
            printf("%s\n", lang::toJSON(astox::ecma::methods::astox_info(NULL)).c_str());
            return 0;
        } else if (string(argv[1]) == "--self-config") {

            std::string srcDir = "/Users/alexstf/Dropbox/astox/src/";
            bool checkSysFile = true;
            std::vector<std::string> dirlist;

#ifdef ASTOX_OS_LINUX
            printf("init scan for unix enviroment\n");
            dirlist.push_back("/usr/bin");
            dirlist.push_back("/usr/include");
#else
            dirlist.push_back("/Applications");
            dirlist.push_back("/usr");
            dirlist.push_back("/opt");
#endif

            for (int i = 2; i < argc; i++) {

                std::string arg = string(argv[i]);

                if (arg == "--src-dir" && (i + 1) < argc) {
                    srcDir = string(argv[i + 1]);
                    printf("setting up astox source folder at: %s\n", srcDir.c_str());
                }

                if (arg == "--check-sys" && (i + 1) < argc) {
                    checkSysFile = string(argv[i + 1]) == "true" ? true : false;
                    printf("check for existing sys file: %s\n", argv[i + 1]);
                }

                if (arg == "--scan-dirs") {
                    dirlist.clear();
                    printf("using custom dir list:\n");

                    for (int j = (i + 1); j < argc; j++) {
                        dirlist.push_back(argv[j]);
                    }
                }

            }


            astox::SelfConfigure::start(checkSysFile, dirlist);
            astox::SelfConfigure::saveMakefile(srcDir);
            return 0;
        } else {
            printf("run file: %s\n", argv[1]);
              printf("getWorkingDirectory file: %s\n",  FileSystem::getWorkingDirectory().c_str());
            

			 

			 
            
			 
              std::string p = FileSystem::getWorkingDirectory();
              p.append(argv[1]);
            
            input = ecma::readFile(p);
           // input = ecma::readFile(argv[1]);
            input = ecma::fixInput(input);
            ecma::execScript(input, context);
			system("pause");

        }

        return result;
    }

	

    return result;
}
