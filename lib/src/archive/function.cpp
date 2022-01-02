/*
 * Function.cpp
 *
 *  Created on: Mar 13, 2014
 *      Author: alexandru
 */

#include <astox.h>

namespace astox {
	using namespace scriptengine;
	using namespace std;
	using namespace ecma::methods;
	using namespace testing;

    bool Function::_ecmaContextCreated = false;

	/*
	 Function * Function::npos = new Function("npos", "", NULL, NULL, NULL, NULL, Syntax::JS, ASTOX_VALUE_UNDEFINED);
	Function * Function::STRCNSTR = Function::Constructor(String_constructor, ASTOX_VALUE_STRING);
    Function * Function::NUMCNSTR = Function::Constructor(Number_constructor,  ASTOX_VALUE_NUMBER);
    Function * Function::BOLCNSTR = Function::Constructor(Boolean_constructor, ASTOX_VALUE_BOOL);
    Function * Function::ARRCNSTR = Function::Constructor(Array_constructor, ASTOX_VALUE_ARRAY);
    Function * Function::OBJCNSTR = Function::Constructor(Object_constructor, ASTOX_VALUE_OBJECT);
    Function * Function::FUNCNSTR = new Function("funcnstr", "", NULL, NULL, NULL, NULL, Syntax::JS, ASTOX_VALUE_UNDEFINED);
	
	*/
   
	Function * Function::npos = NULL;
	Function * Function::STRCNSTR = NULL;
    Function * Function::NUMCNSTR = NULL;
    Function * Function::BOLCNSTR = NULL;
    Function * Function::ARRCNSTR = NULL;
    Function * Function::OBJCNSTR = NULL;
    Function * Function::FUNCNSTR = NULL;

    Function * Function::_ecmaContext = NULL;

    Function * Function::buildStandardContext(lexic &lxc) {

			Function::npos = new Function("npos", "", NULL, NULL, NULL, NULL, Syntax::JS, ASTOX_VALUE_UNDEFINED);
			Function::STRCNSTR = Function::Constructor(String_constructor, ASTOX_VALUE_STRING);
			Function::NUMCNSTR = Function::Constructor(Number_constructor, ASTOX_VALUE_NUMBER);
			Function::BOLCNSTR = Function::Constructor(Boolean_constructor, ASTOX_VALUE_BOOL);
			Function::ARRCNSTR = Function::Constructor(Array_constructor, ASTOX_VALUE_ARRAY);
			Function::OBJCNSTR = Function::Constructor(Object_constructor, ASTOX_VALUE_OBJECT);
			Function::FUNCNSTR = new Function("funcnstr", "", NULL, NULL, NULL, NULL, Syntax::JS, ASTOX_VALUE_UNDEFINED);
	
       
            _ecmaContext = Function::New(ASTOX_VALUE_NUMBER, lxc);
            _ecmaContext->addKey(lxc.env.printfnc, Function::New(print, lxc, ASTOX_VALUE_CUSTOM));
            _ecmaContext->addKey("expect_stdout", new ExpectStdout());

            Function::ARRCNSTR->addPrototype(lxc.env.tostring, proto_toString, lxc)
					->addPrototype(lxc.env.reverse, array_reverse, lxc)
					->addPrototype(lxc.env.push, array_push, lxc)
            		->addPrototype(lxc.env.tojson, proto_toJSON, lxc);
            _ecmaContext->addKey(lxc.env.array_construct, Function::ARRCNSTR);

            Function::STRCNSTR->addPrototype(lxc.env.tostring, proto_toString, lxc)
            		->addPrototype(lxc.env.tojson, proto_toJSON, lxc)
            		->addPrototype(lxc.env.reverse, array_reverse, lxc)
            		->addPrototype(lxc.env.charat, string_charAt, lxc)
            		->addPrototype(lxc.env.trim, string_trim, lxc)
            		->addPrototype(lxc.env.split, string_split, lxc)
            		->addPrototype(lxc.env.splitbywhitespace, string_splitByWhiteSpace, lxc)
            		->addPrototype(lxc.env.splitbychar, string_splitByChar, lxc)
            		->addPrototype(lxc.env.sort, array_sort, lxc)
            		->addPrototype(lxc.env.indexof, array_indexOf, lxc)
            		->addPrototype(lxc.env.search, array_indexOf, lxc)
            		->addPrototype(lxc.env.replace, string_replace, lxc)
            		->addPrototype(lxc.env.killwhitespace, string_killWhiteSpace, lxc);
            _ecmaContext->addKey(lxc.env.string_construct, Function::STRCNSTR);

            Function::OBJCNSTR->addPrototype(lxc.env.tostring, proto_toString, lxc)
            		->addPrototype(lxc.env.tojson, proto_toJSON, lxc);
            _ecmaContext->addKey(lxc.env.object_construct,  Function::OBJCNSTR);

            Function::NUMCNSTR->addPrototype(lxc.env.tostring, proto_toString, lxc)
					->addPrototype(lxc.env.tojson, proto_toJSON, lxc);
            _ecmaContext->addKey(lxc.env.number_construct, Function::NUMCNSTR);

            Function::FUNCNSTR->addPrototype(lxc.env.tostring, proto_toString, lxc)
            					->addPrototype(lxc.env.tojson, proto_toJSON, lxc);

            Function::BOLCNSTR->addPrototype(lxc.env.tostring, proto_toString, lxc)
          					->addPrototype(lxc.env.tojson, proto_toJSON, lxc);
            _ecmaContext->addKey(lxc.env.boolean_construct, Function::BOLCNSTR);


            _ecmaContext->addKey(lxc.env.regexp_construct, new RegExp(lxc));
            _ecmaContext->addKey("astox_info", Function::New(astox_info, lxc, ASTOX_VALUE_OBJECT));


            Function * dateConstructor = Function::New(Date_constructor, new Object(lxc), lxc, ASTOX_VALUE_DATE);
            _ecmaContext->addKey("Date", dateConstructor);

            Function * errorConstructor = Function::New(Error_constructor, new Object(lxc), lxc, ASTOX_VALUE_DATE);
            _ecmaContext->addKey("Error", errorConstructor);

            Function * setIntervalFunction = Function::New(setInterval, new Object(lxc), lxc, ASTOX_VALUE_UNDEFINED);
            _ecmaContext->addKey("setInterval", setIntervalFunction);

            Function * requireConstructor = Function::New(Require_constructor, new Object(lxc), lxc, ASTOX_VALUE_UNDEFINED);
            _ecmaContext->addKey("require", requireConstructor);

			return _ecmaContext;
    }


