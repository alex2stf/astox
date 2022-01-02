/*
 * Array.cpp
 *
 *  Created on: Jan 26, 2014
 *      Author: Alexandru
 */

#include "astox.h"


namespace astox {

	using namespace std;
	using namespace serializers;
    Array * Array::npos = new Array(Syntax::JS);

    Array::Array(const lexic &l): Value(l) {
        valueType = ASTOX_VALUE_ARRAY;
        it = 0;
        sortingCondition = NULL;
    }

    Array::Array(int size, const lexic &l) : Value(l) {
        valueType = ASTOX_VALUE_ARRAY;
        it = 0;
        for (int i = 0; i < size; i++) {
            items.push_back(Value::npos);
        }
        sortingCondition = NULL;
    }

    Array::Array(Array * copy,  const lexic &l) : Value(l) {
		valueType = ASTOX_VALUE_ARRAY;
        it = 0;
        for (int i = 0; i < copy->length(); i++) {
            items.push_back(copy->at(i));
        }
        sortingCondition = NULL;
    }

    Array::Array(Array * copy, Array * concatenable,  const lexic &l): Value(l) {
        it = 0;
        int i;
        for (i = 0; i < copy->length(); i++) {
            items.push_back(copy->at(i));
        }

        for (i = 0; i < concatenable->length(); i++) {
            items.push_back(concatenable->at(i));
        }
        valueType = ASTOX_VALUE_ARRAY;
        sortingCondition = NULL;
    }

    Array * Array::New(const lexic &l){
    	return new Array(l);
    };

    Array::~Array() {
        // TODO Auto-generated destructor stub
    }

    stxtr Array::str() {
       stxtr r = "";
	   r += _lx.l_sqbracket;
	   for(int i = 0; i < items.size(); i++){
		   if(i > 0) {  r+=_lx.comma; }
		   if (items.at(i)) {
			   if (items.at(i)->isString()) { r += _lx.q2; }
			   r.append(items.at(i)->str());
			   if (items.at(i)->isString()) { r += _lx.q2; }
		   }
		   else {
			   r.append("NULL");
		   }
	   }
	   r+=_lx.r_sqbracket;
	   return r;
    }


    void Array::updateGetters() {
		objmap[_lx.env.length] = Number::New((double)items.size(), _lx);
    };

    void Array::setGetters() {
        objmap["join"] = Function::New(ecma::methods::array_join, this, _lx);
        objmap["pop"] = Function::New(ecma::methods::array_pop, this, _lx);
        objmap["shift"] = Function::New(ecma::methods::array_shift, this, _lx);
        objmap["concat"] = Function::New(ecma::methods::array_concat, this, _lx);
        objmap["unshift"] = Function::New(ecma::methods::array_unshift, this, _lx);
        objmap["forEach"] = Function::New(ecma::methods::array_forEach, this, _lx);
        objmap["slice"] = Function::New(ecma::methods::array_slice, this, _lx);
        objmap["splice"] = Function::New(ecma::methods::array_splice, this, _lx);

    };

    void Array::dropData() {
        items.clear();
    }

    bool Array::isEmpty() {
        return items.empty();
    }

    const char * Array::strtype() {
        return _lx.env.array;
    }

    size_t Array::length() {
        return items.size();
    }

    set<stxtr> Array::key_names() {
        set<string> response;

        for (unsigned int i = 0; i < items.size(); i++) {
            response.insert(stringAlg::intToString(i));
        }

        map<stxtr, Value *>::iterator it;
        for (it = objmap.begin(); it != objmap.end(); it++) {
            response.insert(it->first);
        }

        return response;
    }

    Value * Array::getValue(stxtr kn) {
        if (astox::stringAlg::stringIsNumber(kn)) {
            int i = atoi(kn.c_str());
			if(i >= items.size() || !items.at(i)){
				return Value::npos;
			}
            return items.at(i);
        }
		if (objmap.find(kn) == objmap.end() || !objmap[kn]) {
			return Value::npos;
		}
        return objmap[kn];
    }

    void Array::push(Value * value) {
    	log_stack("push_back %s", blurt(value).c_str());
        items.push_back(value);
        updateGetters();
    }

    void Array::push(stxtr value) {
        items.push_back(String::New(value, _lx));
        updateGetters();
    }

    void Array::push(int value) {
        items.push_back(Number::New(value, _lx));
        updateGetters();
    }

    void Array::push(bool value) {
        items.push_back(Number::New(value, _lx));
        updateGetters();
    }

    astox::Value * Array::at(int index) {
        if (index < (int) items.size()) {
            return items.at(index);
        }
        return Value::npos;
    };

    astox::Value * Array::at(std::string indexStr) {
        if (indexStr == "*") {
            return at((int) (items.size() - 1));
        }
        return at(atoi(indexStr.c_str()));
    };

    Value * Array::get(Value * d) {
    	log_stack("get %s\n", d->str().c_str());
        if (d->isNumber()) {
        	int v = d->toInt();
        	if(v >= items.size() || !items.at(v)){
        		return Value::npos;
        	}
            return items.at(v);
        }
        else if(d->isArray()){
        	log_stack("get by array %s", serializers::to_csv(d).c_str());
        	return getValue(serializers::to_csv(d));
        }
        else if (d->str() == "*") {
            return items.at(items.size() - 1);
        }
		return  getValue(d->str());
    }

    Value * Array::update(std::string keyName, astox::Value * newValue) {

        int index = atoi(keyName.c_str());
        //	cout << "arrayindexupdate" << index << " from " << items.size() << endl;

        if (index < (int) items.size()) {
            return items[index] = newValue;

        } else if (index == (int) items.size()) {
            items.insert(items.begin() + index, newValue);
        }
        return newValue;
    }

