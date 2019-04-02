/*
 * Value.cpp
 *
 *  Created on: Jan 26, 2014
 *      Author: Alexandru
 */

#include <astox.h>
//#ifndef __STDC_IEC_559__
//#error "Requires IEEE 754 floating point!"
//#endif

namespace astox {

    using namespace std;
    using namespace scriptengine;
    using namespace serializers;
    using namespace util;
    using namespace stringAlg;

    Value * Value::npos = new Value(Syntax::JS);

    Value::Value(const lexic lexic) {
    	_lx = lexic;
    	env = lexic.env;
        valueType = ASTOX_VALUE_UNDEFINED;
        parentUid = -1;
        static int ASTOX_NUM_INSTANCES = 1;
        uid = ASTOX_NUM_INSTANCES++;
        numCalls = 0;
        _proto = NULL;
        _constr = NULL;
    }




    Object * Value::convert(RegExpMatch r, const lexic &l){
    	Object * obj = new Object(l);
    	obj->addKey("start", r.start);
    	obj->addKey("end", r.end);
    	obj->addKey("part", r.part);
    	return obj;
    }

    Array * Value::convert(vector<RegExpMatch> matches, const lexic &l){
    	 Array * a = Array::New(l);
    	 for(int i = 0; i < matches.size(); i++){
    		 a->push(convert(matches.at(i), l));
    	 }
    	 return a;
    };


    bool Value::hasKey(stxtr name) {
        return (objmap.find(name) != objmap.end());
    };


    map<stxtr, Value *> * Value::getObjectList() {
        return &objmap;
    }

    void Value::iterate(void (onIterate) (std::string keyName, astox::Value * value)) {
        object_iterator it;
    	for (it = objmap.begin(); it != objmap.end(); it++) {
            onIterate(it->first, it->second);
        }
    };



    void Value::addKey(stxtr key, Value * value) {
        key = replace(key, "\"", "");
        key = trim(key);
        objmap[key] = value;
        _key_names.insert(key);
    }

    void Value::addKey(string key, int v){
    	addKey(key, Number::New(v, _lx));
    };

    void Value::addKey(string key, size_t v){
    	addKey(key, Number::New(static_cast<double>(v) , _lx));
    };

    void Value::addKey(string key, string v){
    	addKey(key, String::New(v, _lx));
    };

    int Value::getNumCalls() {
        return numCalls;
    };

    /**
	 * @Deprecated
	 * @param arguments
	 * @param context
	 * @return
	 */
    void Value::setNumCalls(int num) {
        numCalls = num;
    };

    /**
     * @Deprecated
     */
    void Value::dropData() {

    }

    Value * Value::call(Value * arguments,  Function *&context) {
        return this;
    };

    Value * Value::call(std::string &args,  const scriptengine::lexic &l, Function *&ctx) {
        return this;
    }

    bool Value::isEmpty() {
        return true;
    }
    /**
	 * @Deprecated
	 */
    void Value::updatePValue(std::string newString) {

    };
    /**
	 * @Deprecated
	 */
    void Value::updatePValue(int newInt) {

    };

    stxtr Value::str() {
        return _lx.env.undefined;
    }

    int Value::type() {
        return valueType;
    }

   const char * Value::strtype() {
        return _lx.env.undefined;
    }

    void Value::setParentUid(int uid) {
        parentUid = uid;
    }

    Boolean * Value::contains(Value * term) {
        return Boolean::bFalse(_lx);
    }

    int Value::getParentUid() {
        return parentUid;
    }

    stxtr Value::id() {
        return "";
    }


    long Value::getId() {
            return 0;
    }

    int Value::toInt() {
        return 0;
    }

    Value * Value::existsIn(Value * term){
    	if(term->isObject()){
    		object_map * om = Object::cast(term)->getObjectList();
    		for(object_iterator it = om->begin(); it != om->end(); ++it){
    			if(it->first == str()){
    				return Boolean::bTrue(_lx);
    			};
    		}
    	}
    	return Boolean::bFalse(_lx);
    };

    double Value::dbl() {
        return 0;
    }

    bool Value::isString() {
        return (valueType == ASTOX_VALUE_STRING);
    }

    bool Value::isNumber() {
        return (valueType == ASTOX_VALUE_NUMBER);
    }

