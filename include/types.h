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
    #include "util.h"
    #include "macros.h"
	#include "stack.h"



/**
 * root namespace
 */
namespace astox {

	using namespace std;
	using namespace scriptengine;


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
		ASTOX_VALUE_ERROR,
		ASTOX_VALUE_CUSTOM = 0
	};




	/**
	 * base data-type
	 */
    class Value {
    protected:
        int valueType;
        int numCalls;
        lexic _lx;
        environment env;
        int uid;
        object_map objmap;

        Value(const lexic lxc);
        set<stxtr> _key_names;

        Object * _proto;
        Function * _constr;
        /**
         * internal usage for constructor, may return NULL
         * @param key
         * @return
         */
        virtual Value * getFromConstructor(stxtr key);
    private:
        int parentUid;

    public:
        virtual Value * getRecursive(stxtr input, const lexic &lxc,
              									Function *& context, int index, int fetchMode, Value * defaultValue = NULL);
        lexic lxc();
        static Value * npos;
        void addKey(string key, Value * value);
        void addKey(string key, size_t value);
        void addKey(string key, int value);
        void addKey(string key, string value);
        virtual int type();
        virtual const char * strtype();

        void setParentUid(int uid);
        int getParentUid();
        /**
         * local session identifier
         */
        virtual stxtr id();

        /**
         * TODO
         *  used for persistence, thread safe at generation, if setId is never called
         */
        long getId();
        virtual Value * rget(stxtr input, const scriptengine::lexic &lxc,
        									Function *& context, int fetchMode, Value * defaultValue = NULL);


        virtual Value * get(Value * data);
        virtual Value * getValue(stxtr keyName);
        virtual String * getString(stxtr keyName);
        virtual Number * getNumber(stxtr keyName);
        virtual Boolean * getBoolean(stxtr keyName);
        virtual Object * getObject(stxtr keyName);
        virtual Array * getArray(stxtr keyName);
        stxtr getStr(stxtr keyName, stxtr defaultValue = "");
        int getInt(stxtr keyName, int defaultValue = -99);
//        virtual Value * update(stxtr keyName, Value * newValue);
        virtual void updatePValue(stxtr newString);
        virtual void updatePValue(int newInt);


        virtual void dropData(); // sterge datele din interiorul clasei

        int getNumCalls();
        void setNumCalls(int num);
        virtual set<stxtr> key_names();

        //AT
        virtual Value * at(int index);
        virtual Value * at(stxtr indexStr);

        //protoutils
        virtual Value * thisInstance();
        virtual Value * prototype();
        virtual Function * constructor();

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
        virtual bool isFunction();

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
        virtual Boolean * notEquals(Value * term);
        virtual Boolean * notEqualsStrict(Value * term);
        virtual Value * orBit(Value * term);
        virtual Value * xorBit(Value * term);
        virtual Value * orStrict(Value * term);
        virtual Value * andBit(Value * term);
        virtual Value * andStrict(Value * term);
        virtual Value * shiftLeft(Value * term);
        virtual Value * shiftRight(Value * term);
        virtual Value * shiftRightUnsigned(Value * term);
        virtual Value * v_plus();
		virtual Value * v_min();
        virtual Value * v_plusplus();
        virtual Value * v_minmin();
        virtual Value * plusplus_v();
	    virtual Value * minmin_v();
	    virtual Value * typeOf();
	    virtual Value * isInstanceOf(Value * term);
	    virtual Value * existsIn(Value * term);

        virtual Boolean * contains(Value * term);
        virtual bool hasKey(stxtr name);

        /**
         * support for modify operators, like
         * +=, -=, *=, /=, <<=, >>=
         * @param term
         * @param operation
         * @return
         */
        virtual Value * modifyBy(Value * term, int operation);
        virtual Boolean * negate();
        virtual Value * negateBit();

        //TO
        virtual stxtr str();
        virtual int toInt();
        virtual double dbl();
        virtual bool bol();
        virtual Value * call(Value * arguments,  Function *&context);
        virtual Value * call(stxtr &args, const scriptengine::lexic &lexic, Function *&context);
        int eraseKey(stxtr keyName);
        String * getTypeOf();
        Boolean * isInstanceOf(stxtr keyName);







        stxtr getKeyToStdString(stxtr keyName, bool pointSensitive = false);
        astox::Value * getKeyLoose(stxtr keyName);
        Value * getKey(stxtr keyName);



        map<stxtr, Value *> * getObjectList();
        void iterate(void (onIterate) (stxtr keyName, astox::Value * value));
        friend std::ostream& operator<<(std::ostream& output, const Value& val);
        friend std::ostream& operator<<(std::ostream& output, const Value * val);

        friend bool operator<(Value &cC1, Value &cC2);
        friend bool operator>(Value &cC1, Value &cC2);

        static Object * convert(RegExpMatch regExpMatch, const lexic &l);
	    static Array * convert(vector<RegExpMatch> matches, const lexic &l);
	    static Value * duplicate(Value * in);
    };

    typedef Value * (*astox_function_type)(Value *, const lexic &_lx);
    typedef Value * (*astox_function_prototype)(Value *, Value *, const lexic &_lx);
    typedef std::map<stxtr, astox_function_prototype> astox_proto_container;



    class String : public Value {

    private:

    public:
        String(char * value, const lexic lxc);
		String(char charVal, const lexic lxc);
        static String * New(stxtr value, const lexic &lxc);
        static String * New(char value, const lexic &lxc);
        static String * New(int value, const lexic &lxc);
        static String * cast(Value * value);
        virtual ~String();
        stxtr str();

        Value * reverse();
        void append(astox::Value * data);
        bool isString();
        bool isNumber();
        bool isBoolean();
        bool isArray();
        bool isObject();
        bool isUndefined();
        Boolean * negate();

        double dbl();
        int toInt();
        bool bol();
        const char * strtype();

        const char * c_str();
        astox::Value * at(stxtr indexStr);
        astox::Value * at(int index);
        String * trim();
        String * killWhiteSpace();
        Array * split(Value * arguments);
        Array * splitByWhiteSpace(Value * arguments);
        Array * splitByChar(Value * arguments);
        String * sort();
        Number * indexOf(stxtr substr);
        Number * lastIndexOf(stxtr substr);
        String * slice(Value * arguments);
        String * slice(int start, int end);
        String * substr(Value * arguments);
        Array * match(Value * arguments);
        String * replace(Value * arguments);
	    Function * constructor();
		Value * getValue(stxtr keyName);
		Value * increment(Value * v);
    private:
        stxtr pvalue;
    };

    class Number : public Value {
    private:
        double pvalue;
    public:
        Number(double value, const lexic &l);
        static Number * New(stxtr strNum, const lexic &l);
        static Number * New(const char * strNum, const lexic &l);
        static Number * New(double value, const lexic &l);
        static Number * New(Value * from);
        static Number * ZERO;
        static Number * NaN;

        virtual ~Number();
        stxtr str();
        bool isString();
        bool isNumber();
        bool isBoolean();
        bool isArray();
        bool isObject();
        bool isUndefined();
        Boolean * negate();
        Value * v_plusplus();
	    Value * v_minmin();
        Value * plusplus_v();
       	Value * minmin_v();
       	Value * modifyBy(Value * term, int operation);
       	const char * strtype();
        static astox::Number * cast(Value * value);

        int toInt();
        double dbl();
        bool bol();
        void updatePValue(int newInt);
        Function * constructor();
        Value * increment(Value * v);
    };

    class Boolean : public astox::Value {
    private:
        bool pvalue;
    public:
        Boolean(bool value, const lexic &l);
        static Boolean * New(bool val, const lexic &l);
        static Boolean * bTrue( const lexic &l);
        static Boolean * bFalse( const lexic &l);
        virtual ~Boolean();
        bool isString();
        bool isNumber();
        bool isBoolean();
        bool isArray();
        bool isObject();
        bool isUndefined();
        const char * strtype();
        stxtr str();
        bool bol();
        Boolean * negate();
        int toInt();
        double dbl();
        Function * constructor();
    };





    class Object : public Value {
    public:
        Object(const lexic &l);
        virtual ~Object();
        static Object * npos;
        virtual stxtr str();
        static Object * cast(Value * value);
        static Object * merge(Object * left, Object * right);
        static Object * New(const lexic &l);
        const char * strtype();
        size_t length();
        bool isString();
        bool isNumber();
        bool isBoolean();
        bool isArray();
        bool isObject();
        bool isUndefined();
        void setKeys(std::map<stxtr, astox::Value *> keys);
        Boolean * negate();
        Boolean * contains(Value * term);
        Function * constructor();
    protected:
        object_iterator it;
    };

    class Array : public Value {
    public:
        Array(const lexic &l);
        Array(Array * copy, const lexic &l);
        Array(Array * copy, Array * concatenable,  const lexic &l);
        Array(int size, const lexic &l);
        virtual ~Array();
        static Array * npos;
        stxtr str();
        void push(astox::Value * value);
        void push(stxtr value);
        void push(int value);
        void push(bool value);
        std::vector<astox::Value *> * getItems();
        static Array * cast(Value * value);
        vector<stxtr> listToString();
        void iterateArray(void (onIterate) (astox::Value * value));
        Value * at(int index);
        Value * at(stxtr indexStr);
        Value * update(stxtr keyName, Value * newValue);
        astox::Value * get(Value * data);

        static Array * New(const lexic &l);
        bool isArray();
        bool isObject();
        void dropData();
        bool isEmpty();
        size_t length();
        Value * getValue(stxtr keyName);
        const char * strtype();
        set<stxtr> key_names();
        Value * reverse();
        String * join(stxtr separator = ",");
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
        Function * constructor();
        Value * increment(Value * v);
    private:
        object_list items;
        unsigned int it;
        object_list_iterator itemsIterator;
    protected:
        void updateGetters();
        void setGetters();
        int quickSort_pivot(object_list &list, int first, int last);
        void quickSort_main(std::vector<Value *> &list, int first, int last);
        Function * sortingCondition;
        bool sortCompare(Value * left, Value * right);

    };

    std::ostream& operator<<(std::ostream& output, const Object& val);
    std::ostream& operator<<(std::ostream& output, const Object * val);
    std::ostream& operator<<(std::ostream& output, const Array& val);
    std::ostream& operator<<(std::ostream& output, const Array * val);





    class Function : public Value {
    private:

        astox_function_type _simplemth;
        astox_function_prototype _protomth;
        Value * _modinstance;
        vector<stxtr> _argsNames;
        stxtr _bodyContent;
        stxtr _argsline;
        static bool _ecmaContextCreated;
        static Function * _ecmaContext;
        int returnType;
        stxtr contextKey;

    public:
        void setContextKey(stxtr key);
        void clearContextKey();
        stxtr getContextKey();
        bool hasProto(stxtr key);
        Function * getProto(stxtr key, Value * mod_instance);

        static Function * STRCNSTR;
        static Function * NUMCNSTR;
        static Function * BOLCNSTR;
        static Function * ARRCNSTR;
        static Function * OBJCNSTR;
        static Function * FUNCNSTR;

        static Function * npos;

        static Function * externContext;
        static Function * Constructor( Value * (*method)(Value *, Value *, const lexic &_lx), int rt);

        static Function * New(int returnType, const lexic &l);
        static Function * New(astox_function_type _pmethodToCall, const lexic &l, int returnType = ASTOX_VALUE_UNDEFINED);
        static Function * New(astox_function_prototype _p_prototype_methodToCall, Value * _p_proto_object, const lexic &l, int returnType = ASTOX_VALUE_UNDEFINED);
        static Function * New(stxtr str_body, stxtr str_args, Function * parent, const lexic &l);

        Function(stxtr string_body,
        		stxtr string_args,
        		Function * parent,
        		astox_function_type simple_method,
        		astox_function_prototype proto_method,
        		Value * instance,
        		const lexic &l,
        		int returnType = ASTOX_VALUE_UNDEFINED);
        int getReturnType();
        virtual ~Function();
        /**
         * signature used by constructors
         * @param arguments
         * @param _instance
         * @param context
         * @return
         */
//        Value * call(Value * arguments, Value * _instance, Function *&context);
        Value * call(Value * arguments, Function *&context);
        Value * call(stxtr &args,  const lexic &l, Function *&ctx);
        void debug();
        stxtr str();
        static Function * buildStandardContext(lexic &lxc);
        static Function * cast(Value * value);
        void import(Function * parent);


        void setParent(Function *& parent);

        const char * strtype();
        Function * clone(Value * minst = NULL);
        stxtr id();

        void setGetters();
        void updateGetters();
        Value * thisInstance();
        Function * addPrototype(stxtr key, Value * (*method)(Value *, Value *, const lexic &_lx), const lexic &l);
        Function * constructor();
    };



    class RegExp : public Value {
    private:
    	string rstr;
        stx_regex_flag flags;
        static stx_regex_flag digestMIG(stxtr &in, bool scanStr = true, stxtr allowedChars = "img");
    public:
        RegExp(string data, stx_regex_flag pflags, const lexic l);
        RegExp(const lexic l);
        Value * call(stxtr &args, Function *& context);
        stxtr str();
        static RegExp * cast(Value * value);
        stx_regex_flag getFlags();
        const char * strtype();
        Value * exec(Value * val);
    };



	

    class Date : public Value {
    private:
        stxtr strDay;
        stxtr strMonth;
        stxtr strJson;
        stxtr strStandardName;
        stxtr _commonStr;
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

        stxtr str();
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

        stxtr getCommonStr();
        stxtr getStandardName();
    };

    class Math : public Value {
    private:
        Object * extension;
        Math();
    public:
        Number * getNumber(stxtr keyName);
        stxtr str();
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
    	const char * src;
    	const char * mth;
    	int line;
    	stxtr strmsg;

    public:
        Error(stxtr msg, const char * source, const char * method, int line = -1);
        virtual const char* what() const throw ();
        stxtr str();
        ~Error() throw ();
        const char * getSource(){ return  src; }
        const char * getMethod(){ return  mth; }
        int getLine(){ return  line; }
        stxtr getMessage(){ return  strmsg; }
        static void throwValue(Value * value);
	};



