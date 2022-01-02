/*
 * Error.cpp
 *
 *  Created on: Sep 9, 2013
 *      Author: alexandru
 */

#include "astox.h"


namespace astox {

	using namespace std;

    Error::Error(stxtr msg, const char * source, const char * method, int _line):
    		Object(Syntax::JS) {
		log_stack("err %s", msg.c_str());
    	valueType = ASTOX_VALUE_ERROR;
		src = source;
		mth = method;
		line = _line;
		std::stringstream ss(stringstream::in | stringstream::out);
		ss << _line;
		strmsg.append(msg)
			  .append(" src: ")
			  .append(source)
			  .append(" mth:")
			  .append(method)
			  .append(" lin:")
			  .append(ss.str());


		objmap["message"] = String::New(msg, _lx);
		objmap["source"] = String::New(src, _lx);
		objmap["method"] = String::New(mth, _lx);
		objmap["line"] = String::New(line, _lx);


		
    }

    void Error::throwValue(Value * v){
    	Error * n;
    	if(v->type() == ASTOX_VALUE_ERROR){
			#ifdef ASTX_VALUE_STATIC_CAST
					n = static_cast<Error *> (v);
			#else
					n = dynamic_cast<Error *> (v);
			#endif
		} else {
			#ifdef ASTX_VALUE_STATIC_CAST
					n = static_cast<Error *> ( ecma::methods::Error_constructor(v, Value::npos,  v->lxc() ));
			#else
					n = dynamic_cast<Error *> ( ecma::methods::Error_constructor(v, Value::npos, v->lxc() ));
			#endif
		}
    	throw *n;
    };

    const char* Error::what() const throw () {
        return strmsg.c_str();
    }

    Error::~Error() throw () {
    }

    stxtr Error::str() {
        return strmsg;
    };



} /* namespace astox */