    Function::Function(stxtr string_body,
    		stxtr string_args,
    		Function * parent,
    		astox_function_type simple_method,
    		astox_function_prototype proto_method,
    		Value * instance,
    		const lexic &l,
    		int rt): Value(l){
		valueType = ASTOX_VALUE_FUNCTION;
    	_lx = l;
    	returnType = rt;
    	_simplemth = simple_method;
    	_protomth = proto_method;
    	_modinstance = instance;
    	_bodyContent = string_body;
    	_argsline = string_args;
    	if(!string_args.empty()){
		  _argsNames = scriptengine::split_by_comma(string_args, _lx);
    	}
    	if(parent != NULL){
    		map<string, Value *>::iterator it;
			for (it = parent->objmap.begin(); it != parent->objmap.end(); it++) {
				objmap[it->first] = it->second;
			}
    	}
    };

    Function * Function::New(int pr, const lexic &l){
    	return new Function(l.env.native_func, "", NULL, NULL, NULL, NULL, l, pr);
    };
    Function *  Function::New(astox_function_prototype _p_prototype_methodToCall, Value * _p_proto_object, const lexic &l, int returnType){
    	return new Function(l.env.native_func, "", NULL, NULL,_p_prototype_methodToCall, _p_proto_object,  l, returnType);
    };
    Function *  Function::New(astox_function_type methodToCall, const lexic &l, int pr) {
    	return new Function(l.env.native_func, "", NULL, methodToCall, NULL, NULL, l, pr);
    };
    Function *  Function::New(stxtr str_body, stxtr str_args, Function * parent, const lexic &l){
    	return new Function(str_body, str_args, parent, NULL, NULL, NULL, l);
    };
    Function * Function::Constructor( Value * (*method2args)(Value *, Value *, const lexic &_lx),  int rt){
    	return new Function("CNSTRCT", "", NULL, NULL, method2args, NULL, Syntax::JS, rt);
    };
    
    Function * Function::addPrototype(stxtr key,  Value * (*method)(Value *, Value *, const lexic &_lx), const lexic &l){
    	if(_proto == NULL){
    		_proto = new Object(l);
    	}
    	_proto->addKey(key, new Function(l.env.native_func, "", NULL, NULL, method, NULL, l));
    	return this;
    };

    void  Function::setGetters(){

    };
    void  Function::updateGetters(){

	  };

