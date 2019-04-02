#include <astox.h>

#ifdef ASTOX_COMPILER_MSC

#ifdef STX_MODE_DLL
//BOOL WINAPI DllMain(HINSTANCE hinstDLL, DWORD fwdReason, LPVOID lpvReserved) {
	//	printf("dll main\n");
		//return astox::JNIContainer::instance() != NULL;
//}

BOOL APIENTRY DllMain( HMODULE hModule,
                       DWORD  ul_reason_for_call,
                       LPVOID lpReserved
					 )
{
	switch (ul_reason_for_call)
	{
	case DLL_PROCESS_ATTACH:
	case DLL_THREAD_ATTACH:
	case DLL_THREAD_DETACH:
	case DLL_PROCESS_DETACH:
		break;
	}
	return astox::JNIContainer::instance() != NULL;
}

#endif
#endif

namespace astox {
	JNIContainer * JNIContainer::_ins = NULL;

	Function * JNIContainer::context() {
		return _ctx;
	};

	JNIContainer::JNIContainer() {
		lexic l = Syntax::JS;
		_ctx = Function::buildStandardContext(l);
	}
	Value * JNIContainer::callFunctionById(stxtr name, Value *& args) {
		return getFunction(name)->call(args, _ctx);
	};

	Function * JNIContainer::newFunction(const char * input) {
		return scriptengine::build_new_function(input, _ctx, _ctx->lxc()).func;
	};

	Value * JNIContainer::eval(std::string input) {
		lexic l = Syntax::JS;
		return scriptengine::eval(input, l, _ctx);
	};

	void JNIContainer::expose(std::string name, Value * value) {
		_ctx->addKey(name, value);
	};

	void JNIContainer::putFunction(Function * f) {
		funcBuf[f->id()] = f;
	};

	Function * JNIContainer::getFunction(stxtr name) {
		if (funcBuf.find(name) != funcBuf.end()) {
			return funcBuf[name];
		}
		return NULL;
	};

	JNIEnv * JNIContainer::env() {
		return _env;
	};


    JavaVM *globalJVM;
    volatile bool attached = false;
    jint JNI_OnLoad(JavaVM *vm, void *reserved) {
        // Keep a reference on the Java VM.
        log_stack("JNI LOADED");
        globalJVM = vm;
        return JNI_VERSION_1_4;
    }
    jclass valueClazz = NULL;
    jclass enumClz = NULL;

	void JNIContainer::setEnv(JNIEnv * env) {


        valueClazz = env->FindClass(STX_EXPOSED_JVM_CLASS_SIGN);
        enumClz = env->FindClass(STX_EXPOSED_JVM_ENUM_SIGN);

        log_stack("define value clazz");

        if(globalJVM == NULL){
            env->GetJavaVM(&globalJVM);
        }
        if(globalJVM != NULL && !attached){

            JavaVMAttachArgs vmAttachArgs;
            vmAttachArgs.version = JNI_VERSION_1_4;
            vmAttachArgs.name = "astox";
            vmAttachArgs.group = NULL;
            globalJVM->AttachCurrentThread((void **)&env, &vmAttachArgs);
            attached = true;
        }

		_env = env;
	};

	jint JNIContainer::throwErr(stxtr msg, const char * filename, const char * funcname, int line) {
		if (_env != NULL) {
			stxtr smsg = "";
			jclass exClass;
			//char * className = ;
			std::stringstream ss(stringstream::in | stringstream::out);
			ss << line;


			smsg.append(msg)
				.append("\nfname: ")
				.append(filename)
				.append("\nfuncs: ")
				.append(funcname)
				.append("\n line: ")
				.append(ss.str());

			exClass = _env->FindClass("axl/stf/astox/ScriptException");
			jmethodID constructor = _env->GetMethodID(exClass, "<init>", "(Ljava/lang/String;Ljava/lang/String;Ljava/lang/String;I)V");
			jstring jmsg = _env->NewStringUTF(msg.c_str());
			jstring jfilename = _env->NewStringUTF(filename);
			jstring jfuncname = _env->NewStringUTF(funcname);
			jint jline = line;
			jthrowable err = (jthrowable) _env->NewObject(exClass, constructor, jmsg, jfilename, jfuncname, jline);
			return _env->Throw(err);
		}
		else {
			astox::Error n(msg, filename, funcname, line);
			throw n;
		}
		return 0;
	};

	JNIContainer * JNIContainer::instance() {
		if (_ins == NULL) {
			_ins = new JNIContainer();
		}
		return _ins;
	};

    Value * JNIContainer::getMember(const char * serialized){
        lexic l = Syntax::JS;
        return scriptengine::get_member(serialized, _ctx, l, NULL, false);
    }

