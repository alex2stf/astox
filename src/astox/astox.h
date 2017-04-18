/*
 * astox.h
 *  Created on: Jul 24, 2014
 *      Author: alexandru
 */

#ifndef ASTOX_H_
    #define ASTOX_H_


    #include <iostream>
    #include <map>
    #include <exception>
    #include <stdexcept>
    #include <cmath>
    #include <iomanip>

    #include "Util.h"
    #include "Macros.h"

    #ifdef ASTX_ENABLE_QT
        #include <QString>
    #endif


    #define ASTX_PARSE_STAT_INTAGMODE "inTagMode"
    #define ASTX_PARSE_STAT_INTXTMODE "inTextMode"
    #define ASTX_DEF_ATTR_NAME "@attributes"


    #define ASTOX_TOSTRING_VALUE "undefined"
    #define ASTOX_TOSTRING_OBJECT "[Object]"
    #define ASTOX_TOSTRING_ARRAY "[Array]"
    #define ASTOX_TOSTRING_NATIVE_FUNC "[ native function]"

    #define ASTOX_VALUE_STR_UNDEFINED "undefined"
    #define ASTOX_VALUE_STR_STRING "string"
    #define ASTOX_VALUE_STR_NUMBER "number"
    #define ASTOX_VALUE_STR_BOOL  "boolean"
    #define ASTOX_VALUE_STR_OBJECT  "object"
    #define ASTOX_VALUE_STR_ARRAY  "array"

    #define IO_COMMAND_CREATE "-CREATE"
    #define IO_COMMAND_LCREATE "-LCREATE"
    #define IO_COMMAND_USE "-USE"
    #define IO_COMMAND_LUSE "-LUSE"
    #define IO_COMMAND_DROP "-DROP"
    #define IO_COMMAND_SAVE "-SAVE"


    #define ASTOX_COUNTOF(x) (sizeof(x)/sizeof(*x))



namespace astox {
    typedef std::map<std::string, std::string> StringMapHolder;

    enum ASTOX_VALUE_TYPES {
        ASTOX_VALUE_UNDEFINED = 1,
        ASTOX_VALUE_STRING,
        ASTOX_VALUE_NUMBER,
        ASTOX_VALUE_BOOL,
        ASTOX_VALUE_OBJECT,
        ASTOX_VALUE_ARRAY,
        ASTOX_VALUE_FUNCTION,
        ASTOX_VALUE_REGEXP,
        ASTOX_VALUE_DATE,
        ASTOX_VALUE_CUSTOM = 0
    };

    enum ASTOX_NODE_STATS {
        ASTX_NODE_STAT_SELF_CLOSED,
        ASTX_NODE_STAT_VIRTUAL,
        ASTX_NODE_STAT_OPENING,
        ASTX_NODE_STAT_CLOSING,
        ASTX_NODE_STAT_COMMENT,
        ASTX_NODE_STAT_DOCTYPE
    };

    enum SELECTOR_NODE_TYPES {
        ST_BASIC_STR,
        ST_NUMERIC,
        ST_EVAL,
        ST_ARG,
        STSUB_INSTANCEOF // inturi pentru subtype selector, gen instanceof
    };

    enum OPERATIONS {
        OP_UNSET = -34,
        OP_RETURN = 2,
        OP_ASSIGN,
        OP_EQUALS,
        OP_EQUALS_STRICT,
        OP_ADDITION,
        OP_MULTIPLY,
        OP_DIVIDE,
        OP_SUBSTRACT,
        OP_MODULUS,
        OP_BIGGER,
        OP_BIGGER_EQ,
        OP_LESSER,
        OP_LESSER_EQ,
        OP_INHERITED,
        OP_INSTRUCTION_END,
        OP_DIFFERS,
        OP_DIFFERS_STRICT,
        OP_AND,
        OP_AND_STRICT,
        OP_OR,
        OP_OR_STRICT,
        OP_SHIFT_LEFT,
        OP_SHIFT_RIGHT,
        OP_CALEE,
        OP_PLUSPLUS,
        OP_MINMIN,
        OP_TEXT_ONLY,
        OP_VAR_IN
    };

    enum STR_INPUT_TYPES {
        STRIN_STRING = 3,
        STRIN_NUMBER,
        STRIN_BOOL,
        STRIN_OBJECT,
        STRIN_LIST,
        STRIN_VARNAME,
        STRIN_UNDEFINED,
        STRIN_TRUE,
        STRIN_FALSE,
        STRIN_ENCAPSULATED,
        STRIN_EMPTY,
        STRIN_FUNCTION
    };