    bool Value::isFunction(){
    	 return (valueType == ASTOX_VALUE_FUNCTION);
    };

    bool Value::isBoolean() {
        return (valueType == ASTOX_VALUE_BOOL);
    }

    bool Value::isArray() {
        return (valueType == ASTOX_VALUE_ARRAY);
    }

    bool Value::isObject() {
        return (valueType == ASTOX_VALUE_OBJECT);
    }

    bool Value::isUndefined() {
        return (valueType == ASTOX_VALUE_UNDEFINED);
    }

    Value * Value::getKeyLoose(std::string keyName) {

         object_iterator it = objmap.find(keyName);
         if (it == objmap.end()) { // not found
             objmap[keyName] = Value::npos;
             return objmap[keyName];
         }
         return objmap[keyName];
     }

//     const char * Value::getWithDefault(std::string kn, const char * (callback)(const char * message)){
//    	object_iterator it = objmap.find(kn);
// 		if (it == objmap.end()) {
// 			return callback(kn.c_str());
// 		}
// 		return objmap[kn]->str().c_str();
//     };

    stxtr Value::getStr(stxtr kn, stxtr dv){
    	object_iterator it = objmap.find(kn);
 		if (it == objmap.end()) {
 			return dv;
 		}
 		return objmap[kn]->str();
     };

     int Value::getInt(stxtr kn, int dv){
    	 object_iterator it = objmap.find(kn);
 		if (it == objmap.end()) {
 			return dv;
 		}
 		return objmap[kn]->toInt();
     };

     /**
      * This method returns NULL if value is not found
      * @param n
      * @return
      */
     Value * Value::getKey(stxtr n){
 		 if (objmap.find(n) == objmap.end()) { // not found
 			return NULL;
 		 }
 		 return objmap[n];
     };

     Value * Value::get(Value * data) {
         return getValue(data->str());
     }

     Value * Value::getValue(stxtr keyName) {
    	object_iterator it = objmap.find(keyName);
        if (it == objmap.end()) { // not found
            return Value::npos;
        }
        return objmap[keyName];
     }

    String * Value::getString(std::string keyName) {
        return new String('\0', Syntax::JS); //TODO String::npos
    }

    Value * Value::getFromConstructor(stxtr key){
    	if(_proto == NULL){
			_proto = new Object(_lx);
		}

		if(constructor()->hasProto(key) && !_proto->hasKey(key)){
			_proto->addKey(key, _constr->getProto(key, this));
		}

		if(_proto->hasKey(key)){
			return _proto->getKey(key);
		}
    	return NULL;
    };

    Boolean * Value::getBoolean(std::string keyName) {
        return Boolean::New(false, _lx);
    }




    int Value::length() {
        return 0;
    }

    Value * Value::at(int index) {
        return Value::npos;
    };

    Value * Value::at(stxtr indexStr) {
        return Value::npos;
    };


    Object * Value::getObject(std::string keyName) {
        object_iterator it = objmap.find(keyName);
        if (it == objmap.end()) { // not found
            return Object::npos;
        }
		#ifdef ASTX_VALUE_STATIC_CAST
				return static_cast<Object *> (it->second);
		#else
				return dynamic_cast<Object *> (it->second);
		#endif
    }



    astox::Array * Value::getArray(std::string keyName) {
    	object_iterator it = objmap.find(keyName);
        if (it == objmap.end()) { // not found
            return Array::New(_lx); //TODO modify
        }
		#ifdef ASTX_VALUE_STATIC_CAST
				return static_cast<Array *> (it->second);
		#else
				return dynamic_cast<Array *> (it->second);
		#endif
    }


   Number * Value::getNumber(std::string keyName) {
        object_iterator it = objmap.find(keyName);
        if (it == objmap.end()) { // not found
            return Number::NaN;
        }
		#ifdef ASTX_VALUE_STATIC_CAST
			return static_cast<Number *> (it->second);
		#else
			return dynamic_cast<Number *> (it->second);
		#endif
    }

   stxtr Value::getKeyToStdString(stxtr keyName, bool pointSensitive) {
       object_iterator it = objmap.find(keyName);
       if (it == objmap.end()) { // not found
           return keyName;
       }
       return it->second->str();
   }


