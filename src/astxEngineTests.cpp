/*
 * unittests.cpp
 *
 *  Created on: Aug 12, 2015
 *      Author: alexstf
 */

#include "astox/Macros.h"
#include <stdlib.h>
#include <stdio.h>
#include "astox/astox.h"
#include "astox/FileSystem.h"
#include "astox/Thread.h"
#include "astox/MinGUI.h"
#include "astox/Socket.h"
#include "astox/EcmaMethods.h"
#include <ctime>



using namespace astox;
using namespace std;

static astox::Function * unitTestContext = new astox::Function();

static int __numtst = 0;

static int __unitTestContext_dbg_defined = 0;
extern int _ecmscriptExpectIndex = -1;

static std::vector<std::string> _ecmscriptExpectsPrint;

Value * print_expect(Value* arguments) {

    __numtst++;
    string given;

    if (arguments->getType() == ASTOX_VALUE_STRING) {
        given = arguments->toString();
    }
    else if (arguments->getType() == ASTOX_VALUE_CUSTOM || arguments->getType() == ASTOX_VALUE_REGEXP || arguments->getType() == ASTOX_VALUE_DATE) {
        given = arguments->toString();
    }
    else {
        given = lang::toJSON(arguments, "", "");
    }

    astox_console_write(FG_YELLOW_2, "%i) ", __numtst);
    astox_console_write(FG_BLUE_2, "given %s (%i ch)", given.c_str(), given.size());


    if (_ecmscriptExpectIndex < _ecmscriptExpectsPrint.size()) {
        string expect = _ecmscriptExpectsPrint.at(_ecmscriptExpectIndex);
        astox_console_write(FG_TURQOISE_1, "expect %s (%i ch)", expect.c_str(), expect.size());
        if (given == expect) {

            astox_console_write(FG_GREEN_1, "SUCCESS\n");
        }
        else {
            astox_console_write(BG_RED_2, "FAIL\n");
            astox_system_pause();
            std::exit(34);
        }
    }
    else {
        astox_console_write(FG_RED_1, " INCOMPATIBLE EXPECTS\n");
        exit(1);
    }

    _ecmscriptExpectIndex++;
    return Value::npos;
}

void ASTX_TEST_REGEXFLAG(AstoxRegexFlag in, AstoxRegexFlag expect) {
    __numtst++;
    if (in == expect) {
        cout << in << " == " << expect << " SUCCESS" << endl;
    }
    else {
        cout << in << " != " << expect << " FAIL" << endl;
    }
}

void ASTX_TEST_ECMASCRIPT(std::string input, std::vector<std::string> expects, bool refreshContext = false, std::string message = "") {

    if (__unitTestContext_dbg_defined == 0) {
        unitTestContext = Function::getEcmaContext();
        unitTestContext->addKey("print", new Function(print_expect));
        __unitTestContext_dbg_defined++;
    }

    printf("ASTX_TEST_ECMASCRIPT with message \"%s\"\n                     input: (%s)\n", message.c_str(), input.c_str());

    _ecmscriptExpectsPrint.clear();
    _ecmscriptExpectsPrint = expects;
    _ecmscriptExpectIndex = 0;

    ecma::execScript(input, unitTestContext);
}

void ASTX_TEST(bool expect, const char * message) {
    __numtst++;
    printf("%i) %s\n", __numtst, message);

    if (expect) {
        astox_console_write(BG_GREEN_1, "%i) %s\n", __numtst, message);
        return;
    } else {
        astox_console_write(BG_RED_1, "%i) %s\n", __numtst, message);
    }
}

void ASX_TEST_STDSTR(std::string in, std::string expect, std::string message = "") {
    __numtst++;
    if (in == expect) {

        if (!message.empty()) {
            astox_console_write(BG_GREEN_1, "SUCCESS! in: %s, expect: %s (%s)\n", in.c_str(), expect.c_str(), message.c_str());
        } else {
            astox_console_write(BG_GREEN_1, "SUCCESS! in: %s, expect: %s\n", in.c_str(), expect.c_str());
        }
        return;
    }
    astox_console_write(BG_RED_1, "FAIL !!! in: %s , expect: %s\n", in.c_str(), expect.c_str());

    std::exit(0);
}

void ASTX_TEST(int in, int expect, std::string message = "") {
    __numtst++;
    if (in == expect) {
        printf("%i) SUCCESS! in: %i , expect: %i  (%s)\n", __numtst, in, expect, message.c_str());
        return;
    }
    printf("%i) FAIL !!! in: %i , expect: %i (%s)\n", __numtst, in, expect, message.c_str());
}

void ASTX_TEST_SSCRIPT_EXTRACT_PATTERN(std::string command, std::vector<SelectorNode *> expect) {
    __numtst++;
    astox_console_write(BG_BLUE_1, "%i) TEST ecma::extractPatternList(\"%s\")\n", __numtst, command.c_str());
    std::vector<SelectorNode> data;
    data = ecma::extractPatternList(command);
    if (expect.size() != data.size()) {
        astox_console_write(BG_RED_1, "first level failed\n");
        astox_system_pause();

    }

    for (unsigned int i = 0; i < data.size(); i++) {
        if (data.at(i).data == expect.at(i)->data) {
            if (data.at(i).type == expect.at(i)->type) {
                astox_console_write(BG_GREEN_1, " PASSED AT INDEX %i,\n", i);
                astox_console_write(BG_GREEN_2, "    expected %i) %s,\n", expect.at(i)->type, expect.at(i)->data.c_str());
                astox_console_write(BG_GREEN_2, "    given    %i) %s,\n", data.at(i).type, data.at(i).data.c_str());
            } else {
                astox_console_write(BG_RED_1, " FAILED AT INDEX %i, (expected %i, given %i) \n", i, expect.at(i)->type, data.at(i).type);
                astox_system_pause();
            }
        } else {
            astox_console_write(BG_RED_1, " FAILED AT INDEX %i, (expected %s, given %s)\n", i, expect.at(i)->data.c_str(), data.at(i).data.c_str());
            astox_system_pause();

        }
    }


}

void ASTX_TEST_CONSOLE_COLORS() {
    for (unsigned int i = 0; i < (sizeof (astox_console_colors) / sizeof (*astox_console_colors)); i++) {
        astox_console_write(astox_console_colors[i],
                "astox_console_colors[%i] = %i\n", i, astox_console_colors[i]);
    }
}

void ASTX_TEST_SSCRIPT(std::string command, double expect) {

    __numtst++;
    astox_console_write(FG_BLUE_1, "%i) NUMERIC TEST START: %s\n", __numtst, command.c_str());
    int start_s = clock();

    Value * valr = ecma::runCommand(command, unitTestContext);
    double result = valr->toDouble();
    int stop_s = clock();
    int execTime = (stop_s - start_s) / double(CLOCKS_PER_SEC)*1000;
    if (result == expect) {
        astox_console_write(FG_GREEN_2, "NUMERIC TEST PASSED: %s = %s (%i ms)\n",
                command.c_str(), astox::lang::toJSON(valr, "", "").c_str(), execTime);
    }
    else {
        astox_console_write(FG_RED_2, "NUMERIC TEST FAILED: %s ( %i != %s ) \n",
                command.c_str(), expect,
                astox::lang::toJSON(valr, "", "").c_str());
        astox_system_pause()

    }
    //cout << "time: " << << endl;
}

void ASTX_TEST_SSCRIPT_STR(std::string command, std::string expect) {

    __numtst++;
    astox_console_write(FG_BLUE_1, "%i) STRING TEST START: %s\n", __numtst, command.c_str());
    int start_s = clock();
    Value * val = ecma::runCommand(command, unitTestContext);
    string result = val->toString();
    int stop_s = clock();
    int execTime = (stop_s - start_s) / double(CLOCKS_PER_SEC)*1000;

    if (result == expect) {
        astox_console_write(FG_GREEN_2, "STRING TEST PASSED: %s = %s (%i ms)\n",
                command.c_str(), astox::lang::toJSON(val, "", "").c_str(), execTime);
    } else {
        const char * jsonized = astox::lang::toJSON(val, "", "").c_str();
        const char * commands = command.c_str();
        astox_console_write(FG_RED_2, "STRING TEST FAILED: %s ( %s != %s ) \n", commands, expect.c_str(), jsonized);

        astox_system_pause();
        delete jsonized;
    }
}

void ASTX_TEST_SSCRIPT_BOOL(std::string command, bool expect) {
    __numtst++;
    int start_s = clock();
    Value * valr = ecma::runCommand(command, unitTestContext);
    astox_console_write(FG_BLUE_2, "%i) BOOLEAN TEST START: %s\n", __numtst, command.c_str());
    bool result = valr->toBoolean();
    int stop_s = clock();
    int execTime = (stop_s - start_s) / double(CLOCKS_PER_SEC)*1000;
    if (result == expect && valr->getType() == ASTOX_VALUE_BOOL) {
        astox_console_write(FG_GREEN_2, "BOOLEAN TEST PASSED: %s = %s (%i ms)\n",
                command.c_str(), astox::lang::toJSON(valr, "", "").c_str(), execTime);
    } else {
        Boolean b(expect);
        string tstr = b.toString();
        astox_console_write(FG_RED_2, "BOOLEAN TEST FAILED: %s != %s \n",
                command.c_str(), astox::lang::toJSON(valr, "", "").c_str());
        astox_system_pause()

    }
}

void ASTX_TEST_SSCRIPT(std::string command,
        astox::Value * expect = astox::Value::npos) {

    Value* result = ecma::runCommand(command, unitTestContext);
    unitTestContext->debug();
    cout << " --- command: ( " << command << ") = " << astox::lang::toJSON(result, "", "") << endl;
}

void ASTX_TEST_FIX_INPUT(std::string in, std::string expect) {
    __numtst++;
    cout << __numtst << ") in: (" << in << ")" << endl;
    string data = ecma::fixInput(in);

    if (data == expect) {
        cout << "SUCCESS" << endl;
    } else {
        cout << "FAILURE" << endl;
    }
    cout << data << endl;

}

void ASTX_TEST_DETECT_CLOSURE(std::string in, std::string expect, char closures[2], bool reversed = false) {
    __numtst++;
    int num = 0;
    string buff;
    for (unsigned int i = 0; i < in.size(); i++) {
        char c = in.at(i);

        bool allow = ecma::charIsEnclosedBy(c, num, closures[0], closures[1]);
        if (reversed) {
            allow = !allow;
        }

        if (allow) {
            buff += c;
        }
    }

    if (buff == expect) {
        cout << "SUCCESS" << endl;
    } else {
        cout << "FAILURE" << endl;
    }


    cout << "buff= " << buff << endl;
}