#ifdef ASTOX_COMPILER_MSC
	#ifdef STX_ENABLE_JNI
		#define throw_error(...){\
				char a[ASTX_MAX_BUFSZ]; sprintf_s(a, ASTX_MAX_BUFSZ, __VA_ARGS__); stxtr r(a); stxtr f(__FILENAME__);\
				astox::JNIContainer::instance()->throwErr(r, f.c_str(), ASTX_CURRENT_FUNCTION, __LINE__); }
	#else
		#define throw_error(...){ \
			char argstr[ASTX_MAX_BUFSZ]; sprintf_s(argstr, ASTX_MAX_BUFSZ, __VA_ARGS__); \
			astox::Error n(argstr, __FILENAME__, ASTX_CURRENT_FUNCTION, __LINE__); throw n; }
	#endif
#else
	#ifdef STX_ENABLE_JNI
        #ifdef __ANDROID__
            #define throw_error(...){ char a[ASTX_MAX_BUFSZ]; \
                    snprintf(a, ASTX_MAX_BUFSZ, __VA_ARGS__);  stxtr er(a); stxtr fn(__FILENAME__); stxtr gx(ASTX_CURRENT_FUNCTION); \
                    astox::JNIContainer::instance()->throwErr(er, fn.c_str(), gx.c_str(), __LINE__); }
        #else
            #define throw_error(...){ char a[ASTX_MAX_BUFSZ]; \
                     snprintf(a, ASTX_MAX_BUFSZ, __VA_ARGS__);  stxtr er(a); stxtr fn(__FILENAME__); stxtr gx(ASTX_CURRENT_FUNCTION); \
                     astox::JNIContainer::instance()->throwErr(er, fn.c_str(), gx.c_str(), __LINE__); }
        #endif
    #else
		#define throw_error(...){ char argstr[ASTX_MAX_BUFSZ]; \
			printf(__VA_ARGS__); snprintf(argstr, ASTX_MAX_BUFSZ, __VA_ARGS__); \
			astox::Error n(argstr, __FILENAME__, ASTX_CURRENT_FUNCTION, __LINE__); throw n; }
	#endif
#endif










    stxtr blurt(Value * value);

}//end namespace astox


#endif /* ASTOX_H_ */