    /**
     * var++ returns its value before incrementing
     * @return
     */
    Value * Value::v_plusplus() {
    	return this;
    };

    Value * Value::v_minmin() {
    	return this;
    };

    Value * Value::v_plus(){
    	return Number::New(toInt(), _lx);
    };

	Value * Value::v_min(){
		return Number::New(toInt() * -1, _lx);
	};

    /**
     * ++var returns its value after incrementing.
     * @return
     */
    Value * Value::plusplus_v() {
		return this;
	};

    /**
     * --var returns its value after incrementing.
     * @return
     */
	Value * Value::minmin_v() {
		return this;
	}

    bool Value::bol() {
    	if(toInt() == 0) return false;
        return true;
    }

    Value * Value::duplicate(Value * in) {
		if (!in) {
			return Value::npos;
		}
        switch (in->type()) {
            case ASTOX_VALUE_BOOL:
                return Boolean::New(in->bol(), in->lxc());
            case ASTOX_VALUE_NUMBER:
                return Number::New(in->dbl(), in->lxc());
            case ASTOX_VALUE_STRING:
                return String::New(in->str(), in->lxc());
            case ASTOX_VALUE_FUNCTION:
            	return Function::cast(in)->clone();

        }
        return in;
    }

    Boolean * Value::isBiggerThan(Value * term) {
        return Boolean::New(dbl() > term->dbl(), _lx);
    }

    Boolean * Value::isLessThan(Value * term) {
        return Boolean::New(dbl() < term->dbl(), _lx);
    }

    Boolean * Value::isBiggerEqThan(Value * term) {
        return Boolean::New(dbl() >= term->dbl(), _lx);
    }

    Boolean * Value::isLessEqThan(Value * term) {
        return Boolean::New( dbl() <= term->dbl(), _lx);
    }

    Value * Value::increment(Value * t) {
        if (valueType == ASTOX_VALUE_UNDEFINED ||
        		t->type() == ASTOX_VALUE_UNDEFINED) {
            return String::New(_lx.env.nanstr, _lx);
        }

        if (isString() || t->isString()) {
            return String::New(str().append(t->str()), _lx);
        }

        if(isArray() && t->isArray()){
        	method_not_implemented();
        }

		if (isBoolean() && t->isArray()) {
			stxtr h = str();
			h.append(to_csv(t));
			return String::New(h, _lx);
		}

        if(isObject() && t->isObject()){
        	return Object::merge(Object::cast(this), Object::cast(t));
		}

        return Number::New( dbl() + t->dbl(), _lx);
    }

    Value * Value::shiftRight(Value * term) {
        int left = toInt();
        int right = term->toInt();
        int result = left >> right;
        return Number::New(result, _lx);
    }

    Value * Value::shiftRightUnsigned(Value * term){
    	int left = toInt();
		int right = term->toInt();
		unsigned int result = (unsigned int) left >> right;
		return Number::New(result, _lx);
    };

    Value * Value::shiftLeft(Value * term) {
        int left = toInt();
        int right = term->toInt();
        int result = left << right;
        return Number::New(result, _lx);
    }

    Value * Value::substract(Value * term) {
    	if(isNumber() && term->isNumber()){
    		double sub = dbl() - term->dbl();
    		return Number::New(sub, _lx);
    	}
    	return Number::New(toInt() - term->toInt(), _lx);
    };

    Value * Value::multiply(Value * term) {
    	if( isNumber() && term->isNumber()){
		  double mult = dbl() * term->dbl();
		  return Number::New(mult, _lx);
    	}
        return Number::New(toInt() * term->toInt(), _lx);
    };

    Value * Value::divide(Value * term) {
    	if(isNumber() && term->isNumber()){
			double div = dbl() / term->dbl();
			return Number::New(div, _lx);
    	}
    	return Number::NaN;
    };

    Value * Value::modulus(Value * term) {
    	if(isNumber() && term->isNumber()){
			double left = toInt();
			double right = term->toInt();
			return Number::New(fmod(left, right), _lx);
    	}
    	return Number::NaN;
    }

    Boolean * Value::equals(Value * term) {
        bool comp = str() == term->str();
        if (valueType == ASTOX_VALUE_NUMBER && term->type() == ASTOX_VALUE_NUMBER) {
            comp = dbl() == term->dbl();
        }
        return Boolean::New(comp, _lx);
    };