    struct SelectorNode {
        SelectorNode(std::string _data, int _type);
        std::string data;
        std::string _instAfter;
        int type;
        int subType;
		#ifdef ASTX_TRC_SCRIPT_ENGINE
        void debug() {
            switch (type) {
                case ST_EVAL:
					stx_trc_script(ASTX_LOCATION, "eval %s", data.c_str());
                    break;
                case ST_BASIC_STR:
					stx_trc_script(ASTX_LOCATION, "basic %s", data.c_str());
                    break;
                case ST_ARG:
					stx_trc_script(ASTX_LOCATION, "args %s", data.c_str());
                    break;
                case ST_NUMERIC:
					stx_trc_script(ASTX_LOCATION, "numeric %s", data.c_str());
                    break;
                default:
					stx_trc_script(ASTX_LOCATION, "unset %s", data.c_str());
                    break;
            }
        }
		#endif
    };


    class String;
    class Number;
    class Boolean;
    class Array;
    class Object;
    class Function;
    class Math;
    class ObjectFactory;

    enum GETR_TYPES {
        GETR_STRICT = 6,
        GETR_SAFE_STR,
        GETR_SAFE_NPOS
    };

    class Value {
    protected:
        int valueType;
        int numCalls;

        int uid;
        std::map<std::string, astox::Value *> objectList;
        //	std::map<std::string , astox::Function *> functionList;

        //function name, and method with value argument:
        //	std::map<std::string, astox::Value* (*)(Value * args)> functionList;
        //	std::map<std::string, astox::Value* (*)(Value * args)>::iterator func_it;
        //	virtual astox::Value * getParsedArgs(std::string &args, Function *& context);
        virtual void updateGetters();
        virtual void setGetters();
    private:
        int parentUid;
    public:
        Value();
        virtual ~Value();
        static Value * npos;
        void addKey(std::string key, astox::Value * value);
        virtual int getType();
        virtual std::string getStrType();
        void setParentUid(int uid);
        int getParentUid();
        int getUid();

        virtual astox::Value * getRecursive(std::vector<SelectorNode> &list, int index, Function *& context, int getType = GETR_STRICT);
        virtual astox::Value * setRecursive(std::vector<SelectorNode> &list, int index, Value * newValue, Function *& context);
        virtual astox::Value *& get(astox::Value * data);
        virtual astox::Value *& getValue(std::string keyName);
        virtual astox::String * getString(std::string keyName);
        virtual astox::Number * getNumber(std::string keyName);
        virtual astox::Boolean * getBoolean(std::string keyName);
        virtual astox::Object * getObject(std::string keyName);
        virtual astox::Array * getArray(std::string keyName);
        virtual astox::Value * updateValue(std::string keyName, astox::Value * newValue);
        virtual void updatePValue(std::string newString);
        virtual void updatePValue(int newInt);

        virtual void dropData(); // sterge datele din interiorul clasei

        int getNumCalls();
        void setNumCalls(int num);
        virtual std::vector<std::string> getKeyNameList();

        //AT
        virtual astox::Value * itemAtIndex(int index);
        virtual astox::Value * itemAtIndex(std::string indexStr);
        static astox::Value * duplicate(Value * in);


        //SIZES:
        int length();


        //IS
        virtual bool isString();
        virtual bool isNumber();
        virtual bool isBoolean();
        virtual bool isArray();
        virtual bool isObject();
        virtual bool isUndefined();
        virtual bool isEmpty();

        //OPERATORS:
        virtual Boolean * isBiggerThan(Value * term);
        virtual Boolean * isLessThan(Value * term);
        virtual Boolean * isBiggerEqThan(Value * term);
        virtual Boolean * isLessEqThan(Value * term);

        virtual Value * increment(Value * term);
        virtual Value * substract(Value * term);
        virtual Value * multiply(Value * term);
        virtual Value * divide(Value * term);
        virtual Value * modulus(Value * term);
        virtual Boolean * equals(Value * term);
        virtual Boolean * equalsStrict(Value * term);
        virtual Boolean * differs(Value * term);
        virtual Boolean * differsStrict(Value * term);
        virtual Value * orBase(Value * term);
        virtual Value * orStrict(Value * term);
        virtual Value * andBase(Value * term);
        virtual Value * andStrict(Value * term);
        virtual Value * shiftLeft(Value * term);
        virtual Value * shiftRight(Value * term);
        virtual Value * plusPlus();
        virtual Value * minMin();

        virtual Boolean * contains(Value * term);
        virtual bool hasKey(std::string name);

        virtual Value * modify(Value * term, int operation);
        virtual Boolean * negate();

