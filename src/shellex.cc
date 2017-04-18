/*
 * shllex.cc
 *
 *  Created on: Jan 8, 2015
 *      Author: alexandru
 */



#include <v8.h>
#include <node.h>
#include "astox/ExtApi.h"

using namespace std;

using namespace astox;
using namespace astox::stringAlg;


void ssh2_connect(const v8::FunctionCallbackInfo<v8::Value>& args) {
	 v8::Isolate* isolate = args.GetIsolate();

	 args.GetReturnValue().Set(v8::String::NewFromUtf8(isolate, "world"));
}


void init(v8::Local<v8::Object> exports) {
	 NODE_SET_METHOD(exports, "hello", ssh2_connect);
}

NODE_MODULE(shellex, init)