    Boolean * Value::equalsStrict(Value * term) {
    	bool comp = (dbl() == term->dbl()) && (type() == term->type());
        return Boolean::New(comp, _lx);
    };

    Boolean * Value::notEquals(Value * term) {
        bool div = toInt() != term->toInt();
        return Boolean::New(div, _lx);
    }

    Boolean * Value::notEqualsStrict(Value * term) {
        bool div = (dbl() != term->dbl()) || (type() != term->type());
        return Boolean::New(div, _lx);
    }

    //TODO check for string and int types
    Value * Value::orBit(Value * term) {
        return Number::New((toInt() | term->toInt()), _lx);
    }

    Value * Value::orStrict(Value * term) {
    	if(this->isBoolean() && term->isBoolean()){
    		return Boolean::New((toInt() || term->toInt()), _lx);
    	}
    	if(isBoolean() && bol() == true){
    		return Boolean::New((toInt() || term->toInt()), _lx);
    	}
    	if(this->toInt() == 0 && term->isBoolean() && term->bol() == false){
    		return Boolean::New(false, _lx);
    	}

        return Number::New((toInt() || term->toInt()), _lx);
    }

    Value * Value::andBit(Value * term) {
        return Number::New((toInt() & term->toInt()), _lx);
    }

    Value * Value::xorBit(Value * term){
    	 return Number::New((toInt() ^ term->toInt()), _lx);
    };

    Value * Value::andStrict(Value * term) {

    	if(this->isBoolean() && term->isBoolean()){
			return Boolean::New((toInt() && term->toInt()), _lx);
		}
    	if(isBoolean() && bol() == false){
			return Boolean::New((toInt() && term->toInt()), _lx);
		}
		if(this->toInt() == 0 && term->isBoolean() && term->bol() == false){
			return Boolean::New(false, _lx);
		}

        return Number::New((toInt() && term->toInt()), _lx);
    }

    Value * Value::modifyBy(Value * term, int operation) {
        return this;
    }

    Boolean * Value::negate() {
    	if(isArray() || isObject()){
    		 return Boolean::New(false, _lx);
    	}
        return Boolean::New(!toInt(), _lx);
    }

    Value * Value::negateBit() {
    	int r = toInt();
    	return Number::New(~r, _lx);
    };


    //TODO create statics

    String * Value::getTypeOf() {
        switch (valueType) {
            case ASTOX_VALUE_UNDEFINED:
                return String::New(_lx.env.undefined, _lx);
            case ASTOX_VALUE_ARRAY:
            case ASTOX_VALUE_OBJECT:
                return String::New(_lx.env.object, _lx);
            case ASTOX_VALUE_BOOL:
                return String::New(_lx.env.boolean, _lx);
            case ASTOX_VALUE_NUMBER:
                return String::New(_lx.env.number, _lx);
            case ASTOX_VALUE_STRING:
                return String::New(_lx.env.string, _lx);
        }
        return String::New(_lx.env.object, _lx);
    }

    Boolean * Value::isInstanceOf(std::string keyName) {
        keyName = stringAlg::trim(keyName);

        std::transform(keyName.begin(), keyName.end(), keyName.begin(), ::tolower);

        cout << " keyname = " << keyName << endl;
        switch (valueType) {

            case ASTOX_VALUE_STRING:
                if (keyName == "string") {
                    return Boolean::New(true, _lx);
                }
                break;
            case ASTOX_VALUE_ARRAY:
                if (keyName == "array") {
                    return Boolean::New(true, _lx);
                }
                break;

        }


        switch (valueType) {
            case ASTOX_VALUE_OBJECT:
            case ASTOX_VALUE_ARRAY:
            case ASTOX_VALUE_FUNCTION:
            case ASTOX_VALUE_CUSTOM:
                if (keyName == "object") {
                    return Boolean::New(true, _lx);
                }
                break;
        }

        return Boolean::New(false, _lx);
    };

    //TODO return non setted values
    set<stxtr> Value::key_names() {
       return _key_names;
    }

    int Value::eraseKey(std::string keyName) {
        if (objmap.find(keyName) != objmap.end()) {
            objmap.erase(objmap.find(keyName));
            return 1;
        }
        return 0;
    }




    lexic Value::lxc(){
    	return _lx;
    }

