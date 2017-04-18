/*
 * Array.cpp
 *
 *  Created on: Jan 26, 2014
 *      Author: Alexandru
 */

#include "astox.h"
namespace astox {

    Array * Array::npos = new Array();

    Array::Array()
    : Object() {
        valueType = ASTOX_VALUE_ARRAY;
        it = 0;
        setGetters();
    }

    Array::Array(int size) {
        valueType = ASTOX_VALUE_ARRAY;
        it = 0;
        for (int i = 0; i < size; i++) {
            items.push_back(new Value());
        }
        setGetters();
    }

    Array::Array(const char * value) {
        std::string s(value);
        s = astox::stringAlg::trim(s);


        if (s.size() > 2 && s.at(0) == '[' && s.at(s.size() - 1) == ']') {
            s = s.substr(1, s.size()); // scot primul
            s = s.substr(0, s.size() - 1); //scot ultimul
            *this = *(ecma::fromStrAsArray(s));
        }

        it = 0;
        valueType = ASTOX_VALUE_ARRAY;
        setGetters();
    }

    Array::Array(Array * copy) {
        it = 0;
        for (int i = 0; i < copy->length(); i++) {
            items.push_back(copy->itemAtIndex(i));
        }
        valueType = ASTOX_VALUE_ARRAY;
        setGetters();
    }

    Array::Array(Array * copy, Array * concatenable) {
        it = 0;
        int i;
        for (i = 0; i < copy->length(); i++) {
            items.push_back(copy->itemAtIndex(i));
        }

        for (i = 0; i < concatenable->length(); i++) {
            items.push_back(concatenable->itemAtIndex(i));
        }
        valueType = ASTOX_VALUE_ARRAY;
        setGetters();
    }

    Array::~Array() {
        // TODO Auto-generated destructor stub
    }

    std::string Array::toString() {
        return astox::lang::toJSON(&(*this), "", "");
    }

    void Array::updateGetters() {
        objectList["length"]->updatePValue(items.size());
    };

    void Array::setGetters() {
        objectList["length"] = Number::New(items.size());
        objectList["toJSON"] = new Function(ecma::methods::proto_toJSON, this);
        objectList["toString"] = new Function(ecma::methods::proto_toString, this);
        objectList["reverse"] = new Function(ecma::methods::array_reverse, this);
        objectList["join"] = new Function(ecma::methods::array_join, this);
        objectList["sort"] = new Function(ecma::methods::array_sort, this);
        objectList["push"] = new Function(ecma::methods::array_push, this);
        objectList["pop"] = new Function(ecma::methods::array_pop, this);
        objectList["shift"] = new Function(ecma::methods::array_shift, this);
        objectList["concat"] = new Function(ecma::methods::array_concat, this);
        objectList["unshift"] = new Function(ecma::methods::array_unshift, this);
        objectList["forEach"] = new Function(ecma::methods::array_forEach, this);
        objectList["slice"] = new Function(ecma::methods::array_slice, this);
        objectList["splice"] = new Function(ecma::methods::array_splice, this);
        sortingCondition = NULL; //new Function(" return a <= b", "a, b", new Function());
    };

    void Array::dropData() {
        items.clear();
    }

    bool Array::isEmpty() {
        return items.empty();
    }

    std::string Array::getStrType() {
        return ASTOX_VALUE_STR_ARRAY;
    }

    int Array::length() {
        return items.size();
    }

    std::vector<std::string> Array::getKeyNameList() {
        using namespace std;
        vector<string> response;

        for (unsigned int i = 0; i < items.size(); i++) {
            response.push_back(stringAlg::intToString(i));
        }

        std::map<std::string, astox::Value *>::iterator it;
        for (it = objectList.begin(); it != objectList.end(); it++) {
            response.push_back(it->first);
        }

        return response;
    }