void ASTX_TEST_DETECT_KEYWORD(std::string data, int epr, int epl, int etype, std::string beforeKeyword = "") {
    __numtst++;
    using namespace ecma;
    ecma::fixInput(data);

    printf("%i) ASTX_TEXT_DETECT_KEYWORD \n", __numtst);
    printf("   input string: [%s]\n", data.c_str());

    string beforeBuff;
    int foundLeft = 0;
    int foundRight = 0;
    int type = ecma::ASTXKT_NONE;
    ecma::detectKeyword(data, beforeBuff, type, foundLeft, foundRight);

    if (foundLeft == epl) {
        printf("   position left success: %i ", foundLeft);
    } else {
        printf("   position left FAILED: expected %i found %i", epl, foundLeft);
    }
    if (foundRight == epr) {
        printf("   position right success: %i ", foundRight);
    } else {
        printf("   position right FAILED: expected %i found %i", epr, foundRight);
    }
    if (type == etype) {
        printf("   expected type success: %i \n", type);
    } else {
        printf("   expected type FAILED: expected %i found %i\n", etype, type);
    }

}
#include <locale>
#define ASTX_UNICODE_TEST_CHARS "¥¤£¬±¶ÇÈ"

void ASTX_SUITE_ARRAY_METHODS() {
    Array * lit = new Array();
    lit->push(Number::New(20));
    lit->push(Number::New(40));
    lit->push(Number::New(5));
    lit->push(Number::New(15));

    lit->push(new String("abc"));
    lit->push(new Boolean(false));
    lit->push(new Boolean(true));
    lit->push(Number::New(11));
    int n = 0;
    lit->push(Number::New(n));
    cout << "original: " << lang::toJSON(lit, "", "") << endl;
    lit->reverse();
    cout << "reversed: " << lang::toJSON(lit, "", "") << endl;
    lit->sort();
    cout << "sorted  : " << lang::toJSON(lit, "", "") << endl;
    Value * popped = lit->pop();
    cout << "popped  :[" << lang::toJSON(popped) << "] from " << lang::toJSON(lit, "", "") << endl;
    Value * shifted = lit->shift();
    cout << "shiftd  :[" << lang::toJSON(shifted) << "] from " << lang::toJSON(lit, "", "") << endl;

    lit = new Array();
    lit->push(Number::New(14));
    lit->push(Number::New(3));
    lit->push(Number::New(77));

    Array * spliceargs = new Array();
    spliceargs->push(Number::New(1));
    spliceargs->push(Number::New(2));
    Value * sliced = lit->slice(spliceargs);

    cout << "sliced(" << spliceargs->join(",")->toString() << ")" << "  :|" << lang::toJSON(sliced, "", "") << "| from " << lang::toJSON(lit, "", "") << endl;
    int zr = 0;
    spliceargs->updateValue("0", Number::New(zr));
    sliced = lit->slice(spliceargs);

    cout << "sliced(" << spliceargs->join(",")->toString() << ")" << "  :|" << lang::toJSON(sliced, "", "") << "| from " << lang::toJSON(lit, "", "") << endl;

    lit = Array::cast(ecma::fromStr("[14, 3, 77, 55, 78]", Function::npos, false));
    cout << "spliced(1, 2)" << "  :|" << lang::toJSON(lit->slice(1, 2), "", "") << "| from " << lang::toJSON(lit, "", "") << endl;
    cout << "spliced(1, 2)" << "  :|" << lang::toJSON(lit->splice(1, 2), "", "");
    cout << "| from " << lang::toJSON(lit, "", "") << endl;

}

void ASTX_SUITE_OPERATORS() {
    printf(" The following suite should thow compilation errors\n");

    Object fromStr = "{a:1, b:2, c:false, str:string, d : undefined long string}";
    cout << "fromStr{} " << fromStr << endl;

    Array fromStrArr = "[1, 2, string long with spaces, false]";
    cout << "fromStrArr[] " << fromStrArr << endl;

    Value * p_value = new Value();
    cout << "p_value: " << p_value << endl;

    Value r_value;
    cout << "r_value: " << r_value << endl;

    String * p_string = new String("dummy");
    cout << "p_string: " << p_string << endl;

    String r_string("reference");
    cout << "r_string: " << r_string << endl;

    Number * p_number = Number::New("27112004");
    cout << "p_number: " << p_number << endl;


    Number r_number(-455.5);
    cout << "r_number: " << r_number << endl;
    r_number++;
    cout << "r_number++ " << r_number << endl;
    ++r_number;
    cout << "++r_number " << r_number << endl;
    r_number--;
    cout << "r_number-- " << r_number << endl;
    r_number--;
    cout << "r_number-- " << r_number << endl;
    r_number--;
    cout << "r_number-- " << r_number << endl;
    --r_number;
    cout << "--r_number: " << r_number << endl;
    --r_number;
    cout << "--r_number: " << r_number << endl;
    --r_number;
    cout << "--r_number: " << r_number << endl;



    Number data = 2;
    cout << "Number data = " << data << endl;
    data + 2;
    cout << "      data+2: " << data << endl;
    data - 1;
    cout << "      data-1: " << data << endl;
    data * 2;
    cout << "      data*2: " << data << endl;
    data / 2;
    cout << "      data/2: " << data << endl;
    cout << "      data+r_number " << data + r_number << endl;
    data += 2;
    cout << "      data+=2; " << data << endl;
    data -= 2;
    cout << "      data-=2 " << data << endl;
    data *= 2;
    cout << "      data*=2 " << data << endl;
    data /= 2;
    cout << "      data/=2 " << data << endl;
    cout << "      data!=r_number " << (data != r_number) << endl;
    cout << "      data==r_number " << (data == r_number) << endl;
    cout << "      data<=r_number " << (data <= r_number) << endl;
    cout << "      data>=r_number " << (data >= r_number) << endl;

    Number dumnr = 3;
    cout << "Number dumnr = " << data << endl;
    cout << "      data!=dumnr; " << (data != dumnr) << endl;
    cout << "      data==dumnr; " << (data == dumnr) << endl;

    cout << "      data>dumnr; " << (data > dumnr) << endl;
    cout << "      data<dumnr; " << (data < dumnr) << endl;

    Number dataNegative = -23344.455;
    cout << "Number dataNegative = " << dataNegative << endl;

    double vald = 3455;
    cout << "double vald = 3455;" << endl;
    Number dVal = vald;
    cout << "Number dVal = vald;" << dVal << endl;

    cout << "dumnr+dval " << dumnr + dVal << endl;
    cout << "dumnr-dval " << dumnr - dVal << endl;

    cout << "dumnr*dval " << dumnr * dVal << endl;
    cout << "dumnr/dval " << dumnr / dVal << endl;

    Boolean * p_bool = new Boolean(true);
    cout << "p_bool: " << p_bool << endl;

    Boolean * p_bool_false = new Boolean(false);
    cout << "p_bool_false: " << p_bool_false << endl;
    cout << "Boolean::btrue = " << Boolean::BTRUE << endl;
    cout << "Boolean::bfalse = " << Boolean::BFALSE << endl;

    Boolean r_bool(false);
    cout << "r_bool: " << r_bool << endl;

    Array * p_arr = new Array();
    cout << "p_arr: " << p_arr << endl;

    Array r_arr;
    cout << "r_arr: " << r_arr << endl;

    Object * p_object = new Object();
    p_object->addKey("number", p_number);
    p_object->addKey("array", p_arr);
    cout << "p_object: " << p_object << endl;

    Object r_obj;
    r_obj.addKey("string", &r_string);
    r_obj.addKey("inheritObject", p_object);
    cout << "r_object: " << r_obj << endl;


#ifdef ASTX_ENABLE_QT
    cout << "QString supported" << endl;
    QString qstring1 = "qstring1";
    String qstr1(qstring1);
    cout << "qstr1: " << qstr1 << endl;

    QString * qstring2 = new QString("asdasdas");
    String * qstr2 = new String(qstring2);
    cout << "qstr2: " << qstr2 << endl;
#endif


    String strOperators = "abcdefghijklmnopqrstuvxyz";
    cout << "strOperators: " << strOperators << endl;
    strOperators++;
    cout << "strOperators++: " << strOperators << endl;
    strOperators++;
    cout << "strOperators++: " << strOperators << endl;
    for (int i = 0; i < 5; i++) {
        strOperators--;
        cout << "strOperators--: " << strOperators << endl;
    }

    strOperators.reverse();

    cout << "strOperators.reverse();  " << strOperators << endl;

    for (int i = 0; i < 5; i++) {
        strOperators--;
        cout << "strOperators--: " << strOperators << endl;
    }


    strOperators.reverse();

    //strOperators+='_';
    strOperators += "|endOfString";

    std::string f = "std::string";
    strOperators += f;

    String temp = ".....<><><><>";
    cout << "String temp = \".....<><><><>\";" << endl;
    Number nums(20);
    cout << "Number nums(20);" << endl;
    String n2(456.455);
    cout << "String n2(456.455);" << endl;
    n2 += nums;
    cout << "n2+=nums;" << endl;
    strOperators += nums;
    cout << "strOperators+= nums; strOperators+= n2 => " << strOperators << endl;
    strOperators += n2;
    cout << "strOperators+temp = " << strOperators + temp << endl;
}