        //TO
        virtual std::string toString();
        virtual int toInt();
        virtual double toDouble();
        virtual bool toBoolean();
        virtual Value * call(Value * arguments);
        virtual Value * call(std::string &args, Function *&context);
        int eraseKey(std::string keyName);
        String * getTypeOf();
        Boolean * isInstanceOf(std::string keyName);


        friend std::ostream& operator<<(std::ostream& output, const Value& val);
        friend std::ostream& operator<<(std::ostream& output, const Value * val);

        friend bool operator<(Value &cC1, Value &cC2);
        friend bool operator>(Value &cC1, Value &cC2);
    };


	

    namespace ecma {
        namespace methods {
            Value * print(Value * arguments);
            Value * astox_info(Value * arguments);

            void map_prototype(Value * prototypeObject, Value * response);

            Value * Array_constructor(Value * arguments, Value * prototypeObject);
            Value * Object_constructor(Value * arguments, Value * prototypeObject);
            Value * String_constructor(Value * arguments, Value * prototypeObject);
            Value * Number_constructor(Value * arguments, Value * prototypeObject);
            Value * Boolean_constructor(Value * arguments, Value * prototypeObject);
            Value * Date_constructor(Value * arguments, Value * prototypeObject);
            Value * Require_constructor(Value * arguments, Value * prototypeObject);

            Value * array_reverse(Value * arguments, Value * object);
            Value * array_join(Value * arguments, Value * object);
            Value * array_sort(Value * arguments, Value * object);
            Value * array_push(Value * arguments, Value * object);
            Value * array_shift(Value * arguments, Value * object);
            Value * array_pop(Value * arguments, Value * object);
            Value * array_unshift(Value * arguments, Value * object);
            Value * array_concat(Value * arguments, Value * object);
            Value * array_forEach(Value * arguments, Value * object);
            Value * array_splice(Value * arguments, Value * object);
            Value * array_slice(Value * arguments, Value * object);
            Value * array_indexOf(Value * arguments, Value * object);
            Value * array_lastIndexOf(Value * arguments, Value * object);


            Value * string_charAt(Value * arguments, Value * object);
            Value * string_trim(Value * arguments, Value * object);
            Value * string_killWhiteSpace(Value * arguments, Value * object);
            Value * string_split(Value * arguments, Value * object);
            Value * string_splitByWhiteSpace(Value * arguments, Value * object);
            Value * string_splitByChar(Value * arguments, Value * object);
            Value * string_match(Value * arguments, Value * object);
            Value * string_replace(Value * arguments, Value * object);
            //ce e cu proto e la toate
            Value * proto_toString(Value * arguments, Value * object);
            Value * proto_toJSON(Value * arguments, Value * object);


            Value * setInterval(Value * arguments, Value * object);
        }
    }

    class String : public Value {
    private:
        static std::map<std::string, String *> _memoizedStrings;

    public:
        String();
        String(const char * value);
        String(std::string value);
        String(double type);
        String(int type);
        String(char charVal);
        static String * New(std::string value);
        static String * NaN;
    #ifdef ASTX_ENABLE_QT
        String(QString * qstring);
        String(QString qstring);
    #endif
        virtual ~String();
        std::string toString();
        static astox::String * cast(Value * value);
        Value * reverse();
        void append(astox::Value * data);
        bool isString();
        bool isNumber();
        bool isBoolean();
        bool isArray();
        bool isObject();
        bool isUndefined();
        Boolean * negate();

        double toDouble();
        int toInt();
        bool toBoolean();
        astox::Value * setRecursive(std::vector<SelectorNode> &list, int index, Value * newValue, Function *& context);
        std::string getStrType();
        astox::Value * itemAtIndex(std::string indexStr);
        astox::Value * itemAtIndex(int index);
        String * trim();
        String * killWhiteSpace();
        Array * split(Value * arguments);
        Array * splitByWhiteSpace(Value * arguments);
        Array * splitByChar(Value * arguments);
        String * sort();
        Number * indexOf(std::string substr);
        Number * lastIndexOf(std::string substr);
        String * slice(Value * arguments);
        String * slice(int start, int end);
        String * substr(Value * arguments);

        Array * match(Value * arguments);
        String * replace(Value * arguments);



        friend std::ostream& operator<<(std::ostream& output, const String& val);
        friend std::ostream& operator<<(std::ostream& output, const String * val);
        friend String operator+(String lhs, const String& rh);
        String& operator+=(const char* rhs);
        String& operator+=(const std::string& rhs);
        String& operator+=(Value& rhs);
        String& operator++();
        String operator++(int);
        String& operator--();
        String operator--(int);
    private:
        std::string pvalue;
    protected:
        void setGetters();
        void updateGetters();
    };