    bool Function::hasProto(stxtr key){
    	return _proto != NULL && _proto->hasKey(key);
    };

    Function * Function::getProto(stxtr key, Value * mod_instance){
    	return Function::cast(_proto->getKey(key))->clone(mod_instance);
    };

    Value * Function::thisInstance(){
    	if(_modinstance == NULL){
    		return this;
    	}
    	return _modinstance;
    };



    //deprecated, use call with context
    void Function::setParent(Function *& parent) {

    };



    Value * Function::call(std::string &args,  const scriptengine::lexic &l, Function *&ctx){
    	log_stack("call string [%s] in context [%s]", args.c_str(), blurt(ctx).c_str());

    	//define parent context
    	std::map<std::string, astox::Value *>::iterator it;
		for (it = ctx->getObjectList()->begin(); it != ctx->getObjectList()->end(); it++) {
			objmap[it->first] = (it->second);
		}
		Function * thisct = (this);
		if (!ctx->getContextKey().empty()) {
			thisct->setContextKey(ctx->getContextKey());
		}
		Value * argument = execute_operation_command(args, l, thisct);
		log_stack("argument = %s", blurt(argument).c_str());
	

		return call(argument, thisct); //call(scriptengine::runCommand(args, context));
    };

    Value * Function::call(Value * arguments,  Function *&context) {
        if (_protomth != NULL) {
            numCalls++;
            return _protomth(arguments, _modinstance, _lx);
        }

        if (_simplemth != NULL) {
            numCalls++;
            return _simplemth(arguments, _lx);
        }


		numCalls++;


        if (!_bodyContent.empty()) {
			//leave this block here to support mapping when called directly
			objmap["arguments"] = arguments;
			if (!_argsNames.empty()) {
				if (arguments->isArray()) {
					Array * arr = Array::cast(arguments);
					for (int i = 0; i < _argsNames.size(); i++) {
						objmap[_argsNames.at(i)] = (arr->at(i));
					}
				}
				else {
					objmap[_argsNames.at(0)] = arguments;
				}
			}

        	//duplicate context to support recursive calls
        	Function * copyctx = clone();
            stxtr copybody = _bodyContent;
            Value * retval = scriptengine::eval(copybody, _lx, copyctx);
			clearContextKey();
            return retval;
        }

        return this;
    };

    void Function::debug() {
        method_not_implemented();
    }

    const char * Function::strtype(){
    	return _lx.env.func;
    };

    stxtr Function::str() {
    	stxtr rsp = _lx.func;
    	rsp +=(_lx.wspace);
    	rsp+=_lx.l_parenthesis;
		for (unsigned int i = 0; i < _argsNames.size(); i++) {
			if (i > 0) {
				rsp+=_lx.comma;
			}
			rsp.append(_argsNames.at(i));
		}
		rsp+=_lx.r_parenthesis;
		rsp+=_lx.l_bracket;
		rsp.append(_bodyContent);
		rsp+=_lx.r_bracket;
		return rsp;
    }

    Function::~Function() {
        // TODO Auto-generated destructor stub
    }

    void Function::import(Function * parent) {
        using namespace std;
        for (std::map<string, Value *>::iterator it = parent->getObjectList()->begin();
                it != parent->getObjectList()->end(); ++it) {
            objmap[it->first] = it->second;
        }
    }




    int Function::getReturnType(){
    	return returnType;
    };


    Function * Function::clone(Value * minst){
    	object_iterator it;
    	if(minst == NULL){
    		return new Function(_bodyContent, _argsline, this,
        			_simplemth, _protomth, _modinstance, _lx, returnType);
    	}

		return new Function(_bodyContent, _argsline, this,
    			_simplemth, _protomth, minst, _lx, returnType);
    };


    /**
     * used for key iteration, if in object {i:b[i]} we need i = 'a'
     * @param key
     */
    void Function::setContextKey(stxtr key){
    	contextKey = key;
    };

    void Function::clearContextKey(){
	    contextKey.clear();
    };

    stxtr Function::getContextKey(){
    	return contextKey;
    }


    stxtr Function::id(){
		return _bodyContent;
    };

    Function * Function::cast(Value * value) {
		#ifdef ASTX_VALUE_STATIC_CAST
			return static_cast<Function *> (value);
		#else
			return dynamic_cast<Function *> (value);
		#endif
    };


    Function *  Function::constructor(){
    	if(_constr == NULL){
    		_constr = Function::FUNCNSTR;
    	}
    	return _constr;
    };









} /* namespace astox */