void ASTX_SUITE_ECMA_SELECTOR_NODES() {
    std::vector<SelectorNode *> expect;
    expect.push_back(new SelectorNode("object", ST_BASIC_STR));
    expect.push_back(new SelectorNode("g", ST_BASIC_STR));
    expect.push_back(new SelectorNode("1 + 1", ST_EVAL));
    expect.push_back(new SelectorNode("4 + 5", ST_EVAL));
    expect.push_back(new SelectorNode("arguments", ST_ARG));
    ASTX_TEST_SSCRIPT_EXTRACT_PATTERN("object.g[1 + 1][4 + 5](arguments)", expect);

    expect.clear();

    expect.push_back(new SelectorNode("print", ST_BASIC_STR));
    expect.push_back(new SelectorNode("list.length, \"some string\"", ST_ARG));
    ASTX_TEST_SSCRIPT_EXTRACT_PATTERN("print(list.length, \"some string\");", expect);

    expect.clear();

    expect.push_back(new SelectorNode("print", ST_BASIC_STR));
    expect.push_back(new SelectorNode("prop", ST_BASIC_STR));
    expect.push_back(new SelectorNode("data", ST_BASIC_STR));
    ASTX_TEST_SSCRIPT_EXTRACT_PATTERN("print.prop.data", expect);


    expect.clear();

    expect.push_back(new SelectorNode("print", ST_BASIC_STR));
    expect.push_back(new SelectorNode("prop", ST_EVAL));
    expect.push_back(new SelectorNode("data", ST_BASIC_STR));
    ASTX_TEST_SSCRIPT_EXTRACT_PATTERN("print[prop].data", expect);

    expect.clear();

    expect.push_back(new SelectorNode("print", ST_BASIC_STR));
    expect.push_back(new SelectorNode("4", ST_NUMERIC));
    expect.push_back(new SelectorNode("data", ST_BASIC_STR));
    ASTX_TEST_SSCRIPT_EXTRACT_PATTERN("print[4].data", expect);

    expect.clear();

    expect.push_back(new SelectorNode("print", ST_BASIC_STR));
    expect.push_back(new SelectorNode("caller()[1]", ST_EVAL));
    expect.push_back(new SelectorNode("caller[2].prop", ST_EVAL));
    expect.push_back(new SelectorNode("do", ST_BASIC_STR));
    expect.push_back(new SelectorNode("arguments[call(4)]", ST_ARG));
    ASTX_TEST_SSCRIPT_EXTRACT_PATTERN("print[caller()[1]][caller[2].prop]do(arguments[call(4)])", expect);
}

void ASTX_SUITE_ASSIGN_ERRORS() {
    ASTX_TEST_SSCRIPT("true = false");
    ASTX_TEST_SSCRIPT("5 = 2");
    ASTX_TEST_SSCRIPT("\"string\" = 2");
}

void ASTX_SUITE_CONTEXT_ALLOC() {
    ASTX_TEST_SSCRIPT("a = b = c = number = d = f = 5", 5);
    ASTX_TEST_SSCRIPT_BOOL("f = 5 == 5", true);
    ASTX_TEST_SSCRIPT_BOOL("g=\"5\"!=5", false);
    ASTX_TEST_SSCRIPT_BOOL("h=\"5\"==5", true);
    ASTX_TEST_SSCRIPT_BOOL("i=\"5\"!==5", false);
    ASTX_TEST_SSCRIPT_BOOL("j=\"5\"===5", false);
    ASTX_TEST_SSCRIPT_BOOL("k=\"6\"==3*2", true);
    ASTX_TEST_SSCRIPT("object = {a: 1, b: \"some_string\", g:[1, 2, 3, 5, true, {a: b}, 5], d: { inh: {a:false} } }");
    ASTX_TEST_SSCRIPT("strVal=\"inh\"");
    ASTX_TEST_SSCRIPT_STR("strVal", "inh");
    ASTX_TEST_SSCRIPT_BOOL("object.d.inh.a", false);
    ASTX_TEST_SSCRIPT_BOOL("object.d['inh']a", false);
    ASTX_TEST_SSCRIPT_BOOL("object.d['inh'].a", false);
    ASTX_TEST_SSCRIPT_BOOL("object.d[strVal].a", false);
    ASTX_TEST_SSCRIPT_BOOL("object.d[strVal]a", false);
    ASTX_TEST_SSCRIPT("object.g[0]", 1);
    ASTX_TEST_SSCRIPT("object.g[\"1\"]", 2);
    ASTX_TEST_SSCRIPT_BOOL("object.g[2 * 2]", true);
    ASTX_TEST_SSCRIPT("object.g[0] = false");
    ASTX_TEST_SSCRIPT("object.g[1+2] = {more: object}");
    ASTX_TEST_SSCRIPT("print()");
    ASTX_TEST_SSCRIPT("print(object)");
    ASTX_TEST_SSCRIPT("print(2)");
    ASTX_TEST_SSCRIPT("print(2+3)");
    ASTX_TEST_SSCRIPT("print(2*3)");
}

void ASTX_SUITE_ECMA_WITHOUT_CONTEXT() {
    Value * testObject =
            astox::ecma::fromStr(
            "{a:1, b: 2, c: {}, d: [], e: simple string, "
            "f: 9833838383837363536778838, g: 23.838373639, "
            "h: 85.3222, i: true, j: false,"
            "k: {a: 23, b: [1, 2, true, other string, {object: inherited}]},"
            "m: [{}, {}, [], [{data: [{a:1}, {}]}]]"
            "}");
    cout << astox::lang::toJSON(testObject) << endl;
}

void ASTX_SUITE_SSCRIPT_NUMERIC() {
    ASTX_TEST_SSCRIPT("2", 2);
    ASTX_TEST_SSCRIPT("2 | 5 | 8", 15);
    ASTX_TEST_SSCRIPT("2|5", 7);
    ASTX_TEST_SSCRIPT("494847", 494847);
    ASTX_TEST_SSCRIPT("-2 + 1", -1);
    ASTX_TEST_SSCRIPT("25 % 3 + 4 % 5 - 2", 3);
    ASTX_TEST_SSCRIPT("7 * 2 / 2 - 3 % 5", 4);
    ASTX_TEST_SSCRIPT("1 + 2 * 3 / (2 + 1 - 1)", 4);
    ASTX_TEST_SSCRIPT("(1+2) * 2", 6);
    ASTX_TEST_SSCRIPT("1+2*2", 5);
    ASTX_TEST_SSCRIPT("5 + 2 * 3 / 2 + 1 - 1 ", 8);
    ASTX_TEST_SSCRIPT("(5 + 2 * 3) / 2 + 1 - 1", 5.5);
    ASTX_TEST_SSCRIPT("9 - 2 * 2", 5); //merge
    ASTX_TEST_SSCRIPT("23/5*(3)", 13.799999999999999);
    ASTX_TEST_SSCRIPT("1 + 2", 3);
    ASTX_TEST_SSCRIPT("1 + 2 + 3", 6);
    ASTX_TEST_SSCRIPT("9 - (7 + 1)", 1);
    ASTX_TEST_SSCRIPT("9 - 7 + 1", 3);
    ASTX_TEST_SSCRIPT("5 + 2 * 3 / 2 + 1 - 1", 8);
    ASTX_TEST_SSCRIPT("7 % 2", 1);
    ASTX_TEST_SSCRIPT("15 % 2 * 2", 2);
    ASTX_TEST_SSCRIPT("2 * 3 + 5 * 2", 16);
    ASTX_TEST_SSCRIPT("7 * 2 / 2", 7);
    ASTX_TEST_SSCRIPT("7 * 2 / 2 + 3 * 5", 22);
    ASTX_TEST_SSCRIPT("7 * 2 / 2 + 3 % 5", 10);
    ASTX_TEST_SSCRIPT("7 * 2 / 2 - 3 % 5", 4);
    ASTX_TEST_SSCRIPT("25 % (3 + 4) % 5", 4);
    ASTX_TEST_SSCRIPT("25 % (3 + 4) % 5 + 9", 13);
    ASTX_TEST_SSCRIPT("25 % (3 + 4) % (5 + 9)", 4);
    ASTX_TEST_SSCRIPT("25 % 3 + 4 % (5 + 9)", 5);
    ASTX_TEST_SSCRIPT("25 % 3 + 4 % 5", 5);
    ASTX_TEST_SSCRIPT("25", 25);
    ASTX_TEST_SSCRIPT("2+5", 7);
    ASTX_TEST_SSCRIPT("2+5-3", 4);
    ASTX_TEST_SSCRIPT("2<<4", 32);
    ASTX_TEST_SSCRIPT("1+1<<4", 32);
    ASTX_TEST_SSCRIPT("2<<4+1", 64);
    ASTX_TEST_SSCRIPT("2<<4<<3", 256);
    ASTX_TEST_SSCRIPT("2 << 4", 32);
    int nan = 0;
    ASTX_TEST_SSCRIPT("2>>4", nan);
    ASTX_TEST_SSCRIPT("8&&2&&4", (8 && 2 && 4));

    ASTX_TEST_SSCRIPT("a = 4", 4);
    ASTX_TEST_SSCRIPT("a + 2", 6);
    ASTX_TEST_SSCRIPT("a++", 5);
    ASTX_TEST_SSCRIPT("a--", 4);

}

void ASTX_SUITE_SSCRIPT_BOOL() {
    printf("booleans:\n");
    ASTX_TEST_SSCRIPT_BOOL("true", true);
    ASTX_TEST_SSCRIPT_BOOL("false", false);
    printf("basic comparisons:\n");
    ASTX_TEST_SSCRIPT_BOOL("2 > 5", false);
    ASTX_TEST_SSCRIPT_BOOL("1 > 2", false);
    ASTX_TEST_SSCRIPT("45698 > 3627.23");
    ASTX_TEST_SSCRIPT_BOOL("2 > 3", false);
    ASTX_TEST_SSCRIPT_BOOL("2 < 3", true);
    ASTX_TEST_SSCRIPT_BOOL("5 > 2 + 3", false);
    ASTX_TEST_SSCRIPT_BOOL("2<=4", true);
    ASTX_TEST_SSCRIPT_BOOL("2>=4", false);
    printf("shifting:\n");
    ASTX_TEST_SSCRIPT_BOOL("2<<4<<3 > 100", true);
    ASTX_TEST_SSCRIPT_BOOL("2<<4<<3 < 100", false);
    ASTX_TEST_SSCRIPT_BOOL("2<<4<<3 == \"256\"", true);
    ASTX_TEST_SSCRIPT_BOOL("2<<4<<3 === \"256\"", false);
    ASTX_TEST_SSCRIPT_BOOL("2<<4<<3 === 250 + 3 * 2", true);
    ASTX_TEST_SSCRIPT_BOOL("15 % 2 * 2 < 15 % 2 * 2 * 3 % 4", false);
    ASTX_TEST_SSCRIPT_BOOL("15 % 2 * 2 == 15 % 2 * 2 * 3 % 4", true);
    ASTX_TEST_SSCRIPT_BOOL("15 % 2 * 2 === 15 % 2 * 2 * 3 % 4", true);
    ASTX_TEST_SSCRIPT_BOOL("2<<4<<3 <= 256", true);
    ASTX_TEST_SSCRIPT_BOOL("25 % 3 + 4 % (5 + 9) == 5", true);
    ASTX_TEST_SSCRIPT_BOOL("7 * 2  - 3 % 5==11", true);
    ASTX_TEST_SSCRIPT_BOOL("25 % 3 + 4 % (5 + 9) == 5", true);
    ASTX_TEST_SSCRIPT_BOOL("25 % (3 + 4) % (5 + 9) < 2", false);
    ASTX_TEST_SSCRIPT_BOOL("15 % 2 * 2 > 15 % 2 * 2 * 3 % 4", false);
    printf("basic negation:\n");
    ASTX_TEST_SSCRIPT_BOOL("!2", false);
    ASTX_TEST_SSCRIPT_BOOL("!'negate string'", false);
    ASTX_TEST_SSCRIPT_BOOL("!{}", false);
    ASTX_TEST_SSCRIPT_BOOL("!{/t /n}", false);
    ASTX_TEST_SSCRIPT_BOOL("![   ]", false);
    ASTX_TEST_SSCRIPT_BOOL("![ /t /n  ]", false);
    printf("in operator:\n");
    ASTX_TEST_SSCRIPT_BOOL("'a' in {a:1, b:2}", true);
    ASTX_TEST_SSCRIPT_BOOL("'c' in {a:1, b:2}", false);
    printf("and march fixes...\n");
    ASTX_TEST_SSCRIPT_BOOL("1 < 10 && 3 != 2", true);
    ASTX_TEST_SSCRIPT_BOOL("1 < 2 == true", true);
    ASTX_TEST_SSCRIPT_BOOL("1 > 2 < 3", true);
    ASTX_TEST_SSCRIPT_BOOL("1 > 2 > 3", false);
    ASTX_TEST_SSCRIPT_BOOL("true < 3 > 5 < 7 == 9 && 3", false);
}