    class Number : public astox::Value {
    private:
        double pvalue;
        Number();

    public:

        Number(double value);
        static Number * New(std::string strNum);
        static Number * New(const char * strNum);
        static Number * New(double value);
        static Number * New();
        static Number * ZERO;
        static Number * NaN;

        virtual ~Number();
        std::string toString();
        bool isString();
        bool isNumber();
        bool isBoolean();
        bool isArray();
        bool isObject();
        bool isUndefined();
        Boolean * negate();

        std::string getStrType();
        static astox::Number * cast(Value * value);

        int toInt();
        double toDouble();
        bool toBoolean();


        void updatePValue(int newInt);


        Value * plusPlus();
        Value * minMin();

        astox::Value * setRecursive(std::vector<SelectorNode> &list, int index, Value * newValue, Function *& context);
        friend std::ostream& operator<<(std::ostream& output, const Number& val);
        friend std::ostream& operator<<(std::ostream& output, const Number * val);
        friend Number operator+(Number lhs, const Number& rh);
        friend Number operator-(Number lhs, const Number& rh);
        friend Number operator*(Number lhs, const Number& rh);
        friend Number operator/(Number lhs, const Number& rh);

        template<class NumberType>
        Number& operator=(const NumberType& value) {
            pvalue = value;
            return *this;
        }

        template<class NumberType>
        Number& operator+(const NumberType& value) {
            pvalue += value;
            return *this;
        }

        template<class NumberType>
        Number& operator-(const NumberType& value) {
            pvalue -= value;
            return *this;
        }

        template<class NumberType>
        Number& operator*(const NumberType& value) {
            pvalue *= value;
            return *this;
        }

        template<class NumberType>
        Number& operator/(const NumberType& value) {
            pvalue /= value;
            return *this;
        }

        template<class NumberType>
        Number& operator+=(const NumberType& rhs) {
            pvalue += rhs;
            return *this; // return the result by reference
        }

        template<class NumberType>
        Number& operator-=(const NumberType& rhs) {
            pvalue -= rhs;
            return *this; // return the result by reference
        }

        template<class NumberType>
        Number& operator*=(const NumberType& rhs) {
            pvalue *= rhs;
            return *this; // return the result by reference
        }

        template<class NumberType>
        Number& operator/=(const NumberType& rhs) {
            pvalue /= rhs;
            return *this; // return the result by reference
        }

        Number& operator++();
        Number operator++(int);
        Number& operator--();
        Number operator--(int);

        friend bool operator==(Number &cP1, Number &cP2);
        friend bool operator!=(Number &cP1, Number &cP2);

        friend bool operator<=(Number &cC1, Number &cC2);
        friend bool operator>=(Number &cC1, Number &cC2);

        friend bool operator<(Number &cC1, Number &cC2);
        friend bool operator>(Number &cC1, Number &cC2);
        friend bool operator<(Number &cC1, String &cC2);
        friend bool operator>(Number &cC1, String &cC2);


    };

    class Boolean : public astox::Value {
    private:
        bool pvalue;
    public:
        Boolean(bool value);
        static Boolean * BFALSE;
        static Boolean * BTRUE;
        static Boolean * New(bool data);
        virtual ~Boolean();
        bool isString();
        bool isNumber();
        bool isBoolean();
        bool isArray();
        bool isObject();
        bool isUndefined();
        std::string getStrType();
        std::string toString();
        bool toBoolean();
        astox::Value * setRecursive(std::vector<SelectorNode> &list, int index, Value * newValue, Function *& context);
        static Boolean * getStatic(bool data);
        Boolean * negate();





        friend std::ostream& operator<<(std::ostream& output, const Boolean& val);
        friend std::ostream& operator<<(std::ostream& output, const Boolean * val);
        friend bool operator==(Boolean &cP1, Boolean &cP2);
        friend bool operator==(Boolean * cP1, Boolean &cP2);
        friend bool operator!=(Boolean &cP1, Boolean &cP2);

    };




    //class Regex: public astox::Value{
    //	Regex();
    //};

    class Object : public astox::Value {
    public:
        Object();
        Object(const char * value);
        virtual ~Object();
        static Object * npos;
        std::string toString();

        bool hasKey(std::string name);


        astox::Value * updateValue(std::string keyName, astox::Value * newValue);
        astox::Value *& getValue(std::string keyName);
        astox::Object * getObject(std::string keyName);
        astox::Number * getNumber(std::string keyName);
        astox::Array * getArray(std::string keyName);
        astox::Value *& get(astox::Value * data);