    Value *& Array::getValue(std::string keyName) {
        if (astox::stringAlg::stringIsNumber(keyName)) {
            int index = atoi(keyName.c_str());
            return items.at(index);
        }
        return objectList[keyName];
    }

    void Array::push(astox::Value * value) {
        items.push_back(value);
        updateGetters();
    }

    void Array::push(std::string value) {
        items.push_back(new String(value));
        updateGetters();
    }

    void Array::push(int value) {
        items.push_back(Number::New(value));
        updateGetters();
    }

    void Array::push(bool value) {
        items.push_back(Number::New(value));
        updateGetters();
    }

    astox::Value * Array::itemAtIndex(int index) {
        //	cout << "array called at index" << index << endl;
        if (index < (int) items.size()) {
            return items.at(index);
        }
        return Value::npos;
    };

    astox::Value * Array::itemAtIndex(std::string indexStr) {
        if (indexStr == "*") {
            return itemAtIndex(items.size() - 1);
        }
        return itemAtIndex(atoi(indexStr.c_str()));
    };

    astox::Value *& Array::get(astox::Value * data) {
        if (data->isNumber()) {
            return items.at(data->toInt());
        } else if (data->toString() == "*") {
            return items.at(items.size() - 1);
        }
        return getValue(data->toString());
    }

    astox::Value * Array::updateValue(std::string keyName, astox::Value * newValue) {

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

    std::vector<std::string> Array::listToString() {
        std::vector<std::string> response;
        for (unsigned int i = 0; i < items.size(); i++) {
            response.push_back(items.at(i)->toString());
        }
        return response;
    }

    std::vector<astox::Value *> * Array::getItems() {
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
            //		if(list.at(i)->toString() <= pivotElement->toString() ) // asta tb pusa in parametru
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
            Array * args = new Array();
            args->push(left);
            args->push(right);
            return sortingCondition->call(args)->toBoolean();
        }
        return left->toString() <= right->toString();
    }

    Array * Array::sort(Function * _sortingMode) {
        //	std::iter_swap(items.begin(), items.begin()+1);

        if (_sortingMode != NULL) {
            sortingCondition = _sortingMode;
        }

        quickSort_main(items, 0, items.size() - 1);


        //	bool dbg = sortingCondition->call(args)->toBoolean();

        //	cout << dbg << endl;

        return this;
    };