void ASTX_SUITE_DATE() {
    printf(" test with location required: BUCHAREST");
    Date * date2 = new Date(1986, 1, 1);
    ASTX_TEST(date2->getHours(), 0, "testing date object");
    ASTX_TEST(date2->getUTCHours(), 22, "getUTCHours");
    ASTX_TEST(date2->getTimezoneOffset(), -120);
    ASTX_TEST(date2->getUTCDate(), 31);
    ASTX_TEST(date2->getDate(), 1);
    ASTX_TEST(date2->getDay(), 6);
    ASTX_TEST(date2->getUTCDay(), 5, "new Date(1986, 1, 1).getUTCDay");
    ASTX_TEST(date2->getHourOffset(), 200, "new Date(1986, 1, 1).getHourOffset");
    cout << date2->toString() << "\n" << endl;


    Date * date3 = new Date(2015, 2, 31, 13, 9, 27);
    ASTX_TEST(date3->getTimezoneOffset(), -180);
    ASTX_TEST(date3->getDay(), 2);
    ASTX_TEST(date3->getUTCDay(), 2);
    ASTX_TEST(date3->getDate(), 31);
    ASTX_TEST(date3->getUTCDate(), 31);
    ASTX_TEST(date3->getUTCHours(), 10);
    ASTX_TEST(date3->getHours(), 13);
    ASTX_TEST(date3->getHourOffset(), 300);
    cout << date3->toString() << "\n" << endl;


    Date * date = new Date(1970, 1, 0);
    ASTX_TEST(date->getYear(), 70);
    ASTX_TEST(date->getUTCFullYear(), 1970, "Date(1970, 0, 0).getUTCFullYear = 1969");
    ASTX_TEST(date->getTimezoneOffset(), -120);



    date = new Date(1970, 0, 1);
    ASX_TEST_STDSTR(date->getCommonStr(), "Thu Jan 01 1970 00:00:00");
    date = new Date(1970, 1, 1);
    ASX_TEST_STDSTR(date->getCommonStr(), "Sun Feb 01 1970 00:00:00");
    date = new Date(1970, 0, 0);
    ASX_TEST_STDSTR(date->getCommonStr(), "Wed Dec 31 1969 00:00:00");


    date = new Date(1960, 0, 1, 25, 67, 89, 1000 * 60);
    ASX_TEST_STDSTR(date->getCommonStr(), "Sat Jan 02 1960 02:09:29");
    date = new Date(1960, 0, 1);
    ASX_TEST_STDSTR(date->getCommonStr(), "Fri Jan 01 1960 00:00:00");
    date = new Date(1950, 0, 1);
    ASX_TEST_STDSTR(date->getCommonStr(), "Sun Jan 01 1950 00:00:00");

    date = new Date(1930, 11, 34);
    ASX_TEST_STDSTR(date->getCommonStr(), "Sat Jan 03 1931 00:00:00");

    date = new Date(1930, 1, 1, 1, 1, 1, -1000);
    ASX_TEST_STDSTR(date->getCommonStr(), "Sat Feb 01 1930 01:01:00");



    date = new Date(1930, -1, 1);
    ASX_TEST_STDSTR(date->getCommonStr(), "Sun Dec 01 1929 00:00:00");

    date = new Date(1930, -13, 1);
    ASX_TEST_STDSTR(date->getCommonStr(), "Sat Dec 01 1928 00:00:00");

    date = new Date(1930, 0);
    ASX_TEST_STDSTR(date->getCommonStr(), "Wed Jan 01 1930 00:00:00");

    date = new Date(1930, 0, 0, 0, 0, 0, -1000);
    ASX_TEST_STDSTR(date->getCommonStr(), "Mon Dec 30 1929 23:59:59");

    date = new Date(1930, 0, -42, 0, 0, 0, -1000);
    ASX_TEST_STDSTR(date->getCommonStr(), "Mon Nov 18 1929 23:59:59");

    date = new Date(1930, 0, -42, 0, 0, 0, -1000);
    ASX_TEST_STDSTR(date->getCommonStr(), "Mon Nov 18 1929 23:59:59");

    date = new Date(1, 0, -42, 0, 0, 0, -1000);
    ASX_TEST_STDSTR(date->getCommonStr(), "Sun Nov 18 1900 23:59:59");

    date = new Date(1910, 0, -42, 0, 0, 0, -2000);
    ASX_TEST_STDSTR(date->getCommonStr(), "Thu Nov 18 1909 23:59:58");

    date = new Date(1930, 0, 0);
    ASX_TEST_STDSTR(date->getCommonStr(), "Tue Dec 31 1929 00:00:00");

    date = new Date(1930, 0, -1);
    ASX_TEST_STDSTR(date->getCommonStr(), "Mon Dec 30 1929 00:00:00");

    date = new Date(1930, 0, -13);
    ASX_TEST_STDSTR(date->getCommonStr(), "Wed Dec 18 1929 00:00:00");

    date = new Date(1930, 0, -31);
    ASX_TEST_STDSTR(date->getCommonStr(), "Sat Nov 30 1929 00:00:00");

    date = new Date(1930, 0, -61);
    ASX_TEST_STDSTR(date->getCommonStr(), "Thu Oct 31 1929 00:00:00");


    date = new Date(1920, 0, 0);
    ASX_TEST_STDSTR(date->getCommonStr(), "Wed Dec 31 1919 00:00:00");

    date = new Date(1910, 0, 0); //ok mac
    ASX_TEST_STDSTR(date->getCommonStr(), "Fri Dec 31 1909 00:00:00");
    date = new Date(1909, 1, 1); //ok mac
    ASX_TEST_STDSTR(date->getCommonStr(), "Mon Feb 01 1909 00:00:00");
    date = new Date(1902, 0, 1); //ok mac
    ASX_TEST_STDSTR(date->getCommonStr(), "Wed Jan 01 1902 00:00:00");
    date = new Date(1901, 12, 31); //ok mac
    ASX_TEST_STDSTR(date->getCommonStr(), "Fri Jan 31 1902 00:00:00");
    date = new Date(1, 12, 31); //ok mac
    ASX_TEST_STDSTR(date->getCommonStr(), "Fri Jan 31 1902 00:00:00");

    date = new Date(99, -12, -31);
    ASX_TEST_STDSTR(date->getCommonStr(), "Sun Nov 30 1997 00:00:00");

    date = new Date(1, 1, 1);
    ASX_TEST_STDSTR(date->getCommonStr(), "Fri Feb 01 1901 00:00:00");

    date = new Date(255, 1, 1);
    ASX_TEST_STDSTR(date->getCommonStr(), "Thu Feb 01 255 00:00:00");

    date = new Date(0, 0, 0);
    ASX_TEST_STDSTR(date->getCommonStr(), "Sun Dec 31 1899 00:00:00");

    date = new Date(100, 0, 0);
    ASX_TEST_STDSTR(date->getCommonStr(), "Thu Dec 31 99 00:00:00");

    date = new Date(100, 1, 1);
    ASX_TEST_STDSTR(date->getCommonStr(), "Mon Feb 01 100 00:00:00");


    date = new Date(100, 1, 0);
    ASX_TEST_STDSTR(date->getCommonStr(), "Sun Jan 31 100 00:00:00");
    date = new Date(-1232, 0, 0);
    ASX_TEST_STDSTR(date->getCommonStr(), "Sat Dec 31 -1233 00:00:00");
    date = new Date(-2, 0, 0);
    ASX_TEST_STDSTR(date->getCommonStr(), "Thu Dec 31 -3 00:00:00", "JS RETURNS Wed Dec 31 -3 00:00:00");


    date = new Date(1971, 0, 0);
    ASTX_TEST(date->getDate(), 31);
    ASTX_TEST(date->getUTCDate(), 30);
    ASTX_TEST(date->getTimezoneOffset(), -120);



    printf(" DATE SUITE FINISHED");
}