        //TODO remove this:
        std::string getKeyToStdString(std::string keyName, bool pointSensitive = false);
        astox::Value * getKeyLoose(std::string keyName);




        static astox::Object * cast(Value * value);
        void iterate(void (onIterate) (std::string keyName, astox::Value * value));

        std::map<std::string, astox::Value *> * getObjectList();

        std::string getStrType();

        int length();

        bool isString();
        bool isNumber();
        bool isBoolean();
        bool isArray();
        bool isObject();
        bool isUndefined();
        void setKeys(std::map<std::string, astox::Value *> keys);
        Boolean * negate();
        Boolean * contains(Value * term);

    protected:

        std::map<std::string, astox::Value *>::iterator it;
        void updateGetters();
        void setGetters();
    };

    class Array : public astox::Object {
    public:
        Array();
        Array(const char * value);
        Array(Array * copy);
        Array(Array * copy, Array * concatenable);
        Array(int size);
        virtual ~Array();
        static Array * npos;

        std::string toString();

        void push(astox::Value * value);
        void push(std::string value);
        void push(int value);
        void push(bool value);

        std::vector<astox::Value *> * getItems();

        static astox::Array * cast(astox::Value * value);

        std::vector<std::string> listToString();
        void iterateArray(void (onIterate) (astox::Value * value));
        astox::Value * itemAtIndex(int index);
        astox::Value * itemAtIndex(std::string indexStr);
        astox::Value * updateValue(std::string keyName, astox::Value * newValue);
        astox::Value *& get(astox::Value * data);



        bool isArray();
        bool isObject();
        void dropData();
        bool isEmpty();
        int length();
        Value *& getValue(std::string keyName);
        std::string getStrType();
        std::vector<std::string> getKeyNameList();
        Value * reverse();
        String * join(std::string separator = ",");
        Array * sort(Function * _sortingMode = NULL);
        Value * pop();
        Value * shift();
        Value * unshift(Value * arguments);
        Array * concat(Array * arguments);
        void forEach(Function * caller = NULL);


        Array * slice(Array * arguments);
        Array * splice(Array * arguments);
        Array * slice(int start, int end);
        Array * splice(int start, int end);




    private:
        std::vector<astox::Value *> items;
        unsigned int it;
        std::vector<astox::Value *>::iterator itemsIterator;

    protected:
        void updateGetters();
        void setGetters();

        int quickSort_pivot(std::vector<Value *> &list, int first, int last);
        void quickSort_main(std::vector<Value *> &list, int first, int last);
        Function * sortingCondition;
        bool sortCompare(Value * left, Value * right);
        //	void registerDefaultFunctions();

    };

    std::ostream& operator<<(std::ostream& output, const Object& val);
    std::ostream& operator<<(std::ostream& output, const Object * val);
    std::ostream& operator<<(std::ostream& output, const Array& val);
    std::ostream& operator<<(std::ostream& output, const Array * val);





    namespace xhtml {

        struct XMLKeyNames {
            const char * attributes;
            const char * type;
            const char * childNodes;
            const char * tagName;
            const char * isVirtual;
            const char * value;
            bool run_in_OBJ;

            XMLKeyNames() {
                attributes = "@attr";
                type = "@type";
                childNodes = "@childNodes";
                tagName = "@tagName";
                isVirtual = "@isVirtual";
                value = "@text";
                run_in_OBJ = true;
            };
        };

        class XHTMLNode {
        private:
            int nodeStat;
            std::string pTagName;
            std::string attributesLine;


        protected:
            void generateAttributes();

        public:
            XHTMLNode(std::string input, bool virtualNode, bool strict = true);
            virtual ~XHTMLNode();
            void traceInfo();
            std::map<std::string, astox::Value *> attributes;
            bool hasAttributes;
            std::string inputValue;
            std::string tagName();
            int getStat();
            std::string getStatStr();
        };


        std::vector<XHTMLNode *> parse(std::string input, bool strict = true);
        void fromXHTMLNodes(std::vector<XHTMLNode *> nodes, astox::Object * buffer, astox::xhtml::XMLKeyNames names);
        void fromXHTMLNodes(std::vector<XHTMLNode *> nodes, astox::Object * buffer);
        astox::Object * fromXHTMLNode(XHTMLNode * node);
    }//exit namespace xhtml


    namespace lang {

        class HashParser {
        private:
            std::map<std::string, astox::Value *> result;
            std::string in;
            int lastGivenType;
            std::string lastReceivedInput;
        protected:

            enum hash_types {
                hash_prop = 2, hash_val, hash_npos
            };
            void receiveInput(std::string data, int type, int info);
        public:
            std::map<std::string, astox::Value *> fromHash(std::string input);
            static std::string toHash(Object * object, char separator = ' ');
            void trace();
            HashParser();
            void refresh();
        };

        static std::vector<int> scannedObjects;
        std::string toJsonLine(astox::Value * value, std::string prefix = "", std::string suffix = "", std::string parentPrefix = "");
        std::string toJSON(astox::Value * value, std::string prefix = " ", std::string suffix = "\n");
        static astox::xhtml::XMLKeyNames noKeyNames;
        std::string toXMLLine(astox::Value * value, astox::xhtml::XMLKeyNames names = noKeyNames, std::string nodeName = "", std::string prefix = " ", std::string suffix = "\n");
        std::string toXML(astox::Value * value, bool scan_as_xhtml = false);
        std::string toCSS(astox::Value * value, bool isRoot = true);
        void iterateValue(astox::Value * value, void (onIterate) (std::string keyName, astox::Value * value), std::string pKeyName = "keyOnly");





    } /* namespace lang */

    class SScriptData {
    private:
        int _operationAfter;
        std::string _buffer;
    public:
        SScriptData(std::string leftOperand, int operationAfter);
        void setOperationAfter(int _after);
        //	std::string getInnerValue();
        void debug();
        std::string getBuffer();
        int getOperation();
        static void splitCommandContent(std::string input, std::vector<SScriptData *> &list);
        //	astox::Value * execInherited(astox::Function * context, std::string input);

        static SScriptData * npos;
    };







    //class SScript {
    //protected:
    //
    //
    //private:
    //	int contextDefined;
    //	Object * pcontext;
    //	std::vector<Value *> _propBuffer;
    //public:
    //	SScript();
    //	virtual ~SScript();
    //
    //	astox::Value * runCommand(std::string str, astox::Function *& context);
    ////	astox::Value * digestList(std::vector<SScriptData *> list, astox::Function *& context, int index = 0);
    //	void setContext(astox::Object * context);
    //
    //
    //	std::string getExpressionUntil(std::string input, std::string keyword, int &found);
    //
    //
    //	static SScript Parser;
    //
    //protected:
    //
    //
    //};



    typedef Value * (*astox_function_type)(Value *);
    typedef Value * (*astox_function_prototype)(Value *, Value *);

    class Function : public Object {
    private:

        astox_function_type _methodToCall;
        astox_function_prototype _prototype_methodToCall;
        Value * _proto_Object;
        std::vector<std::string> _argsNames;
        std::string _bodyContent;
        static bool _ecmaContextCreated;
        static Function * _ecmaContext;



    public:
        static Function * npos;
        static Function * externContext;
        Value * protoype;

        Function();
        Function(Function * parent);
        Function(astox_function_type _pmethodToCall);
        Function(astox_function_prototype _p_prototype_methodToCall, Value * _p_proto_object);
        Function(std::string str_body, std::string str_args, Function * parent);

        virtual ~Function();
        Value * call(Value * arguments);
        Value * call(std::string &args, Function *&context);
        void debug();
        std::string toString();
        static Function * getEcmaContext();
        static astox::Function * cast(Value * value);
        void import(Function * parent);
        void updatePrototype(Value * val);
        bool prototypeIsObject();
        void setParent(Function *& parent);

    };

    class RegExp : public Value {
    private:
        std::string regexStrValue;
        static bool _regexMapCreated;
        AstoxRegexFlag flags;
        static AstoxRegexFlag digestMIG(std::string &in, bool scanStr = true, std::string allowedChars = "img");
    public:
        RegExp();
        RegExp(std::string data, AstoxRegexFlag pflags);
        Value * call(std::string &args, Function *& context);
        std::string toString();
        static AstoxRegexFlag strIsRegexFormat(std::string input, int &plastPoz);
        static RegExp * cast(Value * value);
        static std::map<char, AstoxRegexFlag> getRegexMap();
        AstoxRegexFlag getFlags();
    };


	struct Point {
		int x;
		int y;
	};

	

    class Date : public Value {
    private:
        std::string strDay;
        std::string strMonth;
        std::string strJson;
        std::string strStandardName;
        std::string _commonStr;
        int _day;
        int _weekDay;
        int _month;
        int _year;
        int _fullYear;
        int _hours;
        int _minutes;
        int _seconds;
        int _miliseconds;

        long _timeZoneOffset;
        long _hourOffset;

        int _utcHours;
        int _utcMins;
        int _utcDay;
        int _utcWeekDay;
        int _utcMonth;
        int _utcYear;
        int _utcFullYear;