	JSTXFunction::JSTXFunction(const lexic &l, JNIEnv * e, jobject o, const char * c, stxtr m) : Object(l) {
		valueType = ASTOX_VALUE_FUNCTION;
		env = e;
		clazzcaller = c;
		objcaller = o;
		methodname = m;
	};

	Value * JSTXFunction::call(std::string &args, const scriptengine::lexic &l, Function *&ctx) {
		Value * argument = execute_operation_command(args, l, ctx);
		return call(argument, ctx);
	};


	Value * JSTXFunction::call(Value * arguments, Function *&context) {
		log_stack("argument%s = %s", arguments->strtype(), blurt(arguments).c_str());
		log_stack("calling (%s)   %s with %s\n", methodname.c_str(), STX_EXPOSED_METHOD_SIGN, blurt(arguments).c_str());
		jclass clz = env->FindClass(clazzcaller);
		jmethodID methodid = env->GetMethodID(clz, methodname.c_str(), STX_EXPOSED_METHOD_SIGN);
		jobject param = serializers::jobject_from_value(env, arguments);
		jobject rv = (jobject)env->CallObjectMethod(objcaller, methodid, param);
		return serializers::value_from_jobject(env, rv);
	};


	stxtr JSTXFunction::str() {
		return "jstxfunction";
	};

	const char * JSTXFunction::strtype() {
		return "jstxfunction";
	};


	namespace serializers {
		jobject jobject_from_value(JNIEnv *env, Value * v) {
			log_stack("init %s", v->strtype());
			jclass vclass = env->FindClass(STX_EXPOSED_JVM_CLASS_SIGN);
			jmethodID constructor; 
			jobject inst;

			if (v->isString()) {
				log_stack("init call str");
				constructor = env->GetMethodID(vclass, "<init>", "(Ljava/lang/String;)V");
				inst = env->NewObject(vclass, constructor, env->NewStringUTF(v->str().c_str()));
				log_stack("instance str done");
			}
			else if (v->isNumber()) {
				log_stack("init call number");
				constructor = env->GetMethodID(vclass, "<init>", "(D)V");
				inst = env->NewObject(vclass, constructor, v->dbl());
				log_stack("instance num done");
				return inst;
			}

			else if (v->isBoolean()) {
				log_stack("init call boolean");
				constructor = env->GetMethodID(vclass, "<init>", "(Z)V");
				inst = env->NewObject(vclass, constructor, v->bol());
				log_stack("instance bol done");
			}

			else if (v->isFunction()) {
				log_stack("init call func");
				stxtr id = v->id();
				JNIContainer::instance()->putFunction(Function::cast(v));
				constructor = env->GetMethodID(vclass, "<init>", "(Ljava/lang/String;Laxl/stf/astox/Type;)V");
				jstring name = env->NewStringUTF(id.c_str());
				inst = env->NewObject(vclass, constructor, name, jvm_get_enum_type(env, ASTOX_VALUE_FUNCTION));
				env->DeleteLocalRef(name);
			}

			else if (v->isArray()) {
				Array * arr = Array::cast(v);
				log_stack("is array");
				constructor = env->GetMethodID(vclass, "<init>", "(Ljava/lang/String;Laxl/stf/astox/Type;)V");
				jstring jstr = env->NewStringUTF("");
				inst = env->NewObject(vclass, constructor, jstr, jvm_get_enum_type(env, ASTOX_VALUE_ARRAY));
				log_stack("constructor and instance done");
				for (int i = 0; i < arr->length(); i++) {
				jmethodID push = env->GetMethodID(vclass, "push", "(Laxl/stf/astox/Value;)Laxl/stf/astox/Value;");
					jobject j1 = jobject_from_value(env, Value::duplicate(arr->at(i)));
					env->CallObjectMethod(inst, push, j1);
				}
				log_stack("instance array done");
			}

			else if (v->isObject()) {
				log_stack("init call object");
				Object * obj = Object::cast(v);
				constructor = env->GetMethodID(vclass, "<init>", "(Ljava/lang/String;Laxl/stf/astox/Type;)V");
				jstring jstr = env->NewStringUTF("dumi");
				inst = env->NewObject(vclass, constructor, jstr, jvm_get_enum_type(env, ASTOX_VALUE_OBJECT));

				object_map * m = obj->getObjectList();
				if (m != NULL && !m->empty()) {
					for (object_iterator it = m->begin(); it != m->end(); ++it) {
						jmethodID push = env->GetMethodID(vclass, "put", "(Ljava/lang/String;Laxl/stf/astox/Value;)Laxl/stf/astox/Value;");
						jstring name = env->NewStringUTF(it->first.c_str());
						jobject jv = jobject_from_value(env, it->second);
						env->CallObjectMethod(inst, push, name, jv);
						env->DeleteLocalRef(name);
					}
				}
			}

			
			else {
				log_stack("init call default");
				constructor = env->GetMethodID(vclass, "<init>", "()V");
				inst = env->NewObject(vclass, constructor);
			}

			return inst;
		};