void ASTX_SUITE_ECMA_ROOTS() {
    printf("ASTX_TEST_FIX_INPUT\n");
    ASTX_TEST_FIX_INPUT("line \n ", " line");
    ASTX_TEST_FIX_INPUT("    line \n blob, \t text", " line blob, text");
    ASTX_TEST_FIX_INPUT("t /*comment // commentr[]*/", " t");
    ASTX_TEST_FIX_INPUT("t/*comment // commentr[]*/s", " t s");
    ASTX_TEST_FIX_INPUT("text \n text", " text text");
    ASTX_TEST_FIX_INPUT("text \n text \"string\"", " text text \"string\"");
    ASTX_TEST_FIX_INPUT("text \n text \"stri \n ng\"", " text text \"stri ng\"");


    printf("ASTX_TEST_DETECT_CLOSURE\n");
    ASTX_TEST_DETECT_CLOSURE("(start()()()rest) after closure", " after closure", "()", true);
    ASTX_TEST_DETECT_CLOSURE("(start()()()rest) after closure", "(start()()()rest)", "()", false);
    ASTX_TEST_DETECT_CLOSURE("[start[]()()rest] after closure", "[start[]()()rest]", "[]", false);
    ASTX_TEST_DETECT_CLOSURE("aa sadasd asdd\n [start[]()()rest] after closure", "[start[]()()rest]", "[]", false);
    ASTX_TEST_DETECT_CLOSURE("aa sadasd asd [start[]()()rest] after closure", "aa sadasd asd  after closure", "[]", true);
    ASTX_TEST_DETECT_CLOSURE("aa sadasd asd {start[]()()res{({{{}}})}t} after closure", "aa sadasd asd  after closure", "{}", true);


    ASTX_TEST_DETECT_KEYWORD(" var data = function(){}; function data() { whilel IGNORE ANYTHINGF", 5, 0, ecma::ASTXKT_VAR);
    ASTX_TEST_DETECT_KEYWORD("data = function [;x]( SKIP THIS  BLOCK \"\"(){} ; ){ SKIP THIS BLOCK; \";\"(){} } endHere; function data() { whilel IGNORE ANYTHINGF", 85, 85, ecma::ASTXKT_BSLINE);
    ASTX_TEST_DETECT_KEYWORD("functionbrok data = function [;x]( SKIP THIS  BLOCK \"\"(){} ; ){ SKIP THIS BLOCK; \";\"(){} } endHere; function data() { whilel IGNORE ANYTHINGF", 98, 98, ecma::ASTXKT_BSLINE);
    ASTX_TEST_DETECT_KEYWORD("function () { whilel IGNORE ANYTHINGF }", 10, 0, ecma::ASTXKT_FUNCTION);
    ASTX_TEST_DETECT_KEYWORD("while", 0, 0, ecma::ASTXKT_BSLINE);

    printf("ecma::detectNodes\n");
    string test = "some string -; function argd(arg){argdbody} function(_args_){_body_*} var data = function(){};function data() { function(){(){}[function(){}]} } if(condition_required) { EXECIF } else if (2nd condition_required) { EXEC ELSEIF } else { EXEC ELSE function(){} } while (WHILEARGS) { EXEC_WHILE } for(FOR_ARGS)  {EXEC_FOR} switch (SWICTH_ARG) {EXEC_SWITCH}";
    std::vector<ecma::ComposedString> data;
    ecma::detectNodes(test, data);

    ASTX_TEST(data.at(0).type == ecma::ASTXKT_BSLINE, "data.at(0).type == ecma::ASTXKT_BSLINE");
    ASTX_TEST(data.at(1).type == ecma::ASTXKT_FUNCTION, "data.at(1).type == ecma::ASTXKT_FUNCTION");
    ASTX_TEST(data.at(2).type == ecma::ASTXKT_FUNCTION, "data.at(2).type == ecma::ASTXKT_FUNCTION");
    ASTX_TEST(data.at(3).type == ecma::ASTXKT_VAR, "data.at(3).type == ecma::ASTXKT_VAR");
    ASTX_TEST(data.at(0).body == "some string -", "data.at(0).body == \"some string -\"");

    for (unsigned int i = 0; i < data.size(); i++) {
        cout << "--------(" << i << ")" << endl;
        cout << "         type: " << data.at(i).type << endl;
        cout << "         name: " << data.at(i).name << endl;
        cout << "         args: " << data.at(i).arguments << endl;
        cout << "         body: " << data.at(i).body << endl;
        cout << "--------\n" << endl;
    }
}


#define FUNCTION_DEF_ASTX_STR "function (){}"

