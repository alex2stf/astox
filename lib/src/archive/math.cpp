/*
 * Math.cpp
 *
 *  Created on: Oct 12, 2015
 *      Author: alexstf
 */

#include "astox.h"

namespace astox {
	using namespace std;

    Math::Math() : Value(scriptengine::Syntax::JS) {
        // TODO Auto-generated constructor stub
        extension = Object::New(_lx);
        objmap["E"] = Number::New(2.718281828459045, _lx);
        objmap["PI"] = Number::New(3.141592653589793, _lx);
        objmap["LN2"] = Number::New(0.6931471805599453, _lx); //Natural logarithm of 2, approximately 0.693.
        objmap["LN10"] = Number::New(2.302585092994046, _lx); //Natural logarithm of 2, approximately 0.693.
        objmap["LOG2E"] = Number::New(1.4426950408889634, _lx); //Natural logarithm of 2, approximately 0.693.
        objmap["LOG10E"] = Number::New(0.4342944819032518, _lx); //Natural logarithm of 2, approximately 0.693.
        objmap["SQRT1_2"] = Number::New(0.7071067811865476, _lx); //Natural logarithm of 2, approximately 0.693.
        objmap["SQRT2"] = Number::New(1.4142135623730951, _lx); //Natural logarithm of 2, approximately 0.693.
//        objmap["abs"] = new Function(Math::m_abs);
//        objmap["acos"] = new Function(Math::m_acos);
//        objmap["acosh"] = new Function(Math::m_acosh);
//        objmap["asin"] = new Function(Math::m_asin);
//        objmap["asinh"] = new Function(Math::m_asinh);
//        objmap["atan"] = new Function(Math::m_atan);
//        objmap["atan2"] = new Function(Math::m_atan2);
//        objmap["atanh"] = new Function(Math::m_atanh);
//        objmap["cbrt"] = new Function(Math::m_cbrt);
//        objmap["ceil"] = new Function(Math::m_ceil);
//        objmap["clz32"] = new Function(Math::m_clz32);
//        objmap["cos"] = new Function(Math::m_cos);
//        objmap["cosh"] = new Function(Math::m_cosh);
//        objmap["exp"] = new Function(Math::m_exp);
//
//        extension->addKey("E", Number::New(std::exp(1.0)));
//        extension->addKey("factorial", new Function(Math::mext_factorial));
//        extension->addKey("gcd", new Function(Math::mext_gcd));



        objmap["ext"] = extension;

    }

    Math::~Math() {
        // TODO Auto-generated destructor stub
    }

    astox::Number * Math::getNumber(std::string keyName) {
        std::map<std::string, astox::Value *>::iterator it = objmap.find(keyName);
        if (it == objmap.end()) { // not found
        	return Number::NaN;
        }
#ifdef ASTX_VALUE_STATIC_CAST
        return static_cast<Number *> (it->second);
#else
        return dynamic_cast<Number *> (it->second);
#endif
    }

    Value * Math::m_abs(Value * z) {
        double y = z->dbl();
        if (y < 0) {
            return Number::New(y * -1, z->lxc());
        }
        return Number::New(y, z->lxc());
    };

    Value * Math::m_acos(Value * z) {
        return Number::New(acos(z->dbl()), z->lxc());
    };

    Value * Math::m_acosh(Value * z) {
#ifdef _MSC_VER
        double x = z->dbl();
        double r = log(x + sqrt((x * x) - 1));
#else
        double r = acosh(z->dbl());
#endif
        return Number::New(r, z->lxc());
    };

    Value * Math::m_asin(Value * z) {
        return Number::New(asin(z->dbl()), z->lxc());
    };

    Value * Math::m_asinh(Value * input) {
#ifdef _MSC_VER
        double x = (input->dbl());
        double r = log(x + sqrt((x * x) + 1));
#else
        double r = asinh(input->dbl());
#endif
        return Number::New(r, input->lxc());
    };

    Value * Math::m_atan(Value * z) {
        return Number::New(atan(z->dbl()), z->lxc());
    };

    Value * Math::m_atanh(Value * input) {
        using namespace std;
#ifdef _MSC_VER
        double x = (input->dbl());
        double r = (log(1 + x) - log(1 - x)) / 2;
#else
        double r = atanh(input->dbl());
#endif
        return Number::New(r, input->lxc());
    };

    Value * Math::m_cbrt(Value * input) {
        double r = cbrt(input->dbl());
        return Number::New(r, input->lxc());
    };

    Value * Math::m_ceil(Value * input) {
        using namespace std;
        double r = ceil(input->dbl());
        return Number::New(r, input->lxc());
    };

    Value * Math::m_cos(Value * input) {
        using namespace std;
        double r = cos(input->dbl());
        return Number::New(r, input->lxc());
    };

    Value * Math::m_cosh(Value * input) {
        using namespace std;
        double r = cosh(input->dbl());
        return Number::New(r, input->lxc());
    };

    Value * Math::m_exp(Value * input) {
        using namespace std;
        double r = exp(input->dbl());
        return Number::New(r, input->lxc());
    };

    Value * Math::m_clz32(Value * input) {
        int x = (input->toInt());
        if (x == 0) return Number::New(32, input->lxc());
        unsigned n = 0;

        if (x <= 0x0000ffff) n += 16, x <<= 16;
        if (x <= 0x00ffffff) n += 8, x <<= 8;
        if (x <= 0x0fffffff) n += 4, x <<= 4;
        if (x <= 0x3fffffff) n += 2, x <<= 2;
        if (x <= 0x7fffffff) n++;

        return Number::New(n, input->lxc());
    };

    Value * Math::m_atan2(Value * input) {
        using namespace std;
        if (input->type() == ASTOX_VALUE_ARRAY) {
            Array * args = Array::cast(input);
            if (args->length() > 1) {
                double r = atan2(args->at(0)->dbl(), args->at(1)->dbl());
                return Number::New(r, input->lxc());
            }
        }
        return Number::NaN;
    };



    //* EXT algorithms: *//

    Value * Math::mext_factorial(Value * in) {

        int max = in->toInt();
        if (max <= 0) {
            return Number::New(1, in->lxc());
        }
        int factorial = 1;
        for (int i = 1; i <= max; i++) {
            factorial = factorial * i;
        }
        return Number::New(factorial, in->lxc());
    };

    long Math::mext_gcd(long a, long b) {

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

    Value * Math::mext_gcd(Value * z) {
        if (z->type() == ASTOX_VALUE_ARRAY) {
            Array * args = Array::cast(z);
            if (args->length() > 1) {
                long r = mext_gcd(args->at(0)->dbl(), args->at(1)->dbl());
                return Number::New((double)r, z->lxc());
            }
            return Number::NaN;
        }
        return Number::NaN;
    };

    std::string Math::str() {
        return "{}";
    }

    Math * Math::getInstance() {
        return new Math();
    };

} /* namespace astox */