    astox::Array * Array::cast(Value * value) {
#ifdef ASTX_VALUE_STATIC_CAST
        return static_cast<Array *> (value);
#else
        return dynamic_cast<Array *> (value);
#endif
    }

    vector<stxtr> Array::listToString() {
        std::vector<std::string> response;
        for (unsigned int i = 0; i < items.size(); i++) {
            response.push_back(items.at(i)->str());
        }
        return response;
    }

    vector<astox::Value *> * Array::getItems() {
        return &items;
    }

    void Array::iterateArray(void (onIterate) (astox::Value * value)) {
        for (unsigned int i = 0; i < items.size(); i++) {
            onIterate(items.at(i));
        }
    }

    bool Array::isArray() {
        return true;
    }

    bool Array::isObject() {
        return true;
    }

    Value * Array::reverse() {
        std::reverse(items.begin(), items.end());
        return this;
    }

    int Array::quickSort_pivot(std::vector<Value *> &list, int first, int last) {
        int p = first;
        Value * pivotElement = list.at(first);

        for (int i = first + 1; i <= last; i++) {

            /* If you want to sort the list in the other order, change "<=" to ">" */
            //		if(list.at(i)->str() <= pivotElement->str() ) // asta tb pusa in parametru
            if (sortCompare(list.at(i), pivotElement)) // asta tb pusa in parametru
            {
                p++;
                //			quickSort_swap(a[i], a[p]);
                std::iter_swap(items.begin() + i, items.begin() + p);
            }


        }

        //	quickSort_swap(a[p], a[first]);
        std::iter_swap(items.begin() + p, items.begin() + first);
        return p;
    };

    void Array::quickSort_main(std::vector<Value *> &list, int first, int last) {
        int pivotElement;

        if (first < last) {
            pivotElement = quickSort_pivot(list, first, last);
            quickSort_main(list, first, pivotElement - 1);
            quickSort_main(list, pivotElement + 1, last);
        }
    }

    bool Array::sortCompare(Value * left, Value * right) {
        if (sortingCondition != NULL) {
            Array * args = Array::New(_lx); //TODO modify
            args->push(left);
            args->push(right);
            return sortingCondition->call(args, Function::npos)->bol();
        }
        return left->str() <= right->str();
    }

    Array * Array::sort(Function * _sortingMode) {

        if (_sortingMode != NULL) {
            sortingCondition = _sortingMode;
        }

        quickSort_main(items, 0, (int)items.size() - 1);
        return this;
    };

    void Array::forEach(Function * caller) {
        if (caller != NULL) {
            for (unsigned int i = 0; i < items.size(); i++) {
                Array * args = Array::New(_lx); //TODO modify
                args->push(items.at(i));
                args->push(Number::New(i, _lx));
                args->push(this);

                caller->call(args, caller);
            }
        }
    }

    String * Array::join(std::string separator) {
        using namespace std;
        string response;
        for (unsigned int i = 0; i < items.size(); i++) {
            if (i > 0) {
                response.append(separator);
            }
            if (items.at(i)->type() > ASTOX_VALUE_UNDEFINED) {
                response.append(items.at(i)->str());
            }

        }
        return String::New(response, _lx);

    };

    Value * Array::pop() {
        if (!items.empty()) {
            Value * removed = items.at(items.size() - 1);
            items.erase(items.end() - 1);
            updateGetters();
            return removed;
        }
        return Value::npos;

    }

    Value * Array::shift() {
        if (!items.empty()) {
            Value * removed = items.at(0);
            items.erase(items.begin());
            updateGetters();
            return removed;
        }
        return Value::npos;

    }

    Value * Array::unshift(Value * arguments) {
        items.insert(items.begin(), arguments);
        updateGetters();
        return arguments;
    };

    Array * Array::concat(Array * arguments) {
        Array * response = new Array(this, arguments, _lx);
        updateGetters();
        return response;
    };

    Array * Array::slice(int start, int end) {
        Array * response = Array::New(_lx);
        if (end > (int) items.size()) {
            end = (int) items.size();
        }
        for (int i = start; i < end; i++) {
            response->push(items.at(i));
        }
        return response;
    };

    Array * Array::slice(Array * arguments) {
        int start = arguments->at(0)->toInt();
        int end = (int)items.size();
        int expectEnd = arguments->at(1)->toInt();
        if (expectEnd > 0) {
            end = expectEnd;
        }
        return slice(start, end);
    };

    Array * Array::splice(int start, int end) {
        if (end > (int) items.size()) {
            end = (int) items.size();
        }
        end = start + end;
      

        Array * remainingData = Array::New(_lx); //TODO modify
        int i = 0;
        for (i = start; i < end; i++) {
            //		cout << i << ") " << lang::toJSON(items.at(i)) << endl;
            remainingData->push(items.at(i));
        }

        items.erase(items.begin() + start, items.begin() + end);

        updateGetters();
        return remainingData;
    };

    Array * Array::splice(Array * arguments) {
        int start = arguments->at(0)->toInt();
        int end = (int) items.size();
        int expectEnd = arguments->at(1)->toInt();
        if (expectEnd > 0) {
            end = expectEnd;
        }
        return splice(start, end);

    };



    Function * Array::constructor(){
    	if(_constr == NULL){
    		_constr = Function::ARRCNSTR;
    	}
    	return _constr;
    };


    Value * Array::increment(Value * v){
		stxtr s = to_csv(this);
		if(v->isFunction()){
			s.append(v->str());
		} else {
			s.append(to_csv(v));
		}
		return String::New(s,_lx);
    };



} /* namespace astox */