void ASTX_SUITE_ECMSCRIPT() {

    string testInput = "f = function(data, one, two){ print(data); }; f('name');";
    std::vector<std::string> _pexpects;
    _pexpects.push_back("name");
    ASTX_TEST_ECMASCRIPT(testInput, _pexpects, "A SIMPLE FUNCTION");

    _pexpects.clear();
    _pexpects.push_back("undefined");
    _pexpects.push_back("23");
    _pexpects.push_back(FUNCTION_DEF_ASTX_STR);
    _pexpects.push_back("{}");

    ASTX_TEST_ECMASCRIPT("function test(){}; b = test(16); print(b); c = 23; print(c); print(test); print(new test)", _pexpects, false, " ASTX_SUITE_ECMSCRIPT contexts ");


    _pexpects.clear();
    _pexpects.push_back("same stuff");
    _pexpects.push_back("[23,45]");
    _pexpects.push_back("[\"inherited\",\"object\"]");
    testInput = "f = function(data, one, two){ print(data); }; f('same stuff');";
    testInput.append("(function rest(unu, doi){print(unu, doi);  function inherit(args){ print('inherited', args)} return {a:1, b:56, m: inherit}; })(23, 45).m('object')");
    ASTX_TEST_ECMASCRIPT(testInput, _pexpects, "FUNCTION CONTEXT 1");

    _pexpects.clear();
    _pexpects.push_back("[]");
    _pexpects.push_back("0string=0");
    _pexpects.push_back("1string=1");
    _pexpects.push_back("2string=2");
    _pexpects.push_back("3string=3");
    _pexpects.push_back("4string=4");
    _pexpects.push_back("5string=5");
    _pexpects.push_back("5string");
    _pexpects.push_back("0string");
    _pexpects.push_back("[\"1string\",\"2string\",\"3string\",\"4string\"]");
    _pexpects.push_back("end string");
    _pexpects.push_back("begin string");
    _pexpects.push_back("[\"CONCAT: \",[\"begin string\",\"1string\",\"2string\",\"3string\",\"4string\",\"end string\",9,9,9]]");
    _pexpects.push_back("[\"begin string\",\"1string\",\"2string\",\"3string\",\"4string\",\"end string\"]");
    _pexpects.push_back("...............");
    string tmp = "array constructor: ";
    tmp.append(FUNCTION_DEF_ASTX_STR);
    _pexpects.push_back(tmp);
    _pexpects.push_back("new Array = []");

    testInput = " var a = new Array(); new Array(); new Array(); print(a);";
    testInput.append(" for(var i = 0 ; i < 6; i++) {a[i] = i+\"string\"; print(a[i]+ '=' + i)} ");
    testInput.append(" print(a.pop()); ");
    testInput.append(" print(a.shift()); ");
    testInput.append(" print(a); print(a.push('end string')); print(a.unshift('begin string')); print('CONCAT: ', a.concat([9, 9, 9])); print(a);");
    testInput.append(" a = new Array(16); var str = a.join('.'); print(str); print('array constructor: ' + Array); var c = new Array; print('new Array = '+c);");
    ASTX_TEST_ECMASCRIPT(testInput, _pexpects, true, " ARRAYS");

    _pexpects.clear();

    _pexpects.push_back("[true,\"string\",24,35,false,\"abc\"]");
    _pexpects.push_back("js sorting...");
    _pexpects.push_back("[24,35,\"abc\",false,\"string\",true]");
    _pexpects.push_back("wat?");
    _pexpects.push_back("10");
    _pexpects.push_back("[10,35,\"abc\",false,\"string\",true]");
    _pexpects.push_back("true");
    _pexpects.push_back("10");
    _pexpects.push_back("string");
    _pexpects.push_back("[\"args on\",2,[false,[],35]]");

    testInput = "Array.prototype.testProto = function(args){  if(args) {print('args on', args, this)} else { print(this) } };";
    testInput.append("a = new Array([true, 'string', 24, 35, false, 'abc'] ); a.testProto();");
    testInput.append("a.sort(); print('js sorting...'); print(a);");
    testInput.append("switch(a[0]){ case 24: print('wat?'); case 28: a[0] = 67; default: a[0] = 10 }; print(a[0]);");
    testInput.append("print(a); print(a.pop()); print(a.shift()); print(a.pop()); a[1] = []; a.reverse(); a.testProto(2);");
    ASTX_TEST_ECMASCRIPT(testInput, _pexpects, true, " ARRAY AND PROTOTYPES");

    _pexpects.clear();
    testInput.clear();
    _pexpects.push_back("false");
    _pexpects.push_back("[]");
    _pexpects.push_back("35");
    _pexpects.push_back("[false,0]");
    _pexpects.push_back("[[],1]");
    _pexpects.push_back("[35,2]");
    _pexpects.push_back("[\"false&0\",[false,[],35]]");
    _pexpects.push_back("[\"[]&1\",[false,[],35]]");
    _pexpects.push_back("[\"35&2\",[false,[],35]]");
    _pexpects.push_back("[\"false&0\",[false,[],35],undefined]");
    _pexpects.push_back("[\"[]&1\",[false,[],35],undefined]");
    _pexpects.push_back("[\"35&2\",[false,[],35],undefined]");

    testInput = "a.forEach(function(a){print(a)});";
    testInput.append("a.forEach(function(a, b){print(a, b)});");
    testInput.append("a.forEach(function(a, b, c){print(a+'&'+b, c)});");
    testInput.append("a.forEach(function(a, b, c, d){print(a+'&'+b, c, d)});");

    ASTX_TEST_ECMASCRIPT(testInput, _pexpects, true, " ARRAY AND PROTOTYPES");

    _pexpects.clear();

    _pexpects.push_back("called from local context");
    _pexpects.push_back("private prop 24");
    _pexpects.push_back("undefined");


    testInput = "var b = (function(){ function privateMethod(){ print(\"called from local context\") } return { publicMethod: privateMethod }; })(); b.publicMethod(); ";
    testInput.append("var a = (function(){ var prop = 24; ");
    testInput.append("function local (){ print('private prop ' + prop); } ");
    testInput.append("return { local: local }; })(); a.local(); print(a.prop); ");
    ASTX_TEST_ECMASCRIPT(testInput, _pexpects, true, "PRIVATE CONTEXT");


    string tststr5 = "var ex = 12+'string'; try{print(\"wtf\");}catch(ex){print(ex); var myassign = 10; return 40; }finally{print('exec finally'+ex); return {data:ex};}   ";
    std::vector<std::string> _pexpects5;
    _pexpects5.push_back("wtf");
    _pexpects5.push_back("exec finally12string");
    ASTX_TEST_ECMASCRIPT(tststr5, _pexpects5, true, " TRY CATCH (return in catch)");
    ASX_TEST_STDSTR(unitTestContext->protoype->toString(), "{\"data\":\"12string\"}");


    string tststr4 = "var ex = 12+'string'; try{print(\"wtf\");return 80;}catch(ex){print(ex); var myassign = 10; return 40; }finally{print('exec finally'+myassign+ex)}   ";
    std::vector<std::string> _pexpects4;
    _pexpects4.push_back("wtf");
    ASTX_TEST_ECMASCRIPT(tststr4, _pexpects4, false, " TRY CATCH (return in try)");
    ASX_TEST_STDSTR(unitTestContext->protoype->toString(), "80");

    string tststr3 = "var ex = 12+'string'; try{print(wft)}catch(ex){print(ex); var myassign = 10.5; return 40 + myassign + '_string'; }finally{print('exec finally'+myassign+ex)}   ";
    std::vector<std::string> _pexpects3;
    _pexpects3.push_back("Error #504 function (){[ native function]} has no prop wft at [value.class]");
    ASTX_TEST_ECMASCRIPT(tststr3, _pexpects3, true, " TRY CATCH (return in catch)");
    ASX_TEST_STDSTR(unitTestContext->protoype->toString(), "50.5_string");


    string tststr2 = "var ex = 12+'string'; try{print(wft)}catch(ex){print(ex); var myassign = 10;}finally{print('exec finally'+myassign+ex)}   ";
    std::vector<std::string> _pexpects2;
    _pexpects2.push_back("Error #504 function (){[ native function]} has no prop wft at [value.class]");
    _pexpects2.push_back("exec finally1012string");
    ASTX_TEST_ECMASCRIPT(tststr2, _pexpects2, true, " TRY CATCH");


    string tststr6 = "var ex = 12+'string'; try{print(wft)}catch(ex){print(ex); var myassign = 10;}  ";
    std::vector<std::string> _pexpects6;
    _pexpects6.push_back("Error #504 function (){[ native function]} has no prop wft at [value.class]");
    _pexpects6.push_back("exec finally1012string");
    ASTX_TEST_ECMASCRIPT(tststr6, _pexpects6, true, " TRY CATCH (no finally block)");


    testInput = "var b = (function(){ function privateMethod(){ print(\"called from local context\") } return { publicMethod: privateMethod }; })(); b.publicMethod(); ";
    testInput.append("var a = (function(){ var prop = 24; ");
    testInput.append("function local (){ print('private prop ' + prop); } ");
    testInput.append("return { local: local }; })(); a.local(); print(a.prop); ");
    ASTX_TEST_ECMASCRIPT(testInput, _pexpects, true, " PRIVATE CONTEXT");

    string tststr7 = "var main = function(){ print('called');  this.data = 20; this.caller = function(){ this.innerCaller = function (){ return 89; } } }; ";
    tststr7.append("print( 'main=', main() );");
    tststr7.append("print( 'main().data=',  main().data); ");
    tststr7.append("print( 'main().caller().innerCaller() = ', main().caller().innerCaller() );");
    tststr7.append("var done = main().caller(); print(done);");
    tststr7.append("done.innerCaller = function(){ var tt = false; print(tt); return { props: { inner: tt, toString: function() { print('tostr'); return 'innerString'+tt; }, others: 34 } }; }; ");
    tststr7.append("print('done.innerCaller().props=', done.innerCaller().props );");
    tststr7.append("print('done.innerCaller().props.inner=', done.innerCaller().props.inner );");
    tststr7.append("print('toString:', done.innerCaller().props.toString() );");

    std::vector<std::string> _pexpects7;
    _pexpects7.push_back("called");
    _pexpects7.push_back("[\"main=\",{\"caller\":function (){ this.innerCaller = function (){ return 89; } },\"data\":20}]");
    _pexpects7.push_back("called");
    _pexpects7.push_back("[\"main().data=\",20]");
    _pexpects7.push_back("called");
    _pexpects7.push_back("[\"main().caller().innerCaller() = \",89]");
    _pexpects7.push_back("called");
    _pexpects7.push_back("{\"innerCaller\":function (){ return 89; }}");
    _pexpects7.push_back("false");
    _pexpects7.push_back("[\"done.innerCaller().props=\",{\"inner\":false,\"others\":34,\"toString\":function (){ print('tostr'); return 'innerString'+tt; }}]");
    _pexpects7.push_back("false");
    _pexpects7.push_back("[\"done.innerCaller().props.inner=\",false]");
    _pexpects7.push_back("false");
    _pexpects7.push_back("tostr");
    _pexpects7.push_back("[\"toString:\",\"innerStringfalse\"]");

    ASTX_TEST_ECMASCRIPT(tststr7, _pexpects7, true, " CHAINING");




    string tststr8 = "";
    std::vector<std::string> _pexpects8;

    tststr8.append(" var a = false; print(!!a); ");
    tststr8.append("print( typeof a); ");
    tststr8.append("print( typeof typeof !a); ");
    tststr8.append("var obj = { a: { b: { obj: false } }}; ");
    tststr8.append("print( obj.a.b instanceof Object); ");
    tststr8.append("if( obj.a instanceof Object) { print('{([obj.a is instance of Object])}') } ; ");
    _pexpects8.push_back("false");
    _pexpects8.push_back("boolean");
    _pexpects8.push_back("string");
    _pexpects8.push_back("true");
    _pexpects8.push_back("{([obj.a is instance of Object])}");
    ASTX_TEST_ECMASCRIPT(tststr8, _pexpects8, true, " TYPE OF AND INSTANCEOF");


    _pexpects8.clear();
    tststr8.clear();
    tststr8.append(" var list = [1, 2, false, 45]; print(list.length);");
    tststr8.append(" list.pop(); print(list + ' length= ' + list.length);");
    tststr8.append(" list.shift(); print(list + ' length= ' + list.length);");
    tststr8.append(" list.shift(); print(list + ' length= ' + list.length);");
    tststr8.append(" list.push('some string'); print(list + ' length= ' + list.length);");
    tststr8.append(" list.unshift(4); print(list + ' length= ' + list.length);");
    tststr8.append(" list.unshift(false); print(list + ' length= ' + list.length);");
    tststr8.append(" list.push(list); print(list + ' length= ' + list.length);");
    tststr8.append(" list.unshift(list); print(list + ' length= ' + list.length);");
    tststr8.append(" list.shift(); print(list + ' length= ' + list.length);");
    tststr8.append(" list.pop(); list.pop(); list[0] = 'modified string'; print(list + ' length= ' + list.length);");
    tststr8.append(" var cclist = list.concat([9,9,false, [20], [45] ]); print(list + ' length= ' + list.length);");
    tststr8.append(" print(cclist); print(cclist[6].length); cclist.forEach(function(a, b, c) { print(a+''+b+''+c); });");
    tststr8.append(" var x = [14, 3, 77, 55, 78];");
    tststr8.append(" var y = x.slice(1, 3);");
    tststr8.append(" print(x);");
    tststr8.append(" print(y);");
    tststr8.append(" var x = [14, 3, 77, 55, 78];");
    tststr8.append(" var y = x.splice(1, 3);");
    tststr8.append(" print(x);");
    tststr8.append(" print(y);");
    _pexpects8.push_back("4");
    _pexpects8.push_back("[1,2,false] length= 3");
    _pexpects8.push_back("[2,false] length= 2");
    _pexpects8.push_back("[false] length= 1");
    _pexpects8.push_back("[false,\"some string\"] length= 2");
    _pexpects8.push_back("[4,false,\"some string\"] length= 3");
    _pexpects8.push_back("[false,4,false,\"some string\"] length= 4");
    _pexpects8.push_back("[false,4,false,\"some string\",\"$this\"] length= 5");
    _pexpects8.push_back("[\"$this\",false,4,false,\"some string\",\"$this\"] length= 6");
    _pexpects8.push_back("[false,4,false,\"some string\",\"$this\"] length= 5");
    _pexpects8.push_back("[\"modified string\",4,false] length= 3");
    _pexpects8.push_back("[\"modified string\",4,false] length= 3");
    _pexpects8.push_back("[\"modified string\",4,false,9,9,false,[20],[45]]");
    _pexpects8.push_back("1");
    _pexpects8.push_back("modified string0[\"modified string\",4,false,9,9,false,[20],[45]]");
    _pexpects8.push_back("41[\"modified string\",4,false,9,9,false,[20],[45]]");
    _pexpects8.push_back("false2[\"modified string\",4,false,9,9,false,[20],[45]]");
    _pexpects8.push_back("93[\"modified string\",4,false,9,9,false,[20],[45]]");
    _pexpects8.push_back("94[\"modified string\",4,false,9,9,false,[20],[45]]");
    _pexpects8.push_back("false5[\"modified string\",4,false,9,9,false,[20],[45]]");
    _pexpects8.push_back("[20]6[\"modified string\",4,false,9,9,false,[20],[45]]");
    _pexpects8.push_back("[45]7[\"modified string\",4,false,9,9,false,[20],[45]]");
    _pexpects8.push_back("[14,3,77,55,78]");
    _pexpects8.push_back("[3,77]");
    _pexpects8.push_back("[14,78]");
    _pexpects8.push_back("[3,77,55]");
    ASTX_TEST_ECMASCRIPT(tststr8, _pexpects8, true, " ARRAY LENGTH CHANGING, SLICE AND SPLICE");





    _pexpects8.clear();
    tststr8 = " var a = \"some-string-sample\"; print(a);";
    tststr8.append(" print(typeof a);");
    tststr8.append(" var answ = (a instanceof string); print(answ);");
    tststr8.append(" answ = (a instanceof StRinG); print(answ); print('a.length=', a.length);");
    tststr8.append(" print('a.toJSON=', a.toJSON);");
    tststr8.append(" a.reverse(); print('a.reverse()=', a);");
    tststr8.append(" print('a[0]=', a[0]);");
    tststr8.append(" print('a[20]=', a[20]);");
    tststr8.append(" print('a[a.length-1]=', a[a.length-1]);");
    tststr8.append(" a.reverse(); print(a[10]+a[11]+a[12]);");
    tststr8.append(" print(a.charAt(10)+a.charAt(11)+a.charAt(12));");
    tststr8.append(" var b = \" some untrimmed string      \";");
    tststr8.append(" print(b); b.trim(); print('trimmed:'+b);");
    tststr8.append(" b.killWhiteSpace(); print('nowhitespace:'+b);");
    tststr8.append(" b = \"abc2abc2abc3abc4abc5abcd\"; print( b.split(\"abc\") );");
    tststr8.append(" print(\"empty string\", \"\"); b = \"abc2abc2abc3abc4abc5abcd\"; print( b.split(\"\") );");
    tststr8.append(" b = \"1 2 3 4 5 6 7 8 9 0\"; print( b.splitByWhiteSpace() );");
    tststr8.append(" b = \"1a2ba3caf\"; print( b.splitByChar(\"a\") ); print(b.sort());");
    tststr8.append(" b = \"some data sample\"; print(b.indexOf(\"data\"));");
    tststr8.append(" b = \"Please locate where 'locate' occurs!\"; print(b.indexOf(\"locate\"));");
    tststr8.append(" print(b.lastIndexOf(\"locate\"), '==21?');");
    tststr8.append(" var str = \"Apple, Banana, Kiwi\"; var res = str.slice(7,13); print(res, '==Banana?');");
    tststr8.append(" res = str.slice(-12,-6); print(res, '==Banana?'); var a = 1 + (-2); print(a); print(str.slice(-12, -6), '==Banana?')");
    tststr8.append(" print(res.substr(7, 6), '==Banana?');");


    _pexpects8.push_back("some-string-sample");
    _pexpects8.push_back("string");
    _pexpects8.push_back("true");
    _pexpects8.push_back("true");
    _pexpects8.push_back("[\"a.length=\",18]");
    _pexpects8.push_back("[\"a.toJSON=\",function (){}]");
    _pexpects8.push_back("[\"a.reverse()=\",\"elpmas-gnirts-emos\"]");
    _pexpects8.push_back("[\"a[0]=\",\"e\"]");
    _pexpects8.push_back("[\"a[20]=\",undefined]");
    _pexpects8.push_back("[\"a[a.length-1]=\",\"s\"]");
    _pexpects8.push_back("g-s");
    _pexpects8.push_back("g-s");
    _pexpects8.push_back(" some untrimmed string ");
    _pexpects8.push_back("trimmed:some untrimmed string");
    _pexpects8.push_back("nowhitespace:someuntrimmedstring");
    _pexpects8.push_back("[\"2\",\"2\",\"3\",\"4\",\"5\",\"d\"]");
    _pexpects8.push_back("[\"empty string\",\"\"]");
    _pexpects8.push_back("[\"a\",\"b\",\"c\",\"2\",\"a\",\"b\",\"c\",\"2\",\"a\",\"b\",\"c\",\"3\",\"a\",\"b\",\"c\",\"4\",\"a\",\"b\",\"c\",\"5\",\"a\",\"b\",\"c\",\"d\"]");
    _pexpects8.push_back("[\"1\",\"2\",\"3\",\"4\",\"5\",\"6\",\"7\",\"8\",\"9\",\"0\"]");
    _pexpects8.push_back("[\"1\",\"2b\",\"3c\",\"f\"]");
    _pexpects8.push_back("123aaabcf");
    _pexpects8.push_back("5");
    _pexpects8.push_back("7");
    _pexpects8.push_back("[21,\"==21?\"]");
    _pexpects8.push_back("[\"Banana\",\"==Banana?\"]");
    _pexpects8.push_back("[\"Banana\",\"==Banana?\"]");
    _pexpects8.push_back("-1");
    _pexpects8.push_back("[\"Banana\",\"==Banana?\"]");
    ASTX_TEST_ECMASCRIPT(tststr8, _pexpects8, true, " STRING OBJECT AND METHODS");



    tststr8 = "var a = RegEx(/a/); RegEx(/a/i); RegEx(/a/i); RegEx(/a/mig); RegEx(/a/img);";
    tststr8.append(" var str = \"The rain in SPAIN stays mainly in the plain\";");
    tststr8.append(" print(str); print(str.match(a)); print(str.match(RegEx(/ain/i)));  print(str.match(RegEx(/ain/g)));");
    tststr8.append(" print(str.replace(\"rain\", \"WINTER\")); print(str.replace(RegEx(/ain/gi), \"NAI\"));");
    tststr8.append(" print(str.replace(RegEx(/ain/gi), \"-NAI-\"));");
    _pexpects8.clear();
    _pexpects8.push_back("The rain in SPAIN stays mainly in the plain");
    _pexpects8.push_back("[\"a\",\"a\",\"a\",\"a\"]");
    _pexpects8.push_back("[\"ain\",\"AIN\",\"ain\",\"ain\"]");
    _pexpects8.push_back("[\"ain\",\"ain\",\"ain\"]");
    _pexpects8.push_back("The WINTER in SPAIN stays mainly in the plain");
    _pexpects8.push_back("The rNAI in SPNAI stays mNAIly in the plNAI");
    _pexpects8.push_back("The r-NAI- in SP-NAI- stays m-NAI-ly in the pl-NAI-");

    ASTX_TEST_ECMASCRIPT(tststr8, _pexpects8, true, " REGEX OBJECT AND METHODS");



    _pexpects8.clear();
    tststr8 = "astox_info(); print(new Date()); require('fs');";
    Date n;
    _pexpects8.push_back(n.toString());
    ASTX_TEST_ECMASCRIPT(tststr8, _pexpects8, true, " ASTOX DATE NOW");


    _pexpects8.clear();
    _pexpects8.push_back("some_data");
    _pexpects8.push_back("1");
    _pexpects8.push_back("{\"data\":\"some data\"}");
    _pexpects8.push_back("false");
    tststr8 = "var a = new String(\"some_data\"); print(a);";
    tststr8.append(" var b = new Number(1); print(b);");
    tststr8.append(" var c = new Object(); c.data = 'some data'; print(c);");
    tststr8.append(" var d = new Boolean(0); print(d);");

    ASTX_TEST_ECMASCRIPT(tststr8, _pexpects8, true, " ASTOX CONSTRUCTORS");



    _pexpects8.clear();
    _pexpects8.push_back("undefined");
    _pexpects8.push_back("1");
    _pexpects8.push_back("2");
    _pexpects8.push_back("3");
    _pexpects8.push_back("5");
    _pexpects8.push_back("8");
    _pexpects8.push_back("13");
    _pexpects8.push_back("21");
    _pexpects8.push_back("34");
    _pexpects8.push_back("55");
    tststr8 = "var p;";
    tststr8.append(" var fib = []; fib[0] = 0; fib[1] = 1; print(p);");
    tststr8.append(" for(i=2; i<=10; i++){");
    tststr8.append(" fib[i] = fib[i-2] + fib[i-1];");
    tststr8.append(" print(fib[i]); }");
    ASTX_TEST_ECMASCRIPT(tststr8, _pexpects8, true, " FIBONACCI NUMBERS");


    _pexpects8.clear();
    _pexpects8.push_back("2");
    _pexpects8.push_back("120");
    _pexpects8.push_back("5.109094217170944e+19");
#ifdef ASTOX_OS_WINDOWS
    _pexpects8.push_back("1.12400072777760768e+021");
#else
    _pexpects8.push_back("1.12400072777760768e+21");
#endif

    _pexpects8.push_back("2");
    tststr8 = " function fact(x){ if(x == 0){ return 1; } return x * fact(x-1); };";
    tststr8.append(" var n = fact(2); print(n); print(fact(5)); print(fact(21)); print(fact(22));");
    tststr8.append(" (function (window){ print(window) } )(2);");
    ASTX_TEST_ECMASCRIPT(tststr8, _pexpects8, true, "FACTORIAL NUMBERS");
	printf("...exit block at %d", __LINE__);
}

