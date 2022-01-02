#ifndef ASTOX_EXTERNAL_JAVA_NATIVE_INTERFACE_SUPPOPRT
#define ASTOX_EXTERNAL_JAVA_NATIVE_INTERFACE_SUPPOPRT


#include "macros.h"
#include "types.h"
#include <jni.h>


namespace astox {
	class JNIContainer {
	public:
		Function * context();
		Value * eval(std::string input);
		void expose(std::string name, Value * value);
		static JNIContainer * instance();
		void putFunction(Function * function);
		Function * getFunction(stxtr name);
		Function * newFunction(const char * input);
		Value * callFunctionById(stxtr name, Value *& args);
		JNIEnv * env();
		void setEnv(JNIEnv * env);
		jint throwErr(stxtr msg, const char * filename, const char * funcname, int line);
		Value * getMember(const char * serialized);
	private:
		static JNIContainer * _ins;
		Function * _ctx;
		JNIContainer();
		map<stxtr, Function*> funcBuf;
		JNIEnv * _env;
	};

	class JSTXFunction : public Object {
	private:
		JNIEnv *env;
		const char * clazzcaller;
		jobject objcaller;
		stxtr methodname;
	public:
		JSTXFunction(const lexic &l, JNIEnv *env, 
			jobject objcaller,
			const char * clazzname,
			stxtr methodname);
			Value * call(Value * arguments, Function *&context);
			Value * call(std::string &args, const scriptengine::lexic &l, Function *&ctx);
			stxtr str();
			const char * strtype();
	};


	namespace serializers {
		jobject jobject_from_value(JNIEnv *env, Value * v);
		Value * value_from_jobject(JNIEnv *env, jobject j);
	}
}



#define STX_EXPOSED_JVM_CLASS_SIGN "axl/stf/astox/Value"
#define STX_EXPOSED_JVM_ENUM_SIGN "axl/stf/astox/Type"
#define STX_EXPOSED_JVM_ENUM_LSIGN "Laxl/stf/astox/Type;"
#define STX_EXPOSED_JVM_CLASS "axl.stf.astox.Value"
#define STX_EXPOSED_METHOD_SIGN "(Laxl/stf/astox/Value;)Laxl/stf/astox/Value;"
stxtr jvm_get_method_name(JNIEnv *env, jobject method, jclass methodClass);
stxtr jvm_get_method_return_type(JNIEnv *env, jobject method, jclass methodClass);
bool jvm_check_params(JNIEnv *env, jobject method, jclass methodClass);
jobject jvm_get_enum_type(JNIEnv *env, int type);






#ifdef __cplusplus
	extern "C" {
#endif
			
JNIEXPORT jobject JNICALL Java_axl_stf_astox_Value_executeNativeFunction(JNIEnv *env, jobject jobj, jstring id, jobject params);
JNIEXPORT jobject JNICALL Java_axl_stf_astox_Scriptengine_evalNative(JNIEnv *env, jobject jobj, jstring input);
JNIEXPORT jobject JNICALL Java_axl_stf_astox_Scriptengine_infoNative(JNIEnv *env, jobject jobj);
JNIEXPORT void JNICALL Java_axl_stf_astox_Scriptengine_exposeNative(JNIEnv *env, jobject jobj, jstring name, jobject inst, jobjectArray methods, jstring clazzname);
jint JNI_OnLoad(JavaVM *vm, void *reserved);

#ifdef __cplusplus
	}
#endif



#endif