        static const char * monthNames[];
        static const char * dayNames[];
        static int daysInMonth[];
        static Date * _dateMinOff;
        static bool _minOffsetCalculated;

        void digestDate(std::tm* timeinfo, bool global = true);
        void digestUTCDate(std::tm* utc_timeinfo);
        void setupDate(int year, int month, int day, int hours, int minutes, int seconds, int milliseconds,
                int utc_year, int utc_month, int utc_day, int utc_hours, int utc_minutes, int utc_seconds, int utc_milliseconds
                );
        int getNumberOfDaysInMonth(int year, int monthIndex);
     
		


    public:
        Date();
        Date(int year, int month, int day = 1, int hours = 0, int minutes = 0, int seconds = 0, int milliseconds = 0);
        Date(std::tm * mtime);
        Date(long in);

        std::string toString();
        static Date * getMinOffset();
        static int getDayOfWeek(int d, int m, int y);
        static bool isLeapYear(int year);
		
    #ifdef ASTOX_OS_WINDOWS
        static int SystemTime2StructTM(SYSTEMTIME *st, struct tm * ptm);
    #endif
        int getDate();
        int getDay();
        int getMonth();
        int getYear();
        int getFullYear();
        int getHours();
        int getMinutes();
        int getSeconds();
		int getMilliseconds();
		int getTimezoneOffset();
        int getHourOffset();
        int getUTCDate();
        int getUTCMonth();
        int getUTCDay();
        int getUTCMinutes();
        int getUTCHours();
        int getUTCFullYear();



		void setMilliseconds(int milliseconds);
		void setSeconds(int seconds);
		void setMinutes(int minutes);
		void setHours(int hours);
		void setDay(int hours);
		void setMonth(int month);

        std::string getCommonStr();
        std::string getStandardName();
    };

    class Math : public Value {
    private:
        Object * extension;
        Math();
    public:
        Number * getNumber(std::string keyName);
        std::string toString();
        static Math * getInstance();
        virtual ~Math();
        static Value * m_abs(Value * input);
        static Value * m_acos(Value * input);
        static Value * m_acosh(Value * input);
        static Value * m_asin(Value * input);
        static Value * m_asinh(Value * input);
        static Value * m_atan(Value * input);
        static Value * m_atan2(Value * input);
        static Value * m_atanh(Value * input);
        static Value * m_cbrt(Value * input);
        static Value * m_ceil(Value * input);
        static Value * m_clz32(Value * input);
        static Value * m_cos(Value * input);
        static Value * m_cosh(Value * input);
        static Value * m_exp(Value * input);

        static Value * mext_factorial(Value * input);
        static Value * mext_gcd(Value * input);
        static long mext_gcd(long a, long b);
    };

    class Error : public Object, std::exception {
    private:
        std::string _gravity;
        std::string _message;
        std::string _fileName;
        std::string _what;
        int _code;

    public:
        static int RANGE_ERROR;
        static int PARSING_ERROR;
        static int ASSIGNMENT_ERROR;
        Error(int gravity, std::string message, int lineNumber, const char * fileName);
        virtual const char* what() const throw ();
        std::string toString();
        ~Error() throw ();
    };

    struct ScriptValueNode {
        Value * value;
        int operation;
        void debug();
    };






    namespace ecma {
        // enum order the same as stx_keywords order

        enum KeyTypes {
            ASTXKT_VAR,
            ASTXKT_FUNCTION,
            ASTXKT_ELIF,
            ASTXKT_IF,
            ASTXKT_ELSE,
            ASTXKT_SWITCH,
            ASTXKT_WHILE,
            ASTXKT_FOR,
            ASTXKT_CASE,
            ASTXKT_RETURN,
            ASTXKT_BREAK,
            ASTXKT_TRY,
            ASTXKT_CATCH,
            ASTXKT_FINALLY,
            ASTXKT_DELETE,
            ASTXKT_THROW,
            ASTXKT_NONE,
            ASTXKT_BSLINE
        };

        extern const char * stx_keywords[16][2];
        extern int stx_size;

        struct ComposedString {
            std::string name;
            std::string arguments;
            std::string body;
            std::string fourth;
            int lastScannedPoz;
            int type;
        };

        struct ConditionStruct {
            bool condition;
            std::string body;
        };


        std::vector<std::string> splitByComma(std::string &input, char comma = ',');

        bool isComparisonTerm(int operation);
        bool isOperator(int operation);
        std::string operatorToString(int oprt);
        Number * fromStrAsNumber(std::string input);
        Array * fromStrAsArray(std::string str, Function * context = Function::npos, bool useStrict = true);
        Object * fromStrAsObject(std::string str, Function * context = Function::npos, bool useStrict = true);
        String * fromStrAsString(std::string str);
        Value * fromStr(std::string str, Function * context = Function::npos, bool useStrict = true);