		Value * value_from_jobject(JNIEnv *env, jobject j) {
			if (j == NULL) {
				return Value::npos;
			}
			lexic lxc = Syntax::JS;

			jmethodID strtype = env->GetMethodID(valueClazz, "strtype", "()Ljava/lang/String;");
			jstring type = (jstring)env->CallObjectMethod(j, strtype);
			const char * nstr = env->GetStringUTFChars(type, 0);
			log_stack("jtype %s", nstr);
			if (strcmp(nstr, "STRING") == 0) {
				jmethodID mid = env->GetMethodID(valueClazz, "str", "()Ljava/lang/String;");
				jstring val = (jstring)env->CallObjectMethod(j, mid);
				const char *c = env->GetStringUTFChars(val, 0);
				String * s = String::New(c, lxc);
				log_stack("jtype %s %s", nstr, blurt(s).c_str());
				env->ReleaseStringUTFChars(val, c);
				return s;
			}

			if (strcmp(nstr, "NUMBER") == 0) {
				jmethodID mid = env->GetMethodID(valueClazz, "dbl", "()D");
				jdouble val = env->CallDoubleMethod(j, mid);
				return Number::New(val, lxc);
			}

			if (strcmp(nstr, "BOOLEAN") == 0) {
				jmethodID mid = env->GetMethodID(valueClazz, "bol", "()Z");
				jboolean val = env->CallBooleanMethod(j, mid);
				return Boolean::New(val == JNI_TRUE, lxc);
			}

			if (strcmp(nstr, "FUNCTION") == 0) {
				jmethodID mid = env->GetMethodID(valueClazz, "id", "()Ljava/lang/String;");
				jstring val = (jstring)env->CallObjectMethod(j, mid);
				const char *c = env->GetStringUTFChars(val, 0);
				return JNIContainer::instance()->newFunction(c);
			}

            if (strcmp(nstr, "UNDEFINED") == 0) {
                return Value::npos;
            }

			if (strcmp(nstr, "OBJECT") == 0 || strcmp(nstr, "ARRAY") == 0) {
                jmethodID mid = env->GetMethodID(valueClazz, "serialized", "()Ljava/lang/String;");
                jstring val = (jstring)env->CallObjectMethod(j, mid);
                const char *c = env->GetStringUTFChars(val, 0);
                return JNIContainer::instance()->getMember(c);
            }

			return Value::npos;
		};
	}
}


using namespace astox;

JNIEXPORT jobject JNICALL Java_axl_stf_astox_Value_executeNativeFunction(JNIEnv *env, jobject jobj, jstring id, jobject params) {
	JNIContainer::instance()->setEnv(env);
	const char * i = env->GetStringUTFChars(id, 0);
	Value * a = serializers::value_from_jobject(env, params);
	Value * rv = JNIContainer::instance()->callFunctionById(i, a);
	log_stack("ret = %s", blurt(rv).c_str());
	env->ReleaseStringUTFChars(id, i);
	return serializers::jobject_from_value(env, rv);
};

JNIEXPORT jobject JNICALL Java_axl_stf_astox_Scriptengine_evalNative(JNIEnv *env, jobject jobj, jstring jstr) {
	JNIContainer::instance()->setEnv(env);
	const char * nstr = env->GetStringUTFChars(jstr, 0);
	log_stack("eval input %s", nstr);
	Value * retval = NULL;
	try {
		retval = JNIContainer::instance()->eval(nstr);
		log_stack("ret eval input %s", blurt(retval).c_str());
	}
	catch (Error &n) {
		log_stack("%s", n.what());
        JNIContainer::instance()->throwErr(n.what(), __FILENAME__, __FUNCTION__, __LINE__);
	}
    catch (...){
        JNIContainer::instance()->throwErr("unexpected exception", __FILENAME__, __FUNCTION__, __LINE__);
    }
	if (retval != NULL) {
		return serializers::jobject_from_value(env, retval);
	}
	return NULL;
};



stxtr jvm_get_method_name(JNIEnv *env, jobject method, jclass methodClass) {
	
	jmethodID getName = env->GetMethodID(methodClass, "getName", "()Ljava/lang/String;");
	jstring methodName = (jstring)env->CallObjectMethod(method, getName);
	const char * nstr = env->GetStringUTFChars(methodName, 0);
	stxtr s(nstr);
	env->ReleaseStringUTFChars(methodName, nstr);
	return s;
};

