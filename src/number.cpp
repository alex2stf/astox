/*
 * Number.cpp
 *
 *  Created on: Jan 26, 2014
 *      Author: Alexandru
 */

#include "astox.h"


namespace astox {

	using namespace scriptengine;
	using namespace serializers;

    Number * Number::ZERO = new Number(0, Syntax::JS);
    Number * Number::NaN = new Number(NAN, Syntax::JS);

    Number * Number::New(double value, const lexic &l) {
		return  new Number(value, l);// _memoizedNumbers[value];
	}

    Number::Number(double value, const lexic &l) : Value(l) {
        pvalue = value;
        valueType = ASTOX_VALUE_NUMBER;
    }

    Number * Number::New(stxtr strNum, const lexic &l) {
        return Number::New(strNum.c_str(), l);
    };

    Number * Number::New(const char * strNum, const lexic &l) {
        return Number::New(atof(strNum), l);
    };

    Number * Number::New(Value * from){
    	return Number::New(from->dbl(), from->lxc());
    };

    Number::~Number() {
        // TODO Auto-generated destructor stub
    }

    Number * Number::cast(Value * value) {
		#ifdef ASTX_VALUE_STATIC_CAST
				return static_cast<Number *> (value);
		#else
				return dynamic_cast<Number *> (value);
		#endif
    }

    void Number::updatePValue(int newInt) {
        pvalue = newInt;
    };

    stxtr Number::str() {
		std::stringstream ss(stringstream::in | stringstream::out);
		ss << pvalue;
		return ss.str();
	}

    const char * Number::strtype() {
        return _lx.env.number;
    }

    bool Number::bol() {
        return pvalue != 0;
    };

    int Number::toInt() {
        return (int) pvalue;
    }

    double Number::dbl() {
        return pvalue;
    }

    Boolean * Number::negate() {
        return Boolean::New(!pvalue, _lx);
    }

    bool Number::isString() {
        return false;
    }

    bool Number::isNumber() {
        return true;
    }

    bool Number::isBoolean() {
        return false;
    }

    bool Number::isArray() {
        return false;
    }

    bool Number::isObject() {
        return false;
    }

    bool Number::isUndefined() {
        return false;
    }


    Value * Number::v_plusplus(){

    	pvalue++;
    	return this;
    };
	Value * Number::v_minmin(){
		pvalue--;
		return this;
	};

	Value * Number::plusplus_v(){
		++pvalue;
		return this;
	};
	Value * Number::minmin_v(){
		--pvalue;
		return this;
	};

	Value * Number::modifyBy(Value * term, int operation){


		switch(operation){
			case PLUS: pvalue += term->dbl(); break;
			case MINUS: pvalue -= term->dbl(); break;
			case DIVIDE: pvalue /= term->dbl(); break;
			case MULTIPLY: pvalue *= term->dbl(); break;
			case SHIFT_RIGHT: pvalue = (int) pvalue >> term->toInt(); break;
			case SHIFT_RIGHT_UNSG: pvalue = (unsigned int) pvalue >> term->toInt(); break;
			case SHIFT_LEFT: pvalue = (int) pvalue << term->toInt(); break;
			case MODULUS: pvalue = (int) pvalue % term->toInt();  break;
			case BIT_OR: pvalue = (int) pvalue | term->toInt();  break;
			case BIT_AND: pvalue = (int) pvalue & term->toInt();  break;
		}
		return this;
	};



	 Function * Number::constructor(){
		 if(_constr == NULL){
			 _constr = Function::NUMCNSTR;
		 }
		 return _constr;
	 };

	 Value * Number::increment(Value * v){
		 if(v->isNumber() || v->isBoolean()){
			 return Number::New(dbl()+v->dbl(), _lx);
		 }
		 stxtr r = (str());
		 if(v->isArray()){
			 r.append(to_csv(v));
		 } else {
			 r.append(v->str());
		 }
		 return String::New(r, _lx);
	 };

} /* namespace astox */
