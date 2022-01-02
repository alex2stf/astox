/*
 * Object.cpp
 *
 *  Created on: Jan 26, 2014
 *      Author: Alexandru
 */

#include "astox.h"

namespace astox {
	using namespace std;
	using namespace scriptengine;

    Object * Object::npos = new Object(Syntax::JS);

    Object::Object(const lexic &l): Value(l) {
		_lx = l;
        valueType = ASTOX_VALUE_OBJECT;
    }

    Object * Object::New(const lexic &l) {
		Object * r = new Object(l);
	//	r->setGetters();
    	return r;
    };

    Object::~Object() {
        // TODO Auto-generated destructor stub
    }


    const char * Object::strtype() {
		return _lx.env.object;
    }

   stxtr Object::str() {

	   set<stxtr>::iterator sit;

	   stxtr r = "{";
	    int c = 0;
	    for(sit = _key_names.begin(); sit != _key_names.end(); ++sit){
	      	if(c > 0){
	      	    r.append(",");
	      	}
	      	Value * v = getValue(*sit);
	      	if(v != Value::npos){
	      		r.append(*sit);
				r.append(":");
				r.append(getValue(*sit)->str());
				c++;
	      	}
	    }
		r.append("}");
	    return r;
    }


    void Object::setKeys(std::map<std::string, astox::Value *> keys) {
        objmap = keys;
    }

    size_t Object::length() {
        return objmap.size();
    }




    Boolean * Object::contains(Value * term) {
        if (objmap.find(term->str()) != objmap.end()) {
            return Boolean::bTrue(_lx);
        }
        return Boolean::bFalse(_lx);
    };

    astox::Object * Object::cast(Value * value) {
		#ifdef ASTX_VALUE_STATIC_CAST
				return static_cast<Object *> (value);
		#else
				return dynamic_cast<Object *> (value);
		#endif
    }

    Object * Object::merge(Object * left, Object * right){
    	object_iterator it;
    	object_map * lm = left->getObjectList();
    	object_map * rm = right->getObjectList();
    	Object * response = Object::New(left->lxc());
    	for(it = lm->begin(); it != lm->end(); ++it){
    		response->addKey(it->first, it->second);
    	}
    	for(it = rm->begin(); it != rm->end(); ++it){
			response->addKey(it->first, it->second);
		}

		return response;
    };



    /*ISSES:*/
    bool Object::isString() {
        return false;
    }

    bool Object::isNumber() {
        return false;
    }

    bool Object::isBoolean() {
        return false;
    }

    bool Object::isArray() {
        return false;
    }

    bool Object::isObject() {
        return true;
    }

    bool Object::isUndefined() {
        return false;
    }

    Boolean * Object::negate() {
        return Boolean::bFalse(_lx);
    }


    Function *  Object::constructor(){
    	if(_constr == NULL){
    		_constr = Function::OBJCNSTR;
    	}
    	return _constr;
    };
} /* namespace astox */
