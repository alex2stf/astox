/*
 * Operators.cpp
 *
 *  Created on: Mar 5, 2015
 *      Author: alexandru
 */




#include "types.h"

namespace astox {

//    String String::operator--(int) {
//        String tmp(*this); // copy
//        operator--();
//        return tmp; // return old value
//    }
//
//    String& String::operator--() {
//        if (!pvalue.empty() && pvalue.size() > 0) {
//            pvalue = pvalue.substr(0, pvalue.size() - 1);
//        }
//        return *this;
//    }

    Number& Number::operator--() {
        pvalue--;
        return *this;
    }

    Number Number::operator--(int) {
        Number tmp(*this); // copy
        operator--(); // pre-increment
        return tmp; // return old value
    }

//    String String::operator++(int) {
//        String tmp(*this); // copy
//        operator++(); // pre-increment
//        return tmp; // return old value
//    }

    Number Number::operator++(int) {
        Number tmp(*this); // copy
        operator++(); // pre-increment
        return tmp; // return old value
    }

    Number& Number::operator++() {
        pvalue++;
        return *this;
    }

//    String& String::operator++() {
//        if (!pvalue.empty() && pvalue.size() > 0) {
//            pvalue += pvalue.at(pvalue.size() - 1);
//        }
//        return *this;
//    }
//
//    String& String::operator+=(Value& rhs) {
//        append(&rhs);
//        return *this; // return the result by reference
//    }

//    String& String::operator+=(const char* rhs) {
//        pvalue.append(rhs);
//        return *this; // return the result by reference
//    }
//
//    String& String::operator+=(const std::string& rhs) {
//        pvalue.append(rhs);
//        return *this; // return the result by reference
//    }


    //not defined in .h

//    String operator+(String lhs, const String& rh) {
//        std::string data = lhs.pvalue;
//        data.append(rh.pvalue);
//        String resObj(data);
//        return resObj;
//    }

    std::ostream& operator<<(std::ostream& output, const Value& val) {
        output << ASTOX_VALUE_STR_UNDEFINED;
        return output;
    }

    std::ostream& operator<<(std::ostream& output, const Value * val) {
        output << ASTOX_VALUE_STR_UNDEFINED;
        return output;
    }

    std::ostream& operator<<(std::ostream& output, const String& val) {
        output << val.pvalue;
        return output;
    }

    std::ostream& operator<<(std::ostream& output, const String * val) {
        output << val->pvalue;
        return output;
    }

    std::ostream& operator<<(std::ostream& output, const Number& val) {
        output << val.pvalue;
        return output;
    }

    std::ostream& operator<<(std::ostream& output, const Number * val) {
        output << val->pvalue;
        return output;
    }

    bool operator==(Boolean &cP1, Boolean &cP2) {
        return (cP1.pvalue == cP2.pvalue);
    }

    bool operator==(Boolean *cP1, Boolean &cP2) {
        return (cP1->pvalue == cP2.pvalue);
    }

    bool operator!=(Boolean &cP1, Boolean &cP2) {
        return (cP1.pvalue != cP2.pvalue);
    }

    bool operator==(Number &cP1, Number &cP2) {
        return (cP1.pvalue == cP2.pvalue);
    }

    bool operator!=(Number &cP1, Number &cP2) {
        return (cP1.pvalue != cP2.pvalue);
    }

    bool operator<=(Number &cC1, Number &cC2) {
        return cC1.pvalue <= cC2.pvalue;
    }

    bool operator<(Value &cC1, Value &cC2) {
        return cC1.str() < cC2.str();
    }

    bool operator<(Number &cC1, Number &cC2) {
        return cC1.str() < cC2.str();
    }

    bool operator<(Number &cC1, String &cC2) {
        return false;
    }

    bool operator>(Value &cC1, Value &cC2) {
        return cC1.str() > cC2.str();
    }

    bool operator>(Number &cC1, Number &cC2) {
        return cC1.str() > cC2.str();
    }

    bool operator>(Number &cC1, String &cC2) {
        return false;
    }

    bool operator>=(Number &cC1, Number &cC2) {
        return cC1.pvalue >= cC2.pvalue;
    }

    Number operator+(Number lhs, const Number& rh) {
        double res = lhs.dbl();
        res += rh.pvalue;
        Number resObj(res);
        return resObj;
    }

    Number operator-(Number lhs, const Number& rh) {
        double res = lhs.dbl();
        res -= rh.pvalue;
        Number resObj(res);
        return resObj;
    }

    Number operator*(Number lhs, const Number& rh) {
        double res = lhs.dbl();
        res *= rh.pvalue;
        Number resObj(res);
        return resObj;
    }

    Number operator/(Number lhs, const Number& rh) {
        double res = lhs.dbl();
        res /= rh.pvalue;
        Number resObj(res);
        return resObj;
    }













}