int counter = 0;
MutexLock pthlock;

struct PassedObject {
    int data;
};

ThreadCallback THREAD_CALLBACK(ThreadArgs param) {
    MutexState mutexState = Thread::mutexLock(&pthlock);
    PassedObject * current = (PassedObject *) param;
    counter++;
    printf("out %i, args=%i\n", counter, current->data);
    switch (mutexState) {
        case MUTEX_OK:
            printf("MUTEX_OK\n");
            break;
        case MUTEX_ABANDONED:
            printf("MUTEX_ABANDONED\n");
            break;
        default:
            printf("MUTEX_UNDEF: \n");
            break;
    }
    Thread::mutexUnlock(&pthlock);
    Thread::exitCurrent(NULL);
    return 0;
}

void ASTX_TEST_THREAD() {
    Thread::mutexInit(&pthlock);
    Thread myThread;
    for (int i = 0; i < 100; i++) {
        PassedObject * obj = new PassedObject();
        obj->data = i;
        myThread.createThread(THREAD_CALLBACK, (ThreadArgs) obj);
    }
    myThread.joinAll();

    cout << "and counter is: " << counter << endl;
    cout << " NUMBER OF CORES: " << Thread::getNumCPU() << endl;
}





const int msgSize = 3;
std::string _messages[msgSize][2] = {
    {"Hello, I'm a client!", "Hello client, how are you?"},
    {"I'm fine, thanks!", "Me too!"},
    {"My third message is here!", "My third answer in here"}
};

Socket server;
Socket client;

void handle_client_messages(int indexToSend, SockId sock) {
    if (indexToSend < msgSize) {
        std::string response;
        Socket::Send(sock, _messages[indexToSend][0]);
        //Socket::SetNonBlock(sock);
        printf("client start receiving:\n");
        Socket::Receive(sock, response);
        astox::stringAlg::trim(response);
        printf("client receives (%s)\n", response.c_str());

        if (response == _messages[indexToSend][1]) {
            printf("-------------------------------- ANSWER IS CORRECT\n");
            handle_client_messages(indexToSend + 1, sock);
        } else {
            printf(" ANSWER IS NOT CORRECT\n");
            astox_system_pause();
        }
    } else {
        Socket::Send(sock, "bye, bye!!!");
    }
}

void client_onConnect(SockId sock, SocketInfo remoteAddr) {
    printf(" client connection on %i\n", sock);
    handle_client_messages(0, sock);
}

void server_onConnect(SockId socket, SocketInfo remoteAddr) {
    cout << "new server connection on (" << socket << ")" << endl;
    std::cout << " remoteAddr.remoteIp = " << remoteAddr.remoteIp << std::endl;
    std::cout << " remoteAddr.remotePort = " << remoteAddr.remotePort << std::endl;
    std::cout << " remoteAddr.currentSocket = " << remoteAddr.currentSocket << std::endl;
    std::cout << " remoteAddr.masterSocket = " << remoteAddr.masterSocket << std::endl;
}

