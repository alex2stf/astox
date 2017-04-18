/*
 * Math.cpp
 *
 *  Created on: Oct 12, 2015
 *      Author: alexstf
 */

#include "astox.h"

namespace astox {

    Math::Math() : Value() {
        // TODO Auto-generated constructor stub
        extension = new Object();
        objectList["E"] = Number::New(2.718281828459045);
        objectList["PI"] = Number::New(3.141592653589793);
        objectList["LN2"] = Number::New(0.6931471805599453); //Natural logarithm of 2, approximately 0.693.
        objectList["LN10"] = Number::New(2.302585092994046); //Natural logarithm of 2, approximately 0.693.
        objectList["LOG2E"] = Number::New(1.4426950408889634); //Natural logarithm of 2, approximately 0.693.
        objectList["LOG10E"] = Number::New(0.4342944819032518); //Natural logarithm of 2, approximately 0.693.
        objectList["SQRT1_2"] = Number::New(0.7071067811865476); //Natural logarithm of 2, approximately 0.693.
        objectList["SQRT2"] = Number::New(1.4142135623730951); //Natural logarithm of 2, approximately 0.693.
        objectList["abs"] = new Function(Math::m_abs);
        objectList["acos"] = new Function(Math::m_acos);
        objectList["acosh"] = new Function(Math::m_acosh);
        objectList["asin"] = new Function(Math::m_asin);
        objectList["asinh"] = new Function(Math::m_asinh);
        objectList["atan"] = new Function(Math::m_atan);
        objectList["atan2"] = new Function(Math::m_atan2);
        objectList["atanh"] = new Function(Math::m_atanh);
        objectList["cbrt"] = new Function(Math::m_cbrt);
        objectList["ceil"] = new Function(Math::m_ceil);
        objectList["clz32"] = new Function(Math::m_clz32);
        objectList["cos"] = new Function(Math::m_cos);
        objectList["cosh"] = new Function(Math::m_cosh);
        objectList["exp"] = new Function(Math::m_exp);

        extension->addKey("E", Number::New(std::exp(1.0)));
        extension->addKey("factorial", new Function(Math::mext_factorial));
        extension->addKey("gcd", new Function(Math::mext_gcd));



        objectList["ext"] = extension;

    }

    Math::~Math() {
        // TODO Auto-generated destructor stub
    }

    astox::Number * Math::getNumber(std::string keyName) {
        std::map<std::string, astox::Value *>::iterator it = objectList.find(keyName);
        if (it == objectList.end()) { // not found
            Error err(Error::RANGE_ERROR, "key: '" + keyName + "' not found", __LINE__, __FILENAME__);
            return Number::NaN;
        }
#ifdef ASTX_VALUE_STATIC_CAST
        return static_cast<Number *> (it->second);
#else
        return dynamic_cast<Number *> (it->second);
#endif
    }

    Value * Math::m_abs(Value * input) {
        using namespace std;
        double val = input->toDouble();
        if (val < 0) {
            return Number::New(val * -1);
        }
        return Number::New(val);
    };

    Value * Math::m_acos(Value * input) {
        using namespace std;
        double r = acos(input->toDouble());
        return Number::New(r);
    };

    Value * Math::m_acosh(Value * input) {
        using namespace std;
#ifdef _MSC_VER
        double x = input->toDouble();
        double r = log(x + sqrt((x * x) - 1));
        //log(x + sqrt(x2 + 1))
#else
        double r = acosh(input->toDouble());
#endif
        return Number::New(r);
    };

    Value * Math::m_asin(Value * input) {
        using namespace std;
        double r = asin(input->toDouble());
        return Number::New(r);
    };

    Value * Math::m_asinh(Value * input) {
        using namespace std;
#ifdef _MSC_VER
        double x = (input->toDouble());
        double r = log(x + sqrt((x * x) + 1));
#else
        double r = asinh(input->toDouble());
#endif
        return Number::New(r);
    };

    Value * Math::m_atan(Value * input) {
        using namespace std;
        double r = atan(input->toDouble());
        return Number::New(r);
    };

    Value * Math::m_atanh(Value * input) {
        using namespace std;
#ifdef _MSC_VER
        double x = (input->toDouble());
        double r = (log(1 + x) - log(1 - x)) / 2;
#else
        double r = atanh(input->toDouble());
#endif
        return Number::New(r);
    };

    Value * Math::m_cbrt(Value * input) {
        using namespace std;
        double r = cbrt(input->toDouble());
        return Number::New(r);
    };

    Value * Math::m_ceil(Value * input) {
        using namespace std;
        double r = ceil(input->toDouble());
        return Number::New(r);
    };

    Value * Math::m_cos(Value * input) {
        using namespace std;
        double r = cos(input->toDouble());
        return Number::New(r);
    };

    Value * Math::m_cosh(Value * input) {
        using namespace std;
        double r = cosh(input->toDouble());
        return Number::New(r);
    };

    Value * Math::m_exp(Value * input) {
        using namespace std;
        double r = exp(input->toDouble());
        return Number::New(r);
    };

    Value * Math::m_clz32(Value * input) {
        using namespace std;
        int x = (input->toInt());
        if (x == 0) return Number::New(32);
        unsigned n = 0;

        if (x <= 0x0000ffff) n += 16, x <<= 16;
        if (x <= 0x00ffffff) n += 8, x <<= 8;
        if (x <= 0x0fffffff) n += 4, x <<= 4;
        if (x <= 0x3fffffff) n += 2, x <<= 2;
        if (x <= 0x7fffffff) n++;

        return Number::New(n);
    };

    Value * Math::m_atan2(Value * input) {
        using namespace std;
        if (input->getType() == ASTOX_VALUE_ARRAY) {
            Array * args = Array::cast(input);
            if (args->length() > 1) {
                double r = atan2(args->itemAtIndex(0)->toDouble(), args->itemAtIndex(1)->toDouble());
                return Number::New(r);
            }
        }
        return Number::NaN;
    };



    //* EXT algorithms: *//

    Value * Math::mext_factorial(Value * input) {
        using namespace std;
        int max = input->toInt();
        if (max <= 0) {
            return Number::New(1);
        }
        int factorial = 1;
        for (int i = 1; i <= max; i++) {
            factorial = factorial * i;
        }
        return Number::New(factorial);
    };

    long Math::mext_gcd(long a, long b) {
        using namespace std;
        if (a < 0) {
            a = -a;
        }
        if (b < 0) {
            b = -b;
        }
        if (b == 0) {
            return a;
        }
        if (a == 0) {
            return b;
        }
        long c = a % b;

        //cout << "c = " << c << "gcd(" << b << ", " << c << ")" << endl;

        return mext_gcd(b, c);
    }

    Value * Math::mext_gcd(Value * input) {
        using namespace std;
        if (input->getType() == ASTOX_VALUE_ARRAY) {
            Array * args = Array::cast(input);
            if (args->length() > 1) {
                long r = mext_gcd(args->itemAtIndex(0)->toDouble(), args->itemAtIndex(1)->toDouble());
                return Number::New(r);
            }
        }
        return Number::NaN;
    };

    std::string Math::toString() {
        return "{}";
    }

    Math * Math::getInstance() {
        return new Math();
    };

} /* namespace astox */