   Value * Value::typeOf(){
	   switch(valueType){
		   case ASTOX_VALUE_ARRAY:
			   return String::New("array", _lx);
		   case ASTOX_VALUE_NUMBER:
			   return String::New("number", _lx);
	   }

	   return String::New("to_do", _lx);
   };

   Value * Value::isInstanceOf(Value * term) {
	   if(!term->isFunction()){
		   return Boolean::New(term->type() == type(), _lx);
	   }
	   return Boolean::New(Function::cast(term)->getReturnType() == type(), _lx);
   };

    astox::Value * Value::rget(string in, const lexic &l, Function *&context, int fetchMode, Value * dv){
    	return getRecursive(in, l, context, 0, fetchMode, dv);
    }

    /**
     *
     * @param in
     * @param l
     * @param context
     * @param index used to detect first item encapsulation like ( )
     * @param defaultValue
     * @return
     */
    Value * Value::getRecursive(string in, const lexic &l,
    		Function *&context, int index, int fetchMode, Value * dv) {

    	in = trim(in);


    	if(in == l.vthis){
    		return this;
    	}
		 command_node cmd;
		 detect_instruction_type(in, &cmd, l);
		 if(string_is_func_format(in, l)) {
			Function * response;
			function_def fd  = build_new_function(in, context, l);
			response = fd.func;
			return response;
		 }



    	int sstat = SIMPLE, lpoz = 0, cntsqb = 0;
    	string cslct, rest, sqselect, argselect;

    	bool cvalid, force_call_method = false;
    	quota_stat qst;
    	//if it's new
		if(string_starts_with(in, l.vnew, ' ', l)) {
			in = in.substr(strlen(l.vnew), in.size() - 1);
			in = trim(in);
			printf("\tnew instance of (%s)\n", in.c_str());
			force_call_method = true;
		}

		Value * currentSelection = NULL;
    	//check for encapsulation, like ()()
    	if(in.at(0) == l.l_parenthesis && index == 0){
    		cslct = get_expression_between(in, l.l_parenthesis, l.r_parenthesis, lpoz, l);
    		log_stack("encaps cslct = %s lpos = %i\n", cslct.c_str(), lpoz);
    		rest = in.substr(lpoz, in.size() - 1);
    		log_stack("encaps %s, \n rest: %s\n", cslct.c_str(), rest.c_str());
    		int cms;
    		currentSelection = scriptengine::execute_command(cslct, l, context, cms);
    		goto current_selection_def;
		}

    	//check for array def, like [].data:
    	if(in.at(0) == l.l_sqbracket && index == 0){
    		cslct = get_expression_between(in, l.l_sqbracket, l.r_sqbracket, lpoz, l);
    		rest = in.substr(lpoz, in.size() - 1);
			currentSelection = scriptengine::build_new_array(cslct, context, l);//scriptengine::execute_command(cslct, l, context, cms);
			goto current_selection_def;
    	}


    	//check for object def, like {}.data:
		if(in.at(0) == l.l_bracket && index == 0){
			cslct = get_expression_between(in, l.l_bracket, l.r_bracket, lpoz, l);
			rest = in.substr(lpoz, in.size() - 1);
			currentSelection = scriptengine::build_new_object(cslct, context, l);//scriptengine::execute_command(cslct, l, context, cms);
			goto current_selection_def;
		}



    	for(int i = 0; i < in.size(); i++){
    		char c = in.at(i);
			quota_stat_read(qst, l, in, i);
			cvalid = quota_stat_valid(qst);

			if(cvalid && (c == l.dot || c == l.delimiter) && sstat == SIMPLE && cntsqb == 0) {        // .
				rest = in.substr(i + 1, in.size() - 1); //i + 1 to skip current char
				goto check_rest;
			}

			else if(cvalid && c == l.l_sqbracket) {         // [
				if(cntsqb == 0){
					sstat = SQBRACKET;
					cntsqb++;
					continue;
				} else {
					cntsqb++;
				}
			}

			else if(cvalid && c == l.l_parenthesis && cntsqb == 0) {         // (
				sstat = PARANTHESIS;
				argselect = get_expression_between(in, l.l_parenthesis, l.r_parenthesis, lpoz, l);
				if(lpoz <= in.size()){
					rest = in.substr(lpoz, in.size());
				}
				force_call_method = true; //call is require when paranthesis are detectd
				goto check_rest;
			}

			else if(cvalid && c == l.r_sqbracket) {         // ]
				cntsqb--;
				if(cntsqb == 0){
					sstat = SIMPLE;
					rest = in.substr(i + 1, in.size() - 1); //i + 1 to skip current char
					goto check_rest;
				}

				//continue;
			}
			switch(sstat){
				case SIMPLE: cslct += c; break;
				case SQBRACKET: sqselect += c; log_stack("sqselect = %s", sqselect.c_str()); break;
				case PARANTHESIS: argselect += c; break;
			}

    	}


    	check_rest:
			stringAlg::trim(sqselect);
			stringAlg::trim(argselect);
			stringAlg::trim(rest);
			stringAlg::trim(cslct);
			log_stack("cslct = <%s> sqselect = <%s> argselect=<%s> rest=<%s>",
					cslct.c_str(),
					sqselect.c_str(),
					argselect.c_str(),
					rest.c_str());



    		if(cslct == l.vthis || cslct.empty()){
    			currentSelection = thisInstance();
//    			log_stack("this = %s", blurt(currentSelection).c_str())
			}
    		else if(cslct == "prototype" ){
    			currentSelection = prototype();
    		}
    		else if(cslct == "constructor"){
    			currentSelection = constructor();
    		}
    		else {
    			//get lazy or update internal lazy members
    			currentSelection = getFromConstructor(cslct);

    			if(currentSelection == NULL){
        			//if key exists:
    				if(objmap.find(cslct) != objmap.end()) {
    					//if assign value is not null
    					if(dv != NULL && rest.empty() ){
    						//delete old value if is not undefined
    						if(objmap[cslct] != Value::npos){
    							objmap.erase(cslct);
    						}
    						//assign new value if assign value is not undefined
    						if(dv != Value::npos){
    							objmap[cslct] = dv;
    							return objmap[cslct];
    						}
    						return Value::npos; //else return undefined
    					}
    					currentSelection = objmap[cslct];
    				}

    				else {
    					//if assign value exists and rest is empty ignore fetch modes
    					if(dv != NULL && rest.empty() ){
    						log_stack("assign %s with %s", cslct.c_str(), blurt(dv).c_str());
    						objmap[cslct] = dv; //assign
    						return objmap[cslct];
    					}

    					//if context is not empty get member without recursive call
    					if(context != Function::npos){
    						log_stack("%s not found, rest = %s", cslct.c_str(), rest.c_str());
    						currentSelection = scriptengine::get_member(cslct, context, l, dv, false);
    						if(currentSelection == Value::npos && !rest.empty()){
    							throw_error("cannot access property [%s] of undefined object returned by caller [%s]",
    								rest.c_str(), cslct.c_str());
    							return Value::npos;
    						}
    					} else {
    						return String::New(cslct, _lx);
    					}
    				}
    			}
			}

		current_selection_def:
			if(currentSelection == NULL && fetchMode == RETURN_VAL){
				return Value::npos;
			}
			//square selection
			if(!sqselect.empty()){
				int cms = 0;
				currentSelection = currentSelection->get(scriptengine::execute_command(sqselect, l, context, cms));
			}
			//method call
    		if(!argselect.empty()){
    			currentSelection = currentSelection->call(argselect, l, context);
    			force_call_method = false;
    		}
    		//new instance
    		if(force_call_method){
    			currentSelection = currentSelection->call(Value::npos, context);
    		}
    		//check for remaining chars
    		if(!rest.empty()){
    			return currentSelection->getRecursive(rest, l, context, index + 1, fetchMode, dv);
    		}
    		return currentSelection;
    }





    Value * Value::thisInstance(){
    	return this;
    };

    Value * Value::prototype(){
		if(_proto == NULL){
			_proto = new Object(_lx);
		}
    	return _proto;
    };

    Function * Value::constructor(){
		if(_constr == NULL){
			return Function::npos;
		}
    	return _constr;
    };





























    stxtr blurt(Value * v){
		if (!v) {
			return "NULL";
		}

		stxtr r;
		r.append(v->strtype());
		r.append("   ");
		r.append(v->str());
		return r;

    };







} /* namespace astox */