void server_onDisconnect(SockId socket) {
    cout << socket << " socket closed " << endl;

}

void handle_server_mesages(SocketInfo &remoteAddr, std::string message) {
    for (unsigned int i = 0; i < remoteAddr.clients.size(); i++) {
        printf("                  handle_mesages (%s) to %i\n", message.c_str(), remoteAddr.clients.at(i));
        Socket::Send(remoteAddr.clients.at(i), message);
    }
}

void server_onReceive(std::string buffer, SocketInfo remoteAddr) {

    astox::stringAlg::trim(buffer);
    printf("server receives(%s) (%i bytes)\n", buffer.c_str(), (int) buffer.size());

    for (int i = 0; i < msgSize; i++) {
        if (buffer == _messages[i][0]) {
            handle_server_mesages(remoteAddr, _messages[i][1]);
            return;
        }
    }

    printf(" you should close the server...\n");
    server.Close();
}

ThreadCallback socketServer_test(ThreadArgs args) {

    server.onConnect(server_onConnect);
    server.onReceive(server_onReceive);
    server.onDisconnect(server_onDisconnect);

    cout << " STARTUP SERVER \n";

    server.runNonBlocking();
    return 0;
}

ThreadCallback socketClient_test(ThreadArgs args) {

    cout << " STARTUP CLIENT \n";
    client.onConnect(client_onConnect);

    while (!server.sockConnected) {
        printf("wait for server to start\n");
    };

    client.runClient("localhost", 9001);
    return 0;
}

void ASTX_TEST_SOCKS_AND_THREADS() {
    __numtst++;
    Thread socksThread;
    socksThread.createThread(socketServer_test, NULL);
    socksThread.createThread(socketClient_test, NULL);
    socksThread.joinAll();
    socksThread.closeAll();
}

Value * watcherHandler(Value * data) {
    cout << data->toString() << endl;
    return Value::npos;
}

void ASTX_FS_TESTS() {
    std::string drives = FileSystem::getRoots();
    printf("FileSystem::getRoots: %s \n", drives.c_str());
    std::string workingDir = FileSystem::getWorkingDirectory();
    printf("FileSystem::getWorkingDirectory: %s \n", workingDir.c_str());
    std::string currentFile = FileSystem::getProcessPath();
    printf("FileSystem::getProcessPath: %s \n", workingDir.c_str());
    //ecma::methods::externs::fs_ftwr(String::New("D:\\CODE\\astox-final"));
    FileSystem::mkdir("testDirRoot");
    printf("mkdir (testDirRoot)\n");

    int maxRootDirs = 10;

    for (int i = 0; i < maxRootDirs; i++) {
        string testDirName = "testDir";
        testDirName.append(stringAlg::intToString(i));
        string pathstr = "testDirRoot";
        pathstr += ASTOX_OS_SEPARATOR;
        pathstr.append(testDirName);
        bool r = FileSystem::mkdir(pathstr);
        if (r) {
            printf("mkdir %s\n", pathstr.c_str());
        } else {
            printf("mkdir %s already exist\n", pathstr.c_str());
        }

        bool exist = FileSystem::isDirectory(pathstr);
        if (exist) {
            printf(" IT REALLY EXIST!\n");
        } else {
            printf(" FileSystem::isDirectory %s FAILED!\n", pathstr.c_str());
            astox_system_pause();
        }


        for (int k = 0; k < 45; k++) {
            string cf = pathstr;
            cf += ASTOX_OS_SEPARATOR;
            cf.append(stringAlg::intToString(k));
            cf.append("fi.java");
            FileSystem::touch(cf, cf.c_str());
        }


        for (int m = 0; m < 35; m++) {
            string cf = "testDirRoot";
            cf += ASTOX_OS_SEPARATOR;
            cf.append(stringAlg::intToString(m));
            cf.append("_root.file");
            FileSystem::touch(cf, cf.c_str());
        }

        for (int j = 0; j < 20; j++) {
            string cdir = pathstr;
            cdir += ASTOX_OS_SEPARATOR;
            cdir.append("fldr");
            cdir.append(stringAlg::intToString(i));
            cdir.append("_");
            cdir.append(stringAlg::intToString(j));

            bool r = FileSystem::mkdir(cdir);
            if (r) {
                printf("mkdir %s\n", cdir.c_str());
            } else {
                printf("mkdir %s already exist\n", cdir.c_str());
            }

            string fname = cdir;
            fname += ASTOX_OS_SEPARATOR;
            fname.append("file");
            fname.append(stringAlg::intToString(i));
            fname.append(stringAlg::intToString(j));

            string a = fname;
            a.append(".txt");
            FileSystem::touch(a, a.c_str());
            string b = fname;
            b.append(".css");
            FileSystem::touch(b, b.c_str());


            if (j % 2 == 0) {
                string c = fname;
                c.append(".js");
                FileSystem::touch(c, c.c_str());
            } else {
                string d = fname;
                d.append(".dat");
                FileSystem::touch(d, d.c_str());
            }
        }
    }
    EventDispatcher::addEventListener(Event::ON_FS_FILE_WATCH_CHANGED, new Function(watcherHandler));
    FileSystem::createWatcher("testDirRoot", 5);

    if (FileSystem::rmDirectory("testDirRoot\\testDir1")) {
        printf("FileSystem::rmDirectory testDirRoot/testDirRoot\n");
    } else {
        printf(" FileSystem::rmDirectory(testDirRoot|testDir1) FAILED!\n");
        astox_system_pause();
    };

    if (FileSystem::rmDirectory("testDirRoot")) {
        printf("FileSystem::rmDirectory testDirRoot\n");
    } else {
        printf(" FileSystem::rmDirectory(testDirRoot) FAILED!\n");
        astox_system_pause();
    };
}

void ASTX_NUMBER_TO_STRING_EXPECT(Value * value, std::string astxExpect, std::string jsExpect, std::string msg = "") {
    __numtst++;
    std::string message = "SUCCESS";
    int color = FG_GREEN_2;
    int corrects = 0;


    for (int i = 0; i < value->toString().size(); i++) {
        if (i < astxExpect.size() &&
                value->toString().at(i) == astxExpect.at(i)) {
            corrects++;
        }
    }

    if (value->toString().size() - corrects > 2) {
        color = FG_RED_2;
        message = "FAIL";
    }



    astox_console_write(color, "%i) %s %s -> astox value is %s, expect %s, js val: %s corrects: %i from %i\n",
            __numtst,
            msg.c_str(),
            message.c_str(),
            value->toString().c_str(),
            astxExpect.c_str(),
            jsExpect.c_str(),
            corrects,
            value->toString().size()
            );
}

void ASTX_TEST_NUMBER_TO_STRING() {
    
	ASTX_NUMBER_TO_STRING_EXPECT(Number::New(NAN), "NaN", "NaN");
	int n = 0;
	ASTX_NUMBER_TO_STRING_EXPECT(Number::New(n), "0", "0");
	ASTX_NUMBER_TO_STRING_EXPECT(Number::New(2), "2", "2");
    Math * math = Math::getInstance();
    ASTX_NUMBER_TO_STRING_EXPECT(Number::New(2.718281828459045), "2.718281828459045", "2.718281828459045");
    ASTX_NUMBER_TO_STRING_EXPECT(math->getNumber("PI"), "3.14159265358979312", "3.141592653589793", "Math.PI");
    ASTX_NUMBER_TO_STRING_EXPECT(math->getNumber("E"), "2.71828182845904509", "2.718281828459045", "Math.E");
    ASTX_NUMBER_TO_STRING_EXPECT(math->getNumber("LN2"), "0.693147180559945286", "0.6931471805599453", "Math.LN2");
    ASTX_NUMBER_TO_STRING_EXPECT(Math::m_acosh(Number::New(2)), "1.3169578969248166", "1.3169578969248166", "Math.acosh(2)");
    ASTX_NUMBER_TO_STRING_EXPECT(Math::m_acosh(Number::New(3)), "1.7627471740390861", "1.762747174039086", "Math.acosh(3)");
}

int main() {

    ASTX_TEST_NUMBER_TO_STRING();
   // system("pause");
   // return 10;

    int in = 0;
    ASTX_TEST_REGEXFLAG(RegExp::strIsRegexFormat("/asd343+dasd/", in), JSREG_BASIC);
    ASTX_TEST_REGEXFLAG(RegExp::strIsRegexFormat("/asdasd/g", in), JSREG_GLOBAL);
    ASTX_TEST_REGEXFLAG(RegExp::strIsRegexFormat("/asdaasd3asd/m", in), JSREG_MULTILINE);
    ASTX_TEST_REGEXFLAG(RegExp::strIsRegexFormat("/asasdasd/i", in), JSREG_IGNORECASE);
    ASTX_TEST_REGEXFLAG(RegExp::strIsRegexFormat("/a/gim", in), JSREG_GLOBAL | JSREG_IGNORECASE | JSREG_MULTILINE);

    ASTX_SUITE_OPERATORS();
    ASTX_SUITE_ECMA_WITHOUT_CONTEXT();
    ASTX_SUITE_SSCRIPT_BOOL();
    ASTX_SUITE_SSCRIPT_NUMERIC();
    ASTX_SUITE_ECMA_SELECTOR_NODES();
    ASTX_SUITE_ECMA_ROOTS();
    ASTX_SUITE_ECMSCRIPT();
    ASTX_SUITE_ARRAY_METHODS();
    ASTX_SUITE_ASSIGN_ERRORS();


    MINGUI_Dialog("for the following test, press OK", "for the following test press OK", NULL, DG_OK_ERROR);
    MINGUI_Dialog("for the following test, press OK", "for the following test press OK", NULL, DG_OKCANCEL_INFO);
    printf("if you don't see the open file window, maybe it is behind the terminal...\n");
    OpenFileResult result = MINGUI_OpenFile("press Here");
    ASTX_TEST_SOCKS_AND_THREADS();


    Path * p;
    MINGUI_Notification("testing mingui notification", "subtitle", "footer", 2000);
    MINGUI_Dialog("for the following test, press OK", "for the following test press OK", NULL, DG_OK_ERROR);

    ASTX_FS_TESTS();
    system("pause");
    return 10;
}