        int detectStrType(std::string &input, bool useStrict = true);
        std::string getExpressionBetween(std::string input, char s, char e, int &lastPosition);
        bool stringIsWellFormed(std::string &input, char quote);
        bool charIsInString(char c, char p, int &_numberOfDoubleQuotes, int &_numberOfSingleQuotes);
        bool charIsEnclosedBy(char c, int &nump, char start = '(', char end = ')');
        bool operatorHasPriority(int operation);
        bool isShiftOperator(int operation);

        Value * executeNodes(std::vector<SScriptData *> &list,
                int index,
                std::vector<SScriptData *> &_operatorsBuffer,
                std::vector<ScriptValueNode> &_shiftersBuffer,
                int &hasShifters,
                Function *& context = Function::npos,
                int errFlags = GETR_STRICT);
        Value * compare(astox::Value * leftOperand, astox::Value * rightOperand, int operation);
        Value * operate(std::vector<SScriptData *> &list, int index, Function *& context);
        Value * operatePrioritized(std::vector<ScriptValueNode> items, Function *& context);
        Value * operateShifters(std::vector<ScriptValueNode> items, Function *& context);
        std::vector<SelectorNode> extractPatternList(std::string input);

        void generatePatternList(std::vector<SelectorNode> &list, std::string &input, char waitFor = '\0');
        Value * getMember(std::string keyName, Function * &context, int errFlags = GETR_STRICT);
        Value * setMember(std::string KeyName, Value * newValue, Function *& context);

        Value * runCommand(std::string input, Function * &context = Function::npos, int errFlags = GETR_STRICT);


        bool expectKeyword(std::string &input, const char * keyword, std::string &beforeKeyword, int &found_pos, std::string allowedChars = " (");

        void detectKeyword(std::string &input, std::string &beforeKeyword, int &detectedType, int &pozLeft, int &pozRight);
        bool isAllowedNameChar(char c);

        std::string stripComments(std::string input);

        std::string readFile(std::string fileName);
        std::string fixInput(std::string &string);
        std::string getAllUntilChar(std::string &input, int &poz, char cp);
        ComposedString splitIntoComposition(std::string &input, int substrPos);

        std::vector<int> findPosOfKeyword(std::string &input, std::string keyword);
        std::vector<std::string> splitByKeyword(std::string &input, std::string keyword);

        void detectNodes(std::string &input, std::vector<ecma::ComposedString> &data);

        int execScript(std::string &input, Function *& context, int errFlags = GETR_STRICT);
        int execIfContext(std::vector<ConditionStruct> &args, Function *& context);
        int prepareForContext(std::string &args, std::string &body, Function *& context);
        int execForContext(std::vector<std::string> &argsToExec, Function *& localContext, std::string &body);
        int execForInContext(std::string &valueName, std::string &body, std::vector<std::string> &keyNames, int index, Function *& context);
        int execWhileContext(std::string &args, std::string &body, Function *& context);
        int execTryCatchContext(ComposedString &compString, Function *&context);
        int prepareSwitchContext(std::string &args, std::string &body, Function *& context);
        int execSwitchContext(std::string &args, std::vector<ComposedString> &condcases, unsigned int index, Function *& context);
        void splitCommandContent(std::string input, std::vector<SScriptData *> &list);
        std::string getExpressionUntil(std::string input, std::string keyword, int &found);

    };

    class ECMAScript {
    private:


        std::string _emptyAllowedVarChars;
        std::string _mainInput;
        std::map<std::string, int> _ecmaKeys;
        bool lineIsComment(std::string in);

        bool charIsEmpty(char c);
        bool checkSubstr(std::string needle, std::string input, size_t& position);
        bool stringContext(char quote, std::string &input, size_t& position);
        bool checkPattern(std::string keyword, std::string any_of, std::string input, size_t& position);
    public:
        ECMAScript();
        virtual ~ECMAScript();

        void variableContext(std::string &input, bool settedByThis = false);
        //	void stringContext(std::string input, char sep);
        void functionContext(std::string input, std::string functionName);
        //	std::string undefinedContext(std::string input);


        void instructionalContext(std::string input, std::string type);




        //	std::string stripComments(std::string input);

        static bool fixLineEnding(std::string& in);
        bool expectKeyword(std::string &input, std::string keyword, std::string allowedChars = " (");




    };








}//end namespace astox


#endif /* ASTOX_H_ */