    void Array::forEach(Function * caller) {
        if (caller != NULL) {
            for (unsigned int i = 0; i < items.size(); i++) {
                Array * args = new Array();
                args->push(items.at(i));
                args->push(Number::New(i));
                args->push(this);

                caller->call(args);
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
            if (items.at(i)->getType() > ASTOX_VALUE_UNDEFINED) {
                response.append(items.at(i)->toString());
            }

        }
        return new String(response);

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
        Array * response = new Array(this, arguments);
        updateGetters();
        return response;
    };

    Array * Array::slice(int start, int end) {
        Array * response = new Array();
        if (end > (int) items.size()) {
            end = items.size();
        }
        for (int i = start; i < end; i++) {
            response->push(items.at(i));
        }
        return response;
    };

    Array * Array::slice(Array * arguments) {
        int start = arguments->itemAtIndex(0)->toInt();
        int end = items.size();
        int expectEnd = arguments->itemAtIndex(1)->toInt();
        if (expectEnd > 0) {
            end = expectEnd;
        }
        return slice(start, end);
    };

    Array * Array::splice(int start, int end) {
        if (end > (int) items.size()) {
            end = items.size();
        }
        end = start + end;
        //	cout << "splice(" << start << "," << end << ")" << endl;

        Array * remainingData = new Array();
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
        int start = arguments->itemAtIndex(0)->toInt();
        int end = items.size();
        int expectEnd = arguments->itemAtIndex(1)->toInt();
        if (expectEnd > 0) {
            end = expectEnd;
        }
        return splice(start, end);

    };


    //ArrayItemAt::ArrayItemAt(astox::Value * p_value)
    //:Function(p_value)
    //{
    //}
    //
    //ArrayItemAt::~ArrayItemAt() {
    //	// TODO Auto-generated destructor stub
    //}
    //
    //
    //astox::Value * ArrayItemAt::execute(std::string arguments){
    //	using namespace std;
    //	if(astox::stringAlg::stringIsNumber(arguments)){
    //		int indx = atoi(arguments.c_str());
    //		return Array::cast(value)->itemAtIndex(indx);
    //	};
    //
    //	return new Value();
    //}
    //
    //PATRNList::PATRNList(astox::Value * p_value)
    //:Function(p_value)
    //{
    //}
    //
    //PATRNList::~PATRNList() {
    //	// TODO Auto-generated destructor stub
    //}

    //
    //astox::Value * PATRNList::execute(std::string arguments){
    //	using namespace std;
    //
    //	std::string result;
    //
    //	if(value->isArray()){
    //			astox::Array * cobj = astox::Array::cast(value);
    //
    //			for(unsigned int i = 0 ; i < cobj->getItems()->size(); i++){
    ////				cout << cobj->getItems()->at(i)->toString() << endl;
    //
    //				std::string start = arguments;
    //				if(cobj->getItems()->at(i)->isObject()){
    //					std::map<std::string , astox::Value *>::iterator it;
    //					Object * cth = Object::cast(cobj->getItems()->at(i));
    //					for(it=cth->getObjectList()->begin(); it!=cth->getObjectList()->end(); ++it){
    //
    //						std::string in = "$";
    //						in.append(it->first);
    //						//cout << "replace " << in << " with " <<  it->second->toString() << " in " << start << endl;
    //						astox::stringAlg::replace_all(start, in, it->second->toString());
    //						//scout << "start is " << start << endl;
    //
    //					}
    //					result.append(start);
    //				}
    //
    //
    //
    //			}
    //		}
    //
    //	//cout << "-------> result is " << result << endl;
    //
    //	return new String(result);
    //}
    //
    //
    //
    //
    //
    ///*LIST FUNCTION:*/
    //BIFARRList::BIFARRList(astox::Value * p_value)
    //:Function(p_value)
    //{
    //	// TODO Auto-generated constructor stub
    //
    //}
    //
    //BIFARRList::~BIFARRList() {
    //	// TODO Auto-generated destructor stub
    //}
    //
    //
    //
    //
    //
    //astox::Value * BIFARRList::execute(std::string arguments){
    //
    //	/*
    //	 * argumnets:
    //	 * before key 1
    //	 * after key 1
    //	 * before value 1
    //	 * after value 1
    //	 */
    //
    //	using namespace std;
    //	using namespace astox;
    //
    //	if(argumentsList.empty()){
    //		argumentsList = parseArgStr(arguments);
    //	}
    //
    //
    //	std::string response;
    //	std::string pre;
    //	if(argumentsList.size() > 0 && argumentsList.at(0)){
    //		pre = argumentsList.at(0)->toString();
    //	}
    //	std::string after;
    //	if(argumentsList.size() > 1 && argumentsList.at(1)){
    //		after = argumentsList.at(1)->toString();
    //	}
    //
    //
    //	if(value->isArray()){
    //		astox::Array * cobj = astox::Array::cast(value);
    //
    //
    //		for(unsigned int i = 0 ; i < cobj->getItems()->size(); i++){
    //			response.append(pre);
    //			response.append(cobj->getItems()->at(i)->toString());
    //			response.append(after);
    //		}
    //
    //
    ////		std::map<std::string , astox::Value *>::iterator it;
    ////		for(it=cobj->getObjectList()->begin(); it!=cobj->getObjectList()->end(); ++it){
    ////			response.append(pre);
    ////			response.append(it->second->toString());
    ////			response.append(after);
    ////		}
    //	}
    //
    //
    //
    //
    //
    //
    //	return new astox::String(response);
    //};






} /* namespace astox */