jobject jvm_get_enum_type(JNIEnv *env, int type) {

	jfieldID fid;
	switch (type) {
	case ASTOX_VALUE_FUNCTION:
		fid = env->GetStaticFieldID(enumClz, "FUNCTION", STX_EXPOSED_JVM_ENUM_LSIGN);
		break;
	case ASTOX_VALUE_ARRAY:
		fid = env->GetStaticFieldID(enumClz, "ARRAY", STX_EXPOSED_JVM_ENUM_LSIGN);
		break;
	case ASTOX_VALUE_OBJECT:
		fid = env->GetStaticFieldID(enumClz, "OBJECT", STX_EXPOSED_JVM_ENUM_LSIGN);
		break;
	}
	
	return env->GetStaticObjectField(enumClz, fid);
};

stxtr jvm_get_method_return_type(JNIEnv *env, jobject method, jclass methodClazz) {
	jmethodID getGenericReturnType = env->GetMethodID(methodClazz, "getReturnType", "()Ljava/lang/Class;");
	jobject returnType = (jobject)env->CallObjectMethod(method, getGenericReturnType);
	jclass typeClass = env->FindClass("java/lang/Class");
	jmethodID getTypeName = env->GetMethodID(typeClass, "getName", "()Ljava/lang/String;");
	jstring jrtn = (jstring)env->CallObjectMethod(returnType, getTypeName);
	const char * rtn = env->GetStringUTFChars(jrtn, 0);
	stxtr s(rtn);
	env->ReleaseStringUTFChars(jrtn, rtn);
	return s;
};


bool jvm_check_params(JNIEnv *env, jobject method, jclass methodClazz) {
	jmethodID gp = env->GetMethodID(methodClazz, "getParameters", "()[Ljava/lang/reflect/Parameter;");
	jobjectArray  pr = (jobjectArray)env->CallObjectMethod(method, gp);
	int pc = (int)env->GetArrayLength(pr);

	if (pc != 1) {
		pc = 3;
		return false;
	}

	jobject param = (jobject)env->GetObjectArrayElement(pr, 0);
	jclass parameterClass = env->FindClass("java/lang/reflect/Parameter");
	jmethodID getType = env->GetMethodID(parameterClass, "getType", "()Ljava/lang/Class;");
	jobject pT = (jobject)env->CallObjectMethod(param, getType);

	jclass classClass = env->FindClass("java/lang/Class");
	jmethodID cGN = env->GetMethodID(classClass, "getName", "()Ljava/lang/String;");
	jstring ctn = (jstring)env->CallObjectMethod(pT, cGN);
	const char * ptn = env->GetStringUTFChars(ctn, 0);
	stxtr s(ptn);
	if (strcmp(ptn, STX_EXPOSED_JVM_CLASS) == 0) {
		env->ReleaseStringUTFChars(ctn, ptn);
		return true;
	}
	env->ReleaseStringUTFChars(ctn, ptn);
	return false;
};

JNIEXPORT void JNICALL Java_axl_stf_astox_Scriptengine_exposeNative(JNIEnv *env,
		jobject jobj, jstring name, jobject inst,
		jobjectArray methods, jstring clazzname) {
	JNIContainer::instance()->setEnv(env);
	jclass clazz = env->GetObjectClass(inst);
	if (clazz == NULL) {
		int z = 2; //leave this shit for clang
		return;
	}


	int len = env->GetArrayLength(methods);
	lexic lxc = Syntax::JS;

	Object * obj = new Object(lxc);
	jclass methodClazz = env->FindClass("java/lang/reflect/Method");
	for (int i = 0; i < len; i++) {
		jobject method = env->GetObjectArrayElement(methods, i);
//		stxtr ret_type = jvm_get_method_return_type(env, method, methodClazz);
//		if (ret_type == STX_EXPOSED_JVM_CLASS) {
			stxtr method_name = jvm_get_method_name(env, method, methodClazz);
			//if (jvm_check_params(env, method, methodClazz)) {
				const char * czn = env->GetStringUTFChars(clazzname, 0);
				JSTXFunction * jf = new JSTXFunction(lxc, env, env->NewGlobalRef(inst), czn, method_name);
				obj->addKey(method_name, jf);
			//}
//		}
//		else {
//			log_stack("ignore method returning %s, signature %s required\n", ret_type.c_str(), STX_EXPOSED_METHOD_SIGN);
//			continue;
//		}
	}
	const char * ptn = env->GetStringUTFChars(name, 0);
	JNIContainer::instance()->expose(ptn, obj);
	env->ReleaseStringUTFChars(name, ptn);
	return;
};


JNIEXPORT jobject JNICALL Java_axl_stf_astox_Scriptengine_infoNative(JNIEnv *env, jobject jobj){
	JNIContainer::instance()->setEnv(env);
    Value * v = ecma::methods::astox_info(NULL, Syntax::JS);
	return serializers::jobject_from_value(env, v);
};
